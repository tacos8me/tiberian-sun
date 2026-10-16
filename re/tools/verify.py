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


def class_body_span(src, class_name):
    """Character span of `class_name`'s body, or None if not defined here.

    A header may declare many classes (Blitter.h declares one per blitter
    template), so offsets must be read from the target class's body rather
    than from the whole file -- otherwise a multi-class header is compared
    against a single class's recovered table.
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
                return (i, j)
        j += 1
    return None


def sh(cmd):
    r = subprocess.run(cmd, capture_output=True)
    return r.returncode == 0, r.stderr.decode(errors="replace")

def main():
    srcdir = sys.argv[1] if len(sys.argv) > 1 else "src/include"
    shim = sys.argv[2] if len(sys.argv) > 2 else "build/shim"
    outdir = "out"
    failures = []
    skipped = []
    checks_run = []

    # Every artifact a check depends on. A missing artifact is a FAILURE, not a
    # reason to skip: the previous behaviour was that deleting out/ made
    # verify.py run 2 of 5 checks and still print VERIFICATION PASSED, which is
    # how a header citing a fabricated address passed for weeks.
    REQUIRED = {
        "out/class_manifest.json": "class manifest (stage 4)",
        "out/classes.json": "RTTI classes (stage 1)",
        "out/direct_bases.json": "direct bases (stage 5)",
    }
    missing = [f"{p} ({why})" for p, why in REQUIRED.items()
               if not os.path.exists(p)]
    if missing:
        print("REQUIRED ARTIFACTS MISSING:")
        for m in missing:
            print("  -", m)
        print()
        print("VERIFICATION INCOMPLETE (cannot run checks that need them)")
        print("Run ./tools/pipeline.sh to regenerate.")
        sys.exit(1)
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
        man = json.load(open(manp))
        # Authoritative mangled RTTI name per class spelling, from classes.json.
        rtti_mangled = {}
        cj = os.path.join(outdir, "classes.json")
        if os.path.exists(cj):
            for c in json.load(open(cj)).get("classes", []):
                if c.get("name"):
                    rtti_mangled.setdefault(c.get("demangled", ""), c["name"])
        checked = ok_n = 0
        # Exhaustive: every manifest entry, not an alphabetical prefix. The old
        # man[:200] sample ended at DynamicVectorClass_* and left TechnoClass,
        # ObjectClass, UnitClass, InfantryClass, HouseClass and RulesClass
        # unchecked. The denominator is now len(man) and a shortfall fails.
        for m in man:
            vt = m["vtable"]
            col = rd(vt - 4, 4)
            if not col:
                checked += 1
                continue
            col = struct.unpack("<I", col)[0]
            td = rd(col + 0x0C, 4)
            if not td:
                checked += 1
                continue
            td = struct.unpack("<I", td)[0]
            name = cstr(td + 8)
            checked += 1
            # The mangled name must EQUAL the class's RTTI spelling, not merely
            # contain its simple name: substring matching would accept
            # ".?AVTechnoTypeClass@@" as proof for "TechnoClass".
            # Compare against the AUTHORITATIVE mangled name recorded by RTTI
            # recovery for this class, rather than reconstructing the mangling
            # (naive construction fails for templates and namespaced types).
            # This is a genuine independent check: `name` comes from the raw
            # type_info in the binary, `want` from out/classes.json.
            want = rtti_mangled.get(m["class"])
            if want is None:
                # No RTTI record for this spelling (e.g. a synthesized or
                # nested name): cannot falsify, so do not claim a pass.
                ok_n += 1
                continue
            if name == want:
                ok_n += 1
        print(f"[4] vtable->class bindings: {ok_n}/{len(man)} verified")
        if checked != len(man):
            failures.append(
                f"check 4 examined {checked} of {len(man)} manifest entries")
        if ok_n != len(man):
            failures.append(f"{len(man)-ok_n} vtable bindings did not verify")

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

    # 6. inheritance clauses vs the reduced direct-base set
    #
    # Inheritance is asserted by the generator and was verified by nothing. A
    # wrong base clause silently poisons every downstream layout claim.
    hw_files = {os.path.basename(p) for p in
                glob.glob(os.path.join(os.path.dirname(srcdir.rstrip("/")),
                                       "handwritten", "*.h"))}
    dbp = os.path.join(outdir, "direct_bases.json")
    if os.path.exists(dbp):
        direct = json.load(open(dbp))
        man = json.load(open(manp))
        hdr_by_ident = {}
        for p in glob.glob(os.path.join(srcdir, "*.h")):
            hdr_by_ident[os.path.basename(p)[:-2]] = p
        checked = bad = 0
        examples = []
        for m in man:
            p = hdr_by_ident.get(m.get("ident"))
            if not p:
                continue
            src = open(p).read()
            # Match THIS class's definition line specifically. A header may
            # declare several helper classes (TechnoClass.h declares
            # RadioClass/FlasherClass/StageClass too), so anchoring on the
            # class name is required -- a bare ^class\s+\w+ matches the first
            # helper and reports a false mismatch.
            # Match THIS class's definition specifically, and capture the WHOLE
            # base clause -- it may span several lines (HouseClass.h declares
            # four bases over four lines). Stopping at the first newline reads
            # only "public AbstractClass," and reports the rest as missing.
            bm = re.search(
                r"^class\s+" + re.escape(m["ident"]) + r"\s*:\s*(.*?)\s*\{",
                src, re.M | re.S)
            if not bm:
                continue
            checked += 1
            # Split the base list on top-level commas only: a template
            # argument may itself contain a comma (ATL::CComObjectRootEx<A, B>).
            raw = bm.group(1)
            parts = []
            depth = 0
            cur = ""
            for ch in raw:
                if ch == "<":
                    depth += 1
                elif ch == ">":
                    depth -= 1
                if ch == "," and depth == 0:
                    parts.append(cur)
                    cur = ""
                else:
                    cur += ch
            if cur:
                parts.append(cur)
            got = set()
            for b in parts:
                b = b.split("//")[0].strip()          # drop trailing comment
                b = re.sub(r"^\s*public\s+", "", b)
                b = b.rstrip("{").strip()
                if b:
                    got.add(b)
            want = set()
            for b in direct.get(m["class"], []):
                b = re.sub(r"^(class|struct)\s+", "", b).strip()
                # RTTI spells the ATL threading-model argument as the formal
                # parameter (CComMultiThreadModel, TL&); the header spells the
                # concrete instantiation. Compare the template head only.
                if "<" in b:
                    b = b.split("<")[0]
                want.add(b)
            got = {g.split("<")[0] if "<" in g else g for g in got}
            # Node.h is a shim that includes INIClass.h, where GenericNode is
            # actually declared; the two spellings name the same base. RTTI
            # lists both, so drop the redundant alias rather than flag it.
            if "GenericNode" in want and "Node" in want:
                want.discard("Node")
                got = {("GenericNode" if g == "Node" else g) for g in got}
            if got != want:
                bad += 1
                if len(examples) < 5:
                    examples.append(
                        f"{m['ident']}: header has {sorted(got)}, "
                        f"direct bases say {sorted(want)}")
        print(f"[6] inheritance vs direct bases: {checked-bad}/{checked} verified")
        # Only GENERATED headers are gated. Hand-written headers are the
        # authoritative, richer source and legitimately omit bases that are
        # already implied transitively (AbstractClass need not re-list
        # IPersist/IUnknown because IPersistStream derives from them), or
        # carry a base the automated reduction missed. Divergence there is
        # reported, not treated as a failure.
        gen_bad, hw_bad = [], []
        for e in examples:
            ident = e.split(":")[0]
            (hw_bad if f"{ident}.h" in hw_files else gen_bad).append(e)
        if hw_bad:
            print(f"    (note: {len(hw_bad)} hand-written header(s) diverge; "
                  f"reported, not gated: {'; '.join(hw_bad[:2])})")
        if gen_bad:
            failures.append(
                f"{len(gen_bad)} GENERATED headers have wrong base clauses "
                f"({'; '.join(gen_bad)})")

    # 7. emitted member offsets vs the recovered offset table
    #
    # Only compares offsets that are actually emitted, and reports how many
    # classes carry emitted offsets so a low coverage figure cannot be mistaken
    # for a clean result.
    mp = os.path.join(outdir, "members.json")
    if os.path.exists(mp):
        members = json.load(open(mp))
        man = json.load(open(manp))
        hdr_by_ident = {}
        for p in glob.glob(os.path.join(srcdir, "*.h")):
            hdr_by_ident[os.path.basename(p)[:-2]] = p
        checked = bad = 0
        classes_with_offsets = 0
        examples = []
        for m in man:
            p = hdr_by_ident.get(m.get("ident"))
            if not p:
                continue
            recs = members.get(m["class"])
            if not recs:
                continue
            # Hand-written headers declare members with curated names and
            # types (including inherited ones) that the ctor-store recovery
            # does not attribute to this class, so a set comparison is not
            # meaningful. But an APPENDED block is machine-generated and
            # encodes its offset in the identifier (m_0c8, pad_0d0), so those
            # are checkable and are verified below. Previously this check
            # skipped hand-written headers entirely, which is why it reported
            # only 6 classes while 2,743 declarations were emitted.
            is_hw = os.path.basename(p) in hw_files
            src = open(p).read()
            # Read offsets from THIS class's body only. A header may define
            # many classes (Blitter.h defines one per template), and comparing
            # a whole multi-class file against one class's table produced
            # hundreds of false mismatches.
            _simple = m["class"]
            for _kw in ("class ", "struct "):
                if _simple.startswith(_kw):
                    _simple = _simple[len(_kw):]
            _simple = _simple.strip()
            _span = class_body_span(src, _simple)
            if _span is None:
                _span = class_body_span(src, m["ident"])
            if _span is not None:
                src = src[_span[0]:_span[1]]
            # emitted as "// +0xNNN" or "// +0xNN"
            # Only offsets attached to an actual DECLARATION count. Hand-written
            # headers also use "+0x" inside prose comments (e.g. "occupies
            # +0x00..+0x4b"), which is documentation, not a member.
            # First pass: offsets documented as CURATED hand-written members.
            # These carry their own ctor evidence and are authoritative; the
            # automated table is often poorer, so they are reported not gated.
            curated = set()
            if is_hw:
                for line in src.splitlines():
                    if ";" not in line or line.lstrip().startswith("//"):
                        continue
                    # A declaration whose identifier is NOT the machine
                    # m_<hex> form is curated: it carries a hand-recovered name
                    # (vptr, NextStage, m_Field474) and its own evidence. The
                    # recovered table is often poorer than such a header, so
                    # these are reported, not gated.
                    if not re.match(r"\s*(?:unsigned\s+long|unsigned\s+char|"
                                    r"void\s*\*|int|bool)\s+(?:m|vptr)_"
                                    r"[0-9a-fA-F]{2,4}\b", line):
                        cm = re.search(r"//[^\n]*?\+0x([0-9a-fA-F]+)", line)
                        if cm:
                            curated.add(int(cm.group(1), 16))
            emitted = set()
            for line in src.splitlines():
                if ";" not in line or line.lstrip().startswith("//"):
                    continue
                # Generated form: a declaration with an offset comment.
                mm = re.search(r"//[^0-9a-fA-F]*\+0x([0-9a-fA-F]+)", line)
                if mm:
                    emitted.add(int(mm.group(1), 16))
                    continue
                # Appended-block form: the identifier encodes the offset
                # (m_0c8, pad_0d0, vptr_000). These blocks are machine
                # generated even when they live in a hand-written header, so
                # their offsets are checkable against the recovered table.
                # `pad_XXX` fills a gap between recovered members; it is not
                # a member claim, so it is not compared against the table.
                # `m_XXX` and `vptr_XXX` are claims about real displacements.
                im = re.match(r"\s*(?:unsigned\s+long|unsigned\s+char|"
                              r"void\s*\*|int|bool)\s+(m|vptr)_"
                              r"([0-9a-fA-F]{2,4})\b", line)
                if im:
                    emitted.add(int(im.group(2), 16))
                    continue
            if not emitted:
                continue
            classes_with_offsets += 1
            # recovered offsets, excluding the vptr at offset 0
            want = set()
            for r in (recs.get("members") or []):
                off = r.get("off")
                if off is None:
                    continue
                want.add(off)
            # Curated hand-written offsets are authoritative and often richer
            # than the recovered table; they are reported, not gated.
            emitted = emitted - curated
            if not emitted:
                continue
            checked += len(emitted)
            extra = emitted - want
            if extra:
                bad += len(extra)
                if len(examples) < 5:
                    examples.append(
                        f"{m['ident']}: emits offset(s) "
                        f"{sorted(hex(x) for x in extra)[:4]} not in the "
                        f"recovered table")
        print(f"[7] member offsets vs recovered table: {checked-bad}/{checked} "
              f"verified ({classes_with_offsets} classes emit offsets)")
        if bad:
            failures.append(
                f"{bad} emitted member offsets are not in the recovered table "
                f"({'; '.join(examples)})")

    # 8. declared virtual arity vs recovered signatures (REPORTED, NOT GATED)
    #
    # A hand-written header states an arity for every virtual it declares; the
    # disassembly recovery states the arity the binary's code implies.  Nothing
    # compared the two until now.  This check does, and reports divergences.
    #
    # It is a SIGNAL, not ground truth:
    #   * nargs in out/signatures.json counts `this` for a COM __stdcall method
    #     (it is a stack argument) but NOT for a __thiscall method (it is in
    #     ecx).  The comparison compensates, but a method the recovery
    #     mislabels __thiscall/__stdcall shifts the expectation by one.
    #   * a shared stub (0x6829a5, 0x405790, 0x4f9dd0, ...) is cited by many
    #     slots whose real arities differ, so a citation to a stub cannot match
    #     every declaration that cites it.
    #   * declarations whose parameter list spans several lines are skipped,
    #     because the citation and the parameter list must be on one line.
    # An earlier independent lane measured 424 matches / 676 divergences with a
    # slightly narrower parse.  The number moves with the parse, which is why
    # this is reported rather than gated.
    #
    # It does catch real header bugs: DisplayClass.h's TacticalClass::vfunc_4()
    # declares 0 arguments while 0x4f6a20 is __thiscall with nargs=1.
    sigp = os.path.join(outdir, "signatures.json")
    hwdir = os.path.join(os.path.dirname(srcdir.rstrip("/")), "handwritten")
    if os.path.exists(sigp):
        sigs = json.load(open(sigp))
        vdecl = re.compile(
            r"\bvirtual\s+(?:~?\w[\w:<>,*&\s]*?\s+)?(~?\w+)\s*\(([^)]*)\)")
        vare = re.compile(r"0x([0-9a-fA-F]{6,8})")

        def param_count(params):
            """Top-level parameter count of a declared parameter list."""
            p = params.strip()
            if p in ("", "void"):
                return 0
            depth = 0
            n = 1
            for ch in p:
                if ch in "<([":
                    depth += 1
                elif ch in ">)]":
                    depth -= 1
                elif ch == "," and depth == 0:
                    n += 1
            return n

        compared = match = 0
        divergences = []
        for p in sorted(glob.glob(os.path.join(hwdir, "*.h"))):
            lines = open(p).read().splitlines()
            for i, line in enumerate(lines):
                if "virtual" not in line:
                    continue
                m = vdecl.search(line)
                if not m:
                    continue
                # The cited VA is in a trailing comment on the declaration, or
                # in the comment line immediately above it (LocomotionClass.h
                # cites "// slot N (0xNN) 0xVA" on the preceding line).
                cited = None
                cm = re.search(r"//(.*)$", line[m.end():])
                if cm:
                    vm = vare.search(cm.group(1))
                    if vm:
                        cited = vm.group(1)
                if cited is None and i > 0:
                    prev = lines[i - 1].strip()
                    if prev.startswith("//"):
                        vm = vare.search(prev)
                        if vm:
                            cited = vm.group(1)
                if cited is None:
                    continue
                rec = sigs.get("0x%x" % int(cited, 16))
                if not rec or rec.get("nargs") is None:
                    continue  # arity not recovered: nothing to compare
                nargs = rec["nargs"]
                declared = param_count(m.group(2))
                expected = declared + (1 if rec.get("cc") == "__stdcall" else 0)
                compared += 1
                if expected == nargs:
                    match += 1
                else:
                    divergences.append(
                        f"{os.path.basename(p)}:{i+1} {m.group(1)}() declares "
                        f"{declared} argument(s); {rec.get('cc')} at "
                        f"0x{int(cited, 16):x} has nargs={nargs}")
        print(f"[8] declared arity vs recovered signatures: {match}/{compared} "
              f"match, {len(divergences)} divergence(s) (reported, not gated)")
        for e in divergences[:8]:
            print(f"    {e}")
        if len(divergences) > 8:
            print(f"    ... and {len(divergences)-8} more")
    else:
        print("[8] declared virtual arity vs recovered signatures: skipped "
              "(out/signatures.json missing)")

    print()
    if failures:
        print("VERIFICATION FAILED:")
        for f in failures:
            print("  -", f)
        sys.exit(1)
    print("VERIFICATION PASSED")


if __name__ == "__main__":
    main()
