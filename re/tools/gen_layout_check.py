#!/usr/bin/env python3
"""Make the compiler the authority on the recovered member layout.

Every offset in out/members.json is a claim about where a member sits in the
game's objects. Nothing checked those claims against C++. The headers are
generated from the same table, so they agree with it by construction, and
verify.py's check 7 only compares emitted offsets against the table they came
from. This tool asks the one authority that cannot be talked into agreeing:
the compiler.

It emits a C++03 translation unit with one assertion per recovered offset for
which a reconstructed header declares a member:

    typedef char chk_0001_AbstractClass_m_000_0x0
        [offsetof(AbstractClass, m_000) == 0x0 ? 1 : -1];

C++03 has no static_assert, so a failing assertion is a char array of negative
length: g++ rejects it and names the symbol. The symbol carries the class, the
member and the claimed offset, so the mismatch list is readable directly from
the compiler's diagnostics.

The classes have vtables and are therefore not POD. offsetof is only
"conditionally-supported" on them and g++ says so under -Winvalid-offsetof;
the warning is suppressed because the question asked here -- where does the
compiler actually put this member -- is exactly what offsetof answers, POD or
not. There is no alternative: the classes are the ones the binary has.

The TU is compiled with -m32 because the game's ABI is 32-bit. A host-ABI
(LP64) compile would answer a different question, which is the defect this
check exists to stop hiding.
What this check does NOT cover, and says so in its output:

  * 718 recovered offsets have no member declaration in any header to assert
    against, and a further ~180 classes could not be named in the TU at all
    (their RTTI spelling leaks the MSVC formal template parameter -- the
    binary says `BlitPlainXlat<E>`, the header says `template <class T>` -- or
    names a type no reconstructed header declares). Both counts are printed,
    never silently skipped, and the dropped classes are listed by name.
  * a `pad_NN` gap filler is not a member claim and is not asserted.
  * an empty assertion set is exit 2, not exit 0: a check that asserts nothing
    must not pass vacuously.


Exit status:
  0  every asserted offset was confirmed by the compiler
  1  at least one offset was not confirmed (the mismatch list is printed)
  2  the check could not run: missing artifact, or no 32-bit toolchain

Usage:
  python3 tools/gen_layout_check.py [--src DIR] [--shim DIR]
                                    [--members out/members.json]
                                    [--tu build/layout_check.cpp]
                                    [--emit-only] [--report] [--max-print N]

  --emit-only  write the TU and stop (no compiler needed)
  --report     compile the TU with -DLAYOUT_REPORT_ONLY and RUN it, printing
               the compiler's actual offset for every asserted member next to
               the recovered one. Diagnostics only; the assertions are skipped
               so the program can link and run.
"""

import argparse
import glob
import json
import os
import re
import subprocess
import sys

# A failed assertion is reported as "size ... of array 'chk_NN_...' is
# negative" (g++ 13 quotes the symbol with U+2018/U+2019). Only the wording
# "is negative" is matched; the symbol is taken from the line's position in
# the TU, which is exact, rather than from the message.

# A class definition in a header, with its optional template header.
DEF_RE = re.compile(
    r"^[ \t]*(template\s*<[^;{}]*>\s*)?(?:class|struct)\s+([\w:]+)\b[^;{}]*\{",
    re.M)

# The identifier of a data declaration: the last identifier before the
# terminating ';', an array bound, a bitfield width or an initialiser.
DECL_RE = re.compile(
    r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*(?::\s*\d+)?\s*(?:=[^;]*)?;\s*$")

OFF_COMMENT = re.compile(r"//[^\n]*?\+0x([0-9a-fA-F]+)")
OFF_IDENT = re.compile(r"(?:m|vptr)_([0-9a-fA-F]{2,4})$")

SYMBOL_OK = re.compile(r"[^A-Za-z0-9_]")


def m32_available():
    """True when g++ can compile and link for the target ABI."""
    r = subprocess.run(["g++", "-m32", "-x", "c++", "-", "-o", os.devnull],
                       input=b"int main(){return 0;}\n", capture_output=True)
    return r.returncode == 0


def body_span(src, start):
    """Character span of the braced body whose opening brace is at `start`."""
    i = src.index("{", start)
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


def find_definition(paths, names):
    """First class definition in `paths` whose name is one of `names`.

    Returns (path, defined_name, is_template, body_text) or None. Matching is
    on the simple name (after any `::`), because a header may define a class
    under a renamed spelling (ATL::CComObject<class X> is defined as
    ATL_CComObject_class_X) -- the header's own name is then what offsetof
    must name.
    """
    for p in paths:
        src = open(p).read()
        for m in DEF_RE.finditer(src):
            name = m.group(2)
            simple = name.split("::")[-1]
            if name in names or simple in names:
                span = body_span(src, m.start())
                if span is None:
                    continue
                return (p, name, m.group(1) is not None,
                        src[span[0]:span[1]])
    return None


def header_paths(srcdir, ident, base):
    """Candidate headers for a class, most specific first."""
    cands = []
    for n in (ident, base.split("::")[-1]):
        p = os.path.join(srcdir, n + ".h")
        if os.path.exists(p):
            cands.append(p)
    rest = sorted(p for p in glob.glob(os.path.join(srcdir, "*.h"))
                  if p not in cands)
    return cands + rest


def declarations(body):
    """Yield (member_ident, claimed_offset) for declarations that claim one.

    The claim is the `// +0xNN` offset comment the header carries, or the
    offset encoded in an `m_NN`/`vptr_NN` identifier. `pad_NN` is excluded: it
    marks a gap between members rather than claiming to be one.
    """
    for line in body.splitlines():
        if line.lstrip().startswith("//") or "(" in line:
            continue
        code = line.split("//")[0]
        if ";" not in code:
            continue
        if re.match(r"\s*(typedef|using|enum|friend|static_assert)\b", code):
            continue
        m = DECL_RE.search(code)
        if not m:
            continue
        ident = m.group(1)
        # A declaration needs a type before the identifier; this rejects an
        # access specifier and a bare label.
        if not re.search(r"[A-Za-z_*&]\s*$", code[:m.start()]):
            continue
        claim = None
        cm = OFF_COMMENT.search(line)
        if cm:
            claim = int(cm.group(1), 16)
        else:
            im = OFF_IDENT.search(ident)
            if im and not ident.startswith("pad_"):
                claim = int(im.group(1), 16)
        if claim is not None:
            yield ident, claim


def sanitize(s):
    return SYMBOL_OK.sub("_", s)


def build(members, srcdir):
    """Return (assertions, skipped) for the recovered member table."""
    assertions = []          # (symbol, class_spelling, member, claimed, header)
    skipped = []             # (class, offset, reason)
    for key, rec in sorted(members.items()):
        spelling = key[6:] if key.startswith("class ") else key
        base = spelling.split("<")[0].strip()
        ident = rec.get("ident") or base
        offsets = sorted({r["off"] for r in (rec.get("members") or [])
                          if r.get("off") is not None})
        if not offsets:
            continue
        found = find_definition(header_paths(srcdir, ident, base),
                                [ident, base, base.split("::")[-1]])
        if not found:
            for off in offsets:
                skipped.append((spelling, off, "no header defines the class"))
            continue
        path, defined, is_template, body = found
        # A template needs a template-id. Use the RTTI spelling when it names
        # the same template; otherwise fall back to the header's own name.
        if is_template:
            if base == defined:
                use = spelling
            else:
                use = defined
        else:
            use = defined
        have = set()
        for member, claim in declarations(body):
            if claim not in offsets:
                continue
            have.add(claim)
            sym = "chk_%04d_%s_%s_0x%x" % (
                len(assertions), sanitize(use), sanitize(member), claim)
            assertions.append((sym, use, member, claim,
                               os.path.basename(path)))
        for off in offsets:
            if off not in have:
                skipped.append((spelling, off,
                                "header declares no member at this offset"))
    return assertions, skipped


def render(assertions, srcdir, drop):
    """Return (text, meta) for the assertion TU, excluding dropped classes.

    `meta` maps a 1-based line number to ("typedef", class) or ("assert",
    index), so a compiler diagnostic can be attributed back to the class or
    the assertion it belongs to.

    Every header is included, not just the ones the assertions come from: an
    RTTI spelling names template arguments (DynamicVectorClass<BuildingTypeClass*>)
    that the header owning the template has no reason to include itself.
    """
    keep = [i for i, a in enumerate(assertions) if a[1] not in drop]
    cls_typedef = {}
    for i in keep:
        cls = assertions[i][1]
        if cls not in cls_typedef:
            cls_typedef[cls] = "layout_cls_%04d" % len(cls_typedef)
    lines = []
    meta = {}

    def add(s, m=None):
        lines.append(s)
        if m is not None:
            meta[len(lines)] = m

    add("// Generated by tools/gen_layout_check.py -- do not edit.")
    add("// One offsetof assertion per recovered member offset that a")
    add("// reconstructed header declares a member for. Compiled at -m32; a")
    add("// negative array length means the compiler put the member somewhere")
    add("// other than the recovered offset.")
    add("#include <cstddef>")
    for p in sorted(glob.glob(os.path.join(srcdir, "*.h"))):
        if os.path.basename(p) != "ts_prelude.h":
            add('#include "%s"' % os.path.basename(p))
    add("")
    for cls, alias in sorted(cls_typedef.items(), key=lambda kv: kv[1]):
        add("typedef %s %s;" % (cls, alias), ("typedef", cls))
    add("")
    add("#ifndef LAYOUT_REPORT_ONLY")
    for i in keep:
        sym, cls, member, claim, _h = assertions[i]
        add("typedef char %s[offsetof(%s, %s) == 0x%x ? 1 : -1];"
            % (sym, cls_typedef[cls], member, claim), ("assert", i))
    add("#else")
    add("#include <cstdio>")
    add("int main() {")
    add('    std::printf("class\\tmember\\tclaimed\\tactual\\n");')
    for i in keep:
        _sym, cls, member, claim, _h = assertions[i]
        add('    std::printf("%s\\t%s\\t0x%x\\t0x%%x\\n", '
            '(unsigned)offsetof(%s, %s));'
            % (cls, member, claim, cls_typedef[cls], member))
    add("    return 0;")
    add("}")
    add("#endif")
    add("")
    return "\n".join(lines) + "\n", meta


# The two flags this check needs, in one place so the gate and the report
# cannot drift apart:
#   -Wno-invalid-offsetof  the classes have vtables, so offsetof on them is
#                          "conditionally supported"; the question here is
#                          where the compiler puts the member, POD or not
#   -fno-access-control    offsetof is access-checked, and a private member
#                          says nothing about layout; without this, 80
#                          assertions fail for encapsulation, not for layout
GXX_FLAGS = ["-m32", "-std=c++03", "-Wno-invalid-offsetof",
             "-fno-access-control"]


def compile_tu(tu_path, srcdir, shim, extra=()):
    cmd = ["g++", "-fsyntax-only"] + GXX_FLAGS + [
        "-I" + srcdir, "-I" + shim]
    cmd += list(extra)
    cmd.append(tu_path)
    r = subprocess.run(cmd, capture_output=True)
    return r.returncode, r.stderr.decode("utf-8", "replace"), cmd


def diagnostics(err, tu_path):
    """(line, message) for every error the compiler reported in the TU."""
    pat = re.compile(re.escape(tu_path) + r":(\d+):(?:\d+:)? error: (.*)")
    for ln in err.splitlines():
        m = pat.match(ln)
        if m:
            yield int(m.group(1)), m.group(2)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--src", default="src/include")
    ap.add_argument("--shim", default="build/shim")
    ap.add_argument("--members", default="out/members.json")
    ap.add_argument("--tu", default="build/layout_check.cpp")
    ap.add_argument("--emit-only", action="store_true")
    ap.add_argument("--report", action="store_true")
    ap.add_argument("--max-print", type=int, default=10)
    args = ap.parse_args()

    if not os.path.exists(args.members):
        print("layout check: CANNOT RUN -- %s missing (stage 6)" % args.members)
        print("Run ./tools/pipeline.sh to regenerate.")
        return 2
    members = json.load(open(args.members))

    assertions, skipped = build(members, args.src)
    if args.emit_only:
        text, _meta = render(assertions, args.src, set())
        open(args.tu, "w").write(text)
        print("layout check TU: %s (%d assertion(s), %d recovered offset(s) "
              "unasserted)" % (args.tu, len(assertions), len(skipped)))
        return 0

    if not m32_available():
        print("layout check: CANNOT RUN -- g++ cannot compile 32-bit code.")
        print("The game's ABI is 32-bit and the host's LP64 layout would "
              "answer a different")
        print("question, so this check fails closed rather than falling back. "
              "Install")
        print("g++-multilib (Debian/Ubuntu: apt-get install g++-multilib).")
        return 2

    drop = set()
    for attempt in (0, 1):
        text, meta = render(assertions, args.src, drop)
        open(args.tu, "w").write(text)
        rc, err, cmd = compile_tu(args.tu, args.src, args.shim)
        unconfirmed = {}
        unnamable = set()
        other = []
        for ln, msg in diagnostics(err, args.tu):
            kind = meta.get(ln)
            if kind is None:
                other.append((ln, msg))
            elif "is negative" in msg and kind[0] == "assert":
                unconfirmed[kind[1]] = msg
            elif kind[0] == "typedef":
                unnamable.add(kind[1])
            else:
                cls = assertions[kind[1]][1]
                if "does not name a type" in msg or "is invalid" in msg:
                    unnamable.add(cls)
                else:
                    unconfirmed[kind[1]] = msg
        if unnamable and attempt == 0:
            drop |= unnamable
            continue
        break

    if args.report:
        # The assertions are skipped (-DLAYOUT_REPORT_ONLY) so the program can
        # compile and run; its point is the compiler's actual offset for every
        # asserted member, next to the recovered one.
        exe = os.path.join(os.path.dirname(args.tu) or ".", "layout_report")
        r = subprocess.run(["g++", "-DLAYOUT_REPORT_ONLY"] + GXX_FLAGS +
                           ["-I" + args.src, "-I" + args.shim, "-o", exe,
                            args.tu], capture_output=True)
        if r.returncode != 0:
            sys.stderr.write(r.stderr.decode("utf-8", "replace"))
            return 2
        subprocess.run([exe])
        return 0

    print("$ " + " ".join(cmd))
    asserted = [a for a in assertions if a[1] not in drop]
    print("[layout] assertions emitted: %d (%d recovered offsets have no "
          "header declaration)" % (len(asserted), len(skipped)))
    if drop:
        print("[layout] %d class(es) could not be named in the TU, so their "
              "assertions were dropped" % len(drop))
        for c in sorted(drop)[:args.max_print]:
            print("    %s" % c)
        if len(drop) > args.max_print:
            print("    ... and %d more (--max-print to see all)"
                  % (len(drop) - args.max_print))
    if other:
        print("[layout] CANNOT RUN -- the TU failed to compile for a reason "
              "that is not an assertion:")
        for ln, msg in other[:args.max_print]:
            print("    %s:%d: %s" % (args.tu, ln, msg))
        return 2

    # A check that asserts nothing passes vacuously. If a generator change
    # stops the headers from carrying an offset comment or an m_NN name, the
    # pairing collapses and every offset silently stops being checked -- so an
    # empty assertion set is a failure, not a pass.
    if not asserted:
        print("[layout] CANNOT RUN -- no assertion was emitted: no header "
              "declares a member at any recovered offset.")
        return 2

    print("[layout] compiler-confirmed recovered member offsets: %d/%d"
          % (len(asserted) - len(unconfirmed), len(asserted)))
    if unconfirmed:
        print("[layout] NOT confirmed (%d):" % len(unconfirmed))
        for i in sorted(unconfirmed)[:args.max_print]:
            a = assertions[i]
            print("    %s: header %s declares %s at +0x%x; the compiler says "
                  "otherwise" % (a[1], a[4], a[2], a[3]))
        if len(unconfirmed) > args.max_print:
            print("    ... and %d more" % (len(unconfirmed) - args.max_print))
        print("    (python3 tools/gen_layout_check.py --report prints the "
              "compiler's actual offset for every one)")
    return 0 if not unconfirmed else 1


def selftest():
    """Prove the gate can fail and can pass, on a fixture whose layout is known.

    A gate that has never failed is not a gate, and one that has never passed
    is not proven either. The fixture is a polymorphic class at -m32, so the
    vptr occupies +0x0 and the two members sit at +0x4 and +0x8.
    """
    import tempfile
    ok = True
    for label, offsets, want in (("correct offsets", [4, 8], 0),
                                 ("displaced offsets", [0, 8], 1)):
        d = tempfile.mkdtemp(prefix="layout_selftest_")
        src = os.path.join(d, "include")
        os.makedirs(src)
        with open(os.path.join(src, "Foo.h"), "w") as f:
            f.write("struct Foo {\n"
                    "    virtual ~Foo();\n"
                    "    unsigned long m_000;  // +0x%03x\n"
                    "    unsigned long m_004;  // +0x%03x\n"
                    "};\n" % (offsets[0], offsets[1]))
        with open(os.path.join(d, "members.json"), "w") as f:
            json.dump({"class Foo": {"ident": "Foo", "members": [
                {"off": offsets[0]}, {"off": offsets[1]}]}}, f)
        r = subprocess.run([sys.executable, os.path.abspath(__file__),
                            "--src", src, "--shim", src,
                            "--members", os.path.join(d, "members.json"),
                            "--tu", os.path.join(d, "tu.cpp")],
                           capture_output=True)
        got = r.returncode
        print("selftest: %-18s -> exit %d (want %d) %s"
              % (label, got, want, "OK" if got == want else "WRONG"))
        if got != want:
            sys.stdout.write(r.stdout.decode(errors="replace"))
            ok = False
    print("gen_layout_check selftest: %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    if "--selftest" in sys.argv[1:]:
        sys.exit(selftest())
    sys.exit(main())
