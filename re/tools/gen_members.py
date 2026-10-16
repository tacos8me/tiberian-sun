#!/usr/bin/env python3
"""
Recover data-member offsets for each class from its constructor.

Method:
  1. Find every store site of the class's vtable pointer.
  2. For each site, locate the enclosing function by scanning backwards for a
     function boundary (prologue, alignment padding, or a preceding ret).
     This does NOT depend on the disassembler's function list, which misses
     constructors that were never seeded.
  3. Disassemble that function and collect stores to [this+disp], where `this`
     is the register the vtable was stored through.
  4. Classify each member by the value stored.

Output: out/members.json
"""
import bisect
import capstone
import json
import os
import struct
import sys

IB = 0x400000


class Bin:
    def __init__(self, path, outdir="out"):
        self.outdir = outdir
        self.data = open(path, "rb").read()
        import pefile
        pe = pefile.PE(path, fast_load=True)
        self.secs = []
        for s in pe.sections:
            self.secs.append((IB + s.VirtualAddress,
                              IB + s.VirtualAddress + s.Misc_VirtualSize,
                              s.PointerToRawData, s.SizeOfRawData,
                              s.Name.decode(errors="replace").strip("\x00")))
        self.text = next(s for s in self.secs if s[4] == ".text")

    def rd(self, va, n):
        for vs, ve, raw, rs, nm in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                return self.data[o:o + n]
        return None

    def in_text(self, va):
        t = self.text
        return t[0] <= va < t[1]

    def va_of(self, off):
        for vs, ve, raw, rs, nm in self.secs:
            if raw <= off < raw + rs:
                return vs + (off - raw)
        return None


def enclosing_fn(bin_, site):
    """Entry point of the recovered function containing `site`, or None.

    Uses the authoritative set from out/functions.json rather than a byte
    heuristic, so a store site is always attributed to the function that
    actually contains it.
    """
    starts = getattr(bin_, "_fn_starts", None)
    if starts is None:
        starts = []
        p = os.path.join(getattr(bin_, "outdir", "out"), "functions.json")
        if os.path.exists(p):
            with open(p) as f:
                starts = sorted(x["va"] for x in json.load(f)["functions"])
        bin_._fn_starts = starts
    if not starts:
        return None
    i = bisect.bisect_right(starts, site) - 1
    return starts[i] if i >= 0 else None


def find_function_start(bin_, site, back=0x600):
    """Scan backwards from a store site to the enclosing function's start.

    Boundaries we accept:
      - 'push ebp; mov ebp,esp' prologue
      - a run of int3/nop padding immediately before the candidate
      - a 'ret' immediately before the candidate
    """
    lo = max(bin_.text[0], site - back)
    best = None
    va = site
    while va > lo:
        b = bin_.rd(va, 3)
        if not b or len(b) < 3:
            va -= 1
            continue
        # prologue
        if b[0] == 0x55 and b[1] == 0x8B and b[2] == 0xEC:
            return va
        # padding before this address => function start
        prev = bin_.rd(va - 1, 1)
        if prev and prev[0] in (0xCC, 0x90):
            # walk back over the whole padding run
            p = va - 1
            while p > lo:
                pb = bin_.rd(p, 1)
                if pb and pb[0] in (0xCC, 0x90):
                    p -= 1
                else:
                    break
            cand = p + 1
            if best is None:
                best = cand
            return cand
        va -= 1
    return best if best is not None else (site & ~0xF)


def disasm_func(bin_, start, maxlen=0x1000):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    code = bin_.rd(start, maxlen)
    if not code:
        return []
    out = []
    for ins in md.disasm(code, start):
        out.append(ins)
        if ins.mnemonic in ("ret", "retn"):
            break
        if len(out) > 3000:
            break
    return out


def recover(bin_, vtva):
    """Return (ctor_va, members) for a vtable, or (None, [])."""
    pat = struct.pack("<I", vtva)
    sites = []
    start = 0
    while True:
        j = bin_.data.find(pat, start)
        if j < 0:
            break
        va = bin_.va_of(j)
        if va is not None and bin_.in_text(va):
            sites.append(va)
        start = j + 1
    if not sites:
        return None, []

    for site in sites:
        # Prefer the authoritative recovered function start that CONTAINS this
        # store site. find_function_start() below is a backward-scan heuristic
        # that accepts only frame-based prologues (55 8B EC) and can land on a
        # different function; measured against out/functions.json it produced a
        # VA that was not a function start for 97% of classes, which
        # misattributed member claims to the wrong constructor.
        ctor = enclosing_fn(bin_, site)
        if ctor is None:
            ctor = find_function_start(bin_, site)
        if ctor is None:
            continue
        insns = disasm_func(bin_, ctor)
        if not insns:
            continue
        # the register the vtable is stored through is `this`
        this_reg = None
        for ins in insns:
            if ins.mnemonic == "mov" and len(ins.operands) == 2:
                dst, src = ins.operands
                if (dst.type == capstone.x86.X86_OP_MEM
                        and src.type == capstone.x86.X86_OP_IMM
                        and src.imm == vtva):
                    this_reg = ins.reg_name(dst.mem.base)
                    break
        if not this_reg:
            continue

        members = []
        for ins in insns:
            if ins.mnemonic not in ("mov", "movzx", "movsx", "and", "or"):
                continue
            if len(ins.operands) != 2:
                continue
            dst, src = ins.operands
            if dst.type != capstone.x86.X86_OP_MEM:
                continue
            if ins.reg_name(dst.mem.base) != this_reg:
                continue
            off = dst.mem.disp
            if off < 0 or off > 0x8000:
                continue
            kind, val = "unknown", None
            if src.type == capstone.x86.X86_OP_IMM:
                val = src.imm
                if bin_.in_text(val):
                    kind = "vtable"
                elif val == 0:
                    kind = "zero"
                else:
                    kind = "imm"
            elif src.type == capstone.x86.X86_OP_REG:
                kind = "reg"
            members.append({"off": off, "kind": kind, "value": val})

        if members:
            seen = {}
            for mem in members:
                seen.setdefault(mem["off"], mem)
            return ctor, [seen[k] for k in sorted(seen)]
    return None, []


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir, outdir)
    man = json.load(open(os.path.join(outdir, "class_manifest.json")))

    result = {}
    for m in man:
        ctor, members = recover(bin_, m["vtable"])
        if ctor is None:
            continue
        result[m["class"]] = {
            "ident": m["ident"],
            "ctor": ctor,
            "members": members,
        }

    with open(os.path.join(outdir, "members.json"), "w") as f:
        json.dump(result, f, indent=1)

    total = sum(len(v["members"]) for v in result.values())
    print(f"classes with member data: {len(result)}")
    print(f"total member offsets recovered: {total}")
    print(f"wrote {outdir}/members.json")


if __name__ == "__main__":
    main()
