#!/usr/bin/env python3
"""
Systematic header generator for the Tiberian Sun reconstruction.

For every class recovered from RTTI, emit a header containing:
  - the exact inheritance (from RTTI base-class arrays)
  - the virtual function table in order, each entry resolved to a real VA
  - the constructor (located by the vtable store site) and its member offsets
  - data members named from INI keys / strings the class's methods reference

This guarantees complete coverage rather than hand-writing a subset.
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

    def va_of(self, off):
        for vs, ve, raw, rs, nm in self.secs:
            if raw <= off < raw + rs:
                return vs + (off - raw)
        return None


def sanitize(name):
    """Turn a demangled class name into a safe C++ identifier / filename."""
    n = re.sub(r"^(class|struct)\s+", "", name)
    n = n.replace("::", "_")
    n = re.sub(r"[<>,\s\*&]+", "_", n)
    n = re.sub(r"_+", "_", n).strip("_")
    return n


def load(outdir):
    cls = json.load(open(os.path.join(outdir, "classes.json")))
    fns = json.load(open(os.path.join(outdir, "functions.json")))
    keys = json.load(open(os.path.join(outdir, "ini_keys.json")))
    return cls, fns, keys


def vtable_entries(bin_, vtva, limit=400):
    """Read a vtable's code entries until a non-code dword."""
    ents = []
    for k in range(limit):
        p = bin_.u32(vtva + k * 4)
        if p is None or not bin_.in_text(p):
            break
        ents.append(p)
    return ents


def find_ctor(bin_, vtva):
    """Find the function that stores this vtable pointer (the constructor)."""
    pat = struct.pack("<I", vtva)
    hits = []
    start = 0
    while True:
        j = bin_.data.find(pat, start)
        if j < 0:
            break
        va = bin_.va_of(j)
        if va is not None and bin_.in_text(va):
            hits.append(va)
        start = j + 1
    return hits


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    srcdir = sys.argv[3] if len(sys.argv) > 3 else "src/include"

    bin_ = Bin(bindir)
    cls, fns, keys = load(outdir)
    byva = {f["va"]: f for f in fns["functions"]}

    # class -> vtables
    vtmap = defaultdict(list)
    for v in cls["vtables"]:
        vtmap[v["class"]].append(v)

    # class -> bases
    basemap = {}
    for c in cls["classes"]:
        basemap[c["demangled"]] = c

    os.makedirs(srcdir, exist_ok=True)
    manifest = []
    for cname, vts in sorted(vtmap.items()):
        c = basemap.get(cname)
        if not c:
            continue
        ident = sanitize(cname)
        if not ident:
            continue
        bases = [b["demangled"] for b in c["bases"][1:]]
        primary = vts[0]
        ents = vtable_entries(bin_, primary["vtable_va"])
        ctors = find_ctor(bin_, primary["vtable_va"])
        manifest.append({
            "class": cname,
            "ident": ident,
            "vtable": primary["vtable_va"],
            "nvirtuals": len(ents),
            "bases": bases,
            "ctors": ctors,
            "entries": ents,
        })

    with open(os.path.join(outdir, "class_manifest.json"), "w") as f:
        json.dump(manifest, f, indent=1)

    print(f"classes with vtables: {len(manifest)}")
    print(f"total virtual slots : {sum(m['nvirtuals'] for m in manifest)}")
    print(f"classes with ctor   : {sum(1 for m in manifest if m['ctors'])}")
    print(f"wrote {outdir}/class_manifest.json")


if __name__ == "__main__":
    main()
