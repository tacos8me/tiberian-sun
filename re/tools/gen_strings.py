#!/usr/bin/env python3
"""
Extract strings referenced by code, including rules.ini field names.

Walks every recovered function's instruction stream, resolves immediate
operands that point into the image's data sections, and records the string at
that address. INI-style keys (CamelCase identifiers) are the original struct
member names.

Output: out/ini_keys.json  { string -> [function VAs that reference it] }
"""
import json
import os
import re
import struct
import sys

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

    def rd(self, va, n):
        for vs, ve, raw, rs, nm in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                return self.data[o:o + n]
        return None

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

    def valid(self, va):
        return any(vs <= va < ve for vs, ve, _, _, _ in self.secs)


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    fns = json.load(open(os.path.join(outdir, "functions.json")))["functions"]

    strings = {}
    for f in fns:
        for ins in f["insns"]:
            if ins["mnem"] not in ("push", "mov", "cmp", "lea"):
                continue
            op = ins["op"]
            if not op.startswith("0x"):
                continue
            try:
                imm = int(op.split(",")[-1].strip(), 16)
            except ValueError:
                continue
            if not bin_.valid(imm):
                continue
            s = bin_.cstr(imm, 200)
            if not s or len(s) < 4:
                continue
            if not all(32 <= ord(c) < 127 for c in s):
                continue
            strings.setdefault(s, set()).add(f["va"])

    out = {k: sorted(v) for k, v in sorted(strings.items())}
    with open(os.path.join(outdir, "ini_keys.json"), "w") as fh:
        json.dump(out, fh, indent=1)

    keys = [k for k in out if re.fullmatch(r"[A-Z][A-Za-z0-9]{2,30}", k)]
    print(f"referenced strings: {len(out)}")
    print(f"INI-key-like      : {len(keys)}")
    print(f"wrote {outdir}/ini_keys.json")


if __name__ == "__main__":
    main()
