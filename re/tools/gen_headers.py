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
import bisect
import glob
import json
import os
import re
import struct
import sys
from collections import defaultdict

import capstone

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


class StoreWidths:
    """Access-width evidence for recovered member offsets, read from the ctor.

    gen_members.py records WHERE a constructor stores (the displacement and the
    kind of value) but not HOW WIDE the store is, so this emitter previously
    declared every member as a 4-byte `unsigned long`. That is wrong for the
    offsets the constructor writes with a byte store (`mov byte ptr [eax+0xc],
    1`), and it silently swallowed every recovered offset that fell inside the
    4-byte span of such a declaration.

    This re-reads the constructor's machine code -- the same evidence the
    offset table came from -- and records the width of the store at each
    displacement. Only stores through the register that receives the class's
    vtable pointer (the `this` register) count, so a store through a reused
    scratch register into some other object is not attributed to a member.
    """

    # Matches gen_members.py / verify_members_binary.py: the vtable write must
    # be early in the function and go through a non-stack register, and the
    # function body is read as one blob from its recovered entry point.
    MAX_FN = 0x800
    VTABLE_WRITE_WINDOW = 200

    def __init__(self, bin_, fn_starts):
        self.bin = bin_
        self.starts = fn_starts
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.md.detail = True
        self._insns = {}
        self._maps = {}

    def _fn_insns(self, site):
        """Instructions of the recovered function enclosing `site`."""
        i = bisect.bisect_right(self.starts, site) - 1
        if i < 0:
            return []
        fn = self.starts[i]
        if fn not in self._insns:
            blob = self.bin.rd(fn, self.MAX_FN)
            self._insns[fn] = list(self.md.disasm(blob, fn)) if blob else []
        return self._insns[fn]

    def width_map(self, vtva, ctor_site):
        """offset -> (store width in bytes, the store instruction as text).

        Where the constructor stores more than one width to one displacement
        the WIDEST store is kept: a field cannot be narrower than a store into
        it, and declaring it narrower would move the members that follow.
        """
        key = (vtva, ctor_site)
        if key in self._maps:
            return self._maps[key]
        insns = self._fn_insns(ctor_site) if ctor_site else []
        this_reg = None
        for pos, ins in enumerate(insns):
            if pos > self.VTABLE_WRITE_WINDOW:
                break
            if ins.mnemonic != "mov" or len(ins.operands) != 2:
                continue
            dst, src = ins.operands
            if (dst.type == capstone.x86.X86_OP_MEM
                    and src.type == capstone.x86.X86_OP_IMM
                    and src.imm == vtva):
                base = ins.reg_name(dst.mem.base)
                if base not in ("esp", "ebp"):
                    this_reg = base
                    break
        wmap = {}
        if this_reg:
            for ins in insns:
                if ins.mnemonic not in ("mov", "movzx", "movsx", "and", "or"):
                    continue
                if len(ins.operands) != 2:
                    continue
                dst = ins.operands[0]
                if dst.type != capstone.x86.X86_OP_MEM:
                    continue
                if ins.reg_name(dst.mem.base) != this_reg:
                    continue
                w = dst.size
                if w not in (1, 2, 4):
                    continue
                off = dst.mem.disp
                cur = wmap.get(off)
                if cur is None or w > cur[0]:
                    wmap[off] = (w, f"{ins.mnemonic} {ins.op_str}")
        self._maps[key] = wmap
        return wmap


# Declared type for each store width the binary uses. `unsigned long` is the
# widest form; nothing here is inferred beyond the store instruction itself.
_WIDTH_TYPE = {1: "unsigned char", 2: "unsigned short", 4: "unsigned long"}


def member_width(widths, off):
    """(declared type, size in bytes, evidence annotation) for member `off`.

    The declared type follows the store width the constructor uses at that
    displacement. A record whose width could not be determined keeps
    `unsigned long` and is annotated `/* width ? */` rather than guessed at.
    """
    rec = widths.get(off)
    if rec:
        w, insn = rec
        typ = _WIDTH_TYPE.get(w)
        if typ:
            return typ, w, f"{w}-byte store: {insn}"
    return "unsigned long", 4, "/* width ? */"


def member_block(rtti_class, members, member_names, widths):
    """Emit the recovered data-member declarations for one class.

    The offset table (out/members.json) is keyed by the RTTI class spelling.
    Offsets are exact; names are only used where an INI key or string supplies
    evidence. Each member is declared at the access width the constructor
    actually stores it with (`widths`, from StoreWidths) and the store
    instruction is quoted in the comment as evidence. The block is returned as
    a list of lines so the same code can either place it inside a class body
    being generated here, or append it to the header that already defines the
    class (nested types and template instantiations have no standalone header
    of their own).

    Returns [] when no offsets were recovered for the class.
    """
    md = members.get(rtti_class)
    if not (md and md.get("members")):
        return []
    _mn = member_names.get(rtti_class) or {}
    named = _mn.get("members", {})
    # A key can map to several offsets (repeated INI reads). Only the first
    # use of a name is emitted; later ones fall back to m_XXX.
    _used_names = set()
    L = []
    L.append("")
    L.append(f"    // ---- data members (offsets from ctor 0x{md['ctor']:08x}) ----")
    L.append("    // Offsets are exact. Each member is declared at the access width")
    L.append("    // the constructor stores it with; the store instruction is quoted")
    L.append("    // as evidence. `/* width ? */` means no store could be classified.")
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
            typ, size, ann = member_width(widths, off)
            L.append(f"    {typ} {nm};  // +0x{off:03x} (INI key); {ann}")
            prev_end = off + size
        elif kind == "zero":
            typ, size, ann = member_width(widths, off)
            L.append(f"    {typ} m_{off:03x};  // +0x{off:03x} zero-initialised; {ann}")
            prev_end = off + size
        elif kind == "imm":
            typ, size, ann = member_width(widths, off)
            L.append(f"    {typ} m_{off:03x};  // +0x{off:03x} init "
                     f"0x{mem['value']:x}; {ann}")
            prev_end = off + size
        else:
            typ, size, ann = member_width(widths, off)
            L.append(f"    {typ} m_{off:03x};  // +0x{off:03x} /* ? */; {ann}")
            prev_end = off + size
    return L


def class_body_span(src, class_name):
    """Locate the body of `class_name` in a header.

    Returns (body_start, body_end) as character indices into `src`, where
    body_end is the index of the '}' that closes the class. Returns None when
    the class is not defined in the file. Matching is anchored to a definition
    line (`class NAME {`, `class NAME : public ... {`, or the same with a
    leading `template <...>`), so a forward declaration (`class NAME;`) or a
    mention in a comment does not match.
    """
    pat = re.compile(
        r"^[ \t]*(?:template\s*<[^;{}]*>\s*)?(?:class|struct)\s+"
        + re.escape(class_name) + r"\b[^;{}]*\{", re.M)
    m = pat.search(src)
    if not m:
        return None
    i = src.index("{", m.start())
    depth = 0
    j = i
    while j < len(src):
        if src[j] == "{":
            depth += 1
        elif src[j] == "}":
            depth -= 1
            if depth == 0:
                return i, j
        j += 1
    return None


def insert_member_block(src, class_name, block):
    """Replace or insert the machine-generated member block for `class_name`.

    Returns the new source, or None when the class body cannot be found.
    Appending to the owning header (rather than emitting a nested class body)
    keeps every header compiling standalone, which verify.py check 1 requires.

    Any block written by a PREVIOUS run is removed first. Without this a rerun
    accumulates: when the recovery improves and no longer produces an offset,
    the stale declaration stays behind and verify.py check 7 correctly reports
    it as an emitted offset with no corresponding recovered entry.
    """
    span = class_body_span(src, class_name)
    if not span:
        return None
    start, end = span
    body = src[start:end]
    banner = "// ---- data members (offsets from ctor"
    if banner in body:
        # Excise the previous machine block: from its banner to the closing
        # brace, keeping any curated text that preceded it.
        cut = body.index(banner)
        body = body[:cut].rstrip() + "\n"
        src = src[:start] + body + src[end:]
        span = class_body_span(src, class_name)
        if not span:
            return None
        _, end = span
    # Back up over the trailing whitespace/newline before '}' so the inserted
    # block lands on its own lines and the closing brace stays put.
    at = end
    while at > 0 and src[at - 1] in " \t\r\n":
        at -= 1
    return src[:at] + "\n" + "\n".join(block) + "\n" + src[at:]




def shared_slot0_map(manifest):
    """slot-0 addresses that more than one class points at.

    A vtable slot 0 shared between classes is a stub, not a destructor: 66 of
    the 749 classes point at 0x405790. Emitting `virtual ~X()` for each of them
    claims a slot position the destructor does not hold, which is what the
    headers did until this map existed.
    """
    counts = {}
    for e in manifest:
        ents = e.get("entries") or []
        if ents:
            counts[ents[0]] = counts.get(ents[0], 0) + 1
    return {va: n for va, n in counts.items() if n > 1}


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    srcdir = sys.argv[3] if len(sys.argv) > 3 else "src/include"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    shared_slot0 = shared_slot0_map(manifest)
    direct = json.load(open(os.path.join(outdir, "direct_bases.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    members_path = os.path.join(outdir, "members.json")
    members = json.load(open(members_path)) if os.path.exists(members_path) else {}
    mn_path = os.path.join(outdir, "member_names.json")
    member_names = json.load(open(mn_path)) if os.path.exists(mn_path) else {}
    # Access-width evidence: re-read each constructor's machine code to learn
    # how wide the store at each recovered displacement is, so a member is
    # declared at its real width instead of always as a 4-byte `unsigned long`.
    # See StoreWidths.
    store_widths = StoreWidths(bin_, sorted(fns))
    widths_by_class = {}
    for _m in manifest:
        _entry = members.get(_m["class"])
        if _entry:
            widths_by_class[_m["class"]] = store_widths.width_map(
                _m["vtable"], _entry.get("ctor"))
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
        for hp in sorted(glob.glob(os.path.join(hw_dir, "*.h"))):
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

    # Types that ts_prelude.h defines are owned by the prelude, exactly as if a
    # hand-written header declared them. Without this, a class that lives only
    # in the prelude (GScreenClass) gets its own generated header and the
    # combined TU fails with a redefinition.
    #
    # The prelude cannot simply be read here: pipeline stage 0 wipes
    # src/include/ and restores only src/handwritten/, while ts_prelude.h is
    # produced later by stage 10 (reconcile_prelude.py). So the set is derived
    # from the two authoritative sources instead -- the hand-written prelude if
    # present, plus reconcile_prelude.py's PRELUDE_ONLY, which is the list that
    # will actually define these types.
    for cand in (os.path.join(srcdir, "ts_prelude.h"),
                 os.path.join(hw_dir, "ts_prelude.h")):
        if os.path.exists(cand):
            ps = open(cand).read()
            for pm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    ps, re.M):
                owned.add(pm.group(1))
            break
    rp = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                      "reconcile_prelude.py")
    if os.path.exists(rp):
        rs = open(rp).read()
        m = re.search(r"PRELUDE_ONLY\s*=\s*\{(.*?)\n\}", rs, re.S)
        if m:
            for pm in re.finditer(r'"([A-Za-z_]\w*)"\s*:', m.group(1)):
                owned.add(pm.group(1))

    # Identifiers the prelude will define; used to keep them out of the
    # deferred-member list, since the prelude is regenerated later.
    #
    # prelude_owned must be initialised HERE, not further down: it is appended
    # to from inside the per-class loop below, and leaving the assignment later
    # raised `UnboundLocalError: prelude_owned` whenever gen_headers.py was run
    # with an already-reconciled src/include/ts_prelude.h present (i.e. outside
    # pipeline stage order, which is exactly how a human re-runs it).
    prelude_owned = []
    prelude_owned_types = set()
    for cand in (os.path.join(srcdir, "ts_prelude.h"),
                 os.path.join(hw_dir, "ts_prelude.h")):
        if os.path.exists(cand):
            for pm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    open(cand).read(), re.M):
                prelude_owned_types.add(pm.group(1))
            break
    if os.path.exists(rp):
        _rs = open(rp).read()
        _m = re.search(r"PRELUDE_ONLY\s*=\s*\{(.*?)\n\}", _rs, re.S)
        if _m:
            for pm in re.finditer(r'"([A-Za-z_]\w*)"\s*:', _m.group(1)):
                prelude_owned_types.add(pm.group(1))

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
        for hp in sorted(glob.glob(os.path.join(hw_dir, "*.h"))):
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
    for hp in (sorted(glob.glob(os.path.join(hw_dir, "*.h"))) if os.path.isdir(hw_dir) else []):
        hs = open(hp).read()
        for hm in re.finditer(
                r"^(?:template\s*<([^>]*)>)\s*(?:class|struct)\s+(\w+)", hs, re.M):
            for _fp in re.findall(r"[A-Za-z_]\w*", hm.group(1)):
                template_header.setdefault(_fp, None)


    # RTTI class name -> the header file that DEFINES it. Nested classes and
    # template instantiations have no standalone generated header: their
    # recovered member offsets still have to be emitted, so the block is
    # appended to the header that owns the definition instead of being dropped.
    # Ownership is derived from the definitions that actually exist in the tree
    # (hand-written headers are copied into srcdir by the pipeline before this
    # stage runs), so this works from a clean checkout.
    # Header basename -> the class names it DEFINES (not forward-declares).
    # Hand-written headers are copied verbatim into srcdir by the pipeline, so a
    # block appended to a hand-written header's srcdir copy is discarded on the
    # next run. The authoritative file for a hand-written header is the one in
    # src/handwritten/, and that is where such a block has to be written for
    # the change to survive regeneration.
    owner_header = {}
    hw_names = set()
    if os.path.isdir(hw_dir):
        hw_names = {os.path.basename(p) for p in glob.glob(os.path.join(hw_dir, "*.h"))}
    # Only a real definition counts. A forward declaration (`class X;`) must not
    # claim ownership: BrainClass.h says `class NeuronClass;` but never defines
    # it, and TechnoClass.h -- not LocomotionClass.h -- defines TechnoClass.
    _def_re = re.compile(
        r"^[ \t]*(?:template\s*<[^;{}]*>\s*)?(?:class|struct)\s+(\w+)"
        r"(?:\s*:[^;{}]*)?\s*\{", re.M)
    for _hp in sorted(glob.glob(os.path.join(srcdir, "*.h"))):
        _base = os.path.basename(_hp)
        _hs = open(_hp).read()
        for _hm in _def_re.finditer(_hs):
            owner_header.setdefault(_hm.group(1), _base)

    def owning_header(rtti_name):
        """(writable path, class name as the header spells it) or None.

        The owner is the header that DEFINES the class: for a template
        instantiation that is the header defining its template
        (BlitPlain<E> -> Blitter.h); for a nested class the header defining its
        enclosing class (SidebarClass::StripClass -> SidebarClass.h). Returns
        None when no header defines the class anywhere.

        The path is redirected into src/handwritten/ when the owning header is
        hand-written, because the pipeline restores that directory over
        src/include/ after this stage runs.
        """
        n = rtti_name
        for _kw in ("class ", "struct "):
            if n.startswith(_kw):
                n = n[len(_kw):]
        n = n.strip()
        base = None
        if n in owner_header:
            base = owner_header[n]
        elif "<" in n and n.split("<")[0].strip() in owner_header:
            n = n.split("<")[0].strip()
            base = owner_header[n]
        elif "::" in n:
            # Prefer the full nested spelling, then the enclosing class, then
            # the simple nested name (the header may write either form).
            if n in owner_header:
                base = owner_header[n]
            else:
                for _cand in (n.split("::")[0].strip(), n.split("::")[-1].strip()):
                    if _cand in owner_header:
                        base = owner_header[_cand]
                        n = _cand
                        break
        if base is None:
            _ident = ident_of.get(rtti_name)
            if _ident and os.path.exists(os.path.join(srcdir, _ident + ".h")):
                base = _ident + ".h"
                n = _ident
        if base is None:
            return None
        if base in hw_names:
            return os.path.join(hw_dir, base), n
        return os.path.join(srcdir, base), n

    # Appended after the per-class loop: (header basename, rtti class, lines).
    # A class the loop skips never enters the file-writing branch, so its
    # recovered offsets would otherwise be computed and thrown away.
    deferred_members = []
    os.makedirs(srcdir, exist_ok=True)
    written = 0
    skipped = 0
    written_idents = set()
    unresolved_bases = defaultdict(set)
    for m in manifest:
        ident = m["ident"]
        if not ident:
            continue
        if ident in owned:
            skipped += 1
            # Hand-written headers own this class. They already document their
            # members, so only append when the header does NOT yet emit the
            # recovered offsets -- otherwise a class whose hand-written header
            # is silent about layout would lose its evidence a second time.
            #
            # Types owned by ts_prelude.h are the exception: the prelude is
            # regenerated by reconcile_prelude.py AFTER this stage, so a block
            # appended to it would be discarded, and a standalone header would
            # define the type twice. Report and skip them.
            if ident in prelude_owned_types:
                prelude_owned.append(m["class"])
                continue
            deferred_members.append(("handwritten", m, ident))
            continue
        if ident in written_idents:
            # Two manifest entries can share one ident (a template head plus a
            # leaked-formal-parameter instantiation). The file already exists;
            # append its offsets there instead of overwriting.
            deferred_members.append(("dup", m, ident))
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
                deferred_members.append(("tmpl", m, ident))
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
                deferred_members.append(("tmpl", m, ident))
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
                # A destructor named in slot 0 is normally the destructor, and
                # the declaration below covers it, so the slot is skipped.
                #
                # But slot 0 is a SHARED STUB for 66 of the 749 classes -- they
                # all point at 0x405790 -- and a shared address is not 66
                # destructors. Skipping it there silently threw the slot away
                # and left `virtual ~X()` claiming a vtable position it does
                # not hold. Emit the slot under its own name instead.
                if e not in shared_slot0:
                    continue
            elif vn:
                L.append(f"    virtual {ret} {vn}({args});  // [{i}] 0x{e:08x}{cctag}{tag}")
            else:
                L.append(f"    virtual {ret} vfunc_{i}({args});  // [{i}] 0x{e:08x}{cctag}{tag}")
        L.append("")
        if m["ctors"]:
            L.append(f"    {ident}();  // 0x{m['ctors'][0]:08x}")
        # The destructor's address is recorded in the manifest's ctor/entry
        # data, not as a slot, so it is declared without claiming one. Where
        # slot 0 is a shared stub, say so, because `virtual ~X()` on its own
        # reads as "this is slot 0" and here that is not true.
        if m["entries"] and m["entries"][0] in shared_slot0:
            L.append("    // The destructor is declared non-virtual here: slot 0 "
                     "holds a stub")
            L.append("    // shared with %d other classes (0x%08x), not the "
                     "destructor." %
                     (shared_slot0[m["entries"][0]], m["entries"][0]))
            L.append(f"    ~{ident}();")
        else:
            L.append(f"    virtual ~{ident}();")

        # ---- data members recovered from the constructor ----
        L.extend(member_block(m["class"], members, member_names,
                                     widths_by_class.get(m["class"]) or {}))
        L.append("};")
        L.append("")

        with open(os.path.join(srcdir, ident + ".h"), "w") as f:
            f.write("\n".join(L))
        written += 1
        written_idents.add(ident)
    # ---- append member blocks for classes with no standalone header ----
    #
    # The per-class loop above only reaches the member emission for classes it
    # writes a file for. Nested classes and template instantiations have no
    # such file: the offsets recovered for them were computed and discarded.
    # Append each block to the header that DEFINES the class instead. A class
    # with no owning header anywhere gets a new one, so nothing is dropped
    # silently.
    append_failed = []
    appended_classes = 0
    appended_records = 0
    skipped_already = 0
    # prelude_owned is initialised earlier (it is appended to above); do not
    # re-bind it here, or those appends are discarded.
    orphan_headers = {}
    seen_blocks = set()
    # Work on a per-file in-memory copy so several classes owned by the same
    # header are appended in one deterministic pass instead of racing writes.
    file_cache = {}
    for _why, m, ident in deferred_members:
        hdr = owning_header(m["class"])
        if hdr is None:
            # Before creating a new file, check whether some header ALREADY
            # defines this class (the prelude, or a header that declares
            # several related types). Creating a standalone header for a type
            # that is already defined elsewhere makes the combined TU fail
            # with a redefinition -- which is exactly what happened to
            # GScreenClass, whose definition lives in ts_prelude.h.
            simple_n = m["class"]
            for _kw in ("class ", "struct "):
                if simple_n.startswith(_kw):
                    simple_n = simple_n[len(_kw):]
            simple_n = simple_n.strip()
            if simple_n in prelude_owned_types:
                prelude_owned.append(m["class"])
                continue
            _defined_elsewhere = False
            for _hp in sorted(glob.glob(os.path.join(srcdir, "*.h"))):
                _hs = open(_hp).read()
                if re.search(r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+"
                             + re.escape(simple_n) + r"\b", _hs, re.M):
                    _defined_elsewhere = True
                    break
            if _defined_elsewhere:
                skipped_already += 1
                continue
            # No header defines this class anywhere. Emit a standalone header
            # for it and say so in a comment, rather than dropping the offsets.
            # The header is written immediately so a second manifest entry for
            # the same class appends to it instead of failing the existence
            # check below.
            key = ident + ".h"
            block = member_block(m["class"], members, member_names,
                                     widths_by_class.get(m["class"]) or {})
            if not block:
                continue
            if key not in orphan_headers:
                simple = m["class"]
                for _kw in ("class ", "struct "):
                    if simple.startswith(_kw):
                        simple = simple[len(_kw):]
                simple = simple.strip()
                lines = [
                    "#pragma once",
                    "// " + "=" * 74,
                    f"// {key}  —  recovered member offsets with no owning header",
                    "//",
                    f"// RTTI class : {m['class']}",
                    "//",
                    "// No reconstructed header defines this class: it has no",
                    "// standalone generated file, and no enclosing-class or",
                    "// template header could be found for it. The ctor-store",
                    "// offsets recovered for it are emitted here so the evidence",
                    "// is not dropped again. See gen_headers.py owning_header().",
                    "// " + "=" * 74,
                    "",
                    f"class {simple}",
                    "{",
                    "public:",
                ]
                lines.extend(block)
                lines.append("};")
                lines.append("")
                open(os.path.join(srcdir, key), "w").write("\n".join(lines))
                orphan_headers[key] = simple
                owner_header[simple] = key
                owner_header[ident] = key
                appended_classes += 1
                appended_records += len(block)
            continue
        path, simple = hdr
        if (path, simple) in seen_blocks:
            continue
        seen_blocks.add((path, simple))
        if os.path.basename(path) == "ts_prelude.h":
            # The prelude is regenerated by reconcile_prelude.py and its
            # PRELUDE_ONLY types (GScreenClass, IUnknown) are redefined there
            # unconditionally, so a block appended to the prelude would be
            # discarded and a separate header would define the type twice.
            # Report it instead of dropping it silently.
            prelude_owned.append(m["class"])
            continue
        if not os.path.exists(path):
            append_failed.append(f"{m['class']} -> {path} (missing)")
            continue
        src = file_cache.get(path)
        if src is None:
            src = open(path).read()
        # Idempotent and non-destructive: a body that already documents its
        # members -- the curated hand-written blocks in VectorClass.h and
        # Pipe.h, or a block appended by an earlier run -- keeps its
        # authoritative layout. Appending a second, raw block would give the
        # class two disagreeing layout claims.
        span = class_body_span(src, simple)
        if span is None:
            append_failed.append(f"{m['class']} -> {os.path.basename(path)} "
                                 f"(no class {simple} in file)")
            continue
        body = src[span[0]:span[1]]
        # NOTE: do NOT skip when a banner is already present. A block written
        # by a previous run must be REPLACED, not kept: when the recovery
        # improves and stops producing an offset, the stale declaration would
        # otherwise survive and verify.py check 7 would (correctly) report an
        # emitted offset with no recovered entry. insert_member_block excises
        # the old block first, so re-running is idempotent.
        block = member_block(m["class"], members, member_names,
                                     widths_by_class.get(m["class"]) or {})
        if not block:
            continue
        # A hand-written header may already declare some of these members BY
        # NAME (AnimTypeClass declares "bool AltPalette;" from an INI key)
        # without carrying a data-members banner. Re-emitting the same
        # identifier is a redeclaration error, so drop any block line whose
        # declared name is already present in the class body.
        #
        # CRITICAL: only CURATED declarations count. A block appended by a
        # previous run is machine-generated, and counting its own members as
        # "already declared" makes the next run drop them and leave only a
        # comment. That is a two-cycle which silently DELETED ~593 declaration
        # lines across 43 headers (AircraftTypeClass lost +0x000/+0x37c/+0x435)
        # and flipped check 7 between 2032/237 and 1716/241. So truncate the
        # body at the machine banner before collecting names.
        curated_body = body
        _banner = "// ---- data members (offsets from ctor"
        if _banner in curated_body:
            curated_body = curated_body[:curated_body.index(_banner)]
        existing = set()
        for line in curated_body.splitlines():
            mm = re.match(r"\s*(?:virtual\s+)?[\w:<>,\s\*&]+?\b(\w+)\s*(?:\[[^\]]*\])?\s*;", line)
            if mm and not line.lstrip().startswith("//"):
                existing.add(mm.group(1))
        if existing:
            kept = []
            dropped = 0
            for line in block:
                dm = re.search(r"\b(\w+)\s*(?:\[[^\]]*\])?\s*;", line)
                if dm and dm.group(1) in existing:
                    dropped += 1
                    continue
                kept.append(line)
            if dropped:
                kept.append(f"    // {dropped} member(s) already declared above; "
                            f"their offsets are recorded in out/members.json")
            block = kept
            if not block:
                skipped_already += 1
                continue
        new = insert_member_block(src, simple, block)
        if new is None or new == src:
            continue
        file_cache[path] = new
        appended_classes += 1
        appended_records += len(block)
    for path, src in sorted(file_cache.items()):
        open(path, "w").write(src)
    # Orphan headers are written as they are created (see above), so nothing to
    # flush here; only their names are reported.
    print(f"member blocks appended to owning headers: {appended_classes} "
          f"classes")
    if prelude_owned:
        print(f"member blocks owned by ts_prelude.h (not appended): "
              f"{len(prelude_owned)} ({', '.join(sorted(prelude_owned))})")
        print(f"member blocks skipped (already documented): {skipped_already} "
              f"classes")
    if orphan_headers:
        print(f"new headers for classes with no owner: {len(orphan_headers)} "
              f"({', '.join(sorted(orphan_headers))})")
    if append_failed:
        print(f"member blocks NOT appended: {len(append_failed)}")
        for e in append_failed[:10]:
            print(f"  {e}")

    print(f"headers written: {written}")
    print(f"skipped (hand-written owns): {skipped}")
    if unresolved_bases:
        print(f"classes with unresolved bases: {len(unresolved_bases)}")
        for k, v in list(unresolved_bases.items())[:10]:
            print(f"  {k}: {sorted(v)}")


if __name__ == "__main__":
    main()
