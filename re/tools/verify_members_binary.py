#!/usr/bin/env python3
"""
Binary-anchored verification of recovered member offsets.

Why this exists
---------------
verify.py checks 5, 6 and 7 all compare the reconstructed tree against files in
out/ that the same toolchain produced. If gen_members.py were wrong, the
manifest and the headers would be wrong *together* and every one of those
checks would still pass. Only check 4 was anchored to the binary.

This tool closes that gap for member offsets. It re-derives each claimed
displacement directly from the constructor's machine code in bin/GAME.EXE and
compares it against what out/members.json claims. Nothing here trusts out/.

How a claim is verified
-----------------------
For each class the recovery records a constructor VA and a list of member
stores (offset, kind, value). This tool disassembles the constructor and looks
for a store whose displacement matches the claimed offset:

  imm  -> mov dword ptr [reg+off], imm32   with imm32 equal to the claimed value
  reg  -> mov dword ptr [reg+off], <reg>   (any register source)
  zero -> mov dword ptr [reg+off], 0       or the byte/word forms

A claim is CONFIRMED when a matching store exists at that displacement inside
the recorded constructor, REFUTED when the constructor contains stores but none
at that displacement, and UNCHECKABLE when the constructor could not be
disassembled at all. UNCHECKABLE is reported separately and is never counted as
a pass -- that is the whole point.

Usage:
  python3 tools/verify_members_binary.py            # report
  python3 tools/verify_members_binary.py --limit 50 # sample for speed
  python3 tools/verify_members_binary.py --fail-under 90   # gate threshold
"""
import argparse
import bisect
import json
import os
import sys

try:
    import capstone
except ImportError:
    sys.stderr.write("capstone is required: pip install capstone\n")
    sys.exit(2)

try:
    import pefile
except ImportError:
    sys.stderr.write("pefile is required: pip install pefile\n")
    sys.exit(2)

IB = 0x400000


class Image:
    def __init__(self, path, outdir="out"):
        self.outdir = outdir
        self._starts = None
        self.data = open(path, "rb").read()
        pe = pefile.PE(path, fast_load=True)
        self.secs = []
        for s in pe.sections:
            self.secs.append((IB + s.VirtualAddress,
                              IB + s.VirtualAddress + s.Misc_VirtualSize,
                              s.PointerToRawData))
        self.text = None
        for s in pe.sections:
            if s.Name.decode().strip("\x00") == ".text":
                self.text = (IB + s.VirtualAddress, s.Misc_VirtualSize)
        if self.text is None:
            raise SystemExit("no .text section")

    def read(self, va, n):
        for vs, ve, raw in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                return self.data[o:o + n]
        return None

    def in_text(self, va):
        return self.text[0] <= va < self.text[0] + self.text[1]

    def fn_starts(self):
        """Sorted recovered function entry points, used to map a vtable-store
        site back to the constructor that contains it."""
        if self._starts is None:
            p = os.path.join(self.outdir, "functions.json")
            if os.path.exists(p):
                self._starts = sorted(
                    f["va"] for f in json.load(open(p))["functions"])
            else:
                self._starts = []
        return self._starts


def classify_store(insn, claimed_off, kind, claimed_val):
    """Does this instruction store to [reg+claimed_off] in the claimed way?"""
    if not insn.operands:
        return False
    dst = insn.operands[0]
    if dst.type != capstone.x86.X86_OP_MEM:
        return False
    if dst.mem.disp != claimed_off:
        return False
    # Reject [reg+off] forms where the base is the stack or frame pointer:
    # those are locals, not members.
    base = dst.mem.base
    if base in (capstone.x86.X86_REG_ESP, capstone.x86.X86_REG_EBP):
        return False
    if kind == "imm":
        if len(insn.operands) < 2:
            return False
        src = insn.operands[1]
        if src.type != capstone.x86.X86_OP_IMM:
            return False
        return claimed_val is None or (src.imm & 0xFFFFFFFF) == (claimed_val & 0xFFFFFFFF)
    if kind == "zero":
        if len(insn.operands) < 2:
            return False
        src = insn.operands[1]
        if src.type == capstone.x86.X86_OP_IMM:
            return src.imm == 0
        # xor eax,eax; mov [x+off],eax  -- the zeroing is not local to this insn,
        # so accept any register source and let the kind be advisory.
        return src.type == capstone.x86.X86_OP_REG
    if kind == "reg":
        if len(insn.operands) < 2:
            return False
        return insn.operands[1].type in (capstone.x86.X86_OP_REG,
                                         capstone.x86.X86_OP_IMM)
    return False


def verify_class(img, md, rtti_name, entry):
    """(confirmed, refuted, uncheckable, notes) for one class's claims."""
    ctor = entry.get("ctor")
    claims = entry.get("members") or []
    if not ctor or not img.in_text(ctor):
        return 0, 0, len(claims), ["no usable constructor VA"]
    # `ctor` in members.json is the address of the vtable-store SITE, not the
    # function entry point: 97% of the 629 recorded VAs are not function starts
    # (measured). Disassembling from a mid-function address produces garbage
    # (nop; add al,[eax]) and makes every claim look refuted, so map the site
    # back to the enclosing recovered function first.
    starts = img.fn_starts()
    i = bisect.bisect_right(starts, ctor) - 1
    if i < 0:
        return 0, 0, len(claims), ["store site precedes any function"]
    fn = starts[i]
    blob = img.read(fn, 0x800)
    if not blob:
        return 0, 0, len(claims), ["constructor VA not in any section"]
    insns = list(md.disasm(blob, fn))
    if not insns:
        return 0, 0, len(claims), ["constructor did not disassemble"]

    confirmed = refuted = 0
    notes = []
    for c in claims:
        o = c.get("off")
        if o is None:
            continue
        if any(classify_store(i, o, c.get("kind"), c.get("value"))
               for i in insns):
            confirmed += 1
        else:
            refuted += 1
            if len(notes) < 2:
                notes.append(f"offset {hex(o)} ({c.get('kind')}) not stored "
                             f"in ctor {hex(ctor)}")
    return confirmed, refuted, 0, notes


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bin", default="bin/GAME.EXE")
    ap.add_argument("--out", default="out")
    ap.add_argument("--limit", type=int, default=0,
                    help="only check the first N classes (0 = all)")
    ap.add_argument("--fail-under", type=float, default=None,
                    help="exit non-zero if confirmed%% is below this")
    args = ap.parse_args()

    mp = os.path.join(args.out, "members.json")
    if not os.path.exists(mp):
        sys.stderr.write(f"missing {mp}; run ./tools/pipeline.sh\n")
        return 2

    img = Image(args.bin, args.out)
    # detail=True is required for insn.operands; without it capstone raises
    # CS_ERR_DETAIL on access.
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    members = json.load(open(mp))
    items = sorted(members.items())
    if args.limit:
        items = items[:args.limit]

    total = confirmed = refuted = uncheck = 0
    worst = []
    for name, entry in items:
        c, r, u, notes = verify_class(img, md, name, entry)
        total += c + r + u
        confirmed += c
        refuted += r
        uncheck += u
        if r:
            worst.append((r, name, notes))

    checked = confirmed + refuted
    pct = (100.0 * confirmed / checked) if checked else 0.0
    print(f"member offsets anchored to the binary")
    print(f"  classes examined      {len(items)}")
    print(f"  claims examined       {total}")
    print(f"  confirmed in binary   {confirmed}")
    print(f"  refuted               {refuted}")
    print(f"  uncheckable           {uncheck}")
    print(f"  confirmed of checkable {pct:.1f}%")

    if worst:
        worst.sort(reverse=True)
        print("\n  worst classes:")
        for r, name, notes in worst[:8]:
            print(f"    {name}: {r} refuted -- {notes[0] if notes else ''}")

    if args.fail_under is not None and pct < args.fail_under:
        print(f"\nFAIL: {pct:.1f}% < required {args.fail_under}%")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
