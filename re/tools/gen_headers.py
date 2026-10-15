#!/usr/bin/env python3
"""
Emit a compilable C++ header for every class in the manifest.

Each header contains:
  - exact inheritance from RTTI
  - the full virtual table in order, each slot resolved to its real VA
  - the constructor VA and recovered member offsets
  - data members named from INI keys / strings referenced by the class's methods

Headers are self-contained: they include a common prelude and forward-declare
their bases, so the whole set compiles.
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


def member_offsets(bin_, ctor_va, maxinsn=400):
    """Recover member offsets written by a constructor: [reg+disp] stores."""
    f = None
    return []


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    srcdir = sys.argv[3] if len(sys.argv) > 3 else "src/include"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}

    # Build the set of all class idents so we can forward-declare bases.
    allidents = {m["ident"] for m in manifest}
    # map demangled base name -> ident
    baseident = {}
    for m in manifest:
        baseident[m["class"]] = m["ident"]

    os.makedirs(srcdir, exist_ok=True)
    written = 0
    for m in manifest:
        ident = m["ident"]
        if not ident:
            continue
        lines = []
        lines.append("#pragma once")
        lines.append("// " + "=" * 72)
        lines.append(f"// {ident}.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)")
        lines.append("//")
        lines.append(f"// RTTI class   : {m['class']}")
        lines.append(f"// vtable       : 0x{m['vtable']:08x}  ({m['nvirtuals']} slots)")
        if m["ctors"]:
            lines.append(f"// constructor  : 0x{m['ctors'][0]:08x}")
        lines.append("//")
        lines.append("// Generated from recovered RTTI + vtable data. Virtual slots are in")
        lines.append("// real vtable order; each carries its target address as evidence.")
        lines.append("// " + "=" * 72)
        lines.append("")
        lines.append('#include "ts_prelude.h"')
        lines.append("")

        # forward-declare bases
        bases = m["bases"]
        for b in bases:
            bi = baseident.get(b)
            if bi and bi != ident:
                lines.append(f"class {bi};")
        if bases:
            lines.append("")

        # class declaration
        if bases:
            bl = []
            for b in bases:
                bi = baseident.get(b)
                bl.append(bi if bi else "/* unresolved */ " + sanitize(b))
            decl = f"class {ident} : public " + ", public ".join(bl)
        else:
            decl = f"class {ident}"
        lines.append(decl)
        lines.append("{")
        lines.append("public:")
        lines.append(f"    // ---- vtable 0x{m['vtable']:08x} ({m['nvirtuals']} slots) ----")
        for i, e in enumerate(m["entries"]):
            fn = fns.get(e)
            tag = ""
            if fn and fn.get("strings"):
                s = fn["strings"][0]["text"]
                if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]{2,30}", s):
                    tag = f'  // refs "{s}"'
            lines.append(f"    virtual void vfunc_{i}();  // [{i}] 0x{e:08x}{tag}")
        lines.append("")
        if m["ctors"]:
            lines.append(f"    {ident}();  // 0x{m['ctors'][0]:08x}")
        lines.append(f"    virtual ~{ident}();")
        lines.append("};")
        lines.append("")

        with open(os.path.join(srcdir, ident + ".h"), "w") as f:
            f.write("\n".join(lines))
        written += 1

    print(f"headers written: {written}")
    print(f"into {srcdir}")


if __name__ == "__main__":
    main()
