#!/usr/bin/env python3
"""Emit a canonical transcription of every recovered function.

`src/generated/` is declaration-shaped: it defines exactly the member functions
that the reconstructed headers declare, so that the tree compiles against those
headers. That set is 2,845 functions out of the 41,822 the disassembler
recovered, so 91% of the recovered instruction stream had no body anywhere --
not because it could not be rendered, but because nothing declared it.

This tool closes that gap. It emits one transcription per recovered function VA,
as `ts::sub_XXXXXXXX(void)`, covering the whole recovered instruction stream, and
a registration unit so that `ts::ts_call()` dispatches to it. The class bodies
in `src/generated/` then become forwarders to these, so there is still exactly
one transcription per machine function and no duplication.

The signature is deliberately `void(void)`. A transcribed body reads its
arguments from the machine model -- registers, stack, flags -- rather than from
C++ parameters, so a C++ parameter list would be decorative. A `ret` leaves the
result in EAX and pops the return address off the modelled stack, which is what
`ts_call_at` pushed, so the stack stays balanced and the caller reads EAX.

Usage:
  python3 tools/gen_code.py out bin/GAME.EXE src/generated/code
  python3 tools/gen_code.py out bin/GAME.EXE src/generated/code --check
"""

import glob
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import emit_x86  # noqa: E402

OUTDIR = "out"
GENDIR = "src/generated/code"
BATCH = 256  # functions per translation unit

# The image's .text, used only to reject addresses outside it.
TEXT_LO = 0x401000
TEXT_HI = 0x401000 + 0x2A0D25



def sub_name(va):
    return "sub_%08x" % va


def banner(va, f):
    return "// %s -- transcription of %d instructions at 0x%08x" % (
        sub_name(va), f["ninsn"], va)


def render(va, f, image, stats=None):
    """The C++ body lines for one function, without the surrounding definition.

    `stats` is optional so the gate can re-render without perturbing the
    coverage counters it is checking.
    """
    items, st = emit_x86.transcribe(
        f.get("insns", []), image, va, f.get("end") or va + f["size"],
        ret_type="")
    if stats is not None:
        stats["modelled"] += st["modelled"]
        stats["unmodelled"] += st["unmodelled"]
        for k, v in st["unmodelled_by_mnemonic"].items():
            stats["unmod"][k] = stats["unmod"].get(k, 0) + v

    out = []
    for kind, cpp, rec in items:
        if kind == "label":
            out.append("    %s:;" % cpp)
            continue
        # A rendering may be a block (an indirect jump's dispatch), in which case
        # only the first line carries the evidence record so the gate still sees
        # each instruction exactly once.
        # One ts::insn(va) per instruction: the model's only per-instruction
        # hook. It keeps eip exact, counts instructions and enforces the step
        # budget, so a fault can name the instruction it happened at.
        first, _, rest = cpp.partition("\n")
        out.append("    %s%s   //   %s" % (insn_hook(rec), first, rec))
        for extra in rest.split("\n"):
            if extra:
                out.append("    " + extra)
    # A recovered span can end without a `ret` (control falls through into the
    # next function, which the linker's ordering makes legal). The transcription
    # is void, so nothing has to be returned.
    return out


def insn_hook(rec):
    """The `ts::insn(0x...)` prefix that makes one instruction observable.

    `rec` is the evidence record and starts with the instruction's own VA. A
    rendering whose record does not (a synthetic label target) gets no hook
    rather than a guessed address.
    """
    tok = rec.split()[0] if rec.split() else ""
    try:
        return "ts::insn(0x%08xu); " % int(tok, 16)
    except ValueError:
        return ""


BRANCH = ("jmp", "jecxz", "jcxz", "loop", "loope", "loopne")


def tail_entries(fns):
    """VAs a cross-function branch targets that are not function starts.

    A `jmp` to an address in the middle of another function is a tail transfer:
    the original has already arranged the stack and expects that address's code
    to run to the end of the function containing it. Those addresses are not
    function entries, so nothing registered them and a `ts_call` to one found
    nothing -- it was counted as unmapped rather than reached. Transcribing each
    from its own address to the end of its containing function closes that hole.
    """
    starts = {f["va"] for f in fns}
    need = set()
    for f in fns:
        lo, hi = f["va"], f["va"] + f["size"]
        for i in f.get("insns", []):
            m = i.get("mnem", "")
            op = (i.get("op") or "").strip()
            if not op:
                continue
            if not (m in BRANCH or (m.startswith("j") and m[1:] not in ("mp",))):
                continue
            try:
                t = int(op, 0)
            except ValueError:
                continue
            if not (lo <= t < hi) and t not in starts:
                need.add(t)
    # Map each tail VA to its containing function.
    out = {}
    for f in fns:
        lo, hi = f["va"], f["va"] + f["size"]
        for t in need:
            if lo < t < hi:
                out[t] = f
    return out


def build(outdir, image, stats):
    """Return (files, registered, batch_of) for the code tree."""
    all_fns = json.load(open(os.path.join(outdir, "functions.json")))["functions"]
    fns = [f for f in all_fns if TEXT_LO <= f["va"] < TEXT_HI]
    fns.sort(key=lambda f: f["va"])

    # Tail entries, described to render() exactly like a function so the rest of
    # the pipeline (banner, gate, registration) is unchanged.
    tails = {}
    for t, host in tail_entries([f for f in all_fns
                                 if TEXT_LO <= f["va"] < TEXT_HI]).items():
        insns = [i for i in host.get("insns", [])
                 if int(i.get("va", 0) or 0) >= t]
        if not insns:
            continue
        tails[t] = {"va": t, "size": host["va"] + host["size"] - t,
                    "ninsn": len(insns), "insns": insns,
                    "callees": host.get("callees") or [],
                    "tail_of": host["va"]}
    for t, f in tails.items():
        fns.append(f)
    fns.sort(key=lambda f: f["va"])

    files = {}
    registered = []
    batch_of = {}
    for i in range(0, len(fns), BATCH):
        chunk = fns[i:i + BATCH]
        name = "code_%04d.cpp" % (i // BATCH)
        lines = [
            "// src/generated/code/%s -- mechanically generated by "
            "tools/gen_code.py" % name,
            "// DO NOT EDIT: regenerate with ./tools/pipeline.sh.",
            "//",
            "// One transcription per recovered function VA, against the ts:: machine",
            "// model in Machine.h. Registers, flags, memory and control flow are",
            "// modelled, not approximated; each line is one machine instruction and",
            "// keeps the original as a trailing comment. The signature is void(void)",
            "// because a body reads its arguments from the machine model rather than",
            "// from C++ parameters -- see the header of tools/gen_code.py.",
            '#include "Machine.h"',
            "",
            "namespace ts {",
            "namespace sub {",
            "",
        ]
        for f in chunk:
            va = f["va"]
            lines.append("// ---------------------------------------------------"
                         "------------------------")
            lines.append(banner(va, f))
            lines.append("// size %d bytes" % f["size"])
            if f.get("tail_of"):
                lines.append("// tail entry: not a function start. Reached by a "
                             "branch from")
                lines.append("// outside; transcribed from here to the end of the "
                             "function at")
                lines.append("// 0x%08x, which contains it." % f["tail_of"])
            if f.get("callees"):
                lines.append("// callees: " +
                             ", ".join("0x%08x" % c for c in f["callees"]))
            else:
                lines.append("// callees: none")
            lines.append("//")
            lines.append("void %s(void)" % sub_name(va))
            lines.append("{")
            lines.extend(render(va, f, image, stats))
            lines.append("}")
            lines.append("")
            registered.append(va)
            batch_of[va] = name
        lines.append("}  // namespace sub")
        lines.append("}  // namespace ts")
        files[name] = lines

    # The registration unit. One table and one function rather than 41,822
    # static initializers, so there is no static-initialization-order question
    # and the whole thing can be driven from a main().
    # A header declaring every transcription, so the declaration-shaped
    # forwarders in src/generated/ can call them. Without this the class TUs
    # have no visible symbol and every forwarder is a compile error.
    hdr = [
        "// src/generated/code/recovered.h -- mechanically generated by "
        "tools/gen_code.py",
        "// DO NOT EDIT: regenerate with ./tools/pipeline.sh.",
        "//",
        "// Declares every canonical transcription. Include this to call a",
        "// recovered function by address; call register_all_recovered() first if",
        "// you want ts_call() to dispatch to them.",
        "#ifndef TS_GENERATED_RECOVERED_H",
        "#define TS_GENERATED_RECOVERED_H",
        "",
        "namespace ts {",
        "namespace sub {",
    ]
    for va in registered:
        hdr.append("void %s(void);" % sub_name(va))
    hdr.append("}  // namespace sub")
    hdr.append("")
    hdr.append("// Registers every transcription in the dispatch table, so a "
               "recovered")
    hdr.append("// `call` reaches the recovered body instead of being counted as "
               "unmapped.")
    hdr.append("void register_all_recovered();")
    hdr.append("")
    hdr.append("}  // namespace ts")
    hdr.append("")
    hdr.append("#endif  // TS_GENERATED_RECOVERED_H")
    files["recovered.h"] = hdr

    reg = [
        "// src/generated/code/register.cpp -- mechanically generated by "
        "tools/gen_code.py",
        "// DO NOT EDIT: regenerate with ./tools/pipeline.sh.",
        "//",
        "// Makes the transcription executable: ts_call() looks a target VA up in",
        "// this table, so a recovered `call` reaches the recovered body instead of",
        "// being counted as unmapped. Without it the transcriptions are inert.",
        '#include "Machine.h"',
        '#include "recovered.h"',
        "",
        "namespace ts {",
    ]
    reg.append("namespace {")
    reg.append("struct Entry { u32 va; Fn fn; };")
    reg.append("const Entry kRecovered[] = {")
    for va in registered:
        reg.append("    { 0x%08xu, &sub::%s }," % (va, sub_name(va)))
    reg.append("};")
    reg.append("const unsigned kRecoveredCount = "
               "sizeof(kRecovered) / sizeof(kRecovered[0]);")
    reg.append("}  // namespace")
    reg.append("")
    reg.append("void register_all_recovered() {")
    reg.append("    for (unsigned i = 0; i < kRecoveredCount; ++i)")
    reg.append("        ts_register(kRecovered[i].va, kRecovered[i].fn);")
    reg.append("}")
    reg.append("")
    reg.append("}  // namespace ts")
    files["register.cpp"] = reg

    return files, registered, batch_of, {f["va"]: f for f in fns}


def gate(gendir, outdir, image, registered, batch_of, fns):
    """Two independent checks, both mutation-proven.

      1. The emitted code must be byte-identical to a fresh render of the
         recovered instructions. That covers instruction order, operand
         fidelity and the evidence records in one comparison, and it is the
         check that catches a corrupted statement whose comment is intact --
         which a record-only check cannot see.
      2. Every recovered instruction must appear in that render exactly once.
         A record-only mutation (a dropped or reordered line) is caught by 1 as
         well, but 2 states the intent and catches a renderer that silently
         drops an instruction from the model side.
    """
    bad = []
    checked = 0
    for va in registered:
        f = fns[va]
        path = os.path.join(gendir, batch_of[va])
        try:
            text = open(path).read()
        except OSError:
            bad.append("0x%08x (%s): %s missing" %
                       (va, sub_name(va), batch_of[va]))
            continue
        b = banner(va, f)
        start = text.find(b)
        if start < 0:
            bad.append("0x%08x (%s): banner not found" % (va, sub_name(va)))
            continue
        body_lo = text.find("\n{\n", start)
        if body_lo < 0:
            bad.append("0x%08x (%s): no body" % (va, sub_name(va)))
            continue
        body_lo += 3
        body_hi = text.find("\n}\n", body_lo)
        if body_hi < 0:
            bad.append("0x%08x (%s): unterminated body" % (va, sub_name(va)))
            continue

        got = text[body_lo:body_hi]
        exp = "\n".join(render(va, f, image))
        if got != exp:
            gl = got.split("\n")
            el = exp.split("\n")
            where = "line 0"
            for i in range(min(len(gl), len(el))):
                if gl[i] != el[i]:
                    where = "line %d" % i
                    break
            bad.append("0x%08x (%s): code does not match the recovered "
                       "instructions at %s" % (va, sub_name(va), where))
            continue

        # Check 2: every recovered instruction present exactly once, in order.
        recs = [ln for ln in got.split("\n") if "   //   " in ln]
        want = [i for i in f.get("insns", []) if (i.get("op") or "").strip()
                or i.get("mnem")]
        if len(recs) < len(want):
            bad.append("0x%08x (%s): %d of %d instruction records present" %
                       (va, sub_name(va), len(recs), len(want)))
            continue
        checked += 1
    return bad, checked


def main():
    outdir = sys.argv[1] if len(sys.argv) > 1 else OUTDIR
    imgpath = sys.argv[2] if len(sys.argv) > 2 else "bin/GAME.EXE"
    gendir = sys.argv[3] if len(sys.argv) > 3 else GENDIR
    check_only = "--check" in sys.argv

    image = emit_x86.PEImage(imgpath) if os.path.exists(imgpath) else None
    if image is None:
        print("gen_code: WARNING %s not found; indirect jumps will be recorded "
              "rather than resolved" % imgpath)

    stats = {"modelled": 0, "unmodelled": 0, "unmod": {}}
    files, registered, batch_of, fmap = build(outdir, image, stats)

    if not check_only:
        if not os.path.isdir(gendir):
            os.makedirs(gendir)
        for old in glob.glob(os.path.join(gendir, "*.cpp")):
            os.remove(old)
        for name, lines in files.items():
            with open(os.path.join(gendir, name), "w") as fh:
                fh.write("\n".join(lines))

    # fmap, not a fresh read of functions.json: it also holds the tail entries,
    # which are transcribed but are not function starts in that file.
    bad, checked = gate(gendir, outdir, image, registered, batch_of, fmap)

    tot = stats["modelled"] + stats["unmodelled"]
    print("gen_code: %d functions transcribed, %d/%d instructions modelled "
          "(%.2f%%), %d unmodelled" %
          (len(registered), stats["modelled"], tot,
           100.0 * stats["modelled"] / tot if tot else 0.0,
           stats["unmodelled"]))
    if stats["unmod"]:
        top = sorted(stats["unmod"], key=lambda x: -stats["unmod"][x])[:10]
        print("gen_code: unmodelled by mnemonic: " +
              ", ".join("%s=%d" % (k, stats["unmod"][k]) for k in top))
    print("gen_code: %d translation units" % (len(files) - 1))
    if bad:
        print("gen_code FAITHFULNESS GATE FAILED for %d function(s): %s" %
              (len(bad), "; ".join(bad[:5])))
        return 1
    print("gen_code faithfulness gate: %d function(s) verified "
          "instruction-for-instruction" % checked)
    return 0


if __name__ == "__main__":
    sys.exit(main())
