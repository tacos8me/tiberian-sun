#!/usr/bin/env python3
"""Emit the out-of-line member definitions the reconstructed headers declare.

WHAT THIS EMITS
---------------
One translation unit per class, under src/generated/, containing out-of-line
definitions for the member functions that the reconstructed header for that
class declares and that the binary supplies a function record for. The
definition's signature is copied verbatim from the header declaration, so the
emitted TU compiles against the reconstructed tree by construction.

WHAT A BODY IS
--------------
A forwarder. The transcription of the machine code lives in
src/generated/code/, one canonical `ts::sub_XXXXXXXX(void)` per recovered
function VA, gated there by tools/gen_code.py. This file's job is to give every
*declaring name* a definition so the tree links, and to name the VA that the
definition is:

    // AbstractClass::QueryInterface -- the body is the canonical transcription
    // of 4 instructions at 0x006829a5, in src/generated/code/.
    HRESULT AbstractClass::QueryInterface()
    {
        ts::sub_006829a5();
        return (HRESULT)(size_t)ts::EAX.d;
    }

Two properties this buys, both of which the previous shape lacked:

  * exactly one transcription per machine function. Before, an inherited
    virtual a class does not override was transcribed once per declaring class;
    the de-duplication by VA fixed the duplication, and moving the code to the
    VA-indexed tree means there is no second copy to keep in step.
  * the return value is read out of the machine model, which is where the
    callee left it, rather than reconstructed from a signature the binary does
    not actually carry.

The signature is void(void) for the same reason as in gen_code.py: a
transcribed body reads its arguments from the machine model, not from C++
parameters. A `ret` pops the return address off the modelled stack, which is
what ts_call_at pushed.

WHAT IS SKIPPED
---------------
  * declarations with no target-VA comment (the DECL pattern requires one)
  * pure virtuals ("= 0") and inline bodies, excluded by that same pattern
  * functions with no record in out/functions.json
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
CODEDIR = "src/generated/code"

# A class/struct definition that opens a body we can parse member declarations
# from. Templates are excluded: their member functions are defined by the
# template header itself, not out of line here.
#
# Group 1 captures an optional `template<...>` prefix, group 2 the class name.
# A template class is skipped entirely: its members are defined by the template
# header that declares it (src/handwritten/VectorClass.h and friends), and an
# out-of-line `DynamicVectorClass::~DynamicVectorClass()` with no argument list
# is not a valid definition at all.
CLASS_OPEN = re.compile(
    r"^[ \t]*(template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)\s*"
    r"(?::[^{;]*)?\{", re.M)

# A single-line member-function declaration carrying a target-VA comment.
# Group 1: declaration text without 'virtual', trailing ';' or comment.
# Group 2: the target VA.
#
# The word boundary is on the plain-identifier branch only. With `\b` in front
# of the whole alternation a destructor could never match -- there is no word
# boundary between a space and '~' -- so for the life of this tool every
# destructor in every header was skipped, which is why the tree did not link.
#
# An access specifier is consumed before the declaration. It is inside the
# declaration's character class (': ' is a legal part of 'Class::method'), so
# without an explicit prefix `public: virtual ~X();` was captured whole and the
# out-of-line definition came out as `public: virtual X::~X()`, which is not a
# declaration at all.
DECL = re.compile(
    r"^[ \t]*(?:(?:public|private|protected)[ \t]*:[ \t]*)?(virtual\s+)?"
    r"((?:[\w:*&<>,\[\]\s]|\bconst\b)*?(?:~\w+|\b\w+)\s*\([^;{}()]*\))"
    r"\s*(const)?\s*;"
    r"(?:[ \t]*//[^\n]*?(0x[0-9a-fA-F]{4,8}))?",
    re.M)

# A vtable-block comment line: "// [3] Next       0x591f40". The hand-written
# headers record the recovered slot addresses in a block above the declarations
# rather than on each declaration, so a declaration like `virtual int
# GetSize();` has no target VA of its own even though the address is right there
# in the file. Reading the block is what turns these from stubs into real
# forwarders.
#
# The name is `\S+` rather than `\w+` so `operator==` is captured whole, and the
# address is optional so a slot the header marks "/* ? */" still records its
# NUMBER -- which is what lets the manifest supply the address instead.
SLOT_VA = re.compile(
    r"^[ \t]*//[ \t]*\[(\d+)\][ \t]+~?(\S+)"
    r"(?:[ \t]+(0x[0-9a-fA-F]{4,8}))?", re.M)

# A destructor declaration with NO target-VA comment.
#
# 227 of the 326 destructor declarations in the reconstructed headers are in
# this state: the header declares the destructor because the compiler needs it
# (a vtable names it, a constructor calls it) but no comment records where it
# is. There is therefore no transcription to forward to. These are emitted as
# explicitly labelled stubs -- not as transcriptions, and not with a guessed VA
# -- because the alternative is a tree that does not link. The count is
# reported in the README rather than hidden.
DTOR_NO_VA = re.compile(
    r"^[ \t]*(?:(?:public|private|protected)[ \t]*:[ \t]*)?(virtual\s+)?"
    r"(~\w+)\s*\(\s*\)\s*;(?![^\n]*0x)", re.M)

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
        if m.group(1):
            continue  # a template: its header defines its members
        depth = src.count("{", 0, m.start()) - src.count("}", 0, m.start())
        opens.append((m.start(), m.end(), m.group(2), depth))
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
        results.append((raw, m.group(3) or "",
                        int(m.group(4), 16) if m.group(4) else None))
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
    # Defensive: the DECL pattern already drops these, but a declaration reached
    # through a different path must not turn `public: virtual` into a return
    # type, which is how `return (virtual)(size_t)...` got emitted.
    for kw in ("public:", "private:", "protected:", "virtual", "explicit",
               "inline", "static"):
        if head.startswith(kw):
            head = head[len(kw):].strip()
    if head.startswith("~"):
        return "", head
    bits = head.rsplit(" ", 1)
    if len(bits) == 1:
        return "", bits[0]
    return bits[0].strip(), bits[1].strip()


def defined_in_code_tree(codedir):
    """The set of VAs the canonical code tree defines.

    A forwarder that names a VA the tree does not define is a link error, so
    the gate checks it against the actual generated files rather than against
    functions.json.
    """
    defined = set()
    if not os.path.isdir(codedir):
        return defined
    pat = re.compile(r"^void (sub_[0-9a-f]{8})\(void\)$", re.M)
    for p in glob.glob(os.path.join(codedir, "code_*.cpp")):
        for m in pat.finditer(open(p).read()):
            defined.add(int(m.group(1)[4:], 16))
    return defined


def main():
    outdir = sys.argv[1] if len(sys.argv) > 1 else OUTDIR
    srcdir = sys.argv[2] if len(sys.argv) > 2 else "src/include"
    gendir = sys.argv[3] if len(sys.argv) > 3 else GENDIR
    codedir = sys.argv[4] if len(sys.argv) > 4 else CODEDIR
    check_only = "--check" in sys.argv

    fns = {f["va"]: f for f in
           json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    # class ident -> vtable slot targets, for declarations whose header records
    # a slot but no address.
    manifest = {}
    try:
        for e in json.load(open(os.path.join(outdir, "class_manifest.json"))):
            manifest[e["ident"]] = e.get("entries") or []
    except (OSError, ValueError, KeyError):
        pass
    defined = defined_in_code_tree(codedir)
    if not defined:
        print("gen_bodies: WARNING no canonical code tree at %s; forwarders "
              "would name symbols nothing defines" % codedir)

    skip = {"no_function_record": 0, "not_in_text": 0, "duplicate": 0,
            "nested_class_covered": 0, "not_in_code_tree": 0}
    emitted = 0
    stubbed = 0
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
            # name -> slot number, and name -> address when the header records
            # one. A declaration with neither falls back to the manifest, whose
            # `entries` are the vtable targets read out of the binary.
            slot_no = {}
            slot_va = {}
            for m in SLOT_VA.finditer(body):
                slot_no.setdefault(m.group(2), int(m.group(1)))
                if m.group(3):
                    slot_va.setdefault(m.group(2), int(m.group(3), 16))
            defs = []
            no_va = []
            for decl, cqual, va in decls_in_body(body):
                ret, name = split_ret_name(decl)
                if not name or not name.lstrip("~").replace("_", "").isalnum():
                    continue
                params = decl[decl.find("("):]
                if va is None:
                    # No trailing VA: fall back to the vtable block, then to the
                    # manifest entry for that slot. The header marks some slots
                    # "/* ? */" and leaves the address out; the manifest has it.
                    bare = name.lstrip("~")
                    va = slot_va.get(bare)
                    if va is None and bare in slot_no:
                        ent = manifest.get(cname.split("::")[-1])
                        if ent and slot_no[bare] < len(ent):
                            va = ent[slot_no[bare]]
                if va is None:
                    no_va.append((ret, name, params, cqual))
                    continue
                if va not in fns:
                    # Distinguish "no function record" from a VA we never saw.
                    skip["no_function_record"] += 1
                    continue
                if not (TEXT_LO <= va < TEXT_HI):
                    skip["not_in_text"] += 1
                    continue
                if va not in defined:
                    skip["not_in_code_tree"] += 1
                    continue
                key = (cname, name, params)
                if key in seen:
                    skip["duplicate"] += 1
                    continue
                seen.add(key)
                defs.append((ret, name, params, cqual, va))
            # Destructors the header declares with no target VA. Skip any the
            # DECL pass already emitted (it matches the ones that do carry one).
            # Destructors the header declares with no address anywhere. These
            # get a labelled stub: the compiler needs the definition (a vtable
            # names it, a constructor calls it) and there is nothing to forward
            # to. Anything with an address has already been handled above.
            have = {name for _, name, _, _, _ in defs}
            stubs = []
            for m in DTOR_NO_VA.finditer(body):
                dname = m.group(2)
                if dname in have:
                    continue
                have.add(dname)
                stubs.append(dname)
            if not defs and not stubs:
                continue
            fname = flat
            files.append((cname, fname, base, defs, stubs))
            emitted += len(defs)
            stubbed += len(stubs)

    if not check_only:
        if not os.path.isdir(gendir):
            os.makedirs(gendir)
        # Regenerate: clear only the .cpp files this tool owns.
        for old in glob.glob(os.path.join(gendir, "*.cpp")):
            os.remove(old)

    # De-duplicate by function VA. An inherited virtual a class does not
    # override is declared by every class that inherits it, so one machine
    # function can have many declaring names. Every name must still get a
    # definition or the tree does not link, so the first declarer carries the
    # forwarder and the rest become aliases that name it.
    canonical = {}
    distinct = 0
    for cname, fname, header, defs, stubs in files:
        lines = []
        lines.append("// %s.cpp -- mechanically generated by tools/gen_bodies.py"
                     % fname)
        lines.append("// DO NOT EDIT: regenerate with ./tools/pipeline.sh.")
        lines.append("//")
        lines.append("// Out-of-line definitions for the member functions this "
                     "class's header")
        lines.append("// declares. Each one FORWARDS to the canonical "
                     "transcription of that")
        lines.append("// machine function, which lives in src/generated/code/ "
                     "as")
        lines.append("// ts::sub_XXXXXXXX(void) and is gated there. The "
                     "transcription is not")
        lines.append("// repeated here: one machine function, one body.")
        lines.append("//")
        lines.append("// The return value is read out of the machine model, "
                     "which is where the")
        lines.append("// callee left it. A body reads its arguments from the "
                     "model too, not from")
        lines.append("// the C++ parameter list -- see tools/gen_code.py.")
        lines.append("//")
        lines.append("// A body marked ALIAS defines the same machine code as "
                     "another")
        lines.append("// declaration; the forwarder is written once, at the "
                     "canonical")
        lines.append("// declaration named on the alias line.")
        lines.append('#include "%s"' % header)
        lines.append('#include "code/recovered.h"   // declarations of the '
                     'canonical')
        lines.append('// transcriptions each body below forwards to.')
        lines.append('#include "Machine.h"   // ts:: machine model, for ts::EAX')
        lines.append("")
        for ret, name, params, cqual, va in defs:
            f = fns[va]
            sig = "ts::sub::sub_%08x" % va
            ret_s = (ret + " ") if ret else ""
            cq = (" " + cqual) if cqual else ""
            returns = ret.strip() not in ("", "void")
            if va in canonical:
                lines.append("// %s::%s -- ALIAS of %s (same machine code at "
                             "0x%08x); the"
                             % (cname, name, canonical[va], va))
                lines.append("// forwarder is written once, at that "
                             "declaration.")
            else:
                canonical[va] = "%s::%s" % (cname, name)
                distinct += 1
                lines.append("// -----------------------------------------------------------"
                             "--------------")
                lines.append("// %s::%s -- the body is the canonical transcription of"
                             % (cname, name))
                lines.append("// %d instructions at 0x%08x, in src/generated/code/."
                             % (f["ninsn"], va))
                lines.append("// size %d bytes; %s" %
                             (f["size"],
                              ("callees: " + ", ".join("0x%08x" % c
                                                       for c in f["callees"]))
                              if f.get("callees") else "callees: none"))
                lines.append("//")
            lines.append("%s%s::%s%s" % (ret_s, cname, name, params + cq))
            lines.append("{")
            lines.append("    %s();" % sig)
            if returns:
                lines.append("    return (%s)(size_t)ts::ret_eax();" % ret.strip())
            lines.append("}")
            lines.append("")
        for dname in stubs:
            lines.append("// %s::%s -- the header declares this destructor but no"
                         % (cname, dname))
            lines.append("// comment names a target VA, so there is no recovered "
                         "function to")
            lines.append("// forward to. This is a STUB, not a transcription: it "
                         "exists so the")
            lines.append("// tree links. Counted in README.md.")
            lines.append("%s::%s()" % (cname, dname))
            lines.append("{")
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
            for cname, fname, header, defs, stubs in files:
                fh.write('#include "%s.cpp"\n' % fname)

    readme = os.path.join(gendir, "README.md")
    if not check_only:
        with open(readme, "w") as fh:
            fh.write("# src/generated -- member definitions and machine code\n\n")
            fh.write("Generated by `tools/gen_bodies.py` and "
                     "`tools/gen_code.py`. Do not edit by hand; regenerate "
                     "with `./tools/pipeline.sh`.\n\n")
            fh.write("## What is in here\n\n")
            fh.write("`code/` holds the **machine code**: one transcription per "
                     "recovered function VA, `ts::sub_XXXXXXXX(void)`, plus a "
                     "registration unit so `ts_call()` reaches it. That is the "
                     "faithful artifact and the one the gate checks.\n\n")
            fh.write("The per-class `.cpp` files here hold **forwarders**: the "
                     "out-of-line definitions the reconstructed headers "
                     "require, each calling the canonical transcription of its "
                     "VA. They are declaration-shaped, so they compile against "
                     "the reconstructed tree; the code is not repeated in "
                     "them.\n\n")
            fh.write("## Coverage\n\n")
            fh.write("| scope | functions |\n|---|---|\n")
            fh.write("| recovered from the binary | %d |\n" % len(fns))
            fh.write("| transcribed in `code/` | %d |\n" % len(defined))
            fh.write("| forwarders emitted (every declaring name) | %d |\n"
                     % emitted)
            fh.write("| distinct machine functions among those | %d |\n"
                     % distinct)
            fh.write("| alias definitions (same machine code) | %d |\n"
                     % (emitted - distinct))
            fh.write("| destructor stubs (declared, no recovered VA) | %d |\n"
                     % stubbed)
            for k in sorted(skip):
                fh.write("| skipped: %s | %d |\n" % (k, skip[k]))
            fh.write("\n")
            if defined:
                fh.write("%.2f%% of the recovered functions have a "
                         "transcription; the rest are the ones\n"
                         "the headers do not declare, and they are transcribed "
                         "in `code/`\nregardless -- see "
                         "`code/` and `tools/gen_code.py`.\n\n"
                         % (100.0 * distinct / len(fns)))
            fh.write("Every transcription is one machine instruction per "
                     "generated line, with the\noriginal instruction kept as a "
                     "trailing comment, so a file can be read against\n"
                     "the recovered listing line for line.\n\n")
            fh.write("This is not source-shaped decompilation. It is not "
                     "claimed to be the shape the\noriginal source had; it is "
                     "claimed to be faithful to what the machine does.\n")

    print("gen_bodies: %d class translation units, %d forwarders, %d DISTINCT "
          "machine functions (%d aliases), %d destructor stubs" %
          (len(files), emitted, distinct, emitted - distinct, stubbed))
    print("skipped: " + json.dumps(skip, sort_keys=True))

    # ------------------------------------------------------------------
    # Gate: every forwarder must name a VA the canonical code tree actually
    # defines. A missing symbol is a link error, and the point of this file is
    # that the tree links.
    # ------------------------------------------------------------------
    bad = []
    checked = 0
    for cname, fname, header, defs, stubs in files:
        path = os.path.join(gendir, fname + ".cpp")
        try:
            text = open(path).read()
        except OSError:
            bad.append("%s.cpp: missing" % fname)
            continue
        for ret, name, params, cqual, va in defs:
            sym = "ts::sub::sub_%08x()" % va
            if sym not in text:
                bad.append("%s::%s: no call to %s" % (cname, name, sym))
                continue
            if va not in defined:
                bad.append("%s::%s: %s is not defined in the code tree"
                           % (cname, name, sym))
                continue
            checked += 1
    if bad:
        print("gen_bodies GATE FAILED for %d definition(s): %s" %
              (len(bad), "; ".join(bad[:5])))
        return 1
    print("gen_bodies gate: %d forwarder(s) verified to name a defined "
          "transcription" % checked)
    return 0


if __name__ == "__main__":
    sys.exit(main())
