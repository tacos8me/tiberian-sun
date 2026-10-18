#!/usr/bin/env python3
"""Unit tests for the load-bearing tools.

`tools/verify.py` tests ARTIFACTS -- that the headers compile, that the manifest
covers the classes, that the member offsets match the recovered table. It never
tests the tools that produce them, so a tool can be wrong in a way that produces
self-consistent output and nothing notices.

Every test below is a REGRESSION TEST for a real defect that shipped, or a
differential test against an independent implementation. That is the bar for
adding one: if it cannot fail, it is not worth the runtime.

  emit_x86 masking        -- `cmp cx, -1` passed 0xffffffff to a u16 helper
  emit_x86 in/out order   -- every `in` emitted `0x5fu = ts::in_op(EAX.d)`
  emit_x86 loop tails     -- a `loop` out of span emitted a goto to no label
  gen_bodies destructors  -- `\\b` before the name meant NO destructor matched
  gen_bodies slot VA      -- vtable-block addresses were not read
  reduce_bases            -- the numContainedBases walk, on a synthetic array
  Machine mul             -- differential against 64-bit reference arithmetic
  Machine div             -- idiv is SIGNED: -1/-1=1, not a false overflow
  Machine flags           -- add/sub carry and overflow, against the manual
  Machine stack           -- push/pop balance
  Machine mmx lanes       -- differential against lane-array reference code
  Machine boot regressions -- call_ptr balance, rep ECX, jmp_indirect, maps
  gen_code tails          -- cross-function tail targets are found
  disasm jump tables      -- table bytes are data, their entries are starts
  Win32 module exports    -- handles name modules, CoCreateInstance writes *ppv
  Win32 message queue     -- WM_ACTIVATEAPP was never queued, so PeekMessageA
                             returned 0 forever and DispatchMessageA never ran
  Win32 surface Lock      -- Lock popped 4 of the 5 __stdcall words, leaking 4
                             bytes per call into the caller's frame

Usage:
  python3 tools/test_tools.py
"""

import os
import re
import sys

# A stale __pycache__ silently shadowed an edited tool during development of
# these very tests: the source said one thing and the imported module said
# another, and the mutation checks "passed" against the old code. Do not write
# bytecode from here.
sys.dont_write_bytecode = True

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

FAILURES = []
CHECKS = [0]


def check(name, cond, detail=""):
    CHECKS[0] += 1
    if not cond:
        FAILURES.append("%s: %s" % (name, detail or "failed"))


def eq(name, got, want):
    check(name, got == want, "got %r, want %r" % (got, want))


# ---------------------------------------------------------------------------
# emit_x86: operand rendering
# ---------------------------------------------------------------------------
def test_emit_masking():
    import emit_x86 as E
    op = E.parse_operand("-1", 4)
    # 16-bit operand: the value must be rendered at 16 bits, not passed through
    # as 0xffffffff, which is a narrowing conversion (and warned).
    eq("mask16", E._read_at(op, 2), "0xffffu")
    eq("mask8", E._read_at(op, 1), "0xffu")
    # At 32 bits the mask is a no-op in value but still renders the two's
    # complement form.
    eq("mask32", E._read_at(op, 4), "0xffffffffu")
    # A register operand is untouched by the mask.
    r = E.parse_operand("ecx", 4)
    eq("mask-reg", E._read_at(r, 2), "ECX.d")


def test_emit_in_out():
    import emit_x86 as E
    ctx = E.Ctx(0x1000, 0x1002, 0x1000, 0x1002, None, "", set())
    # `in al, dx`: AL is the destination, DX the port.
    got = E._render("in", E.parse_operands("al, dx", 1), ctx)
    eq("in-order", got, "AL = ts::in_op(DX);")
    # `out 0x13, al`: the port comes first.
    got = E._render("out", E.parse_operands("0x13, al", 1), ctx)
    eq("out-order", got, "ts::out_op(0x13u, AL);")


def test_emit_loop_tail():
    import emit_x86 as E
    # A `loop` whose target lies BEFORE the function start: outside the span, so
    # it must become a tail transfer, not a goto to a label nobody emitted.
    insns = [{"va": 0x2000, "mnem": "loop", "op": "0x1f00", "bytes": "e2fe"}]
    items, _st = E.transcribe(insns, None, 0x2000, 0x2002, ret_type="")
    body = "\n".join(c for k, c, _ in items if k == "stmt")
    check("loop-tail-no-goto", "goto" not in body, body)
    # A TAIL transfer, not a nested call: the transfer replaces the frame, so a
    # jump chain or a span-crossing loop costs no host stack.
    check("loop-tail-dispatch", "ts::ts_tail" in body, body)
    check("loop-tail-not-nested", "ts::ts_call" not in body, body)
    # A target INSIDE the span is still a label.
    insns = [{"va": 0x2000, "mnem": "loop", "op": "0x2000", "bytes": "e2fe"}]
    items, _st = E.transcribe(insns, None, 0x2000, 0x2004, ret_type="")
    body = "\n".join(c for k, c, _ in items if k == "stmt")
    check("loop-in-span-goto", "goto L_00002000" in body, body)


def test_emit_ret():
    import emit_x86 as E
    ctx = E.Ctx(0x1000, 0x1004, 0x1000, 0x1004, None, "", set())
    # void: pop and return, never `return <value>` (illegal in a constructor).
    # Even a `ret` with no immediate pops -- it pops the RETURN ADDRESS the
    # caller pushed. Emitting a bare `return;` leaked 4 bytes per call and made
    # ESP drift, so both forms are asserted here.
    eq("ret-void", E._ret(0, ctx), "ts::ret_pop(0x0u); return;")
    eq("ret-void-imm", E._ret(4, ctx), "ts::ret_pop(0x4u); return;")
    # a value-returning function yields EAX, and also pops.
    ctx2 = E.Ctx(0x1000, 0x1004, 0x1000, 0x1004, None, "HRESULT", set())
    eq("ret-typed", E._ret(0, ctx2),
       "return (HRESULT)(size_t)ts::ret_pop_val(0x0u, EAX.d);")
    eq("ret-typed-imm", E._ret(8, ctx2),
       "return (HRESULT)(size_t)ts::ret_pop_val(0x8u, EAX.d);")


# ---------------------------------------------------------------------------
# gen_bodies: the declaration pattern and the vtable-block fallback
# ---------------------------------------------------------------------------
def test_decl_destructor():
    import gen_bodies as G
    # This is the defect that left 214 destructors undefined and the tree
    # unlinkable: `\b` before the name cannot match a `~`.
    m = G.DECL.search("    ~AbstractClass();      // 0x405710 / 0x405730")
    check("decl-dtor", m is not None, "destructor declaration did not match")
    if m:
        eq("decl-dtor-name", G.split_ret_name(m.group(2).strip())[1],
           "~AbstractClass")
        eq("decl-dtor-va", int(m.group(4), 16), 0x405710)
    # An access specifier must not be captured into the declaration.
    m = G.DECL.search("public: virtual ~Foo();   // 0x401000")
    check("decl-dtor-public", m is not None)
    if m:
        ret, name = G.split_ret_name(m.group(2).strip())
        eq("decl-dtor-public-ret", ret, "")
        eq("decl-dtor-public-name", name, "~Foo")
    # A declaration with no address still matches, so the slot fallback can run.
    m = G.DECL.search("    virtual int  GetSize();")
    check("decl-no-va", m is not None and m.group(4) is None)
    # Pure virtuals are excluded by requiring ';' right after the parens.
    check("decl-pure", G.DECL.search("    virtual void Blit() = 0;") is None)


def test_slot_va():
    import gen_bodies as G
    body = ("    // [1] GetSize    0x591fb0\n"
            "    // [1] Blit      /* ? */ pure virtual in effect\n")
    found = {m.group(2): (int(m.group(1)), m.group(3))
             for m in G.SLOT_VA.finditer(body)}
    eq("slot-va-addr", found["GetSize"], (1, "0x591fb0"))
    # A slot with no address must still record its NUMBER, so the manifest can
    # supply the target.
    eq("slot-va-num", found["Blit"], (1, None))


def test_slot0_shared():
    import gen_headers as H
    man = [{"entries": [0x100, 0x200]}, {"entries": [0x100, 0x300]},
           {"entries": [0x999]}]
    sm = H.shared_slot0_map(man)
    eq("slot0-shared", sm, {0x100: 2})


# ---------------------------------------------------------------------------
# reduce_bases: the numContainedBases walk
# ---------------------------------------------------------------------------
def test_reduce_bases():
    import reduce_bases as R
    # A pre-order base array: C's list is [C, B(ncb=1), A]. B owns the next
    # entry as its own ancestor, so A is an ancestor of both B and C.
    classes = [{
        "demangled": "C",
        "bases": [
            {"demangled": "C", "numContainedBases": 0},
            {"demangled": "B", "numContainedBases": 1},
            {"demangled": "A", "numContainedBases": 0},
        ],
    }]
    g = R.build_graph(classes)
    check("rb-B-in-C", "B" in g["C"], str(g))
    check("rb-A-in-B", "A" in g["B"], str(g))
    check("rb-A-in-C", "A" in g["C"], str(g))
    memo, seen = {}, set()
    eq("rb-transitive", R.ancestors("C", g, memo, seen), {"B", "A"})
    # A base that has no top-level entry of its own must still be resolvable.
    classes.append({"demangled": "A", "bases": [
        {"demangled": "A", "numContainedBases": 0}]})
    g2 = R.build_graph(classes)
    memo, seen = {}, set()
    eq("rb-leaf", R.ancestors("A", g2, memo, seen), set())


# ---------------------------------------------------------------------------
# gen_code: cross-function tail entries
# ---------------------------------------------------------------------------
def test_tail_entries():
    import gen_code as C
    # Two functions, each branching only within its own span. Nothing is a tail.
    fns = [{"va": 0x1000, "size": 0x20,
            "insns": [{"va": 0x1000, "mnem": "jmp", "op": "0x1010"},
                      {"va": 0x1010, "mnem": "ret", "op": ""}]},
           {"va": 0x2000, "size": 0x20,
            "insns": [{"va": 0x2000, "mnem": "jmp", "op": "0x2000"},
                      {"va": 0x2010, "mnem": "ret", "op": ""}]}]
    eq("tail-none", C.tail_entries(fns), {})
    # A branch out of span to a real function start is a plain call, not a tail.
    fns[0]["insns"].append({"va": 0x1014, "mnem": "jmp", "op": "0x3000"})
    fns.append({"va": 0x3000, "size": 0x10,
                "insns": [{"va": 0x3000, "mnem": "ret", "op": ""}]})
    eq("tail-start-not-tail", C.tail_entries(fns), {})
    # A branch to a mid-function address nothing starts at IS a tail entry.
    fns[2] = {"va": 0x2ff0, "size": 0x20,
              "insns": [{"va": 0x2ff0, "mnem": "ret", "op": ""},
                        {"va": 0x3000, "mnem": "ret", "op": ""}]}
    tails = C.tail_entries(fns)
    check("tail-found", 0x3000 in tails, str(tails))
    if 0x3000 in tails:
        eq("tail-host", tails[0x3000]["va"], 0x2ff0)


def test_indirect_tail_targets():
    """A switch-table entry is a dispatch target, not just a table word.

    The memcpy tail tables at 0x683c10 carry a dummy entry 0 and target
    0x683c4c, which is an instruction boundary inside the span at 0x683c49 but
    not a function start. Nothing registered it, so `ts::jmp_indirect` treated
    the dispatch as an unregistered call and abandoned memcpy's frame (ESI kept
    the source pointer, 16 bytes of stack leaked), which is what stopped the
    MIX index read after one block. This is that shape, synthetically.
    """
    import struct
    import gen_code as C

    class TableImage(object):
        def __init__(self, words):
            self.words = words

        def read(self, va, n):
            if va not in self.words:
                raise ValueError("outside the table")
            return struct.pack("<I", self.words[va])

    fns = [{"va": 0x1000, "size": 0x10,
            "insns": [{"va": 0x1000, "mnem": "jmp",
                       "op": "dword ptr [eax*4 + 0x1800]"},
                      {"va": 0x1006, "mnem": "ret", "op": ""}]},
           {"va": 0x2000, "size": 0x20,
            "insns": [{"va": 0x2000, "mnem": "ret", "op": ""},
                      {"va": 0x2010, "mnem": "ret", "op": ""}]}]
    # Entry 0 is the dummy the compiler emits for an index it knows is not
    # zero; entry 3 is inside the span at 0x2000 but not on an instruction
    # boundary, so no transcription can start there.
    words = {0x1800: 0xdeadbeef, 0x1804: 0x2010, 0x1808: 0x1000,
             0x180c: 0x2004, 0x1810: 0x2010}
    targets = C.indirect_jump_targets(fns, TableImage(words))
    check("indirect-target-found", 0x2010 in targets, str(targets))
    check("indirect-target-start", 0x1000 in targets, str(targets))
    check("indirect-target-not-boundary", 0x2004 not in targets, str(targets))
    # A function start needs no tail entry of its own.
    tails = C.tail_entries(fns, extra=targets)
    check("indirect-tail-entry", 0x2010 in tails, str(tails))
    check("indirect-start-not-tail", 0x1000 not in tails, str(tails))
    # No image means no resolution: the computed dispatch is kept, unchanged.
    eq("indirect-no-image", C.indirect_jump_targets(fns, None), {})


# ---------------------------------------------------------------------------
# disasm: switch-table bytes are data, not code
# ---------------------------------------------------------------------------
class _FakeImage(object):
    """The slice of disasm.Binary that `recover` reads, over a synthetic .text."""

    def __init__(self, base, blob):
        self.base = base
        self.blob = bytes(blob)
        self.data = self.blob
        self.text = {"name": ".text", "va": base, "vsize": len(self.blob),
                     "raw": 0, "rawsize": len(self.blob), "flags": 0}
        self.sections = [self.text]

    def in_text(self, va):
        return self.base <= va < self.base + len(self.blob)

    def section_of(self, va):
        return self.text if self.in_text(va) else None

    def read(self, va, n):
        if not self.in_text(va):
            return None
        off = va - self.base
        return self.blob[off:off + n]

    def u32(self, va):
        import struct
        b = self.read(va, 4)
        return struct.unpack("<I", b)[0] if b and len(b) == 4 else None


def test_jump_table_data():
    """A switch table is data; its entries are entry points.

    The memcpy tail tables carry an unreachable index-0 hole, so the run of
    entries starts at base+4. The recovery used to sweep the gap linearly and
    decode those entries as code, which left entry 1 (the real unaligned-copy
    loop) not on an instruction boundary; `ts::jmp_indirect` dispatches by exact
    VA and abandoned memcpy's frame. This test is mutation-proven in both
    directions: it asserts the table is detected, and it asserts that without
    the data region the very same bytes ARE decoded as a span.
    """
    import struct
    import disasm

    base = 0x1000
    blob = bytearray(b"\xcc" * 0x100)
    def put(va, bs):
        blob[va - base:va - base + len(bs)] = bs

    put(0x1000, b"\x8b\xc1")                                    # mov eax, ecx
    put(0x1002, b"\x83\xe0\x03")                                # and eax, 3
    put(0x1005, b"\xff\x24\x85" + struct.pack("<I", 0x1010))    # jmp [eax*4+1010]
    put(0x100c, b"\x90")                                        # nop
    # The next instruction's displacement bytes overlap the table's index-0
    # hole, exactly as in the binary: the dword at 0x1010 is not a code pointer.
    put(0x100d, b"\xff\x24\x8d" + struct.pack("<I", 0x2000))
    put(0x1014, struct.pack("<III", 0x1040, 0x1050, 0x1060))
    put(0x1040, b"\xb8\x01\x00\x00\x00\xc3")                    # mov eax,1; ret
    put(0x1050, b"\xb8\x02\x00\x00\x00\xc3")
    put(0x1060, b"\xb8\x03\x00\x00\x00\xc3")
    img = _FakeImage(base, blob)

    # Without the data region the gap sweep decodes the table bytes as code.
    plain = disasm.recover(img, {0x1000}, set())
    check("jumptable-false-span-without-data", 0x1014 in plain,
          "expected the table bytes decoded as a span at 0x1014")

    regions, targets = disasm.static_jump_table_data(img, plain)
    eq("jumptable-synth-region", regions.get(0x1014), 0x1020)
    for t in (0x1040, 0x1050, 0x1060):
        check("jumptable-synth-target-%x" % t, targets.get(t) == 0x1010,
              str(targets))
    check("jumptable-synth-hole-not-target", 0x2000 not in targets, str(targets))

    fixed = disasm.recover(img, {0x1000}, set(), regions)
    check("jumptable-no-false-span", 0x1014 not in fixed,
          "table bytes were decoded as code despite the data region")
    for t in (0x1040, 0x1050, 0x1060):
        check("jumptable-synth-entry-%x" % t, t in fixed, str(sorted(fixed)))
    inside = [i["va"] for f in fixed.values() for i in f["insns"]
              if 0x1014 <= i["va"] < 0x1020]
    eq("jumptable-no-instruction-in-table", inside, [])

    # The real binary's three unaligned-copy tables: base, hole, entry 1.
    bin_path = os.path.join(HERE, "..", "bin", "GAME.EXE")
    if os.path.exists(bin_path):
        b = disasm.Binary(bin_path)
        fns = {
            0x683bec: {"va": 0x683bec, "size": 0x19, "insns": [
                {"va": 0x683bfd, "mnem": "jmp",
                 "op": "dword ptr [eax*4 + 0x683c10]"}]},
            0x683c14: {"va": 0x683c14, "size": 0x35, "insns": [
                {"va": 0x683c14, "mnem": "and",
                 "op": "byte ptr [eax + ebp*2], bh"}]},
            0x683d80: {"va": 0x683d80, "size": 0x20, "insns": [
                {"va": 0x683d8d, "mnem": "jmp",
                 "op": "dword ptr [eax*4 + 0x683d98]"}]},
            0x683d9c: {"va": 0x683d9c, "size": 0x1b, "insns": [
                {"va": 0x683d9c, "mnem": "test", "op": "al, 0x3d"}]},
            0x68ce20: {"va": 0x68ce20, "size": 0x20, "insns": [
                {"va": 0x68ce2d, "mnem": "jmp",
                 "op": "dword ptr [eax*4 + 0x68ce40]"}]},
            0x68ce44: {"va": 0x68ce44, "size": 0x21, "insns": [
                {"va": 0x68ce44, "mnem": "push", "op": "eax"}]},
        }
        r, t = disasm.static_jump_table_data(b, fns)
        eq("jumptable-memcpy-region", r.get(0x683c14), 0x683c20)
        eq("jumptable-memcpy-entry1", t.get(0x683c20), 0x683c10)
        check("jumptable-memcpy-entry2", t.get(0x683c4c) == 0x683c10, str(t))
        check("jumptable-memcpy-entry3", t.get(0x683c70) == 0x683c10, str(t))
        eq("jumptable-memmove-region", r.get(0x683d9c), 0x683da8)
        eq("jumptable-memmove-entry1", t.get(0x683da8), 0x683d98)
        eq("jumptable-third-region", r.get(0x68ce44), 0x68ce50)
        eq("jumptable-third-entry1", t.get(0x68ce50), 0x68ce40)


# ---------------------------------------------------------------------------
# Machine model: differential against independent arithmetic
# ---------------------------------------------------------------------------
def test_machine_mul():
    import subprocess
    src = r'''
#include <cstdio>
#include "Machine.h"
static unsigned long long ref(unsigned a, unsigned b) {
    return (unsigned long long)a * (unsigned long long)b;
}
int main() {
    unsigned bad = 0;
    unsigned seeds[] = {0u, 1u, 2u, 0x7fffffffu, 0x80000000u, 0xffffffffu,
                        0x12345678u, 0xdeadbeefu, 0x0000ffffu, 0xffff0000u};
    for (unsigned i = 0; i < sizeof(seeds)/sizeof(seeds[0]); ++i)
        for (unsigned j = 0; j < sizeof(seeds)/sizeof(seeds[0]); ++j) {
            unsigned a = seeds[i], b = seeds[j];
            unsigned long long r = ref(a, b);
            if (ts::mul_lo(a, b) != (unsigned)(r & 0xffffffffu)) ++bad;
            if (ts::mul_hi(a, b) != (unsigned)(r >> 32)) ++bad;
        }
    // Deterministic pseudo-random sweep.
    unsigned s = 12345u;
    for (int k = 0; k < 2000000; ++k) {
        s = s * 1103515245u + 12345u; unsigned a = s;
        s = s * 1103515245u + 12345u; unsigned b = s;
        unsigned long long r = ref(a, b);
        if (ts::mul_lo(a, b) != (unsigned)(r & 0xffffffffu)) ++bad;
        if (ts::mul_hi(a, b) != (unsigned)(r >> 32)) ++bad;
    }
    std::printf("%u\n", bad);
    return 0;
}
'''
    open("/tmp/ts_mul_test.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O1", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_mul_test",
                        "/tmp/ts_mul_test.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-mul-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_mul_test"], capture_output=True)
    eq("machine-mul", r.stdout.decode().strip(), "0")


def test_machine_div():
    """`idiv` is signed, `div` is unsigned, against the host's own 64-bit math.

    Regression test for the boot stop at 0x00422a59: `idiv ecx` with
    EDX:EAX = 0xffffffffffffffff and ECX = -1. The true quotient is 1, but
    divmod64 ignored its `sign` flag, divided the unsigned value by `-d` (= 1)
    and raised "divide overflow". The reference here is the host's signed
    64-bit `/` and `%`, which truncate toward zero and give the remainder the
    dividend's sign exactly as x86 does -- an independent formulation, not a
    copy of the magnitude arithmetic in Machine.h. INT64_MIN / -1 is excluded
    from the reference because it is undefined in C++ (the model must fault on
    it, and the vector checks that).
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Machine.h"

static int fails = 0;
static void ck(const char *what, bool ok) {
    if (!ok) { std::printf("FAIL %s\n", what); ++fails; }
}
static void clear() { ts::reset(); }

// Host reference for idiv. Returns false when x86 would raise #DE (quotient
// does not fit int32, or the INT64_MIN / -1 case the host cannot compute).
static bool ref_idiv(unsigned hi, unsigned lo, unsigned d,
                     unsigned &q, unsigned &r) {
    unsigned long long un = ((unsigned long long)hi << 32) | lo;
    long long n = (long long)un;
    long long sd = (long long)(int)d;
    if (sd == -1 && n == (-9223372036854775807LL - 1LL)) return false;
    long long qq = n / sd;
    long long rr = n % sd;
    if (qq < -2147483647LL - 1LL || qq > 2147483647LL) return false;
    q = (unsigned)qq;
    r = (unsigned)rr;
    return true;
}

static bool ref_div(unsigned hi, unsigned lo, unsigned d,
                    unsigned &q, unsigned &r) {
    unsigned long long n = ((unsigned long long)hi << 32) | lo;
    unsigned long long qq = n / d;
    r = (unsigned)(n % d);
    if (qq > 0xffffffffull) return false;
    q = (unsigned)qq;
    return true;
}

int main() {
    unsigned rem = 0xdeadbeefu, q;

    // The exact boot case: EDX:EAX = -1, ECX = -1.
    clear();
    q = ts::idiv32(0xffffffffu, 0xffffffffu, 0xffffffffu, rem);
    ck("(-1)/-1 q", q == 1u);
    ck("(-1)/-1 rem", rem == 0u);
    ck("(-1)/-1 does not fault", !ts::mach().faulted);

    // Truncation toward zero; the remainder takes the dividend's sign.
    clear();
    q = ts::idiv32(0u, 7u, 2u, rem);
    ck("7/2 q", q == 3u); ck("7/2 rem", rem == 1u);
    q = ts::idiv32(0xffffffffu, 0xfffffff9u, 2u, rem);          // -7 / 2
    ck("-7/2 q", q == 0xfffffffdu); ck("-7/2 rem", rem == 0xffffffffu);
    q = ts::idiv32(0u, 7u, 0xfffffffeu, rem);                   // 7 / -2
    ck("7/-2 q", q == 0xfffffffdu); ck("7/-2 rem", rem == 1u);
    q = ts::idiv32(0xffffffffu, 0xfffffff9u, 0xfffffffeu, rem); // -7 / -2
    ck("-7/-2 q", q == 3u); ck("-7/-2 rem", rem == 0xffffffffu);

    // INT32_MIN is representable; one past it is overflow.
    clear();
    q = ts::idiv32(0u, 0x80000000u, 0xffffffffu, rem);          // 2^31 / -1
    ck("2^31/-1 q", q == 0x80000000u);
    ck("2^31/-1 rem", rem == 0u);
    ck("2^31/-1 does not fault", !ts::mach().faulted);
    clear();
    ts::idiv32(0u, 0x80000000u, 1u, rem);                       // 2^31 / 1
    ck("2^31/1 overflows", ts::mach().faulted);
    clear();
    ts::idiv32(0u, 0x80000001u, 0xffffffffu, rem);              // -(2^31+1)
    ck("-2^31-1 overflows", ts::mach().faulted);
    clear();
    ts::idiv32(0x80000000u, 0u, 0xffffffffu, rem);              // INT64_MIN/-1
    ck("INT64_MIN/-1 overflows", ts::mach().faulted);
    clear();
    ts::idiv32(1u, 0u, 2u, rem);                                // 2^32 / 2
    ck("2^32/2 overflows", ts::mach().faulted);
    clear();
    q = ts::idiv32(1u, 0u, 0xfffffffeu, rem);                   // 2^32 / -2
    ck("2^32/-2 q", q == 0x80000000u);
    ck("2^32/-2 rem", rem == 0u);
    ck("2^32/-2 does not fault", !ts::mach().faulted);

    // Divide by zero.
    clear();
    ts::idiv32(0u, 1u, 0u, rem);
    ck("idiv by zero faults", ts::mach().faulted);

    // Unsigned `div` must keep its own (unsigned) overflow rule.
    clear();
    q = ts::div32(0u, 100u, 7u, rem);
    ck("u 100/7 q", q == 14u); ck("u 100/7 rem", rem == 2u);
    q = ts::div32(0u, 0xffffffffu, 2u, rem);
    ck("u 0xffffffff/2 q", q == 0x7fffffffu);
    ck("u 0xffffffff/2 rem", rem == 1u);
    q = ts::div32(1u, 0u, 2u, rem);                             // 2^32 / 2
    ck("u 2^32/2 q", q == 0x80000000u);
    ck("u 2^32/2 rem", rem == 0u);
    ck("u 2^32/2 does not fault", !ts::mach().faulted);
    clear();
    ts::div32(0xffffffffu, 0xffffffffu, 0xffffffffu, rem);      // 2^32+1
    ck("u overflow faults", ts::mach().faulted);
    clear();
    ts::div32(0u, 1u, 0u, rem);
    ck("u divide by zero faults", ts::mach().faulted);

    // Differential sweep: 200k random EDX:EAX/divisor pairs, both signs.
    unsigned s = 0x9e3779b9u;
    for (int k = 0; k < 200000; ++k) {
        s = s * 1103515245u + 12345u; unsigned hi = s;
        s = s * 1103515245u + 12345u; unsigned lo = s;
        s = s * 1103515245u + 12345u; unsigned d = s;
        if (d == 0u) d = 1u;

        unsigned rq = 0, rr = 0, gq = 0, gr = 0;
        bool ok = ref_idiv(hi, lo, d, rq, rr);
        clear();
        gq = ts::idiv32(hi, lo, d, gr);
        if (ok != !ts::mach().faulted || (ok && (gq != rq || gr != rr))) {
            std::printf("FAIL idiv %08x:%08x / %08x -> %08x rem %08x, "
                        "want %08x rem %08x (fault %d)\n",
                        hi, lo, d, gq, gr, rq, rr, (int)ts::mach().faulted);
            ++fails;
            break;
        }

        ok = ref_div(hi, lo, d, rq, rr);
        clear();
        gq = ts::div32(hi, lo, d, gr);
        if (ok != !ts::mach().faulted || (ok && (gq != rq || gr != rr))) {
            std::printf("FAIL div %08x:%08x / %08x -> %08x rem %08x, "
                        "want %08x rem %08x (fault %d)\n",
                        hi, lo, d, gq, gr, rq, rr, (int)ts::mach().faulted);
            ++fails;
            break;
        }
    }

    std::printf("%d\n", fails);
    return fails ? 1 : 0;
}
'''
    open("/tmp/ts_div_test.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O1", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_div_test",
                        "/tmp/ts_div_test.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-div-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_div_test"], capture_output=True)
    out = r.stdout.decode("utf-8", "replace").strip()
    if out and out.splitlines()[-1] != "0":
        check("machine-div", False, out)
    else:
        eq("machine-div", out.splitlines()[-1] if out else "", "0")


def test_machine_flags():
    import subprocess
    src = r'''
#include <cstdio>
#include "Machine.h"
static int bad = 0;
#define T(c) do { if (!(c)) { std::printf("FAIL %s\n", #c); ++bad; } } while (0)
int main() {
    // add32 carry: 0xffffffff + 1 carries out.
    ts::Machine &m = ts::mach();
    ts::add32(0xffffffffu, 1u);
    T(m.cf == 1);
    T(m.zf == 1);          // result 0
    // add32 signed overflow: 0x7fffffff + 1 -> 0x80000000.
    ts::add32(0x7fffffffu, 1u);
    T(m.of == 1);
    T(m.sf == 1);
    T(m.cf == 0);
    // sub32 borrow: 0 - 1 borrows.
    ts::sub32(0u, 1u);
    T(m.cf == 1);
    T(m.zf == 0);
    // sub32 equal: zero flag, no borrow.
    ts::sub32(5u, 5u);
    T(m.zf == 1);
    T(m.cf == 0);
    // sub32 signed overflow: 0x80000000 - 1 -> 0x7fffffff.
    ts::sub32(0x80000000u, 1u);
    T(m.of == 1);
    // 8-bit add must not leak into the 32-bit flags.
    ts::add8(0xffu, 1u);
    T(m.cf == 1);
    T(m.zf == 1);
    std::printf("%d\n", bad);
    return 0;
}
'''
    open("/tmp/ts_flag_test.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_flag_test",
                        "/tmp/ts_flag_test.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-flags-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_flag_test"], capture_output=True)
    eq("machine-flags", r.stdout.decode().strip(), "0")


def test_machine_narrow_flags():
    """8- and 16-bit flag derivation, against the CPU's own flags.

    The defect: f_add/f_sub/f_inc/f_dec/f_log derived SF and OF from bit 31
    while the narrow helpers passed a zero-extended result. Every JLE/JG on a
    byte was therefore wrong for 0x80..0xff, which made the game's own vsprintf
    (FUN_0068667c) copy `%` literally and GetCDClass (FUN_004a7600) stop its
    drive scan at `c`.

    The oracle here is the hardware, via inline asm on x86-64, so the model
    cannot agree with a reference that repeats its own mistake. The JLE/JG
    predicates are taken from emit_x86.COND so this tests the condition the
    generator actually emits, not a copy of it.
    """
    import subprocess
    import emit_x86 as E
    le = E.COND['le']
    jg = E.COND['g']
    jl = E.COND['l']
    src = r'''
#include <cstdio>
#include "Machine.h"
static int bad = 0;
#define T(c) do { if (!(c)) { std::printf("FAIL %s (line %d)\n", #c, __LINE__); ++bad; } } while (0)
#define JLE() (@LE@)
#define JG()  (@JG@)
#define JL()  (@JL@)

#define F_CF 1
#define F_PF 2
#define F_AF 4
#define F_ZF 8
#define F_SF 16
#define F_OF 32
#define F_ALL 63
#define F_NO_AF 59   // AF is undefined after a logical op

#if defined(__x86_64__)
#define HW 1
static unsigned long hw_cmp8(unsigned a, unsigned b) {
    unsigned long fl;
    __asm__ __volatile__("cmpb %2, %1\n\tpushfq\n\tpopq %0"
        : "=r"(fl) : "q"((unsigned char)a), "q"((unsigned char)b) : "cc");
    return fl;
}
static unsigned long hw_cmp16(unsigned a, unsigned b) {
    unsigned long fl;
    __asm__ __volatile__("cmpw %2, %1\n\tpushfq\n\tpopq %0"
        : "=r"(fl) : "r"((unsigned short)a), "r"((unsigned short)b) : "cc");
    return fl;
}
static unsigned long hw_cmp32(unsigned a, unsigned b) {
    unsigned long fl;
    __asm__ __volatile__("cmpl %2, %1\n\tpushfq\n\tpopq %0"
        : "=r"(fl) : "r"(a), "r"(b) : "cc");
    return fl;
}
static unsigned long hw_add8(unsigned a, unsigned b) {
    unsigned long fl; unsigned char x = (unsigned char)a;
    __asm__ __volatile__("addb %2, %0\n\tpushfq\n\tpopq %1"
        : "+q"(x), "=r"(fl) : "q"((unsigned char)b) : "cc");
    return fl;
}
static unsigned long hw_sub8(unsigned a, unsigned b) {
    unsigned long fl; unsigned char x = (unsigned char)a;
    __asm__ __volatile__("subb %2, %0\n\tpushfq\n\tpopq %1"
        : "+q"(x), "=r"(fl) : "q"((unsigned char)b) : "cc");
    return fl;
}
static unsigned long hw_add16(unsigned a, unsigned b) {
    unsigned long fl; unsigned short x = (unsigned short)a;
    __asm__ __volatile__("addw %2, %0\n\tpushfq\n\tpopq %1"
        : "+r"(x), "=r"(fl) : "r"((unsigned short)b) : "cc");
    return fl;
}
static unsigned long hw_sub16(unsigned a, unsigned b) {
    unsigned long fl; unsigned short x = (unsigned short)a;
    __asm__ __volatile__("subw %2, %0\n\tpushfq\n\tpopq %1"
        : "+r"(x), "=r"(fl) : "r"((unsigned short)b) : "cc");
    return fl;
}
static unsigned long hw_add32(unsigned a, unsigned b) {
    unsigned long fl; unsigned x = a;
    __asm__ __volatile__("addl %2, %0\n\tpushfq\n\tpopq %1"
        : "+r"(x), "=r"(fl) : "r"(b) : "cc");
    return fl;
}
static unsigned long hw_sub32(unsigned a, unsigned b) {
    unsigned long fl; unsigned x = a;
    __asm__ __volatile__("subl %2, %0\n\tpushfq\n\tpopq %1"
        : "+r"(x), "=r"(fl) : "r"(b) : "cc");
    return fl;
}
static unsigned long hw_test8(unsigned a, unsigned b) {
    unsigned long fl;
    __asm__ __volatile__("testb %2, %1\n\tpushfq\n\tpopq %0"
        : "=r"(fl) : "q"((unsigned char)a), "q"((unsigned char)b) : "cc");
    return fl;
}
static unsigned long hw_inc8(unsigned a, int cf_in) {
    unsigned long fl; unsigned char x = (unsigned char)a;
    if (cf_in)
        __asm__ __volatile__("stc\n\tincb %0\n\tpushfq\n\tpopq %1"
            : "+q"(x), "=r"(fl) : : "cc");
    else
        __asm__ __volatile__("clc\n\tincb %0\n\tpushfq\n\tpopq %1"
            : "+q"(x), "=r"(fl) : : "cc");
    return fl;
}
static unsigned long hw_dec8(unsigned a, int cf_in) {
    unsigned long fl; unsigned char x = (unsigned char)a;
    if (cf_in)
        __asm__ __volatile__("stc\n\tdecb %0\n\tpushfq\n\tpopq %1"
            : "+q"(x), "=r"(fl) : : "cc");
    else
        __asm__ __volatile__("clc\n\tdecb %0\n\tpushfq\n\tpopq %1"
            : "+q"(x), "=r"(fl) : : "cc");
    return fl;
}
#else
#define HW 0
static unsigned long hw_cmp8(unsigned, unsigned) { return 0; }
static unsigned long hw_cmp16(unsigned, unsigned) { return 0; }
static unsigned long hw_cmp32(unsigned, unsigned) { return 0; }
static unsigned long hw_add8(unsigned, unsigned) { return 0; }
static unsigned long hw_sub8(unsigned, unsigned) { return 0; }
static unsigned long hw_add16(unsigned, unsigned) { return 0; }
static unsigned long hw_sub16(unsigned, unsigned) { return 0; }
static unsigned long hw_add32(unsigned, unsigned) { return 0; }
static unsigned long hw_sub32(unsigned, unsigned) { return 0; }
static unsigned long hw_test8(unsigned, unsigned) { return 0; }
static unsigned long hw_inc8(unsigned, int) { return 0; }
static unsigned long hw_dec8(unsigned, int) { return 0; }
#endif

static void chk(const char *what, unsigned long hw, int mask,
                int cf, int pf, int af, int zf, int sf, int of) {
    ts::Machine &m = ts::mach();
    int gcf = (int)(hw & 1), gpf = (int)((hw >> 2) & 1), gaf = (int)((hw >> 4) & 1);
    int gzf = (int)((hw >> 6) & 1), gsf = (int)((hw >> 7) & 1), gof = (int)((hw >> 11) & 1);
#define FCHK(bit, field, want, got) \
    do { if ((mask & (bit)) && (field) != (want)) { \
        std::printf("FAIL %s %s model=%d want=%d hw=%d\n", what, #field, (int)(field), (int)(want), (int)(got)); ++bad; } } while (0)
    FCHK(F_CF, m.cf, cf, gcf);
    FCHK(F_PF, m.pf, pf, gpf);
    FCHK(F_AF, m.af, af, gaf);
    FCHK(F_ZF, m.zf, zf, gzf);
    FCHK(F_SF, m.sf, sf, gsf);
    FCHK(F_OF, m.of, of, gof);
#if HW
    // The hardware must also agree with the expected vector; otherwise a typo
    // in the expectation would pass against a matching typo in the model.
    FCHK(F_CF, gcf, cf, gcf);
    FCHK(F_PF, gpf, pf, gpf);
    FCHK(F_AF, gaf, af, gaf);
    FCHK(F_ZF, gzf, zf, gzf);
    FCHK(F_SF, gsf, sf, gsf);
    FCHK(F_OF, gof, of, gof);
#else
    (void)gcf; (void)gpf; (void)gaf; (void)gzf; (void)gsf; (void)gof;
#endif
#undef FCHK
}

// The game's vsprintf classifies a format byte with this exact sequence
// (FUN_0068667c, 0x6866b9..0x6866c1):
//     cmp bl, 0x20 ; jl  skip
//     cmp bl, 0x78 ; jg  skip
// '%' is 0x25 and must fall through to the conversion table. Under the old
// flag derivation 0x25 - 0x78 = 0xad read as non-negative, `jg` was taken, and
// the format was copied literally -- GetDriveTypeA("%c:\\"), CreateFileA(
// "EXPAND%02d.MIX"), MessageBoxA("%s\\n\\n%s").
static int classifies(unsigned char c) {
    ts::cmp8(c, 0x20);
    if (JL()) return 0;
    ts::cmp8(c, 0x78);
    if (JG()) return 0;
    return 1;
}

int main() {
    ts::Machine &m = ts::mach();

    // GetCDClass (FUN_004a7600) scans 'c'..'z' with cmp8(BL,0x7a); jle.
    // Signed byte comparison: 'c' <= 'z', so JLE must be taken.
    ts::cmp8(0x63, 0x7a);
    chk("cmp8 c,z", hw_cmp8(0x63, 0x7a), F_ALL, 1, 0, 1, 0, 1, 0);
    T(JLE()); T(!JG());
    ts::cmp8(0x7a, 0x7a);
    chk("cmp8 z,z", hw_cmp8(0x7a, 0x7a), F_ALL, 0, 1, 0, 1, 0, 0);
    T(JLE()); T(!JG());
    ts::cmp8(0x7b, 0x7a);
    chk("cmp8 {,z", hw_cmp8(0x7b, 0x7a), F_ALL, 0, 0, 0, 0, 0, 0);
    T(!JLE()); T(JG());

    // The 0x7f/0x80 sign boundary. 0x7f - 0x80 = 0xff overflows, so
    // sf != of and JLE is NOT taken (127 > -128). 0x80 - 0x7f also overflows,
    // sf == of, so JLE IS taken (-128 <= 127). Deriving SF from bit 31 makes
    // both of these read as non-negative and gets both wrong.
    ts::cmp8(0x7f, 0x80);
    chk("cmp8 7f,80", hw_cmp8(0x7f, 0x80), F_ALL, 1, 1, 0, 0, 1, 1);
    T(!JLE()); T(JG());
    ts::cmp8(0x80, 0x7f);
    chk("cmp8 80,7f", hw_cmp8(0x80, 0x7f), F_ALL, 0, 0, 1, 0, 0, 1);
    T(JLE()); T(!JG());

    // Word forms.
    ts::cmp16(0x7fff, 0x8000);
    chk("cmp16 7fff,8000", hw_cmp16(0x7fff, 0x8000), F_ALL, 1, 1, 0, 0, 1, 1);
    T(!JLE()); T(JG());
    ts::cmp16(0x8000, 0x7fff);
    chk("cmp16 8000,7fff", hw_cmp16(0x8000, 0x7fff), F_ALL, 0, 0, 1, 0, 0, 1);
    T(JLE()); T(!JG());

    // add/sub carry, overflow, AF and PF at byte and word width.
    ts::add8(0x7f, 1);
    chk("add8 7f+1", hw_add8(0x7f, 1), F_ALL, 0, 0, 1, 0, 1, 1);
    ts::add8(0xff, 1);
    chk("add8 ff+1", hw_add8(0xff, 1), F_ALL, 1, 1, 1, 1, 0, 0);
    ts::sub8(0x00, 1);
    chk("sub8 0-1", hw_sub8(0x00, 1), F_ALL, 1, 1, 1, 0, 1, 0);
    ts::add16(0x7fff, 1);
    chk("add16 7fff+1", hw_add16(0x7fff, 1), F_ALL, 0, 1, 1, 0, 1, 1);
    ts::sub16(0x8000, 1);
    chk("sub16 8000-1", hw_sub16(0x8000, 1), F_ALL, 0, 1, 1, 0, 0, 1);

    // inc/dec preserve CF and set OF at the width's own boundary.
    m.cf = 0; ts::inc8(0x7f);
    chk("inc8 7f", hw_inc8(0x7f, 0), F_ALL, 0, 0, 1, 0, 1, 1);
    m.cf = 1; ts::inc8(0x7f);
    chk("inc8 7f cf", hw_inc8(0x7f, 1), F_ALL, 1, 0, 1, 0, 1, 1);
    m.cf = 0; ts::dec8(0x80);
    chk("dec8 80", hw_dec8(0x80, 0), F_ALL, 0, 0, 1, 0, 0, 1);

    // test8: logical flags, PF from the low byte, SF from bit 7 (AF undefined).
    ts::test8(0x80, 0x80);
    chk("test8 80", hw_test8(0x80, 0x80), F_NO_AF, 0, 0, 0, 0, 1, 0);
    ts::test8(0xff, 0xff);
    chk("test8 ff", hw_test8(0xff, 0xff), F_NO_AF, 0, 1, 0, 0, 1, 0);

    // 32-bit regression: unchanged by the width fix.
    ts::add32(0x7fffffff, 1);
    chk("add32 7fffffff+1", hw_add32(0x7fffffff, 1), F_ALL, 0, 1, 1, 0, 1, 1);
    ts::sub32(0x80000000, 1);
    chk("sub32 80000000-1", hw_sub32(0x80000000, 1), F_ALL, 0, 1, 1, 0, 0, 1);
    ts::cmp32(5, 5);
    chk("cmp32 5,5", hw_cmp32(5, 5), F_ALL, 0, 1, 0, 1, 0, 0);
    ts::cmp32(0x7fffffff, 0x80000000);
    chk("cmp32 7fffffff,80000000", hw_cmp32(0x7fffffff, 0x80000000),
        F_ALL, 1, 1, 0, 0, 1, 1);

    // The vsprintf classification, end to end at the branch level.
    T(classifies('%')); T(classifies('d')); T(classifies('s'));
    T(!classifies(0x19)); T(!classifies('y'));

    std::printf("%d\n", bad);
    return 0;
}
'''
    src = src.replace("@LE@", le).replace("@JG@", jg).replace("@JL@", jl)
    open("/tmp/ts_narrow_flag_test.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_narrow_flag_test",
                        "/tmp/ts_narrow_flag_test.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-narrow-flags-build", False,
              r.stderr.decode("utf-8", "replace")[:500])
        return
    r = subprocess.run(["/tmp/ts_narrow_flag_test"], capture_output=True)
    out = r.stdout.decode("utf-8", "replace")
    eq("machine-narrow-flags", out.strip(), "0")


def test_machine_stack():
    import subprocess
    src = r'''
#include <cstdio>
#include "Machine.h"
int main() {
    ts::Machine &m = ts::mach();
    m.r[4].d = 0x300000u;
    unsigned base = m.r[4].d;
    ts::push32(0xdeadbeefu);
    ts::push32(0x12345678u);
    if (ts::ld32(base - 4) != 0xdeadbeefu) { std::printf("FAIL order\n"); return 1; }
    if (ts::pop32() != 0x12345678u) { std::printf("FAIL pop1\n"); return 1; }
    if (ts::pop32() != 0xdeadbeefu) { std::printf("FAIL pop2\n"); return 1; }
    if (m.r[4].d != base) { std::printf("FAIL balance\n"); return 1; }
    std::printf("0\n");
    return 0;
}
'''
    open("/tmp/ts_stack_test.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_stack_test",
                        "/tmp/ts_stack_test.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-stack-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_stack_test"], capture_output=True)
    eq("machine-stack", r.stdout.decode().strip(), "0")


def test_machine_mmx():
    """MMX lanes against an independent implementation of the definitions.

    The reference takes the operand apart with memcpy and works on a byte
    array, so it shares no expression with the shift-based implementation in
    Machine.h. Every MMX mnemonic the recovered instruction stream uses is
    covered; a lane operation that is wrong in one saturation or interleave
    shows up as a non-zero mismatch count.
    """
    import subprocess
    src = r'''
#include <cstdio>
#include <cstring>
#include "Machine.h"

typedef unsigned long long U64;
typedef unsigned int U32;

static int ref_sat_u8(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }
static int ref_sat_i16(int v) {
    return v < -32768 ? -32768 : (v > 32767 ? 32767 : v);
}
static void bytes(U64 v, unsigned char *o) { std::memcpy(o, &v, 8); }
static U64 pack(const unsigned char *b) {
    U64 v; std::memcpy(&v, b, 8); return v;
}
static unsigned ref_word(const unsigned char *b, int i) {
    return (unsigned)b[2 * i] | ((unsigned)b[2 * i + 1] << 8);
}
static unsigned ref_dword(const unsigned char *b, int i) {
    return ref_word(b, 2 * i) | (ref_word(b, 2 * i + 1) << 16);
}
static int ref_sword(const unsigned char *b, int i) {
    return (short)ref_word(b, i);
}
static int ref_sdword(const unsigned char *b, int i) { return (int)ref_dword(b, i); }

static U64 ref_paddusb(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 8; ++i) { int s = x[i] + y[i]; r[i] = (unsigned char)(s > 255 ? 255 : s); }
    return pack(r);
}
static U64 ref_packuswb(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 4; ++i) {
        r[i] = (unsigned char)ref_sat_u8(ref_sword(x, i));
        r[i + 4] = (unsigned char)ref_sat_u8(ref_sword(y, i));
    }
    return pack(r);
}
static U64 ref_packssdw(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 2; ++i) {
        unsigned sa = (unsigned)(unsigned short)ref_sat_i16(ref_sdword(x, i));
        unsigned sb = (unsigned)(unsigned short)ref_sat_i16(ref_sdword(y, i));
        r[2 * i] = (unsigned char)sa; r[2 * i + 1] = (unsigned char)(sa >> 8);
        r[4 + 2 * i] = (unsigned char)sb; r[5 + 2 * i] = (unsigned char)(sb >> 8);
    }
    return pack(r);
}
static U64 ref_pmullw(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 4; ++i) {
        unsigned p = ref_word(x, i) * ref_word(y, i);
        r[2 * i] = (unsigned char)p; r[2 * i + 1] = (unsigned char)(p >> 8);
    }
    return pack(r);
}
static U64 ref_pmulhw(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 4; ++i) {
        int p = ref_sword(x, i) * ref_sword(y, i);
        unsigned h = (unsigned)(unsigned short)(p >> 16);
        r[2 * i] = (unsigned char)h; r[2 * i + 1] = (unsigned char)(h >> 8);
    }
    return pack(r);
}
static U64 ref_punpcklbw(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 4; ++i) { r[2 * i] = x[i]; r[2 * i + 1] = y[i]; }
    return pack(r);
}
static U64 ref_punpckhbw(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 4; ++i) { r[2 * i] = x[4 + i]; r[2 * i + 1] = y[4 + i]; }
    return pack(r);
}
static U64 ref_punpcklwd(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 2; ++i) {
        r[4 * i] = x[2 * i]; r[4 * i + 1] = x[2 * i + 1];
        r[4 * i + 2] = y[2 * i]; r[4 * i + 3] = y[2 * i + 1];
    }
    return pack(r);
}
static U64 ref_punpckhwd(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 2; ++i) {
        r[4 * i] = x[4 + 2 * i]; r[4 * i + 1] = x[5 + 2 * i];
        r[4 * i + 2] = y[4 + 2 * i]; r[4 * i + 3] = y[5 + 2 * i];
    }
    return pack(r);
}
static U64 ref_punpckldq(U64 a, U64 b) {
    unsigned char x[8], y[8], r[8];
    bytes(a, x); bytes(b, y);
    for (int i = 0; i < 4; ++i) { r[i] = x[i]; r[4 + i] = y[i]; }
    return pack(r);
}
static U64 ref_psrlw(U64 a, unsigned n) {
    unsigned char x[8], r[8];
    bytes(a, x);
    for (int i = 0; i < 4; ++i) {
        unsigned w = (n > 15) ? 0 : (ref_word(x, i) >> n);
        r[2 * i] = (unsigned char)w; r[2 * i + 1] = (unsigned char)(w >> 8);
    }
    return pack(r);
}
static U64 ref_psrad(U64 a, unsigned n) {
    unsigned char x[8], r[8];
    bytes(a, x);
    for (int i = 0; i < 2; ++i) {
        int d = ref_sdword(x, i);
        unsigned w = (unsigned)(n > 31 ? (d >> 31) : (d >> n));
        r[4 * i] = (unsigned char)w; r[4 * i + 1] = (unsigned char)(w >> 8);
        r[4 * i + 2] = (unsigned char)(w >> 16); r[4 * i + 3] = (unsigned char)(w >> 24);
    }
    return pack(r);
}

static unsigned bad = 0;
static void cmp(const char *what, U64 got, U64 want) {
    if (got != want) { ++bad; if (bad < 6) std::printf("%s\n", what); }
}

int main() {
    const U64 edge[] = { 0ull, 1ull, 0xffffffffffffffffull, 0x8000000000000000ull,
                         0x7fff80007fff8000ull, 0x0001000100010001ull,
                         0xff00ff00ff00ff00ull, 0x00ff00ff00ff00ffull };
    const int ne = (int)(sizeof(edge) / sizeof(edge[0]));
    U64 s = 0x123456789abcdefull;
    for (int k = 0; k < 20000; ++k) {
        U64 a, b;
        if (k < ne * ne) { a = edge[k / ne]; b = edge[k % ne]; }
        else {
            s = s * 6364136223846793005ull + 1442695040888963407ull; a = s;
            s = s * 6364136223846793005ull + 1442695040888963407ull; b = s;
        }
        cmp("pxor", ts::mmx_pxor(a, b), a ^ b);
        cmp("paddusb", ts::mmx_paddusb(a, b), ref_paddusb(a, b));
        cmp("packuswb", ts::mmx_packuswb(a, b), ref_packuswb(a, b));
        cmp("packssdw", ts::mmx_packssdw(a, b), ref_packssdw(a, b));
        cmp("pmullw", ts::mmx_pmullw(a, b), ref_pmullw(a, b));
        cmp("pmulhw", ts::mmx_pmulhw(a, b), ref_pmulhw(a, b));
        cmp("punpcklbw", ts::mmx_punpcklbw(a, b), ref_punpcklbw(a, b));
        cmp("punpckhbw", ts::mmx_punpckhbw(a, b), ref_punpckhbw(a, b));
        cmp("punpcklwd", ts::mmx_punpcklwd(a, b), ref_punpcklwd(a, b));
        cmp("punpckhwd", ts::mmx_punpckhwd(a, b), ref_punpckhwd(a, b));
        cmp("punpckldq", ts::mmx_punpckldq(a, b), ref_punpckldq(a, b));
        for (unsigned n = 0; n < 33; ++n) {
            cmp("psrlw", ts::mmx_psrlw(a, n), ref_psrlw(a, n));
            cmp("psrad", ts::mmx_psrad(a, n), ref_psrad(a, n));
        }
    }
    std::printf("%u\n", bad);
    return 0;
}
'''
    open("/tmp/ts_mmx_test.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O1", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_mmx_test",
                        "/tmp/ts_mmx_test.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-mmx-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_mmx_test"], capture_output=True)
    eq("machine-mmx", r.stdout.decode().strip(), "0")

def test_machine_depth_guard():
    import subprocess
    src = r'''
#include <cstdio>
#include "Machine.h"
static int depth = 0;
static void recurse() { ++depth; ts::ts_call(0x1000u); }
int main() {
    ts::Machine &m = ts::mach();
    m.call_depth_limit = 64;
    m.call_depth = 0;
    m.depth_exceeded = 0;
    ts::ts_register(0x1000u, &recurse);
    ts::ts_call(0x1000u);
    // The guard must stop the recursion rather than blowing the host stack,
    // and must say so.
    if (m.depth_exceeded == 0) { std::printf("FAIL no guard\n"); return 1; }
    if (depth > 64) { std::printf("FAIL too deep %d\n", depth); return 1; }
    if (m.call_depth != 0) { std::printf("FAIL unbalanced\n"); return 1; }
    std::printf("0\n");
    return 0;
}
'''
    open("/tmp/ts_depth_test.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_depth_test",
                        "/tmp/ts_depth_test.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-depth-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_depth_test"], capture_output=True)
    eq("machine-depth", r.stdout.decode().strip(), "0")


def test_machine_boot_regressions():
    """The four model defects that blocked the boot, each a shipped bug.

    ts_call_ptr used to leak the return address on an unregistered target;
    the conditional repeats zeroed ECX instead of leaving the count that did
    not run (so `repne scasb; not ecx` produced a 4 GB length); jmp_indirect
    was a no-op that skipped the target span and its epilogue; and the
    coverage maps are new, so a bit that is not set is a metric that silently
    reads low. Each check is independent and prints the failing one.
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Machine.h"

static void noop() {}

int main() {
    ts::Machine &m = ts::mach();
    m.r[4].d = 0x300000u;
    m.step_limit = 0;
    m.text_lo = 0x401000u;
    m.text_hi = 0x697225u;

    // 1. An indirect call to an unregistered target still returns, and the
    //    return address it pushed must be popped. Before the fix this leaked
    //    4 bytes per call; 27 of them made ESI hold a return address.
    ts::u32 base = m.r[4].d;
    ts::ts_call_ptr(0x00999999u, 0x12345678u);
    if (m.r[4].d != base) { std::printf("FAIL call_ptr esp %u\n", base - m.r[4].d); return 1; }
    if (m.eip != 0x12345678u) { std::printf("FAIL call_ptr eip\n"); return 1; }

    // 2. repne scasb must leave ECX = iterations that did not run. The inline
    //    strlen is `repne scasb; not ecx`, so ECX == 0 makes the length -1.
    //    Memory: 11 22 33 00, scan 16 bytes for AL == 0.
    ts::u32 buf = 0x200000u;
    ts::st8(buf + 0, 0x11); ts::st8(buf + 1, 0x22);
    ts::st8(buf + 2, 0x33); ts::st8(buf + 3, 0x00);
    m.r[7].d = buf; m.r[1].d = 16u; m.r[0].b[0] = 0; m.df = 0;
    ts::ts_repne_scasb();
    if (m.r[1].d != 12u) { std::printf("FAIL repne ecx %u\n", m.r[1].d); return 1; }
    if (m.r[7].d != buf + 4) { std::printf("FAIL repne edi\n"); return 1; }
    if (!m.zf) { std::printf("FAIL repne zf\n"); return 1; }

    // repe scasb: 11 11 22, scan for AL == 11; stops on the mismatch.
    ts::st8(buf + 0, 0x11); ts::st8(buf + 1, 0x11); ts::st8(buf + 2, 0x22);
    m.r[7].d = buf; m.r[1].d = 16u; m.r[0].b[0] = 0x11;
    ts::ts_rep_scasb();
    if (m.r[1].d != 13u) { std::printf("FAIL repe ecx %u\n", m.r[1].d); return 1; }

    // 3. jmp_indirect is a tail transfer, not a counted no-op. Skipping it
    //    skipped the target span's epilogue (memcpy's pop esi/edi; leave; ret).
    m.tail = 0;
    ts::jmp_indirect(0x0056789au);
    if (m.tail != 0x0056789au) { std::printf("FAIL jmp_indirect tail\n"); return 1; }

    // 4. The coverage maps are written by insn() and ts_call(). A map that is
    //    not marked reads as code that never ran.
    static ts::u8 emap[16], fmap[16];
    for (int i = 0; i < 16; ++i) { emap[i] = 0; fmap[i] = 0; }
    m.exec_map = emap; m.exec_map_lo = 0x401000u; m.exec_map_hi = 0x401080u;
    m.func_map = fmap; m.func_map_lo = 0x401000u; m.func_map_hi = 0x401080u;
    ts::insn(0x401004u);
    if (!(emap[(0x401004u - 0x401000u) >> 3] & (1u << (0x401004u & 7u)))) {
        std::printf("FAIL exec map\n"); return 1;
    }
    ts::ts_register(0x401020u, &noop);
    ts::ts_call(0x401020u);
    if (!(fmap[(0x401020u - 0x401000u) >> 3] & (1u << (0x401020u & 7u)))) {
        std::printf("FAIL func map\n"); return 1;
    }
    // A VA outside the map must not write.
    ts::insn(0x500000u);
    std::printf("0\n");
    return 0;
}
'''
    open("/tmp/ts_boot_regress.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_boot_regress",
                        "/tmp/ts_boot_regress.cpp"], capture_output=True)
    if r.returncode != 0:
        check("machine-boot-regress-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_boot_regress"], capture_output=True)
    eq("machine-boot-regress", r.stdout.decode().strip(), "0")


def test_win32_module_exports():
    """Regression test for the COM registration blocker, at the model level.

    Two shipped defects are exercised. LoadLibraryA ignored its argument and
    returned a counter, so a handle meant "some DLL" and GetProcAddress could
    not resolve a GetProcAddress-only export: blowfish.dll's DllRegisterServer
    read as null and the game took its error box. CoCreateInstance was a
    generic row that returned S_OK WITHOUT writing its out pointer, so
    FUN_005e87a0 called OleRun and QueryInterface through an uninitialised
    stack slot.

    The test calls the stubs through the same dispatch table the boot uses, so
    it also checks that every stub pops exactly its argument count: the
    project has shipped a 4-byte over-pop that corrupted _initterm.
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Win32Stubs.h"

static int fails = 0;
static void ck(const char *what, bool ok) {
    if (!ok) { std::printf("FAIL %s\n", what); ++fails; }
}
static void put_guid(ts::u32 p, const unsigned char *b) {
    for (unsigned i = 0; i < 16u; ++i) ts::st8(p + i, b[i]);
}

int main() {
    ts::register_win32_stubs();

    // A handle identifies a module by name; the modelled modules are distinct.
    ts::u32 hb = ts::w32::module_load("blowfish.dll");
    ts::u32 hc = ts::w32::module_load("comctl32.dll");
    ck("blowfish handle", hb != 0);
    ck("comctl32 handle", hc != 0);
    ck("distinct handles", hb != hc);
    ck("same name, same handle", ts::w32::module_load("BLOWFISH.DLL") == hb);

    // blowfish.dll exports DllRegisterServer as a callable stub, and its table
    // is per module: it must not serve comctl32's DllGetVersion.
    ts::u32 rs = ts::w32::module_find_export(hb, "DllRegisterServer");
    ts::u32 us = ts::w32::module_find_export(hb, "DllUnregisterServer");
    ck("DllRegisterServer resolved", rs != 0 && ts::ts_lookup(rs) != 0);
    ck("DllUnregisterServer resolved", us != 0 && ts::ts_lookup(us) != 0);
    ck("blowfish has no DllGetVersion",
       ts::w32::module_find_export(hb, "DllGetVersion") == 0);
    ck("comctl32 has DllGetVersion",
       ts::w32::module_find_export(hc, "DllGetVersion") != 0);

    // The LoadLibraryA stub itself must hand back the module's handle, not a
    // counter. Call it for comctl32 AFTER two module_load calls so a counter
    // (0x00500000) would not coincidentally equal the right handle.
    ts::u32 ll = ts::stub_va("KERNEL32", "LoadLibraryA");
    ts::u32 gpa = ts::stub_va("KERNEL32", "GetProcAddress");
    ck("LoadLibraryA resolved", ll != 0 && ts::ts_lookup(ll) != 0);
    ck("GetProcAddress resolved", gpa != 0 && ts::ts_lookup(gpa) != 0);
    ts::u32 pcomctl = ts::w32::lit("comctl32.dll");
    ts::u32 sp = 0x002ff000u;
    ESP.d = sp;
    ts::st32(sp + 0u, pcomctl);
    ts::ts_call_at(ll, 0x11111111u);
    ck("LoadLibraryA names the module", EAX.d == hc);
    ck("LoadLibraryA stack balanced", ESP.d == sp + 4u);
    // GetProcAddress through that handle resolves the module's own export.
    ts::u32 pdgv = ts::w32::lit("DllGetVersion");
    ESP.d = sp;
    ts::st32(sp + 0u, EAX.d);
    ts::st32(sp + 4u, pdgv);
    ts::ts_call_at(gpa, 0x11111112u);
    ck("GetProcAddress resolves comctl32!DllGetVersion",
       EAX.d == ts::w32::module_find_export(hc, "DllGetVersion"));
    ck("GetProcAddress stack balanced", ESP.d == sp + 8u);
    // And the GetProcAddress-only export is reachable through blowfish's
    // handle. The old global name lookup returned 0 here: DllRegisterServer is
    // not in the import directory.
    ts::u32 pblow = ts::w32::lit("blowfish.dll");
    ESP.d = sp;
    ts::st32(sp + 0u, pblow);
    ts::ts_call_at(ll, 0x11111113u);
    ts::u32 hb2 = EAX.d;
    ts::u32 pdrs = ts::w32::lit("DllRegisterServer");
    ESP.d = sp;
    ts::st32(sp + 0u, hb2);
    ts::st32(sp + 4u, pdrs);
    ts::ts_call_at(gpa, 0x11111114u);
    ck("GetProcAddress resolves blowfish!DllRegisterServer", EAX.d == rs);
    ck("GetProcAddress blowfish stack balanced", ESP.d == sp + 8u);

    // Calling it pops exactly 0 arguments and returns S_OK.
    ESP.d = sp;
    ts::ts_call_at(rs, 0x11111111u);
    ck("DllRegisterServer returns S_OK", EAX.d == 0);
    ck("DllRegisterServer stack balanced", ESP.d == sp);

    // An unknown DLL keeps the old non-null handle, but resolves nothing.
    ts::u32 hu = ts::w32::module_load("nosuch.dll");
    ck("unknown module handle", hu != 0);
    ck("unknown module exports nothing",
       ts::w32::module_find_export(hu, "DllRegisterServer") == 0);

    // CoCreateInstance writes a real object with a real vtable and returns
    // S_OK. The old row returned S_OK and wrote nothing.
    static const unsigned char clsid[16] = {
        0x10,0xad,0x40,0x14, 0xa8,0x6a, 0xd1,0x11,
        0xb6,0xf9,0x00,0xa0, 0x24,0xdd,0xaf,0xd1 };
    static const unsigned char iid_unk[16] = {
        0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00,
        0xc0,0x00,0x00,0x00, 0x00,0x00,0x00,0x46 };
    ts::u32 pclsid = ts::w32::alloc_block(16u, 4u);
    ts::u32 piid = ts::w32::alloc_block(16u, 4u);
    ts::u32 out = ts::w32::alloc_block(4u, 4u);
    put_guid(pclsid, clsid);
    put_guid(piid, iid_unk);
    ts::st32(out, 0u);

    ts::u32 cc = ts::stub_va("ole32", "CoCreateInstance");
    ck("CoCreateInstance resolved", cc != 0 && ts::ts_lookup(cc) != 0);
    sp = 0x002ff000u;
    ESP.d = sp;
    ts::st32(sp + 0u, pclsid);
    ts::st32(sp + 4u, 0u);        // pUnkOuter
    ts::st32(sp + 8u, 7u);        // CLSCTX
    ts::st32(sp + 12u, piid);
    ts::st32(sp + 16u, out);
    ts::ts_call_at(cc, 0x22222222u);
    ck("CoCreateInstance returns S_OK", EAX.d == 0);
    ck("CoCreateInstance stack balanced", ESP.d == sp + 20u);
    ts::u32 obj = ts::ld32(out);
    ck("CoCreateInstance writes the object", obj != 0);
    ts::u32 vt = obj ? ts::ld32(obj) : 0;
    ck("object has a vtable", vt != 0);
    ck("QueryInterface is callable", vt && ts::ts_lookup(ts::ld32(vt)) != 0);
    ck("AddRef is callable", vt && ts::ts_lookup(ts::ld32(vt + 4u)) != 0);
    ck("Release is callable", vt && ts::ts_lookup(ts::ld32(vt + 8u)) != 0);

    // QueryInterface fills its out pointer with the same object: the wrapper
    // at 0x5e87a0 asks for IUnknown and then calls OleRun on the result.
    ts::u32 qi = vt ? ts::ld32(vt) : 0;
    ts::u32 out2 = ts::w32::alloc_block(4u, 4u);
    ts::st32(out2, 0u);
    sp = 0x002ff000u;
    ESP.d = sp;
    ts::st32(sp + 0u, obj);
    ts::st32(sp + 4u, piid);
    ts::st32(sp + 8u, out2);
    ts::ts_call_at(qi, 0x33333333u);
    ck("QueryInterface returns S_OK", EAX.d == 0);
    ck("QueryInterface writes the object", ts::ld32(out2) == obj);
    ck("QueryInterface stack balanced", ESP.d == sp + 12u);

    // IBlowfish's own methods, from BLOWFISH.DLL's vtable 0x1101e480:
    // slot 4 GetMaxKeyLength = 56, slot 5 GetBlockSize = 8.
    ts::u32 gmk = vt ? ts::ld32(vt + 16u) : 0;
    ts::u32 gbs = vt ? ts::ld32(vt + 20u) : 0;
    ts::u32 n = ts::w32::alloc_block(4u, 4u);
    ts::st32(n, 0u);
    sp = 0x002ff000u;
    ESP.d = sp;
    ts::st32(sp + 0u, obj);
    ts::st32(sp + 4u, n);
    ts::ts_call_at(gmk, 0x44444444u);
    ck("GetMaxKeyLength returns S_OK", EAX.d == 0);
    ck("GetMaxKeyLength is 56", ts::ld32(n) == 56u);
    ts::st32(n, 0u);
    ESP.d = sp;
    ts::st32(sp + 0u, obj);
    ts::st32(sp + 4u, n);
    ts::ts_call_at(gbs, 0x55555555u);
    ck("GetBlockSize is 8", ts::ld32(n) == 8u);

    std::printf("%d\n", fails);
    return fails ? 1 : 0;
}
'''
    open("/tmp/ts_win32_exports.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_win32_exports",
                        "/tmp/ts_win32_exports.cpp"], capture_output=True)
    if r.returncode != 0:
        check("win32-module-exports-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_win32_exports"], capture_output=True)
    eq("win32-module-exports", r.stdout.decode().strip().splitlines()[-1]
       if r.stdout.strip() else "", "0")


def test_win32_message_queue():
    """The USER32 queue, driven through the dispatch table the boot uses.

    Regression test for the message-loop blocker: RegisterClassA recorded the
    class name but CreateWindowExA never queued WM_ACTIVATEAPP, so PeekMessageA
    always returned 0, DispatchMessageA was never called, and the game spun on
    the "app active" flag its WndProc sets from WM_ACTIVATEAPP. A message that
    is queued but never dispatched, a MSG written at the wrong offsets, a
    PM_NOREMOVE that consumes the message, and a stub that pops the wrong
    number of argument words are all defects this exercises. It also checks
    the model's stack convention: after a __stdcall stub with N arguments ESP
    must be the caller's ESP + 4*N.
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Win32Stubs.h"

static int fails = 0;
static void ck(const char *what, bool ok) {
    if (!ok) { std::printf("FAIL %s\n", what); ++fails; }
}

// What the game's own window procedure would record.
static ts::u32 g_calls, g_hwnd, g_msg, g_wp, g_lp;
static void fake_wndproc() {
    ++g_calls;
    g_hwnd = ts::ld32(ESP.d + 4u);
    g_msg  = ts::ld32(ESP.d + 8u);
    g_wp   = ts::ld32(ESP.d + 12u);
    g_lp   = ts::ld32(ESP.d + 16u);
    ESP.d += 4u + 16u;   // __stdcall: pop the return address and 4 args
}

// Call a stub exactly as a transcription does: arguments at the caller's ESP,
// ts_call_at pushes the return address, the stub pops ret + its own args.
static ts::u32 call_stub(ts::u32 fn, const ts::u32 *a, unsigned n, ts::u32 sp) {
    for (unsigned i = 0; i < n; ++i) ts::st32(sp + i * 4u, a[i]);
    ESP.d = sp;
    ts::ts_call_at(fn, 0x00c0ffeeu);
    return EAX.d;
}

static void put_cstr(ts::u32 at, const char *s) {
    unsigned i = 0;
    for (; s[i]; ++i) ts::st8(at + i, (ts::u8)s[i]);
    ts::st8(at + i, 0);
}

int main() {
    ts::register_win32_stubs();
    const ts::u32 sp = 0x002ff000u;
    const ts::u32 wc = 0x00300000u, name = 0x00300100u, msg = 0x00300200u;
    const ts::u32 proc_va = 0x00c0de01u;
    ts::ts_register(proc_va, &fake_wndproc);
    put_cstr(name, "TSTCLASS");
    for (unsigned i = 0; i < 40u; i += 4u) ts::st32(wc + i, 0u);
    ts::st32(wc + 4u, proc_va);    // lpfnWndProc
    ts::st32(wc + 36u, name);      // lpszClassName

    ts::u32 reg = ts::stub_va("USER32", "RegisterClassA");
    ts::u32 cwe = ts::stub_va("USER32", "CreateWindowExA");
    ts::u32 peek = ts::stub_va("USER32", "PeekMessageA");
    ts::u32 getm = ts::stub_va("USER32", "GetMessageA");
    ts::u32 disp = ts::stub_va("USER32", "DispatchMessageA");
    ts::u32 post = ts::stub_va("USER32", "PostMessageA");
    ts::u32 send = ts::stub_va("USER32", "SendMessageA");
    ts::u32 quit = ts::stub_va("USER32", "PostQuitMessage");
    ck("stubs resolve", reg && cwe && peek && getm && disp && post && send && quit);

    // RegisterClassA records the class name and its procedure.
    ts::u32 a1[1] = { wc };
    ts::u32 atom = call_stub(reg, a1, 1, sp);
    ck("RegisterClassA returns an atom", atom != 0);
    ck("RegisterClassA pops 1 arg", ESP.d == sp + 4u);

    // CreateWindowExA sends WM_CREATE synchronously and queues WM_ACTIVATEAPP.
    ts::u32 a12[12] = { 0u, name, 0u, 0x00cf0000u,
                        0x80000000u, 0x80000000u, 800u, 600u, 0u, 0u, 0u, 0u };
    g_calls = 0;
    ts::u32 hwnd = call_stub(cwe, a12, 12, sp);
    ck("CreateWindowExA returns a handle", hwnd != 0);
    ck("CreateWindowExA pops 12 args", ESP.d == sp + 48u);
    ck("WM_CREATE was sent", g_calls == 1u && g_msg == 0x0001u);
    ck("WM_CREATE hwnd", g_hwnd == hwnd);
    ck("WM_CREATE has a CREATESTRUCT", g_lp != 0u);

    // PM_NOREMOVE must fill the MSG and leave it queued.
    ts::u32 pm[5] = { msg, 0u, 0u, 0u, 0u };   // wRemoveMsg = PM_NOREMOVE
    ts::u32 r = call_stub(peek, pm, 5, sp);
    ck("PeekMessage PM_NOREMOVE sees WM_ACTIVATEAPP", r == 1u);
    ck("PeekMessage pops 5 args", ESP.d == sp + 20u);
    ck("MSG.hwnd at +0", ts::ld32(msg + 0u) == hwnd);
    ck("MSG.message at +4", ts::ld32(msg + 4u) == 0x001cu);
    ck("MSG.wParam at +8", ts::ld32(msg + 8u) == 1u);
    ck("MSG.lParam at +12", ts::ld32(msg + 12u) == 0u);
    ck("MSG.pt at +20/+24", ts::ld32(msg + 20u) == 0u && ts::ld32(msg + 24u) == 0u);
    r = call_stub(peek, pm, 5, sp);
    ck("PM_NOREMOVE leaves it queued", r == 1u);

    // DispatchMessageA must call the recorded procedure with the MSG fields.
    ts::u32 a1m[1] = { msg };
    g_calls = 0;
    call_stub(disp, a1m, 1, sp);
    ck("DispatchMessageA pops 1 arg", ESP.d == sp + 4u);
    ck("DispatchMessageA calls the WndProc once", g_calls == 1u);
    ck("dispatched hwnd", g_hwnd == hwnd);
    ck("dispatched message", g_msg == 0x001cu);
    ck("dispatched wParam", g_wp == 1u);
    ck("dispatched lParam", g_lp == 0u);

    // PM_REMOVE consumes it; the queue is then empty.
    pm[4] = 1u;
    r = call_stub(peek, pm, 5, sp);
    ck("PM_REMOVE returns the message", r == 1u && ts::ld32(msg + 4u) == 0x001cu);
    r = call_stub(peek, pm, 5, sp);
    ck("queue is empty after PM_REMOVE", r == 0u);

    // PostMessageA queues; GetMessageA returns it and removes it.
    ts::u32 a4[4] = { hwnd, 0x0400u, 7u, 9u };
    ck("PostMessageA succeeds", call_stub(post, a4, 4, sp) == 1u);
    ck("PostMessageA pops 4 args", ESP.d == sp + 16u);
    ts::u32 gm[4] = { msg, 0u, 0u, 0u };
    ck("GetMessageA returns a queued message", call_stub(getm, gm, 4, sp) == 1u);
    ck("GetMessageA pops 4 args", ESP.d == sp + 16u);
    ck("GetMessageA MSG.message", ts::ld32(msg + 4u) == 0x0400u);
    ck("GetMessageA MSG.wParam", ts::ld32(msg + 8u) == 7u);
    ck("GetMessageA MSG.lParam", ts::ld32(msg + 12u) == 9u);
    // An empty queue is NOT an immediate WM_QUIT: it yields a benign WM_NULL.
    ck("GetMessageA empty queue does not quit", call_stub(getm, gm, 4, sp) == 1u);
    ck("idle message is WM_NULL", ts::ld32(msg + 4u) == 0u);

    // WM_QUIT is the one message that makes GetMessageA return 0.
    ts::u32 a1q[1] = { 0u };
    call_stub(quit, a1q, 1, sp);
    ck("GetMessageA returns 0 for WM_QUIT", call_stub(getm, gm, 4, sp) == 0u);
    ck("WM_QUIT message delivered", ts::ld32(msg + 4u) == 0x0012u);

    // SendMessageA is synchronous and returns the procedure's value.
    ts::u32 a4s[4] = { hwnd, 0x0401u, 3u, 4u };
    g_calls = 0;
    call_stub(send, a4s, 4, sp);
    ck("SendMessageA pops 4 args", ESP.d == sp + 16u);
    ck("SendMessageA calls synchronously", g_calls == 1u);
    ck("SendMessageA message", g_msg == 0x0401u);
    ck("SendMessageA wParam", g_wp == 3u);
    ck("SendMessageA lParam", g_lp == 4u);

    std::printf("%d\n", fails);
    return fails ? 1 : 0;
}
'''
    open("/tmp/ts_win32_queue.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_win32_queue",
                        "/tmp/ts_win32_queue.cpp"], capture_output=True)
    if r.returncode != 0:
        check("win32-message-queue-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_win32_queue"], capture_output=True)
    out = r.stdout.decode("utf-8", "replace").strip()
    if out and out.splitlines()[-1] != "0":
        check("win32-message-queue", False, out)
    else:
        eq("win32-message-queue", out.splitlines()[-1] if out else "", "0")


def test_win32_dialog():
    """The dialog layer: template controls, WM_INITDIALOG, and scripted input.

    Regression test for the dialog-setup loop. CreateDialogIndirectParamA used
    to be a constant return: no window, no controls, no WM_INITDIALOG, and
    GetDlgItem a fixed handle, so the game re-ran the same setup forever.
    This checks the parts that make the loop break: a real dialog record, the
    template's own control IDs and text, WM_INITDIALOG sent before the call
    returns, a stable per-control handle (and 0 for an ID the template does
    not declare), window extra bytes at DWLP_USER (8), and a WM_COMMAND for
    the default button that the game's own PeekMessage/DispatchMessage loop
    carries to the dialog procedure. It also pins the four-argument arity of
    SetViewportOrgEx/SetWindowOrgEx: with three, two calls leaked 8 bytes and
    a caller four frames away restored a stale EBX.
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Win32Stubs.h"

static int fails = 0;
static void ck(const char *what, bool ok) {
    if (!ok) { std::printf("FAIL %s\n", what); ++fails; }
}

static ts::u32 g_calls, g_hwnd, g_msg, g_wp, g_lp;
static void fake_dlgproc() {
    ++g_calls;
    g_hwnd = ts::ld32(ESP.d + 4u);
    g_msg  = ts::ld32(ESP.d + 8u);
    g_wp   = ts::ld32(ESP.d + 12u);
    g_lp   = ts::ld32(ESP.d + 16u);
    ESP.d += 4u + 16u;   // __stdcall: pop the return address and 4 args
}

static ts::u32 call_stub(ts::u32 fn, const ts::u32 *a, unsigned n, ts::u32 sp) {
    for (unsigned i = 0; i < n; ++i) ts::st32(sp + i * 4u, a[i]);
    ESP.d = sp;
    ts::ts_call_at(fn, 0x00c0ffeeu);
    return EAX.d;
}

int main() {
    ts::register_win32_stubs();
    const ts::u32 sp = 0x002ff000u;
    const ts::u32 tpl = 0x00300000u, msg = 0x00300100u;
    const ts::u32 proc_va = 0x00c0de02u;
    ts::ts_register(proc_va, &fake_dlgproc);

    // DLGTEMPLATE: style, exstyle, cdit=1, rect, then menu/class/title = 0.
    ts::st32(tpl + 0u, 0u);
    ts::st32(tpl + 4u, 0u);
    ts::st16(tpl + 8u, 1u);
    ts::st16(tpl + 10u, 0u); ts::st16(tpl + 12u, 0u);
    ts::st16(tpl + 14u, 100u); ts::st16(tpl + 16u, 50u);
    ts::st16(tpl + 18u, 0u); ts::st16(tpl + 20u, 0u); ts::st16(tpl + 22u, 0u);
    // DLGITEMTEMPLATE at +24: a visible button, id 1, class 0x80, text "OK".
    ts::st32(tpl + 24u, 0x50000001u);   // WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON
    ts::st32(tpl + 28u, 0u);
    ts::st16(tpl + 32u, 0u); ts::st16(tpl + 34u, 0u);
    ts::st16(tpl + 36u, 50u); ts::st16(tpl + 38u, 14u);
    ts::st16(tpl + 40u, 1u);            // control id = IDOK
    ts::st16(tpl + 42u, 0xffffu); ts::st16(tpl + 44u, 0x0080u);   // "Button"
    ts::st16(tpl + 46u, 'O'); ts::st16(tpl + 48u, 'K'); ts::st16(tpl + 50u, 0u);
    ts::st16(tpl + 52u, 0u);            // cbCreationData

    ts::u32 cd = ts::stub_va("USER32", "CreateDialogIndirectParamA");
    ts::u32 gdi = ts::stub_va("USER32", "GetDlgItem");
    ts::u32 swl = ts::stub_va("USER32", "SetWindowLongA");
    ts::u32 gwl = ts::stub_va("USER32", "GetWindowLongA");
    ts::u32 enw = ts::stub_va("USER32", "EnableWindow");
    ts::u32 isw = ts::stub_va("USER32", "IsWindowEnabled");
    ts::u32 peek = ts::stub_va("USER32", "PeekMessageA");
    ts::u32 disp = ts::stub_va("USER32", "DispatchMessageA");
    ts::u32 dw = ts::stub_va("USER32", "DestroyWindow");
    ts::u32 vp = ts::stub_va("GDI32", "SetViewportOrgEx");
    ts::u32 wo = ts::stub_va("GDI32", "SetWindowOrgEx");
    ck("dialog stubs resolve", cd && gdi && swl && gwl && enw && isw && peek &&
                               disp && dw && vp && wo);

    // WM_INITDIALOG is sent synchronously, before CreateDialog returns.
    ts::u32 a5[5] = { 0x00400000u, tpl, 0u, proc_va, 0x1234u };
    g_calls = 0;
    ts::u32 dlg = call_stub(cd, a5, 5, sp);
    ck("CreateDialogIndirectParamA returns a handle", dlg != 0);
    ck("CreateDialogIndirectParamA pops 5 args", ESP.d == sp + 20u);
    ck("WM_INITDIALOG was sent once", g_calls == 1u && g_msg == 0x0110u);
    ck("WM_INITDIALOG hwnd", g_hwnd == dlg);
    ck("WM_INITDIALOG wParam is 0", g_wp == 0u);
    ck("WM_INITDIALOG lParam is dwInitParam", g_lp == 0x1234u);

    // The template's control has a stable handle; an undeclared ID does not.
    ts::u32 a2[2] = { dlg, 1u };
    ts::u32 ok = call_stub(gdi, a2, 2, sp);
    ck("GetDlgItem returns a control handle", ok != 0u && ok != dlg);
    ck("GetDlgItem is stable", call_stub(gdi, a2, 2, sp) == ok);
    ts::u32 a2b[2] = { dlg, 99u };
    ck("GetDlgItem returns 0 for an undeclared id", call_stub(gdi, a2b, 2, sp) == 0u);
    ck("GetDlgItem pops 2 args", ESP.d == sp + 8u);

    // DWLP_USER lives in the window's extra bytes at index 8.
    ts::u32 a3[3] = { dlg, 8u, 0x00badbeeu };
    ck("SetWindowLongA pops 3 args", call_stub(swl, a3, 3, sp) == 0u && ESP.d == sp + 12u);
    ts::u32 a2w[2] = { dlg, 8u };
    ck("GetWindowLongA returns DWLP_USER", call_stub(gwl, a2w, 2, sp) == 0x00badbeeu);

    // EnableWindow records state and returns the previous one.
    ts::u32 a2e[2] = { ok, 0u };
    ck("EnableWindow returns the previous state", call_stub(enw, a2e, 2, sp) == 1u);
    ts::u32 a1i[1] = { ok };
    ck("IsWindowEnabled reads it back", call_stub(isw, a1i, 1, sp) == 0u);

    // The scripted WM_COMMAND is queued for the default button, not sent.
    ts::u32 pm[5] = { msg, 0u, 0u, 0u, 1u };
    ck("PeekMessage sees the scripted WM_COMMAND", call_stub(peek, pm, 5, sp) == 1u);
    ck("WM_COMMAND hwnd", ts::ld32(msg + 0u) == dlg);
    ck("WM_COMMAND message", ts::ld32(msg + 4u) == 0x0111u);
    ck("WM_COMMAND control id is IDOK", ts::ld32(msg + 8u) == 1u);
    ck("WM_COMMAND lParam is the control", ts::ld32(msg + 12u) == ok);

    // Dispatching it runs the dialog procedure the game stored.
    ts::u32 a1m[1] = { msg };
    g_calls = 0;
    call_stub(disp, a1m, 1, sp);
    ck("DispatchMessageA reaches the dialog proc", g_calls == 1u);
    ck("dispatched WM_COMMAND", g_msg == 0x0111u && g_wp == 1u && g_lp == ok);

    // SetViewportOrgEx/SetWindowOrgEx take four arguments.
    ts::u32 a4[4] = { 0x00030001u, 0u, 0u, 0u };
    call_stub(vp, a4, 4, sp);
    ck("SetViewportOrgEx pops 4 args", ESP.d == sp + 16u);
    call_stub(wo, a4, 4, sp);
    ck("SetWindowOrgEx pops 4 args", ESP.d == sp + 16u);

    // Destroying the dialog frees its controls.
    ts::u32 a1d[1] = { dlg };
    call_stub(dw, a1d, 1, sp);
    ck("DestroyWindow pops 1 arg", ESP.d == sp + 4u);
    ck("GetDlgItem on a destroyed dialog is 0", call_stub(gdi, a2, 2, sp) == 0u);

    std::printf("%d\n", fails);
    return fails ? 1 : 0;
}
'''
    open("/tmp/ts_win32_dialog.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_win32_dialog",
                        "/tmp/ts_win32_dialog.cpp"], capture_output=True)
    if r.returncode != 0:
        check("win32-dialog-build", False,
              r.stderr.decode("utf-8", "replace")[:400])
        return
    r = subprocess.run(["/tmp/ts_win32_dialog"], capture_output=True)
    out = r.stdout.decode("utf-8", "replace").strip()
    if out and out.splitlines()[-1] != "0":
        check("win32-dialog", False, out)
    else:
        eq("win32-dialog", out.splitlines()[-1] if out else "", "0")


def test_win32_surface_lock():
    """IDirectDrawSurface::Lock pops all five words the game pushes.

    Regression test for the four-byte-per-Lock leak that walked the caller's
    frame at 0x487ad0, zeroed EBP, left the global AlternateSurface
    (0x714ed4) null and produced five null-`this` teardown calls. COM methods
    are __stdcall with `this` as the first STACK argument: the game's call
    site 0x487b31 pushes hEvent, flags, desc, rect and this and has no add-esp
    cleanup, so the callee owns five argument words. The vtable slot is driven
    exactly as the transcription drives it -- through ts_call_at, which pushes
    the return address -- and ESP must end at caller_ESP + 0x14.
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Win32Stubs.h"

static int fails = 0;
static void ck(const char *what, bool ok) {
    if (!ok) { std::printf("FAIL %s\n", what); ++fails; }
}

int main() {
    ts::reset();
    ts::register_win32_stubs();

    const ts::u32 sp = 0x002ff000u;
    ts::u32 surf = ts::w32::make_surface();
    ck("make_surface returns an object", surf != 0);
    ts::u32 vt = ts::ld32(surf);
    ts::u32 lock = ts::ld32(vt + 25u * 4u);   // IDirectDrawSurface::Lock
    ck("slot 25 is registered", lock != 0);
    ck("slot 25 is the Lock override, not a generic slot stub",
       lock != ts::ld32(vt + 26u * 4u));

    // The game's five pushed words, oldest push first: hEvent, flags, desc,
    // rect, this. In memory at the caller's ESP that is this at +0.
    const ts::u32 rect = 0x00300000u, desc = 0x00300100u, hevent = 0x00abcdeeu;
    ts::st32(sp + 0u, surf);           // this
    ts::st32(sp + 4u, rect);           // lpDestRect
    ts::st32(sp + 8u, desc);           // lpDDSurfaceDesc
    ts::st32(sp + 12u, 0x00000001u);   // dwFlags (DDLOCK_WAIT)
    ts::st32(sp + 16u, hevent);        // hEvent
    ESP.d = sp;
    ts::ts_call_at(lock, 0x00c0ffeeu);

    ck("Lock pops return address + five arguments",
       ESP.d == sp + 0x14u);
    ck("Lock does not fault", !ts::mach().faulted);
    ck("Lock fills DDSURFACEDESC.dwSize", ts::ld32(desc + 0u) == 108u);
    ck("Lock fills DDSURFACEDESC.lpSurface", ts::ld32(desc + 0x24u) != 0u);
    ck("Lock fills the pixel bit count", ts::ld32(desc + 0x54u) == 16u);

    std::printf("%d\n", fails);
    return fails ? 1 : 0;
}
'''
    open("/tmp/ts_surface_lock.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_surface_lock",
                        "/tmp/ts_surface_lock.cpp"], capture_output=True)
    if r.returncode != 0:
        check("win32-surface-lock-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_surface_lock"], capture_output=True)
    out = r.stdout.decode("utf-8", "replace").strip()
    if out and out.splitlines()[-1] != "0":
        check("win32-surface-lock", False, out)
    else:
        eq("win32-surface-lock", out.splitlines()[-1] if out else "", "0")


def test_win32_clock():
    """The clock must advance with executed work, not only with Sleep.

    Regression test for the VQA movie stall: tick() was a static counter that
    only Sleep advanced, but the movie pump polls timeGetTime in a busy loop
    and never sleeps. Time froze, the stream pacer (sub_006786a0) returned -10
    ("too early") forever, the audio descriptor's done bit was never cleared,
    and the decoder returned -9 without decoding a frame. Before the fix this
    test's first check failed (the clock did not move at all).
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Win32Stubs.h"

static int fails = 0;
static void ck(const char *what, bool ok) {
    if (!ok) { std::printf("FAIL %s\n", what); ++fails; }
}

int main() {
    ts::mach().steps = 0;
    ts::u32 t0 = ts::w32::tick();
    ts::mach().steps = 20000;
    ts::u32 t1 = ts::w32::tick();
    ck("clock advances with executed work", t1 == t0 + 2u);

    ts::u32 t2 = ts::w32::tick_advance(33u);
    ck("Sleep advances the clock", t2 == t1 + 33u);

    ts::mach().steps = 40000;
    ck("clock keeps advancing after Sleep", ts::w32::tick() == t2 + 2u);

    std::printf("%d\n", fails);
    return fails ? 1 : 0;
}
'''
    open("/tmp/ts_win32_clock.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_win32_clock",
                        "/tmp/ts_win32_clock.cpp"], capture_output=True)
    if r.returncode != 0:
        check("win32-clock-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_win32_clock"], capture_output=True)
    out = r.stdout.decode("utf-8", "replace").strip()
    if out and out.splitlines()[-1] != "0":
        check("win32-clock", False, out)
    else:
        eq("win32-clock", out.splitlines()[-1] if out else "", "0")


def test_win32_blowfish():
    """IBlowfish is a real Blowfish-ECB cipher, not an identity copy.

    Regression test for the MIX-index blocker: slots 6/7 copied their input
    unchanged, so the game's TIBSUN.MIX index decrypted to ciphertext, the
    PKStraw read returned 0, and a wrapped operator_new exhausted the page
    table. The four standard known-answer vectors prove the pi tables, the key
    schedule and the 16-round Feistel; the fifth vector is the first block of
    the shipped TIBSUN.MIX index, with the key tools/mix.py derives and the
    plaintext PyCryptodome produces (count=17, body=75,094,416). Round-trip,
    in-place, multi-block, tail-copy, unkeyed-copy and argument validation are
    checked too, all through the dispatch table, including stack balance.
    """
    import subprocess
    src = r'''
#include <cstdio>
#include "Win32Stubs.h"

static int fails = 0;
static void ck(const char *what, bool ok) {
    if (!ok) { std::printf("FAIL %s\n", what); ++fails; }
}
static void put(ts::u32 at, const unsigned char *b, unsigned n) {
    for (unsigned i = 0; i < n; ++i) ts::st8(at + i, b[i]);
}
static bool same(ts::u32 at, const unsigned char *b, unsigned n) {
    for (unsigned i = 0; i < n; ++i) if (ts::ld8(at + i) != b[i]) return false;
    return true;
}
// Call a vtable slot exactly as a transcription does.
static ts::u32 call(ts::u32 fn, const ts::u32 *a, unsigned n, ts::u32 sp) {
    for (unsigned i = 0; i < n; ++i) ts::st32(sp + i * 4u, a[i]);
    ESP.d = sp;
    ts::ts_call_at(fn, 0x00c0ffeeu);
    return EAX.d;
}
static const ts::u32 SP0 = 0x002ff000u;

int main() {
    ts::register_win32_stubs();
    ts::u32 obj = ts::w32::make_blowfish_object();
    ts::u32 vt = ts::ld32(obj);
    ts::u32 setkey = ts::ld32(vt + 12u);
    ts::u32 enc = ts::ld32(vt + 24u);
    ts::u32 dec = ts::ld32(vt + 28u);
    ck("slots 3/6/7 registered",
       ts::ts_lookup(setkey) && ts::ts_lookup(enc) && ts::ts_lookup(dec));

    ts::u32 key = ts::w32::alloc_block(64u, 4u);
    ts::u32 pt = ts::w32::alloc_block(64u, 4u);
    ts::u32 ct = ts::w32::alloc_block(64u, 4u);
    ts::u32 out = ts::w32::alloc_block(64u, 4u);

    struct V { const char *name; const unsigned char *k; unsigned klen;
               const unsigned char *p; const unsigned char *c; };
    // The four standard Blowfish known-answer vectors.
    static const unsigned char k0[8] = {0,0,0,0,0,0,0,0};
    static const unsigned char p0[8] = {0,0,0,0,0,0,0,0};
    static const unsigned char c0[8] = {0x4E,0xF9,0x97,0x45,0x61,0x98,0xDD,0x78};
    static const unsigned char k1[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    static const unsigned char c1[8] = {0x51,0x86,0x6F,0xD5,0xB8,0x5E,0xCB,0x8A};
    static const unsigned char k2[8] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
    static const unsigned char p2[8] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
    static const unsigned char c2[8] = {0x61,0xF9,0xC3,0x80,0x22,0x81,0xB0,0x96};
    static const unsigned char k3[26] = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','z' };
    static const unsigned char p3[8] = {'B','L','O','W','F','I','S','H'};
    static const unsigned char c3[8] = {0x32,0x4E,0xD0,0xFE,0xF4,0x13,0xA2,0x03};
    // The shipped TIBSUN.MIX index: the key tools/mix.py derives from the
    // archive's RSA block, its first 8-byte ciphertext block, and the plaintext
    // PyCryptodome decodes (u16 count=17, u32 body=75,094,416).
    static const unsigned char kt[56] = {
        0x82,0x06,0x03,0x23,0x1a,0x9d,0xb6,0x43,
        0x56,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
        0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
        0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
        0xd2,0x3b,0x90,0xc2,0xac,0x04,0x3a,0x40,
        0xdc,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,
        0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb };
    static const unsigned char pt_c[8] = {0x2c,0xe3,0x17,0x1b,0xdb,0xec,0xb4,0x0e};
    static const unsigned char pt_p[8] = {0x11,0x00,0x90,0xd9,0x79,0x04,0x84,0x9a};
    static const V vecs[5] = {
        { "0000000000000000/0000000000000000", k0, 8, p0, c0 },
        { "FFFFFFFFFFFFFFFF/FFFFFFFFFFFFFFFF", k1, 8, k1, c1 },
        { "0123456789ABCDEF/1111111111111111", k2, 8, p2, c2 },
        { "abcdefghijklmnopqrstuvwxyz/BLOWFISH", k3, 26, p3, c3 },
        { "TIBSUN.MIX index block 0", kt, 56, pt_p, pt_c },
    };

    for (unsigned i = 0; i < 5u; ++i) {
        put(key, vecs[i].k, vecs[i].klen);
        put(pt, vecs[i].p, 8);
        for (unsigned j = 0; j < 8u; ++j) ts::st8(ct + j, 0xAA);
        ts::u32 a[3] = { obj, vecs[i].klen, key };
        ck("SetKey S_OK", call(setkey, a, 3, SP0) == 0u);
        ck("SetKey pops 3", ESP.d == SP0 + 12u);
        ck("key length stored exactly", ts::w32::blowfish_key_len() == vecs[i].klen);
        bool same_key = true;
        for (unsigned j = 0; j < vecs[i].klen; ++j)
            if (ts::w32::blowfish_key()[j] != vecs[i].k[j]) same_key = false;
        ck("key bytes stored exactly", same_key);
        ts::u32 e[4] = { obj, 8u, pt, ct };
        ck("Encrypt S_OK", call(enc, e, 4, SP0) == 0u);
        ck("Encrypt pops 4", ESP.d == SP0 + 16u);
        if (!same(ct, vecs[i].c, 8)) {
            std::printf("FAIL vector %s: got ", vecs[i].name);
            for (unsigned j = 0; j < 8u; ++j) std::printf("%02X", ts::ld8(ct + j));
            std::printf("\n");
            ++fails;
        }
        ts::u32 d[4] = { obj, 8u, ct, out };
        ck("Decrypt S_OK", call(dec, d, 4, SP0) == 0u);
        ck("Decrypt pops 4", ESP.d == SP0 + 16u);
        if (!same(out, vecs[i].p, 8)) {
            std::printf("FAIL round trip %s\n", vecs[i].name);
            ++fails;
        }
    }

    // Two consecutive blocks transform independently, and round-trip.
    {
        unsigned char p[16] = {0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
                               0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22};
        put(key, k2, 8); put(pt, p, 16);
        ts::u32 a[3] = { obj, 8u, key };
        call(setkey, a, 3, SP0);
        ts::u32 e[4] = { obj, 16u, pt, ct };
        call(enc, e, 4, SP0);
        ck("block 0 of a 16-byte call", same(ct, c2, 8));
        ts::u32 d[4] = { obj, 16u, ct, out };
        call(dec, d, 4, SP0);
        ck("two-block round trip", same(out, p, 16));
    }

    // The game passes in == out; reading both words before writing is what
    // makes that safe.
    {
        unsigned char p[8] = {0,0,0,0,0,0,0,0};
        put(key, k0, 8); put(pt, p, 8);
        ts::u32 a[3] = { obj, 8u, key };
        call(setkey, a, 3, SP0);
        ts::u32 e[4] = { obj, 8u, pt, pt };
        call(enc, e, 4, SP0);
        ck("in-place encrypt", same(pt, c0, 8));
        ts::u32 d[4] = { obj, 8u, pt, pt };
        call(dec, d, 4, SP0);
        ck("in-place round trip", same(pt, p, 8));
    }

    // len % 8: whole blocks transform, the tail is copied verbatim.
    {
        unsigned char p[10] = {0,0,0,0,0,0,0,0,0x5A,0xA5};
        put(key, k0, 8); put(pt, p, 10);
        ts::u32 a[3] = { obj, 8u, key };
        call(setkey, a, 3, SP0);
        ts::u32 e[4] = { obj, 10u, pt, ct };
        call(enc, e, 4, SP0);
        ck("whole block of a 10-byte call", same(ct, c0, 8));
        ck("tail copied verbatim",
           ts::ld8(ct + 8u) == 0x5A && ts::ld8(ct + 9u) == 0xA5);
    }

    // With no key the DLL copies verbatim (SetKey length 0 clears the flag).
    {
        unsigned char p[8] = {1,2,3,4,5,6,7,8};
        put(pt, p, 8);
        ts::u32 a[3] = { obj, 0u, key };
        call(setkey, a, 3, SP0);
        ts::u32 e[4] = { obj, 8u, pt, ct };
        call(enc, e, 4, SP0);
        ck("unkeyed Encrypt copies", same(ct, p, 8));
        ts::u32 d[4] = { obj, 8u, pt, out };
        call(dec, d, 4, SP0);
        ck("unkeyed Decrypt copies", same(out, p, 8));
    }

    // The DLL's argument validation.
    {
        ts::u32 a[3] = { obj, 0u, 0u };
        ck("SetKey(NULL) -> E_POINTER", call(setkey, a, 3, SP0) == 0x80004003u);
        ts::u32 a2[3] = { obj, 57u, key };
        ck("SetKey(57) -> E_INVALIDARG", call(setkey, a2, 3, SP0) == 0x80070057u);
        ts::u32 e[4] = { obj, 8u, 0u, ct };
        ck("Encrypt(NULL in) -> E_POINTER", call(enc, e, 4, SP0) == 0x80004003u);
        ts::u32 d[4] = { obj, 8u, ct, 0u };
        ck("Decrypt(NULL out) -> E_POINTER", call(dec, d, 4, SP0) == 0x80004003u);
    }

    std::printf("%d\n", fails);
    return fails ? 1 : 0;
}
'''
    open("/tmp/ts_win32_blowfish.cpp", "w").write(src)
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Ibuild/shim", "-o", "/tmp/ts_win32_blowfish",
                        "/tmp/ts_win32_blowfish.cpp"], capture_output=True)
    if r.returncode != 0:
        check("win32-blowfish-build", False,
              r.stderr.decode("utf-8", "replace")[:300])
        return
    r = subprocess.run(["/tmp/ts_win32_blowfish"], capture_output=True)
    out = r.stdout.decode("utf-8", "replace").strip()
    if out and out.splitlines()[-1] != "0":
        check("win32-blowfish", False, out)
    else:
        eq("win32-blowfish", out.splitlines()[-1] if out else "", "0")


def main():
    for fn in (test_emit_masking, test_emit_in_out, test_emit_loop_tail,
               test_emit_ret, test_decl_destructor, test_slot_va,
               test_slot0_shared, test_reduce_bases, test_tail_entries,
               test_indirect_tail_targets, test_jump_table_data,
               test_machine_mul, test_machine_div, test_machine_flags,
               test_machine_narrow_flags,
               test_machine_stack, test_machine_mmx,
               test_machine_depth_guard, test_machine_boot_regressions,
               test_win32_module_exports, test_win32_message_queue,
               test_win32_dialog,
               test_win32_surface_lock, test_win32_clock, test_win32_blowfish):
        try:
            fn()
        except Exception as exc:  # a tool that raises is a failure, not a skip
            FAILURES.append("%s: raised %s: %s" % (fn.__name__,
                                                   type(exc).__name__, exc))
    if FAILURES:
        print("tool unit tests: %d of %d checks FAILED" %
              (len(FAILURES), CHECKS[0]))
        for f in FAILURES:
            print("  - " + f)
        return 1
    print("tool unit tests: %d checks passed" % CHECKS[0])
    return 0


if __name__ == "__main__":
    sys.exit(main())
