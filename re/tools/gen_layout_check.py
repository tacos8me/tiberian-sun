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
What this check does NOT cover, and says so in its output.  Every recovered
offset that is not asserted is counted and printed with a reason, so the check
can never quietly check less than it did the run before:

  * vptr-store: the recovered store writes the vtable pointer.  C++ has no name
    for the vptr -- the compiler inserts it -- so there is nothing to write an
    offsetof against.  The claim is still pinned indirectly: the declaration
    that follows the vptr in the header fixes the byte after it.
  * no-member: the header declares no member at that offset in the class or in
    its primary base chain.  This is the one real gap and it belongs to the
    header emitter, not to this check.
  * beyond: the offset lies past the extent the header declares for the class.
    The constructor recovered under that key initialises an object that embeds
    the class, so the offset is the embedder's member, not this class's (the
    template instantiations whose RTTI names the instantiation are the bulk).
  * no-header: no reconstructed header defines the class at all.
  * no-block: a header defines the class but declares no member offset in it at
    all, so there is nothing to pair a recovered offset with.
  * a `pad_NN` gap filler is not a member claim and is not asserted as one.  It
    is asserted where the recovered table has an offset exactly there -- that is
    where the emitter could not name the store, and the pad's position is then
    the same claim -- and the declaration that follows a pad is what pins the
    gap's end.
  * an empty assertion set is exit 2, not exit 0: a check that asserts nothing
    must not pass vacuously.

COVERAGE
An assertion set that silently shrinks looks exactly like a pass, so the check
fails when the pairing collapses for a class: a class with at least one
recovered non-vptr offset inside the extent its own header declares, and no
assertion at all, is a failure and is named.  The assertion count and the
unasserted count, broken down by reason, are printed on every run.

NAMING
A class is named in the TU by its header's spelling.  Where the RTTI spelling
is not a C++ type -- it leaks the MSVC formal parameter, `BlitPlainXlat<E>`
where the header says `template <class T>` -- the class is named by
instantiating the header's template on a probe type instead, and every such
substitution is reported.  That is sound only because those templates' layout
does not depend on the argument: each of their members is a fixed-width
integer or a pointer.  Where even that fails, the class is dropped and listed.


Exit status:
  0  every asserted offset was confirmed by the compiler
  1  at least one offset was not confirmed, or the assertion set stopped
     covering a class the recovered table has offsets for (the lists are
     printed)
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
# A hand-curated member block writes its offset comment without the '+'
# (`// 0x1D7 INI "TiberiumChainReaction"`) and often puts a parenthesis in the
# same comment, so neither the '+'-only pattern above nor a "skip a line with a
# parenthesis in it" guard finds it.  A claim is only ever used when it equals a
# recovered offset for that class, and every recovered offset is below
# MAX_OFFSET, so a string or function address quoted in a comment cannot be
# mistaken for a member offset.
OFF_COMMENT_CURATED = re.compile(r"//\s*\+?0x([0-9a-fA-F]+)\b")
OFF_IDENT = re.compile(r"(?:m|vptr|pad)_([0-9a-fA-F]{2,4})$")
# The largest recovered member offset.  A claim above it is a quoted address,
# not a member offset; it is what measures how far a header's member block
# reaches, which is what tells a missing member from an embedder's member.
MAX_OFFSET = 0x10000
# How far up the primary base chain to look for a member declaration.  Only the
# FIRST base of each class is followed: MSVC places the first base subobject at
# offset 0 for non-virtual inheritance (no reconstructed header uses virtual
# inheritance), so a base-relative offset is also a derived-relative one.  A
# displaced base's offsets would be wrong for the derived class, so a base claim
# is additionally required to be one of the class's recovered offsets.
PRIMARY_BASE_DEPTH = 6
# The type a template is instantiated on when its RTTI spelling is not a C++
# type.  Every member of those templates is a fixed-width integer or a pointer,
# so their layout does not depend on the argument.
PROBE_TYPE = "layout_probe_t"
# Why a recovered offset has no assertion, in the order the report prints them.
# Every recovered offset is classified into one of these; an offset that fell
# outside them would be a bug in this tool, not a gap in the tree.
REASON_VPTR = "vptr-store (the compiler inserts the vptr; C++ has no name for it)"
REASON_NO_MEMBER = "the header declares no member at this offset"
REASON_BEYOND = ("the offset is past the extent the header declares: the "
                 "constructor recovered under this key initialises an object "
                 "that embeds the class")
REASON_NO_BLOCK = "the header declares no member offset at all"
REASON_NO_HEADER = "no reconstructed header defines the class"
REASONS = (REASON_VPTR, REASON_NO_MEMBER, REASON_BEYOND, REASON_NO_BLOCK,
           REASON_NO_HEADER)
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


def class_bases(src, match):
    """Simple names in the base clause of the class definition at `match`."""
    names = []
    seg = src[match.end(2):src.index("{", match.end(2))]
    for part in seg.split(","):
        part = re.sub(r"^[\s:]+", "", part.strip())
        part = re.sub(r"^(public|private|protected|virtual)\s+", "", part).strip()
        part = part.split("<")[0].strip()
        if part:
            names.append(part.split("::")[-1])
    return names


def find_definition(paths, names):
    """First class definition in `paths` whose name is one of `names`.

    Returns a dict with `path`, `name` (the header's own spelling), `template`
    (the formal parameter list, or None), `is_template`, `body` and `bases`, or
    None.  Matching is on the simple name (after any `::`), because a header may
    define a class under a renamed spelling (ATL::CComObject<class X> is defined
    as ATL_CComObject_class_X) -- the header's own name is then what offsetof
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
                return {"path": p, "name": name, "template": m.group(1),
                        "is_template": m.group(1) is not None,
                        "body": src[span[0]:span[1]],
                        "bases": class_bases(src, m)}
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

    The claim is the `// +0xNN` offset comment a generated member block carries,
    the `// 0xNN` form a hand-curated member block uses, or the offset encoded in
    an `m_NN`/`vptr_NN`/`pad_NN` identifier.  A `pad_NN` gap filler is normally
    not a member claim -- it marks a gap -- but the caller keeps a claim only
    when the recovered table has an offset exactly there, and where it does the
    pad is where the emitter could not name the store, so asking the compiler
    where that byte lands is the same question.

    Whether a line is a declaration at all is decided from its code, never from
    its comment: an identifier has to end it and a type has to precede that
    identifier.  That is what rejects a virtual function declaration, a
    destructor and an access specifier without guessing from the comment -- and
    it is why a comment may contain a parenthesis (the curated blocks quote
    `(0x6BFE14)` and `mov byte ptr [esi + 0x475], bl (0)`) without the line being
    mistaken for a function.
    """
    for line in body.splitlines():
        if line.lstrip().startswith("//"):
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
        cm = OFF_COMMENT.search(line)
        if cm is None:
            cm = OFF_COMMENT_CURATED.search(line)
        if cm is not None:
            yield ident, int(cm.group(1), 16)
            continue
        im = OFF_IDENT.search(ident)
        if im:
            # A `pad_NN` gap filler is included: it is not a member claim, but
            # when the recovered table has an offset exactly there the emitter
            # put the pad where it could not name the store, and asserting where
            # the compiler places that byte is the same question.  The caller
            # drops every claim the recovered table does not have.
            yield ident, int(im.group(1), 16)


def declaration_bodies(srcdir, found):
    """The class body and its primary base chain, nearest first.

    A member a derived constructor initialises is often declared in the base it
    comes from -- the derived header's own member block fills the base subobject
    with `pad_NN` -- so the declarations to match a derived class's recovered
    offsets against are the ones in its base chain too.  See PRIMARY_BASE_DEPTH
    for why only the first base is followed and why the claim still has to be one
    of the derived class's recovered offsets.
    """
    bodies = [found["body"]]
    seen = {found["name"].split("::")[-1]}
    cur = found
    for _ in range(PRIMARY_BASE_DEPTH):
        if not cur["bases"]:
            break
        name = cur["bases"][0]
        if name in seen:
            break
        seen.add(name)
        nxt = find_definition(header_paths(srcdir, name, name), [name])
        if nxt is None:
            break
        bodies.append(nxt["body"])
        cur = nxt
    return bodies


def split_top_level(text):
    """Split on commas that are not inside <>, () or []."""
    parts = []
    depth = 0
    cur = ""
    for ch in text:
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += ch
    parts.append(cur)
    return parts


def probe_spelling(found):
    """`BlitPlainXlat<layout_probe_t>` for the template `found` defines.

    The RTTI spelling of an instantiation created inside another template keeps
    the MSVC formal parameter (`BlitPlainXlat<E>`, `VectorClass<D&*>`), which is
    not a C++ type.  The header's own template is named on a probe type instead.

    Returns None when the template cannot be probed: no formal parameters, a
    template-template parameter, or a formal parameter list this cannot read.
    A probe that does not compile is dropped by the ordinary feedback loop, so a
    wrong guess costs a dropped class, never a wrong assertion.
    """
    if not found["is_template"]:
        return None
    text = found["template"]
    inner = text[text.index("<") + 1:text.rindex(">")]
    params = [p.strip() for p in split_top_level(inner) if p.strip()]
    if not params:
        return None
    args = []
    for p in params:
        if p.startswith("template"):
            return None
        head = p.split("=")[0].strip()
        if re.match(r"(class|typename)\b", head) or re.match(r"^[A-Za-z_]\w*$", head):
            args.append(PROBE_TYPE)
        else:
            args.append("0")
    return "%s<%s>" % (found["name"], ", ".join(args))


def sanitize(s):
    return SYMBOL_OK.sub("_", s)


def build(members, srcdir):
    """Return (assertions, unasserted, probes) for the recovered member table.

    assertions: (symbol, spelling to use in the TU, member, claimed, header,
                 recovered class key).  The two spellings differ when the RTTI
                 name is not a C++ type and the class is named on a probe type;
                 the key is what coverage is counted against, so a renamed class
                 can never look like a class that stopped being checked.
    unasserted: (class_spelling, offset, reason, within the header's extent)
    probes:     {class_spelling: probe spelling or None} for every class that is
                a template instantiation, so a class whose RTTI spelling is not
                a C++ type can be named on a second attempt.
    """
    assertions = []
    unasserted = []
    probes = {}
    for key, rec in sorted(members.items()):
        spelling = key[6:] if key.startswith("class ") else key
        base = spelling.split("<")[0].strip()
        ident = rec.get("ident") or base
        by_off = {r["off"]: r for r in (rec.get("members") or [])
                  if r.get("off") is not None}
        offsets = sorted(by_off)
        if not offsets:
            continue
        found = find_definition(header_paths(srcdir, ident, base),
                                [ident, base, base.split("::")[-1]])
        if not found:
            for off in offsets:
                unasserted.append((spelling, off, REASON_NO_HEADER, False))
            continue
        # A template needs a template-id. Use the RTTI spelling when it names
        # the same template; otherwise fall back to the header's own name.
        if found["is_template"]:
            use = spelling if base == found["name"] else found["name"]
            probes[spelling] = probe_spelling(found)
        else:
            use = found["name"]
        claims = []
        for body in declaration_bodies(srcdir, found):
            claims.extend(declarations(body))
        # How far the header's member block reaches. A claim above the largest
        # recovered offset is a quoted address, not a member offset.
        extent = max([c for _m, c in claims if c <= MAX_OFFSET] or [-1])
        have = set()
        emitted = set()
        for member, claim in claims:
            if claim not in by_off or (member, claim) in emitted:
                continue
            emitted.add((member, claim))
            have.add(claim)
            sym = "chk_%04d_%s_%s_0x%x" % (
                len(assertions), sanitize(use), sanitize(member), claim)
            assertions.append((sym, use, member, claim,
                               os.path.basename(found["path"]), spelling))
        for off in offsets:
            if off in have:
                continue
            # `within` marks the offsets this check could have asserted and did
            # not: a non-vptr offset inside the extent the header's own member
            # block reaches.  Those are what the coverage gate below is about.
            # +0x0 is the vptr slot of a polymorphic class, so an unasserted
            # offset 0 is the vptr even when the table does not flag it: the
            # constructor stored a register there, not a vtable address, and the
            # header's member block starts at the first member after it.
            if by_off[off].get("vptr") or off == 0:
                reason, within = REASON_VPTR, False
            elif not claims:
                reason, within = REASON_NO_BLOCK, False
            elif off > extent:
                reason, within = REASON_BEYOND, False
            else:
                reason, within = REASON_NO_MEMBER, True
            unasserted.append((spelling, off, reason, within))
    return assertions, unasserted, probes


def render(assertions, srcdir, drop, rename=None):
    """Return (text, meta) for the assertion TU, excluding dropped classes.

    `meta` maps a 1-based line number to ("typedef", class) or ("assert",
    index), so a compiler diagnostic can be attributed back to the class or
    the assertion it belongs to.

    `rename` maps a class spelling whose RTTI name is not a C++ type to the
    template-id that names the same template on a probe type.  The typedef then
    uses the probe spelling while every diagnostic still reports the class's
    own name.

    Every header is included, not just the ones the assertions come from: an
    RTTI spelling names template arguments (DynamicVectorClass<BuildingTypeClass*>)
    that the header owning the template has no reason to include itself.
    """
    rename = rename or {}
    keep = [i for i, a in enumerate(assertions) if a[5] not in drop]
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
    if any(cls in rename for cls in cls_typedef):
        add("// A class whose RTTI spelling leaks the MSVC formal template")
        add("// parameter is named by instantiating its template on a probe type.")
        add("struct %s {};" % PROBE_TYPE)
        add("")
    for cls, alias in sorted(cls_typedef.items(), key=lambda kv: kv[1]):
        add("typedef %s %s;" % (rename.get(cls, cls), alias), ("typedef", cls))
    add("")
    add("#ifndef LAYOUT_REPORT_ONLY")
    for i in keep:
        sym, cls, member, claim, _h, _key = assertions[i]
        add("typedef char %s[offsetof(%s, %s) == 0x%x ? 1 : -1];"
            % (sym, cls_typedef[cls], member, claim), ("assert", i))
    add("#else")
    add("#include <cstdio>")
    add("int main() {")
    add('    std::printf("class\\tmember\\tclaimed\\tactual\\n");')
    for i in keep:
        _sym, cls, member, claim, _h, _key = assertions[i]
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

    assertions, unasserted, probes = build(members, args.src)
    total_offsets = sum(len({r["off"] for r in (rec.get("members") or [])
                             if r.get("off") is not None})
                        for rec in members.values())
    if args.emit_only:
        text, _meta = render(assertions, args.src, set())
        open(args.tu, "w").write(text)
        print("layout check TU: %s (%d assertion(s), %d recovered offset(s) "
              "unasserted)" % (args.tu, len(assertions), len(unasserted)))
        return 0

    if not m32_available():
        print("layout check: CANNOT RUN -- g++ cannot compile 32-bit code.")
        print("The game's ABI is 32-bit and the host's LP64 layout would "
              "answer a different")
        print("question, so this check fails closed rather than falling back. "
              "Install")
        print("g++-multilib (Debian/Ubuntu: apt-get install g++-multilib).")
        return 2

    # Attempt 0 names every class by its RTTI spelling. A class the compiler
    # cannot name that way is a template instantiation whose RTTI spelling kept
    # the MSVC formal parameter, so attempt 1 names it on a probe type instead.
    # A class that survives neither is dropped and listed.
    drop = set()
    renamed = {}
    for attempt in (0, 1):
        text, meta = render(assertions, args.src, drop, renamed)
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
            for cls in unnamable:
                if probes.get(cls):
                    renamed[cls] = probes[cls]
                else:
                    drop.add(cls)
            continue
        drop |= unnamable
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
    asserted = [a for a in assertions if a[5] not in drop]
    covered = {(a[5], a[3]) for a in asserted}
    by_reason = {r: 0 for r in REASONS}
    for _cls, _off, reason, _within in unasserted:
        by_reason[reason] = by_reason.get(reason, 0) + 1
    # The coverage block is the first-class record of how much of the recovered
    # table this run actually asked the compiler about. A drop in it is the one
    # failure mode a "confirmed" count cannot show, so it is printed every run
    # and gated below.
    print("[layout] coverage: %d/%d recovered offsets asserted by an offsetof "
          "assertion; %d unasserted"
          % (len(covered), total_offsets, len(unasserted)))
    for reason in REASONS:
        if by_reason.get(reason):
            print("[layout]   unasserted: %-6d %s" % (by_reason[reason], reason))
    if by_reason.get(None):
        print("[layout]   unasserted: %-6d UNCLASSIFIED (a bug in this tool)"
              % by_reason[None])
    # The offsets the header could have declared a member for and did not are
    # the emitter's work list, so they are named, not just counted.
    gaps = [(cls, off) for cls, off, reason, _w in unasserted
            if reason in (REASON_NO_MEMBER, REASON_NO_BLOCK)]
    if gaps:
        print("[layout] %d offset(s) the header could have declared a member "
              "for (the header emitter's work list):" % len(gaps))
        for cls, off in gaps[:args.max_print]:
            print("    %s +0x%x" % (cls, off))
        if len(gaps) > args.max_print:
            print("    ... and %d more (--max-print to see all)"
                  % (len(gaps) - args.max_print))
    # Only the substitutions that were actually needed are reported: a class
    # whose RTTI spelling compiles is left named the way the binary names it.
    named_on_probe = {c: renamed[c] for c in {a[1] for a in asserted}
                      if c in renamed}
    if named_on_probe:
        print("[layout] %d class(es) named on a probe type argument, because "
              "their RTTI" % len(named_on_probe))
        print("[layout] spelling is not a C++ type (it keeps the MSVC formal "
              "template parameter):")
        for cls in sorted(named_on_probe)[:args.max_print]:
            print("    %s -> %s" % (cls, named_on_probe[cls]))
        if len(named_on_probe) > args.max_print:
            print("    ... and %d more (--max-print to see all)"
                  % (len(named_on_probe) - args.max_print))
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

    # ...and the same collapse can be partial. A class whose header does claim
    # an offset the recovered table has inside the extent the header declares,
    # and which contributes no assertion at all, is one whose pairing has
    # collapsed: without this the run would report a handful of confirmations as
    # a pass while most of the tree had quietly stopped being checked.
    collapsed = sorted({cls for cls, _off, _r, within in unasserted
                        if within and cls not in {a[5] for a in asserted}})
    print("[layout] assertions emitted: %d over %d distinct recovered offsets"
          % (len(asserted), len(covered)))
    print("[layout] compiler-confirmed recovered member offsets: %d/%d"
          % (len(asserted) - len(unconfirmed), len(asserted)))
    if collapsed:
        print("[layout] COVERAGE COLLAPSE -- %d class(es) have recovered "
              "offsets inside the extent their header declares and no "
              "assertion at all:" % len(collapsed))
        for c in collapsed[:args.max_print]:
            n = len([1 for cls, _off, _r, within in unasserted
                     if cls == c and within])
            print("    %s (%d unasserted offset(s) inside its declared extent)"
                  % (c, n))
        if len(collapsed) > args.max_print:
            print("    ... and %d more (--max-print to see all)"
                  % (len(collapsed) - args.max_print))
    if unconfirmed:
        print("[layout] NOT confirmed (%d):" % len(unconfirmed))
        for i in sorted(unconfirmed)[:args.max_print]:
            a = assertions[i]
            print("    %s%s: header %s declares %s at +0x%x; the compiler says "
                  "otherwise"
                  % (a[5], "" if a[1] == a[5] else " (named as %s)" % a[1],
                     a[4], a[2], a[3]))
        if len(unconfirmed) > args.max_print:
            print("    ... and %d more" % (len(unconfirmed) - args.max_print))
        print("    (python3 tools/gen_layout_check.py --report prints the "
              "compiler's actual offset for every one)")
    return 0 if not unconfirmed and not collapsed else 1


def selftest():
    """Prove the gate can fail and can pass, on fixtures whose layout is known.

    A gate that has never failed is not a gate, and one that has never passed is
    not proven either.  The fixtures are polymorphic at -m32, so the vptr
    occupies +0x0 and the two members sit at +0x4 and +0x8.  Each case names the
    failure it is meant to catch.
    """
    import tempfile

    def poly(name, first, second):
        return ("struct %s {\n"
                "    virtual ~%s();\n"
                "    unsigned long m_000;  // +0x%03x\n"
                "    unsigned long m_004;  // +0x%03x\n"
                "};\n" % (name, name, first, second))

    def members(*entries):
        return {("class " + cls): {"ident": cls.split("<")[0],
                                   "members": [{"off": o} for o in offs]}
                for cls, offs in entries}

    fixtures = (
        # The compiler's layout is the recovered one: nothing to report.
        ("correct offsets", {"Foo.h": poly("Foo", 4, 8)},
         members(("Foo", [4, 8])), 0, None),
        # Every member is where it is not: the mismatch list has to fail.
        ("displaced offsets", {"Foo.h": poly("Foo", 0, 8)},
         members(("Foo", [0, 8])), 1, None),
        # Foo's header claims +0x10 and +0x14 where the table says +0x4 and +0x8,
        # so Foo contributes no assertion at all while Bar still confirms two: a
        # run that reported "2/2 confirmed" would be a pass over a tree that had
        # stopped being checked.  The coverage gate has to fail it and name it.
        ("coverage collapse",
         {"Foo.h": poly("Foo", 0x10, 0x14), "Bar.h": poly("Bar", 4, 8)},
         members(("Foo", [4, 8]), ("Bar", [4, 8])), 1, "COVERAGE COLLAPSE"),
        # The RTTI spelling of an instantiation made inside a template keeps the
        # MSVC formal parameter, so `Box<E>` is not a C++ type.  Naming the
        # header's template on a probe type is what makes it nameable.
        ("probe-named template",
         {"Box.h": ("template <class T>\n"
                    "struct Box {\n"
                    "    virtual ~Box();\n"
                    "    T *m_004;  // +0x004\n"
                    "    T *m_008;  // +0x008\n"
                    "};\n")},
         members(("Box<E>", [4, 8])), 0, None),
    )
    ok = True
    for label, headers, table, want, want_text in fixtures:
        d = tempfile.mkdtemp(prefix="layout_selftest_")
        src = os.path.join(d, "include")
        os.makedirs(src)
        for name, text in headers.items():
            with open(os.path.join(src, name), "w") as f:
                f.write(text)
        with open(os.path.join(d, "members.json"), "w") as f:
            json.dump(table, f)
        r = subprocess.run([sys.executable, os.path.abspath(__file__),
                            "--src", src, "--shim", src,
                            "--members", os.path.join(d, "members.json"),
                            "--tu", os.path.join(d, "tu.cpp")],
                           capture_output=True)
        got = r.returncode
        out = r.stdout.decode(errors="replace")
        bad = got != want or (want_text and want_text not in out)
        print("selftest: %-18s -> exit %d (want %d) %s"
              % (label, got, want, "OK" if not bad else "WRONG"))
        if bad:
            sys.stdout.write(out)
            ok = False
    print("gen_layout_check selftest: %s" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    if "--selftest" in sys.argv[1:]:
        sys.exit(selftest())
    sys.exit(main())
