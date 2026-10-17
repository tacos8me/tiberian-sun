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
  3. Extend the worklist to a fixpoint: every body feeds back its direct
     call/jmp targets, every branch target (conditional jumps included) and the
     in-.text entries of any switch table it dispatches through, and the start
     of each remaining unattributed gap.
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




def read_jump_table(bin_, base):
    """In-.text entries of the switch table at `base`.

    The table ends at the first dword that is not a .text VA, which is where the
    next table or the rodata begins. Do NOT keep reading past it: table
    0x663f24 legitimately ends after 7 entries.
    """
    targets = []
    for k in range(JUMP_TABLE_MAX):
        p = bin_.u32(base + k * 4)
        if p is None or not bin_.in_text(p):
            break
        targets.append(p)
    return targets


def jump_table_base(ins):
    """Table base VA of a `jmp dword ptr [reg*4 + imm]` dispatch, else None.

    MSVC emits `jmp dword ptr [reg*4 + table]` for a switch over a dense integer
    range. The table holds 4-byte absolute code VAs; the selector bound is not
    recovered here because the table's end is unambiguous -- the first dword
    that is not a .text VA.
    """
    if ins.mnemonic != "jmp" or "*4" not in ins.op_str:
        return None
    imm = None
    for tok in ins.op_str.replace("[", " ").replace("]", " ").split():
        if tok.startswith("0x"):
            try:
                imm = int(tok, 16)
            except ValueError:
                continue
    return imm


def push_imm_targets(bin_, insns):
    """VAs a body loads with `push <imm32>` where a `call` follows within a few
    instructions.

    This is how the binary registers atexit/onexit callbacks and how it passes
    function pointers to CRT helpers: `push <fn>; ... ; call <helper>`. The
    pushed immediate is a code address the original compiler emitted, so it is a
    function entry point even when no direct `call <fn>` exists.
    """
    targets = set()
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


class CoveredText:
    """Merged .text intervals of recovered bodies, for the overlap gate.

    Bodies are added as they are recovered and the intervals are kept merged, so
    a single bisect answers "does va sit strictly inside an already-recovered
    function body?" -- the question the safety gate asks of every new start.
    Non-.text bodies are never added: their VAs are numerically inside the .text
    range (e.g. 0x0060f590) and would otherwise split real .text gaps.
    """

    def __init__(self):
        self.ivs = []
        self.los = []

    def add(self, a, b):
        if b <= a:
            return
        i = bisect.bisect_left(self.los, a)
        lo, hi = a, b
        if i > 0 and self.ivs[i - 1][1] >= a:
            i -= 1
            lo = min(lo, self.ivs[i][0])
            hi = max(hi, self.ivs[i][1])
        j = i
        while j < len(self.ivs) and self.ivs[j][0] <= hi:
            hi = max(hi, self.ivs[j][1])
            j += 1
        self.ivs[i:j] = [[lo, hi]]
        self.los[i:j] = [lo]

    def contains_strict(self, va):
        i = bisect.bisect_right(self.los, va) - 1
        return i >= 0 and va < self.ivs[i][1]


def text_intervals(funcs, bin_):
    """Merged .text intervals of the recovered .text functions."""
    iv = sorted((f["va"], f["va"] + f["size"]) for f in funcs.values()
                if bin_.in_text(f["va"]))
    merged = []
    for a, b in iv:
        if merged and a <= merged[-1][1]:
            merged[-1][1] = max(merged[-1][1], b)
        else:
            merged.append([a, b])
    return merged


def strip_leading_padding(bin_, va, limit):
    """Advance past leading 0x00/0x90/0xcc padding before seeding a gap start.

    A gap usually opens with the previous function's alignment padding; seeding
    the raw gap start disassembles a run of nops (1,598 of 1,884 candidate
    starts begin `90 90`) instead of the function that follows it.
    """
    while va < limit:
        b = bin_.read(va, 1)
        if not b or b[0] not in (0x00, 0x90, 0xcc):
            break
        va += 1
    return va

# The gap seeder works at a 16-byte minimum: most shorter gaps are the previous
# function's alignment residue, and seeding them decodes padding. A short gap that
# decodes to instructions filling it exactly and ending in a transfer is not
# residue, and `short_gap_body` admits it. 0x6905b1 is the case that named this:
# `push [0x834c60]; call [0x698108]; ret`, 13 bytes, no prologue, called by the
# program and named by the code-pointer table at 0x6be3f0.
GAP_MIN = 16


def short_gap_body(md, bin_, a, b):
    """True if a..b is exactly one prologue-less function and nothing else.

    The test is byte-exact rather than heuristic: the sweep from the stripped gap
    start must consume the gap to its last byte and stop on a control transfer. A
    run of `nop`-form fill, a fragment of a table, or a body that would continue
    past the gap all fail it, so the minimum stays in force for those.
    """
    s = strip_leading_padding(bin_, a, b)
    if s >= b:
        return False
    sec = bin_.section_of(s)
    off = sec["raw"] + (s - sec["va"])
    end = s
    last = None
    for ins in md.disasm(bin_.data[off:off + (b - s)], s):
        if ins.address + ins.size > b:
            return False
        last = ins
        end = ins.address + ins.size
        if ins.mnemonic in ("ret", "retn", "jmp"):
            break
    return (last is not None and end == b
            and last.mnemonic in ("ret", "retn", "jmp"))

# --- Residual .text: jump tables and padding --------------------------------
#
# Function discovery leaves about 104 KB of .text unattributed. It is not
# unknown code: it is the switch tables MSVC emits inline in .text plus the
# linker's alignment padding between functions. Both are identified from the
# bytes rather than assumed, and whatever is neither is reported instead of
# being forced into a category:
#
#   * A jump table is a run of dwords that are all .text addresses -- the same
#     evidence `read_jump_table` uses for a table a `jmp` names, applied by
#     content so that a table whose dispatcher sits in a body that was never
#     recovered is still found. The table at 0x683e24 is one of those: no
#     recovered instruction references it, and the code entry points it names
#     are the only reason the block at 0x683e44 is not recovered.
#   * The companion byte array MSVC emits after a switch's target table is the
#     run of bytes below 0x10 that follows it. Its exact length is not
#     recoverable from the bytes alone, so the bytes are emitted with the run
#     and counted apart from the targets.
#   * Padding is a run of fill bytes (0x00, 0x90, 0xcc) or of the multi-byte
#     no-op and hot-patch encodings MSVC emits between functions.
#   * Everything else is listed verbatim in out/text_data.json. In this binary
#     that is 568 bytes: code fragments shorter than the 16-byte minimum the
#     gap seeder works at, plus two 53-byte blocks that are the tail of an
#     unreferenced switch's copy routine.
#
# The two figures this produces are kept apart on purpose: the code figure
# counts recovered function spans, the classified figure counts tables and
# padding, and neither is allowed to stand in for the other.

_FILL = (0x00, 0x90, 0xcc)

# The multi-byte no-op and hot-patch fill forms MSVC emits: `mov edi,edi` (the
# 2-byte hot-patch no-op), the `lea`-based long no-ops, and `xchg ax,ax`.
_NOP_FORMS = [bytes.fromhex(h) for h in (
    "8bff", "8d4900", "8d642400", "8da42400000000", "8d9b00000000",
    "8db600000000", "8d8000000000", "8dbf00000000", "6690",
    "0f1f00", "0f1f4000", "0f1f440000", "0f1f8000000000", "0f1f840000000000")]
_NOP_FORMS.sort(key=len, reverse=True)


def nop_form_len(blob, i):
    """Length of the no-op/hot-patch encoding at `i`, or 0."""
    for p in _NOP_FORMS:
        if blob.startswith(p, i):
            return len(p)
    return 0


def dword_run(blob, i, bin_):
    """Number of consecutive dwords at `i` that are all .text addresses."""
    n = 0
    while i + 4 * (n + 1) <= len(blob):
        if not bin_.in_text(struct.unpack_from("<I", blob, i + 4 * n)[0]):
            break
        n += 1
    return n


def classify_residual(bin_, funcs):
    """Attribute every .text byte that no recovered function covers."""
    t = bin_.text
    text_va = t["va"]
    text_end = t["va"] + t["vsize"]
    ivs = text_intervals(funcs, bin_)
    gaps = []
    prev = text_va
    for a, b in ivs:
        if a > prev:
            gaps.append((prev, a))
        prev = max(prev, b)
    if prev < text_end:
        gaps.append((prev, text_end))

    tables = []
    index_tables = []
    other = []
    table_bytes = index_bytes = pad_bytes = other_bytes = pad_runs = 0
    fill_counts = {}
    for a, b in gaps:
        raw = bin_.data[t["raw"] + (a - text_va): t["raw"] + (b - text_va)]
        n = len(raw)
        i = 0
        while i < n:
            k = dword_run(raw, i, bin_)
            if k >= 2:
                tables.append({
                    "va": a + i,
                    "size": 4 * k,
                    "targets": [struct.unpack_from("<I", raw, i + 4 * j)[0]
                                for j in range(k)]})
                table_bytes += 4 * k
                i += 4 * k
                j = i
                while j < n and raw[j] < 0x10:
                    j += 1
                if j > i:
                    index_tables.append({"va": a + i, "size": j - i,
                                         "bytes": raw[i:j].hex()})
                    index_bytes += j - i
                    i = j
                continue
            if raw[i] in _FILL:
                j = i
                while j < n and raw[j] in _FILL:
                    j += 1
                key = "fill_%02x" % raw[i]
                fill_counts[key] = fill_counts.get(key, 0) + (j - i)
                pad_bytes += j - i
                pad_runs += 1
                i = j
                continue
            length = nop_form_len(raw, i)
            if length:
                key = raw[i:i + length].hex()
                fill_counts[key] = fill_counts.get(key, 0) + 1
                pad_bytes += length
                pad_runs += 1
                i += length
                continue
            # Neither a table nor padding: report the run verbatim.
            j = i + 1
            while j < n and raw[j] not in _FILL and not nop_form_len(raw, j) \
                    and dword_run(raw, j, bin_) < 2:
                j += 1
            other.append({"va": a + i, "size": j - i, "bytes": raw[i:j].hex()})
            other_bytes += j - i
            i = j

    covered = sum(b - a for a, b in ivs)
    return {
        "text": {"va": text_va, "size": text_end - text_va},
        "code_bytes": covered,
        "jump_tables": tables,
        "jump_tables_bytes": table_bytes,
        "switch_index_tables": index_tables,
        "switch_index_tables_bytes": index_bytes,
        "padding": {"bytes": pad_bytes, "runs": pad_runs,
                    "by_form": fill_counts},
        "padding_bytes": pad_bytes,
        "unclassified": other,
        "unclassified_bytes": other_bytes,
        "unattributed_bytes": text_end - text_va - covered,
    }


def print_text_attribution(data):
    """.text attribution: code, classified data, and what is left over.

    The code figure is recovered function spans and nothing else. The
    classified figure is jump tables plus padding, and it is printed beside the
    code figure rather than added into it, so "attributed" cannot be read as
    "recovered code".
    """
    total = data["text"]["size"]
    code = data["code_bytes"]
    classified = (data["jump_tables_bytes"] + data["switch_index_tables_bytes"]
                  + data["padding_bytes"])
    print(f".text attribution: {total} bytes")
    print(f"  code (recovered function spans)  {code} bytes "
          f"({100.0 * code / total:.2f}%)")
    print(f"  jump tables                      {data['jump_tables_bytes']} "
          f"bytes in {len(data['jump_tables'])} tables")
    print(f"  switch index tables              "
          f"{data['switch_index_tables_bytes']} bytes in "
          f"{len(data['switch_index_tables'])} runs")
    print(f"  alignment padding                {data['padding_bytes']} bytes "
          f"in {data['padding']['runs']} runs")
    print(f"  attributed as code or data       "
          f"{100.0 * (code + classified) / total:.2f}%")
    print(f"  unclassified (reported)          {data['unclassified_bytes']} "
          f"bytes in {len(data['unclassified'])} runs -- see "
          f"out/text_data.json")


def pointer_table_targets(bin_, funcs, iat):
    """Entry points the program's own code-pointer tables name.

    A dword in a data section that points into .text is a code pointer the
    original compiler stored. A *run* of consecutive such dwords is a table of
    them -- a static-initializer list, a per-class dispatch table -- and such a
    table is self-evidencing: when its other entries are already recovered
    function starts and one entry is not, that entry is a function start the
    sweep swallowed. 0x6bb170 holds 0x4034f0 -- the static initializer
    `fldl 0x6988b0; fmull 0x6985f0; fstpl 0x714e28` -- inside a run of 28 dwords
    of which the other 27 are function starts. The boot calls 0x4034f0 and
    nothing was registered there, which is how the case was found; the table is
    what makes it fixable without the boot.

    IAT slots are skipped. Their dwords are the addresses of import thunks in the
    file and of stubs at run time, not code the program wrote down, and the
    0x698000..0x698580 block would otherwise dominate the result.

    A named VA that sits in a *gap* is admitted as well, and there the table is
    the only evidence there is: no span covers those bytes, so nothing can be
    split. 0x6be3f0 names 0x6905b1 -- `push [0x834c60]; call [0x698108]; ret`, a
    13-byte leaf with no prologue -- which the gap seeder never reached because it
    works at a 16-byte minimum. The boot calls it too.

    A candidate inside a span is also required to be an instruction boundary of
    that span: the sweep already decoded an instruction there, so splitting loses
    and duplicates nothing. Candidates that are not a boundary are returned apart
    and reported -- they say the containing span's sweep is misaligned, which is a
    defect in that span rather than a missing entry point.

    Returns ({va: slot_va}, set_of_non_boundary_candidates).
    """
    starts = set(funcs)
    bounds = sorted((f["va"], f["va"] + f["size"]) for f in funcs.values()
                    if bin_.in_text(f["va"]))
    los = [b[0] for b in bounds]
    host_insns = {}

    def host_of(va):
        i = bisect.bisect_right(los, va) - 1
        return bounds[i] if i >= 0 and va < bounds[i][1] else None

    def is_boundary(host_start, va):
        vas = host_insns.get(host_start)
        if vas is None:
            vas = host_insns[host_start] = set(i["va"] for i in
                                               funcs[host_start]["insns"])
        return va in vas

    found = {}
    unbounded = set()
    for sec in bin_.sections:
        if sec["name"] == ".text":
            continue
        raw = bin_.data[sec["raw"]:sec["raw"] + sec["rawsize"]]
        n = len(raw) // 4
        vals = [struct.unpack_from("<I", raw, 4 * i)[0] for i in range(n)]
        i = 0
        while i < n:
            if sec["va"] + 4 * i in iat or not bin_.in_text(vals[i]):
                i += 1
                continue
            j = i
            while (j < n and sec["va"] + 4 * j not in iat
                   and bin_.in_text(vals[j])):
                j += 1
            # A single stray dword is not a table; a run whose entries are all
            # unknown says nothing either. A run that already names recovered
            # function starts is a table, and its remaining entries are entries.
            if j - i >= 2 and any(v in starts for v in vals[i:j]):
                for k in range(i, j):
                    va = vals[k]
                    if va in starts:
                        continue
                    host = host_of(va)
                    if not host or is_boundary(host[0], va):
                        found.setdefault(va, sec["va"] + 4 * k)
                    else:
                        unbounded.add(va)
            i = j
    return found, unbounded


def recover(bin_, seeds, vtable_code):
    """Run the worklist to a fixpoint and return the recovered bodies.

    Every body feeds its successors back in (direct call/jmp targets, branch
    targets, switch-table entries, code pointers pushed to a CRT helper); when
    the worklist empties the remaining unattributed gaps are seeded and it runs
    again, until no gap yields a new body. Nothing here invents a start: each VA
    is the target of a real branch, the destination of a real table read, the
    first non-padding byte of a gap, or a seed the caller supplies.

    It is a function rather than a block because a start can only be *recognised*
    from a recovered body -- a data-section pointer table names entry points, and
    "the entry the table names is inside a span" is only visible afterwards. The
    caller re-runs this from scratch with the new seed instead of patching a
    recovered span, so every seed is admitted before any sweep begins and a body
    stops at it. That is what keeps instruction coverage at exactly 100%: a split
    made after the fact would decode the same bytes twice.
    """
    text_va = bin_.text["va"]
    text_end = text_va + bin_.text["vsize"]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    # 1. SEH handler immediates (`push -1; push <handler>`).
    seh_handlers = find_seh_handlers(bin_)
    seh_in_text = {h for h in seh_handlers if bin_.in_text(h)}

    # 2. Code named by a vtable entry but living outside .text. Admitted only
    #    when a vtable slot holds the address AND the bytes there look like a
    #    function prologue. No blanket scan of .data/.rdata is performed, and a
    #    vtable's trailing zero dword (which is not code) is rejected by the
    #    byte test.
    ext_seeds = set()
    ext_rejected = 0
    for va in sorted(vtable_code):
        if va in seeds:
            continue
        if bin_.section_of(va) is None or not looks_like_code_start(bin_, va):
            ext_rejected += 1
            continue
        ext_seeds.add(va)

    # --- Fixpoint worklist --------------------------------------------------
    #
    # Every body disassembled from any start feeds its successors back into the
    # worklist: direct call/jmp targets, conditional branch targets, the in-.text
    # entries of any switch table it dispatches through, and code pointers it
    # pushes to a CRT helper. The outer loop then seeds the remaining
    # unattributed gaps (after stripping their leading padding) and runs again,
    # until no gap yields a new body. Nothing here invents a start: each VA is
    # the target of a real branch, the destination of a real table read, or the
    # first non-padding byte of a gap.
    #
    # SAFETY: a start is admitted only when it is not strictly inside an
    # already-recovered body, unless a recorded direct call/jmp reaches it (a
    # legitimate shared tail). Intra-body control flow -- a forward conditional
    # jump to the next basic block, say -- must not split the body containing
    # it. Accepted and rejected counts are printed per source.
    funcs = {}
    seen = set()
    call_edges = defaultdict(set)
    admitted = set(seeds) | ext_seeds
    direct_branch_targets = set()
    seen_tables = set()
    accepted = defaultdict(int)
    rejected = defaultdict(int)

    worklist = deque((s, "seed") for s in sorted(seeds))
    for s in sorted(ext_seeds):
        worklist.append((s, "vtable"))
    for s in sorted(seh_in_text):
        worklist.append((s, "seh"))

    covered = CoveredText()

    def _queue(va, source):
        if not bin_.section_of(va):
            return
        if source != "vtable" and not bin_.in_text(va):
            return
        if va in seen:
            return
        admitted.add(va)
        worklist.append((va, source))

    def _gate(va, source):
        if source in ("seed", "call", "jmp"):
            return True
        if va in direct_branch_targets:
            return True
        return not covered.contains_strict(va)

    def _recover(start):
        """Linear-sweep disassemble one body and return its candidates."""
        sec = bin_.section_of(start)
        off = sec["raw"] + (start - sec["va"])
        code = bin_.data[off:off + 0x4000]
        insns = []
        raw = []
        callees = set()
        cond = set()
        tables = []
        end = start
        for ins in md.disasm(code, start):
            # Stop at the start of any other known function so a body can never
            # swallow a real entry point.
            if ins.address != start and ins.address in admitted:
                break
            insns.append({
                "va": ins.address,
                "mnem": ins.mnemonic,
                "op": ins.op_str,
                "bytes": ins.bytes.hex(),
            })
            raw.append(ins)
            end = ins.address + ins.size
            if ins.mnemonic in ("call", "jmp") and ins.operands:
                op = ins.operands[0]
                if op.type == capstone.x86.X86_OP_IMM:
                    tgt = op.imm
                    if bin_.in_text(tgt):
                        callees.add(tgt)
                if ins.mnemonic == "jmp":
                    base = jump_table_base(ins)
                    if base is not None:
                        tables.append(base)
            elif ins.mnemonic.startswith("j") and ins.operands:
                op = ins.operands[0]
                if op.type == capstone.x86.X86_OP_IMM:
                    cond.add(op.imm)
            if ins.mnemonic in ("ret", "retn", "int3", "jmp"):
                break
            if len(insns) > 20000:
                break
        return insns, raw, callees, cond, tables, end

    gap_tried = set()
    gap_rounds = 0
    while True:
        while worklist:
            start, source = worklist.popleft()
            if start in seen:
                continue
            if not _gate(start, source):
                rejected[source] += 1
                continue
            if not bin_.section_of(start):
                continue
            if source == "vtable" and not looks_like_code_start(bin_, start):
                rejected[source] += 1
                continue
            seen.add(start)
            accepted[source] += 1
            insns, raw, callees, cond, tables, end = _recover(start)
            if not insns:
                continue
            funcs[start] = {
                "va": start,
                "end": end,
                "size": end - start,
                "ninsn": len(insns),
                "callees": sorted(callees),
                "insns": insns,
            }
            if bin_.in_text(start):
                covered.add(start, end)
            for c in callees:
                call_edges[start].add(c)
                direct_branch_targets.add(c)
                _queue(c, "call")
            for c in cond:
                _queue(c, "branch")
            for base in tables:
                if base in seen_tables:
                    continue
                seen_tables.add(base)
                for tgt in read_jump_table(bin_, base):
                    _queue(tgt, "table")
            for tgt in push_imm_targets(bin_, raw):
                _queue(tgt, "push")

        # --- Gap seeding: the remaining unattributed bytes.
        ivs = text_intervals(funcs, bin_)
        gaps = []
        prev = text_va
        for a, b in ivs:
            if a > prev:
                gaps.append((prev, a))
            prev = max(prev, b)
        if prev < text_end:
            gaps.append((prev, text_end))
        progressed = False
        for a, b in gaps:
            if b - a < GAP_MIN and not short_gap_body(md, bin_, a, b):
                continue
            s = strip_leading_padding(bin_, a, b)
            if s >= b or s in seen or s in gap_tried:
                continue
            gap_tried.add(s)
            admitted.add(s)
            worklist.append((s, "gap"))
            progressed = True
        if not progressed:
            break
        gap_rounds += 1

    print(f"jump tables: {len(seen_tables)} tables; "
          f"gap rounds: {gap_rounds}")
    srcs = sorted(set(accepted) | set(rejected))
    print("seed admission accepted/rejected: " + " ".join(
        f"{s}={accepted[s]}/{rejected[s]}" for s in srcs))
    print(f"rejected = strictly inside a recovered body and not a recorded "
          f"call/jmp target; {ext_rejected} non-.text vtable pointers rejected "
          f"by the prologue byte test")
    print(f"functions recovered: {len(funcs)}")
    return funcs


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    bin_ = Binary(path)
    iat = load_imports(bin_)
    print(f"imports: {len(iat)}")

    t = bin_.text
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

    # --- Entry points the pointer tables name -------------------------------
    #
    # A recovered body is the only place a swallowed entry point is visible, so the
    # seed set grows by re-running the whole worklist: a start added between rounds
    # is in `admitted` before any sweep begins, so the body that contained it stops
    # at it instead of being truncated afterwards. Each round is deterministic and
    # the seed set only grows, so this terminates; the loop is unbounded rather than
    # capped because a cap would silently leave the last batch unseeded.
    round_no = 0
    while True:
        funcs = recover(bin_, seeds, vtable_code)
        found, unbounded = pointer_table_targets(bin_, funcs, iat)
        extra = {va: slot for va, slot in found.items() if va not in seeds}
        if not extra:
            print(f"pointer-table rounds: {round_no}; no further entry point named")
            break
        round_no += 1
        print(f"pointer-table round {round_no}: {len(extra)} entry point(s) named "
              f"by a data-section code-pointer table; {len(unbounded)} candidate(s) "
              f"rejected as not an instruction boundary")
        for va in sorted(extra)[:12]:
            print(f"  0x{va:08x} <- dword at 0x{extra[va]:08x}")
        if len(extra) > 12:
            print(f"  ... {len(extra) - 12} more")
        seeds |= set(extra)


    # --- Remaining unattributed .text: jump tables, padding, and what is left
    data = classify_residual(bin_, funcs)
    print_text_attribution(data)

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
    with open(os.path.join(outdir, "text_data.json"), "w") as fh:
        json.dump(data, fh, indent=1, sort_keys=True)

    # summary
    total_insn = sum(f["ninsn"] for f in funcs.values())
    print(f"total instructions: {total_insn}")
    print(f"wrote {outdir}/functions.json")
    print(f"wrote {outdir}/text_data.json")


if __name__ == "__main__":
    main()
