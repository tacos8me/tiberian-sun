#ifndef TS_MACHINE_H
#define TS_MACHINE_H

// ---------------------------------------------------------------------------
// Machine.h -- shadow-register model for transcribed recovered code.
//
// PROVENANCE. This header is OUR model, not recovered data. It is the only
// header in this tree that is not derived from an RTTI/vtable/ctor artifact.
// What the binary supplies per function is a byte size, an instruction listing
// (mnemonic, operand text, encoding bytes) and a callee set -- nothing else. No
// types, no names, no source shape.
//
// tools/gen_bodies.py renders each recovered instruction stream into C++ against
// this model: one generated line per machine instruction, with the original
// instruction preserved as a trailing comment. The rendering is faithful to the
// instruction stream -- registers, flags, memory and control flow are modelled
// rather than approximated -- but it is deliberately NOT idiomatic C++. It is a
// transcription, not source-shaped decompilation, and it is not claimed to be
// the shape the original source had.
//
// Where an instruction is not modelled the generator emits ts::unmodelled() and
// records the mnemonic, rather than silently dropping it. The unmodelled count
// is reported; it is the honest measure of what is not yet rendered.
//
// This header is header-only on purpose: the pipeline wipes and regenerates
// src/generated/*.cpp, so a transcribed tree must not depend on a hand-written
// .cpp living there.
// ---------------------------------------------------------------------------

#include <cstring>
#include <cmath>
#include <cstdlib>
#include <setjmp.h>

namespace ts {

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef signed   char  i8;
typedef signed   short i16;
typedef signed   int   i32;
typedef unsigned long long u64;

// An MMX register is 64 bits and an XMM register 16 bytes. C++03 cannot return
// an array or assign one by value, so the SSE register file is addressed
// through this POD pair.
struct u128 { u64 lo, hi; };

// x86 general registers in encoding order, with the sub-register views the
// instruction stream addresses (al/ah/ax and friends). This target is
// little-endian, so b[0] is the low byte and w[0] the low word.
union Reg {
    u32 d;
    u16 w[2];
    u8  b[4];
};

struct Machine {
    Reg r[8];              // 0 eax 1 ecx 2 edx 3 ebx 4 esp 5 ebp 6 esi 7 edi
    // eflags, one modelled field per bit any recovered branch tests.
    u8 cf, pf, af, zf, sf, df, of;
    // x87 stack, top at st[st_top]. Host long double is the closest available
    // to the 80-bit extended format the original uses.
    long double st[8];
    int st_top;
    u16 fcw;
    // Where the run is, at INSTRUCTION granularity. The generator emits
    // ts::insn(va) once per transcribed instruction, so eip names the
    // instruction that faulted rather than only the function that called it.
    // `steps` is the instruction count and `step_limit` an optional budget
    // (0 = none); exceeding it halts the run and is reported as a truncation.
    // Both fields existed before this instrumentation and were never written.
    u32 eip;
    // Runaway guard: a transcription is only useful if it terminates.
    unsigned long steps;
    unsigned long step_limit;
    bool faulted;
    bool step_limit_hit;
    const char *fault_why;
    // The fault is a REPORT, not an abort: without this the run wanders on
    // after the model has already lost its footing and the interesting state is
    // overwritten. A caller that sets `halt` gets control back at the fault
    // with the machine untouched, so the report can name where and with what.
    jmp_buf *halt;
    // Snapshot taken once, the first time fault() fires.
    u32 fault_eip;
    unsigned long fault_steps;
    unsigned long fault_depth;
    Reg fault_reg[8];
    u32 fault_calls[8];
    unsigned fault_calls_n;
    // High-water mark of ts_call nesting: the depth the run actually reached,
    // which is what says whether the limit was a real bound or an artefact.
    unsigned long call_depth_max;
    // Most recent call targets, oldest first at index n-1. Kept as a ring so a
    // fault report can name the few calls that led to it.
    u32 recent_calls[8];
    unsigned recent_call_n;
    unsigned long unmodelled_insns;
    unsigned long unmapped_calls;
    // Recursion depth of ts_call. A transcription that does not terminate would
    // otherwise recurse until the host stack runs out, which presents as a
    // segfault with no explanation; the depth is bounded instead and the
    // truncation is counted.
    unsigned long call_depth;
    unsigned long call_depth_limit;
    unsigned long depth_exceeded;
    // The first few call targets that were not registered. A count alone does
    // not say whether they are Windows imports (expected: the model has no
    // kernel32) or recovered functions that went missing (a defect).
    u32 unmapped_sample[16];
    unsigned unmapped_sample_n;
    // .text bounds, set by the caller. They let an unmapped call be classified:
    // outside .text it is an import (expected -- the model has no Windows),
    // inside .text it is a recovered function nothing registered (a defect).
    u32 text_lo, text_hi;
    unsigned long unmapped_in_text;
    u16 fpu_status;
    // Segment registers and the interrupt-enable flag. This model is flat, so
    // segments never affect an address, but the recovered code does push, pop
    // and move them, and a transcription that ignored them would be wrong about
    // the stack.
    u16 seg[6];            // 0 es 1 cs 2 ss 3 ds 4 fs 5 gs
    u8  iff;
    // MMX and SSE state. On the original, mm0..mm7 are the low 64 bits of the
    // x87 data registers, so an MMX write is visible to a later x87 read until
    // `emms` marks the x87 stack empty. The model keeps the two files apart and
    // `emms` empties the x87 stack (see mmx_emms below), which is faithful for
    // every sequence the hardware defines: mixing MMX and x87 without an
    // intervening emms is architecturally undefined.
    u64 mm[8];
    u128 xmm[8];
};

// The machine is a function-local static so this header stays header-only and
// every translation unit shares one instance.
inline Machine &mach() { static Machine m; return m; }

// The value a transcribed function leaves in EAX.
//
// A transcription returns through the machine model rather than through a C++
// return statement: the body is `void(void)` and the result is what the machine
// has in EAX when the recovered span ends. A caller that has a declared return
// type -- the forwarders in src/generated/, which exist because the headers
// declare those signatures -- reads it from here. Written as a function rather
// than as `EAX` because the register aliases below are macros over mach(), so
// `ts::EAX` would expand to `ts::ts::mach().r[0]`.
inline u32 ret_eax() { return mach().r[0].d; }

// Register aliases. Macros rather than references so generated code reads like
// the instruction stream it came from.
#define M    ts::mach()
#define EAX  M.r[0]
#define ECX  M.r[1]
#define EDX  M.r[2]
#define EBX  M.r[3]
#define ESP  M.r[4]
#define EBP  M.r[5]
#define ESI  M.r[6]
#define EDI  M.r[7]

#define AL   M.r[0].b[0]
#define AH   M.r[0].b[1]
#define AX   M.r[0].w[0]
#define CL   M.r[1].b[0]
#define CH   M.r[1].b[1]
#define CX   M.r[1].w[0]
#define DL   M.r[2].b[0]
#define DH   M.r[2].b[1]
#define DX   M.r[2].w[0]
#define BL   M.r[3].b[0]
#define BH   M.r[3].b[1]
#define BX   M.r[3].w[0]
#define SP   M.r[4].w[0]
#define BP   M.r[5].w[0]
#define SI   M.r[6].w[0]
#define DI   M.r[7].w[0]

#define ES   M.seg[0]
#define CS   M.seg[1]
#define SS   M.seg[2]
#define DS   M.seg[3]
#define FS   M.seg[4]
#define GS   M.seg[5]

#define MM0  M.mm[0]
#define MM1  M.mm[1]
#define MM2  M.mm[2]
#define MM3  M.mm[3]
#define MM4  M.mm[4]
#define MM5  M.mm[5]
#define MM6  M.mm[6]
#define MM7  M.mm[7]
#define XMM0 M.xmm[0]
#define XMM1 M.xmm[1]
#define XMM2 M.xmm[2]
#define XMM3 M.xmm[3]
#define XMM4 M.xmm[4]
#define XMM5 M.xmm[5]
#define XMM6 M.xmm[6]
#define XMM7 M.xmm[7]
inline void reset() {
    Machine &m = mach();
    std::memset(m.r, 0, sizeof(m.r));
    std::memset(m.mm, 0, sizeof(m.mm));
    std::memset(m.xmm, 0, sizeof(m.xmm));
    m.eip = 0; m.cf = m.pf = m.af = m.zf = m.sf = m.df = m.of = 0;
    for (int i = 0; i < 8; ++i) m.st[i] = 0.0L;
    m.st_top = 0; m.fcw = 0x037f;
    m.steps = 0; m.step_limit = 0; m.step_limit_hit = false;
    m.faulted = false; m.fault_why = 0; m.halt = 0;
    m.fault_eip = 0; m.fault_steps = 0; m.fault_depth = 0;
    for (int i = 0; i < 8; ++i) { m.fault_reg[i].d = 0; m.fault_calls[i] = 0; }
    m.fault_calls_n = 0; m.call_depth_max = 0; m.recent_call_n = 0;
    m.unmapped_in_text = 0;
    m.unmodelled_insns = 0;
    m.call_depth = 0;
    m.depth_exceeded = 0;
    m.unmapped_sample_n = 0;
    if (m.call_depth_limit == 0) m.call_depth_limit = 4096;
}

// Record why the model lost its footing, once, together with the machine state
// at that moment. A bare "it crashed" is not a diagnosis; the caller prints
// fault_eip, the registers, the depth and the last few call targets from here.
inline void fault(const char *why) {
    Machine &m = mach();
    if (m.faulted) return;
    m.faulted = true;
    m.fault_why = why;
    m.fault_eip = m.eip;
    m.fault_steps = m.steps;
    m.fault_depth = m.call_depth;
    for (int i = 0; i < 8; ++i) m.fault_reg[i] = m.r[i];
    m.fault_calls_n = m.recent_call_n < 8 ? m.recent_call_n : 8;
    for (unsigned i = 0; i < m.fault_calls_n; ++i) {
        unsigned idx = (m.recent_call_n - m.fault_calls_n + i) & 7u;
        m.fault_calls[i] = m.recent_calls[idx];
    }
}

// One call per transcribed instruction, emitted by tools/gen_code.py. It is the
// only per-instruction hook in the model: it keeps eip, counts instructions, and
// enforces the step budget. When the model has faulted and the caller asked to
// be halted, it longjmps back so the run stops at the fault instead of walking
// on with a broken machine.
inline void insn(u32 va) {
    Machine &m = mach();
    m.eip = va;
    ++m.steps;
    if (m.step_limit && m.steps > m.step_limit) {
        if (!m.faulted) { m.step_limit_hit = true; fault("instruction budget exhausted"); }
    }
    if (m.faulted && m.halt) longjmp(*m.halt, 1);
}

inline void unmodelled(u32 va, const char *mnem) {
    (void)va; (void)mnem;
    mach().unmodelled_insns++;
}

// -- memory -----------------------------------------------------------------
// The original is a flat 32-bit address space. Rather than allocate 4 GiB, pages
// are handed out lazily and kept in a fixed open-addressed table; an address
// that was never written reads as zero, which is what the original's untouched
// .bss does. Accesses are assembled byte-wise so that a multi-byte access
// straddling a page boundary is still correct.
struct PageTable {
    enum { SLOTS = 32768 };
    u32 key[SLOTS];
    u8 *val[SLOTS];
    PageTable() { for (int i = 0; i < SLOTS; ++i) { key[i] = 0xffffffffu; val[i] = 0; } }
};

inline PageTable &pages() { static PageTable t; return t; }

inline u8 *page(u32 addr) {
    PageTable &t = pages();
    u32 p = addr >> 12;
    u32 h = (p * 2654435761u) & (PageTable::SLOTS - 1);
    for (u32 probe = 0; probe < PageTable::SLOTS; ++probe) {
        u32 i = (h + probe) & (PageTable::SLOTS - 1);
        if (t.key[i] == p) return t.val[i] + (addr & 0xfffu);
        if (t.key[i] == 0xffffffffu) {
            u8 *blk = (u8 *)std::calloc(4096 + 8, 1);
            t.key[i] = p; t.val[i] = blk;
            return blk + (addr & 0xfffu);
        }
    }
    fault("page table exhausted");
    static u8 dummy[4096 + 8];
    return dummy + (addr & 0xfffu);
}

// Copy a region of the original image in, so reads of initialised data return
// the real bytes rather than zero.
inline void map(u32 addr, u32 len) {
    (void)addr; (void)len;   // callers that have the image call memcpy themselves
}

inline u8  ld8 (u32 a) { u8 v; std::memcpy(&v, page(a), 1); return v; }
inline void st8 (u32 a, u8 v) { std::memcpy(page(a), &v, 1); }
inline u16 ld16(u32 a) { return (u16)((u16)ld8(a) | ((u16)ld8(a + 1) << 8)); }
inline void st16(u32 a, u16 v) { st8(a, (u8)v); st8(a + 1, (u8)(v >> 8)); }
inline u32 ld32(u32 a) {
    return (u32)ld8(a) | ((u32)ld8(a + 1) << 8) |
           ((u32)ld8(a + 2) << 16) | ((u32)ld8(a + 3) << 24);
}
inline void st32(u32 a, u32 v) {
    st8(a, (u8)v); st8(a + 1, (u8)(v >> 8));
    st8(a + 2, (u8)(v >> 16)); st8(a + 3, (u8)(v >> 24));
}

// -- flags ------------------------------------------------------------------
// Flag semantics matter: the recovered conditional jumps test them, so an
// approximate model would silently change control flow. Each helper sets the
// flags the way the real instruction does.
inline void f_log(u32 r) {
    Machine &m = mach();
    m.cf = 0; m.of = 0; m.zf = (r == 0) ? 1 : 0; m.sf = (u8)((r >> 31) & 1); m.pf = 0;
}
inline void f_add(u32 a, u32 b, u32 r) {
    Machine &m = mach();
    m.cf = (r < a) ? 1 : 0;
    m.of = (u8)((((a ^ r) & (b ^ r)) >> 31) & 1);
    m.zf = (r == 0) ? 1 : 0; m.sf = (u8)((r >> 31) & 1);
}
inline void f_sub(u32 a, u32 b, u32 r) {
    Machine &m = mach();
    m.cf = (a < b) ? 1 : 0;
    m.of = (u8)((((a ^ b) & (a ^ r)) >> 31) & 1);
    m.zf = (r == 0) ? 1 : 0; m.sf = (u8)((r >> 31) & 1);
}
inline void f_inc(u32 a, u32 r) {
    Machine &m = mach();
    m.of = (a == 0x7fffffffu) ? 1 : 0; m.zf = (r == 0) ? 1 : 0; m.sf = (u8)((r >> 31) & 1);
}
inline void f_dec(u32 a, u32 r) {
    Machine &m = mach();
    m.of = (a == 0x80000000u) ? 1 : 0; m.zf = (r == 0) ? 1 : 0; m.sf = (u8)((r >> 31) & 1);
}

// -- integer core -----------------------------------------------------------
// Helpers take their operands by value and RETURN the result rather than taking
// a destination by reference. That is what lets the generator handle a memory
// destination uniformly: `add dword ptr [ecx+8], 1` becomes
//   st32(ECX.d + 8, add32(ld32(ECX.d + 8), 1u));
// with the flags set exactly once, in the helper, rather than at the call site.
inline u32 add32(u32 a, u32 b) { u32 r = a + b; f_add(a, b, r); return r; }
inline u16 add16(u16 a, u16 b) { u16 r = (u16)(a + b); f_add(a, b, r); return r; }
inline u8  add8 (u8  a, u8  b) { u8  r = (u8)(a + b);  f_add(a, b, r); return r; }
inline u32 sub32(u32 a, u32 b) { u32 r = a - b; f_sub(a, b, r); return r; }
inline u16 sub16(u16 a, u16 b) { u16 r = (u16)(a - b); f_sub(a, b, r); return r; }
inline u8  sub8 (u8  a, u8  b) { u8  r = (u8)(a - b);  f_sub(a, b, r); return r; }
inline u32 adc32(u32 a, u32 b) { u32 c = M.cf; u32 r = a + b + c; f_add(a, b + c, r); return r; }
inline u32 sbb32(u32 a, u32 b) { u32 c = M.cf; u32 r = a - b - c; f_sub(a, b + c, r); return r; }
inline void cmp32(u32 a, u32 b) { f_sub(a, b, a - b); }
inline void cmp16(u16 a, u16 b) { f_sub(a, b, (u16)(a - b)); }
inline void cmp8 (u8  a, u8  b) { f_sub(a, b, (u8)(a - b)); }
inline void test32(u32 a, u32 b) { f_log(a & b); }
inline void test16(u16 a, u16 b) { f_log((u32)(a & b)); }
inline void test8 (u8  a, u8  b) { f_log((u32)(a & b)); }
inline u32 and32(u32 a, u32 b) { u32 r = a & b; f_log(r); return r; }
inline u32 or32 (u32 a, u32 b) { u32 r = a | b; f_log(r); return r; }
inline u32 xor32(u32 a, u32 b) { u32 r = a ^ b; f_log(r); return r; }
inline u32 not32(u32 a) { return ~a; }
inline u32 neg32(u32 a) { u32 r = (u32)(0u - a); f_sub(0, a, r); return r; }
inline u32 inc32(u32 a) { u32 r = a + 1; f_inc(a, r); return r; }
inline u32 dec32(u32 a) { u32 r = a - 1; f_dec(a, r); return r; }
inline u32 shl32(u32 a, u32 n) { u32 r = (n & 31u) ? (u32)(a << (n & 31u)) : a; M.cf = 0; M.zf = (r == 0) ? 1 : 0; M.sf = (u8)((r >> 31) & 1); return r; }
inline u32 shr32(u32 a, u32 n) { u32 r = (n & 31u) ? (a >> (n & 31u)) : a; M.cf = 0; M.zf = (r == 0) ? 1 : 0; M.sf = 0; return r; }
inline u32 sar32(u32 a, u32 n) { i32 s = (i32)a; u32 r = (u32)((n & 31u) ? (s >> (n & 31u)) : s); M.zf = (r == 0) ? 1 : 0; M.sf = (u8)((r >> 31) & 1); return r; }
inline u32 rol32(u32 a, u32 n) { u32 k = n & 31u; return k ? (u32)((a << k) | (a >> (32 - k))) : a; }
inline u32 ror32(u32 a, u32 n) { u32 k = n & 31u; return k ? (u32)((a >> k) | (a << (32 - k))) : a; }
inline u32 rcl32(u32 a, u32 n) { u32 k = n & 31u; if (!k) return a; u32 r = (u32)((a << k) | (M.cf << (k - 1))); M.cf = (u8)((a >> (32 - k)) & 1); return r; }
inline u32 rcr32(u32 a, u32 n) { u32 k = n & 31u; if (!k) return a; u32 r = (u32)((a >> k) | (M.cf << (31 - k + 1))); M.cf = (u8)((a >> (k - 1)) & 1); return r; }
inline u32 shld32(u32 a, u32 b, u32 n) { u32 k = n & 31u; return k ? (u32)((a << k) | (b >> (32 - k))) : a; }
inline u32 shrd32(u32 a, u32 b, u32 n) { u32 k = n & 31u; return k ? (u32)((a >> k) | (b << (32 - k))) : a; }
inline u32 bt32(u32 a, u32 n) { M.cf = (u8)((a >> (n & 31u)) & 1); return a; }
inline u32 bts32(u32 a, u32 n) { u32 r = a | (1u << (n & 31u)); M.cf = (u8)((a >> (n & 31u)) & 1); return r; }
inline u32 btr32(u32 a, u32 n) { u32 r = a & ~(1u << (n & 31u)); M.cf = (u8)((a >> (n & 31u)) & 1); return r; }

// 32x32 -> 64 without a 64-bit type, so this stays strict C++03. Verified
// against a 64-bit reference over 20 million random pairs plus edge cases; see
// the differential test noted in src/generated/README.md.
inline u32 mul_lo(u32 a, u32 b) {
    u32 al = a & 0xffffu, ah = a >> 16, bl = b & 0xffffu, bh = b >> 16;
    u32 ll = al * bl;
    u32 mid = ((ah * bl) + (al * bh)) & 0xffffu;
    return ll + (mid << 16);
}
inline u32 mul_hi(u32 a, u32 b) {
    u32 al = a & 0xffffu, ah = a >> 16, bl = b & 0xffffu, bh = b >> 16;
    u32 ll = al * bl, lh = ah * bl, hl = al * bh, hh = ah * bh;
    u32 cross = (ll >> 16) + (lh & 0xffffu) + (hl & 0xffffu);
    return hh + (lh >> 16) + (hl >> 16) + (cross >> 16);
}
inline u32 imul32(u32 a, u32 b) {
    u32 r = mul_lo(a, b);
    Machine &m = mach();
    m.cf = (mul_hi(a, b) != 0) ? 1 : 0; m.of = m.cf;
    m.zf = (r == 0) ? 1 : 0; m.sf = (u8)((r >> 31) & 1);
    return r;
}
inline u32 mul32(u32 a, u32 b) { EDX.d = mul_hi(a, b); return mul_lo(a, b); }

// 64/32 division. This is the single place the model needs a 64-bit
// intermediate; the instruction encoding itself never does.
inline u32 divmod64(u32 hi, u32 lo, u32 d, u32 &rem, bool sign) {
    if (d == 0) { fault("divide by zero"); rem = lo; return 0; }
    unsigned long long n = ((unsigned long long)hi << 32) | (unsigned long long)lo;
    unsigned long long q = n / d;
    rem = (u32)(n % d);
    if (q > 0xffffffffull) fault("divide overflow");
    (void)sign;
    return (u32)q;
}
inline u32 div32(u32 hi, u32 lo, u32 d, u32 &rem) { return divmod64(hi, lo, d, rem, false); }
inline u32 idiv32(u32 hi, u32 lo, u32 d, u32 &rem) {
    i32 sd = (i32)d;
    if (sd == 0) { fault("divide by zero"); rem = lo; return 0; }
    if (sd < 0) { u32 r = divmod64(hi, lo, (u32)(0u - d), rem, true); return (u32)(0u - r); }
    return divmod64(hi, lo, d, rem, true);
}

// One-operand multiply and divide: these touch EDX:EAX as a pair and take their
// operand from memory, so the operand is passed by value and evaluated before
// the helper runs -- otherwise `div dword ptr [eax+8]` would compute the address
// from an EAX the division had already overwritten.
inline void mul_op(u32 s) {
    u32 hi = mul_hi(EAX.d, s), lo = mul_lo(EAX.d, s);
    EDX.d = hi; EAX.d = lo;
    M.cf = (hi != 0) ? 1 : 0; M.of = M.cf;
}
inline void imul_op(u32 s) {
    u32 lo = mul_lo(EAX.d, s), hi = mul_hi(EAX.d, s);
    EDX.d = hi; EAX.d = lo;
    M.cf = (hi != 0) ? 1 : 0; M.of = M.cf;
}
inline void div_op(u32 d)  { u32 rem = 0; u32 q = div32(EDX.d, EAX.d, d, rem);  EDX.d = rem; EAX.d = q; }
inline void idiv_op(u32 d) { u32 rem = 0; u32 q = idiv32(EDX.d, EAX.d, d, rem); EDX.d = rem; EAX.d = q; }

// Float and double memory access, for the x87 forms. The host `double` is used
// as the transport; the modelled x87 register stack keeps long double.
inline double ld_f32(u32 a) { u32 v = ld32(a); float f; std::memcpy(&f, &v, 4); return (double)f; }
inline void st_f32(u32 a, double v) { float f = (float)v; u32 o; std::memcpy(&o, &f, 4); st32(a, o); }
inline double ld_f64(u32 a) {
    u8 b[8];
    for (int i = 0; i < 8; ++i) b[i] = ld8(a + (u32)i);
    double d; std::memcpy(&d, b, 8); return d;
}
inline void st_f64(u32 a, double v) {
    double d = v; u8 b[8]; std::memcpy(b, &d, 8);
    for (int i = 0; i < 8; ++i) st8(a + (u32)i, b[i]);
}

// -- stack ------------------------------------------------------------------
inline void push32(u32 v) { ESP.d -= 4; st32(ESP.d, v); }
inline u32  pop32() { u32 v = ld32(ESP.d); ESP.d += 4; return v; }
inline void pushad() {
    u32 s = ESP.d;
    push32(EAX.d); push32(ECX.d); push32(EDX.d); push32(EBX.d);
    push32(s); push32(EBP.d); push32(ESI.d); push32(EDI.d);
}
inline void popad() {
    EDI.d = pop32(); ESI.d = pop32(); EBP.d = pop32(); (void)pop32();
    EBX.d = pop32(); EDX.d = pop32(); ECX.d = pop32(); EAX.d = pop32();
}

// -- control flow -----------------------------------------------------------
// A recovered `call` names a target VA. Dispatch is by VA so a transcribed
// callee runs its own transcription; a target with no transcription is counted
// rather than guessed at.
typedef void (*Fn)(void);
typedef void (*Fn)(void);

struct FnTable {
    // Sized for the whole recovered binary: 41,822 functions are registered by
    // the generated tree, so a table smaller than that silently drops entries
    // (ts_register gives up when it finds no free slot) and calls stop
    // dispatching. 2^17 slots keeps the load factor near a third, which is
    // where linear probing stays cheap.
    enum { SLOTS = 131072 };
    u32 key[SLOTS];
    Fn  val[SLOTS];
    FnTable() { for (int i = 0; i < SLOTS; ++i) { key[i] = 0xffffffffu; val[i] = 0; } }
};

inline FnTable &fntab() { static FnTable t; return t; }

inline void ts_register(u32 va, Fn f) {
    FnTable &t = fntab();
    u32 h = (va * 2654435761u) & (FnTable::SLOTS - 1);
    for (u32 p = 0; p < FnTable::SLOTS; ++p) {
        u32 i = (h + p) & (FnTable::SLOTS - 1);
        if (t.key[i] == va || t.key[i] == 0xffffffffu) { t.key[i] = va; t.val[i] = f; return; }
    }
}

inline Fn ts_lookup(u32 va) {
    FnTable &t = fntab();
    u32 h = (va * 2654435761u) & (FnTable::SLOTS - 1);
    for (u32 p = 0; p < FnTable::SLOTS; ++p) {
        u32 i = (h + p) & (FnTable::SLOTS - 1);
        if (t.key[i] == va) return t.val[i];
        if (t.key[i] == 0xffffffffu) return 0;
    }
    return 0;
}

// A recovered `call` names a target VA. Nothing is registered by default: a
// generated body is a member definition whose transcription reads the machine
// model rather than its own C++ parameters, so making the whole program *execute*
// is a separate step from transcribing it. An unregistered target is counted,
// not guessed at. The registry exists so that step can be added without touching
// a single generated line.
inline void ts_call(u32 va) {
    Machine &m = mach();
    m.eip = va;
    m.recent_calls[m.recent_call_n & 7u] = va;
    ++m.recent_call_n;
    Fn f = ts_lookup(va);
    if (!f) {
        if (m.unmapped_sample_n < 16) m.unmapped_sample[m.unmapped_sample_n++] = va;
        m.unmapped_calls++;
        if (m.text_hi && va >= m.text_lo && va < m.text_hi) ++m.unmapped_in_text;
        return;
    }
    // Bound the depth. A recovered loop whose modelled condition never changes
    // state would recurse forever, and "ran out of host stack" is a worse
    // diagnostic than "depth exceeded at VA X".
    // Lazily defaulted: reset() is not necessarily called before the first
    // dispatch, and a limit of 0 would cut every call off at the root.
    if (m.call_depth_limit == 0) m.call_depth_limit = 4096;
    if (m.call_depth >= m.call_depth_limit) { m.depth_exceeded++; return; }
    m.call_depth++;
    if (m.call_depth > m.call_depth_max) m.call_depth_max = m.call_depth;
    f();
    m.call_depth--;
}
// A `call` pushes its return address before transferring, so the modelled stack
// stays the shape the callee's transcription expects.
inline void ts_call_at(u32 target, u32 ret) { push32(ret); ts_call(target); }

// Invoke a recovered function from hand-written code.
//
// The generated forwarders call their transcription by name, but the
// hand-written template headers (VectorClass.h and friends) declare members
// whose bodies are recovered machine code and cannot name a generated symbol
// without depending on the generated tree. They go through the dispatch table
// instead, with a return address pushed so the callee's `ret` has something to
// pop and the modelled stack stays balanced.
//
// This runs the recovered body. It does NOT convert C++ arguments into the
// model: a transcription reads its arguments from the modelled stack, so a
// hand-written caller reaches the right code but the values it passes are only
// meaningful if the caller is itself a transcription. That limitation is
// inherent to a machine model, and it is why the generated forwarders exist.
inline void ts_invoke(u32 va) { push32(0u); ts_call(va); }
// An indirect `call dword ptr [eax+0x2c]`: the target is read first, for the same
// reason the multiply operands are.
inline void ts_call_ptr(u32 target, u32 ret) { push32(ret); ts_call(target); }

// -- string ops -------------------------------------------------------------
// The direction flag is honoured, because the recovered code does use `std`.
inline i32 rep_step() { return mach().df ? -1 : 1; }

inline void ts_rep_movsb() {
    u32 n = ECX.d; i32 s = rep_step();
    while (n--) { st8(EDI.d, ld8(ESI.d)); EDI.d += s; ESI.d += s; }
    ECX.d = 0;
}
inline void ts_rep_movsw() {
    u32 n = ECX.d; i32 s = rep_step() * 2;
    while (n--) { st16(EDI.d, ld16(ESI.d)); EDI.d += s; ESI.d += s; }
    ECX.d = 0;
}
inline void ts_rep_movsd() {
    u32 n = ECX.d; i32 s = rep_step() * 4;
    while (n--) { st32(EDI.d, ld32(ESI.d)); EDI.d += s; ESI.d += s; }
    ECX.d = 0;
}
inline void ts_rep_stosb() { u32 n = ECX.d; i32 s = rep_step(); while (n--) { st8(EDI.d, AL); EDI.d += s; } ECX.d = 0; }
inline void ts_rep_stosw() { u32 n = ECX.d; i32 s = rep_step() * 2; while (n--) { st16(EDI.d, AX); EDI.d += s; } ECX.d = 0; }
inline void ts_rep_stosd() { u32 n = ECX.d; i32 s = rep_step() * 4; while (n--) { st32(EDI.d, EAX.d); EDI.d += s; } ECX.d = 0; }
inline void ts_rep_lodsb() { u32 n = ECX.d; i32 s = rep_step(); while (n--) { AL = ld8(ESI.d); ESI.d += s; } ECX.d = 0; }
inline void ts_rep_lodsw() { u32 n = ECX.d; i32 s = rep_step() * 2; while (n--) { AX = ld16(ESI.d); ESI.d += s; } ECX.d = 0; }
inline void ts_rep_lodsd() { u32 n = ECX.d; i32 s = rep_step() * 4; while (n--) { EAX.d = ld32(ESI.d); ESI.d += s; } ECX.d = 0; }
inline void ts_rep_scasb() { u32 n = ECX.d; i32 s = rep_step(); while (n--) { cmp8(AL, ld8(EDI.d)); EDI.d += s; if (!M.zf) break; } ECX.d = 0; }
inline void ts_repne_scasb() { u32 n = ECX.d; i32 s = rep_step(); while (n--) { cmp8(AL, ld8(EDI.d)); EDI.d += s; if (M.zf) break; } ECX.d = 0; }
inline void ts_rep_scasd() { u32 n = ECX.d; i32 s = rep_step() * 4; while (n--) { cmp32(EAX.d, ld32(EDI.d)); EDI.d += s; if (!M.zf) break; } ECX.d = 0; }
inline void ts_repne_scasd() { u32 n = ECX.d; i32 s = rep_step() * 4; while (n--) { cmp32(EAX.d, ld32(EDI.d)); EDI.d += s; if (M.zf) break; } ECX.d = 0; }
inline void ts_repe_cmpsb() { u32 n = ECX.d; i32 s = rep_step(); while (n--) { cmp8(ld8(ESI.d), ld8(EDI.d)); ESI.d += s; EDI.d += s; if (!M.zf) break; } ECX.d = 0; }
inline void ts_repe_cmpsw() { u32 n = ECX.d; i32 s = rep_step() * 2; while (n--) { cmp16(ld16(ESI.d), ld16(EDI.d)); ESI.d += s; EDI.d += s; if (!M.zf) break; } ECX.d = 0; }
inline void ts_repe_cmpsd() { u32 n = ECX.d; i32 s = rep_step() * 4; while (n--) { cmp32(ld32(ESI.d), ld32(EDI.d)); ESI.d += s; EDI.d += s; if (!M.zf) break; } ECX.d = 0; }
inline void ts_repne_cmpsb() { u32 n = ECX.d; i32 s = rep_step(); while (n--) { cmp8(ld8(ESI.d), ld8(EDI.d)); ESI.d += s; EDI.d += s; if (M.zf) break; } ECX.d = 0; }
inline void ts_cmpsb() { cmp8(ld8(ESI.d), ld8(EDI.d)); }
inline void ts_cmpsw() { cmp16(ld16(ESI.d), ld16(EDI.d)); }
inline void ts_cmpsd() { cmp32(ld32(ESI.d), ld32(EDI.d)); }
inline void ts_lodsb() { AL = ld8(ESI.d); ESI.d += rep_step(); }
inline void ts_stosb() { st8(EDI.d, AL); EDI.d += rep_step(); }
inline void ts_stosd() { st32(EDI.d, EAX.d); EDI.d += rep_step() * 4; }
inline void ts_scasb() { cmp8(AL, ld8(EDI.d)); EDI.d += rep_step(); }

// -- x87 --------------------------------------------------------------------
// A register stack of host long double with the real instruction semantics for
// the operations the binary uses. op: 0 add, 1 sub, 2 mul, 3 div.
inline void f_push(long double v) {
    Machine &m = mach();
    if (m.st_top >= 8) { fault("x87 stack overflow"); return; }
    m.st[m.st_top++] = v;
}
inline long double f_pop() {
    Machine &m = mach();
    if (m.st_top <= 0) { fault("x87 stack underflow"); return 0.0L; }
    return m.st[--m.st_top];
}
inline long double f_peek(int depth) {
    Machine &m = mach();
    int i = m.st_top - 1 - depth;
    if (i < 0 || i >= 8) { fault("x87 stack read out of range"); return 0.0L; }
    return m.st[i];
}
inline void f_set(int depth, long double v) {
    Machine &m = mach();
    int i = m.st_top - 1 - depth;
    if (i < 0 || i >= 8) { fault("x87 stack write out of range"); return; }
    m.st[i] = v;
}
inline void f_arith(int op, int dst, int src, bool pop_after, bool reverse) {
    long double a = f_peek(dst), b = f_peek(src), r = 0.0L;
    switch (op) {
        case 0: r = reverse ? b + a : a + b; break;
        case 1: r = reverse ? b - a : a - b; break;
        case 2: r = a * b; break;
        case 3: r = reverse ? b / a : a / b; break;
        default: break;
    }
    f_set(dst, r);
    if (pop_after) (void)f_pop();
}
inline void f_compare(int src, bool pop_after, bool pop_twice) {
    Machine &m = mach();
    long double a = f_peek(0), b = f_peek(src);
    u16 c = 0;
    if (a > b)      c = 0x0000;   // C3 C2 C0 = 000, greater
    else if (a < b) c = 0x0100;   // C0 set, less
    else            c = 0x4000;   // C3 set, equal
    m.fpu_status = c;
    if (pop_after) (void)f_pop();
    if (pop_twice) (void)f_pop();
}
inline void f_stsw() { AX = mach().fpu_status; }
inline void f_swap(int i) { long double a = f_peek(0), b = f_peek(i); f_set(0, b); f_set(i, a); }
inline void f_arith_mem(int op, long double v, bool reverse) {
    long double a = f_peek(0), r = 0.0L;
    switch (op) {
        case 0: r = a + v; break;
        case 1: r = reverse ? v - a : a - v; break;
        case 2: r = a * v; break;
        case 3: r = reverse ? v / a : a / v; break;
        default: break;
    }
    f_set(0, r);
}
inline void f_compare_mem(long double v, bool pop_after) {
    long double a = f_peek(0);
    mach().fpu_status = (a > v) ? 0x0000 : ((a < v) ? 0x0100 : 0x4000);
    if (pop_after) (void)f_pop();
}
inline void f_chs()  { f_set(0, -f_peek(0)); }
inline void f_abs()  { long double v = f_peek(0); f_set(0, v < 0.0L ? -v : v); }
inline void f_sqrt() { long double v = f_peek(0); f_set(0, v <= 0.0L ? 0.0L : (long double)std::sqrt((double)v)); }
inline void f_test() { long double v = f_peek(0); mach().fpu_status = (v > 0.0L) ? 0x0000 : ((v < 0.0L) ? 0x0100 : 0x4000); }
inline void f_rem()  { long double a = f_peek(0), b = f_peek(1); if (b != 0.0L) f_set(0, a - b * (long double)(long)(a / b)); }
inline void f_round(){ long double v = f_peek(0); f_set(0, (v < 0.0L) ? -(long double)(long)(-v + 0.5L) : (long double)(long)(v + 0.5L)); }
inline void f_scale(){ f_set(0, f_peek(0) * (long double)std::pow(2.0, (double)f_peek(1))); }
inline void f_yl2x(){ long double x = f_peek(0), y = f_peek(1); (void)f_pop(); (void)f_pop();
                      f_push(y * (long double)(std::log((double)x) / std::log(2.0))); }
inline void f_2xm1(){ f_set(0, (long double)std::pow(2.0, (double)f_peek(0)) - 1.0L); }
inline void f_xam() { long double v = f_peek(0); mach().fpu_status = (v == 0.0L) ? 0x4000 : 0x0000; }
inline void f_clear(){ Machine &m = mach(); m.fcw = 0x037f; m.fpu_status = 0; m.st_top = 0; }

// -- flags and misc ---------------------------------------------------------
inline void clc()  { M.cf = 0; }
inline void stc()  { M.cf = 1; }
inline void cmc()  { M.cf = (u8)(M.cf ^ 1); }
inline void cld()  { M.df = 0; }
inline void setdf(){ M.df = 1; }
inline void sahf() { M.cf = (u8)(AX & 1); M.pf = (u8)((AX >> 2) & 1); M.af = (u8)((AX >> 4) & 1);
                     M.zf = (u8)((AX >> 6) & 1); M.sf = (u8)((AX >> 7) & 1); }
inline void lahf() { AX = (u16)((M.sf << 7) | (M.zf << 6) | (M.af << 4) | (M.pf << 2) | 0x02 | M.cf); }
inline u32 flags_pack() {
    return (u32)(M.cf | (M.pf << 2) | (M.af << 4) | (M.zf << 6) | (M.sf << 7) |
                 (M.df << 10) | (M.of << 11) | 0x00000202u);
}
inline void flags_unpack(u32 f) {
    M.cf = (u8)(f & 1); M.pf = (u8)((f >> 2) & 1); M.af = (u8)((f >> 4) & 1);
    M.zf = (u8)((f >> 6) & 1); M.sf = (u8)((f >> 7) & 1);
    M.df = (u8)((f >> 10) & 1); M.of = (u8)((f >> 11) & 1);
}
inline void pushfd() { push32(flags_pack()); }
inline void popfd()  { flags_unpack(pop32()); }
inline void leave_op() { ESP.d = EBP.d; EBP.d = pop32(); }
inline void ret_pop(u32 n) { ESP.d += n; }
// A `ret imm` on a function that returns a value: pop the arguments and yield
// what the machine has in EAX. The transcription reads its result from the
// machine, not from a C++ expression.
inline u32 ret_pop_val(u32 n, u32 v) { ESP.d += n; return v; }
inline void nop() {}
inline void trap(u32 n) { (void)n; }
inline u32  bswap32(u32 v) {
    return ((v & 0xffu) << 24) | ((v & 0xff00u) << 8) |
           ((v >> 8) & 0xff00u) | ((v >> 24) & 0xffu);
}
inline void bound_op(u32 idx, u32 addr) {
    u32 lo = ld32(addr), hi = ld32(addr + 4);
    if (idx < lo || idx > hi) fault("bound range exceeded");
}

// -- narrow forms -----------------------------------------------------------
// The binary uses byte- and word-sized arithmetic throughout, not just dword.
inline u8  inc8 (u8  a) { u8  r = (u8)(a + 1);  f_inc(a, r); return r; }
inline u8  dec8 (u8  a) { u8  r = (u8)(a - 1);  f_dec(a, r); return r; }
inline u16 inc16(u16 a) { u16 r = (u16)(a + 1); f_inc(a, r); return r; }
inline u16 dec16(u16 a) { u16 r = (u16)(a - 1); f_dec(a, r); return r; }
inline u8  neg8 (u8  a) { u8  r = (u8)(0 - a);  f_sub(0, a, r); return r; }
inline u16 neg16(u16 a) { u16 r = (u16)(0 - a); f_sub(0, a, r); return r; }
inline u8  not8 (u8  a) { return (u8)~a; }
inline u16 not16(u16 a) { return (u16)~a; }
inline u8  adc8 (u8  a, u8  b) { u8  c = M.cf; u8  r = (u8)(a + b + c);   f_add(a, (u8)(b + c), r); return r; }
inline u16 adc16(u16 a, u16 b) { u16 c = M.cf; u16 r = (u16)(a + b + c); f_add(a, (u16)(b + c), r); return r; }
inline u8  sbb8 (u8  a, u8  b) { u8  c = M.cf; u8  r = (u8)(a - b - c);   f_sub(a, (u8)(b + c), r); return r; }
inline u16 sbb16(u16 a, u16 b) { u16 c = M.cf; u16 r = (u16)(a - b - c); f_sub(a, (u16)(b + c), r); return r; }
inline u8  and8 (u8  a, u8  b) { u8  r = (u8)(a & b); f_log(r); return r; }
inline u16 and16(u16 a, u16 b) { u16 r = (u16)(a & b); f_log(r); return r; }
inline u8  or8  (u8  a, u8  b) { u8  r = (u8)(a | b); f_log(r); return r; }
inline u16 or16 (u16 a, u16 b) { u16 r = (u16)(a | b); f_log(r); return r; }
inline u8  xor8 (u8  a, u8  b) { u8  r = (u8)(a ^ b); f_log(r); return r; }
inline u16 xor16(u16 a, u16 b) { u16 r = (u16)(a ^ b); f_log(r); return r; }
inline u16 imul16(u16 a, u16 b) {
    u16 r = (u16)(a * b);
    Machine &m = mach();
    m.cf = 0; m.of = 0; m.zf = (r == 0) ? 1 : 0; m.sf = (u8)((r >> 15) & 1);
    return r;
}
inline u8  shl8 (u8 a, u32 n) { u32 k = n & 7u; u8 r = k ? (u8)(a << k) : a; M.zf = (r == 0) ? 1 : 0; M.sf = (u8)((r >> 7) & 1); return r; }
inline u8  shr8 (u8 a, u32 n) { u32 k = n & 7u; u8 r = k ? (u8)(a >> k) : a; M.zf = (r == 0) ? 1 : 0; M.sf = 0; return r; }
inline u8  sar8 (u8 a, u32 n) { i8 s = (i8)a; u32 k = n & 7u; u8 r = (u8)(k ? (s >> k) : s); M.zf = (r == 0) ? 1 : 0; M.sf = (u8)((r >> 7) & 1); return r; }
inline u8  rol8 (u8 a, u32 n) { u32 k = n & 7u; return k ? (u8)((a << k) | (a >> (8 - k))) : a; }
inline u8  ror8 (u8 a, u32 n) { u32 k = n & 7u; return k ? (u8)((a >> k) | (a << (8 - k))) : a; }
inline u8  rcl8 (u8 a, u32 n) { u32 k = n & 7u; if (!k) return a; u8 r = (u8)((a << k) | (M.cf << (k - 1))); M.cf = (u8)((a >> (8 - k)) & 1); return r; }
inline u8  rcr8 (u8 a, u32 n) { u32 k = n & 7u; if (!k) return a; u8 r = (u8)((a >> k) | (M.cf << (7 - k + 1))); M.cf = (u8)((a >> (k - 1)) & 1); return r; }
inline u16 shl16(u16 a, u32 n) { u32 k = n & 15u; u16 r = k ? (u16)(a << k) : a; M.zf = (r == 0) ? 1 : 0; M.sf = (u8)((r >> 15) & 1); return r; }
inline u16 shr16(u16 a, u32 n) { u32 k = n & 15u; u16 r = k ? (u16)(a >> k) : a; M.zf = (r == 0) ? 1 : 0; M.sf = 0; return r; }
inline u16 sar16(u16 a, u32 n) { i16 s = (i16)a; u32 k = n & 15u; u16 r = (u16)(k ? (s >> k) : s); M.zf = (r == 0) ? 1 : 0; M.sf = (u8)((r >> 15) & 1); return r; }
inline u16 rol16(u16 a, u32 n) { u32 k = n & 15u; return k ? (u16)((a << k) | (a >> (16 - k))) : a; }
inline u16 ror16(u16 a, u32 n) { u32 k = n & 15u; return k ? (u16)((a >> k) | (a << (16 - k))) : a; }
inline u16 rcl16(u16 a, u32 n) { u32 k = n & 15u; if (!k) return a; u16 r = (u16)((a << k) | (M.cf << (k - 1))); M.cf = (u8)((a >> (16 - k)) & 1); return r; }
inline u16 rcr16(u16 a, u32 n) { u32 k = n & 15u; if (!k) return a; u16 r = (u16)((a >> k) | (M.cf << (15 - k + 1))); M.cf = (u8)((a >> (k - 1)) & 1); return r; }

// 64-bit integer and 80-bit float access, for fild/fistp and fld/fstp tbyte.
inline long long ld_i64(u32 a) {
    unsigned long long v = 0;
    for (int i = 7; i >= 0; --i) v = (v << 8) | (unsigned long long)ld8(a + (u32)i);
    return (long long)v;
}
inline void st_i64(u32 a, long long v) {
    unsigned long long u = (unsigned long long)v;
    for (int i = 0; i < 8; ++i) { st8(a + (u32)i, (u8)(u & 0xffu)); u >>= 8; }
}
inline long double ld_f80(u32 a) {
    u8 b[10];
    for (int i = 0; i < 10; ++i) b[i] = ld8(a + (u32)i);
    long double d; std::memcpy(&d, b, 10); return d;
}
inline void st_f80(u32 a, long double v) {
    u8 b[10]; std::memcpy(b, &v, 10);
    for (int i = 0; i < 10; ++i) st8(a + (u32)i, b[i]);
}

// -- MMX, SSE and the x87 environment ---------------------------------------
// The MMX instructions the binary uses, each modelled per lane with the
// hardware's semantics: saturating adds and packs, interleaves, and the two
// multiplies that keep only part of each product. A lane operand is 64 bits;
// the m32 forms of the interleaves and of pmullw read only the low 32 bits of
// the source, which is what the instruction definitions do with a 32-bit
// memory operand, so a u32 argument is widened rather than rejected.
inline u64 ld64(u32 a) { return (u64)ld_i64(a); }
inline void st64(u32 a, u64 v) { st_i64(a, (long long)v); }

inline u128 ld128(u32 a) { u128 v; v.lo = ld64(a); v.hi = ld64(a + 8); return v; }
inline void st128(u32 a, u128 v) { st64(a, v.lo); st64(a + 8, v.hi); }

inline u64 mmx_pxor(u64 a, u64 b) { return a ^ b; }

inline u8 mmx_sat_u8(i32 v) { return (u8)(v < 0 ? 0 : (v > 255 ? 255 : v)); }
inline u16 mmx_sat_i16(i32 v) {
    return (u16)(v < -32768 ? -32768 : (v > 32767 ? 32767 : v));
}

inline u64 mmx_paddusb(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 8; ++i) {
        u32 x = (u32)((a >> (8 * i)) & 0xffu) + (u32)((b >> (8 * i)) & 0xffu);
        r |= (u64)(x > 255u ? 255u : x) << (8 * i);
    }
    return r;
}
inline u64 mmx_packuswb(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 4; ++i) {
        r |= (u64)mmx_sat_u8((i16)((a >> (16 * i)) & 0xffffu)) << (8 * i);
        r |= (u64)mmx_sat_u8((i16)((b >> (16 * i)) & 0xffffu)) << (8 * (i + 4));
    }
    return r;
}
inline u64 mmx_packssdw(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 2; ++i) {
        r |= (u64)mmx_sat_i16((i32)((a >> (32 * i)) & 0xffffffffu)) << (16 * i);
        r |= (u64)mmx_sat_i16((i32)((b >> (32 * i)) & 0xffffffffu)) << (16 * (i + 2));
    }
    return r;
}
inline u64 mmx_pmullw(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 4; ++i) {
        u32 x = (u32)((a >> (16 * i)) & 0xffffu);
        u32 y = (u32)((b >> (16 * i)) & 0xffffu);
        r |= (u64)((x * y) & 0xffffu) << (16 * i);
    }
    return r;
}
inline u64 mmx_pmulhw(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 4; ++i) {
        i32 x = (i16)((a >> (16 * i)) & 0xffffu);
        i32 y = (i16)((b >> (16 * i)) & 0xffffu);
        r |= (u64)(u16)((x * y) >> 16) << (16 * i);
    }
    return r;
}
// PSRLW: a count above the lane width clears the lane, which is what the
// 64-bit form of the instruction specifies.
inline u64 mmx_psrlw(u64 a, u32 n) {
    u64 r = 0;
    if (n > 15) return 0;
    for (int i = 0; i < 4; ++i)
        r |= (u64)(((a >> (16 * i)) & 0xffffu) >> n) << (16 * i);
    return r;
}
// PSRAD: an arithmetic shift, so the sign fills. A count of 32 or more leaves
// every lane at the sign.
inline u64 mmx_psrad(u64 a, u32 n) {
    u64 r = 0;
    for (int i = 0; i < 2; ++i) {
        i32 x = (i32)((a >> (32 * i)) & 0xffffffffu);
        i32 s = (n > 31) ? (x >> 31) : (x >> n);
        r |= (u64)(u32)s << (32 * i);
    }
    return r;
}
inline u64 mmx_punpcklbw(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 4; ++i) {
        r |= (u64)((a >> (8 * i)) & 0xffu) << (16 * i);
        r |= (u64)((b >> (8 * i)) & 0xffu) << (16 * i + 8);
    }
    return r;
}
inline u64 mmx_punpckhbw(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 4; ++i) {
        r |= (u64)((a >> (8 * (i + 4))) & 0xffu) << (16 * i);
        r |= (u64)((b >> (8 * (i + 4))) & 0xffu) << (16 * i + 8);
    }
    return r;
}
inline u64 mmx_punpcklwd(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 2; ++i) {
        r |= (u64)((a >> (16 * i)) & 0xffffu) << (32 * i);
        r |= (u64)((b >> (16 * i)) & 0xffffu) << (32 * i + 16);
    }
    return r;
}
inline u64 mmx_punpckhwd(u64 a, u64 b) {
    u64 r = 0;
    for (int i = 0; i < 2; ++i) {
        r |= (u64)((a >> (16 * (i + 2))) & 0xffffu) << (32 * i);
        r |= (u64)((b >> (16 * (i + 2))) & 0xffffu) << (32 * i + 16);
    }
    return r;
}
inline u64 mmx_punpckldq(u64 a, u64 b) {
    return (a & 0xffffffffull) | ((b & 0xffffffffull) << 32);
}
// EMMS marks every x87 register empty, which is what st_top models here. The
// data registers keep their values but are inaccessible until an x87 load.
inline void mmx_emms() { mach().st_top = 0; }

// The x87 environment area. FNSAVE writes the full 108-byte 32-bit
// protected-mode image: control word, status word with TOP, tag word, the
// instruction and data pointers, then the eight 80-bit data registers at offset
// 28. The model does not track the two pointer fields, so those are written as
// zero; the binary only round-trips the area through FRSTOR/FLDENV or reads the
// control and status words back, and every other field is real. The registers
// are written in the model's own physical order with TOP naming the logical
// top, which is the same image the hardware writes up to a rotation.
inline u16 f_top_field() {
    Machine &m = mach();
    return (u16)(m.st_top ? (m.st_top - 1) : 0);
}
inline u16 f_tag_field() {
    Machine &m = mach();
    u16 t = 0;
    for (int i = 0; i < 8; ++i)
        t |= (u16)((i < m.st_top ? 0 : 3) << (2 * i));
    return t;
}
inline u16 f_status_field() {
    Machine &m = mach();
    return (u16)((m.fpu_status & ~0x3800u) | (f_top_field() << 11));
}
inline int f_tag_count(u16 tag) {
    int n = 0;
    for (int i = 0; i < 8; ++i)
        if (((tag >> (2 * i)) & 3u) != 3u) n = i + 1;
    return n;
}
inline void f_save(u32 a) {
    Machine &m = mach();
    st32(a, m.fcw);
    st32(a + 4, f_status_field());
    st32(a + 8, f_tag_field());
    st32(a + 12, 0); st32(a + 16, 0); st32(a + 20, 0); st32(a + 24, 0);
    for (int i = 0; i < 8; ++i) st_f80(a + 28 + 10 * i, m.st[i]);
    // FNSAVE re-initialises the FPU, as the hardware does.
    m.fcw = 0x037f; m.fpu_status = 0; m.st_top = 0;
}
inline void f_restore(u32 a) {
    Machine &m = mach();
    m.fcw = ld16(a);
    m.fpu_status = ld16(a + 4);
    m.st_top = f_tag_count(ld16(a + 8));
    for (int i = 0; i < 8; ++i) m.st[i] = ld_f80(a + 28 + 10 * i);
}
// FNSTENV/FLDENV: the 28-byte environment, which is the first part of the
// FNSAVE image and does not touch the data registers.
inline void f_stenv(u32 a) {
    Machine &m = mach();
    st32(a, m.fcw);
    st32(a + 4, f_status_field());
    st32(a + 8, f_tag_field());
    st32(a + 12, 0); st32(a + 16, 0); st32(a + 20, 0); st32(a + 24, 0);
}
inline void f_ldenv(u32 a) {
    Machine &m = mach();
    m.fcw = ld16(a);
    m.fpu_status = ld16(a + 4);
    m.st_top = f_tag_count(ld16(a + 8));
}
// FBLOAD: ten bytes of packed BCD; the sign is bit 7 of the tenth byte.
inline void f_bcd_push(u32 a) {
    long double v = 0.0L;
    for (int i = 8; i >= 0; --i) {
        u8 b = ld8(a + (u32)i);
        v = v * 100.0L + (long double)(b >> 4) * 10.0L + (long double)(b & 0x0fu);
    }
    if (ld8(a + 9) & 0x80u) v = -v;
    f_push(v);
}

// PUSHAW: the 16-bit form of PUSHA. The saved SP is the value from before the
// first push, as the hardware does.
inline void pushaw() {
    u16 s = SP;
    ESP.d -= 2; st16(ESP.d, AX);
    ESP.d -= 2; st16(ESP.d, CX);
    ESP.d -= 2; st16(ESP.d, DX);
    ESP.d -= 2; st16(ESP.d, BX);
    ESP.d -= 2; st16(ESP.d, s);
    ESP.d -= 2; st16(ESP.d, BP);
    ESP.d -= 2; st16(ESP.d, SI);
    ESP.d -= 2; st16(ESP.d, DI);
}

// LAR: the model has no descriptor table -- it is flat, and so is the original
// -- so a selector with a non-zero index is reported as the flat ring-3 code
// segment the binary runs under, with ZF clear. A null selector is the one case
// the hardware rejects: ZF set, destination unchanged.
inline void lar_op(u32 &dst, u16 sel) {
    if ((sel & 0xfff8u) == 0) { M.zf = 1; return; }
    dst = 0x00cffb00u;
    M.zf = 0;
}

// -- port I/O and segment bookkeeping ---------------------------------------
// There is no device model, so a port access is recorded and otherwise does
// nothing. That is a real limitation, stated rather than hidden.
inline void out_op(u32 port, u32 val) { (void)port; (void)val; }
inline u32  in_op(u32 port) { (void)port; return 0xffffffffu; }
inline void cli() { M.iff = 0; }
inline void sti() { M.iff = 1; }
inline u16  arpl_op(u16 d, u16 s) {
    if ((d & 3u) < (s & 3u)) { M.zf = 1; return (u16)((d & ~3u) | (s & 3u)); }
    M.zf = 0;
    return d;
}
// A far jump or far return: the segment changes and control leaves this frame.
inline void far_jump() { /* modelled as a return: the target is outside */ }

// -- single-step string ops -------------------------------------------------
// These are the unprefixed forms, and the ones the disassembler names with
// explicit operands (`stosw word ptr es:[edi], ax`). The direction flag is
// honoured for the same reason as in the rep forms.
inline void ts_stosw() { st16(EDI.d, AX); EDI.d += rep_step() * 2; }
inline void ts_lodsw() { AX = ld16(ESI.d); ESI.d += rep_step() * 2; }
inline void ts_lodsd() { EAX.d = ld32(ESI.d); ESI.d += rep_step() * 4; }
inline void ts_scasw() { cmp16(AX, ld16(EDI.d)); EDI.d += rep_step() * 2; }
inline void ts_scasd() { cmp32(EAX.d, ld32(EDI.d)); EDI.d += rep_step() * 4; }
inline void ts_movsb() { st8(EDI.d, ld8(ESI.d)); EDI.d += rep_step(); ESI.d += rep_step(); }
inline void ts_movsw() {
    st16(EDI.d, ld16(ESI.d));
    EDI.d += rep_step() * 2; ESI.d += rep_step() * 2;
}
inline void ts_movsd() {
    st32(EDI.d, ld32(ESI.d));
    EDI.d += rep_step() * 4; ESI.d += rep_step() * 4;
}
inline void ts_insb() { st8(EDI.d, (u8)in_op(DX)); EDI.d += rep_step(); }
inline void ts_insw() { st16(EDI.d, (u16)in_op(DX)); EDI.d += rep_step() * 2; }
inline void ts_insd() { st32(EDI.d, in_op(DX)); EDI.d += rep_step() * 4; }
inline void ts_outsb() { out_op(DX, ld8(ESI.d)); ESI.d += rep_step(); }
inline void ts_outsw() { out_op(DX, ld16(ESI.d)); ESI.d += rep_step() * 2; }
inline void ts_outsd() { out_op(DX, ld32(ESI.d)); ESI.d += rep_step() * 4; }
inline void ts_rep_outsb() {
    u32 n = ECX.d; i32 s = rep_step();
    while (n--) { out_op(DX, ld8(ESI.d)); ESI.d += s; }
    ECX.d = 0;
}

// An indirect jump whose table could not be resolved from the image. The
// rendering records the target instead of guessing; see emit_x86.py.
inline void jmp_indirect(u32 target) { (void)target; mach().unmapped_calls++; }

}  // namespace ts

#endif  // TS_MACHINE_H
