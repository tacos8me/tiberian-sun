#!/usr/bin/env python3
"""
Emit a compilable C++ header for every class in the manifest.

Design:
  - Inheritance uses ONLY direct bases (out/direct_bases.json), because RTTI
    base arrays list transitive bases which C++ must not repeat.
  - Each header #includes its direct base headers, so types are complete.
  - Classes with no recovered base derive from nothing.
  - Virtual slots are emitted in real vtable order with their target VA.
  - The constructor VA is recorded; member offsets are recovered from the
    constructor's store sequence where possible.

Output is deterministic and regenerable.
"""
import glob
import json
import os
import re
import struct
import sys
from collections import defaultdict

_KEYWORDS = {"class", "struct", "const", "unsigned", "signed", "int",
             "char", "short", "long", "void", "bool", "float", "double"}

IB = 0x400000


class Bin:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        import pefile
        pe = pefile.PE(path, fast_load=True)
        self.secs = []
        for s in pe.sections:
            self.secs.append((IB + s.VirtualAddress,
                              IB + s.VirtualAddress + s.Misc_VirtualSize,
                              s.PointerToRawData, s.SizeOfRawData,
                              s.Name.decode(errors="replace").strip("\x00")))
        self.text = next(s for s in self.secs if s[4] == ".text")

    def rd(self, va, n):
        for vs, ve, raw, rs, nm in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                return self.data[o:o + n]
        return None

    def u32(self, va):
        b = self.rd(va, 4)
        return struct.unpack("<I", b)[0] if b and len(b) == 4 else None

    def cstr(self, va, n=256):
        b = self.rd(va, n)
        if not b:
            return None
        z = b.find(b"\x00")
        if z >= 0:
            b = b[:z]
        try:
            return b.decode("ascii")
        except UnicodeDecodeError:
            return b.decode("latin-1")

    def in_text(self, va):
        t = self.text
        return t[0] <= va < t[1]


def sanitize(name):
    n = re.sub(r"^(class|struct)\s+", "", name)
    n = n.replace("::", "_")
    n = re.sub(r"[<>,\s\*&]+", "_", n)
    n = re.sub(r"_+", "_", n).strip("_")
    return n


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    srcdir = sys.argv[3] if len(sys.argv) > 3 else "src/include"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    direct = json.load(open(os.path.join(outdir, "direct_bases.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    members_path = os.path.join(outdir, "members.json")
    members = json.load(open(members_path)) if os.path.exists(members_path) else {}
    mn_path = os.path.join(outdir, "member_names.json")
    member_names = json.load(open(mn_path)) if os.path.exists(mn_path) else {}
    vn_path = os.path.join(outdir, "virtual_names.json")
    # Fail loudly rather than silently defaulting to {}. The silent default is
    # exactly what hid the fact that no pipeline stage produced this file: a
    # clean regeneration emitted vfunc_N for all 1,213 recoverable names and
    # nothing complained.
    if not os.path.exists(vn_path):
        sys.stderr.write(
            "ERROR: %s is missing.\n"
            "       It is produced by tools/name_virtuals.py (pipeline stage "
            "8b).\n"
            "       Refusing to emit headers with no virtual names.\n" % vn_path)
        sys.exit(1)
    virtual_names = json.load(open(vn_path))
    sg_path = os.path.join(outdir, "signatures.json")
    signatures = json.load(open(sg_path)) if os.path.exists(sg_path) else {}

    # demangled class name -> ident, for resolving base includes
    ident_of = {m["class"]: m["ident"] for m in manifest}
    # ident -> authoritative RTTI name. The RTTI name is the correct C++
    # spelling of the type, so it is used verbatim for base clauses instead of
    # trying to un-mangle the ident.
    rtti_by_ident = {m["ident"]: m["class"] for m in manifest}
    # Nested types: "class INIClass::INISection" -> INISection qualifies as
    # INIClass::INISection. MSVC's RTTI drops the qualification inside template
    # arguments, so it has to be restored when spelling a base.
    nested = {}
    for m in manifest:
        c = m["class"]
        for kw in ("class ", "struct "):
            if c.startswith(kw):
                c = c[len(kw):]
        if "::" in c and "<" not in c:
            nested[c.split("::")[-1]] = c
    for m in manifest:
        ident_of.setdefault(sanitize(m["class"]), m["ident"])

    # Types supplied by ts_prelude.h are valid bases but have no header of
    # their own; map them so inheritance resolves instead of being dropped.
    prelude_types = {
        "IUnknown", "IPersist", "IPersistStream", "ISequentialStream", "IStream",
        "ILinkStream", "ISwizzle", "IApplication", "IGameMap", "IFlyControl",
        "IClassFactory", "IConnectionPoint", "IConnectionPointContainer",
        "IEnumConnectionPoints", "IEnumConnections", "IRTTITypeInfo",
        "IPiggyback", "ILocomotion", "IHouse", "IPublicHouse",
        "BounceClass", "DiskSwap", "FlasherClass", "StageClass",
        "CDownloadEventSink", "CNetUtilEventSink",
        "WOL_IChatEvent", "WOL_IDownloadEvent", "WOL_INetUtilEvent",
        "ATL_CComObjectRootBase",
        "ATL_CComObjectRootEx_class_CComMultiThreadModel_TL",
    }
    for t in prelude_types:
        ident_of.setdefault(t, t)

    # Sanitized ident -> real C++ spelling for namespaced prelude types.
    cpp_name = {
        "WOL_IChatEvent": "WOL::IChatEvent",
        "WOL_IDownloadEvent": "WOL::IDownloadEvent",
        "WOL_INetUtilEvent": "WOL::INetUtilEvent",
        "ATL_CComObjectRootBase": "ATL::CComObjectRootBase",
        "ATL_CComObjectRootEx_class_CComMultiThreadModel_TL":
            "ATL::CComObjectRootEx<ATL::CComMultiThreadModel, void>",
    }

    # Classes already defined by hand-written headers must not be regenerated,
    # or the two definitions collide. Hand-written headers are richer (named
    # virtuals, member offsets), so they win.
    # Derive ownership from src/handwritten/ directly, so the pipeline works
    # from a clean checkout without depending on a stale out/ artifact.
    owned = set()
    hw_dir = os.path.join(os.path.dirname(srcdir.rstrip("/")), "handwritten")
    if not os.path.isdir(hw_dir):
        hw_dir = "src/handwritten"
    if os.path.isdir(hw_dir):
        for hp in glob.glob(os.path.join(hw_dir, "*.h")):
            hs = open(hp).read()
            for hm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    hs, re.M):
                owned.add(hm.group(1))
    # Fall back to the recorded artifact if the directory is absent.
    if not owned:
        owned_path = os.path.join(outdir, "handwritten_types.json")
        if os.path.exists(owned_path):
            owned = set(json.load(open(owned_path)))

    # Template name -> the header that actually defines it. Emitting
    # #include "<template>.h" is wrong when the template lives in a
    # differently-named file (DynamicVectorClass is in VectorClass.h, Node is
    # in INIClass.h); the include must name the defining file.
    template_header = {
        "VectorClass": "VectorClass.h",
        "DynamicVectorClass": "VectorClass.h",
        "TypeList": "VectorClass.h",
        "Node": "INIClass.h",
    }
    if os.path.isdir(hw_dir):
        for hp in glob.glob(os.path.join(hw_dir, "*.h")):
            hs = open(hp).read()
            for hm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    hs, re.M):
                template_header.setdefault(hm.group(1),
                                           os.path.basename(hp))
    # Templates whose formal parameters have no evidence-backed concrete type.
    # MSVC leaks the formal parameter name into RTTI for instantiations that
    # occur inside the template body, so e.g. "VectorClass<H>" has no real H.
    # Such an argument must be forward-declared, not spelled as a template
    # argument of an unknown type.
    for hp in glob.glob(os.path.join(hw_dir, "*.h")) if os.path.isdir(hw_dir) else []:
        hs = open(hp).read()
        for hm in re.finditer(
                r"^(?:template\s*<([^>]*)>)\s*(?:class|struct)\s+(\w+)", hs, re.M):
            for _fp in re.findall(r"[A-Za-z_]\w*", hm.group(1)):
                template_header.setdefault(_fp, None)

    os.makedirs(srcdir, exist_ok=True)
    written = 0
    skipped = 0
    unresolved_bases = defaultdict(set)

    for m in manifest:
        ident = m["ident"]
        if not ident:
            continue
        if ident in owned:
            skipped += 1
            continue
        # A template instantiation whose template is defined by a hand-written
        # header is already defined by that header; skip the generated
        # duplicate. This is decided from the RTTI name, not the mangled ident.
        _rtti = m.get("class", "")
        if "<" in _rtti:
            _tmpl = _rtti.split("<")[0]
            for _kw in ("class ", "struct "):
                if _tmpl.startswith(_kw):
                    _tmpl = _tmpl[len(_kw):]
            if _tmpl.strip() in owned:
                skipped += 1
                continue

        # direct bases only, resolved to idents, deduplicated
        bases = []
        seen_bases = set()
        for b in direct.get(m["class"], []):
            bi = ident_of.get(b) or ident_of.get(sanitize(b))
            if bi and bi != ident and bi not in seen_bases:
                seen_bases.add(bi)
                bases.append(bi)
            elif not bi:
                unresolved_bases[m["class"]].add(b)

        L = []
        L.append("#pragma once")
        L.append("// " + "=" * 74)
        L.append(f"// {ident}.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)")
        L.append("//")
        L.append(f"// RTTI class   : {m['class']}")
        L.append(f"// vtable       : 0x{m['vtable']:08x}  ({m['nvirtuals']} slots)")
        if m.get("ctors"):
            L.append(f"// constructor  : 0x{m['ctors'][0]:08x}")
        if m.get("ctor_sites"):
            L.append(f"// vtable store : 0x{m['ctor_sites'][0]:08x}  (inside the ctor)")
        L.append("//")
        L.append("// Virtual slots are in real vtable order; each carries its target")
        L.append("// address as evidence. Slots named vfunc_N have no established")
        L.append("// semantics yet.")
        L.append("// " + "=" * 74)
        L.append("")
        L.append('#include "ts_prelude.h"')
        for b in bases:
            if b in prelude_types:
                continue
            if os.path.exists(os.path.join(srcdir, b + ".h")):
                L.append(f'#include "{b}.h"')
                continue
            # A base that is an instantiation of a hand-written template
            # (Node<...>, VectorClass<...>, BlitPlain<...>): include the
            # template's header, since no header exists for the instantiation.
            _bt = rtti_by_ident.get(b, "")
            if "<" in _bt:
                _t = _bt.split("<")[0]
                for _kw in ("class ", "struct "):
                    if _t.startswith(_kw):
                        _t = _t[len(_kw):]
                _t = _t.strip()
                _hdr = template_header.get(_t)
                if _hdr:
                    L.append(f'#include "{_hdr}"')
                # The base's template ARGUMENTS must also be complete types:
                # DynamicVectorClass<ObjectClass*> needs ObjectClass.h. Include
                # the header of every argument that has one.
                for _at in re.findall(r"[A-Za-z_]\w*", _bt[_bt.index("<"):]):
                    if _at in _KEYWORDS:
                        continue
                    if os.path.exists(os.path.join(srcdir, _at + ".h")):
                        L.append(f'#include "{_at}.h"')
                    elif template_header.get(_at):
                        L.append(f'#include "{template_header[_at]}"')
        # A template instantiation needs its template arguments declared.
        # Derive them from the RTTI name, e.g.
        #   class DynamicVectorClass<AbstractClass*>  ->  AbstractClass.h
        rtti = m.get("class", "")
        if "<" in rtti:
            tail = rtti[rtti.index("<"):]
            seen = set()
            for tok in re.findall(r"[A-Za-z_]\w*", tail):
                if tok in _KEYWORDS or tok in seen:
                    continue
                seen.add(tok)
                if os.path.exists(os.path.join(srcdir, tok + ".h")):
                    L.append(f'#include "{tok}.h"')
                elif template_header.get(tok):
                    # The argument is itself defined by a hand-written template
                    # header (e.g. an instantiation nested in another template).
                    L.append(f'#include "{template_header[tok]}"')
                elif tok not in owned and tok not in prelude_types:
                    # The argument has no header of its own (MSVC emits the
                    # formal parameter name for instantiations inside a
                    # template, e.g. DynamicVectorClass<D>). Forward-declare it
                    # so the template-id is well-formed.
                    L.append(f"class {tok};")
        L.append("")

        if bases:
            # Base clauses may mention a template argument that is only a leaked
            # MSVC formal parameter (CounterClass : public VectorClass<H>). Such
            # an argument has no evidence-backed type, so forward-declare it
            # rather than inventing one; this keeps the template-id well-formed
            # while preserving the recovered inheritance edge.
            _declared = set()
            for _b in bases:
                _bt = rtti_by_ident.get(_b, "")
                if "<" not in _bt:
                    continue
                for _tok in re.findall(r"[A-Za-z_]\w*", _bt[_bt.index("<"):]):
                    if _tok in _KEYWORDS or _tok in _declared:
                        continue
                    _declared.add(_tok)
                    if os.path.exists(os.path.join(srcdir, _tok + ".h")):
                        continue
                    if _tok in owned or _tok in prelude_types:
                        continue
                    if template_header.get(_tok):
                        continue
                    L.append(f"class {_tok};  // RTTI formal param, no type evidence")
            spelled = []
            for b in bases:
                if b in cpp_name:
                    spelled.append(cpp_name[b])
                    continue
                # Use the authoritative RTTI spelling of the base. MSVC drops
                # the qualification of nested types inside template arguments,
                # so restore it (Node<INISection*, INIClass> must be written
                # Node<INIClass::INISection, INIClass>).
                r = rtti_by_ident.get(b)
                if not r:
                    spelled.append(b)
                    continue
                for _kw in ("class ", "struct "):
                    if r.startswith(_kw):
                        r = r[len(_kw):]
                r = r.strip()
                for _simple, _qual in nested.items():
                    r = re.sub(r"\b" + re.escape(_simple) + r"\b", _qual, r)
                spelled.append(r)
            L.append(f"class {ident} : public " + ", public ".join(spelled))
        else:
            L.append(f"class {ident}")
        L.append("{")
        L.append("public:")
        L.append(f"    // ---- vtable 0x{m['vtable']:08x} ({m['nvirtuals']} slots) ----")
        vnames = virtual_names.get(m["class"]) or virtual_names.get(m["ident"]) or {}
        for i, e in enumerate(m["entries"]):
            fn = fns.get(e)
            tag = ""
            if fn and fn.get("strings"):
                s = fn["strings"][0]["text"]
                if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]{2,30}", s):
                    tag = f'  // refs "{s}"'
            vn = vnames.get(str(i))
            sg = signatures.get(hex(e), {})
            cc = sg.get("cc")
            na = sg.get("nargs")
            # build an honest signature: only state what was recovered
            if na is not None and na > 0:
                args = ", ".join(f"int a{k}" for k in range(na))
            elif na == 0:
                args = ""
            else:
                args = ""
            # Return type is recorded as a comment, not in the declaration:
            # the heuristic can disagree with a base class and break overrides.
            ret = "void"
            bits = []
            if cc:
                bits.append(cc)
            if sg.get("ret"):
                bits.append(f"returns {sg['ret']}")
            cctag = ("  // " + ", ".join(bits)) if bits else ""
            if vn and vn.startswith("~"):
                # The destructor is emitted explicitly below; skip it here to
                # avoid a duplicate declaration.
                continue
            elif vn:
                L.append(f"    virtual {ret} {vn}({args});  // [{i}] 0x{e:08x}{cctag}{tag}")
            else:
                L.append(f"    virtual {ret} vfunc_{i}({args});  // [{i}] 0x{e:08x}{cctag}{tag}")
        L.append("")
        if m["ctors"]:
            L.append(f"    {ident}();  // 0x{m['ctors'][0]:08x}")
        L.append(f"    virtual ~{ident}();")

        # ---- data members recovered from the constructor ----
        md = members.get(m["class"])
        _mn = member_names.get(m["class"]) or member_names.get(m["ident"]) or {}
        named = _mn.get("members", {})
        # A key can map to several offsets (repeated INI reads). Only the first
        # use of a name is emitted; later ones fall back to m_XXX.
        _used_names = set()
        if md and md.get("members"):
            L.append("")
            L.append(f"    // ---- data members (offsets from ctor 0x{md['ctor']:08x}) ----")
            L.append("    // Offsets are exact; names are not yet recovered, so members")
            L.append("    // are emitted as byte blobs at their real displacements.")
            prev_end = 0
            for mem in md["members"]:
                off = mem["off"]
                if off < prev_end:
                    continue
                if off > prev_end:
                    L.append(f"    unsigned char pad_{prev_end:03x}[0x{off - prev_end:x}];")
                kind = mem["kind"]
                nm = named.get(str(off), {}).get("name")
                if kind == "vtable":
                    L.append(f"    void* vptr_{off:03x};  // vtable 0x{mem['value']:08x}")
                    prev_end = off + 4
                elif nm and nm not in _used_names:
                    _used_names.add(nm)
                    L.append(f"    unsigned long {nm};  // +0x{off:03x} (INI key)")
                    prev_end = off + 4
                elif kind == "zero":
                    L.append(f"    unsigned long m_{off:03x};  // zero-initialised")
                    prev_end = off + 4
                elif kind == "imm":
                    L.append(f"    unsigned long m_{off:03x};  // init 0x{mem['value']:x}")
                    prev_end = off + 4
                else:
                    L.append(f"    unsigned long m_{off:03x};  // /* ? */")
                    prev_end = off + 4
        L.append("};")
        L.append("")

        with open(os.path.join(srcdir, ident + ".h"), "w") as f:
            f.write("\n".join(L))
        written += 1

    print(f"headers written: {written}")
    print(f"skipped (hand-written owns): {skipped}")
    if unresolved_bases:
        print(f"classes with unresolved bases: {len(unresolved_bases)}")
        for k, v in list(unresolved_bases.items())[:10]:
            print(f"  {k}: {sorted(v)}")


if __name__ == "__main__":
    main()
