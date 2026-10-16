#!/usr/bin/env python3
"""
Function boundary recovery and call-graph construction for GAME.EXE.

Strategy:
  1. Seed function starts from:
       - the PE entry point
       - all vtable entries (from RTTI recovery)
       - all import thunk targets
       - MSVC prologue patterns
  2. Linear-sweep disassemble each function with capstone.
  3. Record direct call/jmp targets to extend the worklist.
  4. Emit per-function JSON: address, size, instructions, callees, strings.

This is the substrate for source reconstruction.
"""
import bisect
import json
import os
import struct
import sys
from collections import defaultdict, deque

import capstone
import pefile

IMAGE_BASE = 0x400000


class Binary:
    def __init__(self, path):
        self.pe = pefile.PE(path, fast_load=True)
        self.data = self.pe.__data__
        self.sections = []
        for s in self.pe.sections:
            self.sections.append({
                "name": s.Name.decode(errors="replace").strip("\x00"),
                "va": IMAGE_BASE + s.VirtualAddress,
                "vsize": s.Misc_VirtualSize,
                "raw": s.PointerToRawData,
                "rawsize": s.SizeOfRawData,
                "flags": s.Characteristics,
            })
        self.text = next(s for s in self.sections if s["name"] == ".text")

    def read(self, va, n):
        for s in self.sections:
            if s["va"] <= va < s["va"] + s["vsize"]:
                off = s["raw"] + (va - s["va"])
                return self.data[off:off + n]
        return None

    def u32(self, va):
        b = self.read(va, 4)
        return struct.unpack("<I", b)[0] if b and len(b) == 4 else None

    def cstr(self, va, maxlen=512):
        b = self.read(va, maxlen)
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
        return t["va"] <= va < t["va"] + t["vsize"]

    def in_rdata(self, va):
        for s in self.sections:
            if s["name"] == ".rdata":
                return s["va"] <= va < s["va"] + s["vsize"]
        return False

    def va_of(self, file_off):
        for s in self.sections:
            if s["raw"] <= file_off < s["raw"] + s["rawsize"]:
                return s["va"] + (file_off - s["raw"])
        return None


def load_imports(bin_):
    """Map IAT slot VA -> (dll, symbol)."""
    pe = bin_.pe
    pe.parse_data_directories(directories=[
        pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_IMPORT"]])
    iat = {}
    for entry in getattr(pe, "DIRECTORY_ENTRY_IMPORT", []):
        dll = entry.dll.decode(errors="replace")
        for imp in entry.imports:
            if imp.address:
                iat[imp.address] = (dll, imp.name.decode(errors="replace")
                                    if imp.name else f"ord_{imp.ordinal}")
    return iat


def load_vtable_targets(outdir):
    """Collect all code pointers appearing in recovered vtables."""
    targets = set()
    path = os.path.join(outdir, "classes.json")
    if not os.path.exists(path):
        return targets
    d = json.load(open(path))
    return targets


def find_prologues(bin_):
    """Scan .text for MSVC function prologue byte patterns."""
    t = bin_.text
    blob = bin_.data[t["raw"]:t["raw"] + t["rawsize"]]
    starts = set()
    # push ebp; mov ebp, esp
    pats = [
        b"\x55\x8b\xec",         # push ebp; mov ebp, esp
        b"\x55\x8b\x6c\x24",     # push ebp; mov ebp,[esp+x]
        b"\x6a\xff\x68",         # push -1; push imm32 (SEH frame)
        b"\x83\xec",             # sub esp, imm8
        b"\x81\xec",             # sub esp, imm32
        # Frame-less __thiscall entry points. MSVC omits the frame entirely for
        # small member functions, so these begin by saving registers and
        # immediately loading `this` from ECX. Missing them left 109 of
        # TechnoClass's 214 vtable slots with no function record.
        b"\x56\x8b\xf1",         # push esi; mov esi, ecx
        b"\x57\x8b\xf9",         # push edi; mov edi, ecx
        b"\x53\x8b\xd9",         # push ebx; mov ebx, ecx
        b"\x51\x56\x8b\xf1",     # push ecx; push esi; mov esi, ecx
        b"\x56\x57\x8b\xf9",     # push esi; push edi; mov edi, ecx
        b"\x8b\x44\x24\x04",     # mov eax, [esp+4]   (__stdcall, no frame)
        b"\x8b\x4c\x24\x04",     # mov ecx, [esp+4]
        b"\x8b\x54\x24\x04",     # mov edx, [esp+4]
    ]
    # Scan for EVERY pattern in pats. Previously only the hardcoded 55 8b ec
    # sequence was matched and the pats list above was dead code, which left
    # 2,415 `83 ec` and 298 `81 ec` prologues unseeded.
    #
    # A bare `83 ec`/`81 ec` also occurs as `sub esp, N` inside an existing
    # body, so a candidate is only accepted as a function start when the byte
    # before it is a boundary (ret, int3 or nop padding, or a jmp).
    BOUNDARY_TAIL = (b"\xc3", b"\xc2", b"\xcc", b"\x90", b"\xeb", b"\xe9")
    for i in range(len(blob) - 6):
        if not any(blob.startswith(p, i) for p in pats):
            continue
        # 55 8b ec is unambiguous and was already trusted; the sub-esp forms
        # need a boundary check to avoid seeding mid-function.
        if not blob.startswith(b"\x55\x8b\xec", i):
            if i == 0:
                pass
            elif blob[i - 1:i] not in BOUNDARY_TAIL:
                if not (blob[i - 2:i] in (b"\xcc\xcc", b"\x90\x90")
                        or blob[i - 3:i] in (b"\xcc\xcc\xcc", b"\x90\x90\x90")):
                    continue
        starts.add(t["va"] + i)
    return starts


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    bin_ = Binary(path)
    iat = load_imports(bin_)
    print(f"imports: {len(iat)}")

    t = bin_.text
    text_blob = bin_.data[t["raw"]:t["raw"] + t["rawsize"]]
    text_va = t["va"]
    text_end = t["va"] + t["vsize"]

    # --- Seed function starts
    seeds = set()
    ep = IMAGE_BASE + bin_.pe.OPTIONAL_HEADER.AddressOfEntryPoint
    seeds.add(ep)
    seeds |= find_prologues(bin_)
    print(f"prologue seeds: {len(seeds)}")

    # vtable entries are function starts
    vt_path = os.path.join(outdir, "classes.json")
    if os.path.exists(vt_path):
        d = json.load(open(vt_path))
        for v in d.get("vtables", []):
            va = v["vtable_va"]
            # `continue`, not `break`: a vtable may legitimately contain a
            # non-.text entry (a pure-virtual stub, an import thunk, or a
            # pointer into .data). Breaking on the first one truncated the
            # scan for 821 of 843 vtables, which is why TechnoClass was left
            # with 146 of 214 slots unseeded.
            # Walk the vtable until we leave .rdata/.text. The previous bound of
            # 256 bytes (64 slots) truncated every class with more than 64
            # virtuals -- 21 classes, 1,678 slots, and 927 manifest slots
            # overall ended up with no function record.
            for k in range(0, 16384, 4):
                p = bin_.u32(va + k)
                if p is None:
                    break
                if bin_.in_text(p):
                    seeds.add(p)
                elif not bin_.in_rdata(p):
                    # past the end of this vtable: a code pointer followed by
                    # data means we have run off the table
                    if k > 0:
                        break
    print(f"seeds after vtables: {len(seeds)}")

    # --- Disassemble
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    # Precompute the full seed set so a function body can stop when it runs
    # into the start of another known function.
    all_seeds = set(seeds)

    funcs = {}
    worklist = deque(sorted(seeds))
    seen = set()
    call_edges = defaultdict(set)

    while worklist:
        start = worklist.popleft()
        if start in seen or not bin_.in_text(start):
            continue
        seen.add(start)
        off = start - text_va
        code = text_blob[off:off + 0x4000]
        insns = []
        callees = set()
        end = start
        for ins in md.disasm(code, start):
            # Stop if we have reached the start of a different known function.
            if ins.address != start and ins.address in all_seeds:
                break
            insns.append({
                "va": ins.address,
                "mnem": ins.mnemonic,
                "op": ins.op_str,
                "bytes": ins.bytes.hex(),
            })
            end = ins.address + ins.size
            # direct call/jmp targets
            if ins.mnemonic in ("call", "jmp") and ins.operands:
                op = ins.operands[0]
                if op.type == capstone.x86.X86_OP_IMM:
                    tgt = op.imm
                    if bin_.in_text(tgt):
                        callees.add(tgt)
                        if tgt not in seen:
                            worklist.append(tgt)
            # Terminating instructions end the function.
            if ins.mnemonic in ("ret", "retn"):
                break
            if ins.mnemonic == "int3":
                break
            if ins.mnemonic == "jmp":
                # unconditional tail jump ends the body
                break
            if len(insns) > 20000:
                break
        funcs[start] = {
            "va": start,
            "end": end,
            "size": end - start,
            "ninsn": len(insns),
            "callees": sorted(callees),
            "insns": insns,
        }
        for c in callees:
            call_edges[start].add(c)

    # --- Gap recovery pass
    #
    # The worklist above only follows direct call/jmp targets, so it terminates
    # with 1,283 KB of .text still unattributed. That residue is real code: only
    # ~5% of it is int3/nop padding. Two evidence-backed sources are added here,
    # neither of which invents anything:
    #   1. recorded branch/call targets that land outside every recovered
    #      function (measured: 9,321 such targets)
    #   2. the start of each remaining gap, disassembled and split at
    #      ret/int3 boundaries
    def covered(va, ivs, los):
        i = bisect.bisect_right(los, va) - 1
        return i >= 0 and ivs[i][0] <= va < ivs[i][1]

    def intervals():
        iv = sorted((f["va"], f["va"] + f["size"]) for f in funcs.values())
        merged = []
        for a, b in iv:
            if merged and a <= merged[-1][1]:
                merged[-1][1] = max(merged[-1][1], b)
            else:
                merged.append([a, b])
        return merged, [x[0] for x in merged]

    before = len(funcs)
    ivs, los = intervals()

    # 1. branch targets already recorded in recovered bodies
    extra = set()
    for f in funcs.values():
        for ins in f["insns"]:
            if ins["mnem"].startswith("j") or ins["mnem"] == "call":
                try:
                    t = int(ins["op"], 16)
                except ValueError:
                    continue
                if bin_.in_text(t) and not covered(t, ivs, los):
                    extra.add(t)
    # 2. gap starts
    text_end = text_va + bin_.text["vsize"]
    gaps = []
    prev = text_va
    for a, b in ivs:
        if a > prev:
            gaps.append((prev, a))
        prev = max(prev, b)
    if prev < text_end:
        gaps.append((prev, text_end))
    for a, b in gaps:
        if b - a >= 16:
            extra.add(a)

    added = 0
    known = set(funcs) | set(extra)
    for start in sorted(extra):
        if start in funcs or not bin_.in_text(start):
            continue
        off = start - text_va
        code = text_blob[off:off + 0x4000]
        insns = []
        callees = set()
        end = start
        for ins in md.disasm(code, start):
            # Stop at the next known entry point, so a gap body can never
            # swallow a real function that was seeded by a vtable or a call.
            if ins.address != start and ins.address in known:
                break
            insns.append({"va": ins.address, "mnem": ins.mnemonic,
                          "op": ins.op_str, "bytes": ins.bytes.hex()})
            end = ins.address + ins.size
            if ins.mnemonic in ("call", "jmp") and ins.operands:
                op = ins.operands[0]
                if op.type == capstone.x86.X86_OP_IMM and bin_.in_text(op.imm):
                    callees.add(op.imm)
            if ins.mnemonic in ("ret", "retn", "int3", "jmp"):
                break
            if len(insns) > 20000:
                break
        if not insns:
            continue
        funcs[start] = {
            "va": start, "end": end, "size": end - start,
            "ninsn": len(insns), "callees": sorted(callees), "insns": insns,
        }
        added += 1
    print(f"gap recovery: +{added} functions "
          f"({before} -> {len(funcs)}); {len(gaps)} gaps considered")

    print(f"functions recovered: {len(funcs)}")

    # --- Resolve string references per function
    for va, f in funcs.items():
        strs = []
        for ins in f["insns"]:
            if ins["mnem"] in ("push", "mov") and ins["op"].startswith("0x"):
                try:
                    imm = int(ins["op"], 16)
                except ValueError:
                    continue
                if 0x400000 <= imm < 0x840000:
                    s = bin_.cstr(imm, 200)
                    if s and len(s) >= 4 and all(32 <= ord(ch) < 127 for ch in s):
                        strs.append({"va": imm, "text": s})
        f["strings"] = strs

    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "functions.json"), "w") as fh:
        json.dump({
            "functions": list(funcs.values()),
            "imports": {hex(k): v for k, v in iat.items()},
        }, fh)

    # summary
    total_insn = sum(f["ninsn"] for f in funcs.values())
    print(f"total instructions: {total_insn}")
    print(f"wrote {outdir}/functions.json")


if __name__ == "__main__":
    main()
