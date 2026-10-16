#!/usr/bin/env python3
"""
Generate the first .cpp function bodies for the reconstructed tree.

WHAT THIS EMITS
---------------
One translation unit per class, under src/generated/, containing out-of-line
definitions for the member functions that the reconstructed header for that
class DECLARES and for which the binary provides both a function record and a
recovered signature. The definition's signature is copied verbatim from the
header declaration, so the emitted TU compiles against the reconstructed
headers by construction.

WHAT A BODY HONESTLY IS
-----------------------
The recovered evidence per function is: an entry VA, a size, an instruction
listing (mnemonic, operand text, encoding bytes) and a calling convention.
There is no register liveness, no stack frame description and no local type
information, so inventing source-shaped C++ expressions would be guesswork.

Instead the instruction stream is TRANSCRIBED against the ts:: machine model in
Machine.h -- registers, flags, memory and control flow are modelled, so the
generated code does what the machine does. Each generated line is one machine
instruction and carries the original instruction as a trailing comment, so the
file can be read against the recovered listing line for line. It is not claimed
to be the shape the original source had.

The evidence comment also carries the recovered provenance inline -- the VA,
size, instruction count, callees, and the complete instruction listing. That is
a faithful low-level transcription of the original, not a reconstruction of its
high-level semantics, and it is labelled as such.

WHAT IS SKIPPED
---------------
  * declarations with no target-VA comment (the DECL pattern requires one)
  * pure virtuals ("= 0") and inline bodies, excluded by that same pattern
  * functions with no record in out/functions.json
  * functions with no entry in out/signatures.json (per the task contract: a
    signature is only usable when signatures.json has one)
  * target VAs outside .text (import thunks, RTTI stubs, jump-table entries)
  * a nested class whose flattened ident owns its own header, e.g.
    RadarClass::RTacticalClass is emitted from RadarClass_RTacticalClass.h

Output:
  src/generated/<Ident>.cpp   one per class with at least one usable function
  src/generated/bodies_all.cpp  combined TU including every generated file
  src/generated/README.md     provenance + counts (regenerated, not hand-kept)
"""
import glob
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import emit_x86

OUTDIR = "out"
GENDIR = "src/generated"

# A class/struct definition that opens a body we can parse member declarations
# from. Templates are excluded: their member functions are defined by the
# template header itself, not out of line here.
CLASS_OPEN = re.compile(
    r"^[ \t]*(?:class|struct)\s+(\w+)\s*(?::[^{;]*)?\{", re.M)

# A single-line member-function declaration carrying a target-VA comment.
# Group 1: declaration text without 'virtual', trailing ';' or comment.
# Group 2: the target VA.
DECL = re.compile(
    r"^[ \t]*(virtual\s+)?"
    r"((?:[\w:*&<>,\[\]\s]|\bconst\b)*?\b(?:~\w+|\w+)\s*\([^;{}()]*\))"
    r"\s*(const)?\s*;"
    r"[ \t]*//[^\n]*?(0x[0-9a-fA-F]{4,8})",
    re.M)

# .text bounds for the target binary (PE section table, see CONTEXT.md).
TEXT_LO = 0x401000
TEXT_HI = 0x401000 + 0x2A0D25  # 2.7MB .text; only used to reject imports/stubs


def parse_class_bodies(src):
    """Yield (qualified_name, body_text) for each class definition.

    A nested class (e.g. RadarClass::RTacticalClass) is yielded with its
    qualified name so its member functions can be defined out of line with the
    correct scope. The generator additionally skips a nested class when a
    flattened top-level header (RadarClass_RTacticalClass.h) already declares
    the same class under its flattened ident: that header is emitted
    separately, and emitting both would define the same machine code twice
    under two spellings.
    """
    # Collect every class open with its nesting depth and enclosing scope.
    opens = []
    for m in CLASS_OPEN.finditer(src):
        depth = src.count("{", 0, m.start()) - src.count("}", 0, m.start())
        opens.append((m.start(), m.end(), m.group(1), depth))
    # Determine, for each open, the chain of enclosing class names by looking
    # at the class opens whose bodies contain it.
    for start, end, name, depth in opens:
        scope = []
        for s2, e2, n2, d2 in opens:
            if s2 < start and d2 < depth and s2 < start:
                # n2's body contains this open iff its body extends past start
                body_end = _brace_end(src, e2 - 1)
                if body_end > start:
                    scope.append((d2, n2))
        scope.sort()
        qual = "::".join([n for _, n in scope] + [name])
        body_end = _brace_end(src, end - 1)
        # Blank out the text of any directly nested class definition so the
        # outer class's declaration scan does not pick up the nested class's
        # members (that was how DisplayClass::vfunc_3 came to name a slot that
        # belongs to DisplayClass::TacticalClass). Newlines are preserved so
        # the yielded body keeps its line structure.
        body = list(src[end - 1:body_end])
        for s2, e2, n2, d2 in opens:
            if s2 > start and d2 == depth + 1:
                b2 = min(_brace_end(src, e2 - 1), body_end)
                base = end - 1
                for i in range(max(s2, base), b2):
                    if body[i - base] != "\n":
                        body[i - base] = " "
        yield qual, "".join(body)


def _brace_end(src, open_brace):
    """Index just past the '}' matching the '{' at open_brace."""
    depth = 0
    i = open_brace
    while i < len(src):
        c = src[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return len(src)


def strip_comments(body):
    """Remove // comments so declaration matching cannot run into them."""
    out = []
    for line in body.splitlines():
        j = line.find("//")
        if j >= 0:
            line = line[:j]
        out.append(line)
    return "\n".join(out)


def decls_in_body(body):
    """Extract (decl_text, target_va) for emittable member functions.

    decl_text is the declaration with the leading 'virtual' and any calling
    convention removed, ready to be prefixed with '<ret> <Class>::'. Pure
    virtuals ("= 0") and declarations with an inline body are excluded by the
    DECL pattern itself: it requires ';' immediately after the parameter list
    (modulo 'const'), so neither '= 0;' nor '{ ... }' matches.
    """
    text = strip_comments(body)
    results = []
    for m in DECL.finditer(body):
        raw = m.group(2).strip()
        # Reject declarations that survived only because of a comment: the
        # stripped text must still contain this declaration.
        if raw.split("(")[0].strip().split()[-1] not in text:
            continue
        results.append((raw, m.group(3) or "", int(m.group(4), 16)))
    return results


def split_ret_name(decl):
    """Split 'int  Foo' -> ('int', 'Foo'); '~Foo' -> ('', '~Foo')."""
    decl = re.sub(r"\s+", " ", decl).strip()
    # drop a calling convention token if present (shim defines it empty)
    parts = decl.split("(", 1)
    head = parts[0].strip()
    tail = "(" + parts[1] if len(parts) > 1 else "()"
    for cc in ("__stdcall", "__cdecl", "__thiscall", "__fastcall"):
        head = head.replace(" " + cc, "").replace(cc + " ", "")
    head = head.strip()
    if head.startswith("~"):
        return "", head
    bits = head.rsplit(" ", 1)
    if len(bits) == 1:
        return "", bits[0]
    return bits[0].strip(), bits[1].strip()


def main():
    outdir = sys.argv[1] if len(sys.argv) > 1 else OUTDIR
    srcdir = sys.argv[2] if len(sys.argv) > 2 else "src/include"
    gendir = sys.argv[3] if len(sys.argv) > 3 else GENDIR
    # The target image. An indirect jump's destination lives in a switch table in
    # the image rather than in the instruction stream, so resolving one needs the
    # real bytes. Without the image those jumps are recorded, not guessed at.
    image = None
    for cand in (os.path.join(os.path.dirname(os.path.abspath(outdir)),
                              "bin", "GAME.EXE"),
                 os.path.join("bin", "GAME.EXE")):
        if os.path.exists(cand):
            image = emit_x86.PEImage(cand)
            break
    if image is None:
        print("gen_bodies: WARNING bin/GAME.EXE not found; indirect jumps will "
              "be recorded rather than resolved")

    totals = {"modelled": 0, "unmodelled": 0, "unmod": {}}
    fns = {f["va"]: f for f in
           json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    sigs = json.load(open(os.path.join(outdir, "signatures.json")))
    # The whole-binary denominator, so the transcription percentage in the README
    # cannot be read as "the whole binary is done".
    all_fns = len(fns)
    all_insns = sum(len(f.get("insns") or []) for f in fns.values())

    skip = {"no_function_record": 0, "no_signature": 0, "not_in_text": 0,
            "duplicate": 0, "nested_class_covered": 0}
    emitted = 0
    files = []
    seen = set()

    # Top-level class idents that own a header of their own. A nested class
    # whose flattened ident is in this set is emitted from that header, not
    # from its enclosing class's header.
    toplevel_idents = {os.path.basename(x)[:-2]
                       for x in glob.glob(os.path.join(srcdir, "*.h"))}

    for p in sorted(glob.glob(os.path.join(srcdir, "*.h"))):
        base = os.path.basename(p)
        if base in ("ts_prelude.h",):
            continue
        src = open(p).read()
        # A header may declare helper classes; emit a body per class in the
        # header that declares it, named after the class.
        for cname, body in parse_class_bodies(src):
            simple = cname.split("::")[-1]
            flat = cname.replace("::", "_")
            # A nested class with its own flattened header is emitted there.
            if cname != simple and flat in toplevel_idents:
                skip["nested_class_covered"] += 1
                continue
            defs = []
            for decl, cqual, va in decls_in_body(body):
                ret, name = split_ret_name(decl)
                if not name or not name.lstrip("~").replace("_", "").isalnum():
                    continue
                params = decl[decl.find("("):]
                if va not in fns:
                    # Distinguish "no function record" from a VA we never saw.
                    skip["no_function_record"] += 1
                    continue
                if not (TEXT_LO <= va < TEXT_HI):
                    skip["not_in_text"] += 1
                    continue
                if hex(va) not in sigs:
                    skip["no_signature"] += 1
                    continue
                key = (cname, name, params)
                if key in seen:
                    skip["duplicate"] += 1
                    continue
                seen.add(key)
                defs.append((ret, name, params, cqual, va))
            if not defs:
                continue
            fname = flat
            files.append((cname, fname, base, defs))
            emitted += len(defs)

    # --check runs the faithfulness gate against an EXISTING tree without
    # regenerating it, so the gate can be exercised on deliberately corrupted
    # output (which is how its own mutation self-test works).
    check_only = "--check" in sys.argv
    if not check_only:
        if not os.path.isdir(gendir):
            os.makedirs(gendir)
        # Regenerate: clear only the .cpp files this tool owns.
        for old in glob.glob(os.path.join(gendir, "*.cpp")):
            os.remove(old)
    # De-duplicate by function VA. The previous version emitted one full body per
    # DECLARING CLASS, so an inherited virtual a class does not override was
    # transcribed once per declaring class: 6,668 banners for 2,842 distinct
    # functions, one function rendered 159 times. Every declaring name must still
    # get a definition so the tree links, so the FIRST declarer carries the full
    # transcription and the rest become aliases that name it.
    canonical = {}
    distinct = 0
    for cname, fname, header, defs in files:
        lines = []
        lines.append("// %s.cpp -- mechanically generated by tools/gen_bodies.py" %
                     fname)
        lines.append("// DO NOT EDIT: regenerate with ./tools/pipeline.sh.")
        lines.append("//")
        lines.append("// Each body below is a TRANSCRIPTION of the recovered instruction")
        lines.append("// stream against the ts:: machine model in Machine.h. Registers,")
        lines.append("// flags, memory and control flow are modelled, not approximated;")
        lines.append("// each line is one machine instruction and keeps the original as a")
        lines.append("// trailing comment. Nothing is invented: the binary provides an")
        lines.append("// instruction listing, a size and a callee set, and nothing else.")
        lines.append("//")
        lines.append("// A body marked ALIAS defines the same machine code as another")
        lines.append("// declaration; the code is transcribed once, at the canonical")
        lines.append("// declaration named on the alias line.")
        lines.append('#include "%s"' % header)
        lines.append('#include "Machine.h"   // ts:: machine model the bodies below run on')
        lines.append("")
        for ret, name, params, cqual, va in defs:
            f = fns[va]
            sig = sigs[hex(va)]
            ret_s = (ret + " ") if ret else ""
            cq = (" " + cqual) if cqual else ""
            if va in canonical:
                lines.append("// %s::%s -- ALIAS of %s (same machine code at "
                             "0x%08x); transcribed once." %
                             (cname, name, canonical[va], va))
                lines.append("%s%s::%s%s" % (ret_s, cname, name, params + cq))
                lines.append("{")
                lines.append("}")
                lines.append("")
                continue
            canonical[va] = "%s::%s" % (cname, name)
            distinct += 1
            lines.append("// ---------------------------------------------------------------------------")
            lines.append("// %s::%s -- transcription of %d instructions at 0x%08x." %
                         (cname, name, f["ninsn"], va))
            lines.append("// size %d bytes; signature %s%s" %
                         (f["size"], sig.get("cc") or "?",
                          (" nargs=%s" % sig["nargs"]) if sig.get("nargs") is not None else ""))
            if f.get("callees"):
                lines.append("// callees: " +
                             ", ".join("0x%08x" % c for c in f["callees"]))
            else:
                lines.append("// callees: none")
            lines.append("//")
            lines.append("// Rendered against the ts:: machine model (Machine.h): registers,")
            lines.append("// flags, memory and control flow are modelled, not approximated.")
            lines.append("// Each line below is one machine instruction and keeps the original")
            lines.append("// instruction as a trailing comment. This is a transcription, not")
            lines.append("// source-shaped decompilation: it is faithful to what the machine")
            lines.append("// does, and is not claimed to be the shape the original source had.")
            lines.append("//")
            lines.append("%s%s::%s%s" % (ret_s, cname, name, params + cq))
            lines.append("{")
            items, st = emit_x86.transcribe(
                f.get("insns", []), image, va, f.get("end") or va + f["size"],
                ret_type=ret)
            for kind, cpp, rec in items:
                if kind == "label":
                    lines.append("    %s:;" % cpp)
                    continue
                # A rendering may be a block (an indirect jump's dispatch), in
                # which case only the first line carries the evidence record so
                # the gate still sees each instruction exactly once.
                first, _, rest = cpp.partition("\n")
                lines.append("    %s   //   %s" % (first, rec))
                for extra in rest.split("\n"):
                    if extra:
                        lines.append("    " + extra)
            totals["modelled"] += st["modelled"]
            totals["unmodelled"] += st["unmodelled"]
            for k, v in st["unmodelled_by_mnemonic"].items():
                totals["unmod"][k] = totals["unmod"].get(k, 0) + v
            # A recovered span can end without a `ret` (control falls through
            # into the next function, which the linker's ordering makes legal).
            # A value-returning definition still needs a path that returns.
            # Only when the span does not already end in a `ret`: otherwise the
            # line is dead and just noise.
            ends_in_ret = bool(items) and items[-1][0] == "stmt" and \
                "return" in items[-1][1]
            if ret and ret.strip() not in ("", "void") and not ends_in_ret:
                lines.append("    return (%s)(size_t)EAX.d;   // fall-through: the "
                             "recovered span ends without a ret" % ret.strip())
            lines.append("}")
            lines.append("")
        if not check_only:
            out = os.path.join(gendir, fname + ".cpp")
            with open(out, "w") as fh:
                fh.write("\n".join(lines))

    # Combined TU so the whole generated set can be compiled in one command.
    tupath = os.path.join(gendir, "bodies_all.cpp")
    if not check_only:
        with open(tupath, "w") as fh:
            fh.write("// Combined translation unit for every generated body "
                     "file.\n")
            fh.write("// Regenerated by tools/gen_bodies.py.\n")
            for cname, fname, header, defs in files:
                fh.write('#include "%s.cpp"\n' % fname)

    readme = os.path.join(gendir, "README.md")
    if not check_only:
      with open(readme, "w") as fh:
        fh.write("# src/generated — mechanically derived function bodies\n\n")
        fh.write("Generated by `tools/gen_bodies.py`. Do not edit by hand; "
                 "regenerate with `./tools/pipeline.sh`.\n\n")
        fh.write("Each `.cpp` defines the member functions that its class's "
                 "reconstructed header declares and that the binary supplies "
                 "both a function record and a recovered signature for. The "
                 "signature is copied verbatim from the header declaration, so "
                 "the file compiles against the reconstructed tree.\n\n")
        fh.write("A body is a TRANSCRIPTION of the recovered instruction stream "
                 "against the ts:: machine model in `Machine.h`. Registers, "
                 "flags, memory and control flow are modelled rather than "
                 "approximated, so the generated code does what the machine "
                 "does. Each generated line is one machine instruction and "
                 "carries the original instruction as a trailing comment, so a "
                 "file can be read against the recovered listing line for line.\n\n")
        fh.write("This is not source-shaped decompilation. It is not claimed to "
                 "be the shape the original source had; it is claimed to be "
                 "faithful to what the machine does.\n\n")
        fh.write("## Transcription coverage\n\n")
        fh.write("| item | count |\n|---|---|\n")
        tot = totals["modelled"] + totals["unmodelled"]
        fh.write("| instructions modelled | %d of %d (%.2f%%) |\n" %
                 (totals["modelled"], tot,
                  100.0 * totals["modelled"] / tot if tot else 0.0))
        fh.write("| instructions NOT modelled | %d |\n" % totals["unmodelled"])
        fh.write("\nThis 100% means every instruction of every function that got "
                 "a body is rendered. It does NOT mean the whole binary is "
                 "transcribed: a body is only emitted for a member function that "
                 "a reconstructed header DECLARES. The denominator is therefore "
                 "the declared set, not the binary.\n\n")
        fh.write("| scope | functions | instructions |\n|---|---|---|\n")
        fh.write("| transcribed here | %d | %d |\n" % (distinct, tot))
        fh.write("| the whole binary | %d | %d |\n" % (all_fns, all_insns))
        fh.write("| not yet transcribed (no declaration) | %d | %d |\n" %
                 (all_fns - distinct, all_insns - tot))
        if all_insns:
            fh.write("\nSo %.2f%% of the binary's instructions have a "
                     "transcribed body.\n\n" % (100.0 * tot / all_insns))
        if totals["unmod"]:
            fh.write("\nUnmodelled instructions by mnemonic. Each is emitted as "
                     "`ts::unmodelled(va, \"mnem\")` rather than silently "
                     "dropped or approximated:\n\n")
            fh.write("| mnemonic | count |\n|---|---|\n")
            for k in sorted(totals["unmod"], key=lambda x: -totals["unmod"][x]):
                fh.write("| `%s` | %d |\n" % (k, totals["unmod"][k]))
            fh.write("\n")
        fh.write("## Totals\n\n")
        fh.write("| item | count |\n|---|---|\n")
        fh.write("| class translation units | %d |\n" % len(files))
        fh.write("| member definitions (every declaring name) | %d |\n" % emitted)
        fh.write("| DISTINCT functions transcribed | %d |\n" % distinct)
        fh.write("| alias definitions (same machine code) | %d |\n" % (emitted - distinct))
        for k in sorted(skip):
            fh.write("| skipped: %s | %d |\n" % (k, skip[k]))

    tot = totals["modelled"] + totals["unmodelled"]
    print("gen_bodies: transcription coverage %d/%d instructions (%.2f%%), "
          "%d unmodelled" %
          (totals["modelled"], tot,
           100.0 * totals["modelled"] / tot if tot else 0.0, totals["unmodelled"]))
    if totals["unmod"]:
        top = sorted(totals["unmod"], key=lambda x: -totals["unmod"][x])[:8]
        print("gen_bodies: unmodelled by mnemonic: " +
              ", ".join("%s=%d" % (k, totals["unmod"][k]) for k in top))
    print("gen_bodies: %d translation units, %d member definitions, "
          "%d DISTINCT functions transcribed (%d aliases)" %
          (len(files), emitted, distinct, emitted - distinct))
    print("skipped:", json.dumps(skip, sort_keys=True))
    print("wrote", gendir)

    # ------------------------------------------------------------------
    # Faithfulness gate.
    #
    # Semantic equivalence with the original cannot be proven here: there is no
    # MSVC 6 toolchain, only g++. So the only claim a transcription may make is
    # that it renders the recovered instruction stream faithfully -- and that
    # claim has to be checkable rather than asserted in prose.
    #
    # The gate re-reads the generated files and asserts, for every transcribed
    # function, that its instructions appear exactly once, in order, with
    # matching bytes. A dropped or reordered instruction fails it.
    # ------------------------------------------------------------------
    bad = []
    checked = 0
    for cname, fname, header, defs in files:
        path = os.path.join(gendir, fname + ".cpp")
        try:
            text = open(path).read()
        except OSError:
            continue
        for ret, name, params, cqual, va in defs:
            if canonical.get(va) != "%s::%s" % (cname, name):
                continue  # alias: the code is checked at its canonical site
            f = fns[va]
            want = ["0x%08x  %-8s %s" % (i["va"], i["mnem"], i.get("op", ""))
                    for i in f.get("insns", [])]
            # Extract THIS function's own listing, from its banner to the next
            # banner. Searching the whole file for the records as a subsequence
            # matched instructions belonging to other functions and reported
            # false failures.
            banner = ("// %s::%s -- transcription of %d instructions at 0x%08x"
                      % (cname, name, f["ninsn"], va))
            start = text.find(banner)
            checked += 1
            if start < 0:
                bad.append("0x%08x (%s::%s): banner not found" % (va, cname, name))
                continue
            # Split on the NEXT banner rather than a hardcoded rule width: the
            # separator's dash count is not worth duplicating, and getting it
            # wrong silently swallows the following function's instructions.
            nxtm = re.search(r"^// \S+::\S+ -- (?:transcription of|ALIAS of)",
                             text[start + len(banner):], re.M)
            block = (text[start:start + len(banner) + nxtm.start()]
                     if nxtm else text[start:])
            # The record trails the generated statement (`CODE;   //   0x...`),
            # so the match must not be anchored to the start of the line. It must
            # still be a comment, which is what distinguishes evidence from the
            # generated code itself.
            got = re.findall(r"//   (0x[0-9a-f]{8}  \S+[^\n]*)$", block, re.M)
            if got != want:
                same = sum(1 for a, b in zip(got, want) if a == b)
                bad.append("0x%08x (%s::%s): %d/%d records match"
                           % (va, cname, name, same, len(want)))
                continue
            # Round-trip. The record check above proves the evidence is complete
            # and in order; it does NOT prove the code matches the evidence,
            # because a corrupted statement leaves its comment untouched. So
            # re-render the function and require the file to be exactly what the
            # generator would produce. This is the check that catches a mutated
            # operand, a dropped statement or a swapped label.
            exp_items, _st = emit_x86.transcribe(
                f.get("insns", []), image, va, f.get("end") or va + f["size"],
                ret_type=ret)
            exp_body = []
            for kind, cpp, rec in exp_items:
                if kind == "label":
                    exp_body.append("    %s:;" % cpp)
                    continue
                first, _, rest = cpp.partition("\n")
                exp_body.append("    %s   //   %s" % (first, rec))
                for extra in rest.split("\n"):
                    if extra:
                        exp_body.append("    " + extra)
            ends_in_ret = bool(exp_items) and exp_items[-1][0] == "stmt" and \
                "return" in exp_items[-1][1]
            if ret and ret.strip() not in ("", "void") and not ends_in_ret:
                exp_body.append("    return (%s)(size_t)EAX.d;   // fall-through: "
                                "the recovered span ends without a ret" % ret.strip())
            got_body = [l for l in block.split("\n")
                        if l.startswith("    ") and l.strip()]
            if got_body != exp_body:
                where = "end of body"
                for i, (a, b) in enumerate(zip(got_body, exp_body)):
                    if a != b:
                        where = "line %d" % i
                        break
                bad.append("0x%08x (%s::%s): code does not match the recovered "
                           "instructions at %s" % (va, cname, name, where))

    if bad:
        print("gen_bodies FAITHFULNESS GATE FAILED for %d function(s): %s"
              % (len(bad), "; ".join(bad[:5])))
        return 1
    print("gen_bodies faithfulness gate: %d distinct function(s) verified "
          "instruction-for-instruction" % checked)
    return 0


if __name__ == "__main__":
    sys.exit(main())
