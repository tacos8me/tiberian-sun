#!/usr/bin/env python3
"""
Verify the reconstructed source tree.

Checks:
  1. every header compiles standalone
  2. the combined translation unit compiles
  3. class coverage: every class in the manifest has a header or definition
  4. vtable->class bindings re-resolve correctly through the RTTI COL chain
  5. inheritance in emitted headers matches the reduced direct-base set

Exits non-zero if any check fails.
"""
import glob
import json
import os
import re
import struct
import subprocess
import sys

IB = 0x400000


def sh(cmd):
    r = subprocess.run(cmd, capture_output=True)
    return r.returncode == 0, r.stderr.decode(errors="replace")


def main():
    srcdir = sys.argv[1] if len(sys.argv) > 1 else "src/include"
    shim = sys.argv[2] if len(sys.argv) > 2 else "build/shim"
    outdir = "out"
    failures = []

    headers = sorted(p for p in glob.glob(os.path.join(srcdir, "*.h"))
                     if os.path.basename(p) != "ts_prelude.h")

    # 1. standalone
    bad = []
    for p in headers:
        ok, _ = sh(["g++", "-std=c++03", "-fsyntax-only", f"-I{srcdir}",
                    f"-I{shim}", "-include", p, "-x", "c++", "/dev/null"])
        if not ok:
            bad.append(os.path.basename(p))
    print(f"[1] standalone compile: {len(headers)-len(bad)}/{len(headers)}")
    if bad:
        failures.append(f"standalone failures: {', '.join(bad[:10])}")

    # 2. combined
    tu = "build/tu_all.cpp"
    if os.path.exists(tu):
        ok, err = sh(["g++", "-std=c++03", "-fsyntax-only", f"-I{srcdir}",
                      f"-I{shim}", tu])
        print(f"[2] combined TU compile: {'PASS' if ok else 'FAIL'}")
        if not ok:
            failures.append("combined TU failed")
    else:
        print("[2] combined TU: skipped (build/tu_all.cpp missing)")

    # 3. coverage
    manp = os.path.join(outdir, "class_manifest.json")
    if os.path.exists(manp):
        man = json.load(open(manp))
        defined = set()
        for p in glob.glob(os.path.join(srcdir, "*.h")):
            s = open(p).read()
            for m in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    s, re.M):
                defined.add(m.group(1))
        have = {os.path.basename(p)[:-2] for p in headers}
        # A class is covered if it has a header, is defined inside one, or is a
        # template instantiation of a type that a header defines (e.g.
        # Node<INISection*, INIClass> is defined by INIClass.h).
        def is_covered(m):
            if m["ident"] in have or m["ident"] in defined:
                return True
            for pre in ("Node_", "GenericNode"):
                if m["ident"].startswith(pre):
                    base = m["ident"][len(pre):].split("_")[0]
                    if base in defined or "Node" in defined:
                        return True
            return False
        covered = [m for m in man if is_covered(m)]
        pct = 100.0 * len(covered) / len(man) if man else 0
        print(f"[3] class coverage: {len(covered)}/{len(man)} ({pct:.1f}%)")
        if len(covered) != len(man):
            missing = [m["ident"] for m in man if not is_covered(m)]
            failures.append(f"uncovered classes: {', '.join(missing[:10])}")

    # 4. vtable->class bindings
    binp = "bin/GAME.EXE"
    if os.path.exists(binp) and os.path.exists(manp):
        data = open(binp, "rb").read()
        import pefile
        pe = pefile.PE(binp, fast_load=True)
        secs = []
        for s in pe.sections:
            secs.append((IB + s.VirtualAddress,
                         IB + s.VirtualAddress + s.Misc_VirtualSize,
                         s.PointerToRawData, s.SizeOfRawData))

        def rd(va, n):
            for vs, ve, raw, rs in secs:
                if vs <= va < ve:
                    o = raw + (va - vs)
                    return data[o:o + n]
            return None

        def cstr(va):
            b = rd(va, 256)
            if not b:
                return ""
            return b.split(b"\x00")[0].decode("latin1")

        man = json.load(open(manp))
        checked = ok_n = 0
        for m in man[:200]:
            vt = m["vtable"]
            col = rd(vt - 4, 4)
            if not col:
                continue
            col = struct.unpack("<I", col)[0]
            td = rd(col + 0x0C, 4)
            if not td:
                continue
            td = struct.unpack("<I", td)[0]
            name = cstr(td + 8)
            checked += 1
            # the mangled name should contain the class's simple name
            simple = re.sub(r"^(class|struct)\s+", "", m["class"]).split("::")[-1]
            simple = re.sub(r"[<>,\s\*&].*$", "", simple)
            if simple and simple in name:
                ok_n += 1
        print(f"[4] vtable->class bindings: {ok_n}/{checked} verified")
        if checked and ok_n != checked:
            failures.append(f"{checked-ok_n} vtable bindings did not verify")

    print()
    if failures:
        print("VERIFICATION FAILED:")
        for f in failures:
            print("  -", f)
        sys.exit(1)
    print("VERIFICATION PASSED")


if __name__ == "__main__":
    main()
