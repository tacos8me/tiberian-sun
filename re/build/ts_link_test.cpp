// ts_link_test.cpp -- link the whole recovered tree, boot the original image
// inside the model, and report what the game actually did.
//
// The generated tree is 41,822 transcriptions of recovered machine functions
// plus ~7,000 declaration-shaped forwarders. Compiling each file proves the
// syntax; it proves nothing about whether the tree LINKS, and a forwarder that
// names a transcription nothing defines is exactly the kind of breakage a
// per-file syntax check cannot see. So this links everything together and then
// drives it:
//
//   1. register_all_recovered()   -- every transcription into the dispatch table
//   2. a sample lookup            -- dispatch resolves a VA to a body
//   3. load_image("bin/GAME.EXE") -- the image's sections at their real VAs
//   4. register_win32_stubs()     -- a modelled body for every import
//   5. resolve_imports()          -- a stub VA written into every IAT slot
//   6. run the real PE entry point (0x6854e3) against the model
//
// Steps 3-5 are what make `call dword ptr [IAT]` do something other than read
// zero. They come from two sibling headers -- Image.h and Win32Stubs.h -- which
// land independently of this file, so each is used when present and a minimal,
// clearly-marked fallback keeps the boot observable when it is not.
//
// WHAT IS ASSERTED, AND WHAT IS ONLY REPORTED. The link and the dispatch table
// are the gate: a sampled VA that does not resolve exits non-zero. The
// entry-point run is a MEASUREMENT -- the CRT wants a Windows heap, a file
// system and 341 kernel32/user32/gdi32/... entry points and the model has stubs
// for those, not implementations -- so a fault or a truncation is reported in
// full and still exits 0. What does exit non-zero is an unmapped call to an
// address INSIDE .text: that is a recovered function nothing registered, which
// is a defect in this tree rather than a missing Windows.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <setjmp.h>
#include <csignal>
#include <unistd.h>

#include "Machine.h"
#include "code/recovered.h"

#if defined(__has_include)
#  if __has_include("Image.h")
#    include "Image.h"
#    define TS_LINK_HAVE_IMAGE 1
#  endif
#  if __has_include("Win32Stubs.h")
#    include "Win32Stubs.h"
#    define TS_LINK_HAVE_STUBS 1
#  endif
#endif

namespace {

typedef ts::u32 u32;
typedef ts::u8  u8;

const u32 kEntryPoint = 0x006854e3u;   // PE entry point of bin/GAME.EXE
const u32 kImageBase  = 0x00400000u;
const u32 kTextLo     = 0x00401000u;   // .text: an unmapped call in here is a
const u32 kTextHi     = 0x00697225u;   // recovered function nothing registered
const u32 kStackTop   = 0x00300000u;   // modelled ESP at boot
const u32 kSentinel   = 0xdeadbeefu;   // return address the entry point pops
const u32 kWrapperBase = 0x00a00000u;  // traced import thunks
const u32 kWrapperStep = 16u;
const unsigned kMaxImports = 512;      // the image has 341
const unsigned kTraceMax   = 64;       // first N stub calls, in order

// A handful of VAs from across .text, to show dispatch resolves more than the
// first entry in the table.
const u32 kSample[] = {
    0x00401000u, 0x00405790u, 0x00445640u, 0x006829a5u, 0x006854e3u,
};

unsigned long g_image_bytes = 0;
unsigned      g_image_sections = 0;
bool          g_returned = false;

// The watchdog: a transcription can spin without ever reaching the next
// instruction (one `rep` with a garbage count is a single instruction), so an
// instruction budget alone does not bound the run. SIGALRM does, and the report
// then names the instruction the run was stopped at.
bool     g_timed_out = false;
jmp_buf *g_halt = 0;

void on_watchdog(int) {
    g_timed_out = true;
    if (g_halt) longjmp(*g_halt, 2);   // POSIX lists longjmp as signal-safe
}

// ---------------------------------------------------------------------------
// A minimal read-only view of the target PE.
//
// It exists so this file can map the image and name its imports even when the
// sibling loader has not landed, and so the import list is available for the
// per-name call counts either way. It reads; it does not relocate.
struct PeFile {
    unsigned char *buf;
    unsigned long  len;
    u32 base;
    unsigned nsec;
    unsigned long sec_off;
    unsigned long opt_off;

    PeFile() : buf(0), len(0), base(0), nsec(0), sec_off(0), opt_off(0) {}
    ~PeFile() { std::free(buf); }

    static u32 p32(const unsigned char *p) {
        return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
    }
    u32 r32(unsigned long off) const { return p32(buf + off); }

    bool open(const char *path);
    long rva_to_off(u32 rva) const;
};

bool PeFile::open(const char *path) {
    std::FILE *f = std::fopen(path, "rb");
    if (!f) return false;
    std::fseek(f, 0, SEEK_END);
    len = (unsigned long)std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    buf = (unsigned char *)std::malloc(len ? len : 1);
    if (!buf) { std::fclose(f); return false; }
    if (std::fread(buf, 1, len, f) != len) { std::fclose(f); return false; }
    std::fclose(f);
    if (len < 0x100 || buf[0] != 'M' || buf[1] != 'Z') return false;
    unsigned long e = r32(0x3c);
    if (e + 0x40 > len || std::memcmp(buf + e, "PE\0\0", 4) != 0) return false;
    unsigned long coff = e + 4;
    nsec = (unsigned)buf[coff + 2] | ((unsigned)buf[coff + 3] << 8);
    unsigned opt_size = (unsigned)buf[coff + 16] | ((unsigned)buf[coff + 17] << 8);
    opt_off = coff + 20;
    if (opt_off + 0x60 > len) return false;
    base = r32(opt_off + 28);
    sec_off = opt_off + opt_size;
    if (sec_off + 40ul * nsec > len) return false;
    return true;
}

long PeFile::rva_to_off(u32 rva) const {
    for (unsigned i = 0; i < nsec; ++i) {
        const unsigned char *s = buf + sec_off + 40ul * i;
        u32 vsize = p32(s + 8), va = p32(s + 12);
        u32 rawsize = p32(s + 16), rawptr = p32(s + 20);
        u32 span = vsize > rawsize ? vsize : rawsize;
        if (rva >= va && rva < va + span) {
            u32 off = rva - va;
            if (off >= rawsize) return -1;          // in .bss: zero, not file
            if ((unsigned long)(rawptr + off) >= len) return -1;
            return (long)(rawptr + off);
        }
    }
    return -1;
}

// ---------------------------------------------------------------------------
// The import table, by name. `resolve_imports()` writes a stub VA into each of
// these IAT slots; this list is what turns a hit back into "KERNEL32!HeapAlloc".
struct ImportSlot { u32 iat; char dll[20]; char fn[56]; };
ImportSlot g_imp[kMaxImports];
unsigned   g_imp_n = 0;
unsigned   g_dlls = 0;

void copy_name(char *dst, unsigned cap, const unsigned char *src) {
    unsigned i = 0;
    while (i + 1 < cap && src[i]) { dst[i] = (char)src[i]; ++i; }
    dst[i] = 0;
}

bool collect_imports(const PeFile &pe) {
    u32 imp_rva = pe.r32(pe.opt_off + 96 + 8);          // data dir 1 = imports
    if (!imp_rva) return false;
    long o = pe.rva_to_off(imp_rva);
    if (o < 0) return false;
    char last[20] = "";
    while (g_imp_n < kMaxImports) {
        if ((unsigned long)o + 20 > pe.len) break;
        const unsigned char *d = pe.buf + o;
        u32 oft = PeFile::p32(d), name_rva = PeFile::p32(d + 12), ft = PeFile::p32(d + 16);
        if (!oft && !name_rva && !ft) break;
        long no = pe.rva_to_off(name_rva);
        long to = pe.rva_to_off(oft ? oft : ft);
        if (no < 0 || to < 0) break;
        char dll[20];
        copy_name(dll, sizeof dll, pe.buf + no);
        if (std::strcmp(dll, last) != 0) { ++g_dlls; std::strcpy(last, dll); }
        for (unsigned i = 0; g_imp_n < kMaxImports; ++i) {
            if ((unsigned long)to + 4ul * i + 4 > pe.len) break;
            u32 t = PeFile::p32(pe.buf + to + 4ul * i);
            if (!t) break;
            ImportSlot &s = g_imp[g_imp_n];
            s.iat = pe.base + ft + 4ul * i;
            std::strcpy(s.dll, dll);
            if (t & 0x80000000u) {
                std::sprintf(s.fn, "ord#%u", (unsigned)(t & 0xffffu));
            } else {
                long x = pe.rva_to_off(t);
                if (x < 0) break;
                copy_name(s.fn, sizeof s.fn, pe.buf + x + 2);
            }
            ++g_imp_n;
        }
        o += 20;
    }
    return g_imp_n > 0;
}

}

// The fallbacks below must be at GLOBAL scope, not inside the anonymous
// namespace: Machine.h's register macros expand to `ts::mach()`, and a nested
// `{anonymous}::ts` is a different namespace that the macros would not find.
//
// ---------------------------------------------------------------------------
// FALLBACK image loader: compiled only when the sibling Image.h is absent.
#if !defined(TS_LINK_HAVE_IMAGE)
namespace ts {

void map_block(u32 va, const unsigned char *src, u32 n) {
    u32 done = 0;
    while (done < n) {                      // one page() per page, not per byte
        u32 chunk = 4096u - ((va + done) & 0xfffu);
        if (chunk > n - done) chunk = n - done;
        std::memcpy(page(va + done), src + done, chunk);
        done += chunk;
    }
}

bool load_image(const char *path) {
    PeFile pe;
    if (!pe.open(path)) return false;
    for (unsigned i = 0; i < pe.nsec; ++i) {
        const unsigned char *s = pe.buf + pe.sec_off + 40ul * i;
        u32 vsize = PeFile::p32(s + 8), va = PeFile::p32(s + 12);
        u32 rawsize = PeFile::p32(s + 16), rawptr = PeFile::p32(s + 20);
        u32 n = vsize < rawsize ? vsize : rawsize;
        if ((unsigned long)rawptr + n > pe.len) n = (u32)(pe.len - rawptr);
        map_block(pe.base + va, pe.buf + rawptr, n);
        g_image_bytes += n;
        ++g_image_sections;
    }
    return true;
}

}  // namespace ts
#endif

// ---------------------------------------------------------------------------
// FALLBACK Win32 stubs: compiled only when the sibling Win32Stubs.h is absent.
//
// These are deliberately crude: every unknown import returns 0 in EAX and pops
// no arguments, so ESP drifts by 4 bytes per argument per call. Recovering the
// real arity and the real return values is the other lane's job; this exists so
// that the run is observable now. The named ones are the handful the MSVC 6 CRT
// startup demonstrably needs to get past its own prologue.
#if !defined(TS_LINK_HAVE_STUBS)
namespace ts {

u32         g_stub_calls = 0;
const char *g_stub_last = "(none)";
u32         g_heap_ptr = 0x20000000u;
bool        g_exit_called = false;
unsigned long g_written = 0;

void stub_note(const char *nm) { ++g_stub_calls; g_stub_last = nm; }
void st_ret() { ESP.d += 4; }                       // pop the return address
void st_ret_n(u32 n) { ESP.d += 4u * (n + 1u); }    // __stdcall: pop args too
u32  st_arg(u32 i) { return ld32(ESP.d + 4u + 4u * i); }
u32  st_alloc(u32 n) {
    if (n == 0) n = 16;
    n = (n + 15u) & ~15u;
    u32 p = g_heap_ptr;
    g_heap_ptr += n + 16u;
    return p;
}
void st_str(u32 at, const char *s) {
    for (unsigned i = 0;; ++i) { st8(at + i, (u8)s[i]); if (!s[i]) break; }
}

void st_GetVersion()     { stub_note("GetVersion"); EAX.d = 0x05650400u; st_ret(); }
void st_GetCommandLineA(){ stub_note("GetCommandLineA"); st_str(0x00b00000u, "GAME.EXE");
                           EAX.d = 0x00b00000u; st_ret(); }
void st_GetStartupInfoA(){ stub_note("GetStartupInfoA"); st32(st_arg(0), 0x44u);
                           EAX.d = 0; st_ret_n(1); }
void st_GetModuleHandleA(){ stub_note("GetModuleHandleA"); EAX.d = 0x00400000u; st_ret_n(1); }
void st_GetModuleFileNameA(){ stub_note("GetModuleFileNameA"); st_str(st_arg(1), "GAME.EXE");
                           EAX.d = 8; st_ret_n(3); }
void st_GetProcessHeap() { stub_note("GetProcessHeap"); EAX.d = 0x00500000u; st_ret(); }
void st_HeapCreate()     { stub_note("HeapCreate"); EAX.d = 0x00500000u; st_ret_n(3); }
void st_HeapAlloc()      { stub_note("HeapAlloc"); EAX.d = st_alloc(st_arg(2)); st_ret_n(3); }
void st_HeapReAlloc()    { stub_note("HeapReAlloc"); EAX.d = st_alloc(st_arg(3)); st_ret_n(4); }
void st_HeapFree()       { stub_note("HeapFree"); EAX.d = 1; st_ret_n(3); }
void st_HeapDestroy()    { stub_note("HeapDestroy"); EAX.d = 1; st_ret_n(1); }
void st_VirtualAlloc()   { stub_note("VirtualAlloc"); EAX.d = st_alloc(st_arg(1)); st_ret_n(4); }
void st_LocalAlloc()     { stub_note("LocalAlloc"); EAX.d = st_alloc(st_arg(1)); st_ret_n(2); }
void st_GetLastError()   { stub_note("GetLastError"); EAX.d = 0; st_ret(); }
void st_SetLastError()   { stub_note("SetLastError"); EAX.d = 0; st_ret_n(1); }
void st_GetStdHandle()   { stub_note("GetStdHandle"); EAX.d = 7; st_ret_n(1); }
void st_GetProcAddress() { stub_note("GetProcAddress"); EAX.d = 0; st_ret_n(2); }
void st_SetUnhandledExceptionFilter() {
    stub_note("SetUnhandledExceptionFilter"); EAX.d = 0; st_ret_n(1);
}
void st_ExitProcess()    { stub_note("ExitProcess"); g_exit_called = true; EAX.d = 0; st_ret_n(1); }
void st_WriteFile() {                 // the game's own output becomes visible
    stub_note("WriteFile");
    u32 buf = st_arg(1), n = st_arg(2), out = st_arg(3);
    if (n > 4096u) n = 4096u;
    for (u32 i = 0; i < n && g_written < 32768ul; ++i, ++g_written)
        std::fputc(ld8(buf + i), stdout);
    std::fflush(stdout);
    if (out) st32(out, n);
    EAX.d = 1;
    st_ret_n(4);
}
void st_GetEnvironmentStrings()  { stub_note("GetEnvironmentStrings"); EAX.d = 0x00b01000u; st_ret(); }
void st_GetEnvironmentStringsW() { stub_note("GetEnvironmentStringsW"); EAX.d = 0x00b01000u; st_ret(); }
void st_FreeEnvironmentStringsA(){ stub_note("FreeEnvironmentStringsA"); EAX.d = 1; st_ret_n(1); }
void st_FreeEnvironmentStringsW(){ stub_note("FreeEnvironmentStringsW"); EAX.d = 1; st_ret_n(1); }
void st_fallback() { stub_note("(unmodelled import)"); EAX.d = 0; st_ret(); }

struct StubEntry { const char *dll; const char *fn; Fn f; };
const StubEntry kStubs[] = {
    { "KERNEL32.dll", "GetVersion",                  &st_GetVersion },
    { "KERNEL32.dll", "GetCommandLineA",             &st_GetCommandLineA },
    { "KERNEL32.dll", "GetStartupInfoA",             &st_GetStartupInfoA },
    { "KERNEL32.dll", "GetModuleHandleA",            &st_GetModuleHandleA },
    { "KERNEL32.dll", "GetModuleFileNameA",          &st_GetModuleFileNameA },
    { "KERNEL32.dll", "GetProcessHeap",              &st_GetProcessHeap },
    { "KERNEL32.dll", "HeapCreate",                  &st_HeapCreate },
    { "KERNEL32.dll", "HeapAlloc",                   &st_HeapAlloc },
    { "KERNEL32.dll", "HeapReAlloc",                 &st_HeapReAlloc },
    { "KERNEL32.dll", "HeapFree",                    &st_HeapFree },
    { "KERNEL32.dll", "HeapDestroy",                 &st_HeapDestroy },
    { "KERNEL32.dll", "VirtualAlloc",                &st_VirtualAlloc },
    { "KERNEL32.dll", "LocalAlloc",                  &st_LocalAlloc },
    { "KERNEL32.dll", "GetLastError",                &st_GetLastError },
    { "KERNEL32.dll", "SetLastError",                &st_SetLastError },
    { "KERNEL32.dll", "GetStdHandle",                &st_GetStdHandle },
    { "KERNEL32.dll", "GetProcAddress",              &st_GetProcAddress },
    { "KERNEL32.dll", "SetUnhandledExceptionFilter", &st_SetUnhandledExceptionFilter },
    { "KERNEL32.dll", "ExitProcess",                 &st_ExitProcess },
    { "KERNEL32.dll", "WriteFile",                   &st_WriteFile },
    { "KERNEL32.dll", "GetEnvironmentStrings",       &st_GetEnvironmentStrings },
    { "KERNEL32.dll", "GetEnvironmentStringsW",      &st_GetEnvironmentStringsW },
    { "KERNEL32.dll", "FreeEnvironmentStringsA",     &st_FreeEnvironmentStringsA },
    { "KERNEL32.dll", "FreeEnvironmentStringsW",     &st_FreeEnvironmentStringsW },
};
enum { kStubCount = sizeof(kStubs) / sizeof(kStubs[0]) };
u32 g_stub_va[kStubCount];
u32 g_fallback_va = 0;
unsigned g_stub_resolved = 0;

bool same_ci(const char *a, const char *b) {
    for (; *a && *b; ++a, ++b) {
        char x = *a, y = *b;
        if (x >= 'A' && x <= 'Z') x = (char)(x - 'A' + 'a');
        if (y >= 'A' && y <= 'Z') y = (char)(y - 'A' + 'a');
        if (x != y) return false;
    }
    return *a == *b;
}

void register_win32_stubs() {
    for (unsigned i = 0; i < (unsigned)kStubCount; ++i) {
        g_stub_va[i] = 0x7e000000u + 16u * i;
        ts_register(g_stub_va[i], kStubs[i].f);
    }
    g_fallback_va = 0x7e001000u;
    ts_register(g_fallback_va, &st_fallback);
}

u32 stub_va(const char *dll, const char *fn) {
    for (unsigned i = 0; i < (unsigned)kStubCount; ++i)
        if (same_ci(kStubs[i].dll, dll) && std::strcmp(kStubs[i].fn, fn) == 0)
            return g_stub_va[i];
    return 0;
}

u32 fallback_stub_va() { return g_fallback_va; }
unsigned stub_calls() { return (unsigned)g_stub_calls; }
const char *stub_last_name() { return g_stub_last; }

unsigned import_total() { return g_imp_n; }
unsigned import_resolved() { return g_stub_resolved; }

unsigned resolve_imports() {
    g_stub_resolved = 0;
    for (unsigned i = 0; i < g_imp_n; ++i) {
        u32 va = stub_va(g_imp[i].dll, g_imp[i].fn);
        if (!va) va = g_fallback_va;
        st32(g_imp[i].iat, va);
        ++g_stub_resolved;
    }
    return g_stub_resolved;
}

}  // namespace ts
#endif

namespace {

// ---------------------------------------------------------------------------
// Traced import thunks.
//
// resolve_imports() writes one shared stub VA into every IAT slot, so a hit
// alone cannot say which import was called. Each slot is therefore re-pointed
// at its own thunk, which records the name and forwards to the stub. That is
// what makes "imports called, by name, in order" answerable at all.
ts::Fn  g_orig[kMaxImports];
const char *g_name[kMaxImports];
char    g_label[kMaxImports][80];
unsigned long g_hits[kMaxImports];
unsigned g_wrapped = 0;
const char *g_trace[kTraceMax];
unsigned g_trace_n = 0;
unsigned long g_thunk_calls = 0;

template <unsigned N> void import_thunk() {
    if (N >= g_wrapped) return;
    if (g_trace_n < kTraceMax) g_trace[g_trace_n++] = g_name[N];
    ++g_hits[N];
    ++g_thunk_calls;
    ts::Fn f = g_orig[N];
    if (f) { f(); return; }
    ts::mach().r[0].d = 0;      // nothing registered at that VA: return 0 and
    ts::pop32();                // still pop the return address ts_call_at pushed
}

template <unsigned N> struct ThunkFiller {
    static void fill(ts::Fn *t) { t[N - 1] = &import_thunk<N - 1>; ThunkFiller<N - 1>::fill(t); }
};
template <> struct ThunkFiller<0> { static void fill(ts::Fn *) {} };

ts::Fn g_thunk[kMaxImports];

// ---------------------------------------------------------------------------
void report_fault(ts::Machine &m) {
    std::printf("\nFAULT: %s\n", m.fault_why ? m.fault_why : "(unrecorded)");
    std::printf("  eip        0x%08x%s\n", m.fault_eip,
                (m.fault_eip >= kTextLo && m.fault_eip < kTextHi) ? "  (inside .text)" : "");
    std::printf("  bytes       ");
    for (unsigned i = 0; i < 16; ++i) std::printf(" %02x", ts::ld8(m.fault_eip + i));
    std::printf("   <- instruction at eip\n");
    static const char *rn[8] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
    for (int i = 0; i < 8; ++i)
        std::printf("  %s%s0x%08x", rn[i], (i == 0 || i == 4) ? "      " : "  ",
                    m.fault_reg[i].d);
    std::printf("\n");
    std::printf("  at fault: depth %lu, instructions %lu\n",
                m.fault_depth, m.fault_steps);
    if (m.fault_calls_n) {
        std::printf("  last calls (oldest first):");
        for (unsigned i = 0; i < m.fault_calls_n; ++i)
            std::printf(" 0x%08x", m.fault_calls[i]);
        std::printf("\n");
    }
}

// Where the run was when it was stopped by the watchdog. A hang is as opaque
// as a crash unless it says which instruction was executing, so it says.
void report_watchdog(ts::Machine &m) {
    std::printf("\nWATCHDOG: the run was stopped by the wall-clock budget\n");
    std::printf("  eip        0x%08x%s\n", m.eip,
                (m.eip >= kTextLo && m.eip < kTextHi) ? "  (inside .text)" : "");
    std::printf("  bytes       ");
    for (unsigned i = 0; i < 16; ++i) std::printf(" %02x", ts::ld8(m.eip + i));
    std::printf("   <- instruction at eip\n");
    std::printf("  eax=0x%08x ecx=0x%08x edx=0x%08x ebx=0x%08x\n",
                m.r[0].d, m.r[1].d, m.r[2].d, m.r[3].d);
    std::printf("  esp=0x%08x ebp=0x%08x esi=0x%08x edi=0x%08x\n",
                m.r[4].d, m.r[5].d, m.r[6].d, m.r[7].d);
    unsigned n = m.recent_call_n < 8 ? m.recent_call_n : 8;
    if (n) {
        std::printf("  last calls (oldest first):");
        for (unsigned i = 0; i < n; ++i)
            std::printf(" 0x%08x", m.recent_calls[(m.recent_call_n - n + i) & 7u]);
        std::printf("\n");
    }
}

void report_imports() {
    std::printf("\nimports called, by name:\n");
    unsigned shown = 0;
    unsigned long total = 0;
    for (unsigned i = 0; i < g_imp_n; ++i) total += g_hits[i];
    bool *done = (bool *)std::calloc(g_imp_n ? g_imp_n : 1, sizeof(bool));
    for (unsigned k = 0; k < 25 && k < g_imp_n; ++k) {
        unsigned best = (unsigned)-1;
        for (unsigned i = 0; i < g_imp_n; ++i) {
            if (done[i] || g_hits[i] == 0) continue;
            if (best == (unsigned)-1 || g_hits[i] > g_hits[best]) best = i;
        }
        if (best == (unsigned)-1) break;
        done[best] = true;
        std::printf("  %6lu  %s\n", g_hits[best], g_label[best]);
        ++shown;
    }
    unsigned never = 0;
    for (unsigned i = 0; i < g_imp_n; ++i) if (g_hits[i] == 0) ++never;
    std::printf("  (%lu calls over %u distinct imports; %u never called)\n",
                total, shown, never);
    std::free(done);
}

void report_message_loop() {
    unsigned long get = 0, peek = 0, disp = 0;
    for (unsigned i = 0; i < g_imp_n; ++i) {
        const char *n = g_imp[i].fn;
        if (!std::strcmp(n, "GetMessageA") || !std::strcmp(n, "GetMessageW")) get += g_hits[i];
        if (!std::strcmp(n, "PeekMessageA") || !std::strcmp(n, "PeekMessageW")) peek += g_hits[i];
        if (!std::strcmp(n, "DispatchMessageA") || !std::strcmp(n, "DispatchMessageW"))
            disp += g_hits[i];
    }
    if (get || peek)
        std::printf("MESSAGE LOOP: reached -- GetMessage %lu, PeekMessage %lu, "
                    "DispatchMessage %lu\n", get, peek, disp);
    else
        std::printf("MESSAGE LOOP: not reached (no GetMessage/PeekMessage call)\n");
}

}  // namespace

int main(int argc, char **argv) {
    const char *imgpath = argc > 1 ? argv[1] : "bin/GAME.EXE";
    unsigned long budget = argc > 2 ? std::strtoul(argv[2], 0, 10) : 60000000ul;
    unsigned long depth_limit = argc > 3 ? std::strtoul(argv[3], 0, 10) : 20000ul;
    unsigned wall_seconds = argc > 4 ? (unsigned)std::strtoul(argv[4], 0, 10) : 60u;

    std::printf("ts_link_test: linking the recovered tree succeeded\n");

    ts::register_all_recovered();

    unsigned resolved = 0;
    for (unsigned i = 0; i < sizeof(kSample) / sizeof(kSample[0]); ++i) {
        ts::Fn f = ts::ts_lookup(kSample[i]);
        std::printf("  dispatch 0x%08x -> %s\n", kSample[i],
                    f ? "resolved" : "NOT REGISTERED");
        if (f) ++resolved;
    }
    std::printf("ts_link_test: %u of %u sampled VAs resolved\n", resolved,
                (unsigned)(sizeof(kSample) / sizeof(kSample[0])));

    ts::Machine &m = ts::mach();
    m.text_lo = kTextLo;
    m.text_hi = kTextHi;

    // -- boot -----------------------------------------------------------------
    std::printf("\n-- boot --\n");
    std::printf("  reading %s ... ", imgpath);
    std::fflush(stdout);
    PeFile pe;
    if (!pe.open(imgpath)) {
        std::printf("FAILED (no such file, or not a PE)\n");
        std::printf("ts_link_test: FAILED (no image to boot)\n");
        return 1;
    }
    std::printf("ok, %lu bytes, image base 0x%08x\n", pe.len, pe.base);

    collect_imports(pe);
    std::printf("  imports: %u across %u DLLs\n", g_imp_n, g_dlls);

    std::printf("  load_image() ... ");
    std::fflush(stdout);
#if defined(TS_LINK_HAVE_IMAGE)
    if (!ts::load_image(imgpath)) { std::printf("FAILED\n"); return 1; }
    std::printf("ok (Image.h)\n");
#else
    if (!ts::load_image(imgpath)) { std::printf("FAILED\n"); return 1; }
    std::printf("ok (fallback: %u sections, %lu bytes mapped)\n",
                g_image_sections, g_image_bytes);
#endif

    std::printf("  register_win32_stubs() ... ");
    std::fflush(stdout);
    ts::register_win32_stubs();
    std::printf("ok\n");

    std::printf("  resolve_imports() ... ");
    std::fflush(stdout);
    unsigned wrote = ts::resolve_imports();
    std::printf("%u of %u IAT slots written\n", wrote, ts::import_total());

    // Re-point every IAT slot at a per-import thunk so calls can be attributed.
    ThunkFiller<kMaxImports>::fill(g_thunk);
    unsigned with_stub = 0, no_stub = 0;
    for (unsigned i = 0; i < g_imp_n; ++i) {
        u32 target = ts::ld32(g_imp[i].iat);
        if (target == 0) { ++no_stub; continue; }
        std::sprintf(g_label[i], "%s!%s", g_imp[i].dll, g_imp[i].fn);
        g_name[i] = g_label[i];
        g_orig[i] = ts::ts_lookup(target);
        u32 va = kWrapperBase + kWrapperStep * i;
        ts::ts_register(va, g_thunk[i]);
        ts::st32(g_imp[i].iat, va);
        ++with_stub;
    }
    g_wrapped = g_imp_n;
    std::printf("  traced thunks: %u imports bound, %u with no stub\n", with_stub, no_stub);

    // -- run ------------------------------------------------------------------
    m.r[4].d = kStackTop;                 // ESP: a plausible stack, not zero
    ts::push32(kSentinel);                // what the entry point's final `ret` pops
    const u32 esp0 = m.r[4].d;
    m.call_depth_limit = depth_limit;
    m.step_limit = budget;
    jmp_buf halt;
    g_halt = &halt;
    g_timed_out = false;
    std::signal(SIGALRM, on_watchdog);
    alarm(wall_seconds);

    std::printf("  running entry point 0x%08x: ESP=0x%08x, instruction budget %lu, "
                "depth limit %lu, wall-clock watchdog %us\n",
                kEntryPoint, esp0, budget, depth_limit, wall_seconds);
    std::fflush(stdout);
    if (setjmp(halt) == 0) {
        ts::ts_call(kEntryPoint);
        g_returned = true;
    }
    alarm(0);
    g_halt = 0;
    m.halt = 0;
    const bool completed = g_returned && m.r[4].d > esp0;

    // -- report ---------------------------------------------------------------
    std::printf("\n-- result --\n");
    std::printf("  entry point: %s\n",
                completed ? "RETURNED to its caller (ran to completion)"
                          : (m.faulted ? "did not return -- stopped at a fault"
                                       : (g_timed_out ? "did not return -- stopped by the watchdog"
                                                      : "did not return -- stopped without a fault")));
    if (m.depth_exceeded)
        std::printf("  depth guard fired %lu time(s): the run is TRUNCATED, "
                    "not complete\n", m.depth_exceeded);
    if (m.step_limit_hit)
        std::printf("  instruction budget exhausted: the run is TRUNCATED, "
                    "not complete\n");
    std::printf("  instructions executed: %lu\n", m.steps);
    std::printf("  deepest ts_call nesting: %lu (limit %lu)\n",
                m.call_depth_max, m.call_depth_limit);
    std::printf("  final eip=0x%08x eax=0x%08x esp=0x%08x\n",
                m.eip, m.r[0].d, m.r[4].d);
    std::printf("  calls to an address nothing registered: %lu "
                "(%lu of them inside .text)\n",
                m.unmapped_calls, m.unmapped_in_text);
    std::printf("  unmodelled instructions: %lu\n", m.unmodelled_insns);
    std::printf("  import thunk calls: %lu\n", g_thunk_calls);

    if (g_trace_n) {
        std::printf("\nfirst %u stub calls, in order:\n", g_trace_n);
        for (unsigned i = 0; i < g_trace_n; ++i)
            std::printf("  %2u. %s\n", i + 1, g_trace[i]);
    }
    report_imports();
    report_message_loop();

    if (m.faulted) report_fault(m);
    if (g_timed_out) report_watchdog(m);

    if (m.unmapped_sample_n) {
        std::printf("\nfirst unmapped call targets:");
        for (unsigned i = 0; i < m.unmapped_sample_n; ++i)
            std::printf(" 0x%08x", m.unmapped_sample[i]);
        std::printf("\n");
    }

    // The gate: the link and the dispatch table, plus any recovered function
    // that nothing registered. A fault or a truncation is progress, not a
    // failure -- the model has stubs for Windows, not Windows.
    if (resolved != sizeof(kSample) / sizeof(kSample[0])) {
        std::printf("ts_link_test: FAILED (dispatch incomplete)\n");
        return 1;
    }
    if (m.unmapped_in_text) {
        std::printf("ts_link_test: FAILED (%lu unmapped calls inside .text: a "
                    "recovered function nothing registered)\n", m.unmapped_in_text);
        return 1;
    }
    std::printf("ts_link_test: OK\n");
    return 0;
}
