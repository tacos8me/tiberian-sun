#!/usr/bin/env python3
"""
Recover data-member offsets for each class from its constructor.

Method:
  1. Locate the constructor: the function containing the store of the class's
     vtable pointer.
  2. Walk that function's instructions and collect every store to
     [reg+disp] where reg holds `this` (the register the vtable was stored
     through). Those displacements are member offsets.
  3. Classify each member by the value stored: vtable pointer, zero-init,
     immediate, or a call result.
  4. Name members from INI keys / strings referenced by the class's methods
     where a correspondence can be established.

Output: out/members.json  { class -> { ctor, members: [ {off, kind, value} ] } }
"""
import capstone
import json
import os
import re
import struct
import sys
from collections import defaultdict

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

    def u32(self, va):
        b = self.rd(va, 4)
        return struct.unpack("<I", b)[0] if b and len(b) == 4 else None

    def in_text(self, va):
        t = self.text
        return t[0] <= va < t[1]

    def va_of(self, off):
        for vs, ve, raw, rs, nm in self.secs:
            if raw <= off < raw + rs:
                return vs + (off - raw)
        return None


def find_enclosing_function(fns, va):
    """Find the recovered function whose body contains va."""
    best = None
    for f in fns:
        if f["va"] <= va < f["end"]:
            if best is None or f["size"] < best["size"]:
                best = f
    return best


def disasm_range(bin_, start, maxlen=0x800):
    """Disassemble from start until a ret, for constructor analysis."""
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
        if len(out) > 2000:
            break
    return out


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    man = json.load(open(os.path.join(outdir, "class_manifest.json")))
    fns = json.load(open(os.path.join(outdir, "functions.json")))["functions"]

    result = {}
    for m in man:
        if not m["ctors"]:
            continue
        vtva = m["vtable"]
        # find the store site of the vtable pointer
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
            continue
        site = sites[0]
        # the constructor is the function containing the store; if the store
        # is not inside a recovered function, disassemble backwards to find
        # the enclosing prologue.
        f = find_enclosing_function(fns, site)
        if f:
            ctor_va = f["va"]
            insns = disasm_range(bin_, ctor_va)
        else:
            # scan backwards for a prologue (push ebp; mov ebp,esp) or a
            # 16-byte aligned start
            ctor_va = site & ~0xF
            while ctor_va > site - 0x400:
                b = bin_.rd(ctor_va, 3)
                if b and b[0] == 0x55 and b[1] == 0x8B and b[2] == 0xEC:
                    break
                ctor_va -= 1
            insns = disasm_range(bin_, ctor_va)

        # find the register the vtable was stored through (usually esi/edi/ecx)
        this_reg = None
        for ins in insns:
            if ins.mnemonic == "mov" and len(ins.operands) == 2:
                dst, src = ins.operands
                if (dst.type == capstone.x86.X86_OP_MEM and src.type == capstone.x86.X86_OP_IMM
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
            if off < 0 or off > 0x4000:
                continue
            kind = "unknown"
            val = None
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

        # dedupe by offset, keep first
        seen = {}
        for mem in members:
            seen.setdefault(mem["off"], mem)
        result[m["class"]] = {
            "ident": m["ident"],
            "ctor": ctor_va,
            "this_reg": this_reg,
            "members": [seen[k] for k in sorted(seen)],
        }

    with open(os.path.join(outdir, "members.json"), "w") as f:
        json.dump(result, f, indent=1)

    total = sum(len(v["members"]) for v in result.values())
    print(f"classes with member data: {len(result)}")
    print(f"total member offsets recovered: {total}")
    print(f"wrote {outdir}/members.json")


if __name__ == "__main__":
    main()
