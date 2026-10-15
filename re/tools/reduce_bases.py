#!/usr/bin/env python3
"""
Reduce RTTI base-class arrays to C++ direct bases.

RTTI lists every base transitively. C++ must only name direct bases, so a base
B is dropped from class C if some other base of C also lists B among its bases.

Output: out/direct_bases.json  { class -> [direct base names] }
"""
import json
import os
import sys


def main():
    outdir = sys.argv[1] if len(sys.argv) > 1 else "out"
    d = json.load(open(os.path.join(outdir, "classes.json")))
    byclass = {c["demangled"]: c for c in d["classes"]}

    def basenames(c):
        return [b["demangled"] for b in c["bases"][1:]]

    direct = {}
    for c in d["classes"]:
        bs = basenames(c)
        trans = set()
        for b in bs:
            bc = byclass.get(b)
            if bc:
                for bb in basenames(bc):
                    if bb in bs:
                        trans.add(bb)
        direct[c["demangled"]] = [b for b in bs if b not in trans]

    with open(os.path.join(outdir, "direct_bases.json"), "w") as f:
        json.dump(direct, f, indent=1)

    multi = sum(1 for v in direct.values() if len(v) > 1)
    print(f"classes: {len(direct)}")
    print(f"with >1 direct base: {multi}")
    print(f"wrote {outdir}/direct_bases.json")


if __name__ == "__main__":
    main()
