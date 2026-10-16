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
    check("loop-tail-dispatch", "ts::ts_call" in body, body)
    # A target INSIDE the span is still a label.
    insns = [{"va": 0x2000, "mnem": "loop", "op": "0x2000", "bytes": "e2fe"}]
    items, _st = E.transcribe(insns, None, 0x2000, 0x2004, ret_type="")
    body = "\n".join(c for k, c, _ in items if k == "stmt")
    check("loop-in-span-goto", "goto L_00002000" in body, body)


def test_emit_ret():
    import emit_x86 as E
    ctx = E.Ctx(0x1000, 0x1004, 0x1000, 0x1004, None, "", set())
    # void: pop and return, never `return <value>` (illegal in a constructor).
    eq("ret-void", E._ret(0, ctx), "return;")
    eq("ret-void-imm", E._ret(4, ctx), "ts::ret_pop(0x4u); return;")
    # a value-returning function yields EAX.
    ctx2 = E.Ctx(0x1000, 0x1004, 0x1000, 0x1004, None, "HRESULT", set())
    eq("ret-typed", E._ret(0, ctx2), "return (HRESULT)(size_t)EAX.d;")


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
               test_machine_depth_guard):
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
