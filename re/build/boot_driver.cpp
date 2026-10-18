// build/boot_driver.cpp -- boot bin/GAME.EXE inside the machine model and
// report, in one page, what the game did before it stopped.
//
// Driven by tools/boot.py, which compiles this file plus the generated tree,
// caches the objects, runs the result under an instruction budget and a
// wall-clock watchdog, and names the recovered function the run stopped in.
// Run this file directly only when debugging the driver itself.
//
// The run is a MEASUREMENT, not a gate. The model has stubs for Windows, not
// Windows, so a fault, a hang or an exhausted budget is progress and exits 0.
// Two conditions do exit non-zero, because both are defects in this tree
// rather than missing Windows:
//
//   2  the image will not load
//   3  an unmapped call to an address inside .text -- a recovered function
//      that nothing registered
//
// WHAT THIS ADDS OVER build/ts_link_test.cpp (which stays as it is: it is the
// pipeline's link gate). ts_link_test reports the first 64 stub calls and a
// per-name call count. That answers "did it get as far as HeapAlloc". It does
// not answer "what was it in the middle of when it stopped", which is the
// question a boot needs, so this driver keeps
//
//   * the last kEventRing import calls with the instruction number and the
//     call depth each happened at, so the end of a run reads as a sequence
//     rather than as a single EIP;
//   * the model's last call targets (Machine.h keeps the most recent 8 in
//     recent_calls; that ring is another lane's file, so 8 is the bound here);
//   * a stable `key: value` report block that tools/boot.py parses to name the
//     containing recovered function and print its first instructions.
//
// Usage (tools/boot.py does all of this; direct use is for driver work):
//   build/boot_driver [--image=PATH] [--budget=N] [--timeout=SECONDS]
//                     [--depth=N] [--imports=N] [--calls=N] [--verbose]
//
// Defaults: --image=bin/GAME.EXE --budget=20000000 --timeout=60 --depth=20000
//           --imports=12 --calls=8
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <setjmp.h>
#include <csignal>
#include <sys/time.h>

#include "Machine.h"
#include "code/recovered.h"
#include "Image.h"
#include "Win32Stubs.h"

namespace {

typedef ts::u32 u32;
typedef ts::u8  u8;

const u32 kEntryPoint  = 0x006854e3u;   // PE entry point of bin/GAME.EXE
const u32 kTextLo      = 0x00401000u;   // .text: an unmapped call in here is a
const u32 kTextHi      = 0x00697225u;   // recovered function nothing registered
const u32 kStackTop    = 0x00300000u;   // modelled ESP at boot
const u32 kSentinel    = 0xdeadbeefu;   // return address the entry point pops
const u32 kWrapperBase = 0x00a00000u;   // traced import thunks live here
const u32 kWrapperStep = 16u;
const unsigned kMaxImports = 512;       // the image has 341
const unsigned kEventRing  = 64;        // last import calls kept, in order

// Coverage: one bit per byte of .text, in BSS so it costs nothing to start and
// nothing to leave out of a report that does not ask. The model writes the
// bits through Machine's maps; this file owns them and counts them.
const unsigned kTextBytes = kTextHi - kTextLo;
const unsigned kMapBytes  = (kTextBytes + 7u) / 8u;
u8 g_exec_map[kMapBytes];
u8 g_func_map[kMapBytes];

unsigned long popcount_map(const u8 *map, unsigned nbytes) {
    unsigned long n = 0;
    for (unsigned i = 0; i < nbytes; ++i) {
        u8 v = map[i];
        while (v) { v = (u8)(v & (u8)(v - 1u)); ++n; }   // clear the low set bit
    }
    return n;
}

// -- the run ----------------------------------------------------------------
struct Options {
    const char *image;
    const char *frame;      // TS_FRAME_DUMP / --frame: dump the primary surface
    const char *surface_dump;  // --surface-dump=PREFIX: PREFIX<i>.png per surface
    unsigned long long budget;   // 64-bit: budgets >= 2^32 must be expressible
    unsigned long depth_limit;
    unsigned wall_seconds;
    unsigned last_imports;
    unsigned last_calls;
    bool verbose;
    bool surfaces;
    Options()
        : image("bin/GAME.EXE"), frame(0), surface_dump(0), budget(20000000ul),
          depth_limit(20000ul), wall_seconds(60u), last_imports(12u),
          last_calls(8u), verbose(false), surfaces(false) {}
};

// The frame dump happens after the run, before the report, so the report can
// say whether the dump succeeded and how much of it is not black. A blank PNG
// is a real measurement; a missing PNG is a defect in the dump path.
bool          g_frame_ok = false;
unsigned      g_frame_w = 0, g_frame_h = 0;
unsigned long g_frame_nonblack = 0;
unsigned long g_frame_surfaces = 0;

bool     g_timed_out = false;
jmp_buf *g_halt = 0;

// The watchdog is periodic, not one-shot, and that is the difference between a
// 60-second iteration and a 0.3-second one.
//
// A transcription renders `rep stosd` as one instruction, so the model executes
// a 134-million-iteration loop inside a single call to insn(). A fault recorded
// half way through it (page table exhausted, say) cannot be acted on until the
// loop ends, because the only place that longjmps is insn() itself. The
// measured case: the run faults at instruction 237 and then burns 60 seconds
// finishing the `rep`. A one-shot alarm reports it a minute late.
//
// So the alarm repeats every kTickMs and the handler asks whether the model has
// already faulted. If it has, the run is over whatever the loop thinks, and it
// stops there. If it has not, the tick is counted and the run continues until
// the wall-clock budget is spent.
const unsigned kTickMs = 250;

unsigned long g_ticks = 0;
unsigned long g_tick_budget = 0;      // ticks allowed before the wall budget ends

void on_watchdog(int) {
    ts::Machine &m = ts::mach();
    if (m.faulted || m.step_limit_hit) {
        if (g_halt) longjmp(*g_halt, 1);   // POSIX lists longjmp as signal-safe
        return;
    }
    if (++g_ticks >= g_tick_budget) {
        g_timed_out = true;
        if (g_halt) longjmp(*g_halt, 2);
    }
}

// A periodic SIGALRM. The interval is what makes the handler re-enter; a caller
// that wants it stopped sets the timer to zero.
void watchdog_start(unsigned seconds) {
    g_ticks = 0;
    g_tick_budget = seconds ? (unsigned long)seconds * 1000ul / kTickMs : 1ul;
    struct itimerval t;
    std::memset(&t, 0, sizeof t);
    t.it_interval.tv_usec = kTickMs * 1000;
    t.it_value.tv_usec = kTickMs * 1000;
    setitimer(ITIMER_REAL, &t, 0);
}

void watchdog_stop() {
    struct itimerval t;
    std::memset(&t, 0, sizeof t);
    setitimer(ITIMER_REAL, &t, 0);
}

// -- import tracing ---------------------------------------------------------
// Image.h writes one stub VA into every IAT slot, so a hit on that VA cannot
// say WHICH import was called. Each slot is therefore re-pointed at its own
// thunk, which records the name and forwards to the stub.
//
// The record carries the instruction number and the call depth, not only the
// name: "HeapAlloc at step 8123, depth 7" says where in the run the call sat,
// and two consecutive records bound the work between them.
struct Bind {
    char dll[64];
    char fn[96];
    char label[168];
    unsigned long hits;
};

struct ImportEvent {
    unsigned idx;
    unsigned long step;
    unsigned depth;
};

Bind   g_bind[kMaxImports];
unsigned g_bind_n = 0;
ts::Fn g_orig[kMaxImports];
ts::Fn g_thunk[kMaxImports];

ImportEvent g_ring[kEventRing];
unsigned long g_events = 0;         // total, not modulo
unsigned long g_import_calls = 0;
bool g_verbose = false;             // print each import call as it happens

void record_import(unsigned i) {
    ImportEvent &e = g_ring[g_events % kEventRing];
    e.idx = i;
    e.step = ts::mach().steps;
    e.depth = (unsigned)ts::mach().call_depth;
    ++g_events;
    ++g_bind[i].hits;
    ++g_import_calls;
    if (g_verbose)
        std::printf("[import] %8lu %s depth=%u\n", e.step, g_bind[i].label, e.depth);
}

template <unsigned N> void import_thunk() {
    if (N >= g_bind_n) return;
    record_import(N);
    ts::Fn f = g_orig[N];
    if (f) { f(); return; }
    ts::mach().r[0].d = 0;      // nothing registered at that VA: return 0 and
    ts::pop32();                // still pop the return address the caller pushed
}

template <unsigned N> struct ThunkFiller {
    static void fill(ts::Fn *t) {
        t[N - 1] = &import_thunk<N - 1>;
        ThunkFiller<N - 1>::fill(t);
    }
};
template <> struct ThunkFiller<0> { static void fill(ts::Fn *) {} };

// The import a traced thunk VA stands for, or 0. The model's call ring records
// the thunk VA, so a report that only named recovered functions would print
// these as addresses belonging to no function -- they are this driver's own.
const char *wrapper_label(u32 va) {
    if (va < kWrapperBase) return 0;
    u32 off = va - kWrapperBase;
    if (off % kWrapperStep) return 0;
    u32 i = off / kWrapperStep;
    if (i >= g_bind_n) return 0;
    return g_bind[i].label;
}

// Bind every IAT slot to its own thunk. `resolve_imports()` has already put a
// working stub VA in each slot; this only interposes the record.
unsigned bind_traced_imports() {
    ts::pe::ImageState &img = ts::pe::image();
    ThunkFiller<kMaxImports>::fill(g_thunk);
    unsigned n = 0;
    for (unsigned i = 0; i < (unsigned)img.imports.size() && i < kMaxImports; ++i) {
        const ts::pe::Import &im = img.imports[i];
        u32 target = ts::ld32(im.slot);
        if (target == 0) continue;      // no stub: nothing to interpose on
        Bind &b = g_bind[n];
        std::strncpy(b.dll, im.dll, sizeof b.dll - 1);
        b.dll[sizeof b.dll - 1] = 0;
        std::strncpy(b.fn, im.fn, sizeof b.fn - 1);
        b.fn[sizeof b.fn - 1] = 0;
        std::sprintf(b.label, "%s!%s", b.dll, b.fn);
        b.hits = 0;
        g_orig[n] = ts::ts_lookup(target);
        u32 va = kWrapperBase + kWrapperStep * n;
        ts::ts_register(va, g_thunk[n]);
        ts::st32(im.slot, va);
        ++n;
    }
    g_bind_n = n;
    return n;
}

}  // namespace

namespace {

// -- reporting ---------------------------------------------------------------
// Everything a run knows, on stdout, as `key: value`. tools/boot.py parses this
// block and turns the addresses in it into recovered function names; nothing
// here is formatted for a human to read directly.

void print_bytes(u32 va) {
    for (unsigned i = 0; i < 16; ++i) std::printf("%s%02x", i ? " " : "", ts::ld8(va + i));
}

void print_regs(const ts::Reg *r) {
    static const char *rn[8] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
    for (int i = 0; i < 8; ++i) std::printf("%s%s=0x%08x", i ? " " : "", rn[i], r[i].d);
}

// The last `n` import calls, oldest first, with consecutive repeats collapsed:
// a run that calls HeapAlloc 40 times in a row should read as one line, because
// the interesting information is where the run left one import for another.
void print_last_imports(unsigned n) {
    unsigned long avail = g_events < kEventRing ? g_events : kEventRing;
    if (n > avail) n = (unsigned)avail;
    if (n == 0) { std::printf("last-imports: (none)\n"); return; }
    std::printf("last-imports:\n");
    unsigned long first = g_events - n;
    unsigned long i = first;
    while (i < g_events) {
        const ImportEvent &e = g_ring[i % kEventRing];
        unsigned long run = 1;
        while (i + run < g_events) {
            const ImportEvent &nx = g_ring[(i + run) % kEventRing];
            if (nx.idx != e.idx) break;
            ++run;
        }
        const ImportEvent &last = g_ring[(i + run - 1) % kEventRing];
        std::printf("  %lu. %s  x%lu  depth=%u  step=%lu\n",
                    i - first + 1, g_bind[e.idx].label, run, e.depth, last.step);
        i += run;
    }
}

void print_import_counts(unsigned n) {
    bool *done = (bool *)std::calloc(g_bind_n ? g_bind_n : 1, sizeof(bool));
    if (!done) return;
    std::printf("imports-by-count:\n");
    for (unsigned k = 0; k < n; ++k) {
        unsigned best = (unsigned)-1;
        for (unsigned i = 0; i < g_bind_n; ++i) {
            if (done[i] || g_bind[i].hits == 0) continue;
            if (best == (unsigned)-1 || g_bind[i].hits > g_bind[best].hits) best = i;
        }
        if (best == (unsigned)-1) break;
        done[best] = true;
        std::printf("  %lu %s\n", g_bind[best].hits, g_bind[best].label);
    }
    std::free(done);
}

// For the call targets that are this driver's import thunks, print what they
// stand for. Emitted only when at least one target is a thunk, so an ordinary
// report does not grow a line of nothing.
void print_thunk_names(const char *key, const u32 *va, unsigned n) {
    bool any = false;
    for (unsigned i = 0; i < n; ++i) if (wrapper_label(va[i])) any = true;
    if (!any) return;
    std::printf("%s:", key);
    for (unsigned i = 0; i < n; ++i) {
        const char *label = wrapper_label(va[i]);
        if (label) std::printf(" 0x%08x=%s", va[i], label);
    }
    std::printf("\n");
}

void print_last_calls(unsigned n) {
    ts::Machine &m = ts::mach();
    unsigned avail = m.recent_call_n < 8 ? m.recent_call_n : 8;
    if (n > avail) n = avail;
    u32 va[8];
    // The model's ring is 8 wide; anything older has been overwritten.
    for (unsigned i = 0; i < n; ++i) va[i] = m.recent_calls[(m.recent_call_n - n + i) & 7u];
    std::printf("last-calls:");
    for (unsigned i = 0; i < n; ++i) std::printf(" 0x%08x", va[i]);
    if (n == 0) std::printf(" (none)");
    std::printf("\n");
    print_thunk_names("last-call-thunks", va, n);
}

// A message loop is the next milestone after the CRT: if the run reached
// GetMessage/PeekMessage the game's own main is running its event pump.
void print_message_loop() {
    unsigned long get = 0, peek = 0, disp = 0, create = 0;
    for (unsigned i = 0; i < g_bind_n; ++i) {
        const char *n = g_bind[i].fn;
        if (!std::strcmp(n, "GetMessageA") || !std::strcmp(n, "GetMessageW")) get += g_bind[i].hits;
        if (!std::strcmp(n, "PeekMessageA") || !std::strcmp(n, "PeekMessageW")) peek += g_bind[i].hits;
        if (!std::strcmp(n, "DispatchMessageA") || !std::strcmp(n, "DispatchMessageW")) disp += g_bind[i].hits;
        if (!std::strcmp(n, "CreateWindowExA")) create += g_bind[i].hits;
    }
    std::printf("message-loop: %s\n", (get || peek) ? "reached" : "not-reached");
    std::printf("message-calls: GetMessage=%lu PeekMessage=%lu DispatchMessage=%lu CreateWindowExA=%lu\n",
                get, peek, disp, create);
}

unsigned long distinct_imports() {
    unsigned long n = 0;
    for (unsigned i = 0; i < g_bind_n; ++i) if (g_bind[i].hits) ++n;
    return n;
}

// TS_HEAP_AT=0xADDR[,0xADDR...] -- which heap block holds this address, and
// whether the block chain itself is intact. An address that is the user pointer
// of a LIVE block is a legitimate allocation; an address inside another block's
// payload is an overflow or an aliased pointer, and the difference decides
// whether the defect is the game's data or this heap.
void print_heap_at(const char *list) {
    ts::u32 p = ts::w32::heap_first();
    unsigned long blocks = 0, live = 0, freeb = 0, bytes = 0;
    ts::u32 prev = 0, break_at = 0, break_next = 0;
    bool links = true, adjacent = true;
    while (p && blocks < 2000000ul) {
        ts::u32 cap = ts::w32::heap_cap(p);
        if (ts::w32::heap_prev(p) != prev) links = false;
        if (prev && adjacent && prev + (ts::u32)ts::w32::HEAP_HDR +
                        ts::w32::heap_cap(prev) != p) {
            adjacent = false;
            break_at = prev;
            break_next = p;
        }
        if (ts::w32::heap_is_free(p)) ++freeb; else ++live;
        bytes += cap;
        prev = p;
        p = ts::w32::heap_next(p);
        ++blocks;
    }
    std::printf("heap-chain: %lu blocks, %lu live, %lu free, %lu payload bytes, "
                "prev-links=%s physically-adjacent=%s\n",
                blocks, live, freeb, bytes,
                links ? "ok" : "BROKEN", adjacent ? "ok" : "BROKEN");
    if (break_at) {
        ts::u32 cap = ts::w32::heap_cap(break_at);
        std::printf("  break: hdr=0x%08x user=0x%08x size=0x%x cap=0x%x free=%u "
                    "next=0x%08x; expected physical next 0x%08x\n",
                    break_at, break_at + (ts::u32)ts::w32::HEAP_HDR,
                    (unsigned)ts::ld32(break_at), cap,
                    ts::w32::heap_is_free(break_at) ? 1u : 0u,
                    ts::w32::heap_next(break_at),
                    break_at + (ts::u32)ts::w32::HEAP_HDR + cap);
        std::printf("         block there: hdr=0x%08x size=0x%x cap=0x%x free=%u "
                    "next=0x%08x\n", break_next, (unsigned)ts::ld32(break_next),
                    ts::w32::heap_cap(break_next),
                    ts::w32::heap_is_free(break_next) ? 1u : 0u,
                    ts::w32::heap_next(break_next));
    }
    const char *q = list;
    while (*q) {
        while (*q == ' ' || *q == ',') ++q;
        if (!*q) break;
        bool deref = (*q == '*');
        if (deref) ++q;
        char *end = 0;
        unsigned long addr = std::strtoul(q, &end, 0);
        if (end == q) break;
        q = end;
        if (deref) {
            ts::u32 v = (ts::u32)ts::ld32((ts::u32)addr);
            std::printf("  *0x%08lx = 0x%08x\n", addr, v);
            addr = v;
        }
        // Walk the chain by the next pointer first; if that misses, walk by
        // physical adjacency (a broken next pointer must not hide the block).
        bool found = false;
        for (int mode = 0; mode < 2 && !found; ++mode) {
            ts::u32 b = ts::w32::heap_first();
            unsigned long guard = 0;
            while (b && guard++ < 2000000ul) {
                ts::u32 cap = ts::w32::heap_cap(b);
                ts::u32 user = b + (ts::u32)ts::w32::HEAP_HDR;
                if ((ts::u32)addr >= user && (ts::u32)addr < user + cap) {
                    std::printf("  0x%08lx -> hdr=0x%08x user=0x%08x size=0x%x "
                                "cap=0x%x free=%u offset=0x%x%s\n", addr, b, user,
                                (unsigned)ts::ld32(b), cap,
                                ts::w32::heap_is_free(b) ? 1u : 0u,
                                (unsigned)((ts::u32)addr - user),
                                mode ? " (physical walk)" : "");
                    found = true;
                    break;
                }
                b = mode ? b + (ts::u32)ts::w32::HEAP_HDR + cap
                         : ts::w32::heap_next(b);
                if (mode && (b >= ts::w32::bump() || cap > 0x08000000u))
                    break;
            }
        }
        if (!found)
            std::printf("  0x%08lx -> not inside any heap block\n", addr);
    }
}

void print_report(const Options &o, bool returned, unsigned long esp0) {
    ts::Machine &m = ts::mach();
    ts::pe::ImageState &img = ts::pe::image();
    const bool completed = returned && m.r[4].d > esp0;

    const char *stop;
    if (m.faulted && m.step_limit_hit) stop = "budget";
    else if (m.faulted) stop = "fault";
    else if (g_timed_out) stop = "watchdog";
    else if (completed) stop = "returned";
    else stop = "halted";

    std::printf("boot-report: begin\n");
    std::printf("image: %s\n", o.image);
    std::printf("image-bytes: %u\n", img.bytes_mapped);
    std::printf("sections: %u\n", img.section_count);
    std::printf("imports-total: %u\n", (unsigned)img.imports.size());
    std::printf("imports-resolved: %u\n", img.resolved);
    std::printf("imports-bound: %u\n", g_bind_n);
    std::printf("entry: 0x%08x\n", kEntryPoint);
    std::printf("esp-start: 0x%08lx\n", esp0);
    std::printf("budget: %llu\n", o.budget);
    std::printf("depth-limit: %lu\n", o.depth_limit);
    std::printf("watchdog: %u\n", o.wall_seconds);
    std::printf("stop: %s\n", stop);
    std::printf("steps: %llu\n", m.steps);
    std::printf("depth: %lu\n", m.call_depth_max);
    std::printf("depth-exceeded: %lu\n", m.depth_exceeded);
    std::printf("eip: 0x%08x\n", m.eip);
    std::printf("eip-bytes: ");
    print_bytes(m.eip);
    std::printf("\n");
    std::printf("regs: ");
    print_regs(m.r);
    std::printf("\n");
    std::printf("unmapped-calls: %lu\n", m.unmapped_calls);
    std::printf("unmapped-in-text: %lu\n", m.unmapped_in_text);
    // Where the garbage targets point. A null function pointer (below 0x1000),
    // a data pointer used as code (.rdata), and a stack/heap value are three
    // different defects; a single "unmapped" count hides which one is left.
    std::printf("unmapped-below-1000: %lu\n", m.unmapped_below_1000);
    std::printf("unmapped-rdata: %lu\n", m.unmapped_in_rdata);
    std::printf("unmapped-elsewhere: %lu\n", m.unmapped_elsewhere);
    // How much of the recovered code the run actually reached. The instruction
    // count is distinct VAs, not executions, so a loop cannot inflate it.
    std::printf("coverage-text-bytes: %u\n", kTextBytes);
    std::printf("coverage-instructions: %lu\n", popcount_map(g_exec_map, kMapBytes));
    std::printf("coverage-functions: %lu\n", popcount_map(g_func_map, kMapBytes));
    std::printf("coverage-functions-total: %lu\n", ts::registry_recovered_count());
    if (m.unmapped_text_sample_n) {
        // The target alone says the stack is wrong; the call site says which
        // push put it there. Both, for the same reason the out-of-text sample
        // carries both.
        std::printf("unmapped-in-text-sample:");
        for (unsigned i = 0; i < m.unmapped_text_sample_n; ++i)
            std::printf(" 0x%08x@0x%08x",
                        m.unmapped_text_sample[i], m.unmapped_text_from[i]);
        std::printf("\n");
    }
    // TS_PEEK=0x7b0d38[,0x7b1fdc,...] -- read a global at the end of the run.
    // The game's state is a dword in .data, and "which state is it in" is the
    // question that decides whether a stop means anything. Cheap, and it beats
    // guessing from the EIP alone.
    // TS_SCAN=start,count -- report entries of a dword table that are neither
    // zero nor inside .text. The static-constructor table (_initterm over
    // 0x6bb000..0x6be3bc) is all-valid in the file, so any entry that is not a
    // .text VA at the END of a run was written during it, and its index says when.
    if (const char *scan = std::getenv("TS_SCAN")) {
        unsigned long start = std::strtoul(scan, 0, 0);
        const char *comma = std::strchr(scan, ',');
        unsigned count = comma ? (unsigned)std::strtoul(comma + 1, 0, 0) : 0;
        unsigned bad = 0;
        for (unsigned i = 0; i < count; ++i) {
            unsigned v = (unsigned)ts::ld32((ts::u32)(start + i * 4u));
            if (v && (v < 0x401000u || v >= 0x698225u)) {
                if (bad < 12)
                    std::printf("  scan[%u] @0x%08lx = 0x%08x  (not a .text VA)\n",
                                i, start + i * 4u, v);
                ++bad;
            }
        }
        std::printf("scan: %u of %u entries are not .text VAs\n", bad, count);
    }
    if (m.leak_n) {
        std::printf("ESP moved across %u distinct callee/delta pairs\n", m.leak_n);
        for (u32 i = 0; i < m.leak_n && i < 14; ++i)
            std::printf("  callee 0x%08x delta %+d  x%lu\n",
                        m.leak_va[i], m.leak_delta[i], m.leak_count[i]);
    }
    if (m.trace_n) {
        std::printf("trace 0x%08x: %u hit(s)\n", m.trace_va, m.trace_n);
        for (u32 i = 0; i < m.trace_n; ++i)
            std::printf("  esp=0x%08x esi=0x%08x edi=0x%08x eax=0x%08x ecx=0x%08x edx=0x%08x\n",
                        m.trace_esp[i], m.trace_esi[i], m.trace_edi[i],
                        m.trace_eax[i], m.trace_ecx[i], m.trace_edx[i]);
    }
    if (m.trace_ring_count) {
        unsigned long cnt = m.trace_ring_count;
        unsigned show = cnt < 8ul ? (unsigned)cnt : 8u;
        std::printf("trace-ring 0x%08x: %lu hit(s), last %u:\n",
                    m.trace_va, cnt, show);
        for (unsigned k = 0; k < show; ++k) {
            unsigned idx = (unsigned)((cnt - show + k) & 7u);
            std::printf("  step=%llu esp=0x%08x arg=[esp+4]=0x%08x eax=0x%08x\n",
                        m.trace_ring_step[idx], m.trace_ring_esp[idx],
                        m.trace_ring_arg[idx], m.trace_ring_eax[idx]);
        }
    }
    if (m.probe_n) {
        std::printf("probes:\n");
        for (u32 i = 0; i < m.probe_n; ++i) {
            unsigned show = m.probe_hits[i] < (u32)ts::Machine::PROBE_HITS
                                ? m.probe_hits[i] : (u32)ts::Machine::PROBE_HITS;
            std::printf("  0x%08x: %u hit(s)", m.probe_va[i], m.probe_hits[i]);
            if (m.probe_hits[i] > show)
                std::printf(" (first %u)", show);
            std::printf("\n");
            for (unsigned k = 0; k < show; ++k) {
                const ts::Machine::ProbeHit &r = m.probe_rec[i][k];
                std::printf("    step=%llu esp=0x%08x ebp=0x%08x eax=0x%08x "
                            "ebx=0x%08x ecx=0x%08x edx=0x%08x esi=0x%08x "
                            "edi=0x%08x a0=0x%08x a1=0x%08x\n",
                            r.step, r.esp, r.ebp, r.eax, r.ebx, r.ecx, r.edx,
                            r.esi, r.edi, r.a0, r.a1);
                if (r.eax_str[0]) std::printf("      eax: \"%s\"\n", r.eax_str);
                if (r.ecx_str[0]) std::printf("      ecx: \"%s\"\n", r.ecx_str);
            }
        }
    }
    if (m.stack_imbalances) {
        std::printf("STACK IMBALANCE at %lu stub call(s)\n", m.stack_imbalances);
        for (u32 i = 0; i < m.stack_imbalance_n; ++i)
            std::printf("  call at 0x%08x left ESP %+d bytes\n",
                        m.stack_imbalance_site[i], m.stack_imbalance_delta[i]);
    }
    if (m.watch_hits) {
        std::printf("watch 0x%08x: %u write(s)\n", m.watch_va, m.watch_hits);
        for (unsigned i = 0; i < m.watch_hits; ++i)
            std::printf("  wrote 0x%08x from 0x%08x\n", m.watch_val[i], m.watch_from[i]);
    }
    // The LAST writes to each ring address, oldest first. A corrupt vptr is the
    // constructor (first writes) followed by something else; only the tail of
    // the write history names that something.
    for (unsigned i = 0; i < m.watch_ring_n; ++i) {
        unsigned long cnt = m.watch_ring_count[i];
        std::printf("watch-ring 0x%08x: %lu write(s)\n", m.watch_ring_va[i], cnt);
        unsigned show = cnt < (unsigned long)ts::Machine::WATCH_RING_HITS
                            ? (unsigned)cnt : (unsigned)ts::Machine::WATCH_RING_HITS;
        for (unsigned k = 0; k < show; ++k) {
            unsigned idx = (unsigned)((cnt - show + k) &
                                      (ts::Machine::WATCH_RING_HITS - 1u));
            std::printf("  byte 0x%02x from 0x%08x step=%llu\n",
                        m.watch_ring_val[i][idx], m.watch_ring_eip[i][idx],
                        m.watch_ring_steps[i][idx]);
        }
    }
    if (m.write_guard_eip) {
        std::printf("write-guard 0x%08x: %u store(s) outside the block at "
                    "*0x%08x\n", m.write_guard_eip, m.write_guard_bad,
                    m.write_guard_ptr_addr);
        if (m.write_guard_bad)
            std::printf("  first at 0x%08x step=%llu base=0x%08x size=0x%x "
                        "esp=0x%08x eax=0x%08x ecx=0x%08x edx=0x%08x\n",
                        m.write_guard_addr, m.write_guard_step,
                        m.write_guard_base, m.write_guard_size,
                        m.write_guard_esp, m.write_guard_eax,
                        m.write_guard_ecx, m.write_guard_edx);
    }
    if (const char *ha = std::getenv("TS_HEAP_AT")) print_heap_at(ha);
    if (const char *peek = std::getenv("TS_PEEK")) {
        std::printf("peek:\n");
        const char *p = peek;
        while (*p) {
            char *end = 0;
            unsigned long va = std::strtoul(p, &end, 0);
            if (end == p) break;
            std::printf("  0x%08lx = 0x%08x\n", va, (unsigned)ts::ld32((ts::u32)va));
            p = (*end == ',') ? end + 1 : end;
        }
    }
    // TS_FOLLOW=0xADDR,count -- read the pointer stored at ADDR, then dump
    // `count` dwords from where it points. A vector's data pointer is only
    // known at run time, so this is the way to read the array it names after a
    // stop: TS_PEEK can name the object, TS_FOLLOW can read its contents.
    if (const char *fol = std::getenv("TS_FOLLOW")) {
        std::printf("follow:\n");
        const char *p = fol;
        while (*p) {
            while (*p == ' ' || *p == ',') ++p;
            if (!*p) break;
            char *end = 0;
            unsigned long va = std::strtoul(p, &end, 0);
            if (end == p) break;
            p = end;
            unsigned count = 0;
            if (*p == ',') {
                ++p;
                count = (unsigned)std::strtoul(p, &end, 0);
                if (end != p) p = end;
            }
            ts::u32 base = (ts::u32)ts::ld32((ts::u32)va);
            std::printf("  [0x%08lx]=0x%08x", va, base);
            for (unsigned i = 0; i < count; ++i)
                std::printf(" [%u]=0x%08x", i, (unsigned)ts::ld32(base + i * 4u));
            std::printf("\n");
        }
    }
    // TS_FUNC_ENTERED=0xADDR[,0xADDR...] -- did the run enter this function?
    // Coverage counts distinct functions; this answers the one question a
    // bisect needs: was a suspect span executed at all?
    if (const char *fe = std::getenv("TS_FUNC_ENTERED")) {
        std::printf("func-entered:\n");
        const char *p = fe;
        while (*p) {
            char *end = 0;
            unsigned long va = std::strtoul(p, &end, 0);
            if (end == p) break;
            unsigned hit = 0;
            if (m.func_map && va >= m.func_map_lo && va < m.func_map_hi) {
                unsigned off = (unsigned)(va - m.func_map_lo);
                hit = (m.func_map[off >> 3] >> (off & 7u)) & 1u;
            }
            std::printf("  0x%08lx %s\n", va, hit ? "entered" : "not-entered");
            p = (*end == ',') ? end + 1 : end;
        }
    }
    // TS_WINDOWS=1 -- the model's live window and control table at the end of
    // the run. A dialog's controls carry the rectangles the button hit-test
    // uses; without this, "the click missed" is indistinguishable from "the
    // dialog was never created" or "it was created at another position".
    if (std::getenv("TS_WINDOWS")) {
        std::printf("windows:\n");
        for (unsigned i = 0; i < (unsigned)ts::w32::MAX_WINDOWS; ++i) {
            const ts::w32::WinRec &w = ts::w32::wins()[i];
            if (!w.hwnd) continue;
            std::printf("  0x%08x parent=0x%08x id=%u atom=0x%02x dialog=%u "
                        "shown=%u enabled=%u rect=(%d,%d %dx%d) text=\"%s\"\n",
                        w.hwnd, w.parent, w.id, w.atom, w.is_dialog, w.shown,
                        w.enabled, w.x, w.y, w.cx, w.cy, w.text);
        }
        std::printf("  input-target=0x%08x\n", ts::w32::input_target());
    }
    if (m.bad_calls) {
        std::printf("calls outside .text (not a stub, not code): %u\n", m.bad_calls);
        for (unsigned i = 0; i < m.bad_call_n; ++i)
            std::printf("  call 0x%08x from 0x%08x\n",
                        m.bad_call_to[i], m.bad_call_from[i]);
    }
    std::printf("unmodelled: %lu\n", m.unmodelled_insns);
    // Page-table occupancy. The model hands out one host page per distinct 4 KB
    // of the 32-bit address space; the table grows, so `used` is the working
    // set and `peak` its high-water mark. A run that crawls is a table at a high
    // load factor, which is why growth is reported.
    {
        ts::PageTable &pt = ts::pages();
        std::printf("pages-used: %u of %u slots (peak %u, growths %u, refused %u)\n",
                    pt.used, pt.cap, pt.peak, pt.growths, pt.refused);
    }
    std::printf("import-calls: %lu\n", g_import_calls);
    std::printf("distinct-imports: %lu\n", distinct_imports());
    print_message_loop();
    // The rendering layer's own state. A run can reach the menu and still draw
    // nothing (a blank primary surface); these counters say whether the game
    // blitted at all, and the dump path says whether the pixels are readable.
    std::printf("render-surfaces: %lu\n", (unsigned long)ts::rnd::surface_n());
    std::printf("render-blits: %lu\n", ts::rnd::stat_blits());
    std::printf("render-fills: %lu\n", ts::rnd::stat_fills());
    std::printf("render-locks: %lu\n", ts::rnd::stat_locks());
    std::printf("render-flips: %lu\n", ts::rnd::stat_flips());
    std::printf("input-messages: %lu\n", ts::w32::input_messages());
    if (o.surfaces) {
        std::printf("surfaces:\n");
        ts::rnd::report_surfaces(stdout);
    }
    if (o.frame) {
        std::printf("frame-dump: %s\n", o.frame);
        std::printf("frame-ok: %u\n", g_frame_ok ? 1u : 0u);
        std::printf("frame-size: %ux%u\n", g_frame_w, g_frame_h);
        std::printf("frame-nonblack: %lu\n", g_frame_nonblack);
        std::printf("frame-surfaces: %lu\n", g_frame_surfaces);
    }
    print_last_calls(o.last_calls);
    print_last_imports(o.last_imports);
    print_import_counts(12);
    if (m.faulted) {
        std::printf("fault: %s\n", m.fault_why ? m.fault_why : "(unrecorded)");
        std::printf("fault-eip: 0x%08x\n", m.fault_eip);
        std::printf("fault-bytes: ");
        print_bytes(m.fault_eip);
        std::printf("\n");
        std::printf("fault-steps: %llu\n", m.fault_steps);
        std::printf("fault-depth: %lu\n", m.fault_depth);
        std::printf("fault-regs: ");
        print_regs(m.fault_reg);
        std::printf("\n");
        std::printf("fault-calls:");
        for (unsigned i = 0; i < m.fault_calls_n; ++i)
            std::printf(" 0x%08x", m.fault_calls[i]);
        std::printf("\n");
        print_thunk_names("fault-call-thunks", m.fault_calls, m.fault_calls_n);
    }
    if (g_timed_out) {
        std::printf("watchdog-eip: 0x%08x\n", m.eip);
        std::printf("watchdog-steps: %llu\n", m.steps);
    }
    if (m.unmapped_sample_n) {
        std::printf("unmapped-sample:");
        for (unsigned i = 0; i < m.unmapped_sample_n; ++i)
            std::printf(" 0x%08x", m.unmapped_sample[i]);
        std::printf("\n");
    }
    // The first unregistered calls, with the state that produced them. The
    // flood after them is the same defect repeated; only these carry the
    // pointer chain. `kind` 0 is a `call`, 1 a tail transfer.
    if (m.unmapped_rec_n) {
        std::printf("first-unmapped:\n");
        for (unsigned i = 0; i < m.unmapped_rec_n; ++i) {
            const ts::Machine::UnmappedRec &r = m.unmapped_rec[i];
            std::printf("  #%u step=%llu %s to=0x%08x from=0x%08x depth=%u "
                        "esp=0x%08x ebp=0x%08x eax=0x%08x ebx=0x%08x "
                        "ecx=0x%08x edx=0x%08x esi=0x%08x edi=0x%08x\n",
                        i, r.steps, r.kind ? "tail" : "call",
                        r.to, r.from, r.depth, r.esp, r.ebp, r.eax, r.ebx,
                        r.ecx, r.edx, r.esi, r.edi);
        }
    }
    if (m.unmapped_text_rec_n) {
        std::printf("first-unmapped-in-text:\n");
        for (unsigned i = 0; i < m.unmapped_text_rec_n; ++i) {
            const ts::Machine::TextUnmappedRec &r = m.unmapped_text_rec[i];
            std::printf("  #%u step=%llu to=0x%08x from=0x%08x depth=%u "
                        "esp=0x%08x ebp=0x%08x eax=0x%08x ecx=0x%08x "
                        "edx=0x%08x esi=0x%08x edi=0x%08x\n",
                        i, r.steps, r.to, r.from, r.depth, r.esp, r.ebp,
                        r.eax, r.ecx, r.edx, r.esi, r.edi);
        }
    }
    // The instruction that left the stack region. `prev` executed and moved
    // ESP; `at` is the next instruction, where the guard noticed.
    if (m.esp_bad_n) {
        std::printf("esp-guard:\n");
        for (unsigned i = 0; i < m.esp_bad_n; ++i) {
            const ts::Machine::EspRec &r = m.esp_bad[i];
            std::printf("  #%u step=%llu moved_by=0x%08x next=0x%08x "
                        "esp=0x%08x ebp=0x%08x eax=0x%08x ecx=0x%08x edx=0x%08x\n",
                        i, r.steps, r.prev_eip, r.eip, r.esp, r.ebp,
                        r.eax, r.ecx, r.edx);
        }
    }
    // Why the page table refused, and where its pages are. A refusal at 48.8%
    // of the table is not a full index: it is one 4 KB host allocation the
    // machine could not make, and the address says whether the demand is a
    // legitimate working set or a wild pointer.
    if (m.fault_addr) {
        std::printf("fault-addr: 0x%08x\n", m.fault_addr);
        std::printf("fault-page: 0x%05x\n", m.fault_page);
        std::printf("fault-table: used=%u cap=%u\n", m.fault_used, m.fault_cap);
    }
    if (m.faulted || std::getenv("TS_PAGES")) {
        // 1 MB regions: 4096 buckets over the 4 GiB space. The top few say
        // whether the footprint is the heap, the stack, or a wild range.
        static unsigned long hist[4096];
        for (unsigned i = 0; i < 4096; ++i) hist[i] = 0;
        ts::PageTable &pt = ts::pages();
        unsigned long live = 0;
        for (ts::u32 i = 0; i < pt.cap; ++i) {
            if (pt.key[i] == ts::PageTable::EMPTY) continue;
            ++hist[pt.key[i] >> 8];
            ++live;
        }
        std::printf("page-regions: %lu live pages\n", live);
        for (unsigned k = 0; k < 12; ++k) {
            unsigned best = 0;
            for (unsigned i = 1; i < 4096; ++i)
                if (hist[i] > hist[best]) best = i;
            if (!hist[best]) break;
            std::printf("  0x%08x-0x%08x  %lu pages (%lu MB)\n",
                        best << 20, ((best + 1u) << 20) - 1u,
                        hist[best], (hist[best] * 4u) / 1024u);
            hist[best] = 0;
        }
    }
    std::printf("boot-report: end\n");
}

// TS_MIX_LOOKUP=a,b,c -- after the run, ask the game's OWN MixFileClass
// lookup (0x547a40) whether a name is in a mounted MIX, and report the offset
// and size it answers. The lookup hashes the name itself, so this answers
// "does the game think this file exists" without reimplementing the hash.
// It must run after the entry point because the MIX list is empty before the
// game mounts its archives.
void mix_lookup_report() {
    const char *list = std::getenv("TS_MIX_LOOKUP");
    if (!list || !*list) return;
    ts::Machine &m = ts::mach();
    m.step_limit = m.steps + 20000000ul;
    m.call_depth_limit = 20000u;
    const ts::u32 name_va = 0x00520000u, mix_va = 0x00520100u,
                   off_va = 0x00520200u, size_va = 0x00520300u;
    std::printf("mix-lookup:\n");
    const char *p = list;
    while (*p) {
        char name[128];
        unsigned n = 0;
        while (*p && *p != ',' && n + 1u < sizeof name) name[n++] = *p++;
        name[n] = 0;
        if (*p == ',') ++p;
        if (!n) continue;
        for (unsigned i = 0; i <= n; ++i) ts::st8(name_va + i, (ts::u8)name[i]);
        ts::st32(mix_va, 0u); ts::st32(off_va, 0u); ts::st32(size_va, 0u);
        ESP.d = kStackTop;
        ts::push32(size_va);
        ts::push32(off_va);
        ts::push32(mix_va);
        ECX.d = name_va;
        EDX.d = 0u;
        ts::ts_call_at(0x00547a40u, 0x00fed000u);
        std::printf("  %-20s %s  offset=0x%08x size=0x%08x\n", name,
                    EAX.d ? "found" : "MISS",
                    (unsigned)ts::ld32(off_va), (unsigned)ts::ld32(size_va));
    }
}

// TS_INI_LOOKUP=0xADDR,... -- after the run, ask the game's own CCINIClass
// (FUN_004d5610, section entry count) what each object holds. ADDR is read as
// a pointer; if it does not look like one, ADDR itself is used as the object.
// This answers "is rules.ini actually loaded" and "does the scenario INI have
// a [Units] section" with the game's own parser rather than a reimplementation.
static void wr_cstr(ts::u32 va, const char *s) {
    unsigned i = 0;
    for (; s[i]; ++i) ts::st8(va + i, (ts::u8)s[i]);
    ts::st8(va + i, 0);
}

static ts::u32 ini_section_count(ts::u32 obj, const char *sec) {
    const ts::u32 sec_va = 0x00521000u;
    wr_cstr(sec_va, sec);
    ESP.d = kStackTop;
    ts::push32(sec_va);
    ECX.d = obj;
    ts::ts_call_at(0x004d5610u, 0x00fed100u);
    return EAX.d;
}

static void ini_entry(ts::u32 obj, const char *sec, unsigned idx,
                      char *out, unsigned n) {
    const ts::u32 sec_va = 0x00521000u, def_va = 0x00521100u,
                   buf_va = 0x00521200u;
    out[0] = 0;
    wr_cstr(sec_va, sec);
    wr_cstr(def_va, "");
    ESP.d = kStackTop;
    ts::push32(idx);
    ts::push32(sec_va);
    ECX.d = obj;
    ts::ts_call_at(0x004d5840u, 0x00fed110u);
    ts::u32 key = EAX.d;
    if (!key) return;
    ts::st8(buf_va, 0);
    ESP.d = kStackTop;
    ts::push32(0x80u);
    ts::push32(buf_va);
    ts::push32(def_va);
    ts::push32(key);
    ts::push32(sec_va);
    ECX.d = obj;
    ts::ts_call_at(0x004d6ed0u, 0x00fed120u);
    unsigned i = 0;
    for (; i + 1u < n; ++i) {
        ts::u8 c = ts::ld8(buf_va + i);
        if (!c) break;
        out[i] = (c >= 0x20u && c < 0x7fu) ? (char)c : '.';
    }
    out[i] = 0;
}

// TS_INI_AT=0xADDR -- run the INI section query at a probe hit, with the INI
// object taken from ECX. Mission-init entry (0x005c4200) is the one that
// matters: its `this` is the scenario INI loaded from the map, and that object
// is destroyed when the caller returns, so a post-run query cannot see it.
// The machine state is saved and restored around the query; the game must not
// notice it ran.
static ts::u32 g_ini_at[8];
static unsigned g_ini_at_n;
static int g_ini_busy;

static void ini_probe_hook(ts::u32 va) {
    bool want = false;
    for (unsigned i = 0; i < g_ini_at_n; ++i)
        if (g_ini_at[i] == va) want = true;
    if (!want || g_ini_busy) return;
    g_ini_busy = 1;
    ts::Machine &m = ts::mach();
    ts::u32 save_r[8];
    for (int i = 0; i < 8; ++i) save_r[i] = m.r[i].d;
    ts::u32 save_esp = ESP.d, save_eip = m.eip, save_steps = m.steps;
    ts::u32 save_depth = m.call_depth, save_rcn = m.recent_call_n;
    ts::u8 save_cf = m.cf, save_pf = m.pf, save_af = m.af, save_zf = m.zf,
            save_sf = m.sf, save_df = m.df, save_of = m.of;
    ts::u32 obj = ECX.d;
    // At the unit constructor, the first stack argument is the house object it
    // was handed; house+0x1dc is the CLSID it passes to CoCreateInstance. A
    // zero CLSID means the house was never initialised, and the generic
    // IUnknown handed back has no slot 3 -- the null call at 0x00630e2d.
    if (va == 0x00630cc0u) {
        ts::u32 type = ts::ld32(ESP.d + 4u);
        ts::u32 house = ts::ld32(ESP.d + 8u);
        char name[40];
        unsigned ni = 0;
        ts::u32 np = ts::ld32(type + 0x14u);
        if (np >= 0x00010000u && np < 0x80000000u)
            for (; ni + 1u < sizeof name; ++ni) {
                ts::u8 c = ts::ld8(np + ni);
                if (!c) break;
                name[ni] = (char)c;
            }
        name[ni] = 0;
        std::printf("  ctor: type=0x%08x vtable=0x%08x name=\"%s\" "
                    "clsid=%08x-%08x-%08x-%08x house=0x%08x\n",
                    type, (unsigned)ts::ld32(type), name,
                    (unsigned)ts::ld32(type + 0x1dcu),
                    (unsigned)ts::ld32(type + 0x1e0u),
                    (unsigned)ts::ld32(type + 0x1e4u),
                    (unsigned)ts::ld32(type + 0x1e8u), house);
        std::printf("    +0x10:");
        for (unsigned k = 0; k < 8u; ++k)
            std::printf(" %08x", (unsigned)ts::ld32(type + 0x10u + k * 4u));
        std::printf("\n    +0x1d0:");
        for (unsigned k = 0; k < 8u; ++k)
            std::printf(" %08x", (unsigned)ts::ld32(type + 0x1d0u + k * 4u));
        std::printf("\n");
    }
    if (va == 0x0061eb30u || va == 0x0063e480u) {
        std::printf("  clsid-check @0x%08x: obj=0x%08x +0x1dc=%08x-%08x-%08x-%08x\n",
                    va, (unsigned)ESI.d,
                    (unsigned)ts::ld32(ESI.d + 0x1dcu),
                    (unsigned)ts::ld32(ESI.d + 0x1e0u),
                    (unsigned)ts::ld32(ESI.d + 0x1e4u),
                    (unsigned)ts::ld32(ESI.d + 0x1e8u));
    }
    static const char *sections[] = {
        "Houses", "Units", "Infantry", "Structures", "Basic", "Terrain",
        "OverlayPack", "ScriptTypes"
    };
    std::printf("ini-at 0x%08x: object 0x%08x\n", va, obj);
    for (unsigned s = 0; s < sizeof sections / sizeof sections[0]; ++s) {
        ts::u32 n = ini_section_count(obj, sections[s]);
        std::printf("  [%-13s] %u", sections[s], n);
        char buf[128];
        for (unsigned i = 0; i < n && i < 2u; ++i) {
            ini_entry(obj, sections[s], i, buf, sizeof buf);
            std::printf("  %u=%s", i, buf);
        }
        std::printf("\n");
    }
    for (int i = 0; i < 8; ++i) m.r[i].d = save_r[i];
    ESP.d = save_esp; m.eip = save_eip; m.steps = save_steps;
    m.call_depth = save_depth; m.recent_call_n = save_rcn;
    m.cf = save_cf; m.pf = save_pf; m.af = save_af; m.zf = save_zf;
    m.sf = save_sf; m.df = save_df; m.of = save_of;
    g_ini_busy = 0;
}

void ini_lookup_report() {
    const char *list = std::getenv("TS_INI_LOOKUP");
    if (!list || !*list) return;
    ts::Machine &m = ts::mach();
    m.step_limit = m.steps + 20000000ul;
    m.call_depth_limit = 20000u;
    static const char *sections[] = {
        "Houses", "VehicleTypes", "InfantryTypes", "BuildingTypes",
        "Units", "Infantry", "Structures", "Basic"
    };
    std::printf("ini-lookup:\n");
    const char *p = list;
    while (*p) {
        char *end = 0;
        unsigned long addr = std::strtoul(p, &end, 0);
        if (end == p) break;
        ts::u32 slot = (ts::u32)addr;
        ts::u32 obj = ts::ld32(slot);
        if (obj < 0x00010000u || obj >= 0x80000000u) obj = slot;
        std::printf("  0x%08lx -> object 0x%08x\n", addr, obj);
        for (unsigned s = 0; s < sizeof sections / sizeof sections[0]; ++s) {
            ts::u32 n = ini_section_count(obj, sections[s]);
            std::printf("    [%-13s] %u entr%s", sections[s], n,
                        n == 1u ? "y" : "ies");
            char buf[128];
            for (unsigned i = 0; i < n && i < 3u; ++i) {
                ini_entry(obj, sections[s], i, buf, sizeof buf);
                std::printf("  %u=%s", i, buf);
            }
            std::printf("\n");
        }
        p = (*end == ',') ? end + 1 : end;
    }
}

// TS_STRTOK_TEST=1 -- after the run, drive the game's own tokenizer
// (FUN_006837ea, the CRT strtok the scenario readers use) over "Nod,BGGY,256".
// Its cursor lives in the C runtime's per-thread data, which is reached through
// TlsGetValue/TlsSetValue; a stub that does not store makes the second call
// start from a fresh zeroed block and return NULL. This answers that with the
// game's code instead of with a reimplementation.
void strtok_test_report() {
    if (!std::getenv("TS_STRTOK_TEST")) return;
    ts::Machine &m = ts::mach();
    m.step_limit = m.steps + 20000000ul;
    m.call_depth_limit = 20000u;
    const ts::u32 str_va = 0x00530000u, del_va = 0x00530100u;
    wr_cstr(str_va, "Nod,BGGY,256,55");
    wr_cstr(del_va, ",");
    std::printf("strtok-test:\n");
    for (int pass = 0; pass < 3; ++pass) {
        ESP.d = kStackTop;
        ts::push32(del_va);
        ts::push32(pass == 0 ? str_va : 0u);
        ts::ts_call_at(0x006837eau, 0x00fed200u);
        ts::u32 tok = EAX.d;
        char buf[64];
        unsigned i = 0;
        buf[0] = 0;
        if (tok >= 0x00010000u && tok < 0x80000000u) {
            for (; i + 1u < sizeof buf; ++i) {
                ts::u8 c = ts::ld8(tok + i);
                if (!c) break;
                buf[i] = (char)c;
            }
            buf[i] = 0;
        }
        std::printf("  call %d: 0x%08x \"%s\"\n", pass, tok, buf);
    }
}

// -- post-run cell/exec diagnostics -----------------------------------------
// TS_EXEC_CHECK=0xADDR,... -- did the run execute each address? The model keeps
// one bit per byte of .text, so this answers "was the terrain pass reached"
// without another instrumented build.
void exec_check_report() {
    const char *e = std::getenv("TS_EXEC_CHECK");
    if (!e || !*e) return;
    std::printf("exec-check:\n");
    const char *p = e;
    while (*p) {
        char *end = 0;
        unsigned long va = std::strtoul(p, &end, 0);
        if (end == p) break;
        bool hit = false;
        if (va >= kTextLo && va < kTextHi) {
            unsigned long off = va - kTextLo;
            hit = ((g_exec_map[off >> 3] >> (off & 7u)) & 1u) != 0u;
        }
        std::printf("  0x%08lx %s\n", va, hit ? "executed" : "NOT executed");
        p = (*end == ',') ? end + 1 : end;
        while (*p == ' ') ++p;
    }
}

// TS_MAPCELLS=1 -- find the map object by its cell array (the object whose
// +0x168 is a pointer array and +0x16c its length, the shape FUN_00502060
// indexes) and report the per-cell state the terrain and shroud passes read:
// +0x30 tile type (-1 = no tile), +0xa4 visibility flags (bit 0x4 seen,
// bit 0x20 fog), +0x24 land type, +0x28 height.
void mapcells_report() {
    const char *e = std::getenv("TS_MAPCELLS");
    if (!e || !*e) return;
    std::printf("mapcells:\n");
    std::printf("  tile-types 0x007b0b9c:");
    for (unsigned i = 0; i < 8; ++i)
        std::printf(" %u=0x%08x", i, (unsigned)ts::ld32(0x007b0b9cu + i * 4u));
    std::printf("\n");
    // The map is a static object at 0x710d28: FUN_005f8c00 loads exactly that
    // into ECX before `call 0x502060`, and its cell array pointer is 0x710e90.
    const u32 v = 0x00710d28u;
    u32 cells = ts::ld32(v + 0x168u);
    u32 n = ts::ld32(v + 0x16cu);
    unsigned w = (unsigned)ts::ld32(v + 0xfcu);
    unsigned h = (unsigned)ts::ld32(v + 0x100u);
    std::printf("  map obj=0x%08x cells=0x%08x n=%u w=%u h=%u "
                "view=(%d,%d) sel=%d\n",
                v, cells, n, w, h,
                (int)ts::ld32(v + 0x5cu), (int)ts::ld32(v + 0x60u),
                (int)ts::ld32(v + 0x94u));
    if (cells < 0x00400000u || cells >= 0x80000000u || (cells & 3u)) {
        std::printf("  cell array pointer is not a model pointer\n");
        return;
    }
    // The cell array is a 512x512 pointer grid (n=262144) whatever +0xfc/+0x100
    // say: this map's objects sit at cell (84,54), so a 48x63 window counts a
    // fraction of the map and misses revealed cells entirely. Scan the array.
    (void)w; (void)h;
    w = 512u; h = 512u;
    unsigned long alloc = 0, tiles = 0, seen = 0, fog = 0, nonzero = 0;
    unsigned ax0 = 512u, ay0 = 512u, ax1 = 0u, ay1 = 0u;
    unsigned sx0 = 512u, sy0 = 512u, sx1 = 0u, sy1 = 0u;
    for (unsigned y = 0; y < h; ++y) {
        for (unsigned x = 0; x < w; ++x) {
            u32 c = ts::ld32(cells + (y * 512u + x) * 4u);
            if (!c) continue;
            ++alloc;
            if (x < ax0) ax0 = x;
            if (y < ay0) ay0 = y;
            if (x > ax1) ax1 = x;
            if (y > ay1) ay1 = y;
            u32 t = ts::ld32(c + 0x30u), f = ts::ld32(c + 0xa4u);
            if (t != 0xffffffffu) ++tiles;
            if (f & 4u) {
                ++seen;
                if (x < sx0) sx0 = x;
                if (y < sy0) sy0 = y;
                if (x > sx1) sx1 = x;
                if (y > sy1) sy1 = y;
            }
            if (f & 0x20u) ++fog;
            if (f) ++nonzero;
        }
    }
    std::printf("    alloc=%lu tiles=%lu seen=%lu fog=%lu flags_nonzero=%lu\n",
                alloc, tiles, seen, fog, nonzero);
    if (alloc)
        std::printf("    alloc bbox=(%u,%u)-(%u,%u)\n", ax0, ay0, ax1, ay1);
    if (seen)
        std::printf("    seen bbox=(%u,%u)-(%u,%u)\n", sx0, sy0, sx1, sy1);
    // The seen-cell set itself, not just its size: "34 cells" at the bottom
    // edge of the map is a coordinate question, and a sample of six fixed
    // cells cannot answer it. Bounded so a fully revealed map stays readable.
    if (seen && seen <= 512u) {
        std::printf("    seen:");
        unsigned shown = 0;
        for (unsigned y = 0; y < h && shown < 512u; ++y) {
            for (unsigned x = 0; x < w && shown < 512u; ++x) {
                u32 c = ts::ld32(cells + (y * 512u + x) * 4u);
                if (!c) continue;
                if (!(ts::ld32(c + 0xa4u) & 4u)) continue;
                std::printf(" (%u,%u)", x, y);
                ++shown;
            }
        }
        std::printf("\n");
    }
    static const unsigned sx[6] = { 0u, 24u, 47u, 10u, 28u, 40u };
    static const unsigned sy[6] = { 0u, 31u, 62u, 10u, 59u, 68u };
    for (unsigned i = 0; i < 6u; ++i) {
        u32 c = ts::ld32(cells + (sy[i] * 512u + sx[i]) * 4u);
        std::printf("    cell(%u,%u)=0x%08x", sx[i], sy[i], c);
        if (c) {
            std::printf(" tile=%d flags=0x%08x land=%d height=%d occ=0x%08x",
                        (int)ts::ld32(c + 0x30u), (unsigned)ts::ld32(c + 0xa4u),
                        (int)ts::ld32(c + 0x24u), (int)ts::ld32(c + 0x28u),
                        (unsigned)ts::ld32(c + 0x3cu));
        }
        std::printf("\n");
    }
}

// TS_OBJECTS=1 -- the live object list the main loop walks (0x7b2d98 count,
// 0x7b2d8c array). An empty list after mission start means the mission placed
// no units, which is also why no cell would be revealed.
void objects_report() {
    const char *e = std::getenv("TS_OBJECTS");
    if (!e || !*e) return;
    u32 n = ts::ld32(0x007b2d98u);
    u32 a = ts::ld32(0x007b2d8cu);
    std::printf("objects: n=%u array=0x%08x\n", (unsigned)n, a);
    // 0x7b2d98 is a per-frame list the renderer walks, so it can legitimately
    // be empty at an arbitrary stop. The world itself lives in the class
    // registries: ObjectClass (0x77bbe0), InfantryClass (0x7b0c08),
    // BuildingClass (0x7b2c48) and the live houses (0x7afce0).
    std::printf("  world: objectclass=%u infantry=%u buildings=%u houses=%u\n",
                (unsigned)ts::ld32(0x0077bbe0u), (unsigned)ts::ld32(0x007b0c08u),
                (unsigned)ts::ld32(0x007b2c48u), (unsigned)ts::ld32(0x007afce0u));
    // Where each world object actually is. The reveal pass walks these
    // registries and reveals around +0x14, so a wrong packed position here is
    // exactly what puts the revealed patch in the wrong place.
    {
        static const struct { const char *name; u32 count_va, array_va; } regs[3] = {
            { "objectclass", 0x0077bbe0u, 0x0077bbb4u },
            { "infantry",    0x007b0c08u, 0x007b0c2cu },
            { "buildings",   0x007b2c48u, 0x007b2c3cu },
        };
        for (unsigned r = 0; r < 3u; ++r) {
            u32 cnt = ts::ld32(regs[r].count_va);
            u32 arr = ts::ld32(regs[r].array_va);
            std::printf("  %s: n=%u array=0x%08x\n", regs[r].name,
                        (unsigned)cnt, arr);
            if (arr < 0x00400000u || arr >= 0x80000000u) continue;
            unsigned lim = cnt < 64u ? (unsigned)cnt : 64u;
            for (unsigned i = 0; i < lim; ++i) {
                u32 o = ts::ld32(arr + i * 4u);
                std::printf("    [%u] obj=0x%08x", i, o);
                if (o >= 0x00400000u && o < 0x80000000u) {
                    // ObjectClass +0x40 is the 12-byte CoordStruct (three ints,
                    // 256 leptons per cell); +0x14 is m_NextObject, not a
                    // position, so reading it as two shorts prints heap
                    // pointers. The reveal pass converts Coord>>8 to a cell.
                    int cx = (int)ts::ld32(o + 0x40u);
                    int cy = (int)ts::ld32(o + 0x44u);
                    int cz = (int)ts::ld32(o + 0x48u);
                    std::printf(" vptr=0x%08x coord=(%d,%d,%d) cell=(%d,%d)",
                                (unsigned)ts::ld32(o), cx, cy, cz,
                                cx >> 8, cy >> 8);
                }
                std::printf("\n");
            }
        }
    }
    if (a < 0x00400000u || a >= 0x80000000u) return;
    unsigned show = n < 12u ? (unsigned)n : 12u;
    for (unsigned i = 0; i < show; ++i) {
        u32 o = ts::ld32(a + i * 4u);
        std::printf("  [%u] obj=0x%08x", i, o);
        if (o >= 0x00400000u && o < 0x80000000u) {
            // +0x14 is the packed position the renderer reads (FUN_005f5eb0);
            // its two shorts are the cell x and y.
            u32 pos = ts::ld32(o + 0x14u);
            std::printf(" vptr=0x%08x pos=(%d,%d) flags=0x%08x",
                        (unsigned)ts::ld32(o),
                        (int)(short)(pos & 0xffffu), (int)(short)(pos >> 16),
                        (unsigned)ts::ld32(o + 0x0cu));
        }
        std::printf("\n");
    }
}

// -- arguments --------------------------------------------------------------
bool starts_with(const char *s, const char *p) {
    return std::strncmp(s, p, std::strlen(p)) == 0;
}

bool parse_args(int argc, char **argv, Options &o) {
    for (int i = 1; i < argc; ++i) {
        const char *a = argv[i];
        if (starts_with(a, "--image="))         o.image = a + 8;
        else if (starts_with(a, "--frame="))    o.frame = a + 8;
        else if (starts_with(a, "--surface-dump=")) o.surface_dump = a + 15;
        else if (!std::strcmp(a, "--surfaces")) o.surfaces = true;
        else if (starts_with(a, "--budget="))   o.budget = std::strtoull(a + 9, 0, 10);
        else if (starts_with(a, "--timeout="))  o.wall_seconds = (unsigned)std::strtoul(a + 10, 0, 10);
        else if (starts_with(a, "--depth="))    o.depth_limit = std::strtoul(a + 8, 0, 10);
        else if (starts_with(a, "--imports="))  o.last_imports = (unsigned)std::strtoul(a + 10, 0, 10);
        else if (starts_with(a, "--calls="))    o.last_calls = (unsigned)std::strtoul(a + 8, 0, 10);
        else if (!std::strcmp(a, "--verbose"))  o.verbose = true;
        else if (!std::strcmp(a, "--help") || !std::strcmp(a, "-h")) {
            std::printf("usage: boot_driver [--image=PATH] [--budget=N] "
                        "[--timeout=SECONDS] [--depth=N] [--imports=N] [--calls=N] "
                        "[--frame=PATH] [--surfaces] [--surface-dump=PREFIX] "
                        "[--verbose]\n");
            return false;
        } else {
            std::fprintf(stderr, "boot_driver: unknown argument %s\n", a);
            return false;
        }
    }
    if (o.last_imports > kEventRing) o.last_imports = kEventRing;
    if (o.last_calls > 8) o.last_calls = 8;
    return true;
}

}  // namespace

int main(int argc, char **argv) {
    Options opt;
    if (!parse_args(argc, argv, opt)) return 1;
    g_verbose = opt.verbose;
    // TS_FRAME_DUMP is the environment form; --frame wins when both are given.
    if (!opt.frame || !*opt.frame) {
        const char *env = std::getenv("TS_FRAME_DUMP");
        if (env && *env) opt.frame = env;
    }

    ts::register_all_recovered();

    if (!ts::load_image(opt.image)) {
        std::fprintf(stderr, "boot_driver: cannot load %s\n", opt.image);
        return 2;
    }
    ts::resolve_imports();
    bind_traced_imports();

    ts::Machine &m = ts::mach();
    if (const char *t = std::getenv("TS_TRACE")) m.trace_va = (ts::u32)std::strtoul(t, 0, 0);
    // TS_PROBE=0xADDR,... -- record registers and the EAX/ECX strings at up to
    // eight addresses. The creation path is a chain of decisions, and one run
    // can read the whole chain.
    if (const char *pr = std::getenv("TS_PROBE")) {
        const char *p = pr;
        while (*p && m.probe_n < (unsigned)ts::Machine::PROBE_MAX) {
            char *end = 0;
            unsigned long va = std::strtoul(p, &end, 0);
            if (end == p) break;
            ts::u32 pv = (ts::u32)va;
            m.probe_va[m.probe_n++] = pv;
            ts::u32 pb = ts::probe_bit(pv);
            m.probe_filter[pb >> 5] |= (ts::u32)1u << (pb & 31u);
            p = (*end == ',') ? end + 1 : end;
        }
    }
    if (const char *ia = std::getenv("TS_INI_AT")) {
        const char *p = ia;
        while (*p && g_ini_at_n < 8u) {
            char *end = 0;
            unsigned long va = std::strtoul(p, &end, 0);
            if (end == p) break;
            g_ini_at[g_ini_at_n++] = (ts::u32)va;
            p = (*end == ',') ? end + 1 : end;
        }
        m.probe_hook = &ini_probe_hook;
    }
    if (const char *w = std::getenv("TS_WATCH")) m.watch_va = (ts::u32)std::strtoul(w, 0, 0);
    if (std::getenv("TS_WATCH_ZERO")) m.watch_zero = 1;
    if (const char *wr = std::getenv("TS_WATCH_RING")) {
        const char *p = wr;
        while (*p && m.watch_ring_n < (unsigned)ts::Machine::WATCH_RING_ADDRS) {
            char *end = 0;
            unsigned long va = std::strtoul(p, &end, 0);
            if (end == p) break;
            m.watch_ring_va[m.watch_ring_n++] = (ts::u32)va;
            p = (*end == ',') ? end + 1 : end;
        }
    }
    // Diagnostics that stop at the FIRST defect instead of at the flood it
    // causes. TS_STOP_UNMAPPED halts at the first unregistered target with the
    // registers at the call site; TS_ESP_GUARD records the instruction that
    // leaves the stack region and TS_ESP_STOP halts there.
    if (std::getenv("TS_STOP_UNMAPPED")) m.stop_on_unmapped = 1;
    if (const char *wg = std::getenv("TS_WRITE_GUARD")) {
        char *end = 0;
        unsigned long eip = std::strtoul(wg, &end, 0);
        if (end != wg) {
            m.write_guard_eip = (ts::u32)eip;
            const char *p = (*end == ',') ? end + 1 : end;
            m.write_guard_ptr_addr = (ts::u32)std::strtoul(p, 0, 0);
        }
    }
    if (std::getenv("TS_WRITE_GUARD_STOP")) m.write_guard_stop = 1;
    if (std::getenv("TS_ESP_GUARD")) m.esp_guard = 1;
    if (std::getenv("TS_ESP_STOP")) { m.esp_guard = 1; m.esp_guard_stop = 1; }
    m.text_lo = kTextLo;
    m.text_hi = kTextHi;
    m.exec_map = g_exec_map; m.exec_map_lo = kTextLo; m.exec_map_hi = kTextHi;
    m.func_map = g_func_map; m.func_map_lo = kTextLo; m.func_map_hi = kTextHi;
    m.r[4].d = kStackTop;                  // ESP: a plausible stack, not zero
    ts::push32(kSentinel);                 // what the entry point's final `ret` pops
    m.esp_prev = m.r[4].d;                 // no delta on the first instruction
    const unsigned long esp0 = m.r[4].d;
    m.call_depth_limit = opt.depth_limit;
    m.step_limit = opt.budget;

    jmp_buf halt;
    g_halt = &halt;
    g_timed_out = false;
    m.halt = &halt;                        // stop AT the fault, machine intact
    std::signal(SIGALRM, on_watchdog);
    watchdog_start(opt.wall_seconds);

    // volatile: setjmp/longjmp makes a non-volatile local modified after
    // setjmp indeterminate when it is read after the longjmp. At -O0 this
    // happened to live in memory; at -O2 it is a register and the report would
    // print the wrong answer.
    volatile bool returned = false;
    std::fflush(stdout);
    if (setjmp(halt) == 0) {
        ts::ts_call(kEntryPoint);
        returned = true;
    }
    watchdog_stop();
    m.halt = 0;
    g_halt = 0;

    // Dump the primary surface as the run left it. This is after the halt (a
    // fault, a budget stop or the watchdog), so it reads the frame the game was
    // in the middle of drawing rather than a clean shutdown it never reached.
    if (opt.frame && *opt.frame) {
        const ts::rnd::Surface *s = ts::rnd::primary();
        g_frame_surfaces = ts::rnd::surface_n();
        g_frame_ok = ts::rnd::dump_primary_png(opt.frame);
        if (s) {
            g_frame_w = s->width;
            g_frame_h = s->height;
            g_frame_nonblack = ts::rnd::nonblack_pixels(s);
        }
    }
    // One PNG per surface, so "the terrain went to an offscreen nobody blits"
    // can be read directly instead of inferred from counters.
    if (opt.surface_dump && *opt.surface_dump) {
        unsigned n = ts::rnd::surface_n();
        for (unsigned i = 0; i < n; ++i) {
            char path[512];
            std::snprintf(path, sizeof path, "%s%u.png", opt.surface_dump, i);
            ts::rnd::dump_surface_png(i, path);
        }
        std::printf("surface-dump: %u file(s) at %s<i>.png\n", n, opt.surface_dump);
    }

    exec_check_report();
    objects_report();
    mapcells_report();
    mix_lookup_report();
    ini_lookup_report();
    strtok_test_report();
    print_report(opt, returned, esp0);
    std::fflush(stdout);

    // The measurement is the report. Only a defect in this tree is a failure.
    if (m.unmapped_in_text) {
        std::fprintf(stderr, "boot_driver: %lu unmapped calls inside .text: a "
                             "recovered function nothing registered\n", m.unmapped_in_text);
        return 3;
    }
    return 0;
}
