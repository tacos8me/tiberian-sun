#!/usr/bin/env python3
"""
Name data members by tracing INI reader store patterns.

The INI readers in this binary follow a consistent shape:

    push <current value>        ; default
    push <key string VA>        ; the INI key
    push <section string>       ; section name
    mov  ecx, <ini object>
    call <INI accessor>
    mov  [this+OFFSET], <result>

Argument order is (default, key, section): the section is the last pushed string
(the first argument) and the member name is the key pushed immediately before it.
When the section is passed in a register, only the key is pushed. The member at
OFFSET is therefore the key, never the section. This recovers real member names
with real offsets.

Output: out/member_names.json  { class -> { offset -> {name, key_va, reader_va} } }
"""
import capstone
import json
import os
import re
import struct
import sys

IB = 0x400000

# INI accessor VAs recovered earlier (see SCHEMA.md).
INI_ACCESSORS = {
    0x4D60B0, 0x4D6960, 0x4D6ED0, 0x4D70B0, 0x4D54C0, 0x4D50F0,
    0x4D6030, 0x4D72B0, 0x4D6290, 0x4D4250, 0x4D6B50, 0x4D5D50,
    0x4D6B00, 0x4D7490, 0x4D5C60, 0x4D7070, 0x447F70,
}

# A call or branch ends the argument-push window: pushes before it belong to
# another call, so the pending string list must not carry across.
_BRANCH_MNEMS = {
    "call", "jmp", "je", "jne", "jz", "jnz", "jg", "jge", "jl", "jle",
    "ja", "jae", "jb", "jbe", "js", "jns", "jo", "jno", "jp", "jnp",
    "jecxz", "loop", "ret",
}


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

    def cstr(self, va, n=128):
        b = self.rd(va, n)
        if not b:
            return None
        z = b.find(b"\x00")
        if z >= 0:
            b = b[:z]
        try:
            return b.decode("ascii")
        except UnicodeDecodeError:
            return b.decode("latin-1")

    def in_text(self, va):
        t = self.text
        return t[0] <= va < t[1]


def is_ini_key(s):
    return bool(s) and bool(re.fullmatch(r"[A-Z][A-Za-z0-9]{2,30}", s))


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    man = json.load(open(os.path.join(outdir, "class_manifest.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    result = {}
    total = 0

    def key_count(f):
        return sum(1 for s in f.get("strings", []) if is_ini_key(s["text"]))

    def reader_set(roots):
        """The class's INI readers: its own high-key vtable slots plus the
        high-key functions they call.

        A class's Read_INI is frequently a small virtual that delegates the
        field parsing to a non-virtual helper, and only the helper carries the
        key strings.  The helper's stores still target this class's object
        because it is entered with the class's `this` in ecx, so its key/offset
        bindings are this class's members.
        """
        out = {}
        stack = [r for r in roots if r in fns]
        seen = set()
        while stack:
            va = stack.pop()
            if va in seen:
                continue
            seen.add(va)
            f = fns[va]
            nk = key_count(f)
            if nk >= 3:
                out[va] = nk
            for c in f.get("callees", []):
                g = fns.get(c)
                if g is not None and c not in seen and key_count(g) >= 3:
                    stack.append(c)
        return out

    for m in man:
        # candidate readers: vtable slots that reference INI keys, and the
        # readers those slots delegate to
        readers = reader_set(m["entries"])
        if not readers:
            continue
        ordered = sorted(readers.items(), key=lambda kv: (-kv[1], kv[0]))

        named = {}
        for rva, _ in ordered[:8]:
            f = fns[rva]
            # re-disassemble with detail so we can read operands
            code = bin_.rd(f["va"], f["size"] + 16)
            if not code:
                continue
            insns = list(md.disasm(code, f["va"]))
            # track the last key string pushed, and the `this` register
            this_reg = None
            pending_strs = []
            for i, ins in enumerate(insns):
                # identify `this`: mov <reg>, ecx near the top
                if (this_reg is None and ins.mnemonic == "mov"
                        and len(ins.operands) == 2
                        and ins.operands[0].type == capstone.x86.X86_OP_REG
                        and ins.operands[1].type == capstone.x86.X86_OP_REG
                        and ins.reg_name(ins.operands[1].reg) == "ecx"):
                    this_reg = ins.reg_name(ins.operands[0].reg)
                # remember pushed identifier-like strings in the current call setup
                if ins.mnemonic == "push" and ins.operands:
                    op = ins.operands[0]
                    if op.type == capstone.x86.X86_OP_IMM:
                        s = bin_.cstr(op.imm)
                        if is_ini_key(s):
                            pending_strs.append((s, op.imm))
                # an accessor call consumes the pending strings
                if ins.mnemonic == "call" and ins.operands:
                    op = ins.operands[0]
                    if op.type == capstone.x86.X86_OP_IMM and op.imm in INI_ACCESSORS:
                        # Argument order is (default, key, section): the section is
                        # the last pushed string and the key the one before it.  When
                        # the section is passed in a register only the key is pushed.
                        key = (pending_strs[-2] if len(pending_strs) >= 2
                               else (pending_strs[-1] if pending_strs else None))
                        # the store that follows names the member
                        for j in range(i + 1, min(i + 6, len(insns))):
                            nxt = insns[j]
                            if nxt.mnemonic != "mov" or len(nxt.operands) != 2:
                                continue
                            dst = nxt.operands[0]
                            if dst.type != capstone.x86.X86_OP_MEM:
                                continue
                            base = nxt.reg_name(dst.mem.base)
                            if this_reg and base != this_reg:
                                continue
                            off = dst.mem.disp
                            if off < 0 or off > 0x8000:
                                continue
                            if key:
                                named.setdefault(off, {
                                    "name": key[0],
                                    "key_va": key[1],
                                    "reader_va": rva,
                                })
                            break
                if ins.mnemonic in _BRANCH_MNEMS:
                    pending_strs = []

        if named:
            result[m["class"]] = {
                "ident": m["ident"],
                "members": {str(k): v for k, v in sorted(named.items())},
            }
            total += len(named)

    with open(os.path.join(outdir, "member_names.json"), "w") as f:
        json.dump(result, f, indent=1)

    print(f"classes with named members: {len(result)}")
    print(f"total members named: {total}")
    print(f"wrote {outdir}/member_names.json")


if __name__ == "__main__":
    main()
