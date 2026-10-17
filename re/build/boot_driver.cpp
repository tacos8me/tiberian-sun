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

// -- the run ----------------------------------------------------------------
struct Options {
    const char *image;
    unsigned long budget;
    unsigned long depth_limit;
    unsigned wall_seconds;
    unsigned last_imports;
    unsigned last_calls;
    bool verbose;
    Options()
        : image("bin/GAME.EXE"), budget(20000000ul), depth_limit(20000ul),
          wall_seconds(60u), last_imports(12u), last_calls(8u), verbose(false) {}
};

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
    std::printf("budget: %lu\n", o.budget);
    std::printf("depth-limit: %lu\n", o.depth_limit);
    std::printf("watchdog: %u\n", o.wall_seconds);
    std::printf("stop: %s\n", stop);
    std::printf("steps: %lu\n", m.steps);
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
    if (m.bad_calls) {
        std::printf("calls outside .text (not a stub, not code): %u\n", m.bad_calls);
        for (unsigned i = 0; i < m.bad_call_n; ++i)
            std::printf("  call 0x%08x from 0x%08x\n",
                        m.bad_call_to[i], m.bad_call_from[i]);
    }
    std::printf("unmodelled: %lu\n", m.unmodelled_insns);
    std::printf("import-calls: %lu\n", g_import_calls);
    std::printf("distinct-imports: %lu\n", distinct_imports());
    print_message_loop();
    print_last_calls(o.last_calls);
    print_last_imports(o.last_imports);
    print_import_counts(12);
    if (m.faulted) {
        std::printf("fault: %s\n", m.fault_why ? m.fault_why : "(unrecorded)");
        std::printf("fault-eip: 0x%08x\n", m.fault_eip);
        std::printf("fault-bytes: ");
        print_bytes(m.fault_eip);
        std::printf("\n");
        std::printf("fault-steps: %lu\n", m.fault_steps);
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
        std::printf("watchdog-steps: %lu\n", m.steps);
    }
    if (m.unmapped_sample_n) {
        std::printf("unmapped-sample:");
        for (unsigned i = 0; i < m.unmapped_sample_n; ++i)
            std::printf(" 0x%08x", m.unmapped_sample[i]);
        std::printf("\n");
    }
    std::printf("boot-report: end\n");
}

// -- arguments --------------------------------------------------------------
bool starts_with(const char *s, const char *p) {
    return std::strncmp(s, p, std::strlen(p)) == 0;
}

bool parse_args(int argc, char **argv, Options &o) {
    for (int i = 1; i < argc; ++i) {
        const char *a = argv[i];
        if (starts_with(a, "--image="))         o.image = a + 8;
        else if (starts_with(a, "--budget="))   o.budget = std::strtoul(a + 9, 0, 10);
        else if (starts_with(a, "--timeout="))  o.wall_seconds = (unsigned)std::strtoul(a + 10, 0, 10);
        else if (starts_with(a, "--depth="))    o.depth_limit = std::strtoul(a + 8, 0, 10);
        else if (starts_with(a, "--imports="))  o.last_imports = (unsigned)std::strtoul(a + 10, 0, 10);
        else if (starts_with(a, "--calls="))    o.last_calls = (unsigned)std::strtoul(a + 8, 0, 10);
        else if (!std::strcmp(a, "--verbose"))  o.verbose = true;
        else if (!std::strcmp(a, "--help") || !std::strcmp(a, "-h")) {
            std::printf("usage: boot_driver [--image=PATH] [--budget=N] "
                        "[--timeout=SECONDS] [--depth=N] [--imports=N] [--calls=N] "
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

    ts::register_all_recovered();

    if (!ts::load_image(opt.image)) {
        std::fprintf(stderr, "boot_driver: cannot load %s\n", opt.image);
        return 2;
    }
    ts::resolve_imports();
    bind_traced_imports();

    ts::Machine &m = ts::mach();
    m.text_lo = kTextLo;
    m.text_hi = kTextHi;
    m.r[4].d = kStackTop;                  // ESP: a plausible stack, not zero
    ts::push32(kSentinel);                 // what the entry point's final `ret` pops
    const unsigned long esp0 = m.r[4].d;
    m.call_depth_limit = opt.depth_limit;
    m.step_limit = opt.budget;

    jmp_buf halt;
    g_halt = &halt;
    g_timed_out = false;
    m.halt = &halt;                        // stop AT the fault, machine intact
    std::signal(SIGALRM, on_watchdog);
    watchdog_start(opt.wall_seconds);

    bool returned = false;
    std::fflush(stdout);
    if (setjmp(halt) == 0) {
        ts::ts_call(kEntryPoint);
        returned = true;
    }
    watchdog_stop();
    m.halt = 0;
    g_halt = 0;

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
