#!/usr/bin/env python3
"""
Verify the reconstructed source tree.

Checks:
  1. every header compiles standalone
  2. the combined translation unit compiles
  3. class coverage: every class in the manifest has a header or definition
  4. vtable->class bindings re-resolve correctly through the RTTI COL chain
  5. header content vs manifest: the vtable VA and per-slot target addresses
     cited in each header match the manifest

Note: checks 1-4 prove the tree compiles and that the MANIFEST agrees with the
binary. Only check 5 reads what a header actually CLAIMS, so it is the only
check that can fail when a header is wrong about the binary. It does not
validate inheritance or member offsets yet.

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
            # A template instantiation is covered when the template it
            # instantiates is defined by a reconstructed header. Emitting a
            # separate header per instantiation would be wrong: one template
            # header legitimately defines every instantiation of it, so
            # coverage is credited to the template, not to a per-instantiation
            # file. The template name is taken from the authoritative RTTI
            # spelling, not from the mangled ident.
            rtti = str(m.get("class", ""))
            if "<" in rtti:
                tmpl = rtti.split("<")[0]
                for kw in ("class ", "struct "):
                    if tmpl.startswith(kw):
                        tmpl = tmpl[len(kw):]
                tmpl = tmpl.strip()
                if tmpl in defined or tmpl in have:
                    return True
                # Namespaced templates (ATL::CComObject) are spelled with the
                # scope collapsed in the ident.
                if tmpl.replace("::", "_") in defined:
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

    # 5. header content vs manifest
    #
    # Checks [1]-[4] only prove the headers PARSE and that the manifest agrees
    # with the binary. None of them reads a header's cited evidence, so a header
    # could cite a fabricated vtable address and still pass. This check closes
    # that hole by comparing what each header CLAIMS against the manifest: the
    # cited vtable VA and the cited per-slot target addresses.
    if os.path.exists(manp):
        man = json.load(open(manp))
        hdr_by_ident = {}
        for p in glob.glob(os.path.join(srcdir, "*.h")):
            hdr_by_ident[os.path.basename(p)[:-2]] = p
        # Every vtable RTTI attributes to a class, so a header citing a
        # secondary (non-primary) vtable is accepted rather than flagged.
        vts_by_class = {}
        cj = os.path.join(outdir, "classes.json")
        if os.path.exists(cj):
            for v in json.load(open(cj)).get("vtables", []):
                vts_by_class.setdefault(v.get("class", ""), set()).add(
                    v["vtable_va"])
        checked = bad = 0
        bad_examples = []
        for m in man:
            ident = m.get("ident")
            p = hdr_by_ident.get(ident)
            if not p:
                continue  # instantiation or nested class: owned by another header
            src = open(p).read()
            # cited vtable VA in the header banner
            vm = re.search(r"//\s*vtable\s*:\s*0x([0-9a-fA-F]+)", src)
            if not vm:
                continue
            checked += 1
            cited = int(vm.group(1), 16)
            # A class may legitimately own several vtables (MSVC emits one per
            # base subobject; RTTI records them with a nonzero `offset`). The
            # header is free to cite any of them, so accept the cited VA when
            # RTTI attributes it to this class, not only when it is the primary.
            known = vts_by_class.get(m["class"], set())
            if known:
                if cited not in known:
                    bad += 1
                    if len(bad_examples) < 5:
                        bad_examples.append(
                            f"{ident}: header cites vtable 0x{cited:08x}, "
                            f"which RTTI does not attribute to this class "
                            f"(known: "
                            f"{', '.join(hex(v) for v in sorted(known)[:4])})")
                    continue
            elif cited != m["vtable"]:
                bad += 1
                if len(bad_examples) < 5:
                    bad_examples.append(
                        f"{ident}: header cites vtable 0x{cited:08x}, "
                        f"manifest has 0x{m['vtable']:08x}")
                continue
            # Slot targets are only comparable when the header cites the
            # PRIMARY vtable, since the manifest's `entries` describe that one.
            if cited != m["vtable"]:
                continue
            # cited per-slot targets, e.g. "// [3] 0x00405790"
            cited_slots = {}
            for sm in re.finditer(r"//\s*\[(\d+)\]\s*0x([0-9a-fA-F]+)", src):
                cited_slots[int(sm.group(1))] = int(sm.group(2), 16)
            for idx, va in cited_slots.items():
                if idx < len(m["entries"]) and m["entries"][idx] != va:
                    bad += 1
                    if len(bad_examples) < 5:
                        bad_examples.append(
                            f"{ident}: slot [{idx}] cites 0x{va:08x}, "
                            f"manifest has 0x{m['entries'][idx]:08x}")
                    break
        print(f"[5] header content vs manifest: {checked-bad}/{checked} verified")
        if bad:
            failures.append(
                f"{bad} headers disagree with the manifest "
                f"({'; '.join(bad_examples)})")

    print()
    if failures:
        print("VERIFICATION FAILED:")
        for f in failures:
            print("  -", f)
        sys.exit(1)
    print("VERIFICATION PASSED")


if __name__ == "__main__":
    main()
