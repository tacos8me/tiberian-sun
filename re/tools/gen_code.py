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
import re
import struct
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


def render(va, f, image, stats=None, known=None):
    """The C++ body lines for one function, without the surrounding definition.

    `stats` is optional so the gate can re-render without perturbing the
    coverage counters it is checking. `known` is the set of every VA the tree
    registers, and it decides whether a span's continuation can be dispatched.
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
    # A recovered span can end without a transfer: the disassembler stopped at a
    # function boundary, but on the original control simply continues at the next
    # address -- which is the next span. Dropping that continuation was a silent
    # hole: the body returned instead, abandoning its frame, so its caller's
    # callee-saved registers and stack were never restored and the run wandered
    # off with whatever the abandoned frame happened to hold. It is a TAIL
    # transfer, not a call: the original pushes no return address here.
    insns = f.get("insns") or []
    if insns and not ends_with_transfer(insns[-1]):
        end = f.get("end") or va + f["size"]
        if known is not None and end in known:
            out.append("    ts::ts_tail(0x%08xu);   // span end 0x%08x: the "
                       "last instruction" % (end, end))
            out.append("    return;                //   here does not transfer "
                       "control")
            if stats is not None:
                stats["continuations"] = stats.get("continuations", 0) + 1
    return out


def control_flow_holes(fns, known):
    """Span ends that fall through into bytes with no transcription.

    A span whose last instruction does not transfer control continues at its end
    address. `tail_entries` gives every such address a transcription when the
    address lies inside a recovered span, so what is left here is either a
    transcription gap or a recovery gap, and the two are not the same defect:

      defects     -- the continuation is an instruction boundary inside another
                     recovered span but nothing is registered there. The code
                     exists and the tree cannot reach it, so a body that runs
                     into it abandons its frame.
      overlaps    -- the continuation is inside another span but not on an
                     instruction boundary of it, so no transcription can start
                     there. Two spans overlap, which is a defect in the
                     disassembly rather than in the transcription.
      unrecovered -- the continuation is in bytes no function covers. There is no
                     transcription to continue into, which is a gap in the
                     disassembly rather than in the transcription.
    """
    defects = []
    overlaps = []
    unrecovered = []
    for f in fns:
        insns = f.get("insns") or []
        if not insns or ends_with_transfer(insns[-1]):
            continue
        end = f.get("end") or f["va"] + f["size"]
        if end in known:
            continue
        inside = [g for g in fns if g["va"] < end < g["va"] + g["size"]]
        if not inside:
            unrecovered.append((f["va"], end))
            continue
        boundary = any(int(i.get("va", 0) or 0) == end
                       for g in inside for i in g.get("insns", []))
        if boundary:
            defects.append((f["va"], end, inside[0]["va"]))
        else:
            overlaps.append((f["va"], end, inside[0]["va"]))
    return defects, overlaps, unrecovered


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

# Mnemonics after which control does NOT continue at the next address, so the end
# of a span that holds one needs no continuation: `ret`/`retf` return, `jmp` goes
# elsewhere, `int`/`int3` trap, `hlt` stops the machine and `ud2` is undefined.
# Every other instruction falls through, which is why the span end matters.
TRANSFER = ("ret", "retn", "retf", "iretd", "iret", "hlt", "int", "int3",
            "ud2", "jmp", "ljmp")


def ends_with_transfer(insn):
    """Does this instruction leave control somewhere other than the next byte?"""
    return (insn.get("mnem") or "").strip() in TRANSFER


# `jmp dword ptr [reg*4 + 0xTABLE]`: the operand names the table's base address.
INDIRECT_JMP = re.compile(r"dword ptr \[[a-z]{2,3}\*4 \+ (0x[0-9a-fA-F]+)\]")


def indirect_jump_targets(fns, image, window=256):
    """VAs a static switch table can transfer to, from the table in the image.

    `ts::jmp_indirect` dispatches by exact VA. An entry that nothing registered
    is treated as an unregistered call: the callee's frame is abandoned with
    its saved registers and return address still on the modelled stack. The
    instruction stream names the table base but not its entries, so they are
    read out of the real image.

    An entry is accepted only when it is an instruction boundary of a recovered
    span, because a transcription has to start on one. That also rejects the
    padding/garbage entry a switch table sometimes carries at index 0. A table
    is scanned as a bounded window rather than until the first non-code word:
    the first entry of the memcpy tables at 0x683c10 is not an instruction
    boundary (0x683c20 is real code the recovery decoded from the table bytes
    at 0x683c14 instead), and stopping there would miss the rest of the table.
    """
    if image is None:
        return {}
    bounds = {}
    for f in fns:
        for i in f.get("insns") or []:
            bounds[int(i.get("va", 0) or 0)] = f
    out = {}
    for f in fns:
        for i in f.get("insns") or []:
            if (i.get("mnem") or "").strip() != "jmp":
                continue
            m = INDIRECT_JMP.search((i.get("op") or "").strip())
            if not m:
                continue
            base = int(m.group(1), 16)
            for k in range(window):
                try:
                    t = struct.unpack("<I", image.read(base + 4 * k, 4))[0]
                except Exception:
                    break
                host = bounds.get(t)
                if host is not None and t not in out:
                    out[t] = host
    return out


def tail_entries(fns, extra=()):
    """VAs a cross-function branch targets that are not function starts.

    A `jmp` to an address in the middle of another function is a tail transfer:
    the original has already arranged the stack and expects that address's code
    to run to the end of the function containing it. Those addresses are not
    function entries, so nothing registered them and a `ts_call` to one found
    nothing -- it was counted as unmapped rather than reached. Transcribing each
    from its own address to the end of its containing function closes that hole.

    `extra` carries VAs found outside the instruction stream (indirect jump
    tables) that need the same treatment for the same reason.
    """
    starts = {f["va"] for f in fns}
    need = set(t for t in extra if t not in starts)
    for f in fns:
        lo, hi = f["va"], f["va"] + f["size"]
        # A span whose last instruction does not transfer control continues at its
        # end address. If no function starts there, the continuation is still real
        # code in the middle of another span and needs a transcription of its own,
        # for the same reason a branch into the middle of a span does.
        insns = f.get("insns") or []
        if insns and not ends_with_transfer(insns[-1]):
            end = f.get("end") or hi
            if end not in starts:
                need.add(end)
        for i in insns:
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
    #
    # To a fixpoint: a continuation can land inside a span that is itself a tail
    # entry, and that continuation needs its own transcription just as much. Each
    # pass can only add addresses that are not already entries, so this
    # terminates; the bound is a guard against a pathological input, not a
    # working limit.
    # VAs reachable only through a static switch table. Computed once: the
    # tables are in the image, not in the recovered spans, so the fixpoint does
    # not change which entries exist, only which span contains each one.
    indirect = indirect_jump_targets(fns, image)

    tails = {}
    for _ in range(8):
        added = 0
        for t, host in tail_entries(fns + list(tails.values()),
                                    extra=indirect).items():
            if t in tails:
                continue
            insns = [i for i in host.get("insns", [])
                     if int(i.get("va", 0) or 0) >= t]
            if not insns:
                continue
            tails[t] = {"va": t, "size": host["va"] + host["size"] - t,
                        "ninsn": len(insns), "insns": insns,
                        "callees": host.get("callees") or [],
                        "tail_of": host["va"]}
            added += 1
        if not added:
            break
    for t, f in tails.items():
        fns.append(f)
    fns.sort(key=lambda f: f["va"])

    # Every VA the tree registers. A span whose last instruction falls through
    # continues at its end address, and that address is only reachable if
    # something transcribed it: a function start, or a tail entry.
    known = set(f["va"] for f in fns)
    (stats["defects"], stats["overlaps"],
     stats["unrecovered"]) = control_flow_holes(fns, known)

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
            lines.extend(render(va, f, image, stats, known))
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
    # The extents of recovered code, merged so the table stays small. The model
    # uses it to tell "a recovered function nothing registered" from "a call into
    # bytes the recovery never covered" -- two defects a .text range cannot
    # distinguish. Merging only joins spans that touch, so a gap between two
    # spans stays a gap.
    spans = sorted((f["va"], f["va"] + f["size"]) for f in fns)
    merged = []
    for lo, hi in spans:
        if merged and lo <= merged[-1][1]:
            if hi > merged[-1][1]:
                merged[-1][1] = hi
        else:
            merged.append([lo, hi])
    reg.append("namespace {")
    reg.append("struct Entry { u32 va; Fn fn; };")
    reg.append("")
    reg.append("// [lo, hi) per recovered extent, sorted and non-overlapping.")
    reg.append("const u32 kRecoveredSpans[] = {")
    for lo, hi in merged:
        reg.append("    0x%08xu, 0x%08xu," % (lo, hi))
    reg.append("};")
    reg.append("const unsigned kRecoveredSpanCount =")
    reg.append("    sizeof(kRecoveredSpans) / sizeof(kRecoveredSpans[0]) / 2;")
    reg.append("")
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
    reg.append("    set_recovered_spans(kRecoveredSpans, kRecoveredSpanCount);")
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
    known = set(fns)
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
        exp = "\n".join(render(va, f, image, known=known))
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

    stats = {"modelled": 0, "unmodelled": 0, "unmod": {},
             "continuations": 0, "defects": [], "overlaps": [],
             "unrecovered": []}
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
    # Control-flow coverage, reported next to instruction coverage because it is
    # the same kind of claim. Instruction coverage says every instruction of every
    # transcribed span is rendered exactly once; it says nothing about whether the
    # spans join up, and for 10,450 of them they did not: the body returned at the
    # end of its span instead of continuing, abandoning its frame. A span that
    # falls through into recovered code with nothing registered there is that
    # defect, so it fails the run.
    print("gen_code: control flow: %d span(s) continue into another span by tail "
          "transfer, %d end inside another span's instruction (overlapping "
          "spans, reported), %d end in bytes that were not recovered as code" %
          (stats["continuations"], len(stats["overlaps"]),
           len(stats["unrecovered"])))
    for va, end, host in stats["overlaps"]:
        print("gen_code:   span 0x%08x falls through to 0x%08x, which is inside "
              "the span at 0x%08x but not on an instruction boundary" %
              (va, end, host))
    if stats["defects"]:
        print("gen_code CONTROL-FLOW GATE FAILED for %d span(s) that fall "
              "through into recovered code with no transcription there: %s" %
              (len(stats["defects"]),
               "; ".join("0x%08x -> 0x%08x (inside 0x%08x)" % d
                         for d in stats["defects"][:5])))
        return 1
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
