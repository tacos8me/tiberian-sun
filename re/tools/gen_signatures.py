#!/usr/bin/env python3
"""
Recover function signatures from evidence: calling convention, where `this`
lives, argument count, argument types, and return type.

The point of this tool is that it does not default.  Every field it emits is
backed by the instruction that shows it, and where the binary does not decide,
the field says so (`null`, or the type string `"?"`) instead of defaulting to
`int`.  A wrong type is worse than a declared unknown, because P3.2's idiom
lifter builds on this.

What is recovered, and from what:

  cc / this_where   the epilogue (`ret N` pops N bytes of stack arguments, so
                    the callee is callee-pops), and whether `ecx` is READ
                    before it is written in the prologue.  A read of ecx
                    before any write to it is `this` in ecx (__thiscall); a
                    `ret N` with no ecx read is callee-pops with no `this` in
                    a register (__stdcall).  A bare `ret` is __cdecl unless ecx
                    was read first.
  nargs             total 4-byte stack argument slots: N/4 for `ret N`, else
                    the highest argument slot the body reads.  `this` is a
                    stack slot here only when the callee pops it, which is
                    exactly what verify.py check 8 assumes.
  nargs_decl        declared parameter count, i.e. stack slots minus the slot
                    that is `this`.  THIS is the apples-to-apples field: a C++
                    declaration lists parameters, never `this`.  null when the
                    binary does not show where `this` is.
  params[i]         what the body DOES with argument slot i, before writing it:
                    a width read, an address-of, a dereference (pointer), an
                    x87 load (float), an arithmetic use (int).  null type with
                    a reason when the use is inconclusive.
  ret               what the function leaves in eax / st0: an image address
                    (pointer), a small constant (int), a dereferenced value
                    (pointer), an x87 result (float/double), or nothing (void).
                    Also cross-checked against how callers consume the result.

Evidence from the *callers* is used for two things: how the return value is
consumed (which types it), and what each argument is pushed as.

Output: out/signatures.json  { "0xVA": {...} }
"""
import capstone
import json
import os
import re
import sys
from collections import Counter, defaultdict

IB = 0x400000

# Image sections that hold addresses: an immediate inside one is a pointer.
PTR_RANGES = []


class Bin:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        import pefile
        pe = pefile.PE(path, fast_load=True)
        self.secs = []
        for s in pe.sections:
            vs = IB + s.VirtualAddress
            ve = vs + s.Misc_VirtualSize
            self.secs.append((vs, ve, s.PointerToRawData, s.SizeOfRawData,
                              s.Name.decode(errors="replace").strip("\x00")))
            if s.Name.rstrip(b"\x00") != b".rsrc":
                PTR_RANGES.append((vs, ve))
        self.text = next(s for s in self.secs if s[4] == ".text")

    def rd(self, va, n):
        for vs, ve, raw, rs, nm in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                return self.data[o:o + n]
        return None


def is_image_ptr(v):
    return any(s <= v < e for s, e in PTR_RANGES)


# ---------------------------------------------------------------------------
# instruction helpers

def mem(ins, op):
    """(base, disp) of a memory operand, base as a register name or ''."""
    return (ins.reg_name(op.mem.base) if op.mem.base else "", op.mem.disp)


def rd(ins, op):
    return bool(op.access & capstone.CS_AC_READ)


def wr(ins, op):
    return bool(op.access & capstone.CS_AC_WRITE)


def disasm(md, bin_, va, size, cap=3000):
    code = bin_.rd(va, min(size + 48, 0x4000))
    if not code:
        return []
    out = []
    for ins in md.disasm(code, va):
        out.append(ins)
        if ins.mnemonic in ("ret", "retn"):
            break
        if len(out) > cap:
            break
    return out


# ---------------------------------------------------------------------------
# prologue: is `this` in ecx, and is a frame set up

def prologue_this(insns):
    """True if ecx is READ before it is written, by evidence strong enough to
    mean `this` in ecx.  Evidence: the instruction.

    Weak evidence is a bare `push ecx`, which MSVC also emits to reserve four
    bytes of local space (0x40f250: `push ecx; mov cl, [0x71544c]` -- the
    incoming ecx is overwritten and never used).  Strong evidence is ecx used
    as a memory base, copied into another register, or read by anything other
    than push/pop.  A write to ecx before any strong read ends the search.
    """
    for ins in insns[:24]:
        if ins.mnemonic in ("ret", "retn", "jmp"):
            break
        for i, op in enumerate(ins.operands):
            if op.type == capstone.x86.X86_OP_MEM and op.mem.base:
                if ins.reg_name(op.mem.base) == "ecx":
                    return True, ins
            elif op.type == capstone.x86.X86_OP_REG and ins.reg_name(op.reg) == "ecx":
                if i == 0 and wr(ins, op) and not rd(ins, op):
                    return False, None
                if rd(ins, op) and ins.mnemonic not in ("push", "pop"):
                    return True, ins
    return False, None


def framed(insns):
    for ins in insns[:6]:
        if ins.mnemonic == "mov" and len(ins.operands) == 2:
            d, s = ins.operands
            if (d.type == capstone.x86.X86_OP_REG
                    and ins.reg_name(d.reg) == "ebp"
                    and s.type == capstone.x86.X86_OP_REG
                    and ins.reg_name(s.reg) == "esp"):
                return True
    return False


# ---------------------------------------------------------------------------
# argument slot attribution
#
# A stack location is an incoming argument slot when it is read before it is
# written and it sits at or above the return address:
#   frame-relative  [ebp + 8 + 4k]   -> slot k   (locals live below ebp+8)
#   entry-relative  [esp + 4 + 4k + bytes_pushed]  -> slot k
# The entry-relative form is only trusted before the first `call`, because a
# callee's stack cleanup makes the running push count drift after that.

def arg_reads(insns, pops):
    """{k: [(idx, ins, op)]} for incoming argument slots.

    An argument slot k is the stack location at esp_at_entry + 4 + 4k, read
    before it is written.  `delta` is the current esp deficit from the entry
    value, so a memory access [esp + disp] is at entry-relative offset
    disp - delta: offset >= 4 is an argument, offset < 4 is a local or the
    return address.  A frame pointer makes the same test exact as
    [ebp + 8 + 4k] for any function that sets one up.

    delta is carried across calls: the `call` pushes a return address (+4) and
    the callee pops its arguments, so delta drops by the bytes pushed for that
    call.  A caller that cleans up itself instead (`add esp, N` right after)
    would double-count, so that add is skipped when it matches.
    """
    fr = framed(insns)
    # A pushed register that the function pops again is a save, not an argument,
    # so it must not count towards the bytes the callee pops.  0x405a30 pushes
    # esi for a call and pops it after; counting it put the read of its own
    # argument one slot too high.
    saved = {i.op_str for i in insns if i.mnemonic == "pop" and i.operands
             and i.operands[0].type == capstone.x86.X86_OP_REG}
    reads = defaultdict(list)
    written = set()
    delta = 0
    pending = 0
    skip_add = None
    for idx, ins in enumerate(insns):
        for op in ins.operands:
            if op.type != capstone.x86.X86_OP_MEM:
                continue
            base, disp = mem(ins, op)
            if base == "ebp" and fr:
                if rd(ins, op) and disp >= 8 and (disp - 8) % 4 == 0:
                    reads[(disp - 8) // 4].append((idx, ins, op))
            elif base == "esp":
                a = disp - delta
                if rd(ins, op) and a >= 4 and (a - 4) % 4 == 0 and a not in written:
                    reads[(a - 4) // 4].append((idx, ins, op))
                if wr(ins, op):
                    written.add(a)
        m = ins.mnemonic
        if m == "push":
            delta += 4
            if not (ins.operands and ins.operands[0].type == capstone.x86.X86_OP_REG
                    and ins.reg_name(ins.operands[0].reg) in saved):
                pending += 1
        elif m == "pop":
            delta -= 4
            pending = 0
            skip_add = None
        elif m in ("sub", "add") and len(ins.operands) == 2:
            d, src = ins.operands
            if (d.type == capstone.x86.X86_OP_REG and ins.reg_name(d.reg) == "esp"
                    and src.type == capstone.x86.X86_OP_IMM):
                if m == "add" and skip_add == src.imm:
                    skip_add = None
                else:
                    delta += src.imm if m == "sub" else -src.imm
                    pending = 0
        elif m == "call":
            delta -= 4 * pending
            skip_add = 4 * pending
            pending = 0
        elif m not in ("mov", "lea", "xor", "test", "cmp", "movzx", "movsx"):
            pending = 0
            skip_add = None
    return reads


# ---------------------------------------------------------------------------
# argument use -> type

INCONCLUSIVE = "?"


def reg_use(insns, idx, reg, path, depth=0):
    """What the register loaded from an argument slot is used as.

    A copy is followed: `mov edx, eax; mov [edx], esi` shows the argument is a
    pointer just as plainly as a direct dereference does.  `path` is the copy
    chain, so the evidence reads back as the register the slot was loaded into.
    """
    for j in range(idx + 1, min(len(insns), idx + 6)):
        ins = insns[j]
        if ins.mnemonic in ("ret", "retn", "jmp"):
            break
        for i, op in enumerate(ins.operands):
            if op.type == capstone.x86.X86_OP_MEM and op.mem.base:
                if ins.reg_name(op.mem.base) == reg:
                    if rd(ins, op):
                        return "void*", "%s dereferenced read at %s+0x%x" % (
                            path, reg, op.mem.disp)
                    return "void*", "%s dereferenced written at %s+0x%x " \
                        "(out-parameter)" % (path, reg, op.mem.disp)
            if op.type == capstone.x86.X86_OP_REG and ins.reg_name(op.reg) == reg:
                if ins.mnemonic in ("cmp", "test") and i <= 1:
                    return INCONCLUSIVE, "%s compared, width not shown" % path
                if ins.mnemonic in ("add", "sub", "imul", "and", "or", "xor",
                                    "shl", "shr", "lea", "neg", "inc", "dec"):
                    return "int", "%s used in %s" % (path, ins.mnemonic)
                if (ins.mnemonic == "mov" and i == 1 and depth < 3
                        and ins.operands[0].type == capstone.x86.X86_OP_REG):
                    nxt = ins.reg_name(ins.operands[0].reg)
                    return reg_use(insns, j, nxt, path + "->" + nxt, depth + 1)
    return INCONCLUSIVE, "no use of %s decides a width" % path


def classify_read(insns, idx, ins, op):
    """(type, evidence) for one read of an argument slot."""
    where = "0x%x %s %s" % (ins.address, ins.mnemonic, ins.op_str)
    m = ins.mnemonic
    ops = ins.operands
    if m in ("movzx", "movsx") and ops[0].type == capstone.x86.X86_OP_REG:
        if op.size == 1:
            return ("unsigned char" if m == "movzx" else "char",
                    "%s (1-byte %s)" % (where, m))
        if op.size == 2:
            return ("unsigned short" if m == "movzx" else "short",
                    "%s (2-byte %s)" % (where, m))
    if m == "mov" and ops[0].type == capstone.x86.X86_OP_REG:
        if op.size == 1:
            return "char", "%s (byte read of the slot)" % where
        if op.size == 2:
            return "short", "%s (word read of the slot)" % where
        if op.size == 4:
            r = ins.reg_name(ops[0].reg)
            return reg_use(insns, idx, r, r)
    if m == "lea" and ops[0].type == capstone.x86.X86_OP_REG:
        return "void*", "%s (address of the slot taken)" % where
    if m == "fld":
        return ("float" if op.size == 4 else "double",
                "%s (x87 load of the slot)" % where)
    if m == "fstp" or m == "fst":
        return INCONCLUSIVE, "%s (x87 store through the slot)" % where
    if m in ("cmp", "test"):
        if op.size == 1:
            return "char", "%s (byte compare)" % where
        return "int", "%s (32-bit %s of the slot)" % (where, m)
    if m == "push":
        return INCONCLUSIVE, "%s (pushed without being inspected)" % where
    if m in ("add", "sub", "and", "or", "xor", "imul", "shl", "shr", "neg"):
        return "int", "%s (arithmetic on the slot)" % where
    if m == "inc" or m == "dec":
        return "int", "%s (increment/decrement)" % where
    return INCONCLUSIVE, "%s (read as %s)" % (where, m)


# ---------------------------------------------------------------------------
# return type

def eax_writes(insns):
    """[(idx, ins, src_op)] for writes to eax."""
    out = []
    for idx, ins in enumerate(insns):
        ops = ins.operands
        if not ops:
            continue
        d = ops[0]
        if (d.type == capstone.x86.X86_OP_REG
                and ins.reg_name(d.reg) in ("eax", "ax", "al")
                and ins.mnemonic != "fnstsw"):
            if wr(ins, d):
                out.append((idx, ins, ops[1] if len(ops) > 1 else None))
    return out


def local_type(insns, idx, base, disp, depth=0):
    """Type of a stack local from the last store into it, or None.

    A function that returns a local returns whatever was stored there, so the
    store is the evidence.  One level only: a store of another local is not
    followed.
    """
    if depth > 1:
        return None, None
    window = list(reversed(insns[max(0, idx - 80):idx]))
    for exact in (True, False):
      for ins in window:
        for op in ins.operands:
            if op.type != capstone.x86.X86_OP_MEM or not wr(ins, op):
                continue
            b, d = mem(ins, op)
            # an esp-relative local keeps its displacement only until the next
            # push, so a store at the same displacement in the same frame is
            # accepted as the same local when the exact pair is not found
            if d != disp or (b != base and not (not exact and b in ("esp", "ebp"))):
                continue
            src = ins.operands[1] if len(ins.operands) > 1 else None
            where = "0x%x %s %s" % (ins.address, ins.mnemonic, ins.op_str)
            if src is None:
                return INCONCLUSIVE, "%s (stored, source not shown)" % where
            if src.type == capstone.x86.X86_OP_IMM:
                if op.size == 1:
                    return "char", "%s (byte constant stored)" % where
                if op.size == 2:
                    return "short", "%s (word constant stored)" % where
                return "int", "%s (constant stored)" % where
            if src.type == capstone.x86.X86_OP_REG:
                if ins.mnemonic == "lea":
                    return "void*", "%s (address stored)" % where
                if op.size == 1:
                    return "char", "%s (byte stored)" % where
                if op.size == 2:
                    return "short", "%s (word stored)" % where
                return INCONCLUSIVE, "%s (register %s stored)" % (
                    where, ins.reg_name(src.reg))
            if src.type == capstone.x86.X86_OP_MEM:
                if op.size == 1:
                    return "char", "%s (byte copied in)" % where
                if op.size == 2:
                    return "short", "%s (word copied in)" % where
                return INCONCLUSIVE, "%s (dword copied in)" % where
            return INCONCLUSIVE, "%s (stored)" % where
    return None, None


def body_return(insns):
    """(type|None, evidence) from the function's own epilogue.

    Only the LAST write to eax counts, and a write that is not the return value
    (fnstsw ax sets the x87 status word, not the result) is skipped.  When the
    last write is a plain register copy the value is whatever that register
    held, which this function alone does not show -- that is a "?", and the
    callers are consulted instead.
    """
    x87 = [i for i in insns if i.mnemonic in ("fld", "fstp", "fdiv", "fdivp",
                                              "fmul", "fmulp", "fadd", "faddp",
                                              "fsub", "fsubp", "fild", "fistp",
                                              "fabs", "fchs")]
    writes = eax_writes(insns)
    if writes:
        idx, ins, src = writes[-1]
        ops = ins.operands
        where = "0x%x %s %s" % (ins.address, ins.mnemonic, ins.op_str)
        if ins.mnemonic == "lea":
            return "void*", "%s (address computed into eax)" % where
        if src is None:
            if ins.mnemonic in ("inc", "dec", "neg", "not"):
                return "int", "%s (arithmetic result)" % where
            return INCONCLUSIVE, "%s (eax written, source not shown)" % where
        if src.type == capstone.x86.X86_OP_REG and src.reg == ops[0].reg:
            return "int", "%s (eax zeroed)" % where
        if src.type == capstone.x86.X86_OP_IMM:
            v = src.imm
            if is_image_ptr(v):
                return "void*", "%s (immediate 0x%x is an image address)" % (where, v)
            if ops[0].size == 1:
                return "char", "%s (byte constant 0x%x into al)" % (where, v)
            if ops[0].size == 2:
                return "short", "%s (word constant 0x%x into ax)" % (where, v)
            return "int", "%s (constant 0x%x, not an image address)" % (where, v)
        if src.type == capstone.x86.X86_OP_MEM:
            base = ins.reg_name(src.mem.base) if src.mem.base else ""
            if src.size == 1:
                return "char", "%s (byte load into eax)" % where
            if src.size == 2:
                return "short", "%s (word load into eax)" % where
            if base in ("esp", "ebp"):
                t, why = local_type(insns, idx, base, src.mem.disp)
                if t is not None:
                    return t, "%s returns the local %s: %s" % (where, base, why)
                return INCONCLUSIVE, "%s (returns a stack slot)" % where
            return INCONCLUSIVE, "%s (dword load; only the callers show " \
                "whether it is a pointer)" % where
        if src.type == capstone.x86.X86_OP_REG:
            if ins.mnemonic in ("add", "sub", "imul", "and", "or", "shl",
                                "shr", "inc", "dec", "neg"):
                return "int", "%s (arithmetic result)" % where
            return INCONCLUSIVE, "%s (eax from %s)" % (where, ins.reg_name(src.reg))
    # nothing wrote eax: a tail call hands the value through
    if insns and insns[-1].mnemonic in ("ret", "retn"):
        for ins in reversed(insns[:-1]):
            if ins.mnemonic == "call":
                return INCONCLUSIVE, "returns the callee's value (tail call)"
            break
    if x87:
        last = x87[-1]
        if last.mnemonic in ("fstp", "fst", "fistp"):
            return ("double" if last.operands[0].size == 8 else "float",
                    "0x%x %s %s (result left in st0)" % (
                        last.address, last.mnemonic, last.op_str))
        size = 8 if any(o.size == 8 for o in last.operands
                        if o.type == capstone.x86.X86_OP_MEM) else 4
        return (("double" if size == 8 else "float"),
                "0x%x %s %s (x87 result left in st0)" % (
                    last.address, last.mnemonic, last.op_str))
    return "void", "no write to eax and no x87 result before ret"


# ---------------------------------------------------------------------------
# caller-side evidence

def ret_use(insns, i):
    """How the caller consumes the value a call left in eax."""
    for ins in insns[i + 1:i + 4]:
        m, ops = ins.mnemonic, ins.operands
        if m in ("test", "cmp") and ops:
            if ops[0].type == capstone.x86.X86_OP_REG:
                if ins.reg_name(ops[0].reg) == "al":
                    return "char", "caller tests al"
                if ins.reg_name(ops[0].reg) == "eax":
                    return INCONCLUSIVE, "caller tests eax (int or pointer)"
        if m in ("movzx", "movsx") and ops and ops[0].type == capstone.x86.X86_OP_REG:
            if ops[1].type == capstone.x86.X86_OP_REG and ins.reg_name(ops[1].reg) == "al":
                return "char", "caller widens al"
        if m == "mov" and len(ops) == 2 and ops[1].type == capstone.x86.X86_OP_REG:
            if ins.reg_name(ops[1].reg) == "eax" and ops[0].type == capstone.x86.X86_OP_REG:
                r = ins.reg_name(ops[0].reg)
                return reg_use(insns, i + 1, r, r)
        if m in ("add", "sub", "imul", "and", "or", "shl", "shr"):
            return "int", "caller does arithmetic on the result"
        if m in ("fstp", "fst", "fld"):
            return "float", "caller moves the result through x87"
        if m in ("ret", "retn", "jmp"):
            return INCONCLUSIVE, "result passed through"
        if m in ("push", "call") and ops and ops[0].type == capstone.x86.X86_OP_REG:
            if ins.reg_name(ops[0].reg) == "eax":
                return INCONCLUSIVE, "result passed on without inspection"
    return "void", "caller ignores the result"


# ---------------------------------------------------------------------------

def analyze(bin_, md, va, size, pops=None):
    insns = disasm(md, bin_, va, size)
    if not insns:
        return None
    ev = []
    cc = None
    nargs = None

    last = insns[-1]
    popped = None
    if last.mnemonic in ("ret", "retn"):
        if last.operands and last.operands[0].type == capstone.x86.X86_OP_IMM:
            popped = last.operands[0].imm
            ev.append("0x%x ret %d (callee pops %d bytes)" % (va, popped, popped))
        else:
            ev.append("0x%x bare ret (callee pops nothing)" % va)

    this_in_ecx, this_ins = prologue_this(insns)
    if this_in_ecx:
        cc = "__thiscall"
        ev.append("0x%x %s %s (ecx read before written: this in ecx)" % (
            this_ins.address, this_ins.mnemonic, this_ins.op_str))
        nargs = (popped // 4) if popped is not None else None
    elif popped is not None and popped > 0:
        cc = "__stdcall"
        nargs = popped // 4
    elif popped == 0:
        cc = "__stdcall"
        nargs = 0
    elif popped is None and last.mnemonic in ("ret", "retn"):
        cc = "__cdecl"
        ev.append("bare ret and ecx not read: __cdecl")

    reads = arg_reads(insns, pops or {})
    if nargs is None and reads:
        nargs = max(reads) + 1
        ev.append("highest argument slot read: [esp/ebp+0x%x]" % (8 + 4 * max(reads)))

    # params
    params = []
    for k in sorted(reads):
        typ, why = None, None
        for idx, ins, op in reads[k]:
            t, w = classify_read(insns, idx, ins, op)
            if typ is None or (typ == INCONCLUSIVE and t != INCONCLUSIVE):
                typ, why = t, w
        params.append({"index": k, "type": typ, "evidence": why})

    # where is `this`?
    #
    # "ecx" and "stack" are PROVEN: ecx read before written, or the callee pops
    # a slot0 that the body then dereferences.  "stack?" is the callee-pops
    # case with no evidence either way -- the function may have an unused
    # `this` on the stack, or arg0 may be an ordinary argument.  It is the one
    # place where the this-slot cannot be settled from this function alone, so
    # both readings are emitted instead of picking one silently.
    this_where = "?"
    slot0 = [p for p in params if p["index"] == 0]
    if this_in_ecx:
        this_where = "ecx"
    elif cc == "__stdcall" and slot0 and slot0[0]["type"] == "void*":
        this_where = "stack"
        ev.append("arg0 dereferenced and the callee pops it: this on the stack")
    elif cc == "__stdcall" and nargs is not None and nargs > 0:
        this_where = "stack?"
        ev.append("callee-pops with stack arguments, no ecx use and no deref of "
                  "arg0: arg0 is either this or an ordinary argument")
    elif cc == "__cdecl" and slot0 and slot0[0]["type"] == "void*":
        this_where = "stack?"
        ev.append("arg0 dereferenced on a caller-pops function: possibly this")

    nargs_decl = None
    nargs_decl_alt = None
    if nargs is not None:
        if this_where in ("stack", "stack?"):
            nargs_decl = nargs - 1
            if nargs_decl < 0:
                nargs_decl = None
            if this_where == "stack?":
                nargs_decl_alt = nargs
        elif this_where == "ecx":
            nargs_decl = nargs
        else:
            nargs_decl = nargs
            nargs_decl_alt = nargs - 1 if nargs >= 1 else None
        ev.append("nargs=%d stack slots, this in %s, nargs_decl=%s" % (
            nargs, this_where, nargs_decl))

    ret_type, ret_ev = body_return(insns)
    return {
        "cc": cc,
        "this_where": this_where,
        "nargs": nargs,
        "nargs_decl": nargs_decl,
        "nargs_decl_alt": nargs_decl_alt,
        "this_ambiguous": this_where in ("stack?", "?"),
        "params": params,
        "ret": ret_type,
        "ret_evidence": ret_ev,
        "evidence": "; ".join(ev),
    }


# ---------------------------------------------------------------------------
# agreement with the hand-written declarations
#
# verify.py check 8 compares a declared parameter count against `nargs` and
# compensates by hand (`+1 if __stdcall`), because `nargs` counts `this` when
# the callee pops it.  That compensation is right for a __stdcall COM method
# and wrong for a free __stdcall function, and it cannot be right for both.
# `nargs_decl` is the honest field: it is the declared parameter count, so the
# comparison needs no compensation at all.  This measures it, using the same
# header parse verify.py uses, so the two numbers are directly comparable.

VDECL = re.compile(r"\bvirtual\s+(?:~?\w[\w:<>,*&\s]*?\s+)?(~?\w+)\s*\(([^)]*)\)")
VARE = re.compile(r"0x([0-9a-fA-F]{6,8})")


def param_count(params):
    p = params.strip()
    if p in ("", "void"):
        return 0
    depth, n = 0, 1
    for ch in p:
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1
        elif ch == "," and depth == 0:
            n += 1
    return n


def cited_declarations(hwdir):
    for p in sorted(os.path.join(hwdir, f) for f in os.listdir(hwdir)
                    if f.endswith(".h")):
        lines = open(p).read().splitlines()
        for i, line in enumerate(lines):
            if "virtual" not in line:
                continue
            m = VDECL.search(line)
            if not m:
                continue
            cited = None
            cm = re.search(r"//(.*)$", line[m.end():])
            if cm:
                vm = VARE.search(cm.group(1))
                if vm:
                    cited = vm.group(1)
            if cited is None and i > 0:
                prev = lines[i - 1].strip()
                if prev.startswith("//"):
                    vm = VARE.search(prev)
                    if vm:
                        cited = vm.group(1)
            if cited is None:
                continue
            yield os.path.basename(p), i + 1, m.group(1), param_count(m.group(2)), int(cited, 16)


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    fns = json.load(open(os.path.join(outdir, "functions.json")))["functions"]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    # caller-side evidence: how return values are consumed, what args are
    # pushed as.  Built once over every function.
    uses = defaultdict(Counter)
    pushed = defaultdict(lambda: defaultdict(Counter))
    pops = {}
    for f in fns:
        insns = disasm(md, bin_, f["va"], f["size"])
        if insns and insns[-1].mnemonic in ("ret", "retn") and insns[-1].operands:
            o = insns[-1].operands[0]
            if o.type == capstone.x86.X86_OP_IMM:
                pops[f["va"]] = o.imm
        for i, ins in enumerate(insns):
            if ins.mnemonic != "call":
                continue
            if ins.operands and ins.operands[0].type != capstone.x86.X86_OP_IMM:
                continue
            tgt = ins.operands[0].imm
            t, why = ret_use(insns, i)
            if t != "void":
                uses[tgt][t] += 1
            # arguments pushed for this call, innermost first
            n = 0
            for j in range(i - 1, max(-1, i - 12), -1):
                p = insns[j]
                if p.mnemonic == "push" and p.operands:
                    op = p.operands[0]
                    if op.type == capstone.x86.X86_OP_IMM:
                        kind = ("address" if is_image_ptr(op.imm)
                                else "small constant" if op.imm < 0x10000
                                else "constant")
                    elif op.type == capstone.x86.X86_OP_REG:
                        kind = "ecx" if ins.reg_name(op.reg) == "ecx" else "register"
                    else:
                        kind = "memory"
                    pushed[tgt][n][kind] += 1
                    n += 1
                elif p.mnemonic in ("test", "cmp", "mov", "lea", "xor"):
                    pass
                else:
                    break

    result = {}
    for f in fns:
        r = analyze(bin_, md, f["va"], f["size"], pops)
        if not r:
            continue
        # refine the return type with caller evidence when the body was silent
        if r["ret"] in (None, INCONCLUSIVE) or r["ret"] == "void":
            u = uses.get(f["va"])
            if u:
                kinds = [k for k, c in u.most_common() if k != "void"]
                if kinds:
                    r["ret_evidence"] = (r["ret_evidence"] or "") + \
                        " | callers: " + ", ".join(
                            "%s x%d" % (k, u[k]) for k in kinds[:3])
                    if (len(kinds) == 1 and kinds[0] in ("char", "int", "void*", "float")
                            and r["ret"] in (None, INCONCLUSIVE, "void")
                            and (u[kinds[0]] >= 2 or kinds[0] == "void*")):
                        r["ret"] = kinds[0]
                        r["ret_evidence"] = "callers consume the result: " + \
                            r["ret_evidence"].split(" | ")[-1]
        # argument evidence from call sites, for slots the body does not read
        for k, c in (pushed.get(f["va"]) or {}).items():
            if any(p["index"] == k for p in r["params"]):
                continue
            if r["nargs"] is not None and k >= r["nargs"]:
                continue
            total = sum(c.values())
            kinds = [x for x, _ in c.most_common()]
            typ = INCONCLUSIVE
            if kinds and c[kinds[0]] >= max(2, 0.8 * total):
                if kinds[0] == "address":
                    typ = "void*"
                elif kinds[0] in ("small constant", "constant"):
                    typ = "int"
            r["params"].append({
                "index": k, "type": typ,
                "evidence": "call sites push it as %s (%d of %d sites)" % (
                    kinds[0] if kinds else "?", c[kinds[0]] if kinds else 0, total)})
        r["params"].sort(key=lambda p: p["index"])
        result["0x%x" % f["va"]] = r

    with open(os.path.join(outdir, "signatures.json"), "w") as fh:
        json.dump(result, fh, indent=1)

    # ---- report ----
    ccs = Counter(v["cc"] for v in result.values())
    tw = Counter(v["this_where"] for v in result.values())
    typed = sum(1 for v in result.values()
                if any(p["type"] not in (INCONCLUSIVE, None) for p in v["params"]))
    np_ = sum(len(v["params"]) for v in result.values())
    npt = sum(1 for v in result.values() for p in v["params"]
              if p["type"] not in (INCONCLUSIVE, None))
    tdist = Counter(p["type"] for v in result.values() for p in v["params"]
                    if p["type"] not in (INCONCLUSIVE, None))
    rdist = Counter(v["ret"] for v in result.values())
    print("functions with signatures: %d" % len(result))
    print("calling conventions:", dict(ccs))
    print("this_where:", dict(tw))
    print("parameter slots named: %d of %d; functions with >=1 typed param: %d"
          % (npt, np_, typed))
    print("parameter types:", dict(tdist.most_common()))
    print("return types:", dict(rdist.most_common()))

    hwdir = os.path.join(os.path.dirname(os.path.abspath(outdir.rstrip("/"))),
                         "src", "handwritten")
    if os.path.isdir(hwdir):
        cmp8 = m8 = cmp_d = md_ = unk = alt = amb = 0
        named = named_match = 0
        kinds = Counter()
        examples = []
        for base, line, name, declared, va in cited_declarations(hwdir):
            rec = result.get("0x%x" % va)
            if not rec or rec["nargs"] is None:
                continue
            cmp8 += 1
            if declared + (1 if rec["cc"] == "__stdcall" else 0) == rec["nargs"]:
                m8 += 1
            if rec["nargs_decl"] is None:
                unk += 1
                continue
            if rec.get("this_ambiguous"):
                amb += 1
            cmp_d += 1
            if rec["nargs_decl"] == declared:
                md_ += 1
            elif rec.get("nargs_decl_alt") == declared:
                alt += 1
            else:
                if declared == 0:
                    kinds["declaration names no parameter" if not rec["params"]
                          else "declaration omits a parameter the body reads"] += 1
                elif rec["nargs_decl"] > declared:
                    kinds["declaration names fewer than the callee pops"] += 1
                else:
                    kinds["declaration names more than the callee pops"] += 1
                if len(examples) < 6:
                    examples.append("%s:%d %s() declares %d; %s at 0x%x "
                                    "pops %d stack slot(s), this in %s" % (
                                        base, line, name, declared, rec["cc"], va,
                                        rec["nargs"], rec["this_where"]))
            if declared > 0:
                named += 1
                named_match += 1 if rec["nargs_decl"] == declared else 0
        print("declared vs recovered arity: verify.py check-8 formula %d/%d; "
              "apples-to-apples on nargs_decl %d/%d (%d rows have no "
              "recoverable this-slot)" % (m8, cmp8, md_, cmp_d, unk))
        print("  ... of which %d agree only under the other reading of the "
              "this-slot (%d rows have an undecidable this-slot)"
              % (alt, amb))
        print("  of declarations that name >=1 parameter: %d/%d agree"
              % (named_match, named))
        print("  divergences:", dict(kinds.most_common()))
        for e in examples:
            print("    e.g.", e)
    print("wrote %s/signatures.json" % outdir)


if __name__ == "__main__":
    main()
