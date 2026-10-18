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

// Force the per-instruction helpers inline. At -O2 GCC's inliner declines them
// at many of the thousands of generated call sites, leaving a real call per
// arithmetic instruction; the 4B profile put ~10% in five flag helpers that are
// a dozen instructions each. This is a code-generation hint only.
#if defined(__GNUC__)
#define TS_HOT_INLINE inline __attribute__((always_inline))
#else
#define TS_HOT_INLINE inline
#endif

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
    // 64-bit: on the -m32 host build `unsigned long` is 32 bits, so a budget
    // above 2^32 never fired and long runs reported a wrapped step count.
    unsigned long long steps;
    unsigned long long step_limit;
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
    unsigned long long fault_steps;
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
    // The 16-slot ring above fills with whatever comes first, and in practice that
    // is non-.text garbage (a stack value used as a function pointer) long before
    // the in-.text defect you are chasing. Keep the in-.text ones separately so a
    // report can name them: "2 unmapped inside .text" is not actionable, "0x68d781
    // and 0x5dcd40" is.
    u32 unmapped_text_sample[16];
    unsigned unmapped_text_sample_n;
    // The CALL SITE of each in-.text unmapped target. The target alone says the
    // stack is wrong; the caller says WHICH call pushed the bad pointer.
    u32 unmapped_text_from[16];
    // Calls whose target is OUTSIDE .text and which no registered stub answers.
    // These are the loudest defect the model can report: the game asked to run
    // something that is not code at all, so a value reached a call instruction
    // that was never a function pointer. `0x11` is the SetCooperativeLevel flags
    // argument arriving in a slot that held a vtable; `0x6a4e08` is a data table
    // being walked as if it held function pointers. Recording the CALL SITE turns
    // "the game is spinning on garbage" into a line number to read.
    // Write watchpoint: the first non-zero byte store to watch_va records the
    // EIP that made it. Lives in Machine (not at namespace scope) because
    // namespace-scope `inline` variables need C++17 and this tree is C++03.
    u32 watch_va;
    u32 watch_zero;      // record zero stores too (who CLEARED this field)
    u32 watch_hits;
    u32 watch_from[8];
    u32 watch_val[8];
    // TS_WATCH_RING=addr[,addr...] -- the LAST few non-zero byte stores to each
    // address, with the EIP that made them. The watch above keeps the FIRST
    // eight, which is the constructor; when an object's vptr is corrupt the
    // question is what wrote it after the constructor, and only a ring of the
    // most recent stores answers that.
    enum { WATCH_RING_ADDRS = 4, WATCH_RING_HITS = 8 };
    u32 watch_ring_va[WATCH_RING_ADDRS];
    unsigned watch_ring_n;
    u32 watch_ring_eip[WATCH_RING_ADDRS][WATCH_RING_HITS];
    u8  watch_ring_val[WATCH_RING_ADDRS][WATCH_RING_HITS];
    unsigned long long watch_ring_steps[WATCH_RING_ADDRS][WATCH_RING_HITS];
    unsigned watch_ring_count[WATCH_RING_ADDRS];
    // TS_WRITE_GUARD=eip,ptr_addr -- at that instruction, check every byte store
    // against the heap block whose user pointer is stored at ptr_addr (its
    // requested size is the heap header's size field). The first store outside
    // that block is an out-of-bounds write, and naming it beats waiting for the
    // corrupted object to be called millions of instructions later.
    u32 write_guard_eip;
    u32 write_guard_ptr_addr;
    u32 write_guard_stop;
    u32 write_guard_bad;
    u32 write_guard_addr;
    u32 write_guard_base;
    u32 write_guard_size;
    unsigned long long write_guard_step;
    u32 write_guard_esp, write_guard_eax, write_guard_ecx, write_guard_edx;
    // STACK BALANCE at stub calls. A __stdcall stub owns its arguments: it pops
    // the return address AND them, so ESP after the stub must equal ESP before the
    // call pushed anything. Any stub whose row under- or over-states its argument
    // count breaks that, and the damage is not local -- every later argument in
    // the caller is read from the wrong slot. That is exactly the shape of the
    // defect that has the game calling ExitProcess(0x6844df) for 1 and passing
    // 0x00a00201 as a MessageBoxA type.
    // Trace one address: when EIP reaches trace_va, record ESP/EAX/ECX so a
    // question like "is ESP right at this push?" is answered by the machine
    // rather than by counting pushes in a disassembly by hand. insn() sees every
    // instruction, so this costs nothing when trace_va is 0.
    u32 trace_va;
    u32 trace_n;
    u32 trace_esp[8];
    u32 trace_eax[8];
    u32 trace_ecx[8];
    u32 trace_edx[8];
    // Which CALLEE leaves ESP wrong. A cdecl call legitimately moves ESP by
    // -4*nargs, so a non-zero delta is not automatically a bug; the point is to
    // see which callees produce it and with what shape. The atexit walk calls
    // 3,311 constructors as `call *%eax` with no arguments, so every one of them
    // must leave ESP exactly where it started -- 27 that do not is the leak.
    u32 leak_va[64];
    int leak_delta[64];
    unsigned long leak_count[64];
    unsigned leak_n;
    u32 trace_esi[8];
    u32 trace_edi[8];
    // The LAST few executions of trace_va, with the first stack argument read
    // from [esp+4]. A render entry called every frame has thousands of hits;
    // the first eight are startup, and the one that matters is the last one
    // before a stop. The argument is what a caller passed (a redraw flag).
    u32 trace_ring_esp[8];
    u32 trace_ring_arg[8];
    u32 trace_ring_eax[8];
    u32 trace_ring_count;
    unsigned long long trace_ring_step[8];
    // Multi-address probe (TS_PROBE=0xADDR,...). trace_va answers one address;
    // a creation path is a chain of decisions at different addresses (section
    // count, first token, owner lookup, type lookup, placement) and each needs
    // the registers *after* a different call. Eight addresses x eight hits,
    // with a short printable snapshot of the string EAX and ECX point at, so a
    // token can be read without a second 20-minute run.
    static const unsigned PROBE_MAX = 16;
    static const unsigned PROBE_HITS = 8;
    static const unsigned PROBE_STR = 24;
    struct ProbeHit {
        unsigned long long step;
        u32 esp, ebp, eax, ebx, ecx, edx, esi, edi;
        u32 a0, a1;          // [esp+4], [esp+8]: the first two stack arguments
        // TS_PROBE_DEREF=1 -- the two dwords a0 points at. A probe at a
        // transform's entry sees the *address* of the coordinate pair in a0,
        // and the pair itself is what decides the answer; reading it here
        // saves a second run and a stack-address guess.
        u32 d0, d1;
        // [esp]: the return address, i.e. the call site. A probe on a function
        // with many callers (a command dispatch) needs to name which one.
        u32 ret;
        char eax_str[PROBE_STR];
        char ecx_str[PROBE_STR];
    };
    u32 probe_va[PROBE_MAX];
    u32 probe_n;
    u32 probe_deref;
    u32 probe_hits[PROBE_MAX];
    ProbeHit probe_rec[PROBE_MAX][PROBE_HITS];
    // 4096-bit Bloom filter over the probe addresses. insn() runs 500 billion
    // times; a linear scan of 16 addresses per instruction would cost more than
    // the run. One multiply/shift/test rejects all but ~0.4% of instructions.
    u32 probe_filter[128];
    // Optional hook run at a probe hit, after the registers are recorded. The
    // caller saves/restores the machine state around it; it exists so a
    // diagnostic can query a live object (the scenario INI at mission-init
    // entry) that does not survive to the end of the run.
    void (*probe_hook)(u32 va);
    unsigned long stub_dispatches;
    // The argument count the most recent stub popped, recorded by stub_done in
    // Win32Stubs.h. A __stdcall stub owns its arguments, so a correct call ends
    // at esp_before + 4*nargs -- not at esp_before. Without this field the
    // tracker compared every stub to the caller's ESP and reported 111,682
    // correct calls as leaks, burying the real signal.
    u32 stub_last_nargs;
    // The call depth stub_done ran at. A recovered callee that calls a stub
    // somewhere inside itself also changes stub_dispatches, but IT must still
    // return to esp_before; only a stub dispatched DIRECTLY at the call's own
    // depth owns arguments. Without the depth, 54,527 recovered callers were
    // still misreported.
    u32 stub_last_depth;
    unsigned long stack_imbalances;
    u32 stack_imbalance_n;
    u32 stack_imbalance_site[16];
    int stack_imbalance_delta[16];
    u32 bad_call_from[16];
    u32 bad_call_to[16];
    unsigned bad_call_n;
    unsigned bad_calls;
    // .text bounds, set by the caller. They let an unmapped call be classified:
    // outside .text it is an import (expected -- the model has no Windows),
    // inside .text it is a recovered function nothing registered (a defect).
    u32 text_lo, text_hi;
    // Coverage maps, owned by the caller (build/boot_driver.cpp). One bit per
    // byte of .text: insn() sets the executed bit, and ts_call() sets the
    // entered bit for every registered target it dispatches. A null map
    // disables the marking, so the model's behaviour is unchanged; the cost is
    // one shift and one OR per instruction.
    u8 *exec_map; u32 exec_map_lo, exec_map_hi;
    u8 *func_map; u32 func_map_lo, func_map_hi;
    // Every unregistered call outside .text, split by where its target points.
    // "below 0x1000" is a null or near-null pointer; ".rdata" is data being
    // called; "elsewhere" is a stack or heap value used as a function pointer.
    unsigned long unmapped_below_1000;
    unsigned long unmapped_in_rdata;
    unsigned long unmapped_elsewhere;
    // An unregistered target inside .text is one of two very different things,
    // and one .text range cannot tell them apart: inside a recovered span it is a
    // recovered function nothing registered (a defect in this tree), and outside
    // every span it is a call into bytes the disassembly never recovered (a gap
    // in the recovery, reported by out/text_data.json). The recovered spans are
    // therefore supplied by the generated registration unit and the two are
    // counted separately.
    unsigned long unmapped_in_text;
    unsigned long unmapped_in_gap;
    // First-N unmapped calls with the register state at the call site. The
    // 16-slot samples above are target-only, and a flood of garbage (7.7M calls
    // on the 479B mission-load run) buries the first one. The first calls are
    // the only ones that say what went wrong: by the time the flood is running
    // the state is already lost. `kind` is 0 for a `call` and 1 for a tail
    // transfer. `steps` places the event in the run so a later one can be
    // bisected to.
    enum { UNMAPPED_RECS = 64 };
    struct UnmappedRec {
        u32 to, from, kind, depth;
        unsigned long long steps;
        u32 esp, ebp, eax, ebx, ecx, edx, esi, edi;
    };
    UnmappedRec unmapped_rec[UNMAPPED_RECS];
    unsigned unmapped_rec_n;
    // In-.text unmapped calls carry the same register state: that is the case
    // where the target is real code and the pointer chain to it is the defect.
    struct TextUnmappedRec {
        u32 to, from, depth;
        unsigned long long steps;
        u32 esp, ebp, eax, ecx, edx, esi, edi;
    };
    TextUnmappedRec unmapped_text_rec[16];
    unsigned unmapped_text_rec_n;
    // TS_STOP_UNMAPPED=1 halts at the first unregistered target with the
    // machine intact, so a long run ends where the defect starts instead of
    // where the flood exhausts the page table.
    u32 stop_on_unmapped;
    // TS_ESP_GUARD=1 checks ESP at every instruction against the game's stack
    // range and records the instruction that left it. A wild ESP turns every
    // later push/ret into a page allocation and an unregistered call; the
    // interesting event is the instruction that moved it, not the flood.
    u32 esp_guard;
    u32 esp_guard_stop;
    u32 esp_prev;
    struct EspRec {
        u32 prev_eip, eip, esp, ebp, eax, ecx, edx;
        unsigned long long steps;
    };
    EspRec esp_bad[8];
    unsigned esp_bad_n;
    // Why the page table refused an insertion. `fault_addr` is the address whose
    // page was demanded; the rest is the table state at that moment, which
    // separates a wild address from a host allocation failure.
    u32 fault_addr;
    u32 fault_page;
    u32 fault_used;
    u32 fault_cap;
    // A tail transfer in flight: the VA a transcribed span continues at after
    // it returns. A recovered `jmp` to another span, and a span whose last
    // instruction does not transfer control, both continue the SAME invocation
    // -- the original pushes no return address -- so the dispatcher in ts_call
    // picks this up in place instead of nesting another host frame.
    u32 tail;
    // The instruction that set `tail`, so an unregistered tail target can name
    // the `jmp` that produced it rather than the call that started the frame.
    u32 tail_from;
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
    m.unmapped_in_gap = 0;
    m.unmapped_below_1000 = 0;
    m.unmapped_in_rdata = 0;
    m.unmapped_elsewhere = 0;
    m.tail = 0; m.tail_from = 0;
    m.unmodelled_insns = 0;
    m.call_depth = 0;
    m.depth_exceeded = 0;
    m.unmapped_sample_n = 0;
    m.unmapped_text_sample_n = 0;
    m.unmapped_rec_n = 0;
    m.unmapped_text_rec_n = 0;
    m.esp_guard = 0; m.esp_guard_stop = 0; m.esp_bad_n = 0; m.esp_prev = 0;
    m.stop_on_unmapped = 0;
    m.fault_addr = 0; m.fault_page = 0; m.fault_used = 0; m.fault_cap = 0;
    m.bad_call_n = 0;
    m.watch_va = 0; m.watch_hits = 0; m.watch_zero = 0;
    m.watch_ring_n = 0;
    for (unsigned wi = 0; wi < (unsigned)Machine::WATCH_RING_ADDRS; ++wi)
        m.watch_ring_count[wi] = 0;
    m.write_guard_eip = 0; m.write_guard_ptr_addr = 0; m.write_guard_stop = 0;
    m.write_guard_bad = 0; m.write_guard_addr = 0; m.write_guard_base = 0;
    m.write_guard_size = 0; m.write_guard_step = 0;
    m.stub_dispatches = 0; m.stub_last_nargs = 0; m.stub_last_depth = 0;
    m.stack_imbalances = 0; m.stack_imbalance_n = 0;
    m.trace_va = 0; m.trace_n = 0; m.trace_ring_count = 0; m.leak_n = 0;
    m.probe_n = 0; m.probe_hook = 0; m.probe_deref = 0;
    for (unsigned pi = 0; pi < (unsigned)Machine::PROBE_MAX; ++pi)
        m.probe_hits[pi] = 0;
    for (unsigned pi = 0; pi < 128u; ++pi) m.probe_filter[pi] = 0;
    m.bad_calls = 0;
    if (m.exec_map && m.exec_map_hi > m.exec_map_lo)
        std::memset(m.exec_map, 0, (m.exec_map_hi - m.exec_map_lo + 7u) / 8u);
    if (m.func_map && m.func_map_hi > m.func_map_lo)
        std::memset(m.func_map, 0, (m.func_map_hi - m.func_map_lo + 7u) / 8u);
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

// Set one bit in a caller-owned coverage map. The map is one bit per byte of
// the covered VA range; a null map (the default) makes this a no-op.
inline void mark_map(u8 *map, u32 lo, u32 hi, u32 va) {
    if (map && va >= lo && va < hi) {
        u32 off = va - lo;
        map[off >> 3] = (u8)(map[off >> 3] | (u8)(1u << (off & 7u)));
    }
}

// Cold half of insn(): tracing, budget enforcement and the halt longjmp. Kept
// out of line (noinline) so the per-instruction body -- emitted at every one of
// the 912,879 instruction sites -- stays small enough for the compiler to
// inline. The order matches the old single body exactly, so a trace recorded on
// the instruction that exhausts the budget is still taken before the halt.
// Defined with the memory helpers below; declared here because the trace ring
// reads the caller's stack argument.
inline u32 ld32(u32 a);
inline u8 ld8(u32 a);

// Snapshot up to PROBE_STR-1 printable bytes of a model string. A probe is a
// diagnostic and must not fault on a wild register, so an implausible pointer
// yields an empty string rather than a page fault.
inline void probe_snapshot(char *dst, u32 a) {
    unsigned i = 0;
    if (a < 0x00010000u || a >= 0x80000000u) { dst[0] = 0; return; }
    for (; i + 1u < Machine::PROBE_STR; ++i) {
        u8 c = ld8(a + i);
        if (!c) break;
        dst[i] = (c >= 0x20u && c < 0x7fu) ? (char)c : '.';
    }
    dst[i] = 0;
}

// Record one hit at every configured probe address. Called from insn() only
// when probe_n is non-zero, so a normal run pays one predictable branch.
inline u32 probe_bit(u32 va) {
    return (va * 2654435761u) >> 20 & 4095u;
}

inline void probe_slow(u32 va) {
    Machine &m = mach();
    u32 bit = probe_bit(va);
    if (!(m.probe_filter[bit >> 5] & (1u << (bit & 31u)))) return;
    for (u32 i = 0; i < m.probe_n; ++i) {
        if (m.probe_va[i] != va) continue;
        u32 h = m.probe_hits[i];
        if (h < Machine::PROBE_HITS) {
            Machine::ProbeHit &r = m.probe_rec[i][h];
            r.step = m.steps;
            r.esp = ESP.d; r.ebp = EBP.d; r.eax = EAX.d; r.ebx = EBX.d;
            r.ecx = ECX.d; r.edx = EDX.d; r.esi = ESI.d; r.edi = EDI.d;
            r.ret = ld32(ESP.d);
            r.a0 = ld32(ESP.d + 4u);
            r.a1 = ld32(ESP.d + 8u);
            if (m.probe_deref) {
                r.d0 = ld32(r.a0);
                r.d1 = ld32(r.a0 + 4u);
            } else {
                r.d0 = 0; r.d1 = 0;
            }
            probe_snapshot(r.eax_str, EAX.d);
            probe_snapshot(r.ecx_str, ECX.d);
        }
        ++m.probe_hits[i];
        if (m.probe_hook) m.probe_hook(va);
        return;
    }
}

#if defined(__GNUC__)
__attribute__((noinline))
#endif
inline void insn_slow(u32 va) {
    Machine &m = mach();
    if (m.trace_va && va == m.trace_va) {
        if (m.trace_n < 8u) {
            m.trace_esp[m.trace_n] = ESP.d;
            m.trace_eax[m.trace_n] = EAX.d;
            m.trace_ecx[m.trace_n] = ECX.d;
            m.trace_edx[m.trace_n] = EDX.d;
            m.trace_esi[m.trace_n] = ESI.d;
            m.trace_edi[m.trace_n] = EDI.d;
            ++m.trace_n;
        }
        unsigned k = m.trace_ring_count & 7u;
        m.trace_ring_esp[k] = ESP.d;
        m.trace_ring_arg[k] = ld32(ESP.d + 4u);
        m.trace_ring_eax[k] = EAX.d;
        m.trace_ring_step[k] = m.steps;
        ++m.trace_ring_count;
    }
    if (m.step_limit && m.steps > m.step_limit) {
        if (!m.faulted) { m.step_limit_hit = true; fault("instruction budget exhausted"); }
    }
    if (m.faulted && m.halt) longjmp(*m.halt, 1);
}

// ESP left the stack region, or moved further in one instruction than any real
// prologue/epilogue does. `prev` is the instruction that made the move (insn()
// is called at the START of each instruction, so the register state here is
// what that instruction left behind). Recording it is the whole point: after a
// wild ESP every later push/ret is garbage, and the flood is not the defect.
#if defined(__GNUC__)
__attribute__((noinline))
#endif
inline void esp_guard_hit(u32 prev, u32 va) {
    Machine &m = mach();
    if (m.esp_bad_n < 8u) {
        Machine::EspRec &r = m.esp_bad[m.esp_bad_n++];
        r.prev_eip = prev; r.eip = va; r.esp = ESP.d; r.ebp = EBP.d;
        r.eax = EAX.d; r.ecx = ECX.d; r.edx = EDX.d;
        r.steps = m.steps;
    }
    if (m.esp_guard_stop && !m.faulted) {
        m.eip = prev;
        fault("ESP outside the modelled stack");
        if (m.halt) longjmp(*m.halt, 1);
    }
}

// One call per transcribed instruction, emitted by tools/gen_code.py. It is the
// only per-instruction hook in the model: it keeps eip, counts instructions,
// and marks coverage. Anything that only happens on a trace, a budget stop or a
// halt goes to insn_slow().
inline void insn(u32 va) {
    Machine &m = mach();
    u32 prev = m.eip;          // the instruction that just executed
    m.eip = va;
    ++m.steps;
    mark_map(m.exec_map, m.exec_map_lo, m.exec_map_hi, va);
    // Off by default, so a normal run pays one predictable branch. When on, the
    // range is the game's own stack (ESP starts at 0x300000 and grows down) and
    // the delta catches a jump that stays inside the range. The largest real
    // frame adjustment in the transcription is under 0x8000.
    if (m.esp_guard && m.steps > 1u) {
        i32 delta = (i32)(ESP.d - m.esp_prev);
        if (ESP.d < 0x00100000u || ESP.d >= 0x00400000u ||
            delta > 0x10000 || delta < -0x10000)
            esp_guard_hit(prev, va);
        m.esp_prev = ESP.d;
    }
    if (m.probe_n)
        probe_slow(va);
    if (m.trace_va || m.faulted || (m.step_limit && m.steps > m.step_limit))
        insn_slow(va);
}

inline void unmodelled(u32 va, const char *mnem) {
    (void)va; (void)mnem;
    mach().unmodelled_insns++;
}

// -- memory -----------------------------------------------------------------
// The original is a flat 32-bit address space. Rather than allocate 4 GiB, pages
// are handed out lazily and kept in an open-addressed table; an address that was
// never written reads as zero, which is what the original's untouched .bss does.
// Accesses are assembled byte-wise so that a multi-byte access straddling a page
// boundary is still correct.
//
// The table GROWS, and that is a correctness property, not a convenience. It was
// once fixed at 8192 slots: a full table made page() return one shared dummy
// page for every new address, so distinct pages silently overwrote each other.
// It was then fixed at 32768 slots, which only moved the failure to a hard fault
// when the GDI mission load exceeded 128 MB. A fixed bound is the wrong shape
// for a model of a machine whose address space is 4 GiB: the table now doubles
// at 75% load up to 2^21 slots (a 50% load over the largest possible 2^20
// distinct pages), and refuses an insertion with a REPORTED fault rather than
// aliasing if the host cannot provide the memory. Every page still gets its own
// host allocation, so no two addresses ever share bytes.
//
// WHY THE DEMAND EXCEEDS 128 MB, measured on the GDI mission load: the model has
// no decommit, so its footprint is every page ever touched, not the program's
// peak live set. The largest contributor is not a leak of the game's: one
// DynamicVectorClass growth loop (resize at 0x0055b050) performs 5,664 growing
// reallocations totalling 679 MB -- each frees the old buffer and asks for a
// slightly larger one, and the model's bump heap cannot extend the freed block
// in place (Win32Stubs.h, not this table). The retail peak for that vector is
// one allocation, at most 273,680 B. Covering the cumulative footprint is what
// a 4 GiB address space is for; the heap-side coalescing fix is separate.
struct PageTable {
    enum {
        EMPTY = 0xffffffffu,      // never a page number: p is 20 bits
        INITIAL_SLOTS = 32768u,   // 128 MB of modelled memory before the first growth
        MAX_SLOTS = 1u << 21      // 4 GiB of address space at a 50% load
    };
    u32 *key;
    u8 **val;
    u32 cap;        // slots, always a power of two
    u32 max_slots;  // growth ceiling; MAX_SLOTS unless a caller asks for less
    u32 used;       // distinct pages handed out
    u32 peak;       // high-water mark of `used`
    u32 growths;    // rehashes performed
    u32 refused;    // insertions that could not be served (reported as a fault)
    // A direct-mapped cache of recent page-number -> block hits. The hash probe
    // is the model's hottest operation: a single `mov eax,[esi+4]` costs one
    // lookup per byte, and the recovered code's tight copy loops touch a
    // handful of pages over and over. Measured at 4B, page_in() was 30% of the
    // run. Only POSITIVE hits are cached, and a page's host block never moves
    // (resize() rehashes the index, not the blocks), so a hit here is always
    // the same answer the probe would give. CACHE_SLOTS is a power of two.
    enum { CACHE_SLOTS = 256 };
    u32 ckey[CACHE_SLOTS];
    u8 *cval[CACHE_SLOTS];
    // `initial` and `max` exist so a test -- or a memory-constrained run -- can
    // use a smaller table and reach the refusal path deliberately. Both are
    // rounded down to a power of two; the default is the full 4 GiB space.
    explicit PageTable(u32 initial = (u32)INITIAL_SLOTS,
                       u32 max = (u32)MAX_SLOTS)
        : key(0), val(0), cap(0), max_slots(0), used(0), peak(0),
          growths(0), refused(0) {
        for (u32 i = 0; i < (u32)CACHE_SLOTS; ++i) { ckey[i] = EMPTY; cval[i] = 0; }
        max_slots = pow2_floor(max ? max : 1u);
        initial = pow2_floor(initial ? initial : 1u);
        if (initial > max_slots) initial = max_slots;
        resize(initial);
    }
    static u32 pow2_floor(u32 v) {
        u32 p = 1u;
        while (p <= v / 2u) p <<= 1;
        return p;
    }
    ~PageTable() { std::free(key); std::free(val); }
    // Allocate a fresh index and reinsert every live entry. The iteration order
    // is the old array's, so the resulting layout is a function of the insertion
    // sequence alone -- the model stays deterministic.
    bool resize(u32 ncap) {
        u32 *nk = (u32 *)std::malloc(ncap * sizeof(u32));
        u8 **nv = (u8 **)std::malloc(ncap * sizeof(u8 *));
        if (!nk || !nv) { std::free(nk); std::free(nv); return false; }
        for (u32 i = 0; i < ncap; ++i) { nk[i] = EMPTY; nv[i] = 0; }
        for (u32 i = 0; i < cap; ++i) {
            if (key[i] == EMPTY) continue;
            u32 h = (key[i] * 2654435761u) & (ncap - 1u);
            while (nk[h] != EMPTY) h = (h + 1u) & (ncap - 1u);
            nk[h] = key[i]; nv[h] = val[i];
        }
        std::free(key); std::free(val);
        key = nk; val = nv; cap = ncap;
        return true;
    }
};

inline PageTable &pages() { static PageTable t; return t; }

// The one page-table lookup. `page()` is this against the model's single table;
// page_in() takes the table explicitly so a test can drive an independent one.
// The cache hit is the hot path and stays inline; a miss (probe, growth or the
// first touch of a page) goes out of line so the inline body is a handful of
// instructions at every load and store in the transcription.
// A refusal is a report about an ADDRESS, not just a table. "page allocation
// failed" at 1,024,211 of 2,097,152 slots is not a full table: it is the host
// refusing the 4 KB block for one page, and which page decides whether the
// demand is legitimate (a heap/stack working set) or a wild pointer walking the
// address space. Record both the address and the table state before faulting.
inline void page_fault(PageTable &t, u32 addr, u32 p, const char *why) {
    Machine &m = mach();
    m.fault_addr = addr;
    m.fault_page = p;
    m.fault_used = t.used;
    m.fault_cap = t.cap;
    fault(why);
}

#if defined(__GNUC__)
__attribute__((noinline))
#endif
inline u8 *page_in_slow(PageTable &t, u32 addr) {
    u32 p = addr >> 12;
    u32 off = addr & 0xfffu;
    u32 ci = p & (PageTable::CACHE_SLOTS - 1u);
    static u8 dummy[4096 + 8];   // only ever reached after fault() has halted
    if (!t.cap) {
        page_fault(t, addr, p, "page table exhausted: index allocation failed");
        return dummy + off;
    }
    for (;;) {
        u32 mask = t.cap - 1u;
        u32 h = (p * 2654435761u) & mask;
        u32 i = h;
        bool empty = false;
        for (u32 probe = 0; probe < t.cap; ++probe) {
            i = (h + probe) & mask;
            if (t.key[i] == p) {
                t.ckey[ci] = p; t.cval[ci] = t.val[i];
                return t.val[i] + off;
            }
            if (t.key[i] == PageTable::EMPTY) { empty = true; break; }
        }
        if (!empty) {
            ++t.refused;
            page_fault(t, addr, p, "page table exhausted: no free slot");
            return dummy + off;
        }
        // Grow before the table fills. A full table is not merely slow: with no
        // empty slot every miss becomes a cap-length probe, and the old code's
        // silent fallback is what aliased pages. Growth keeps the load at or
        // below 75%, so a miss is a short probe and a lookup never degrades.
        if (t.used + 1u > t.cap - (t.cap >> 2)) {
            if (t.cap >= t.max_slots || !t.resize(t.cap << 1)) {
                ++t.refused;
                page_fault(t, addr, p, "page table exhausted: host allocation failed");
                return dummy + off;
            }
            ++t.growths;
            continue;                 // re-probe in the larger index
        }
        u8 *blk = (u8 *)std::calloc(4096 + 8, 1);
        if (!blk) {
            ++t.refused;
            page_fault(t, addr, p, "page table exhausted: page allocation failed");
            return dummy + off;
        }
        t.key[i] = p; t.val[i] = blk;
        t.ckey[ci] = p; t.cval[ci] = blk;
        ++t.used;
        if (t.used > t.peak) t.peak = t.used;
        return blk + off;
    }
}

TS_HOT_INLINE u8 *page_in(PageTable &t, u32 addr) {
    u32 p = addr >> 12;
    u32 ci = p & (PageTable::CACHE_SLOTS - 1u);
    if (t.ckey[ci] == p) return t.cval[ci] + (addr & 0xfffu);
    return page_in_slow(t, addr);
}

TS_HOT_INLINE u8 *page(u32 addr) { return page_in(pages(), addr); }

// Copy a region of the original image in, so reads of initialised data return
// the real bytes rather than zero.
inline void map(u32 addr, u32 len) {
    (void)addr; (void)len;   // callers that have the image call memcpy themselves
}

inline u8  ld8 (u32 a) { u8 v; std::memcpy(&v, page(a), 1); return v; }
// A write watchpoint. "Something writes garbage into this address" is the
// hardest question to answer by reading code and the easiest by recording the EIP
// of the store. st8 is the primitive every wider store funnels through, so the
// hook lives here; putting it in st32 missed every byte-wise write.
// The ring form of the write watch. Called only when a ring is armed, so an
// ordinary run pays one predictable branch per byte store.
inline void watch_ring_note(u32 a, u8 v) {
    Machine &m = mach();
    for (unsigned i = 0; i < m.watch_ring_n; ++i) {
        if (a != m.watch_ring_va[i]) continue;
        unsigned k = m.watch_ring_count[i] & (Machine::WATCH_RING_HITS - 1u);
        m.watch_ring_eip[i][k] = m.eip;
        m.watch_ring_val[i][k] = v;
        m.watch_ring_steps[i][k] = m.steps;
        ++m.watch_ring_count[i];
    }
}
// A wide store must take the byte path when any ring address falls inside it,
// or the ring would miss a 32-bit write to the watched field.
inline bool watch_ring_overlaps(u32 a, u32 w) {
    Machine &m = mach();
    for (unsigned i = 0; i < m.watch_ring_n; ++i)
        if ((u32)(m.watch_ring_va[i] - a) < w) return true;
    return false;
}

inline void st8 (u32 a, u8 v) {
    Machine &m = mach();
    // watch_zero records zero stores too: "who cleared this field" is the
    // question a nonzero-only watch cannot answer.
    if (m.watch_va && a == m.watch_va && (v || m.watch_zero) && m.watch_hits < 8u) {
        m.watch_from[m.watch_hits] = m.eip;
        m.watch_val[m.watch_hits] = v;
        ++m.watch_hits;
    }
    if (m.watch_ring_n) watch_ring_note(a, v);
    if (m.write_guard_eip && m.eip == m.write_guard_eip) {
        u32 pa = m.write_guard_ptr_addr;
        u32 base = (u32)ld8(pa) | ((u32)ld8(pa + 1u) << 8) |
                   ((u32)ld8(pa + 2u) << 16) | ((u32)ld8(pa + 3u) << 24);
        u32 sz = 0;
        if (base)
            sz = (u32)ld8(base - 16u) | ((u32)ld8(base - 15u) << 8) |
                 ((u32)ld8(base - 14u) << 16) | ((u32)ld8(base - 13u) << 24);
        if (!base || a < base || a >= base + sz) {
            if (!m.write_guard_bad) {
                m.write_guard_addr = a;
                m.write_guard_base = base;
                m.write_guard_size = sz;
                m.write_guard_step = m.steps;
                m.write_guard_esp = ESP.d; m.write_guard_eax = EAX.d;
                m.write_guard_ecx = ECX.d; m.write_guard_edx = EDX.d;
            }
            ++m.write_guard_bad;
            if (m.write_guard_stop && !m.faulted) {
                fault("write outside the guarded heap block");
                if (m.halt) longjmp(*m.halt, 1);
            }
        }
    }
    std::memcpy(page(a), &v, 1);
}
// A multi-byte access that does not cross a page boundary is one page lookup
// and one memcpy instead of one lookup per byte. The byte-wise form stays as
// the fallback because a straddling access is still correct, and the host is
// little-endian like the target, so memcpy of the native value is the same
// bytes the composition would produce. A store whose range contains the write
// watchpoint still takes the byte path so the watch sees the same EIP and byte.
inline u16 ld16(u32 a) {
    if ((a & 0xfffu) <= 0xffeu) { u16 v; std::memcpy(&v, page(a), 2); return v; }
    return (u16)((u16)ld8(a) | ((u16)ld8(a + 1) << 8));
}
inline void st16(u32 a, u16 v) {
    Machine &m = mach();
    if ((a & 0xfffu) <= 0xffeu && !(m.watch_va && (u32)(m.watch_va - a) < 2u) &&
        !(m.watch_ring_n && watch_ring_overlaps(a, 2u))) {
        std::memcpy(page(a), &v, 2); return;
    }
    st8(a, (u8)v); st8(a + 1, (u8)(v >> 8));
}
inline u32 ld32(u32 a) {
    if ((a & 0xfffu) <= 0xffcu) { u32 v; std::memcpy(&v, page(a), 4); return v; }
    return (u32)ld8(a) | ((u32)ld8(a + 1) << 8) |
           ((u32)ld8(a + 2) << 16) | ((u32)ld8(a + 3) << 24);
}
inline void st32(u32 a, u32 v) {
    Machine &m = mach();
    if ((a & 0xfffu) <= 0xffcu && !(m.watch_va && (u32)(m.watch_va - a) < 4u) &&
        !(m.watch_ring_n && watch_ring_overlaps(a, 4u))) {
        std::memcpy(page(a), &v, 4); return;
    }
    st8(a, (u8)v); st8(a + 1, (u8)(v >> 8));
    st8(a + 2, (u8)(v >> 16)); st8(a + 3, (u8)(v >> 24));
}

// -- flags ------------------------------------------------------------------
// Flag semantics matter: the recovered conditional jumps test them, so an
// approximate model would silently change control flow. Each helper sets the
// flags the way the real instruction does.
//
// WIDTH MATTERS, and the helpers below take it explicitly. An 8- or 16-bit
// operation sets SF and OF from the result's own sign bit, not from bit 31:
// `cmp bl, 0x7a` is a signed comparison of bytes, so with SF taken from bit 31
// of the zero-extended result every byte value 0x80..0xff reads as
// non-negative and every JLE/JG on a byte is wrong. The same mistake in OF
// makes `jl`/`jg` on 0x7f/0x80 boundaries wrong. CF and AF are likewise the
// carry/borrow out of the operation's own top bit and bit 3, and PF is the
// parity of the low byte at every width. f_* are the 32-bit forms kept for
// their existing call sites; the narrow helpers pass 8 or 16.
inline u8 parity8(u32 r) {
    u32 v = r & 0xffu;
    v ^= v >> 4; v ^= v >> 2; v ^= v >> 1;
    return (u8)((v & 1u) ? 0u : 1u);
}
inline u32 width_mask(int w) { return (w >= 32) ? 0xffffffffu : ((1u << w) - 1u); }
inline u32 width_sign(int w) { return 1u << (w - 1); }

TS_HOT_INLINE void f_log_w(u32 r, int w) {
    Machine &m = mach();
    r &= width_mask(w);
    m.cf = 0; m.of = 0; m.af = 0;   // AF is undefined after a logical op; model 0.
    m.zf = (r == 0) ? 1 : 0;
    m.sf = (u8)((r & width_sign(w)) ? 1 : 0);
    m.pf = parity8(r);
}
TS_HOT_INLINE void f_add_w(u32 a, u32 b, u32 r, int w) {
    Machine &m = mach();
    u32 mask = width_mask(w), sign = width_sign(w);
    a &= mask; b &= mask; r &= mask;
    m.cf = (r < a) ? 1 : 0;
    m.of = (u8)((((a ^ r) & (b ^ r)) & sign) ? 1 : 0);
    m.zf = (r == 0) ? 1 : 0;
    m.sf = (u8)((r & sign) ? 1 : 0);
    m.af = (u8)(((a ^ b ^ r) >> 4) & 1);
    m.pf = parity8(r);
}
TS_HOT_INLINE void f_sub_w(u32 a, u32 b, u32 r, int w) {
    Machine &m = mach();
    u32 mask = width_mask(w), sign = width_sign(w);
    a &= mask; b &= mask; r &= mask;
    m.cf = (a < b) ? 1 : 0;
    m.of = (u8)((((a ^ b) & (a ^ r)) & sign) ? 1 : 0);
    m.zf = (r == 0) ? 1 : 0;
    m.sf = (u8)((r & sign) ? 1 : 0);
    m.af = (u8)(((a ^ b ^ r) >> 4) & 1);
    m.pf = parity8(r);
}
TS_HOT_INLINE void f_inc_w(u32 a, u32 r, int w) {
    Machine &m = mach();
    u32 mask = width_mask(w), sign = width_sign(w);
    a &= mask; r &= mask;
    m.of = (a == (sign - 1u)) ? 1 : 0;   // CF is unaffected by inc/dec.
    m.zf = (r == 0) ? 1 : 0;
    m.sf = (u8)((r & sign) ? 1 : 0);
    m.af = (u8)(((a ^ 1u ^ r) >> 4) & 1);
    m.pf = parity8(r);
}
TS_HOT_INLINE void f_dec_w(u32 a, u32 r, int w) {
    Machine &m = mach();
    u32 mask = width_mask(w), sign = width_sign(w);
    a &= mask; r &= mask;
    m.of = (a == sign) ? 1 : 0;
    m.zf = (r == 0) ? 1 : 0;
    m.sf = (u8)((r & sign) ? 1 : 0);
    m.af = (u8)(((a ^ 1u ^ r) >> 4) & 1);
    m.pf = parity8(r);
}

TS_HOT_INLINE void f_log(u32 r) { f_log_w(r, 32); }
TS_HOT_INLINE void f_add(u32 a, u32 b, u32 r) { f_add_w(a, b, r, 32); }
TS_HOT_INLINE void f_sub(u32 a, u32 b, u32 r) { f_sub_w(a, b, r, 32); }
TS_HOT_INLINE void f_inc(u32 a, u32 r) { f_inc_w(a, r, 32); }
TS_HOT_INLINE void f_dec(u32 a, u32 r) { f_dec_w(a, r, 32); }

// -- integer core -----------------------------------------------------------
// Helpers take their operands by value and RETURN the result rather than taking
// a destination by reference. That is what lets the generator handle a memory
// destination uniformly: `add dword ptr [ecx+8], 1` becomes
//   st32(ECX.d + 8, add32(ld32(ECX.d + 8), 1u));
// with the flags set exactly once, in the helper, rather than at the call site.
inline u32 add32(u32 a, u32 b) { u32 r = a + b; f_add(a, b, r); return r; }
inline u16 add16(u16 a, u16 b) { u16 r = (u16)(a + b); f_add_w(a, b, r, 16); return r; }
inline u8  add8 (u8  a, u8  b) { u8  r = (u8)(a + b);  f_add_w(a, b, r, 8); return r; }
inline u32 sub32(u32 a, u32 b) { u32 r = a - b; f_sub(a, b, r); return r; }
inline u16 sub16(u16 a, u16 b) { u16 r = (u16)(a - b); f_sub_w(a, b, r, 16); return r; }
inline u8  sub8 (u8  a, u8  b) { u8  r = (u8)(a - b);  f_sub_w(a, b, r, 8); return r; }
inline u32 adc32(u32 a, u32 b) { u32 c = M.cf; u32 r = a + b + c; f_add(a, b + c, r); return r; }
inline u32 sbb32(u32 a, u32 b) { u32 c = M.cf; u32 r = a - b - c; f_sub(a, b + c, r); return r; }
inline void cmp32(u32 a, u32 b) { f_sub(a, b, a - b); }
inline void cmp16(u16 a, u16 b) { f_sub_w(a, b, (u16)(a - b), 16); }
inline void cmp8 (u8  a, u8  b) { f_sub_w(a, b, (u8)(a - b), 8); }
inline void test32(u32 a, u32 b) { f_log(a & b); }
inline void test16(u16 a, u16 b) { f_log_w((u32)(a & b), 16); }
inline void test8 (u8  a, u8  b) { f_log_w((u32)(a & b), 8); }
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
//
// `div` divides the unsigned EDX:EAX by an unsigned operand; `idiv` divides the
// SIGNED EDX:EAX by a SIGNED operand. Both truncate toward zero, both give the
// remainder the dividend's sign, and both raise #DE when the divisor is zero or
// the quotient does not fit the destination register -- for `idiv` that is
// -2^31..2^31-1, not 0..2^32-1. `sign` selects the interpretation.
//
// The signed path divides magnitudes instead of casting to a signed 64-bit
// type: INT64_MIN / -1 is undefined in C++ (and traps on some hosts) while the
// x86 answer is a well-defined #DE, and the magnitude form raises exactly that.
inline u32 divmod64(u32 hi, u32 lo, u32 d, u32 &rem, bool sign) {
    if (d == 0) { fault("divide by zero"); rem = lo; return 0; }
    unsigned long long n = ((unsigned long long)hi << 32) | (unsigned long long)lo;
    if (!sign) {
        unsigned long long q = n / d;
        rem = (u32)(n % d);
        if (q > 0xffffffffull) fault("divide overflow");
        return (u32)q;
    }
    bool neg_n = (hi & 0x80000000u) != 0;
    bool neg_d = ((i32)d) < 0;
    unsigned long long un = neg_n ? 0ull - n : n;
    unsigned long long ud = neg_d ? (unsigned long long)(u32)(0u - d)
                                  : (unsigned long long)d;
    unsigned long long uq = un / ud;
    unsigned long long ur = un % ud;
    bool neg_q = (neg_n != neg_d);
    u32 r = neg_n ? (u32)(0u - (u32)ur) : (u32)ur;
    rem = r;
    if (uq > (neg_q ? 0x80000000ull : 0x7fffffffull)) {
        fault("divide overflow");
        return neg_q ? (u32)(0u - (u32)uq) : (u32)uq;
    }
    return neg_q ? (u32)(0u - (u32)uq) : (u32)uq;
}
inline u32 div32(u32 hi, u32 lo, u32 d, u32 &rem) { return divmod64(hi, lo, d, rem, false); }
inline u32 idiv32(u32 hi, u32 lo, u32 d, u32 &rem) { return divmod64(hi, lo, d, rem, true); }

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

// How many distinct VAs are registered, and how many of those are recovered
// functions rather than import stubs. The generated transcriptions live in
// .text (0x401000..0x698225); Image.h registers the import stubs and
// boot_driver the traced thunks at 0xa00000. The boot report uses the second
// count as the denominator for "N of M recovered functions entered".
inline unsigned long &registry_count() { static unsigned long n = 0; return n; }
inline unsigned long &registry_recovered_count() { static unsigned long n = 0; return n; }

inline void ts_register(u32 va, Fn f) {
    FnTable &t = fntab();
    u32 h = (va * 2654435761u) & (FnTable::SLOTS - 1);
    for (u32 p = 0; p < FnTable::SLOTS; ++p) {
        u32 i = (h + p) & (FnTable::SLOTS - 1);
        if (t.key[i] == va) { t.val[i] = f; return; }
        if (t.key[i] == 0xffffffffu) {
            t.key[i] = va; t.val[i] = f;
            ++registry_count();
            if (va < 0x700000u) ++registry_recovered_count();
            return;
        }
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

// -- recovered code extents ---------------------------------------------------
// The [lo, hi) ranges the tree has a transcription for, supplied once by the
// generated register_all_recovered(). Sorted and non-overlapping, so a target can
// be classified with a binary search rather than a guessed .text range.
inline const u32 *&span_table() { static const u32 *p = 0; return p; }
inline unsigned &span_count() { static unsigned n = 0; return n; }

inline void set_recovered_spans(const u32 *pairs, unsigned n) {
    span_table() = pairs;
    span_count() = n;
}

inline bool in_recovered_span(u32 va) {
    const u32 *p = span_table();
    unsigned n = span_count();
    unsigned lo = 0, hi = n;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2u;
        if (va < p[2u * mid]) hi = mid;
        else if (va >= p[2u * mid + 1u]) lo = mid + 1u;
        else return true;
    }
    return false;
}

// A recovered `call` names a target VA. Nothing is registered by default: a
// generated body is a member definition whose transcription reads the machine
// model rather than its own C++ parameters, so making the whole program *execute*
// is a separate step from transcribing it. An unregistered target is counted,
// not guessed at. The registry exists so that step can be added without touching
// a single generated line.
// Where did an unregistered target come from? Inside a recovered span it is a
// recovered function nothing registered, which is a defect; anywhere else in
// .text it is bytes the recovery never covered. With no extent table supplied the
// old, coarser rule stands, so a caller that does not register spans cannot
// accidentally pass by classifying every defect as a gap.
inline void count_unmapped_target(Machine &m, u32 va, u32 from) {
    if (!m.text_hi || va < m.text_lo || va >= m.text_hi) return;
    if (span_count() == 0 || in_recovered_span(va)) {
        ++m.unmapped_in_text;
        if (m.unmapped_text_sample_n < 16) {
            m.unmapped_text_from[m.unmapped_text_sample_n] = from;
            m.unmapped_text_sample[m.unmapped_text_sample_n++] = va;
        }
    } else {
        ++m.unmapped_in_gap;
    }
}

// All the accounting for one unregistered target, in one place: the existing
// samples and per-region counters, the first-N records with register state, and
// the optional stop-at-first. `kind` is 0 for a `call`, 1 for a tail transfer.
// The first records are what identify the defect; a run can produce millions of
// these and only the first few carry the state that produced them.
inline void note_unmapped(Machine &m, u32 va, u32 from, u32 kind) {
    if (m.unmapped_sample_n < 16) m.unmapped_sample[m.unmapped_sample_n++] = va;
    m.unmapped_calls++;
    if (!m.text_hi || va < m.text_lo || va >= m.text_hi) {
        ++m.bad_calls;
        if (va < 0x1000u) ++m.unmapped_below_1000;
        else if (va >= 0x698000u && va < 0x6bb000u) ++m.unmapped_in_rdata;
        else ++m.unmapped_elsewhere;
        if (m.bad_call_n < 16) {
            m.bad_call_from[m.bad_call_n] = from;
            m.bad_call_to[m.bad_call_n] = va;
            ++m.bad_call_n;
        }
    }
    count_unmapped_target(m, va, from);
    if (m.unmapped_rec_n < (unsigned)Machine::UNMAPPED_RECS) {
        Machine::UnmappedRec &r = m.unmapped_rec[m.unmapped_rec_n++];
        r.to = va; r.from = from; r.kind = kind; r.depth = (u32)m.call_depth;
        r.steps = m.steps;
        r.esp = ESP.d; r.ebp = EBP.d; r.eax = EAX.d; r.ebx = EBX.d;
        r.ecx = ECX.d; r.edx = EDX.d; r.esi = ESI.d; r.edi = EDI.d;
    }
    if (m.text_hi && va >= m.text_lo && va < m.text_hi &&
        m.unmapped_text_rec_n < 16u) {
        Machine::TextUnmappedRec &r =
            m.unmapped_text_rec[m.unmapped_text_rec_n++];
        r.to = va; r.from = from; r.depth = (u32)m.call_depth;
        r.steps = m.steps;
        r.esp = ESP.d; r.ebp = EBP.d; r.eax = EAX.d; r.ecx = ECX.d;
        r.edx = EDX.d; r.esi = ESI.d; r.edi = EDI.d;
    }
    if (m.stop_on_unmapped && !m.faulted) {
        // Halt AT the call site with the registers untouched, so the report
        // names the instruction that made the call and the pointer it used.
        m.eip = from;
        fault("first unmapped call");
        if (m.halt) longjmp(*m.halt, 1);
    }
}

inline void ts_call(u32 va) {
    Machine &m = mach();
    u32 from = m.eip;      // the call site, before eip is overwritten
    m.eip = va;
    m.recent_calls[m.recent_call_n & 7u] = va;
    ++m.recent_call_n;
    Fn f = ts_lookup(va);
    if (!f) {
        note_unmapped(m, va, from, 0u);
        return;
    }
    mark_map(m.func_map, m.func_map_lo, m.func_map_hi, va);
    // Bound the depth. A recovered loop whose modelled condition never changes
    // state would recurse forever, and "ran out of host stack" is a worse
    // diagnostic than "depth exceeded at VA X".
    // Lazily defaulted: reset() is not necessarily called before the first
    // dispatch, and a limit of 0 would cut every call off at the root.
    if (m.call_depth_limit == 0) m.call_depth_limit = 4096;
    if (m.call_depth >= m.call_depth_limit) { m.depth_exceeded++; return; }
    m.call_depth++;
    if (m.call_depth > m.call_depth_max) m.call_depth_max = m.call_depth;
    // Run the body, then honour any tail transfer it left behind. A tail
    // transfer replaces the current frame rather than nesting one, so a chain of
    // continuations -- or a loop that crosses a span boundary -- costs no host
    // stack. Without this the continuation would have to be a nested ts_call,
    // and such a loop would grow the host stack once per iteration.
    m.tail = 0;
    for (;;) {
        f();
        if (!m.tail) break;
        va = m.tail;
        m.tail = 0;
        if (m.tail_from) from = m.tail_from;
        m.eip = va;
        m.recent_calls[m.recent_call_n & 7u] = va;
        ++m.recent_call_n;
        f = ts_lookup(va);
        if (!f) {
            note_unmapped(m, va, from, 1u);
            break;
        }
        mark_map(m.func_map, m.func_map_lo, m.func_map_hi, va);
    }
    m.call_depth--;
}
// A `call` pushes its return address before transferring, so the modelled stack
// stays the shape the callee's transcription expects.
// A call whose target nothing registered still RETURNS. Emulating that matters
// more than it looks: ts_call_at pushes the return address, and if no body runs
// nothing ever pops it, so the address LEAKS -- 4 bytes per unregistered call.
// The caller then does its own pop or ret and reads a return address where it
// expects a saved register. That is exactly how ESI became 0x6844df: the atexit
// walk at 0x6844dd called 71 unregistered targets, the leaked return addresses
// accumulated, and `pop %esi` in that same function loaded one of them. ESI is
// the pointer the CRT calls WinMain through, so the game "called WinMain" at an
// address inside ExitProcess and the link test saw an unmapped call from
// 0x5e7317. One leaked return address, four layers of confusion.
inline void ts_call_at(u32 target, u32 ret) {
    Machine &m = mach();
    u32 site = m.eip;              // the call instruction, for the report
    u32 esp_before = ESP.d;
    u32 depth_before = m.call_depth;
    unsigned long stubs_before = m.stub_dispatches;
    push32(ret);
    if (!ts_lookup(target)) {
        ESP.d += 4u;       // the callee is a no-op, so the call still returns
        ts_call(target);   // records it: unmapped, and outside-.text if it is
        m.eip = ret;       // the caller really does continue here
        return;
    }
    ts_call(target);
    // A __stdcall stub owns its arguments: stub_done pops the return address AND
    // the nargs words its table row declares, so a correct stub call ends at
    // esp_before + 4*nargs. Comparing it to esp_before made every correct stub
    // look like a leak (111,682 of them) and hid the real signal. A recovered
    // (non-stub) callee pushed nothing and popped only the return address, so it
    // must still end exactly at esp_before.
    if (m.stub_dispatches != stubs_before &&
        m.stub_last_depth == depth_before + 1u) {
        u32 expected = esp_before + 4u * m.stub_last_nargs;
        if (ESP.d != expected) {
            ++m.stack_imbalances;
            if (m.stack_imbalance_n < 16) {
                m.stack_imbalance_site[m.stack_imbalance_n] = site;
                m.stack_imbalance_delta[m.stack_imbalance_n] = (int)(ESP.d - esp_before);
                ++m.stack_imbalance_n;
            }
        }
    } else if (ESP.d != esp_before && m.leak_n < 64u) {
        u32 callee = target;
        unsigned i = 0;
        for (; i < m.leak_n; ++i)
            if (m.leak_va[i] == callee && m.leak_delta[i] == (int)(ESP.d - esp_before))
                break;
        if (i == m.leak_n) {
            m.leak_va[i] = callee;
            m.leak_delta[i] = (int)(ESP.d - esp_before);
            m.leak_count[i] = 0;
            ++m.leak_n;
        }
        ++m.leak_count[i];
    }
}

// A transfer that does NOT push a return address, so the callee's `ret` pops the
// caller's. Two cases in the recovered code are exactly that: a `jmp` to another
// span, and the end of a span whose last instruction is not a transfer (the
// original simply continues at the next address). Both name where to continue
// and return; ts_call dispatches it in the frame already running.
inline void ts_tail(u32 target) {
    Machine &m = mach();
    m.tail = target;
    m.tail_from = m.eip;
}

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
//
// This is the same call as ts_call_at and must balance the same way. It used to
// push the return address and hand off to ts_call, which is a no-op when the
// target is unregistered -- so every indirect call into an unregistered address
// leaked its return address. The atexit walk at 0x6844dd is one of those calls
// (`call eax`), and 27 of them leaked 108 bytes, which is how `pop %esi` at
// 0x6844ea came to load 0x6844df. Delegate rather than duplicate the balance.
inline void ts_call_ptr(u32 target, u32 ret) { ts_call_at(target, ret); }

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
// The conditional repeats (REP/REPNE with SCAS/CMPS) must leave ECX holding
// the iterations that did NOT run. The standard inline `strlen` is
// `repne scasb; not ecx`, so zeroing ECX here makes `not ecx` return
// 0xffffffff and the following `rep movsd` copy 4 GB. The unconditional
// repeats above legitimately finish with ECX == 0.
inline void ts_rep_scasb() { i32 s = rep_step(); while (ECX.d) { cmp8(AL, ld8(EDI.d)); EDI.d += s; --ECX.d; if (!M.zf) break; } }
inline void ts_repne_scasb() { i32 s = rep_step(); while (ECX.d) { cmp8(AL, ld8(EDI.d)); EDI.d += s; --ECX.d; if (M.zf) break; } }
inline void ts_rep_scasd() { i32 s = rep_step() * 4; while (ECX.d) { cmp32(EAX.d, ld32(EDI.d)); EDI.d += s; --ECX.d; if (!M.zf) break; } }
inline void ts_repne_scasd() { i32 s = rep_step() * 4; while (ECX.d) { cmp32(EAX.d, ld32(EDI.d)); EDI.d += s; --ECX.d; if (M.zf) break; } }
inline void ts_repe_cmpsb() { i32 s = rep_step(); while (ECX.d) { cmp8(ld8(ESI.d), ld8(EDI.d)); ESI.d += s; EDI.d += s; --ECX.d; if (!M.zf) break; } }
inline void ts_repe_cmpsw() { i32 s = rep_step() * 2; while (ECX.d) { cmp16(ld16(ESI.d), ld16(EDI.d)); ESI.d += s; EDI.d += s; --ECX.d; if (!M.zf) break; } }
inline void ts_repe_cmpsd() { i32 s = rep_step() * 4; while (ECX.d) { cmp32(ld32(ESI.d), ld32(EDI.d)); ESI.d += s; EDI.d += s; --ECX.d; if (!M.zf) break; } }
inline void ts_repne_cmpsb() { i32 s = rep_step(); while (ECX.d) { cmp8(ld8(ESI.d), ld8(EDI.d)); ESI.d += s; EDI.d += s; --ECX.d; if (M.zf) break; } }
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
// `ret` and `ret N` both pop the RETURN ADDRESS (4 bytes) as well as N bytes of
// arguments. Popping only N leaves the stack 4 bytes high on every single call,
// so ESP drifts upward and anything that reads [esp] as a return address reads a
// stale value -- which is what produced calls to garbage like 0x68649620. The
// stub layer always discarded the return address explicitly; the transcriptions
// did not, and the two have to agree.
inline void ret_pop(u32 n) { ESP.d += n + 4u; }
// A `ret imm` on a function that returns a value: pop the arguments and yield
// what the machine has in EAX. The transcription reads its result from the
// machine, not from a C++ expression.
inline u32 ret_pop_val(u32 n, u32 v) { ESP.d += n + 4u; return v; }
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
inline u8  inc8 (u8  a) { u8  r = (u8)(a + 1);  f_inc_w(a, r, 8); return r; }
inline u8  dec8 (u8  a) { u8  r = (u8)(a - 1);  f_dec_w(a, r, 8); return r; }
inline u16 inc16(u16 a) { u16 r = (u16)(a + 1); f_inc_w(a, r, 16); return r; }
inline u16 dec16(u16 a) { u16 r = (u16)(a - 1); f_dec_w(a, r, 16); return r; }
inline u8  neg8 (u8  a) { u8  r = (u8)(0 - a);  f_sub_w(0, a, r, 8); return r; }
inline u16 neg16(u16 a) { u16 r = (u16)(0 - a); f_sub_w(0, a, r, 16); return r; }
inline u8  not8 (u8  a) { return (u8)~a; }
inline u16 not16(u16 a) { return (u16)~a; }
inline u8  adc8 (u8  a, u8  b) { u8  c = M.cf; u8  r = (u8)(a + b + c);   f_add_w(a, (u8)(b + c), r, 8); return r; }
inline u16 adc16(u16 a, u16 b) { u16 c = M.cf; u16 r = (u16)(a + b + c); f_add_w(a, (u16)(b + c), r, 16); return r; }
inline u8  sbb8 (u8  a, u8  b) { u8  c = M.cf; u8  r = (u8)(a - b - c);   f_sub_w(a, (u8)(b + c), r, 8); return r; }
inline u16 sbb16(u16 a, u16 b) { u16 c = M.cf; u16 r = (u16)(a - b - c); f_sub_w(a, (u16)(b + c), r, 16); return r; }
inline u8  and8 (u8  a, u8  b) { u8  r = (u8)(a & b); f_log_w(r, 8); return r; }
inline u16 and16(u16 a, u16 b) { u16 r = (u16)(a & b); f_log_w(r, 16); return r; }
inline u8  or8  (u8  a, u8  b) { u8  r = (u8)(a | b); f_log_w(r, 8); return r; }
inline u16 or16 (u16 a, u16 b) { u16 r = (u16)(a | b); f_log_w(r, 16); return r; }
inline u8  xor8 (u8  a, u8  b) { u8  r = (u8)(a ^ b); f_log_w(r, 8); return r; }
inline u16 xor16(u16 a, u16 b) { u16 r = (u16)(a ^ b); f_log_w(r, 16); return r; }
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

// A `jmp dword ptr [reg*4 + table]` the emitter could not resolve statically:
// the table is real, but its entries land in tail spans outside the enclosing
// function's recorded extent, which is the property the static resolver tests
// for. The instruction still reads a real target out of image memory at run
// time, so this is an ordinary tail transfer, not an unmapped call.
//
// It used to count the jump and do nothing. That silently skipped the target
// span -- and with it the function's epilogue: `memcpy` at 0x68cde0 dispatches
// here after `rep movsd`, so its `pop esi/edi; leave; ret` never ran, leaving
// ESP 16 bytes low and ESI holding the source pointer. The static-constructor
// walk calls memcpy, so the corruption surfaced as _initterm walking .text as
// a function table.
inline void jmp_indirect(u32 target) { ts_tail(target); }

}  // namespace ts

#undef TS_HOT_INLINE

#endif  // TS_MACHINE_H
