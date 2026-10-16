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
count, a callee list and a calling convention. There is no register liveness,
no stack frame description and no local type information, so inventing C++
expressions would be guesswork. Each body is therefore an EMPTY definition
whose leading comment carries the full recovered evidence inline -- the VA,
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

    fns = {f["va"]: f for f in
           json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    sigs = json.load(open(os.path.join(outdir, "signatures.json")))

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

    if not os.path.isdir(gendir):
        os.makedirs(gendir)
    # Regenerate: clear only the .cpp files this tool owns.
    for old in glob.glob(os.path.join(gendir, "*.cpp")):
        os.remove(old)
    for cname, fname, header, defs in files:
        lines = []
        lines.append("// %s.cpp -- mechanically generated by tools/gen_bodies.py" %
                     fname)
        lines.append("// DO NOT EDIT: regenerate with ./tools/pipeline.sh.")
        lines.append("//")
        lines.append("// Bodies below are EMPTY definitions with the recovered")
        lines.append("// evidence transcribed as a comment. No control flow or")
        lines.append("// expression is invented: the binary provides only an")
        lines.append("// instruction listing, a size and a callee set.")
        lines.append('#include "%s"' % header)
        lines.append("")
        for ret, name, params, cqual, va in defs:
            f = fns[va]
            sig = sigs[hex(va)]
            lines.append("// ---------------------------------------------------------------------------")
            lines.append("// %s::%s -- body derived from %d instructions at 0x%08x; "
                         "semantics not reconstructed." %
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
            for ins in f.get("insns", []):
                lines.append("//   0x%08x  %-8s %s" %
                             (ins["va"], ins["mnem"], ins.get("op", "")))
            ret_s = (ret + " ") if ret else ""
            cq = (" " + cqual) if cqual else ""
            lines.append("%s%s::%s%s" % (ret_s, cname, name, params + cq))
            lines.append("{")
            lines.append("}")
            lines.append("")
        out = os.path.join(gendir, fname + ".cpp")
        with open(out, "w") as fh:
            fh.write("\n".join(lines))

    # Combined TU so the whole generated set can be compiled in one command.
    tupath = os.path.join(gendir, "bodies_all.cpp")
    with open(tupath, "w") as fh:
        fh.write("// Combined translation unit for every generated body file.\n")
        fh.write("// Regenerated by tools/gen_bodies.py.\n")
        for cname, fname, header, defs in files:
            fh.write('#include "%s.cpp"\n' % fname)

    readme = os.path.join(gendir, "README.md")
    with open(readme, "w") as fh:
        fh.write("# src/generated — mechanically derived function bodies\n\n")
        fh.write("Generated by `tools/gen_bodies.py`. Do not edit by hand; "
                 "regenerate with `./tools/pipeline.sh`.\n\n")
        fh.write("Each `.cpp` defines the member functions that its class's "
                 "reconstructed header declares and that the binary supplies "
                 "both a function record and a recovered signature for. The "
                 "signature is copied verbatim from the header declaration, so "
                 "the file compiles against the reconstructed tree.\n\n")
        fh.write("A body is an EMPTY definition. The recovered evidence — "
                 "target VA, byte size, instruction count, callees and the "
                 "complete instruction listing — is transcribed into the "
                 "leading comment. No control flow or expression is invented.\n\n")
        fh.write("## Totals\n\n")
        fh.write("| item | count |\n|---|---|\n")
        fh.write("| class translation units | %d |\n" % len(files))
        fh.write("| function bodies emitted | %d |\n" % emitted)
        for k in sorted(skip):
            fh.write("| skipped: %s | %d |\n" % (k, skip[k]))

    print("gen_bodies: %d translation units, %d function bodies" %
          (len(files), emitted))
    print("skipped:", json.dumps(skip, sort_keys=True))
    print("wrote", gendir)


if __name__ == "__main__":
    main()
