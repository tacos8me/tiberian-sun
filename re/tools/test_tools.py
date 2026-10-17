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
  Machine mul/div         -- differential against 64-bit reference arithmetic
  Machine flags           -- add/sub carry and overflow, against the manual
  Machine stack           -- push/pop balance
  Machine mmx lanes       -- differential against lane-array reference code
  gen_code tails          -- cross-function tail targets are found

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


def main():
    for fn in (test_emit_masking, test_emit_in_out, test_emit_loop_tail,
               test_emit_ret, test_decl_destructor, test_slot_va,
               test_slot0_shared, test_reduce_bases, test_tail_entries,
               test_machine_mul, test_machine_flags, test_machine_stack,
               test_machine_mmx, test_machine_depth_guard):
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
