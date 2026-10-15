#!/usr/bin/env python3
"""
Recover function signatures: calling convention, argument count, return kind.

Evidence used:
  - 'ret N' (C2 imm16)      => __stdcall, N/4 arguments
  - 'ret'  (C3)             => __cdecl/__thiscall; count distinct [esp+X] reads
  - 'mov <reg>, ecx' at entry and use of that reg as a base => __thiscall
  - 'xor eax,eax' before ret => returns 0 (bool/int)
  - 'mov eax,<imm>' before ret => returns a constant

Output: out/signatures.json  { va -> {cc, nargs, ret, evidence} }
"""
import capstone
import json
import os
import re
import struct
import sys

IB = 0x400000


class Bin:
    def __init__(self, path):
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


def analyze(bin_, md, va, size):
    code = bin_.rd(va, min(size + 32, 0x4000))
    if not code:
        return None
    insns = []
    for ins in md.disasm(code, va):
        insns.append(ins)
        if ins.mnemonic in ("ret", "retn"):
            break
        if len(insns) > 4000:
            break
    if not insns:
        return None

    cc = None
    nargs = None
    ret = None
    ev = []

    # calling convention + arg count from the epilogue
    last = insns[-1]
    if last.mnemonic in ("ret", "retn"):
        if last.operands and last.operands[0].type == capstone.x86.X86_OP_IMM:
            n = last.operands[0].imm
            cc = "__stdcall"
            nargs = n // 4
            ev.append(f"ret {n}")
        else:
            cc = "__cdecl"

    # __thiscall detection: this in ecx
    this_reg = None
    for ins in insns[:12]:
        if (ins.mnemonic == "mov" and len(ins.operands) == 2
                and ins.operands[0].type == capstone.x86.X86_OP_REG
                and ins.operands[1].type == capstone.x86.X86_OP_REG
                and ins.reg_name(ins.operands[1].reg) == "ecx"):
            this_reg = ins.reg_name(ins.operands[0].reg)
            break
    if this_reg:
        cc = "__thiscall"
        ev.append(f"this in ecx -> {this_reg}")

    # argument count from stack reads (only meaningful for non-stdcall)
    if nargs is None:
        offs = set()
        for ins in insns:
            if ins.mnemonic in ("mov", "movzx", "movsx", "lea", "cmp", "add", "sub"):
                for op in ins.operands:
                    if op.type == capstone.x86.X86_OP_MEM:
                        base = ins.reg_name(op.mem.base)
                        if base in ("esp", "ebp") and op.mem.disp > 0:
                            offs.add(op.mem.disp)
        if offs:
            # ebp+8 is arg0; esp+N maps differently, so only trust ebp
            ebp_offs = sorted(o for o in offs if o >= 8)
            if ebp_offs:
                nargs = len({(o - 8) // 4 for o in ebp_offs if (o - 8) % 4 == 0})
                ev.append(f"ebp+{ebp_offs[0]}..")

    # return kind
    tail = insns[-4:] if len(insns) >= 4 else insns
    for ins in tail:
        if ins.mnemonic == "xor" and ins.op_str.replace(" ", "") == "eax,eax":
            ret = "int"
            ev.append("xor eax,eax")
            break
        if ins.mnemonic == "mov" and len(ins.operands) == 2:
            d, s = ins.operands
            if (d.type == capstone.x86.X86_OP_REG and ins.reg_name(d.reg) == "eax"
                    and s.type == capstone.x86.X86_OP_IMM):
                ret = "int"
                ev.append(f"eax={s.imm:#x}")
                break

    if cc is None and nargs is None and ret is None:
        return None
    return {"cc": cc, "nargs": nargs, "ret": ret, "evidence": "; ".join(ev)}


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    fns = json.load(open(os.path.join(outdir, "functions.json")))["functions"]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    result = {}
    for f in fns:
        r = analyze(bin_, md, f["va"], f["size"])
        if r:
            result[hex(f["va"])] = r

    with open(os.path.join(outdir, "signatures.json"), "w") as fh:
        json.dump(result, fh, indent=1)

    from collections import Counter
    ccs = Counter(v["cc"] for v in result.values() if v["cc"])
    print(f"functions with signatures: {len(result)}")
    print("calling conventions:", dict(ccs))
    print(f"wrote {outdir}/signatures.json")


if __name__ == "__main__":
    main()
