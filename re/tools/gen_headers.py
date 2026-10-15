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
import json
import os
import re
import struct
import sys
from collections import defaultdict

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

    # demangled class name -> ident, for resolving base includes
    ident_of = {m["class"]: m["ident"] for m in manifest}
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
    owned = set()
    owned_path = os.path.join(outdir, "handwritten_types.json")
    if os.path.exists(owned_path):
        owned = set(json.load(open(owned_path)))

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
        if m["ctors"]:
            L.append(f"// constructor  : 0x{m['ctors'][0]:08x}")
        L.append("//")
        L.append("// Virtual slots are in real vtable order; each carries its target")
        L.append("// address as evidence. Slots named vfunc_N have no established")
        L.append("// semantics yet.")
        L.append("// " + "=" * 74)
        L.append("")
        L.append('#include "ts_prelude.h"')
        for b in bases:
            if b not in prelude_types:
                L.append(f'#include "{b}.h"')
        L.append("")

        if bases:
            spelled = [cpp_name.get(b, b) for b in bases]
            L.append(f"class {ident} : public " + ", public ".join(spelled))
        else:
            L.append(f"class {ident}")
        L.append("{")
        L.append("public:")
        L.append(f"    // ---- vtable 0x{m['vtable']:08x} ({m['nvirtuals']} slots) ----")
        for i, e in enumerate(m["entries"]):
            fn = fns.get(e)
            tag = ""
            if fn and fn.get("strings"):
                s = fn["strings"][0]["text"]
                if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]{2,30}", s):
                    tag = f'  // refs "{s}"'
            L.append(f"    virtual void vfunc_{i}();  // [{i}] 0x{e:08x}{tag}")
        L.append("")
        if m["ctors"]:
            L.append(f"    {ident}();  // 0x{m['ctors'][0]:08x}")
        L.append(f"    virtual ~{ident}();")

        # ---- data members recovered from the constructor ----
        md = members.get(m["class"])
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
                if kind == "vtable":
                    L.append(f"    void* vptr_{off:03x};  // vtable 0x{mem['value']:08x}")
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
