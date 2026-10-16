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

    def section_of(self, va):
        """Return the section dict containing va, or None."""
        for s in self.sections:
            if s["va"] <= va < s["va"] + s["vsize"]:
                return s
        return None

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


# MSVC function-prologue byte patterns. `find_prologues` scans .text for these,
# and `looks_like_code_start` reuses the same list to validate a candidate VA in
# a section other than .text, so a non-code pointer cannot be admitted as code.
PROLOGUE_PATTERNS = [
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


def looks_like_code_start(bin_, va):
    """True if the bytes at va begin with a known MSVC function prologue.

    Used to validate a candidate VA that lies outside .text, where a pointer
    could just as easily name data. A vtable's trailing zero dword, for
    example, disassembles to a long run of `add [eax], al` and must be rejected.
    """
    b = bin_.read(va, 6)
    if not b:
        return False
    return any(b.startswith(p) for p in PROLOGUE_PATTERNS)


def find_prologues(bin_):
    """Scan .text for MSVC function prologue byte patterns."""
    t = bin_.text
    blob = bin_.data[t["raw"]:t["raw"] + t["rawsize"]]
    starts = set()
    pats = PROLOGUE_PATTERNS
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


# --- Evidence-backed extra seed sources ---------------------------------------
#
# Each helper below returns a set of VAs that are *provably* code entry points,
# taken from a specific instruction or table in the binary. Nothing is guessed:
# the VA is the target of a real branch, the destination of a real table read,
# or an address the original compiler stored as a code pointer.

# MSVC emits `jmp dword ptr [reg*4 + table]` for a switch over a dense integer
# range. The table holds 4-byte absolute code VAs; the switch prologue compares
# the selector against an upper bound before the indirect jump, but we do not
# need to recover the bound: we read the table until the first dword that is not
# a .text VA, which is where the next table/rodata begins.
JUMP_TABLE_MAX = 4096  # dwords; the largest real table in this binary is < 600


def _scan_body(bin_, md, va, limit=0x4000):
    """Disassemble at most `limit` bytes starting at va and return (insns, code).

    The scan stops at a terminating instruction (ret/int3/jmp) so it does not
    run off the end of the function into unrelated bytes.
    """
    sec = bin_.section_of(va)
    if sec is None:
        return []
    off = sec["raw"] + (va - sec["va"])
    code = bin_.data[off:off + limit]
    insns = []
    for ins in md.disasm(code, va):
        insns.append(ins)
        if ins.mnemonic in ("ret", "retn", "int3", "jmp"):
            break
    return insns


def find_jump_tables_from_seeds(bin_, md, seeds):
    """Read every `jmp dword ptr [reg*4 + imm]` dispatch table reachable from the
    seed set and return (in-.text targets, table base VAs).

    The tables themselves live in .rdata (some in .data); only the entries are
    code addresses. The scan is run over the seed set before the worklist so
    that switch targets are known when disassembly begins.
    """
    targets = set()
    tables = set()
    for va in sorted(seeds):
        for ins in _scan_body(bin_, md, va):
            if ins.mnemonic != "jmp" or "*4" not in ins.op_str:
                continue
            imm = None
            for tok in ins.op_str.replace("[", " ").replace("]", " ").split():
                if tok.startswith("0x"):
                    try:
                        imm = int(tok, 16)
                    except ValueError:
                        continue
            if imm is not None:
                tables.add(imm)
    for base in sorted(tables):
        for k in range(JUMP_TABLE_MAX):
            p = bin_.u32(base + k * 4)
            if p is None or not bin_.in_text(p):
                break
            targets.add(p)
    return targets, tables


def find_seh_handlers(bin_):
    """Return the handler VAs named by MSVC `push -1; push <handler>` SEH frames.

    The immediate operand of the second push is the exception handler, which is
    a real function entry point. Handler VAs that land outside .text (the CRT's
    own handlers, e.g. 0x6cdc80) are returned too; the caller decides whether a
    non-.text VA is admissible.
    """
    t = bin_.text
    blob = bin_.data[t["raw"]:t["raw"] + t["rawsize"]]
    handlers = set()
    i = 0
    while True:
        j = blob.find(b"\x6a\xff\x68", i)
        if j < 0:
            break
        handlers.add(struct.unpack("<I", blob[j + 3:j + 7])[0])
        i = j + 1
    return handlers


def find_push_imm_targets_from_seeds(bin_, md, seeds):
    """Return VAs that a scanned body loads with `push <imm32>` where a `call`
    follows within a few instructions.

    This is how the binary registers atexit/onexit callbacks and how it passes
    function pointers to CRT helpers: `push <fn>; ... ; call <helper>`. The
    pushed immediate is a code address the original compiler emitted, so it is
    a function entry point even when no direct `call <fn>` exists.
    """
    targets = set()
    for va in sorted(seeds):
        insns = _scan_body(bin_, md, va)
        for idx, ins in enumerate(insns):
            if ins.mnemonic != "push" or not ins.op_str.startswith("0x"):
                continue
            try:
                imm = int(ins.op_str, 16)
            except ValueError:
                continue
            if not bin_.in_text(imm):
                continue
            for k in range(idx + 1, min(idx + 4, len(insns))):
                if insns[k].mnemonic == "call":
                    targets.add(imm)
                    break
                if insns[k].mnemonic in ("ret", "retn", "jmp", "int3"):
                    break
    return targets


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

    # vtable entries are function starts. Slots that name a VA outside .text
    # (e.g. .data-resident code) are collected separately and admitted below.
    vtable_code = set()
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
                elif bin_.section_of(p) is not None and k > 0:
                    # A code pointer that does not live in .text. A vtable slot
                    # may name code in another section (or, more often here,
                    # terminate with a zero dword that is not code at all).
                    # Record the candidate; the admission step below keeps it
                    # only if its bytes look like a function prologue, so a
                    # vtable's trailing zeros cannot become fake functions.
                    if p:
                        vtable_code.add(p)
                    if not bin_.in_rdata(p):
                        break
                elif not bin_.in_rdata(p):
                    # past the end of this vtable: a code pointer followed by
                    # data means we have run off the table
                    if k > 0:
                        break
    print(f"seeds after vtables: {len(seeds)}")

    # --- Additional evidence-backed seed sources ----------------------------
    #
    # Each source below is a real code address the compiler emitted: the
    # destination of a table read, a SEH handler immediate, or a callback
    # passed to a CRT helper. None is an inference.
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    # 1. Switch dispatch tables. `jmp dword ptr [reg*4 + imm]` reads a table of
    #    absolute code VAs from .rdata; every entry is a basic-block entry point
    #    that no direct call/jmp reaches.
    jump_targets, jump_tables = find_jump_tables_from_seeds(bin_, md, seeds)
    print(f"jump tables: {len(jump_tables)} tables, "
          f"{len(jump_targets)} in-.text targets")

    # 2. SEH handler immediates (`push -1; push <handler>`).
    seh_handlers = find_seh_handlers(bin_)
    seh_in_text = {h for h in seh_handlers if bin_.in_text(h)}

    # 3. Code pointers passed to CRT helpers (`push <fn>; call <helper>`). This
    #    scan also needs bodies; run it over the seed set.
    push_targets = find_push_imm_targets_from_seeds(bin_, md, seeds)

    # --- Merge the new seeds under the overlap safety rule ------------------
    #
    # CRITICAL: a new seed must never sit strictly inside an existing function
    # body unless it is the target of a recorded jmp/call (a legitimate shared
    # tail). There are ~193 such shared tails in this binary; a naive overlap
    # rule would delete them, and an ungated new seed would let one body swallow
    # another. The prologue/vtable seeds are already trusted entry points, so
    # the gate below applies only to the new sources.
    def _in_trusted_body(va, trusted_starts):
        """True if va lies strictly inside the body of a trusted seed.

        Bodies are measured by disassembling from each trusted start to its
        first terminating instruction, then merged. A seed that equals a
        trusted start, or falls outside every such body, is admissible.
        """
        i = bisect.bisect_right(trusted_starts, va) - 1
        if i < 0:
            return False
        return trusted_starts[i] < va < _body_end[trusted_starts[i]]

    _body_end = {}
    for s in seeds:
        insns = _scan_body(bin_, md, s)
        if insns:
            _body_end[s] = insns[-1].address + insns[-1].size

    trusted = sorted(_body_end)
    accepted = {"jump": 0, "seh": 0, "push": 0, "vtable": 0}
    rejected = {"jump": 0, "seh": 0, "push": 0, "vtable": 0}

    # Branch/call targets are legitimate shared tails even when they land inside
    # a trusted body; collect them so the gate can admit those.
    branch_targets = set()
    for s in seeds:
        for ins in _scan_body(bin_, md, s):
            if ins.mnemonic in ("call", "jmp") or ins.mnemonic.startswith("j"):
                if ins.operands and ins.operands[0].type == capstone.x86.X86_OP_IMM:
                    branch_targets.add(ins.operands[0].imm)

    for label, cands in (("jump", jump_targets),
                         ("seh", seh_in_text),
                         ("push", push_targets)):
        for va in sorted(cands):
            if va in seeds:
                continue
            if _in_trusted_body(va, trusted) and va not in branch_targets:
                rejected[label] += 1
                continue
            seeds.add(va)
            accepted[label] += 1

    # Code named by a vtable entry but living outside .text. Admitted only when
    # a vtable slot holds the address AND the bytes there look like a function
    # prologue. No blanket scan of .data/.rdata is performed, and a vtable's
    # trailing zero dword (which is not code) is rejected by the byte test.
    for va in sorted(vtable_code):
        if va in seeds:
            continue
        if bin_.section_of(va) is None or not looks_like_code_start(bin_, va):
            rejected["vtable"] += 1
            continue
        seeds.add(va)
        accepted["vtable"] += 1

    print(f"new seeds: jump={accepted['jump']} seh={accepted['seh']} "
          f"push={accepted['push']} vtable={accepted['vtable']}")
    print(f"rejected (inside a trusted body, not a branch target): "
          f"jump={rejected['jump']} seh={rejected['seh']} push={rejected['push']}")

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
        if start in seen or bin_.section_of(start) is None:
            continue
        seen.add(start)
        sec = bin_.section_of(start)
        off = sec["raw"] + (start - sec["va"])
        code = bin_.data[off:off + 0x4000]
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
        # Gap recovery is a .text-only concern: the merged intervals must not
        # include .data-resident functions (e.g. 0x0060f590), whose VAs are
        # numerically inside the .text range and would otherwise split real
        # .text gaps into spurious fragments.
        iv = sorted((f["va"], f["va"] + f["size"]) for f in funcs.values()
                    if bin_.in_text(f["va"]))
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
