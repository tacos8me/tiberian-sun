#ifndef TS_WIN32STUBS_H
#define TS_WIN32STUBS_H

// ---------------------------------------------------------------------------
// Win32Stubs.h -- the image's imported Windows functions, made to do something.
//
// PROVENANCE. This header is OUR model, not recovered data. The import list it
// is built from IS recovered: bin/GAME.EXE's import directory at RVA 0x2b8848
// yields 341 imports across 13 DLLs (KERNEL32 153, USER32 92, GDI32 24,
// WSOCK32 19, ole32 15, OLEAUT32 11, ADVAPI32 8, COMCTL32 7, WINMM 6,
// VERSION 3, SHELL32 1, DDRAW 1, DSOUND 1). Every name in the table below is
// copied from that directory verbatim, so a name that is not in the binary
// cannot appear here.
//
// WHY. The transcribed program calls its imports through the IAT:
//
//     call dword ptr [0x698254]   ->   ts_call_ptr(ld32(0x698254), ret)
//
// The model never loaded .idata, so every IAT slot read 0 and every import call
// landed on address 0 -- 16,383 of them during the entry point, none of which
// did anything. This header is the other half: a stub per import, registered in
// the same dispatch table the recovered functions use, at a synthetic VA the
// loader writes into the IAT. With it, `call dword ptr [IAT]` reaches real
// code.
//
// A stub is entered exactly as the original callee would be: the caller's
// `call` has already pushed the return address, so ESP points at it and the
// __stdcall arguments sit above it at ESP+4, ESP+8, ... A stub therefore reads
// its arguments from the shadow stack, does the minimum plausible thing, and
// returns by discarding the return address AND the arguments the callee owns.
// That last part is what keeps the modelled stack balanced: a stub that forgot
// to pop would shift every later frame.
//
// HONESTY. A stub that cannot do the real thing does not pretend to. It is
// named, counted, and returns a value chosen to keep the caller alive rather
// than to crash it. An import with no stub at all reaches fallback_stub_va(),
// which logs the name and returns 0 in EAX, so an unimplemented import is
// VISIBLE rather than silent.
//
// This header is header-only for the same reason Machine.h is: the pipeline
// wipes src/generated, so nothing here may depend on a hand-written .cpp.
// ---------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Machine.h"

namespace ts {
namespace w32 {

// -- synthetic stub addresses ------------------------------------------------
// Well clear of the image (0x400000..0x437000), of Lane J's image mapping, and
// of the bump allocator below (0x20000000). 16 bytes apart so a stub VA is
// recognisable at a glance in a trace.
inline u32 &stub_va_next() { static u32 v = 0x7f000000u; return v; }
inline u32 alloc_stub_va() {
    u32 v = stub_va_next();
    stub_va_next() = v + 16u;
    return v;
}

// -- bump allocator ----------------------------------------------------------
// The model hands out pages lazily and zeroes them, so any address in this
// region is already valid writable memory; "allocating" is moving a cursor.
// Nothing is ever freed, which is why every free stub succeeds.
// Deliberately high and out of the game's way. A stub heap at 0x20000000 put
// CRT allocations and the game's own allocator in the same pages: the CRT's atexit
// table landed at 0x20005900 and the game wrote 0x11/0x31 through its own
// pointers into it, so exit walked 37 "handlers" that were never registered.
inline u32 &bump() { static u32 b = 0x50000000u; return b; }
inline u32 alloc_block(u32 size, u32 align) {
    if (size == 0) size = 1;
    u32 &b = bump();
    b = (b + (align ? align - 1u : 0u)) & ~(align ? align - 1u : 0u);
    u32 p = b;
    b += size + 16u;   // slack: a later overrun must not collide with the next
    return p;
}

// -- observable state --------------------------------------------------------
// Every stub call is counted. That count is the difference between "the import
// did something" and "the import was reached and did nothing", and the run
// cannot tell the two apart without it.
struct StubState {
    unsigned long calls;            // every stub invocation, of any kind
    unsigned long fallbacks;        // invocations that reached a fallback stub
    unsigned long resolved;         // imports that were given a stub VA
    unsigned long fallback_names;   // distinct names that needed a fallback
    // Two counts that must both be zero for the modelled stack to be sound: an
    // import resolved to a fallback whose argument count is not known, and a
    // call to one of those. Either one is a hole a reader has to see.
    unsigned long unbalanced_at_resolution;
    unsigned long unbalanced;       // fallback calls that could not pop
    unsigned long exit_calls;       // ExitProcess invocations the model walked past
    unsigned long proc_address_misses;  // GetProcAddress names nothing implements
    unsigned long file_opens;           // CreateFileA that found a real file
    unsigned long file_misses;          // CreateFileA that returned INVALID_HANDLE
    char last[64];                  // name of the most recent stub called
    bool verbose;                   // print each stub call
    StubState() : calls(0), fallbacks(0), resolved(0), fallback_names(0),
                  unbalanced_at_resolution(0), unbalanced(0), exit_calls(0),
                  proc_address_misses(0), verbose(false) {
        last[0] = 0;
    }
};
inline StubState &state() { static StubState s; return s; }

inline unsigned long *counts() { static unsigned long c[512]; return c; }
// Declared here, defined next to the import table it indexes.
inline unsigned long &count_of(const char *fn);

inline void note(const char *name) {
    StubState &s = state();
    s.calls++;
    count_of(name)++;
    std::strncpy(s.last, name, sizeof(s.last) - 1);
    s.last[sizeof(s.last) - 1] = 0;
    if (s.verbose) std::printf("  [stub] %s\n", name);
}


// -- calling convention ------------------------------------------------------
inline u32 arg(unsigned i) { return ld32(ESP.d + 4u + i * 4u); }
inline void stub_done(unsigned nargs, u32 value) {
    mach().stub_dispatches++;   // the stack-balance gate in ts_call_at watches this
    // ts_call_at checks the call against esp_before + 4*nargs: a __stdcall stub
    // pops the return address and its own arguments, so it does NOT return to
    // esp_before. Recording the count here is what lets the tracker tell a
    // correct stub from a real defect.
    mach().stub_last_nargs = nargs;
    mach().stub_last_depth = mach().call_depth;   // direct-stub test in ts_call_at
    ESP.d += 4u + nargs * 4u;
    EAX.d = value;
}
// An import's argument count belongs to ONE place: its row in the import table,
// which tools/gen_win32_stubs.py derives from the call sites in the recovered
// instruction stream (`--evidence` re-derives and checks it). A body that
// carried its own count was a second, unverifiable claim about the same fact --
// and for 20 of the 32 ordinal imports the two claims disagreed, so the body
// would have popped 7 words where the caller had pushed 2. A body therefore
// asks the table how many words it owns.
inline void stub_done_for(const char *dll, const char *fn, u32 value);

// 0x800000a1 -> "#161": the spelling the loader uses for an ordinal import,
// because an ordinal has no name string and its number is all it carries.
inline const char *ord_name(u32 raw) {
    static char b[16];
    std::sprintf(b, "#%u", (unsigned)(raw & 0xffffu));
    return b;
}

#define TS_BEGIN(w) void k_##w() { note(#w);
#define TS_END }
#define TS_RET(w, n, v) TS_BEGIN(w) stub_done(n, v); TS_END

// An ordinal import's C++ symbol carries its DLL and its raw ILT value
// (0x800000a1), because the same ordinal number means a different function in
// every DLL. Its argument count is NOT written here: it lives in the import
// table row for "#161", which is where `--evidence` checks it. A body that
// named its own count would be a second claim about the same fact.
#define TS_ORD_BEGIN(sym, dllname, raw) \
    void k_##sym() { const char *const ord = ord_name(raw); note(ord);
#define TS_ORD_END }
#define TS_ORD_RET(sym, dllname, raw, v) \
    TS_ORD_BEGIN(sym, dllname, raw) stub_done_for(dllname, ord, v); TS_ORD_END

// -- memory and string helpers ----------------------------------------------
inline u32 c_strlen(u32 p) { u32 n = 0; while (ld8(p + n)) ++n; return n; }
inline u32 w_strlen(u32 p) { u32 n = 0; while (ld16(p + n * 2u)) ++n; return n; }
inline void c_strcpy(u32 d, u32 s) {
    while (ld8(s)) { st8(d, ld8(s)); ++d; ++s; }
    st8(d, 0);
}
inline void c_strncpy(u32 d, u32 s, u32 n) {
    u32 i = 0;
    for (; i + 1u < n && ld8(s + i); ++i) st8(d + i, ld8(s + i));
    if (n) st8(d + i, 0);
}
// A literal, written into the model so the program can read it back.
inline u32 lit(const char *s) {
    u32 n = (u32)std::strlen(s) + 1u;
    u32 p = alloc_block(n, 4u);
    for (u32 i = 0; i < n; ++i) st8(p + i, (u8)s[i]);
    return p;
}
inline const char *ld_cstr(u32 p) {
    static char buf[512];
    unsigned i = 0;
    for (; i + 1u < sizeof(buf); ++i) { u8 c = ld8(p + i); if (!c) break; buf[i] = (char)c; }
    buf[i] = 0;
    return buf;
}
inline u32 zero_block(u32 size, u32 align) {
    u32 p = alloc_block(size, align);
    for (u32 i = 0; i < size; ++i) st8(p + i, 0);
    return p;
}

// A C string printed so that every byte is visible. A stub that shows a program
// a message box has to report the text, and a boot that has not loaded the
// game's string table yet hands over a pointer into zeroed memory or into
// whatever the model happened to leave there: `%s` turns that into a mojibake
// line, and a reader cannot tell "empty" from "garbled" from "a control code".
// Escaping every byte makes the difference legible, and the length makes an
// unterminated buffer visible instead of a read that runs off the page.
inline void print_escaped(const char *label, u32 p, unsigned max) {
    std::printf("  %s 0x%08x: \"", label, p);
    unsigned i = 0;
    for (; i < max; ++i) {
        u8 c = ld8(p + i);
        if (!c) break;
        if (c >= 0x20u && c < 0x7fu) std::printf("%c", (char)c);
        else std::printf("\\x%02x", (unsigned)c);
    }
    std::printf("\" (%u byte%s%s)\n", i, i == 1u ? "" : "s",
                i == max ? ", no terminator in range" : "");
}

// -- clock -------------------------------------------------------------------
// A monotonic millisecond counter. The original reads the real clock; a
// monotonic counter is what every caller actually depends on, and it makes a
// run deterministic, which a real clock would not.
inline u32 &tick() { static u32 t = 1000u; return t; }
inline u32 tick_advance(u32 ms) { tick() += ms; return tick(); }

// Defined with the table: name -> stub VA, ignoring the DLL.
inline u32 find_by_name(const char *fn);

// -- module handles ----------------------------------------------------------
// A handle a caller gets from LoadLibraryA has to be identifiable by module
// name, because GetProcAddress's whole job is "the export of THIS module". The
// old body ignored its argument and returned a counter, so every handle meant
// "some DLL" and GetProcAddress could only search one global name space. That
// made a GetProcAddress-only export impossible to model: blowfish.dll's
// DllRegisterServer is not in the import directory, so there was no row for it
// and the game's registration path saw a null and took its error box.
//
// A handle is now the address of a loaded-module row; the row names the module
// and carries its export table. The exports are registered as ordinary stubs,
// so a caller that gets one back can call it.
struct ExportDef { const char *fn; u8 nargs; u32 ret; Fn real; u32 va; };
struct ModuleDef { const char *name; ExportDef *exports; unsigned nexports; };

inline ModuleDef *module_defs(unsigned *count);
inline u32 module_load(const char *name);
inline u32 module_find_export(u32 handle, const char *fn);
inline void register_module_exports();
inline bool same_dll(const char *a, const char *b);

// ===========================================================================
// KERNEL32
// ===========================================================================

TS_RET(GetVersion, 0, 0x04000004u)   // NT4/Win95-OSR2 class: 4.0
TS_RET(GetLastError, 0, 0u)

TS_BEGIN(SetLastError)
    static u32 err = 0;
    err = arg(0);
    stub_done(1, 0u);
TS_END

TS_BEGIN(GetVersionExA)
    u32 p = arg(0);
    for (u32 i = 0; i < 0x9cu; ++i) st8(p + i, 0);
    st32(p, 0x9cu);
    st32(p + 4, 4u);          // dwMajorVersion
    st32(p + 8, 0u);          // dwMinorVersion
    st32(p + 12, 1381u);      // dwBuildNumber
    st32(p + 16, 2u);         // VER_PLATFORM_WIN32_NT
    stub_done(1, 1u);
TS_END

TS_BEGIN(GetModuleHandleA)
    // lpModuleName == 0 means "this image"; either way the only module the
    // model has is the image, at its real base.
    (void)arg(0);
    stub_done(1, 0x00400000u);
TS_END

TS_BEGIN(GetModuleFileNameA)
    u32 buf = arg(1), cch = arg(2);
    const char *n = "GAME.EXE";
    u32 len = (u32)std::strlen(n);
    if (cch) {
        u32 i = 0;
        for (; i + 1u < cch && n[i]; ++i) st8(buf + i, (u8)n[i]);
        st8(buf + i, 0);
    }
    stub_done(3, len);
TS_END

TS_BEGIN(LoadLibraryA)
    // The name is the handle's identity now, not a counter's. A module the
    // model knows (blowfish.dll, comctl32.dll) gets its row and its exports; an
    // unknown DLL still gets a non-null handle so a caller that only checks for
    // failure keeps running, but it is named in the log and its GetProcAddress
    // will resolve nothing.
    const char *name = ld_cstr(arg(0));
    u32 h = module_load(name);
    if (!h) {
        std::printf("win32stubs: LoadLibraryA(\"%s\") -> 0 -- module table full; "
                    "the caller takes its failure path\n", name);
    }
    stub_done(1, h);
TS_END

TS_RET(FreeLibrary, 1, 1u)

TS_BEGIN(GetProcAddress)
    // The returned address must be a registered stub or the caller's first call
    // lands on nothing, so a name with no stub returns 0 -- and says so. A
    // silent 0 reads as "this DLL does not export that", which is a claim the
    // caller acts on: the game asks comctl32 for DllGetVersion, and on a 0 it
    // decides the system is too old, puts up a message box and exits.
    u32 handle = arg(0);
    const char *name = ld_cstr(arg(1));
    u32 va = module_find_export(handle, name);
    if (!va) {
        state().proc_address_misses++;
        std::printf("win32stubs: GetProcAddress(module 0x%08x, \"%s\") -> 0 -- no "
                    "stub for it; a caller that tests this takes its failure path\n",
                    handle, name);
    }
    stub_done(2, va);
TS_END

TS_BEGIN(GetCommandLineA)
    static u32 p = 0;
    if (!p) p = lit("GAME.EXE");
    stub_done(0, p);
TS_END

TS_BEGIN(GetStartupInfoA)
    // STARTUPINFOA is 68 bytes. dwFlags stays 0 so the CRT uses its defaults
    // rather than believing in std handles it was never given.
    u32 p = arg(0);
    for (u32 i = 0; i < 68u; ++i) st8(p + i, 0);
    st32(p, 68u);
    st16(p + 48u, 1u);        // wShowWindow = SW_SHOWNORMAL
    st32(p + 56u, 0x0000040au);  // hStdInput
    st32(p + 60u, 0x0000040bu);  // hStdOutput
    st32(p + 64u, 0x0000040cu);  // hStdError
    stub_done(1, 0u);
TS_END

// -- heaps -------------------------------------------------------------------
TS_BEGIN(HeapCreate)
    static u32 n = 0;
    stub_done(3, 0x00010000u + (n++));
TS_END
TS_RET(HeapDestroy, 1, 1u)
TS_RET(GetProcessHeap, 0, 0x00010000u)

TS_BEGIN(HeapAlloc)
    u32 size = arg(2);
    // A size this large is never a real request: it is a length the caller
    // computed from data it did not get (a failed stream read leaves the
    // stack slot it was read from uninitialised). Naming it here turns the
    // model's page-table exhaustion into the value that caused it.
    if (std::getenv("TS_FILE_TRACE") && size > 0x08000000u)
        std::fprintf(stderr, "win32stubs: HeapAlloc(size=0x%08x) -- implausible "
                             "allocation, step %lu\n", size, mach().steps);
    u32 p = alloc_block(size, 8u);
    st32(p, size);
    u32 q = p + 16u;
    // Zero the block. Not a convenience -- a correctness requirement. Fresh pages
    // from the OS are zero, and the CRT relies on it: its atexit table is scanned
    // for non-null slots, so a table of stale bytes "contains" 37 registered
    // handlers that were never registered, and exit calls all 37 as function
    // pointers. That produced calls to 0x11 and 0x6a4e08 from 0x6844dd and, via
    // the return addresses they leaked, an ESI that made the CRT call WinMain at
    // an address inside ExitProcess. HEAP_ZERO_MEMORY asks for this explicitly;
    // a real heap's first use of a page is zero whether you ask or not.
    for (u32 i = 0; i < size; i += 4u) st32(q + i, 0u);
    stub_done(3, q);
TS_END

TS_BEGIN(HeapFree)
    // Nothing is reused; the block stays valid, which is why a use-after-free
    // in the program reads plausible bytes instead of nothing.
    stub_done(3, 1u);
TS_END

TS_BEGIN(HeapSize)
    u32 p = arg(2);
    stub_done(3, p ? ld32(p - 16u) : 0u);
TS_END

TS_BEGIN(HeapReAlloc)
    u32 old = arg(2), size = arg(3);
    u32 np = alloc_block(size, 8u);
    if (old) {
        u32 n = ld32(old - 16u);
        for (u32 i = 0; i < n && i < size; ++i) st8(np + 16u + i, ld8(old + i));
    }
    st32(np, size);
    stub_done(4, np + 16u);
TS_END

TS_BEGIN(LocalAlloc)
    u32 size = arg(1);
    u32 p = alloc_block(size, 8u);
    st32(p, size);
    stub_done(2, p + 16u);
TS_END
TS_RET(LocalFree, 1, 0u)
TS_RET(GlobalFree, 1, 0u)

TS_BEGIN(GlobalAlloc)
    u32 size = arg(1);
    u32 p = alloc_block(size, 8u);
    st32(p, size);
    stub_done(2, p + 16u);
TS_END

// -- virtual memory ----------------------------------------------------------
TS_BEGIN(VirtualAlloc)
    u32 size = arg(1) ? arg(1) : 0x1000u;
    stub_done(4, zero_block(size, 0x1000u));
TS_END
TS_RET(VirtualFree, 3, 1u)

TS_BEGIN(VirtualProtect)
    u32 old = arg(3);
    if (old) st32(old, 0x40u);   // PAGE_EXECUTE_READWRITE
    stub_done(4, 1u);
TS_END

TS_BEGIN(VirtualQuery)
    u32 addr = arg(0), out = arg(2);
    for (u32 i = 0; i < 28u; ++i) st8(out + i, 0);
    st32(out + 0u, addr & ~0xfffu);        // BaseAddress
    st32(out + 4u, addr & ~0xfffu);        // AllocationBase
    st32(out + 8u, 0x40u);                 // AllocationProtect
    st32(out + 12u, 0x1000u);              // RegionSize
    st32(out + 16u, 0x1000u);              // State = MEM_COMMIT
    st32(out + 20u, 0x40u);                // Protect
    st32(out + 24u, 0x20000u);             // Type = MEM_PRIVATE
    stub_done(3, 28u);
TS_END

TS_BEGIN(GetSystemInfo)
    u32 p = arg(0);
    for (u32 i = 0; i < 36u; ++i) st8(p + i, 0);
    st32(p + 4u, 0x1000u);          // dwPageSize
    st32(p + 8u, 0x10000u);         // lpMinimumApplicationAddress
    st32(p + 12u, 0x7fffffffu);     // lpMaximumApplicationAddress
    st32(p + 16u, 1u);              // dwActiveProcessorMask
    st32(p + 20u, 1u);              // dwNumberOfProcessors
    st32(p + 24u, 586u);            // dwProcessorType
    st32(p + 28u, 0x10000u);        // dwAllocationGranularity
    stub_done(1, 0u);
TS_END

// -- time --------------------------------------------------------------------
inline void write_systemtime(u32 p) {
    st16(p + 0u, 1999u); st16(p + 2u, 9u); st16(p + 4u, 3u); st16(p + 6u, 1u);
    st16(p + 8u, 12u); st16(p + 10u, 0u); st16(p + 12u, 0u); st16(p + 14u, 0u);
}
TS_BEGIN(GetSystemTime) write_systemtime(arg(0)); stub_done(1, 0u); TS_END
TS_BEGIN(GetLocalTime)  write_systemtime(arg(0)); stub_done(1, 0u); TS_END

TS_BEGIN(GetSystemTimeAsFileTime)
    u32 p = arg(0);
    st32(p, 0x2b0e8b00u); st32(p + 4u, 0x01bf0000u);
    stub_done(1, 0u);
TS_END

TS_RET(GetTickCount, 0, tick())
TS_RET(timeGetTime, 0, tick())

TS_BEGIN(Sleep) stub_done(1, tick_advance(arg(0)) ? 0u : 0u); TS_END

TS_BEGIN(QueryPerformanceCounter)
    st32(arg(0), tick_advance(1u));
    stub_done(1, 1u);
TS_END
TS_BEGIN(QueryPerformanceFrequency) st32(arg(0), 1000000u); stub_done(1, 1u); TS_END

TS_BEGIN(GetTimeZoneInformation)
    u32 p = arg(0);
    for (u32 i = 0; i < 172u; ++i) st8(p + i, 0);
    stub_done(1, 0u);   // TIME_ZONE_ID_UNKNOWN
TS_END

// -- processes and threads ---------------------------------------------------
TS_RET(GetCurrentProcess, 0, 0xffffffffu)
TS_RET(GetCurrentThread, 0, 0xfffffffeu)
TS_RET(GetCurrentProcessId, 0, 1234u)
TS_RET(GetCurrentThreadId, 0, 1234u)

TS_BEGIN(ExitProcess)
    // The program has decided to stop, and it says with what code. The model
    // cannot unwind to its caller from here, so the stub returns and the frames
    // above it run to their own `ret`: that is wrong about control flow, and
    // leaving it unspoken is worse, because a run that walks on after an
    // ExitProcess looks like a run that is making progress. It is printed, and
    // counted, so the log says which it is.
    std::printf("win32stubs: ExitProcess(%u) from 0x%08x -- the program has "
                "decided to stop; halting here, as a real process would\n",
                (unsigned)arg(0), (unsigned)ld32(ESP.d));
    state().exit_calls++;
    // Actually stop. Returning from here let the frames above run to their own
    // `ret` and the model walked on past a fatal exit -- so a "message loop
    // reached" AFTER an ExitProcess was a loop the real game never ran, and the
    // exit path's own bookkeeping (an atexit table malloc had since overwritten)
    // produced 71 calls to addresses that are not code. A process that exits is
    // finished; the model must agree or every later observation is fiction.
    ts::fault("the program called ExitProcess");
    stub_done(1, 0u);
TS_END
TS_RET(TerminateProcess, 2, 1u)

TS_BEGIN(GetExitCodeProcess)
    u32 p = arg(1);
    if (p) st32(p, 0u);
    stub_done(2, 1u);
TS_END

TS_BEGIN(CreateThread)
    u32 id = arg(5);
    if (id) st32(id, 1234u);
    static u32 n = 0;
    stub_done(6, 0x00040000u + (n++));
TS_END

TS_RET(WaitForSingleObject, 2, 0u)     // WAIT_OBJECT_0
TS_RET(WaitForMultipleObjects, 4, 0u)

TS_BEGIN(CreateEventA)
    static u32 n = 0;
    stub_done(4, 0x00050000u + (n++));
TS_END
TS_BEGIN(CreateEventW)
    static u32 n = 0;
    stub_done(4, 0x00060000u + (n++));
TS_END
TS_BEGIN(CreateMutexA)
    static u32 n = 0;
    stub_done(3, 0x00070000u + (n++));
TS_END
TS_BEGIN(OpenMutexA) stub_done(3, 0x00070000u); TS_END
TS_RET(SetEvent, 1, 1u)
TS_RET(ResetEvent, 1, 1u)
TS_RET(ReleaseMutex, 1, 1u)

TS_BEGIN(CreateProcessA)
    (void)arg(0);
    stub_done(10, 1u);
TS_END

// -- critical sections -------------------------------------------------------
// Single-threaded, so enter/leave are genuinely no-ops. The section is zeroed
// so a debugger's view of it is at least self-consistent.
TS_BEGIN(InitializeCriticalSection)
    u32 p = arg(0);
    for (u32 i = 0; i < 24u; ++i) st8(p + i, 0);
    stub_done(1, 0u);
TS_END
TS_RET(EnterCriticalSection, 1, 0u)
TS_RET(LeaveCriticalSection, 1, 0u)
TS_RET(DeleteCriticalSection, 1, 0u)

TS_BEGIN(InterlockedIncrement)
    u32 p = arg(0);
    u32 v = ld32(p) + 1u;
    st32(p, v);
    stub_done(1, v);
TS_END

TS_BEGIN(InterlockedDecrement)
    u32 p = arg(0);
    u32 v = ld32(p) - 1u;
    st32(p, v);
    stub_done(1, v);
TS_END

// -- TLS ---------------------------------------------------------------------
TS_RET(TlsAlloc, 0, 0u)
TS_RET(TlsFree, 1, 1u)
TS_RET(TlsGetValue, 1, 0u)
TS_RET(TlsSetValue, 2, 1u)

// -- exception handling ------------------------------------------------------
TS_RET(IsBadReadPtr, 2, 0u)
TS_RET(IsBadCodePtr, 1, 0u)
TS_RET(IsBadHugeReadPtr, 2, 0u)
TS_RET(IsBadHugeWritePtr, 2, 0u)

TS_BEGIN(RaiseException)
    // The CRT throws C++ exceptions through this. Swallowing it means a throw
    // never unwinds, but the caller keeps running instead of stopping.
    stub_done(4, 0u);
TS_END
TS_BEGIN(RtlUnwind) stub_done(4, 0u); TS_END
TS_RET(UnhandledExceptionFilter, 1, 1u)   // EXCEPTION_EXECUTE_HANDLER
TS_RET(SetUnhandledExceptionFilter, 1, 0u)
TS_RET(SetErrorMode, 1, 0u)

// -- console and std handles -------------------------------------------------
TS_BEGIN(GetStdHandle)
    stub_done(1, 0x00000400u + (u32)(0u - arg(0)));
TS_END
TS_RET(SetStdHandle, 2, 1u)
TS_RET(SetHandleCount, 1, 1u)
TS_RET(GetFileType, 1, 2u)   // FILE_TYPE_CHAR
TS_RET(AllocConsole, 0, 1u)
TS_RET(FreeConsole, 0, 1u)
TS_RET(GetConsoleMode, 2, 1u)
TS_RET(SetConsoleMode, 2, 1u)
TS_RET(SetConsoleCtrlHandler, 2, 1u)
TS_RET(GetNumberOfConsoleInputEvents, 2, 1u)
TS_RET(PeekConsoleInputA, 4, 1u)
TS_RET(ReadConsoleInputA, 4, 1u)

TS_BEGIN(WriteConsoleA)
    u32 written = arg(3);
    if (written) st32(written, arg(2));
    stub_done(5, 1u);
TS_END

// -- files -------------------------------------------------------------------
// Real file I/O over a host directory.
//
// This used to be a lie: every file "existed" and read as empty, and
// GetFileSize/SetFilePointer agreed, so nothing ever read past an end that was
// never written. It was consistent and it was useless -- the game cannot read
// rules.ini, TIBSUN.MIX or the CD, so it exits during init, and no amount of
// DirectDraw work changes that. `tools/oracle.py` confirmed the retail binary
// gives up at the same stage in this environment, for the same reason.
//
// Paths are resolved case-insensitively per component, because Windows is
// case-insensitive and the game asks for `rules.ini` when the file on disk may be
// `RULES.INI`. A miss returns INVALID_HANDLE_VALUE and sets the last error, which
// is what lets a caller take its real failure path instead of reading zeroes.
#include <dirent.h>
#include <sys/stat.h>

inline const char *ts_root() {
    const char *r = std::getenv("TS_ROOT");
    return (r && *r) ? r : "../gamedir";
}

// The CD is a second host root, because it is a second *drive* to the game.
// GetCDClass (0x4a7600) scans c:..z: with GetDriveTypeA and keeps the letters
// that answer DRIVE_CDROM; Force_CD_Available then requires one of them to
// carry the volume label "TS%d" and to hold TS%d.dsk. TS_CD names the host
// directory that stands in for that drive, and D: is the letter it is given.
inline const char *ts_cd_root() {
    const char *r = std::getenv("TS_CD");
    return (r && *r) ? r : "";
}
enum { CD_DRIVE_INDEX = 3 };   // 'D'
inline int drive_index_of(const char *path) {
    if (path && path[0] && path[1] == ':') {
        char c = path[0];
        if (c >= 'a' && c <= 'z') c = (char)(c - 32);
        if (c >= 'A' && c <= 'Z') return c - 'A';
    }
    return -1;
}
inline bool path_is_cd_drive(const char *path) {
    return ts_cd_root()[0] && drive_index_of(path) == (int)CD_DRIVE_INDEX;
}

inline void model_rd(void *dst, u32 va, u32 n) {
    u8 *d = (u8 *)dst;
    for (u32 i = 0; i < n; ++i) d[i] = ld8(va + i);
}
inline void model_wr(u32 va, const void *src, u32 n) {
    const u8 *sp = (const u8 *)src;
    for (u32 i = 0; i < n; ++i) st8(va + i, sp[i]);
}

// Resolve one path component against a directory, ignoring case. Returns the
// on-disk spelling, which is what fopen needs on a case-sensitive filesystem.
inline bool resolve_ci(const char *dir, const char *want, char *out, size_t outn) {
    DIR *d = opendir(dir[0] ? dir : ".");
    if (!d) return false;
    bool ok = false;
    while (struct dirent *e = readdir(d)) {
        const char *a = e->d_name, *b = want;
        while (*a && *b) {
            char ca = (*a >= 'A' && *a <= 'Z') ? (char)(*a + 32) : *a;
            char cb = (*b >= 'A' && *b <= 'Z') ? (char)(*b + 32) : *b;
            if (ca != cb) break;
            ++a; ++b;
        }
        if (*a == 0 && *b == 0) {
            std::snprintf(out, outn, "%s/%s", dir[0] ? dir : ".", e->d_name);
            ok = true;
            break;
        }
    }
    closedir(d);
    return ok;
}

// Map a Windows path to a host path. A drive prefix selects the root: D: is
// the CD when TS_CD names one, and every other drive is the install root.
// Then each component is resolved case-insensitively.
inline bool host_path(const char *win, char *out, size_t outn) {
    if (!win || !*win) return false;
    char buf[512];
    std::snprintf(buf, sizeof buf, "%s", win);
    char *p = buf;
    const char *base = ts_root();
    if (p[0] && p[1] == ':') {
        if (path_is_cd_drive(p)) base = ts_cd_root();
        p += 2;
    }
    while (*p == '\\' || *p == '/') ++p;
    std::snprintf(out, outn, "%s", base);
    char *save = 0;
    for (char *tok = strtok_r(p, "\\/", &save); tok; tok = strtok_r(0, "\\/", &save)) {
        if (!*tok) continue;
        if (!strcmp(tok, ".") || !strcmp(tok, "..")) continue;
        char next[512];
        if (!resolve_ci(out, tok, next, sizeof next)) {
            // Not present under any spelling: keep the literal name so a caller
            // that is about to create the file still gets a usable path.
            std::snprintf(next, sizeof next, "%s/%s", out, tok);
        }
        std::snprintf(out, outn, "%s", next);
    }
    return true;
}

struct HostFile { std::FILE *f; unsigned long size; int used; };
inline HostFile *file_table() { static HostFile t[64]; return t; }
inline u32 &file_serial() { static u32 n = 0; return n; }

inline HostFile *file_slot(u32 h) {
    u32 i = h - 0x00080000u;
    if (i >= 64u) return 0;
    HostFile *f = &file_table()[i];
    return f->used ? f : 0;
}

TS_BEGIN(CreateFileA)
    const char *name = ld_cstr(arg(0));
    u32 access = arg(1), disp = arg(4);
    char hp[512];
    host_path(name, hp, sizeof hp);
    // GENERIC_WRITE is bit 0x40000000; CREATE_ALWAYS/OPEN_ALWAYS create.
    const char *mode = (access & 0x40000000u) ? ((disp == 2u) ? "wb" : "r+b") : "rb";
    if (mode[0] == 'r' && mode[1] == '+' && !std::fopen(hp, "rb")) mode = "wb";
    std::FILE *f = std::fopen(hp, mode);
    if (!f && (access & 0x40000000u)) f = std::fopen(hp, "wb");
    if (!f) {
        std::printf("win32stubs: CreateFileA(\"%s\") -> INVALID_HANDLE_VALUE "
                    "(no %s)\n", name, hp);
        state().file_misses++;
        stub_done(7, 0xffffffffu);
    } else {
        HostFile *t = file_table();
        u32 slot = 64u;
        for (u32 i = 0; i < 64u; ++i) if (!t[i].used) { slot = i; break; }
        if (slot == 64u) { std::fclose(f); stub_done(7, 0xffffffffu); }
        else {
            t[slot].f = f;
            t[slot].used = 1;
            std::fseek(f, 0, SEEK_END);
            long sz = std::ftell(f);
            t[slot].size = sz < 0 ? 0 : (unsigned long)sz;
            std::fseek(f, 0, SEEK_SET);
            std::printf("win32stubs: CreateFileA(\"%s\") -> %s (%lu bytes, "
                        "handle 0x%08x)\n", name, hp, t[slot].size,
                        0x00080000u + slot);
            state().file_opens++;
            stub_done(7, 0x00080000u + slot);
        }
    }
TS_END

TS_BEGIN(ReadFile)
    u32 h = arg(0), buf = arg(1), want = arg(2), got = arg(3);
    HostFile *f = file_slot(h);
    u32 n = 0;
    if (f && want) {
        static u8 tmp[65536];
        u32 cap = want < sizeof tmp ? want : (u32)sizeof tmp;
        n = (u32)std::fread(tmp, 1, cap, f->f);
        model_wr(buf, tmp, n);
    }
    if (got) st32(got, n);
    // TS_FILE_TRACE -- what each read actually returned, and where the file
    // pointer was afterwards. A short read at a non-EOF position is the shape
    // of a file-layer defect, and the caller's next move (a size computed from
    // the bytes it expected) is otherwise impossible to attribute.
    if (std::getenv("TS_FILE_TRACE"))
        std::fprintf(stderr, "win32stubs: ReadFile(h=0x%08x want=%u) -> %u "
                             "(pos=%ld, step=%lu)\n", h, want, n,
                     f ? std::ftell(f->f) : -1L, mach().steps);
    stub_done(5, 1u);
TS_END

TS_BEGIN(WriteFile)
    u32 h = arg(0), buf = arg(1), want = arg(2), written = arg(3);
    HostFile *f = file_slot(h);
    u32 n = 0;
    if (f && want) {
        static u8 tmp[65536];
        u32 cap = want < sizeof tmp ? want : (u32)sizeof tmp;
        model_rd(tmp, buf, cap);
        n = (u32)std::fwrite(tmp, 1, cap, f->f);
        if ((unsigned long)std::ftell(f->f) > f->size) f->size = (unsigned long)std::ftell(f->f);
    }
    if (written) st32(written, n);
    stub_done(5, 1u);
TS_END

TS_BEGIN(CloseHandle)
    HostFile *f = file_slot(arg(0));
    if (f) {
        std::fclose(f->f);
        f->used = 0;
        f->f = 0;
    }
    stub_done(1, 1u);
TS_END

TS_BEGIN(GetFileSize)
    HostFile *f = file_slot(arg(0));
    u32 hi = arg(1);
    if (hi) st32(hi, 0u);
    stub_done(2, f ? (u32)f->size : 0xffffffffu);
TS_END

TS_BEGIN(SetFilePointer)
    HostFile *f = file_slot(arg(0));
    long lo = (long)arg(1);
    u32 method = arg(3);
    int whence = method == 0u ? SEEK_SET : (method == 1u ? SEEK_CUR : SEEK_END);
    if (!f) { stub_done(4, 0xffffffffu); }
    else {
        std::fseek(f->f, lo, whence);
        long pos = std::ftell(f->f);
        stub_done(4, pos < 0 ? 0xffffffffu : (u32)pos);
    }
TS_END

TS_RET(SetEndOfFile, 1, 1u)
TS_RET(FlushFileBuffers, 1, 1u)
TS_RET(FlushInstructionCache, 3, 1u)
TS_RET(SetFileTime, 4, 1u)
TS_RET(GetFileTime, 4, 1u)
TS_RET(GetFileInformationByHandle, 2, 1u)

TS_BEGIN(DeleteFileA)
    char hp[512];
    if (host_path(ld_cstr(arg(0)), hp, sizeof hp)) std::remove(hp);
    stub_done(1, 1u);
TS_END

TS_BEGIN(GetFileAttributesA)
    char hp[512];
    struct stat st;
    u32 r = 0xffffffffu;
    if (host_path(ld_cstr(arg(0)), hp, sizeof hp) && stat(hp, &st) == 0)
        r = S_ISDIR(st.st_mode) ? 0x10u : 0x20u;
    stub_done(1, r);
TS_END

TS_BEGIN(GetOverlappedResult)
    u32 got = arg(2);
    if (got) st32(got, 0u);
    stub_done(4, 1u);
TS_END

TS_BEGIN(FindFirstFileA)
    // One match, named after the pattern's extension: a caller looking for
    // *.mix is handed tsdata.mix rather than a name from another directory.
    const char *pat = ld_cstr(arg(0));
    const char *dot = std::strrchr(pat, '.');
    u32 p = arg(1);
    for (u32 i = 0; i < 0x140u; ++i) st8(p + i, 0);
    st32(p + 0u, 0x20u);                 // dwFileAttributes
    st32(p + 0x1cu, 0u);                 // nFileSizeLow
    const char *base = "tsdata";
    u32 i = 0;
    for (; base[i]; ++i) st8(p + 0x2cu + i, (u8)base[i]);
    if (dot && dot[1]) {
        u32 j = 0;
        for (; dot[j] && i + j < 260u; ++j) st8(p + 0x2cu + i + j, (u8)dot[j]);
    }
    stub_done(2, 0x00090000u);
TS_END

TS_RET(FindNextFileA, 2, 0u)   // one match, then no more
TS_RET(FindClose, 1, 1u)

TS_BEGIN(GetFullPathNameA)
    u32 src = arg(0), cch = arg(1), buf = arg(2), part = arg(3);
    u32 len = c_strlen(src);
    if (cch && buf) c_strncpy(buf, src, cch);
    if (part) {
        const char *s = ld_cstr(src);
        const char *b = std::strrchr(s, '\\');
        st32(part, buf + (u32)((b ? b + 1 : s) - s));
    }
    stub_done(4, len);
TS_END

TS_BEGIN(GetCurrentDirectoryA)
    u32 cch = arg(0), buf = arg(1);
    const char *d = "C:\\TS";
    if (cch) c_strncpy(buf, lit(d), cch);
    stub_done(2, (u32)std::strlen(d));
TS_END

TS_RET(SetCurrentDirectoryA, 1, 1u)
TS_RET(CreateDirectoryA, 2, 1u)

// -- environment -------------------------------------------------------------
TS_BEGIN(GetEnvironmentVariableA) stub_done(3, 0u); TS_END

TS_BEGIN(GetEnvironmentStrings)
    static u32 p = 0;
    if (!p) { p = alloc_block(4u, 4u); st32(p, 0u); }
    stub_done(0, p);
TS_END
TS_RET(FreeEnvironmentStringsA, 1, 1u)
TS_RET(SetEnvironmentVariableA, 2, 1u)

// -- locale and code pages ---------------------------------------------------
// The CRT's locale initialisation is a long chain of these, and it aborts on
// a failure. Everything below therefore succeeds and reports a single-byte,
// US-ASCII/CP1252 world, which is what the shipped build targets.
TS_RET(GetACP, 0, 1252u)
TS_RET(GetOEMCP, 0, 437u)
TS_RET(GetUserDefaultLCID, 0, 0x0409u)
TS_RET(IsValidCodePage, 1, 1u)
TS_RET(IsValidLocale, 2, 1u)
TS_RET(EnumSystemLocalesA, 2, 1u)

TS_BEGIN(GetCPInfo)
    u32 p = arg(1);
    for (u32 i = 0; i < 18u; ++i) st8(p + i, 0);
    st32(p + 0u, 1u);       // MaxCharSize
    st8(p + 4u, '?');       // DefaultChar[0]
    stub_done(2, 1u);
TS_END

inline void classify(u32 src, int count, u32 out) {
    for (int i = 0; i < count; ++i) {
        u8 c = ld8(src + (u32)i);
        u16 t = 0;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) t = 0x0100u;  // C1_ALPHA
        else if (c >= '0' && c <= '9') t = 0x0004u;                         // C1_DIGIT
        else if (c == ' ') t = 0x0008u;                                     // C1_SPACE
        else if (c < 32u) t = 0x0020u;                                      // C1_CNTRL
        else t = 0x0010u;                                                   // C1_PUNCT
        st16(out + (u32)i * 2u, t);
    }
}

TS_BEGIN(GetStringTypeA)
    int n = (int)arg(3);
    u32 src = arg(2);
    if (n < 0) n = (int)c_strlen(src) + 1;
    classify(src, n, arg(4));
    stub_done(5, 1u);
TS_END

TS_BEGIN(GetStringTypeW)
    int n = (int)arg(3);
    u32 src = arg(2);
    if (n < 0) n = (int)w_strlen(src) + 1;
    for (int i = 0; i < n; ++i) {
        u16 c = ld16(src + (u32)i * 2u);
        u16 t = 0;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) t = 0x0100u;
        else if (c >= '0' && c <= '9') t = 0x0004u;
        else if (c == ' ') t = 0x0008u;
        st16(arg(4) + (u32)i * 2u, t);
    }
    stub_done(4, 1u);
TS_END

TS_BEGIN(MultiByteToWideChar)
    u32 src = arg(2), dst = arg(4);
    int n = (int)arg(3), cap = (int)arg(5);
    if (n < 0) n = (int)c_strlen(src) + 1;
    if (!dst || cap == 0) { stub_done(6, (u32)n); }
    else {
        int k = (n < cap) ? n : cap;
        for (int i = 0; i < k; ++i) st16(dst + (u32)i * 2u, ld8(src + (u32)i));
        stub_done(6, (u32)k);
    }
TS_END

TS_BEGIN(WideCharToMultiByte)
    u32 src = arg(2), dst = arg(4);
    int n = (int)arg(3), cap = (int)arg(5);
    if (n < 0) n = (int)w_strlen(src) + 1;
    if (!dst || cap == 0) { stub_done(8, (u32)n); }
    else {
        int k = (n < cap) ? n : cap;
        for (int i = 0; i < k; ++i) {
            u16 w = ld16(src + (u32)i * 2u);
            st8(dst + (u32)i, (u8)(w < 256u ? w : (u16)'?'));
        }
        stub_done(8, (u32)k);
    }
TS_END

TS_BEGIN(CompareStringA)
    int c1 = (int)arg(3), c2 = (int)arg(4);
    u32 s1 = arg(2), s2 = arg(4);
    if (c1 < 0) c1 = (int)c_strlen(s1) + 1;
    if (c2 < 0) c2 = (int)c_strlen(s2) + 1;
    int n = (c1 < c2) ? c1 : c2;
    int r = 0;
    for (int i = 0; i < n && !r; ++i) {
        u8 a = ld8(s1 + (u32)i), b = ld8(s2 + (u32)i);
        if (a != b) r = (a < b) ? -1 : 1;
    }
    if (!r && c1 != c2) r = (c1 < c2) ? -1 : 1;
    stub_done(6, (u32)(r < 0 ? 1 : (r > 0 ? 3 : 2)));
TS_END

TS_BEGIN(GetLocaleInfoA)
    u32 type = arg(1), buf = arg(2), cch = arg(3);
    const char *v = "enu";
    if (type == 0x00000059u) v = "en";
    else if (type == 0x00000001u) v = "0409";
    u32 len = (u32)std::strlen(v) + 1u;
    if (cch && buf) c_strncpy(buf, lit(v), cch);
    stub_done(4, len);
TS_END

TS_BEGIN(LCMapStringA)
    u32 src = arg(2), dst = arg(4), cap = arg(5);
    int n = (int)arg(3);
    if (n < 0) n = (int)c_strlen(src) + 1;
    if (!dst || cap == 0) { stub_done(6, (u32)n); }
    else {
        u32 k = ((u32)n < cap) ? (u32)n : cap;
        for (u32 i = 0; i < k; ++i) st8(dst + i, ld8(src + i));
        stub_done(6, k);
    }
TS_END

TS_BEGIN(GetDateFormatA)
    u32 buf = arg(3), cch = arg(4);
    const char *d = "09/01/1999";
    if (cch && buf) c_strncpy(buf, lit(d), cch);
    stub_done(5, (u32)std::strlen(d) + 1u);
TS_END

TS_BEGIN(GetTimeFormatA)
    u32 buf = arg(3), cch = arg(4);
    const char *d = "12:00:00";
    if (cch && buf) c_strncpy(buf, lit(d), cch);
    stub_done(5, (u32)std::strlen(d) + 1u);
TS_END

// -- strings -----------------------------------------------------------------
TS_BEGIN(lstrlen) stub_done(1, c_strlen(arg(0))); TS_END
TS_BEGIN(lstrlenW) stub_done(1, w_strlen(arg(0))); TS_END

TS_BEGIN(lstrcpy)
    u32 d = arg(0);
    c_strcpy(d, arg(1));
    stub_done(2, d);
TS_END

TS_BEGIN(lstrcat)
    u32 d = arg(0), s = arg(1);
    c_strcpy(d + c_strlen(d), s);
    stub_done(2, d);
TS_END

// -- resources ---------------------------------------------------------------
// The game's dialog templates and its entire UI string table live in
// LANGUAGE.DLL's .rsrc, not in GAME.EXE (GAME.EXE has two dialogs; LANGUAGE.DLL
// has 60, including 206 -- the message box -- and 240). The old stub returned a
// non-null zero block, so every DLGTEMPLATE had cdit=0 and every LoadStringA
// failed: the game's message boxes had no text and no buttons. This reads the
// module's own resource directory from the file the model opened for it, so
// the control IDs and the strings are the shipped ones.
//
// The tree is IMAGE_RESOURCE_DIRECTORY (type -> name -> language); every entry
// offset is relative to the resource directory's RVA. Only integer IDs are
// matched -- every resource this binary asks for by ID is one.
struct ResImage {
    u8 *data; u32 size;
    u32 res_rva;
    unsigned nsec;
    u32 sec_va[16], sec_vs[16], sec_raw[16], sec_rs[16];
    char name[64];
};
inline ResImage *res_images() { static ResImage r[4]; return r; }
inline unsigned &res_image_n() { static unsigned n = 0; return n; }

inline u32 res_rd16(const u8 *p) { return (u32)p[0] | ((u32)p[1] << 8); }
inline u32 res_rd32(const u8 *p) {
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

// The handle -> module name lookup is defined with the module table below.
inline const char *module_name_of(u32 handle);

inline u32 res_off(const ResImage *im, u32 rva) {
    for (unsigned i = 0; i < im->nsec; ++i) {
        u32 span = im->sec_vs[i] > im->sec_rs[i] ? im->sec_vs[i] : im->sec_rs[i];
        if (rva >= im->sec_va[i] && rva < im->sec_va[i] + span) {
            u32 off = im->sec_raw[i] + (rva - im->sec_va[i]);
            return off < im->size ? off : 0xffffffffu;
        }
    }
    return 0xffffffffu;
}

inline ResImage *res_load(const char *module) {
    if (!module || !*module) return 0;
    for (unsigned i = 0; i < res_image_n(); ++i)
        if (!std::strcmp(res_images()[i].name, module)) return &res_images()[i];
    if (res_image_n() >= 4u) return 0;
    char hp[512];
    if (!host_path(module, hp, sizeof hp)) return 0;
    std::FILE *f = std::fopen(hp, "rb");
    if (!f) return 0;
    std::fseek(f, 0, SEEK_END);
    long sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (sz < 0x40) { std::fclose(f); return 0; }
    u8 *d = (u8 *)std::malloc((size_t)sz);
    if (!d) { std::fclose(f); return 0; }
    if (std::fread(d, 1, (size_t)sz, f) != (size_t)sz) {
        std::free(d); std::fclose(f); return 0;
    }
    std::fclose(f);
    u32 pe = res_rd32(d + 0x3c);
    if (pe + 24u > (u32)sz || res_rd32(d + pe) != 0x00004550u) {
        std::free(d); return 0;
    }
    unsigned nsec = res_rd16(d + pe + 6);
    unsigned optsz = res_rd16(d + pe + 20);
    u32 opt = pe + 24u;
    u32 magic = res_rd16(d + opt);
    u32 dd = opt + (magic == 0x10bu ? 96u : 112u);
    u32 rrva = res_rd32(d + dd + 16u);
    if (nsec > 16u) nsec = 16u;
    ResImage &im = res_images()[res_image_n()];
    im.data = d; im.size = (u32)sz; im.res_rva = rrva; im.nsec = nsec;
    u32 st = opt + optsz;
    for (unsigned i = 0; i < nsec; ++i) {
        u32 o = st + i * 40u;
        im.sec_vs[i] = res_rd32(d + o + 8u);
        im.sec_va[i] = res_rd32(d + o + 12u);
        im.sec_rs[i] = res_rd32(d + o + 16u);
        im.sec_raw[i] = res_rd32(d + o + 20u);
    }
    unsigned j = 0;
    for (; module[j] && j + 1u < sizeof(im.name); ++j) im.name[j] = module[j];
    im.name[j] = 0;
    res_image_n() = res_image_n() + 1u;
    return &im;
}

inline u32 res_find_entry(ResImage *im, u32 dir, u32 id, bool any) {
    const u8 *d = im->data;
    if (dir + 16u > im->size) return 0u;
    unsigned n = res_rd16(d + dir + 12u) + res_rd16(d + dir + 14u);
    for (unsigned i = 0; i < n; ++i) {
        u32 e = dir + 16u + i * 8u;
        if (e + 8u > im->size) return 0u;
        if (!any) {
            u32 nm = res_rd32(d + e);
            if (nm & 0x80000000u) continue;   // a named entry, not an integer ID
            if (nm != id) continue;
        }
        return e;
    }
    return 0u;
}

inline bool res_lookup(ResImage *im, u32 type_id, u32 name_id,
                       u32 *out_rva, u32 *out_size) {
    u32 base = res_off(im, im->res_rva);
    if (base == 0xffffffffu) return false;
    u32 e = res_find_entry(im, base, type_id, false);
    if (!e) return false;
    u32 off = res_rd32(im->data + e + 4u);
    if (!(off & 0x80000000u)) return false;   // a type is a directory
    u32 dir = base + (off & 0x7fffffffu);
    e = res_find_entry(im, dir, name_id, false);
    if (!e) return false;
    off = res_rd32(im->data + e + 4u);
    u32 de;
    if (off & 0x80000000u) {
        dir = base + (off & 0x7fffffffu);
        e = res_find_entry(im, dir, 0u, true);   // first language
        if (!e) return false;
        off = res_rd32(im->data + e + 4u);
        if (off & 0x80000000u) return false;
    }
    de = base + off;
    if (de + 16u > im->size) return false;
    *out_rva = res_rd32(im->data + de);
    *out_size = res_rd32(im->data + de + 4u);
    return true;
}

struct ResHandle { u32 magic; u32 module; u32 rva; u32 size; };
enum { RES_HANDLE_MAGIC = 0x52535243u };   // "RSRC"

TS_BEGIN(FindResourceA)
    // FindResourceA(hModule, lpName, lpType). An integer resource ID is passed
    // as a small value cast to a pointer, which is how FUN_0046ed10 asks for
    // RT_DIALOG 206; anything at or above 0x10000 is a string pointer, and no
    // caller here uses one.
    u32 hmod = arg(0), name = arg(1), type = arg(2);
    const char *mn = module_name_of(hmod);
    ResImage *im = mn ? res_load(mn) : 0;
    u32 rva = 0u, size = 0u;
    if (!im || name >= 0x10000u || type >= 0x10000u ||
        !res_lookup(im, type, name, &rva, &size)) {
        stub_done(3, 0u);
        return;
    }
    u32 h = alloc_block(16u, 4u);
    st32(h + 0u, (u32)RES_HANDLE_MAGIC);
    st32(h + 4u, hmod);
    st32(h + 8u, rva);
    st32(h + 12u, size);
    stub_done(3, h);
TS_END

TS_BEGIN(LoadResource)
    // The resource's bytes, copied into model memory. Nothing is freed: a
    // dialog template must stay valid for as long as the dialog lives, and the
    // bump allocator is the model's lifetime.
    u32 hp = arg(1);
    if (!hp || ld32(hp) != (u32)RES_HANDLE_MAGIC) { stub_done(2, 0u); return; }
    const char *mn = module_name_of(ld32(hp + 4u));
    ResImage *im = mn ? res_load(mn) : 0;
    u32 off = im ? res_off(im, ld32(hp + 8u)) : 0xffffffffu;
    u32 size = ld32(hp + 12u);
    if (!im || off == 0xffffffffu || off + size > im->size) {
        stub_done(2, 0u);
        return;
    }
    u32 dst = alloc_block(size ? size : 1u, 4u);
    for (u32 i = 0; i < size; ++i) st8(dst + i, im->data[off + i]);
    stub_done(2, dst);
TS_END

TS_BEGIN(LockResource) stub_done(1, arg(0)); TS_END

// -- drives, volumes, filesystem information ---------------------------------
TS_BEGIN(GetDriveTypeA)
    // GetCDClass (0x4a7600) finds the CD by asking this for every letter, and
    // Force_CD_Available (0x4dc670) then requires the answer to carry the
    // label "TS%d" and to hold TS%d.dsk. Reporting DRIVE_CDROM for the drive
    // backed by TS_CD is what lets the game see its disc; every other drive is
    // a fixed disk.
    stub_done(1, path_is_cd_drive(ld_cstr(arg(0))) ? 5u : 3u);   // DRIVE_CDROM / DRIVE_FIXED
TS_END

TS_BEGIN(GetDiskFreeSpaceA)
    st32(arg(1), 0x10000u); st32(arg(2), 512u);
    st32(arg(3), 0x10000u); st32(arg(4), 0x40000u);
    stub_done(5, 1u);
TS_END

TS_BEGIN(GetLogicalDriveStringsA)
    // "C:\D:\" when a CD is configured. FUN_00647a70 walks this list and
    // checks every DRIVE_CDROM entry for the Tiberian Sun label.
    u32 buf = arg(1), cch = arg(0), n = 4u;
    if (buf && cch >= 4u) {
        st8(buf, 'C'); st8(buf + 1u, ':'); st8(buf + 2u, '\\'); st8(buf + 3u, 0);
        if (ts_cd_root()[0]) {
            if (cch >= 8u) {
                st8(buf + 4u, 'D'); st8(buf + 5u, ':'); st8(buf + 6u, '\\'); st8(buf + 7u, 0);
                n = 8u;
            }
        }
        st8(buf + n, 0);
    }
    stub_done(2, n);
TS_END

TS_BEGIN(GetVolumeInformationA)
    // The volume label is what identifies the disc: the game formats "TS%d"
    // and compares it with this answer before it will use the drive. Only the
    // CD root carries that label; a fixed drive gets a neutral one.
    u32 name = arg(1);
    const char *label = path_is_cd_drive(ld_cstr(arg(0))) ? "TS1" : "TSVOL";
    if (name) c_strcpy(name, lit(label));
    stub_done(8, 1u);
TS_END
TS_RET(SetVolumeLabelA, 2, 1u)

TS_BEGIN(GlobalMemoryStatus)
    u32 p = arg(0);
    for (u32 i = 0; i < 32u; ++i) st8(p + i, 0);
    st32(p + 0u, 32u);
    st32(p + 4u, 50u);              // dwMemoryLoad
    st32(p + 8u, 0x08000000u);      // dwTotalPhys
    st32(p + 12u, 0x04000000u);     // dwAvailPhys
    st32(p + 16u, 0x20000000u);     // dwTotalPageFile
    st32(p + 20u, 0x10000000u);     // dwAvailPageFile
    st32(p + 24u, 0x80000000u);     // dwTotalVirtual
    st32(p + 28u, 0x40000000u);     // dwAvailVirtual
    stub_done(1, 0u);
TS_END

// -- file times --------------------------------------------------------------
TS_BEGIN(CompareFileTime)
    u32 a = arg(0), b = arg(1);
    if (ld32(a + 4u) != ld32(b + 4u)) stub_done(2, ld32(a + 4u) < ld32(b + 4u) ? 0xffffffffu : 1u);
    else stub_done(2, ld32(a) < ld32(b) ? 0xffffffffu : (ld32(a) > ld32(b) ? 1u : 0u));
TS_END

TS_BEGIN(FileTimeToLocalFileTime)
    u32 d = arg(1);
    st32(d, ld32(arg(0))); st32(d + 4u, ld32(arg(0) + 4u));
    stub_done(2, 1u);
TS_END

TS_BEGIN(FileTimeToSystemTime) write_systemtime(arg(1)); stub_done(2, 1u); TS_END
TS_BEGIN(SystemTimeToFileTime)
    u32 p = arg(1);
    st32(p, 0x2b0e8b00u); st32(p + 4u, 0x01bf0000u);
    stub_done(2, 1u);
TS_END

TS_BEGIN(DosDateTimeToFileTime)
    u32 p = arg(2);
    st32(p, 0x2b0e8b00u); st32(p + 4u, 0x01bf0000u);
    stub_done(3, 1u);
TS_END

TS_BEGIN(FileTimeToDosDateTime)
    st16(arg(1), 0x2b81u); st16(arg(2), 0x6000u);
    stub_done(3, 1u);
TS_END

// -- the rest of kernel32 ----------------------------------------------------
TS_RET(AddAtomW, 1, 0xc000u)
TS_RET(BackupSeek, 6, 1u)
TS_RET(CloseProfileUserMapping, 0, 1u)
TS_RET(ClearCommBreak, 1, 1u)
TS_RET(ClearCommError, 3, 1u)
TS_RET(DeviceIoControl, 8, 1u)
TS_RET(EscapeCommFunction, 2, 1u)
TS_RET(FatalAppExitA, 2, 0u)
TS_RET(GetCommMask, 2, 1u)
TS_RET(GetCommModemStatus, 2, 1u)
TS_RET(GetCommState, 2, 1u)
TS_RET(PurgeComm, 2, 1u)
TS_RET(SetCommBreak, 1, 1u)
TS_RET(SetCommState, 2, 1u)
TS_RET(SetCommTimeouts, 2, 1u)
TS_RET(SetupComm, 3, 1u)

// ===========================================================================
// USER32
// ===========================================================================
// What has to be REAL here is not the window; it is the state a program reads
// back out of the window system. A window procedure finds its own `this` with
// GetWindowLongA(hwnd, GWL_USERDATA), posts to itself with PostMessageA, and
// drives its frame loop with PeekMessageA returning 0 when the queue is empty.
// A constant returned from any of those makes the program read someone else's
// value, so this section keeps a window table, a class table and a message
// queue, and the calls operate on them.
//
// The one thing that is genuinely fake is the window itself: nothing is
// rasterised and no message ever arrives from the operating system. Every
// message a run sees is one the program posted to itself, which is exactly the
// set the program can be expected to handle.

// 128 slots, not 32: a dialog owns its controls as window records too, and the
// game creates and destroys a dialog per message box. A slot is freed by
// DestroyWindow, which also frees the dialog's children.
enum { MAX_WINDOWS = 128, MAX_CLASSES = 16, MAX_MSGS = 128 };

// The screen this model reports, and the single value every geometry answer is
// derived from: GetSystemMetrics, GetClientRect, the created window's size and
// the DirectDraw display mode all agree, because a program that finds them
// disagreeing takes its "this machine cannot do it" path.
enum { SCREEN_W = 800, SCREEN_H = 600, SCREEN_BPP = 16 };

// How many times GetMessageA answers an empty queue with WM_NULL before it
// answers 0. A real GetMessage blocks; this model cannot, and 0 is WM_QUIT, so
// the loop has to be given something benign to run on and then an honest end.
enum { IDLE_POLLS = 64 };

struct WinRec {
    u32 hwnd;        // 0 marks a free slot
    u32 proc;        // GWL_WNDPROC (a dialog's DLGPROC lives here too)
    u32 userdata;    // GWL_USERDATA: where a program stores its `this`
    u32 style, exstyle, parent, id, atom, shown;
    u32 enabled;     // EnableWindow state; a disabled control refuses input
    u32 is_dialog;   // created by CreateDialog*, so messages go to `proc`
    u32 has_template;  // controls came from the DLGTEMPLATE, not lazily
    u8  extra[64];   // window extra bytes: DWLP_MSGRESULT 0, DLGPROC 4, USER 8
    i32 x, y, cx, cy;   // window rect, in screen coordinates
    char text[64];
};
inline WinRec *wins() { static WinRec w[MAX_WINDOWS]; return w; }

// Dialogs and controls get their own handle range: 0x0002xxxx. The main
// window's range is 0x0001xxxx (CreateWindowExA), and 0x0003xxxx is a device
// context. A handle only ever has to be distinct and stable, but keeping the
// ranges apart makes a trace readable.
inline u32 &win_handle_serial() { static u32 s = 0; return s; }
inline u32 new_win_handle() { return 0x00020001u + (win_handle_serial()++); }

inline void win_zero_extra(WinRec *w) {
    for (unsigned i = 0; i < sizeof(w->extra); ++i) w->extra[i] = 0;
}
inline u32 win_extra_get(WinRec *w, u32 off) {
    if (!w || off + 4u > sizeof(w->extra)) return 0u;
    return (u32)w->extra[off] | ((u32)w->extra[off + 1u] << 8) |
           ((u32)w->extra[off + 2u] << 16) | ((u32)w->extra[off + 3u] << 24);
}
inline void win_extra_set(WinRec *w, u32 off, u32 v) {
    if (!w || off + 4u > sizeof(w->extra)) return;
    w->extra[off] = (u8)v;
    w->extra[off + 1u] = (u8)(v >> 8);
    w->extra[off + 2u] = (u8)(v >> 16);
    w->extra[off + 3u] = (u8)(v >> 24);
}

struct ClassRec { u32 atom; u32 proc; char name[48]; };
inline ClassRec *classes() { static ClassRec c[MAX_CLASSES]; return c; }
inline unsigned &class_n() { static unsigned n = 0; return n; }

// The message queue. Circular, fixed, and never full in practice: a program
// that posts without pumping drops its posts, which is what a real queue's
// limit does too.
struct MsgRec { u32 hwnd, msg, wparam, lparam; };
inline MsgRec *msgs() { static MsgRec m[MAX_MSGS]; return m; }
inline unsigned &msg_head() { static unsigned h = 0; return h; }
inline unsigned &msg_tail() { static unsigned t = 0; return t; }
inline bool &quit_posted() { static bool q = false; return q; }
inline unsigned &idle_polls() { static unsigned n = 0; return n; }

// The primary window: the first one created. It is what an hwnd of 0 in
// GetMessageA/PeekMessageA means, and what GetActiveWindow and friends report.
inline u32 &wnd_handle() { static u32 h = 0; return h; }
inline u32 &wnd_proc() { static u32 p = 0; return p; }
inline u32 &paint_dc() { static u32 d = 0; return d; }

inline WinRec *win_find(u32 hwnd) {
    if (!hwnd) return 0;
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i)
        if (wins()[i].hwnd == hwnd) return &wins()[i];
    return 0;
}

inline WinRec *win_alloc() {
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i)
        if (wins()[i].hwnd == 0) return &wins()[i];
    return 0;
}

inline void win_copy(char *dst, unsigned cap, u32 src) {
    unsigned i = 0;
    if (src) for (; i + 1u < cap && ld8(src + i); ++i) dst[i] = (char)ld8(src + i);
    dst[i] = 0;
}

inline void msg_push(u32 hwnd, u32 msg, u32 wparam, u32 lparam) {
    unsigned tail = msg_tail();
    if (tail - msg_head() >= (unsigned)MAX_MSGS) return;   // full: dropped
    MsgRec &m = msgs()[tail % (unsigned)MAX_MSGS];
    m.hwnd = hwnd; m.msg = msg; m.wparam = wparam; m.lparam = lparam;
    msg_tail() = tail + 1u;
}

// A filter of (first, last) == (0, 0) means every message, which is the form
// the game's own posts are read back with. Otherwise both bounds must match,
// and so must hwnd when one is named. `remove` is PeekMessageA's PM_REMOVE:
// false leaves the message at the head of the queue for the next call.
inline bool msg_pop(MsgRec *out, u32 hwnd, u32 first, u32 last, bool remove) {
    unsigned head = msg_head();
    if (head == msg_tail()) return false;
    const MsgRec &m = msgs()[head % (unsigned)MAX_MSGS];
    if (hwnd && m.hwnd != hwnd) return false;
    if ((first || last) && (m.msg < first || m.msg > last)) return false;
    if (out) *out = m;
    if (remove) msg_head() = head + 1u;
    return true;
}

// MSG is 28 bytes: hwnd, message, wParam, lParam, time, pt.x, pt.y.
inline void msg_fill(u32 p, const MsgRec &m) {
    st32(p + 0u, m.hwnd);
    st32(p + 4u, m.msg);
    st32(p + 8u, m.wparam);
    st32(p + 12u, m.lparam);
    st32(p + 16u, tick());
    st32(p + 20u, 0u);
    st32(p + 24u, 0u);
}

// -- dialogs ----------------------------------------------------------------
// A dialog is a window whose procedure is a DLGPROC and whose controls are
// child windows. The template is the module's own RT_DIALOG resource, read from
// the DLL on disk (see module_resources below), so the control IDs are the
// shipped ones and a GetDlgItem for an ID the template does not declare
// answers 0 exactly as Windows does.
inline void win_copy_w(char *dst, unsigned cap, u32 src) {
    unsigned i = 0;
    if (src) for (; i + 1u < cap && ld16(src + i * 2u); ++i)
        dst[i] = (char)(ld16(src + i * 2u) & 0xffu);
    dst[i] = 0;
}

inline WinRec *win_child_find(u32 parent, u32 id) {
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i) {
        WinRec &w = wins()[i];
        if (w.hwnd && w.parent == parent && w.id == id) return &w;
    }
    return 0;
}

inline WinRec *win_child_new(u32 parent, u32 id) {
    WinRec *w = win_alloc();
    if (!w) return 0;
    w->hwnd = new_win_handle();
    w->proc = 0u; w->userdata = 0u;
    w->style = 0u; w->exstyle = 0u;
    w->parent = parent; w->id = id; w->atom = 0u;
    w->shown = 0u; w->enabled = 1u; w->is_dialog = 0u; w->has_template = 0u;
    win_zero_extra(w);
    w->x = w->y = w->cx = w->cy = 0;
    w->text[0] = 0;
    return w;
}

// GetDlgItem's answer: an existing control of this dialog, or (for a dialog
// whose template was not available) a stable handle allocated on demand. A
// dialog with a parsed template answers 0 for an ID it does not declare, which
// is what the real API does and what a caller testing for an optional control
// relies on.
inline u32 dlg_item(u32 hdlg, u32 id) {
    WinRec *d = win_find(hdlg);
    if (!d) return 0u;
    WinRec *c = win_child_find(hdlg, id);
    if (c) return c->hwnd;
    if (d->has_template) return 0u;
    c = win_child_new(hdlg, id);
    return c ? c->hwnd : 0u;
}

// The default button a scripted click presses: IDOK when the template declares
// it, else the button whose text is "OK", else nothing. A dialog with only a
// Cancel button (a progress window) is deliberately not clicked: cancelling it
// would be a different program, not this one.
inline u32 dlg_default_button(u32 hdlg) {
    u32 fallback = 0u;
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i) {
        WinRec &w = wins()[i];
        if (!w.hwnd || w.parent != hdlg) continue;
        if (w.atom != 0x80u && w.atom != 0u) continue;   // 0x80 = "Button"
        if (w.id == 1u) return 1u;
        if (!fallback) fallback = w.id;
        const char *t = w.text;
        if ((t[0] == 'O' || t[0] == 'o') && (t[1] == 'K' || t[1] == 'k') && !t[2])
            return w.id;
    }
    // An untemplated dialog has no text to recognise; IDOK is the only safe
    // guess and only when a control with that id was actually asked for.
    WinRec *c = win_child_find(hdlg, 1u);
    if (c) return 1u;
    (void)fallback;
    return 0u;
}

// Focus and foreground are single handles, as they are on one desktop.
inline u32 &focus_handle() { static u32 h = 0; return h; }
inline u32 &foreground_handle() { static u32 h = 0; return h; }

// Call the program's own window procedure. The game's wndproc is recovered
// code, so it is entered as a __stdcall callee: arguments right to left, a
// return address under them, and ESP restored afterwards whatever the callee
// did with its own frame. Running the program's window code is the difference
// between "a window object exists" and "the program's window code ran".
inline u32 call_wndproc(u32 proc, u32 hwnd, u32 msg, u32 wparam, u32 lparam) {
    if (!proc) return 0u;
    u32 sp = ESP.d;
    push32(lparam);
    push32(wparam);
    push32(msg);
    push32(hwnd);
    ts_call_at(proc, 0x00fed000u);
    u32 r = EAX.d;
    ESP.d = sp;
    return r;
}

TS_BEGIN(RegisterClassA)
    // WNDCLASSA: +0 style, +4 lpfnWndProc, +8 cbClsExtra, +12 cbWndExtra,
    // +16 hInstance, +20 hIcon, +24 hCursor, +28 hbrBackground, +32 lpszMenuName,
    // +36 lpszClassName. The class name is kept, not just the proc: a program
    // that registers two classes and creates the second one by name has to get
    // the second class's procedure.
    u32 wc = arg(0);
    u32 atom = 0;
    unsigned n = class_n();
    if (wc && n < (unsigned)MAX_CLASSES) {
        ClassRec &c = classes()[n];
        c.atom = 0xc000u + n;
        c.proc = ld32(wc + 4u);
        win_copy(c.name, sizeof(c.name), ld32(wc + 36u));
        class_n() = n + 1u;
        wnd_proc() = c.proc;   // the most recent registration, for an unregistered class
        atom = c.atom;
    }
    stub_done(1, atom);
TS_END

// The class a CreateWindowExA call names: by name, or by atom when the caller
// passes MAKEINTATOM (a value below 0x10000).
inline u32 class_proc(u32 clsname, u32 *atom) {
    if (clsname && clsname < 0x10000u) {
        for (unsigned i = 0; i < class_n(); ++i)
            if (classes()[i].atom == clsname) { *atom = classes()[i].atom; return classes()[i].proc; }
        return 0u;
    }
    if (clsname) {
        const char *want = ld_cstr(clsname);
        for (unsigned i = 0; i < class_n(); ++i)
            if (!std::strcmp(classes()[i].name, want)) { *atom = classes()[i].atom; return classes()[i].proc; }
    }
    *atom = 0u;
    return 0u;
}

TS_BEGIN(CreateWindowExA)
    // CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y,
    //                nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam).
    // The handle is non-zero, distinct per window and stable for the life of
    // the window -- a program stores it and compares against it for the rest of
    // the run.
    static u32 serial = 0;
    u32 clsname = arg(1), title = arg(2), style = arg(3);
    i32 x = (i32)arg(4), y = (i32)arg(5);
    i32 cx = (i32)arg(6), cy = (i32)arg(7);
    u32 parent = arg(8), id = arg(9);
    u32 h = 0;
    WinRec *w = win_alloc();
    if (w) {
        u32 atom = 0;
        u32 proc = class_proc(clsname, &atom);
        if (!proc) proc = wnd_proc();
        // CW_USEDEFAULT is a request, not a size.
        if (cx <= 0 || cx == (i32)0x80000000) cx = (i32)SCREEN_W;
        if (cy <= 0 || cy == (i32)0x80000000) cy = (i32)SCREEN_H;
        if (x == (i32)0x80000000) x = 0;
        if (y == (i32)0x80000000) y = 0;
        h = 0x00010001u + (serial++);
        w->hwnd = h;
        w->proc = proc;
        w->userdata = 0u;
        w->style = style;
        w->exstyle = arg(0);
        w->parent = parent;
        w->id = id;
        w->atom = atom;
        w->shown = 0u;
        w->enabled = 1u;
        w->is_dialog = 0u;
        w->has_template = 0u;
        win_zero_extra(w);
        w->x = x; w->y = y; w->cx = cx; w->cy = cy;
        win_copy(w->text, sizeof(w->text), title);
        if (!wnd_handle()) wnd_handle() = h;
        // Windows sends WM_NCCREATE and then WM_CREATE from inside
        // CreateWindowEx, and a program that initialises in WM_CREATE expects
        // it. WM_CREATE carries a CREATESTRUCTA in lParam: 48 bytes, the
        // pointer the caller passed last and the geometry it asked for.
        u32 cs = zero_block(48u, 4u);
        st32(cs + 0u, arg(11));      // lpCreateParams
        st32(cs + 4u, arg(10));      // hInstance
        st32(cs + 8u, id);           // hMenu
        st32(cs + 12u, parent);      // hwndParent
        st32(cs + 16u, (u32)cy);     // cy
        st32(cs + 20u, (u32)cx);     // cx
        st32(cs + 24u, (u32)y);
        st32(cs + 28u, (u32)x);
        st32(cs + 32u, style);
        st32(cs + 36u, title);
        st32(cs + 40u, clsname);
        st32(cs + 44u, arg(0));      // dwExStyle
        call_wndproc(proc, h, 0x0001u, 0u, cs);   // WM_CREATE
        // WM_ACTIVATEAPP is a POSTED message, not a synchronous send: the
        // window manager queues it when the window becomes the active one, and
        // the program's own message loop delivers it. A program that gates its
        // frame loop on "am I the active application" -- as this game does,
        // spinning on the flag its WM_ACTIVATEAPP handler sets -- never leaves
        // that spin if the message is not queued. wParam is TRUE for the
        // window being activated. lParam is the thread id; nothing reads it.
        msg_push(h, 0x001cu, 1u, 0u);             // WM_ACTIVATEAPP, active
    }
    stub_done(12, h);
TS_END

TS_BEGIN(GetMessageA)
    // GetMessageA(MSG*, hwnd, wMsgFilterMin, wMsgFilterMax). It does NOT return
    // 0 to mean "nothing to do": 0 IS WM_QUIT, and a program that reads it ends
    // its loop. So an empty queue is answered with a benign WM_NULL for a
    // bounded number of idle polls and only then with 0, which terminates a run
    // without telling the loop on its first iteration that the program is over.
    u32 p = arg(0);
    MsgRec m;
    if (msg_pop(&m, arg(1), arg(2), arg(3), true)) {
        msg_fill(p, m);
        stub_done(4, m.msg == 0x0012u ? 0u : 1u);   // 0 for WM_QUIT
    } else if (quit_posted() || idle_polls() >= (unsigned)IDLE_POLLS) {
        quit_posted() = false;
        idle_polls() = 0u;
        stub_done(4, 0u);
    } else {
        ++idle_polls();
        m.hwnd = wnd_handle(); m.msg = 0u; m.wparam = 0u; m.lparam = 0u;
        msg_fill(p, m);
        stub_done(4, 1u);
    }
TS_END

TS_BEGIN(PeekMessageA)
    // PeekMessageA(MSG*, hwnd, min, max, wRemoveMsg). 0 means "no message", and
    // that is the return value that keeps a game loop running: it says "carry
    // on with this frame", not "quit".
    u32 p = arg(0);
    MsgRec m;
    if (msg_pop(&m, arg(1), arg(2), arg(3), (arg(4) & 1u) != 0u)) {
        msg_fill(p, m);
        stub_done(5, 1u);
    } else {
        stub_done(5, 0u);
    }
TS_END

TS_BEGIN(TranslateMessage)
    // A WM_KEYDOWN becomes a WM_CHAR for the same window; nothing else is
    // translated. The return value is whether a message was posted, so a caller
    // that pumps until TranslateMessage says "no" still terminates.
    u32 p = arg(0);
    u32 msg = ld32(p + 4u), r = 0u;
    if (msg == 0x0100u || msg == 0x0104u) {   // WM_KEYDOWN, WM_SYSKEYDOWN
        u32 vk = ld32(p + 8u);
        if (vk >= 0x20u && vk < 0x7fu) {
            msg_push(ld32(p + 0u), 0x0102u, vk, ld32(p + 12u));   // WM_CHAR
            r = 1u;
        }
    }
    stub_done(1, r);
TS_END

TS_BEGIN(PostQuitMessage)
    // WM_QUIT goes through the queue like any other message, because a program
    // that pumps with PeekMessageA never sees a return value from GetMessage.
    msg_push(wnd_handle(), 0x0012u, arg(0), 0u);
    quit_posted() = true;
    stub_done(1, 0u);
TS_END

TS_BEGIN(DefWindowProcA)
    // The messages that have a defined answer. Everything else returns 0, which
    // is what DefWindowProc answers for the messages a program is expected to
    // ignore.
    u32 hwnd = arg(0), msg = arg(1), r = 0u;
    if (msg == 0x0081u) {          // WM_NCCREATE
        r = 1u;
    } else if (msg == 0x0014u) {   // WM_ERASEBKGND
        r = 1u;
    } else if (msg == 0x0020u) {   // WM_SETCURSOR
        r = 1u;
    } else if (msg == 0x0010u) {   // WM_CLOSE
        WinRec *w = win_find(hwnd);
        if (w) w->hwnd = 0u;
    } else if (msg == 0x0024u) {   // WM_GETMINMAXINFO
        u32 p = arg(3);
        if (p) { st32(p + 8u, SCREEN_W); st32(p + 12u, SCREEN_H); }
    }
    stub_done(4, r);
TS_END

TS_BEGIN(PostMessageA)
    // A program that posts to itself and pumps is the only source of messages
    // this model has, so the post has to be remembered.
    u32 hwnd = arg(0);
    msg_push(hwnd ? hwnd : wnd_handle(), arg(1), arg(2), arg(3));
    stub_done(4, 1u);
TS_END

TS_BEGIN(SendMessageA)
    // SendMessage is synchronous: the answer is the window procedure's return
    // value, and a caller that sends a private query reads it.
    u32 hwnd = arg(0), msg = arg(1), wp = arg(2), lp = arg(3), r = 0u;
    if (hwnd == 0xffffu) {                    // HWND_BROADCAST
        for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i)
            if (wins()[i].hwnd)
                r = call_wndproc(wins()[i].proc, wins()[i].hwnd, msg, wp, lp);
    } else {
        WinRec *w = win_find(hwnd);
        u32 proc = w ? w->proc : (hwnd == wnd_handle() ? wnd_proc() : 0u);
        r = call_wndproc(proc, hwnd, msg, wp, lp);
    }
    stub_done(4, r);
TS_END

TS_BEGIN(CallWindowProcA)
    // CallWindowProcA(lpPrevWndProc, hWnd, Msg, wParam, lParam) -- the subclass
    // chain. Without this the game's own window subclassing calls nothing.
    stub_done(5, call_wndproc(arg(0), arg(1), arg(2), arg(3), arg(4)));
TS_END

TS_BEGIN(DispatchMessageA)
    // MSG: +0 hwnd, +4 message, +8 wParam, +12 lParam. The procedure is looked
    // up through the window the message names, so a message posted to a second
    // window reaches that window's procedure and not the main one.
    u32 p = arg(0);
    u32 hwnd = ld32(p + 0u), msg = ld32(p + 4u);
    WinRec *w = win_find(hwnd);
    u32 proc = w ? w->proc : wnd_proc();
    stub_done(1, call_wndproc(proc, hwnd, msg, ld32(p + 8u), ld32(p + 12u)));
TS_END

// The slot a handle names, or -1. Two callers derive a per-window device
// context from it, so it lives with the table rather than inside one stub.
inline unsigned win_index(u32 hwnd) {
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i)
        if (wins()[i].hwnd == hwnd) return i;
    return 0xffffffffu;
}

// A window's device context: one per window, stable for its life, because a
// program selects a font or a bitmap into it once and expects the selection to
// still be there on the next paint.
inline u32 win_dc(u32 hwnd) {
    unsigned i = win_index(hwnd);
    return 0x00030001u + (i == 0xffffffffu ? 0u : i);
}

TS_BEGIN(GetClientRect)
    // The client area is the window's own size. A program that asks for it and
    // gets a different rectangle than the one it created sizes its surfaces and
    // its back buffers for the wrong area.
    u32 p = arg(1);
    WinRec *w = win_find(arg(0));
    st32(p + 0u, 0u);
    st32(p + 4u, 0u);
    st32(p + 8u, w ? (u32)w->cx : (u32)SCREEN_W);
    st32(p + 12u, w ? (u32)w->cy : (u32)SCREEN_H);
    stub_done(2, 1u);
TS_END

TS_BEGIN(GetWindowRect)
    u32 p = arg(1);
    WinRec *w = win_find(arg(0));
    st32(p + 0u, w ? (u32)w->x : 0u);
    st32(p + 4u, w ? (u32)w->y : 0u);
    st32(p + 8u, w ? (u32)(w->x + w->cx) : (u32)SCREEN_W);
    st32(p + 12u, w ? (u32)(w->y + w->cy) : (u32)SCREEN_H);
    stub_done(2, 1u);
TS_END

TS_BEGIN(BeginPaint)
    // PAINTSTRUCT: +0 hdc, +8 fErase, +12 rcPaint. The DC is the window's own,
    // so a selection made in one paint survives into the next.
    u32 p = arg(1), hwnd = arg(0);
    u32 dc = win_dc(hwnd);
    if (!paint_dc()) paint_dc() = dc;
    if (p) {
        for (u32 i = 0; i < 64u; ++i) st8(p + i, 0);
        st32(p + 0u, dc);
        st32(p + 8u, 1u);            // fErase: the background is erased
        WinRec *w = win_find(hwnd);
        st32(p + 12u, 0u);
        st32(p + 16u, 0u);
        st32(p + 20u, w ? (u32)w->cx : (u32)SCREEN_W);
        st32(p + 24u, w ? (u32)w->cy : (u32)SCREEN_H);
    }
    stub_done(2, dc);
TS_END

TS_BEGIN(EndPaint) stub_done(2, 1u); TS_END

TS_BEGIN(GetDC)
    stub_done(1, win_dc(arg(0)));
TS_END
TS_RET(ReleaseDC, 2, 1u)

inline bool &in_paint() { static bool b = false; return b; }

TS_BEGIN(ShowWindow)
    // The previous visibility, which is what ShowWindow returns: a program that
    // restores the old state after a temporary hide reads it.
    u32 hwnd = arg(0), cmd = arg(1);
    WinRec *w = win_find(hwnd);
    u32 prev = 0u;
    if (w) {
        prev = w->shown;
        w->shown = (cmd == 0u /*SW_HIDE*/) ? 0u : 1u;
    }
    stub_done(2, prev);
TS_END

TS_BEGIN(UpdateWindow)
    // UpdateWindow sends WM_PAINT now if the window is dirty. The guard keeps a
    // WM_PAINT handler that invalidates and updates again from recursing: real
    // Windows coalesces paints, and this is the cheapest faithful form of that.
    u32 hwnd = arg(0);
    WinRec *w = win_find(hwnd);
    if (w && w->proc && !in_paint()) {
        in_paint() = true;
        call_wndproc(w->proc, hwnd, 0x000fu, 0u, 0u);   // WM_PAINT
        in_paint() = false;
    }
    stub_done(1, 1u);
TS_END

TS_RET(InvalidateRect, 3, 1u)
TS_RET(ValidateRect, 2, 1u)
TS_RET(RedrawWindow, 5, 1u)

TS_BEGIN(IsWindow) stub_done(1, win_find(arg(0)) ? 1u : 0u); TS_END

TS_BEGIN(IsWindowEnabled)
    WinRec *w = win_find(arg(0));
    stub_done(1, (w && w->enabled) ? 1u : 0u);
TS_END

TS_BEGIN(EnableWindow)
    // The previous state is the return value, and a disabled control refuses
    // input: a program that disables a button and re-enables it later reads
    // both transitions back.
    u32 hwnd = arg(0), enable = arg(1);
    WinRec *w = win_find(hwnd);
    u32 prev = 0u;
    if (w) { prev = w->enabled; w->enabled = enable ? 1u : 0u; }
    stub_done(2, prev);
TS_END

TS_BEGIN(DestroyWindow)
    // A dialog owns its controls: destroying it frees their slots too, or the
    // game's message-box loop exhausts the table after a few dozen boxes.
    u32 hwnd = arg(0);
    WinRec *w = win_find(hwnd);
    if (w) {
        for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i)
            if (wins()[i].hwnd && wins()[i].parent == hwnd) wins()[i].hwnd = 0u;
        w->hwnd = 0u;
        if (wnd_handle() == hwnd) wnd_handle() = 0u;
        if (focus_handle() == hwnd) focus_handle() = 0u;
        if (foreground_handle() == hwnd) foreground_handle() = 0u;
        stub_done(1, 1u);
    } else {
        stub_done(1, 0u);
    }
TS_END

TS_RET(CloseWindow, 1, 1u)
TS_RET(BringWindowToTop, 1, 1u)

TS_BEGIN(MoveWindow)
    u32 hwnd = arg(0);
    WinRec *w = win_find(hwnd);
    if (w) {
        w->x = (i32)arg(1); w->y = (i32)arg(2);
        w->cx = (i32)arg(3); w->cy = (i32)arg(4);
    }
    stub_done(6, w ? 1u : 0u);
TS_END

TS_BEGIN(SetWindowPos)
    // SWP_NOSIZE 0x0001, SWP_NOMOVE 0x0002.
    u32 hwnd = arg(0), flags = arg(6);
    WinRec *w = win_find(hwnd);
    if (w) {
        if (!(flags & 0x0002u)) { w->x = (i32)arg(2); w->y = (i32)arg(3); }
        if (!(flags & 0x0001u)) { w->cx = (i32)arg(4); w->cy = (i32)arg(5); }
    }
    stub_done(7, w ? 1u : 0u);
TS_END

TS_BEGIN(SetActiveWindow)  stub_done(1, wnd_handle()); TS_END
TS_BEGIN(SetForegroundWindow)
    // The handle is remembered: a dialog that asks whether it is the
    // foreground window after asking to become it must be told yes.
    foreground_handle() = arg(0) ? arg(0) : wnd_handle();
    stub_done(1, 1u);
TS_END
TS_BEGIN(GetForegroundWindow)
    stub_done(0, foreground_handle() ? foreground_handle() : wnd_handle());
TS_END
TS_BEGIN(GetActiveWindow)     stub_done(0, wnd_handle()); TS_END
TS_BEGIN(GetFocus)
    stub_done(0, focus_handle() ? focus_handle() : wnd_handle());
TS_END
TS_BEGIN(SetFocus)
    // Returns the window that had focus. A dialog's WM_INITDIALOG ends by
    // focusing one of its controls, and the answer is read back.
    u32 prev = focus_handle();
    WinRec *w = win_find(arg(0));
    focus_handle() = w ? w->hwnd : 0u;
    stub_done(1, prev);
TS_END
TS_BEGIN(GetCapture)          stub_done(0, wnd_handle()); TS_END
TS_BEGIN(SetCapture)          stub_done(1, wnd_handle()); TS_END
TS_RET(ReleaseCapture, 0, 1u)
TS_BEGIN(GetTopWindow) stub_done(0, wnd_handle()); TS_END

TS_BEGIN(GetParent)
    WinRec *w = win_find(arg(0));
    stub_done(1, w ? w->parent : 0u);
TS_END

TS_BEGIN(GetWindow)
    // GW_OWNER (4) is the only relation this model has.
    u32 hwnd = arg(0), cmd = arg(1);
    WinRec *w = win_find(hwnd);
    stub_done(2, (w && cmd == 4u) ? w->parent : 0u);
TS_END

TS_RET(ChildWindowFromPoint, 3, 0u)
TS_BEGIN(WindowFromPoint) stub_done(2, wnd_handle()); TS_END

TS_BEGIN(FindWindowA)
    // Either argument may be null, which means "any". Returning the first match
    // is what makes a program that looks for its own window find it.
    const char *cls = arg(0) ? ld_cstr(arg(0)) : 0;
    char want[64];
    win_copy(want, sizeof(want), arg(1));
    u32 found = 0u;
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS && !found; ++i) {
        WinRec &w = wins()[i];
        if (!w.hwnd) continue;
        if (cls) {
            bool ok = false;
            for (unsigned c = 0; c < class_n(); ++c)
                if (classes()[c].atom == w.atom && !std::strcmp(classes()[c].name, cls)) ok = true;
            if (!ok) continue;
        }
        if (arg(1) && std::strcmp(w.text, want)) continue;
        found = w.hwnd;
    }
    stub_done(2, found);
TS_END

TS_BEGIN(EnumChildWindows)
    // Call the program's callback for every child of hwndParent, in table
    // order. A dialog's WM_INITDIALOG enumerates its controls to build its
    // own record of them, so an enumeration that calls nothing leaves that
    // record empty. A callback that returns 0 stops the walk.
    u32 parent = arg(0), cb = arg(1), lp = arg(2), r = 1u;
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i) {
        u32 child = wins()[i].hwnd;
        if (!child || wins()[i].parent != parent) continue;
        if (!cb) continue;
        u32 sp = ESP.d;
        push32(lp);
        push32(child);
        ts_call_at(cb, 0x00fed000u);
        u32 keep = EAX.d;
        ESP.d = sp;
        if (!keep) { r = 0u; break; }
    }
    stub_done(3, r);
TS_END
TS_RET(GetMenu, 1, 0x00060001u)
TS_RET(GetWindowContextHelpId, 1, 0u)
TS_RET(WaitForInputIdle, 2, 0u)
TS_RET(TranslateAccelerator, 3, 0u)
TS_RET(WinHelpA, 4, 1u)
TS_BEGIN(LoadStringA)
    // LoadStringA(hInstance, uID, lpBuffer, cchBufferMax). RT_STRING is a
    // table of blocks of 16 strings; block id = id/16 + 1, and each string is
    // a WORD length followed by that many UTF-16 code units (no terminator).
    // The game's UI text is ASCII, so the ANSI form is the low byte.
    u32 hmod = arg(0), id = arg(1), buf = arg(2), cch = arg(3);
    const char *mn = module_name_of(hmod);
    ResImage *im = mn ? res_load(mn) : 0;
    u32 rva = 0u, size = 0u, n = 0u;
    if (im && res_lookup(im, 6u, id / 16u + 1u, &rva, &size)) {
        u32 off = res_off(im, rva);
        if (off != 0xffffffffu) {
            const u8 *d = im->data;
            u32 p = off, end = off + size;
            for (u32 k = 0; k < 16u && p + 2u <= end; ++k) {
                u32 len = res_rd16(d + p);
                p += 2u;
                if (p + len * 2u > end) break;
                if (k == id % 16u) {
                    if (buf && cch) {
                        u32 j = 0;
                        for (; j + 1u < cch && j < len; ++j)
                            st8(buf + j, (u8)(res_rd16(d + p + j * 2u) & 0xffu));
                        st8(buf + j, 0u);
                        n = j;
                    }
                    break;
                }
                p += len * 2u;
            }
        }
    }
    stub_done(4, n);
TS_END
TS_RET(GetUpdateRect, 3, 1u)
TS_RET(GetAsyncKeyState, 1, 0u)
TS_RET(GetKeyState, 1, 0u)
TS_RET(MapVirtualKeyA, 2, 0u)
TS_RET(ToAscii, 5, 1u)
TS_RET(RegisterHotKey, 4, 1u)
TS_RET(SetTimer, 4, 1u)
TS_RET(KillTimer, 2, 1u)
TS_RET(IntersectRect, 3, 1u)

TS_BEGIN(DrawTextA)
    // One line of text at the font height GetTextMetricsA reports. A caller
    // that measures to lay out rows reads the height, not the truth.
    stub_done(5, 16u);
TS_END

TS_BEGIN(AdjustWindowRectEx)
    // The window rect is the client rect grown by the frame, so the two cannot
    // disagree. A 4-pixel frame and a 19-pixel caption, which is what
    // GetSystemMetrics reports for SM_CXFRAME and SM_CYCAPTION.
    u32 p = arg(0);
    if (p) {
        i32 l = (i32)ld32(p + 0u), t = (i32)ld32(p + 4u);
        i32 r = (i32)ld32(p + 8u), b = (i32)ld32(p + 12u);
        st32(p + 0u, (u32)(l - 4));
        st32(p + 4u, (u32)(t - 23));
        st32(p + 8u, (u32)(r + 4));
        st32(p + 12u, (u32)(b + 4));
    }
    stub_done(4, 1u);
TS_END

TS_BEGIN(SetWindowTextA)
    WinRec *w = win_find(arg(0));
    if (w) { win_copy(w->text, sizeof(w->text), arg(1)); stub_done(2, 1u); }
    else stub_done(2, 0u);
TS_END

// A variable-length DLGTEMPLATE/DLGITEMTEMPLATE field: 0 means absent, 0xffff
// means an ordinal (one more WORD), otherwise a NUL-terminated UTF-16 string.
inline u32 dlg_var_end(u32 p) {
    u16 v = (u16)ld16(p);
    if (v == 0u) return p + 2u;
    if (v == 0xffffu) return p + 4u;
    while (ld16(p)) p += 2u;
    return p + 2u;
}

// Create the dialog's window record and the controls its DLGTEMPLATE declares.
// The template is the shipped RT_DIALOG resource (see the resource reader), so
// the IDs, classes and styles are the original ones. Returns 0 when the window
// table is full.
inline u32 dialog_create(u32 tpl, u32 parent, u32 proc, u32 init) {
    WinRec *d = win_alloc();
    if (!d) return 0u;
    u32 style = 0x40u, exstyle = 0u, cdit = 0u;
    i32 x = 0, y = 0, cx = 0, cy = 0;
    if (tpl) {
        style = ld32(tpl);
        exstyle = ld32(tpl + 4u);
        cdit = ld16(tpl + 8u);
        x = (i32)(i16)ld16(tpl + 10u);
        y = (i32)(i16)ld16(tpl + 12u);
        cx = (i32)(i16)ld16(tpl + 14u);
        cy = (i32)(i16)ld16(tpl + 16u);
    }
    d->hwnd = new_win_handle();
    d->proc = proc;
    d->userdata = 0u;
    d->style = style;
    d->exstyle = exstyle;
    d->parent = parent;
    d->id = 0u;
    d->atom = 0u;
    d->shown = 0u;
    d->enabled = 1u;
    d->is_dialog = 1u;
    d->has_template = cdit ? 1u : 0u;
    win_zero_extra(d);
    d->x = x; d->y = y; d->cx = cx; d->cy = cy;
    d->text[0] = 0;
    u32 h = d->hwnd;

    if (tpl && cdit) {
        u32 p = tpl + 18u;
        p = dlg_var_end(p);          // menu
        p = dlg_var_end(p);          // window class
        p = dlg_var_end(p);          // title
        if (style & 0x40u) {         // DS_SETFONT: point size + typeface
            p += 2u;
            while (ld16(p)) p += 2u;
            p += 2u;
        }
        for (u32 k = 0; k < cdit; ++k) {
            p = (p + 3u) & ~3u;
            u32 cstyle = ld32(p);
            u32 cexstyle = ld32(p + 4u);
            i32 ccx = (i32)(i16)ld16(p + 8u);
            i32 ccy = (i32)(i16)ld16(p + 10u);
            i32 ccxx = (i32)(i16)ld16(p + 12u);
            i32 ccyy = (i32)(i16)ld16(p + 14u);
            u32 cid = ld16(p + 16u);
            u32 q = p + 18u;
            u32 catom = 0u;
            u16 cv = (u16)ld16(q);
            if (cv == 0xffffu) { catom = ld16(q + 2u); q += 4u; }
            else if (cv == 0u) q += 2u;
            else q = dlg_var_end(q);
            char ctext[64];
            ctext[0] = 0;
            u16 tv = (u16)ld16(q);
            if (tv == 0xffffu) q += 4u;
            else if (tv == 0u) q += 2u;
            else { win_copy_w(ctext, sizeof ctext, q); q = dlg_var_end(q); }
            u16 cextra = (u16)ld16(q);
            p = q + 2u + cextra;
            WinRec *c = win_child_new(h, cid);
            if (!c) break;
            c->style = cstyle;
            c->exstyle = cexstyle;
            c->atom = catom;
            c->shown = (cstyle & 0x10000000u) ? 1u : 0u;   // WS_VISIBLE
            c->x = ccx; c->y = ccy; c->cx = ccxx; c->cy = ccyy;
            for (unsigned t = 0; t + 1u < sizeof(c->text) && ctext[t]; ++t)
                c->text[t] = ctext[t];
        }
    }

    // The real API sends WM_INITDIALOG before it returns, and a dialog that
    // initialises its controls there expects to have run. wParam is the control
    // that takes the default focus (0 here) and lParam is dwInitParam.
    call_wndproc(proc, h, 0x0110u, 0u, init);

    // A scripted input source: press the dialog's default button. The command
    // is queued, not delivered here, so the game's own PeekMessage/GetMessage/
    // DispatchMessage loop carries it to the dialog procedure.
    u32 btn = dlg_default_button(h);
    if (btn) msg_push(h, 0x0111u, btn, dlg_item(h, btn));
    return h;
}

TS_BEGIN(SetDlgItemTextA)
    u32 h = dlg_item(arg(0), arg(1));
    WinRec *w = win_find(h);
    if (w) { win_copy(w->text, sizeof(w->text), arg(2)); stub_done(3, 1u); }
    else stub_done(3, 0u);
TS_END

TS_BEGIN(GetDlgItemTextA)
    u32 h = dlg_item(arg(0), arg(1)), buf = arg(2), cch = arg(3), n = 0;
    WinRec *w = win_find(h);
    const char *src = w ? w->text : "";
    while (src[n]) ++n;
    if (buf && cch) {
        u32 i = 0;
        for (; i + 1u < cch && src[i]; ++i) st8(buf + i, (u8)src[i]);
        st8(buf + i, 0u);
    }
    stub_done(4, n);
TS_END

TS_BEGIN(SendDlgItemMessageA)
    // The control messages a dialog's own code sends: text, and a button's
    // check state. Everything else answers 0, which is what a control with no
    // such message returns.
    u32 h = dlg_item(arg(0), arg(1)), msg = arg(2), wp = arg(3), lp = arg(4), r = 0u;
    WinRec *w = win_find(h);
    if (w) {
        if (msg == 0x000cu) {                       // WM_SETTEXT
            win_copy(w->text, sizeof(w->text), lp);
            r = 1u;
        } else if (msg == 0x000du) {                // WM_GETTEXT
            u32 n = 0;
            while (w->text[n]) ++n;
            if (lp) {
                u32 i = 0;
                for (; i + 1u < wp && w->text[i]; ++i) st8(lp + i, (u8)w->text[i]);
                st8(lp + i, 0u);
            }
            r = n;
        } else if (msg == 0x000eu) {                // WM_GETTEXTLENGTH
            while (w->text[r]) ++r;
        } else if (msg == 0x00f1u) {                // BM_SETCHECK
            w->extra[0] = (u8)wp;
        } else if (msg == 0x00f0u) {                // BM_GETCHECK
            r = w->extra[0];
        }
    }
    stub_done(5, r);
TS_END

TS_BEGIN(EndDialog)
    // Destroy the dialog and its controls. The result is not kept: this model
    // has no modal loop that would read it.
    u32 h = arg(0);
    WinRec *w = win_find(h);
    if (w) {
        for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i)
            if (wins()[i].hwnd && wins()[i].parent == h) wins()[i].hwnd = 0u;
        w->hwnd = 0u;
    }
    stub_done(2, 1u);
TS_END

TS_BEGIN(IsDialogMessage)
    // The scripted input is a WM_COMMAND, not a keystroke, so the dialog
    // manager has nothing to translate and must let the message through to
    // DispatchMessageA. Answering 1 here would swallow the command.
    (void)arg(0); (void)arg(1);
    stub_done(2, 0u);
TS_END

TS_BEGIN(GetDlgItem)
    stub_done(2, dlg_item(arg(0), arg(1)));
TS_END

TS_BEGIN(GetNextDlgTabItem)
    // The next control of the same dialog after hctl, in table order.
    u32 hdlg = arg(0), hctl = arg(1);
    u32 r = 0u;
    bool after = (hctl == 0u);
    for (unsigned i = 0; i < (unsigned)MAX_WINDOWS && !r; ++i) {
        WinRec &w = wins()[i];
        if (!w.hwnd || w.parent != hdlg) continue;
        if (after) r = w.hwnd;
        if (w.hwnd == hctl) after = true;
    }
    stub_done(3, r);
TS_END

TS_BEGIN(GetDlgCtrlID)
    WinRec *w = win_find(arg(0));
    stub_done(1, w ? w->id : 0u);
TS_END

TS_BEGIN(MessageBoxA)
    // A message box is the program telling a human why it is about to stop.
    // Nothing here can show one, so the text goes to the run's log -- that log
    // is the only place the reason survives -- and the answer is IDOK, because
    // the alternative is a caller that treats the failure as fatal.
    // The call site and the raw pointers are printed too: the text is fetched
    // at runtime from the game's own string table, so on a boot that has not
    // filled that table the "message" is whatever the model holds at the
    // pointer, and the only way to tell that apart from a real message is to
    // see the address it came from.
    std::printf("win32stubs: MessageBoxA(hwnd 0x%08x, text 0x%08x, caption 0x%08x, "
                "type 0x%08x) called from 0x%08x (ebp 0x%08x esp 0x%08x) -> IDOK\n",
                arg(0), arg(1), arg(2), arg(3), ld32(ESP.d), EBP.d, ESP.d);
    print_escaped("text", arg(1), 64u);
    print_escaped("caption", arg(2), 64u);
    stub_done(4, 1u);
TS_END

TS_RET(MessageBoxIndirectA, 1, 1u)

// The DLGTEMPLATE a dialog-name API asks for, copied into model memory from
// the module's RT_DIALOG resource. Returns 0 when the module has no such
// template -- the caller then takes its real failure path instead of being
// handed a zeroed block that reads as a dialog with no controls.
inline u32 dialog_template_by_id(u32 hinst, u32 name) {
    const char *mn = module_name_of(hinst);
    ResImage *im = mn ? res_load(mn) : 0;
    u32 rva = 0u, size = 0u;
    if (!im || name >= 0x10000u || !res_lookup(im, 5u, name, &rva, &size)) return 0u;
    u32 off = res_off(im, rva);
    if (off == 0xffffffffu) return 0u;
    u32 dst = alloc_block(size ? size : 1u, 4u);
    for (u32 i = 0; i < size; ++i) st8(dst + i, im->data[off + i]);
    return dst;
}

TS_BEGIN(DialogBoxParamA)
    // A modal dialog would block inside this call until EndDialog; the game's
    // observed startup path uses CreateDialog*, so this creates the dialog (so
    // WM_INITDIALOG runs) and answers IDOK without a modal loop.
    u32 hinst = arg(0), name = arg(1), parent = arg(2), proc = arg(3), init = arg(4);
    u32 tpl = dialog_template_by_id(hinst, name);
    u32 h = tpl ? dialog_create(tpl, parent, proc, init) : 0u;
    if (h) {
        WinRec *w = win_find(h);
        if (w) {
            for (unsigned i = 0; i < (unsigned)MAX_WINDOWS; ++i)
                if (wins()[i].hwnd && wins()[i].parent == h) wins()[i].hwnd = 0u;
            w->hwnd = 0u;
        }
    }
    stub_done(5, 1u);
TS_END
TS_RET(DialogBoxIndirectParamA, 5, 1u)

TS_BEGIN(CreateDialogParamA)
    u32 hinst = arg(0), name = arg(1), parent = arg(2), proc = arg(3), init = arg(4);
    u32 tpl = dialog_template_by_id(hinst, name);
    stub_done(5, tpl ? dialog_create(tpl, parent, proc, init) : 0u);
TS_END

TS_BEGIN(CreateDialogIndirectParamA)
    // CreateDialogIndirectParamA(hInstance, lpTemplate, hWndParent,
    // lpDialogFunc, dwInitParam). Creates the dialog and its controls from the
    // template, sends WM_INITDIALOG synchronously, and returns the handle.
    stub_done(5, dialog_create(arg(1), arg(2), arg(3), arg(4)));
TS_END

TS_BEGIN(SetWindowLongA)
    // Per window, and the previous value is returned: a subclassing wrapper
    // saves what it replaced and restores it later, and a game stores its own
    // `this` in GWL_USERDATA and reads it back inside its window procedure.
    u32 hwnd = arg(0), idx = arg(1), val = arg(2), prev = 0u;
    WinRec *w = win_find(hwnd);
    if (!w && hwnd == wnd_handle()) w = win_find(wnd_handle());
    if (w) {
        // A non-negative index addresses the window's extra bytes. A dialog
        // procedure stores its context at DWLP_USER (8) and reads it back in
        // every message; without this, the first WM_COMMAND writes through a
        // null pointer.
        if (idx < 64u) { prev = win_extra_get(w, idx); win_extra_set(w, idx, val); }
        else if (idx == 0xfffffffcu) { prev = w->proc; w->proc = val; wnd_proc() = val; }
        else if (idx == 0xffffffebu) { prev = w->userdata; w->userdata = val; }
        else if (idx == 0xfffffff0u) { prev = w->style; w->style = val; }
        else if (idx == 0xfffffff4u) { prev = w->id; w->id = val; }
        else if (idx == 0xfffffff8u) { prev = w->parent; w->parent = val; }
    }
    stub_done(3, prev);
TS_END

TS_BEGIN(GetWindowLongA)
    u32 hwnd = arg(0), idx = arg(1), r = 0u;
    WinRec *w = win_find(hwnd);
    if (w) {
        if (idx < 64u) r = win_extra_get(w, idx);
        else if (idx == 0xfffffffcu) r = w->proc;
        else if (idx == 0xffffffebu) r = w->userdata;
        else if (idx == 0xfffffff0u) r = w->style;
        else if (idx == 0xfffffff4u) r = w->id;
        else if (idx == 0xfffffff8u) r = w->parent;
    }
    stub_done(2, r);
TS_END

TS_BEGIN(SetRect)
    u32 p = arg(0);
    st32(p, arg(1)); st32(p + 4u, arg(2)); st32(p + 8u, arg(3)); st32(p + 12u, arg(4));
    stub_done(5, 1u);
TS_END

TS_BEGIN(GetSystemMetrics)
    // The screen this model reports. A program that asks for the desktop size
    // before creating its window uses this to pick a resolution, so it has to
    // be the same 800x600 the window and the DirectDraw display mode use.
    u32 i = arg(0), r = 0u;
    switch (i) {
    case 0:  r = SCREEN_W; break;          // SM_CXSCREEN
    case 1:  r = SCREEN_H; break;          // SM_CYSCREEN
    case 2:  r = 17u; break;               // SM_CXVSCROLL
    case 3:  r = 17u; break;               // SM_CYHSCROLL
    case 4:  r = 19u; break;               // SM_CYCAPTION
    case 5:  r = 1u; break;                // SM_CXBORDER
    case 6:  r = 1u; break;                // SM_CYBORDER
    case 7:  r = 4u; break;                // SM_CXDLGFRAME
    case 8:  r = 4u; break;                // SM_CYDLGFRAME
    case 11: r = 32u; break;               // SM_CXICON
    case 12: r = 32u; break;               // SM_CYICON
    case 13: r = 32u; break;               // SM_CXCURSOR
    case 14: r = 32u; break;               // SM_CYCURSOR
    case 15: r = 19u; break;               // SM_CYMENU
    case 16: r = SCREEN_W; break;          // SM_CXFULLSCREEN
    case 17: r = SCREEN_H; break;          // SM_CYFULLSCREEN
    case 19: r = 1u; break;                // SM_MOUSEPRESENT
    case 20: r = 17u; break;               // SM_CYVSCROLL
    case 21: r = 17u; break;               // SM_CXHSCROLL
    case 28: r = 112u; break;              // SM_CXMIN
    case 29: r = 27u; break;               // SM_CYMIN
    case 30: r = 16u; break;               // SM_CXSIZE
    case 31: r = 16u; break;               // SM_CYSIZE
    case 32: r = 4u; break;                // SM_CXFRAME
    case 33: r = 4u; break;                // SM_CYFRAME
    case 34: r = 112u; break;              // SM_CXMINTRACK
    case 35: r = 27u; break;               // SM_CYMINTRACK
    case 36: r = 4u; break;                // SM_CXDOUBLECLK
    case 37: r = 4u; break;                // SM_CYDOUBLECLK
    case 38: r = 75u; break;               // SM_CXICONSPACING
    case 39: r = 75u; break;               // SM_CYICONSPACING
    case 43: r = 3u; break;                // SM_CMOUSEBUTTONS
    case 45: r = 2u; break;                // SM_CXEDGE
    case 46: r = 2u; break;                // SM_CYEDGE
    case 49: r = 16u; break;               // SM_CXSMICON
    case 50: r = 16u; break;               // SM_CYSMICON
    case 67: r = 1u; break;                // SM_CMONITORS
    case 68: r = 1u; break;                // SM_SAMEDISPLAYFORMAT
    case 70: r = 2u; break;                // SM_CXDRAG
    case 71: r = 2u; break;                // SM_CYDRAG
    default: r = 0u; break;
    }
    stub_done(1, r);
TS_END

TS_BEGIN(LoadCursorA)
    // A non-zero handle, distinct per resource, so a program that loads the
    // arrow and then the wait cursor and compares the two sees a difference.
    u32 res = arg(1);
    stub_done(2, 0x00040001u + (res & 0xffffu));
TS_END

TS_BEGIN(LoadIconA)
    u32 res = arg(1);
    stub_done(2, 0x00050001u + (res & 0xffffu));
TS_END

TS_BEGIN(SetCursor)
    // Returns the PREVIOUS cursor, which is what a program restores after a
    // temporary change.
    static u32 cur = 0x00040001u;
    u32 prev = cur;
    cur = arg(0) ? arg(0) : 0x00040001u;
    stub_done(1, prev);
TS_END

TS_BEGIN(ShowCursor)
    // Returns the new display count, which a program tests for >= 0.
    static i32 count = 0;
    count += (i32)arg(0) ? 1 : -1;
    stub_done(1, (u32)count);
TS_END

TS_BEGIN(GetCursorPos)
    static i32 cx = SCREEN_W / 2, cy = SCREEN_H / 2;
    u32 p = arg(0);
    st32(p + 0u, (u32)cx);
    st32(p + 4u, (u32)cy);
    stub_done(1, 1u);
TS_END

TS_BEGIN(SetCursorPos)
    // Remembered, because a program that warps the pointer and then asks where
    // it is expects its own answer.
    static i32 cx = SCREEN_W / 2, cy = SCREEN_H / 2;
    cx = (i32)arg(0);
    cy = (i32)arg(1);
    stub_done(2, 1u);
TS_END

TS_BEGIN(ClientToScreen)
    u32 p = arg(1);
    WinRec *w = win_find(arg(0));
    if (p) {
        st32(p + 0u, ld32(p + 0u) + (u32)(w ? w->x : 0));
        st32(p + 4u, ld32(p + 4u) + (u32)(w ? w->y : 0));
    }
    stub_done(2, 1u);
TS_END

TS_BEGIN(ScreenToClient)
    u32 p = arg(1);
    WinRec *w = win_find(arg(0));
    if (p) {
        st32(p + 0u, ld32(p + 0u) - (u32)(w ? w->x : 0));
        st32(p + 4u, ld32(p + 4u) - (u32)(w ? w->y : 0));
    }
    stub_done(2, 1u);
TS_END

TS_RET(FillRect, 3, 1u)

TS_BEGIN(GetWindowTextA)
    // The text the window was created with, or the last one set on it: a
    // program that sets its title and reads it back gets its own string.
    u32 buf = arg(1), cch = arg(2), n = 0;
    WinRec *w = win_find(arg(0));
    const char *src = w ? w->text : "";
    while (src[n]) ++n;
    if (buf && cch) {
        u32 i = 0;
        for (; i + 1u < cch && src[i]; ++i) st8(buf + i, (u8)src[i]);
        st8(buf + i, 0);
    }
    stub_done(3, n);
TS_END

TS_BEGIN(GetClassNameA)
    // The class this window was created from, which a program compares against
    // to tell its own windows from a dialog's. A template control's class is
    // the predefined atom its DLGITEMTEMPLATE names (0x80 Button, 0x81 Edit,
    // 0x82 Static, ...), and a dialog's is "Dialog".
    u32 buf = arg(1), cch = arg(2), n = 0;
    const char *name = "";
    WinRec *w = win_find(arg(0));
    if (w) {
        if (w->is_dialog) name = "Dialog";
        else if (w->atom == 0x80u) name = "Button";
        else if (w->atom == 0x81u) name = "Edit";
        else if (w->atom == 0x82u) name = "Static";
        else if (w->atom == 0x83u) name = "ListBox";
        else if (w->atom == 0x84u) name = "ScrollBar";
        else if (w->atom == 0x85u) name = "ComboBox";
        else for (unsigned i = 0; i < class_n(); ++i)
            if (classes()[i].atom == w->atom) name = classes()[i].name;
    }
    while (name[n]) ++n;
    if (buf && cch) {
        u32 i = 0;
        for (; i + 1u < cch && name[i]; ++i) st8(buf + i, (u8)name[i]);
        st8(buf + i, 0);
    }
    stub_done(3, n);
TS_END

TS_RET(GetKeyNameTextA, 4, 0u)
TS_BEGIN(CharNextA)
    stub_done(1, arg(0) + 1u);
TS_END
TS_BEGIN(CharPrevA)
    stub_done(2, arg(0) + 1u);
TS_END

TS_BEGIN(CharToOemBuffA)
    u32 n = arg(2);
    for (u32 i = 0; i < n; ++i) st8(arg(1) + i, ld8(arg(0) + i));
    stub_done(3, 1u);
TS_END

TS_BEGIN(wsprintfA)
    // wsprintfA is __cdecl and variadic: the caller pops the arguments, so this
    // stub discards only the return address and reads the rest without owning
    // it. It formats %[flags][width][l]s %c %d %i %u %x %X %p and %%; a
    // conversion it does not know is copied through with its flags, so a bad
    // format is visible in the output rather than silently eating an argument.
    u32 dst = arg(0);
    const char *fmt = ld_cstr(arg(1));
    u32 out = 0, ai = 2;
    char field[48];
    for (u32 i = 0; fmt[i]; ++i) {
        if (fmt[i] != '%') { st8(dst + out++, (u8)fmt[i]); continue; }
        ++i;
        if (fmt[i] == '%') { st8(dst + out++, (u8)'%'); continue; }
        bool pad0 = false, left = false;
        for (;; ++i) {
            if (fmt[i] == '0') pad0 = true;
            else if (fmt[i] == '-') left = true;
            else break;
        }
        unsigned width = 0;
        while (fmt[i] >= '0' && fmt[i] <= '9') {
            width = width * 10u + (unsigned)(fmt[i] - '0');
            ++i;
        }
        if (fmt[i] == 'l') ++i;
        char c = fmt[i];
        unsigned n = 0, head = 0;   // head: characters that precede zero padding
        if (c == 's') {
            u32 src = arg(ai++);
            while (ld8(src) && n + 1u < sizeof(field)) field[n++] = (char)ld8(src++);
        } else if (c == 'c') {
            field[n++] = (char)(u8)arg(ai++);
        } else if (c == 'd' || c == 'i' || c == 'u' || c == 'x' || c == 'X' || c == 'p') {
            u32 v = arg(ai++);
            bool neg = false;
            if ((c == 'd' || c == 'i') && (i32)v < 0) { neg = true; v = (u32)(0u - v); }
            u32 base = (c == 'x' || c == 'X' || c == 'p') ? 16u : 10u;
            bool upper = (c == 'X');
            char rev[24];
            unsigned k = 0;
            do {
                u32 dg = v % base;
                rev[k++] = (char)(dg < 10u ? ('0' + dg)
                                          : ((upper ? 'A' : 'a') + dg - 10u));
                v /= base;
            } while (v && k < sizeof(rev));
            if (neg) field[n++] = '-';
            if (c == 'p') { field[n++] = '0'; field[n++] = 'x'; }
            head = n;
            while (k) field[n++] = rev[--k];
        } else {
            field[n++] = '%';
            if (c) field[n++] = c;
            head = n;
        }
        unsigned pad = width > n ? width - n : 0u;
        if (!left && !pad0) for (unsigned k = 0; k < pad; ++k) st8(dst + out++, (u8)' ');
        for (unsigned k = 0; k < head; ++k) st8(dst + out++, (u8)field[k]);
        if (!left && pad0) for (unsigned k = 0; k < pad; ++k) st8(dst + out++, (u8)'0');
        for (unsigned k = head; k < n; ++k) st8(dst + out++, (u8)field[k]);
        if (left) for (unsigned k = 0; k < pad; ++k) st8(dst + out++, (u8)' ');
    }
    st8(dst + out, 0);
    stub_done(0, out);
TS_END

// ===========================================================================
// GDI32
// ===========================================================================
// Handles are serial numbers. Nothing is rasterised, so what matters is that
// every create returns a distinct non-null object, every select returns the
// previous one, and nothing is ever deleted twice.

inline u32 &gdi_serial() { static u32 n = 0; return n; }

// What is selected into each device context. Without this, SelectObject cannot
// answer with the object it replaced, and a caller that selects a font, draws,
// and selects the old one back gets somebody else's object.
inline u32 *dc_handles() { static u32 h[16]; return h; }
inline u32 *dc_objects() { static u32 o[16]; return o; }

TS_BEGIN(GetStockObject)
    stub_done(1, 0x00070000u + arg(0));
TS_END
TS_BEGIN(SelectObject)
    // Per device context. A fresh DC holds a stock object, which is what the
    // first call reports as the one it replaced.
    u32 dc = arg(0), obj = arg(1), prev = 0x00070001u;
    for (unsigned i = 0; i < 16u; ++i) {
        if (dc_handles()[i] == 0u) { dc_handles()[i] = dc; dc_objects()[i] = prev; }
        if (dc_handles()[i] == dc) {
            prev = dc_objects()[i];
            dc_objects()[i] = obj;
            break;
        }
    }
    stub_done(2, prev);
TS_END
TS_RET(DeleteObject, 1, 1u)
TS_RET(DeleteDC, 1, 1u)
TS_RET(BitBlt, 9, 1u)
TS_RET(StretchBlt, 11, 1u)
TS_RET(PatBlt, 6, 1u)
TS_RET(Rectangle, 7, 1u)
TS_RET(SetBkMode, 2, 1u)
TS_RET(SetBkColor, 2, 0x00ffffffu)
TS_RET(SetTextColor, 2, 0u)
TS_RET(GetBkColor, 1, 0x00ffffffu)
TS_RET(GetTextColor, 1, 0u)
TS_RET(GetBkMode, 1, 1u)             // TRANSPARENT
TS_RET(SetTextAlign, 2, 0u)
TS_RET(SetGraphicsMode, 2, 1u)
TS_RET(TextOutA, 5, 1u)
TS_RET(SaveDC, 1, 1u)
TS_RET(RestoreDC, 2, 1u)
// Both take FOUR arguments -- (hdc, x, y, lppt) -- and the call sites in
// FUN_0065a190 push all four. The table used to say 3, so the stub left one
// argument on the stack on every call. Two such calls leaked 8 bytes inside
// FUN_00659e90, which shifted its epilogue by two slots: its POP EBX restored
// a stale value (0xff) instead of the caller's 0, and the window-record hash
// walk in FUN_0057a260 -- which uses EBX as its null sentinel -- then looped
// forever on a null bucket. One wrong argument count, an infinite loop four
// frames away.
TS_RET(SetViewportOrgEx, 4, 1u)
TS_RET(SetWindowOrgEx, 4, 1u)
TS_RET(DPtoLP, 3, 1u)
TS_RET(ModifyWorldTransform, 3, 1u)

TS_BEGIN(CreateCompatibleDC) stub_done(1, 0x00070000u + (gdi_serial()++)); TS_END
TS_BEGIN(CreateCompatibleBitmap) stub_done(3, 0x00070000u + (gdi_serial()++)); TS_END
TS_BEGIN(CreateFontA) stub_done(14, 0x00070000u + (gdi_serial()++)); TS_END
TS_BEGIN(CreateFontIndirectA) stub_done(1, 0x00070000u + (gdi_serial()++)); TS_END
TS_BEGIN(CreatePen) stub_done(3, 0x00070000u + (gdi_serial()++)); TS_END
TS_BEGIN(CreateSolidBrush) stub_done(1, 0x00070000u + (gdi_serial()++)); TS_END

TS_BEGIN(CreateDIBSection)
    // The bitmap handle is fake but the pixel buffer is real model memory, so
    // a caller that draws through *ppvBits is writing somewhere that exists.
    u32 ppv = arg(4);
    u32 bits = zero_block(0x100000u, 0x1000u);
    if (ppv) st32(ppv, bits);
    stub_done(6, 0x00070000u + (gdi_serial()++));
TS_END

TS_BEGIN(GetTextExtentPoint32A)
    u32 p = arg(3);
    st32(p, (u32)((i32)arg(2) * 8));
    st32(p + 4u, 16u);
    stub_done(4, 1u);
TS_END

TS_BEGIN(GetTextMetricsA)
    u32 p = arg(1);
    for (u32 i = 0; i < 0x38u; ++i) st8(p + i, 0);
    st32(p + 0u, 16u);    // tmHeight
    st32(p + 4u, 16u);    // tmAscent
    st32(p + 20u, 8u);    // tmAveCharWidth
    stub_done(2, 1u);
TS_END

// ===========================================================================
// DirectDraw
// ===========================================================================
// DirectDrawCreate must hand back a COM object: a pointer whose first dword is
// a vtable of registered stubs. The game calls those methods through the
// vtable, so the slots have to be real addresses and each stub has to pop the
// argument count the real method takes (COM methods are __stdcall with `this`
// as the first stack argument). Returning DD_OK (0) everywhere keeps the
// wrappers alive; the slots that write an out-parameter are the ones the game
// actually reads, and those are written.

TS_RET(dd_QueryInterface, 3, 0u)
TS_RET(dd_AddRef, 1, 1u)
TS_RET(dd_Release, 1, 1u)
TS_RET(dd_Compact, 1, 0u)
TS_RET(dd_DuplicateSurface, 3, 0u)
TS_RET(dd_EnumDisplayModes, 5, 0u)
TS_RET(dd_EnumSurfaces, 5, 0u)
TS_RET(dd_FlipToGDISurface, 1, 0u)
TS_RET(dd_GetFourCCCodes, 3, 0u)
TS_RET(dd_GetGDISurface, 2, 0u)
TS_RET(dd_Initialize, 2, 0u)
TS_RET(dd_RestoreDisplayMode, 1, 0u)
TS_RET(dd_SetCooperativeLevel, 3, 0u)
TS_RET(dd_SetDisplayMode, 4, 0u)
TS_RET(dd_WaitForVerticalBlank, 3, 0u)

inline u32 make_dd_palette();
inline u32 make_dd_clipper();

TS_BEGIN(dd_CreateClipper)
    // arg(0)=this, arg(1)=dwFlags, arg(2)=**lplpClipper, arg(3)=pUnkOuter.
    // The old body was TS_RET(...,4,0): DD_OK with the out pointer untouched.
    // FUN_004874c0 then calls (*lpClipper)->SetHWnd through a null, which is
    // the 0x4876e4 call to address 0 in the boot. Writing the object is the
    // whole contract of this method.
    if (arg(2)) st32(arg(2), make_dd_clipper());
    stub_done(4, 0u);   // DD_OK
TS_END

TS_BEGIN(dd_CreatePalette)
    // arg(0)=this, arg(1)=dwFlags, arg(2)=lpColorTable, arg(3)=**lplpPalette
    if (arg(3)) st32(arg(3), make_dd_palette());
    stub_done(4, 0u);
TS_END

TS_BEGIN(dd_GetAvailableVidMem)
    // Wine's trace of the REAL game (WINEDEBUG=+ddraw) shows this is called
    // before SetCooperativeLevel, with the DDSCAPS of the memory class the game
    // intends to use. It reads *total and *free to size its own texture budget,
    // so a zero here reads as "this card has no video memory" and the game sizes
    // everything to nothing. Report a plausible 4 MB.
    if (arg(2)) st32(arg(2), 0x00400000u);   // *lpdwTotal
    if (arg(3)) st32(arg(3), 0x00400000u);   // *lpdwFree
    stub_done(4, 0u);
TS_END

TS_BEGIN(dd_GetCaps)
    // The game reads dwCaps/dwCaps2 to decide what it may ask for. Zeroing
    // them means "no hardware anything", which is true of this stub.
    st32(arg(1), 0u);
    st32(arg(2), 0u);
    stub_done(3, 0u);
TS_END

TS_BEGIN(dd_GetDisplayMode)
    // DDSURFACEDESC: +0 dwSize, +4 dwFlags, +8 dwHeight, +12 dwWidth,
    // +104 ddpfPixelFormat.dwRGBBitCount. The mode is the screen this model
    // reports from GetSystemMetrics: a program that asks both and is told
    // different sizes sizes its surfaces for the wrong screen.
    u32 p = arg(1);
    if (p) {
        for (u32 i = 0; i < 108u; ++i) st8(p + i, 0);
        st32(p + 0u, 108u);
        st32(p + 4u, 0x00000041u);   // DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT
        st32(p + 8u, SCREEN_H);
        st32(p + 12u, SCREEN_W);
        st32(p + 104u, SCREEN_BPP);
    }
    stub_done(2, 0u);
TS_END

TS_BEGIN(dd_GetMonitorFrequency) st32(arg(1), 60u); stub_done(2, 0u); TS_END
TS_BEGIN(dd_GetScanLine) st32(arg(1), 0u); stub_done(2, 0u); TS_END
TS_BEGIN(dd_GetVerticalBlankStatus) st32(arg(1), 1u); stub_done(2, 0u); TS_END

// Defined below with the surface vtable; declared here because CreateSurface
// is the only caller and the vtable needs the stubs that come after it.
inline u32 make_surface();

TS_BEGIN(dd_CreateSurface)
    // A surface object with the same shape as the DirectDraw one: a pointer to
    // a vtable of registered stubs. ppSurface is at arg(2).
    u32 out = arg(2);
    if (out) st32(out, make_surface());
    stub_done(4, 0u);
TS_END

// ===========================================================================
// The long tail: ole32, OLEAUT32, ADVAPI32, WINMM, COMCTL32, VERSION, SHELL32,
// WSOCK32, DSOUND. Cheap ones get a body; the rest fall back to the named
// generic stub, which is correct about the stack and honest about the rest.
// ===========================================================================


// -- ADVAPI32 registry -------------------------------------------------------
// No registry exists, so an opened key is a handle and a query misses. A miss
// is what the game wants when it is looking for an install path: it then uses
// its own defaults instead of a value this stub invented.
TS_RET(RegCloseKey, 1, 0u)
TS_RET(RegDeleteKeyA, 2, 0u)
TS_RET(RegSetValueExA, 6, 0u)
TS_RET(RegQueryValueExA, 6, 2u)     // ERROR_FILE_NOT_FOUND

TS_BEGIN(RegOpenKeyExA)
    u32 out = arg(4);
    if (out) st32(out, 0x000a0001u);
    stub_done(5, 0u);
TS_END

TS_BEGIN(RegCreateKeyExA)
    u32 out = arg(7);
    if (out) st32(out, 0x000a0002u);
    if (arg(8)) st32(arg(8), 1u);   // REG_CREATED_NEW_KEY
    stub_done(9, 0u);
TS_END

TS_BEGIN(RegEnumKeyExA)
    // One key, named after nothing in particular, then ERROR_NO_MORE_ITEMS.
    static u32 seen = 0;
    u32 name = arg(1), cch = arg(2);
    if (seen++ > 0u) { stub_done(8, 259u); }   // ERROR_NO_MORE_ITEMS
    else {
        if (name && cch) c_strncpy(name, lit("SOFTWARE"), cch);
        stub_done(8, 0u);
    }
TS_END

TS_BEGIN(RegQueryInfoKeyA)
    u32 p = arg(11);
    if (p) st32(p, 0u);
    stub_done(12, 0u);
TS_END

// -- ole32 / OLEAUT32 --------------------------------------------------------
// CoCreateInstance has a body of its own (below, next to the IBlowfish object):
// it writes a fake COM object into *ppv, because a null is dereferenced
// immediately and an S_OK with an unwritten out pointer is worse than a
// failure -- the caller believes it has an object and calls through whatever
// the slot held.
TS_RET(OleInitialize, 1, 0u)
TS_RET(OleUninitialize, 0, 0u)
TS_RET(CoDisconnectObject, 2, 0u)
TS_RET(CoRevokeClassObject, 1, 0u)
TS_RET(OleRun, 1, 0u)
TS_RET(OleSaveToStream, 2, 0u)
TS_RET(OleLoadFromStream, 3, 0u)
TS_RET(CoRegisterClassObject, 5, 0u)
TS_RET(StgCreateDocfile, 4, 0u)
TS_RET(StgOpenStorage, 6, 0u)

TS_BEGIN(CoFileTimeNow)
    u32 p = arg(0);
    st32(p, 0x2b0e8b00u); st32(p + 4u, 0x01bf0000u);
    stub_done(1, 0u);
TS_END

TS_BEGIN(CLSIDFromString)
    u32 p = arg(1);
    for (u32 i = 0; i < 16u; ++i) st8(p + i, 0);
    stub_done(2, 0u);
TS_END

TS_BEGIN(StringFromCLSID)
    u32 out = arg(1);
    if (out) st32(out, lit("{00000000-0000-0000-0000-000000000000}"));
    stub_done(2, 0u);
TS_END

TS_BEGIN(StringFromGUID2)
    u32 buf = arg(1);
    if (arg(2)) c_strncpy(buf, lit("{00000000-0000-0000-0000-000000000000}"), arg(2));
    stub_done(3, 39u);
TS_END

// SysAllocString returns a BSTR: a length prefix followed by the characters.
inline u32 make_bstr(u32 src, u32 chars) {
    u32 p = alloc_block(chars * 2u + 8u, 4u);
    st32(p, chars * 2u);
    for (u32 i = 0; i < chars; ++i) st16(p + 4u + i * 2u, ld8(src + i));
    st16(p + 4u + chars * 2u, 0u);
    return p + 4u;
}
// -- OLEAUT32, by ordinal ----------------------------------------------------
// The ARGUMENT COUNTS of every import in this section are measured, not looked
// up: tools/gen_win32_stubs.py --evidence walks the recovered instruction
// stream, counts the pushes in front of each `call dword ptr [slot]` and fails
// if the import table disagrees with them. The NAMES are inferred from the
// shape of those call sites and are marked /* ? */ where they are a guess: a
// name is a convenience, an argument count is not.
//
// Only the ordinals whose return value carries something get a body. The rest
// go to the named generic stub, which pops exactly the words the table says and
// prints once, on first call, that it is unimplemented.

TS_ORD_BEGIN(oleaut_ord_2147483809, "OLEAUT32", 0x800000a1u)   // LoadTypeLib /* ? */
    // Measured 2 arguments at 0x5e5910: the wide string "SUN.TLB" and an
    // out-pointer. Answering success without writing an object would be a lie
    // the caller dereferences, so this fails the way a missing typelib fails.
    u32 out = arg(1);
    if (out) st32(out, 0u);
    stub_done_for("OLEAUT32", ord, 0x8002801du);   // TYPE_E_CANTLOADLIBRARY
TS_ORD_END

TS_ORD_BEGIN(oleaut_ord_2147483650, "OLEAUT32", 0x80000002u)   // SysAllocString /* ? */
    u32 s = arg(0);
    stub_done_for("OLEAUT32", ord, s ? make_bstr(s, c_strlen(s)) : 0u);
TS_ORD_END

TS_ORD_BEGIN(oleaut_ord_2147483656, "OLEAUT32", 0x80000008u)   // VariantInit /* ? */
    // The call sites pass `lea reg,[ebp-N]`, a local VARIANT. A VARIANT that
    // was never zeroed is a variant with a garbage type tag.
    u32 p = arg(0);
    for (u32 i = 0; i < 16u; ++i) st8(p + i, 0);
    stub_done_for("OLEAUT32", ord, 0u);
TS_ORD_END

// -- WINMM -------------------------------------------------------------------
// timeGetTime is the millisecond clock the game's frame timer reads; it shares
// the counter GetTickCount uses, so the two cannot disagree.
TS_RET(timeBeginPeriod, 1, 0u)
TS_RET(timeEndPeriod, 1, 0u)
TS_RET(timeGetDevCaps, 2, 0u)
TS_RET(timeKillEvent, 1, 1u)
TS_RET(timeSetEvent, 5, 1u)

// -- COMCTL32 ----------------------------------------------------------------
TS_RET(ImageList_Destroy, 1, 1u)
TS_RET(ImageList_BeginDrag, 3, 1u)
TS_RET(ImageList_EndDrag, 0, 0u)
TS_RET(ImageList_DragEnter, 3, 1u)
TS_RET(ImageList_DragMove, 2, 1u)
TS_RET(ImageList_DragShowNolock, 1, 1u)
// InitCommonControls: measured 0 arguments at 0x65c04b, where the call sits
// directly behind the prologue with nothing pushed. It is the call a program
// makes before RegisterClass, which is what the enclosing function does.
// DllGetVersion is NOT in the import table -- the program reaches it through
// GetProcAddress -- so it has to exist as an export here or the game never finds
// it. That matters more than it looks: FUN_00478e60 does
//   LoadLibraryA(dll); p = GetProcAddress(h,"DllGetVersion");
//   if (p) { vi.cbSize=0x14; if (p(&vi) >= 0) version = vi.dwMajor<<16 | vi.dwMinor; }
// and FUN_005e5b80 then does
//   if (version < 0x40046) { MessageBoxA(...,MB_ICONERROR); ExitProcess(1); }
// 0x40046 is comctl32 4.00.70. With no export the version is 0, the check fails,
// and the game shows an error box and exits before it initialises anything. So
// report a comctl32 new enough to pass, and be internally consistent about it:
// 4.72 is what shipped with IE 4-era installs and is what this reports.
TS_BEGIN(DllGetVersion)
    u32 p = arg(0);
    u32 cb = p ? ld32(p) : 0;
    if (p && cb >= 0x14u) {
        st32(p + 0x04u, 4u);      // dwMajorVersion
        st32(p + 0x08u, 0x48u);   // dwMinorVersion -> version 0x40048 >= 0x40046
        st32(p + 0x0cu, 0u);      // dwBuildNumber
        st32(p + 0x10u, 1u);      // dwPlatformID
    }
    stub_done(1, 0u);             // S_OK
TS_END

TS_ORD_RET(comctl32_ord_2147483665, "COMCTL32", 0x80000011u, 0u)

// -- VERSION / SHELL32 -------------------------------------------------------
TS_RET(GetFileVersionInfoSizeA, 2, 0u)   // no version resource, honestly
TS_RET(GetFileVersionInfoA, 4, 0u)
TS_RET(VerQueryValueA, 4, 0u)
TS_RET(FindExecutableA, 3, 1u)

// -- WSOCK32, by ordinal -----------------------------------------------------
// NOTHING IN THIS BINARY CALLS THESE. Each of the 19 slots is referenced once,
// by a `jmp dword ptr [slot]` thunk in the 0x6824d8 block, and nothing in .text
// or .data references any of those thunks. So there is no call site to measure
// an argument count from, and the counts in the import table for these are the
// documented Win32 signatures, tagged "doc" in tools/gen_win32_stubs.py and
// listed as unmeasured by `--evidence`. This section used to carry its own
// counts -- 7 for the import the table called 2 -- which is exactly the kind of
// second, unverifiable claim this header now avoids.
//
// WSAStartup is the one worth a body: it must return 0 and fill WSADATA, or a
// caller that checks the result gives up on networking entirely. Its argument
// count is measured all the same -- ordinal 115 is the one ordinal of the 19
// whose number the classic table and this header agree on.
TS_ORD_BEGIN(wsock_ord_2147483763, "WSOCK32", 0x80000073u)   // WSAStartup
    u32 p = arg(1);
    for (u32 i = 0; i < 0x190u; ++i) st8(p + i, 0);
    st16(p + 0u, 0x0101u);   // wVersion = 1.1
    st16(p + 2u, 0x0101u);   // wHighVersion
    c_strcpy(p + 4u, lit("Tiberian Sun WinSock"));
    st16(p + 0x18cu, 0u);
    st16(p + 0x18eu, 0u);
    stub_done_for("WSOCK32", ord, 0u);
TS_ORD_END

// ===========================================================================
// fake COM objects
// ===========================================================================
// DirectDraw and DirectSound hand back an interface pointer whose first dword
// is a vtable of REGISTERED stubs. A slot stub reads its own definition from a
// table indexed by the slot number, so it knows both the method's argument
// count (the modelled stack must stay balanced) and its name (so a call is
// visible). This is the mechanism that makes a fake object usable rather than
// merely non-null.

struct SlotDef { const char *name; u8 nargs; u32 ret; };

inline SlotDef *slot_defs() { static SlotDef s[512]; return s; }
inline u32 &slot_next() { static u32 n = 0; return n; }

// Per-slot call counts. A COM method is not an import, so it has no row in the
// import table; without this the calls a DirectDraw wrapper makes would be
// counted in total and attributed to nothing.
inline unsigned long *slot_counts() { static unsigned long c[512]; return c; }

template<unsigned I> void slot_stub() {
    const SlotDef &d = slot_defs()[I];
    note(d.name ? d.name : "com");
    slot_counts()[I]++;
    stub_done(d.nargs, d.ret);
}

inline Fn *slot_fns() {
    static Fn f[512] = {
        &slot_stub<0>,   &slot_stub<1>,   &slot_stub<2>,   &slot_stub<3>,
        &slot_stub<4>,   &slot_stub<5>,   &slot_stub<6>,   &slot_stub<7>,
        &slot_stub<8>,   &slot_stub<9>,   &slot_stub<10>,  &slot_stub<11>,
        &slot_stub<12>,  &slot_stub<13>,  &slot_stub<14>,  &slot_stub<15>,
        &slot_stub<16>,  &slot_stub<17>,  &slot_stub<18>,  &slot_stub<19>,
        &slot_stub<20>,  &slot_stub<21>,  &slot_stub<22>,  &slot_stub<23>,
        &slot_stub<24>,  &slot_stub<25>,  &slot_stub<26>,  &slot_stub<27>,
        &slot_stub<28>,  &slot_stub<29>,  &slot_stub<30>,  &slot_stub<31>,
        &slot_stub<32>,  &slot_stub<33>,  &slot_stub<34>,  &slot_stub<35>,
        &slot_stub<36>,  &slot_stub<37>,  &slot_stub<38>,  &slot_stub<39>,
        &slot_stub<40>,  &slot_stub<41>,  &slot_stub<42>,  &slot_stub<43>,
        &slot_stub<44>,  &slot_stub<45>,  &slot_stub<46>,  &slot_stub<47>,
        &slot_stub<48>,  &slot_stub<49>,  &slot_stub<50>,  &slot_stub<51>,
        &slot_stub<52>,  &slot_stub<53>,  &slot_stub<54>,  &slot_stub<55>,
        &slot_stub<56>,  &slot_stub<57>,  &slot_stub<58>,  &slot_stub<59>,
        &slot_stub<60>,  &slot_stub<61>,  &slot_stub<62>,  &slot_stub<63>,
        &slot_stub<64>,  &slot_stub<65>,  &slot_stub<66>,  &slot_stub<67>,
        &slot_stub<68>,  &slot_stub<69>,  &slot_stub<70>,  &slot_stub<71>,
        &slot_stub<72>,  &slot_stub<73>,  &slot_stub<74>,  &slot_stub<75>,
        &slot_stub<76>,  &slot_stub<77>,  &slot_stub<78>,  &slot_stub<79>,
        &slot_stub<80>,  &slot_stub<81>,  &slot_stub<82>,  &slot_stub<83>,
        &slot_stub<84>,  &slot_stub<85>,  &slot_stub<86>,  &slot_stub<87>,
        &slot_stub<88>,  &slot_stub<89>,  &slot_stub<90>,  &slot_stub<91>,
        &slot_stub<92>,  &slot_stub<93>,  &slot_stub<94>,  &slot_stub<95>,
        &slot_stub<96>,  &slot_stub<97>,  &slot_stub<98>,  &slot_stub<99>,
        &slot_stub<100>, &slot_stub<101>, &slot_stub<102>, &slot_stub<103>,
        &slot_stub<104>, &slot_stub<105>, &slot_stub<106>, &slot_stub<107>,
        &slot_stub<108>, &slot_stub<109>, &slot_stub<110>, &slot_stub<111>,
        &slot_stub<112>, &slot_stub<113>, &slot_stub<114>, &slot_stub<115>,
        &slot_stub<116>, &slot_stub<117>, &slot_stub<118>, &slot_stub<119>,
        &slot_stub<120>, &slot_stub<121>, &slot_stub<122>, &slot_stub<123>,
        &slot_stub<124>, &slot_stub<125>, &slot_stub<126>, &slot_stub<127>,
        &slot_stub<128>, &slot_stub<129>, &slot_stub<130>, &slot_stub<131>,
        &slot_stub<132>, &slot_stub<133>, &slot_stub<134>, &slot_stub<135>,
        &slot_stub<136>, &slot_stub<137>, &slot_stub<138>, &slot_stub<139>,
        &slot_stub<140>, &slot_stub<141>, &slot_stub<142>, &slot_stub<143>,
        &slot_stub<144>, &slot_stub<145>, &slot_stub<146>, &slot_stub<147>,
        &slot_stub<148>, &slot_stub<149>, &slot_stub<150>, &slot_stub<151>,
        &slot_stub<152>, &slot_stub<153>, &slot_stub<154>, &slot_stub<155>,
        &slot_stub<156>, &slot_stub<157>, &slot_stub<158>, &slot_stub<159>,
        &slot_stub<160>, &slot_stub<161>, &slot_stub<162>, &slot_stub<163>,
        &slot_stub<164>, &slot_stub<165>, &slot_stub<166>, &slot_stub<167>,
        &slot_stub<168>, &slot_stub<169>, &slot_stub<170>, &slot_stub<171>,
        &slot_stub<172>, &slot_stub<173>, &slot_stub<174>, &slot_stub<175>,
        &slot_stub<176>, &slot_stub<177>, &slot_stub<178>, &slot_stub<179>,
        &slot_stub<180>, &slot_stub<181>, &slot_stub<182>, &slot_stub<183>,
        &slot_stub<184>, &slot_stub<185>, &slot_stub<186>, &slot_stub<187>,
        &slot_stub<188>, &slot_stub<189>, &slot_stub<190>, &slot_stub<191>,
        &slot_stub<192>, &slot_stub<193>, &slot_stub<194>, &slot_stub<195>,
        &slot_stub<196>, &slot_stub<197>, &slot_stub<198>, &slot_stub<199>,
        &slot_stub<200>, &slot_stub<201>, &slot_stub<202>, &slot_stub<203>,
        &slot_stub<204>, &slot_stub<205>, &slot_stub<206>, &slot_stub<207>,
        &slot_stub<208>, &slot_stub<209>, &slot_stub<210>, &slot_stub<211>,
        &slot_stub<212>, &slot_stub<213>, &slot_stub<214>, &slot_stub<215>,
        &slot_stub<216>, &slot_stub<217>, &slot_stub<218>, &slot_stub<219>,
        &slot_stub<220>, &slot_stub<221>, &slot_stub<222>, &slot_stub<223>,
        &slot_stub<224>, &slot_stub<225>, &slot_stub<226>, &slot_stub<227>,
        &slot_stub<228>, &slot_stub<229>, &slot_stub<230>, &slot_stub<231>,
        &slot_stub<232>, &slot_stub<233>, &slot_stub<234>, &slot_stub<235>,
        &slot_stub<236>, &slot_stub<237>, &slot_stub<238>, &slot_stub<239>,
        &slot_stub<240>, &slot_stub<241>, &slot_stub<242>, &slot_stub<243>,
        &slot_stub<244>, &slot_stub<245>, &slot_stub<246>, &slot_stub<247>,
        &slot_stub<248>, &slot_stub<249>, &slot_stub<250>, &slot_stub<251>,
        &slot_stub<252>, &slot_stub<253>, &slot_stub<254>, &slot_stub<255>,
    };
    return f;
}

void k_com_QueryInterface();

TS_BEGIN(com_QueryInterface)
    // QueryInterface(this, riid, ppvObject) for every fake object in the model.
    // Filling ppvObject is the whole job: a wrapper that asks for an interface,
    // is told S_OK, and then calls through a ppvObject nobody wrote calls
    // through garbage. This object answers every interface with itself.
    u32 out = arg(2);
    if (out) st32(out, arg(0));
    stub_done(3, 0u);   // S_OK
TS_END

// Build a vtable of registered stubs: one SlotDef per slot (so a slot stub
// knows its own name and argument count) and one registered VA per slot. The
// vtable is built once per interface SHAPE and shared by every object of that
// shape, because slot_defs() is a fixed table and a vtable per surface would
// exhaust it after eleven surfaces.
inline u32 make_com_vtable(const char *const *names, const u8 *nargs,
                           const Fn *override_fns, unsigned slots) {
    u32 base = slot_next();
    for (unsigned i = 0; i < slots; ++i) {
        SlotDef &d = slot_defs()[base + i];
        d.name = names[i];
        d.nargs = nargs[i];
        d.ret = 0u;
    }
    u32 vt = alloc_block(slots * 4u, 4u);
    for (unsigned i = 0; i < slots; ++i) {
        Fn f = override_fns[i] ? override_fns[i] : slot_fns()[base + i];
        if (i == 0u) f = &k_com_QueryInterface;   // slot 0 is QueryInterface
        u32 va = alloc_stub_va();
        ts_register(va, f);
        st32(vt + i * 4u, va);
    }
    slot_next() = base + slots;
    return vt;
}

// A fake COM object: an object block whose first dword points at its vtable.
inline u32 make_com_object(const char *const *names, const u8 *nargs,
                           const Fn *override_fns, unsigned slots) {
    u32 vt = make_com_vtable(names, nargs, override_fns, slots);
    u32 obj = alloc_block(16u, 4u);
    st32(obj, vt);
    return obj;
}

inline u32 make_dd_clipper() {
    // IDirectDrawClipper, DirectX 6 slot order: QueryInterface/AddRef/Release/
    // GetClipList/GetHWnd/Initialize/IsClipListChanged/SetClipList/SetHWnd.
    // Only SetHWnd is called here (FUN_004874c0 attaches the clipper to the
    // window), but the whole shape is modelled so an interface pointer handed
    // out is never a short vtable.
    static const char *const names[9] = {
        "IDirectDrawClipper::QueryInterface", "IDirectDrawClipper::AddRef",
        "IDirectDrawClipper::Release", "IDirectDrawClipper::GetClipList",
        "IDirectDrawClipper::GetHWnd", "IDirectDrawClipper::Initialize",
        "IDirectDrawClipper::IsClipListChanged", "IDirectDrawClipper::SetClipList",
        "IDirectDrawClipper::SetHWnd",
    };
    static const u8 nargs[9] = { 3, 1, 1, 4, 2, 3, 2, 3, 3 };
    Fn over[9] = { 0 };
    static u32 obj = 0;
    if (!obj) obj = make_com_object(names, nargs, over, 9);
    return obj;
}

// ===========================================================================
// IBlowfish -- the class BLOWFISH.DLL registers for CLSID 1440AD10-...
// ===========================================================================
// Recovered from the DLL itself, not guessed: its class factory vtable is at
// 0x1101d070 and its CreateInstance (0x11001251) constructs an object whose
// vtable is 0x1101e480. That vtable has EIGHT slots, and each body below was
// read from the corresponding function in BLOWFISH.DLL:
//
//   slot 3  0x11002011  SetKey(this, int len, const void *key)   len 0..56
//   slot 4  0x1100204d  GetMaxKeyLength(this, int *out)          *out = 56
//   slot 5  0x1100206c  GetBlockSize(this, int *out)             *out = 8
//   slot 6  0x1100208b  Encrypt(this, int len, src, dst)         ret 0x10
//   slot 7  0x110020cb  Decrypt(this, int len, src, dst)         ret 0x10
//
// The game's own wrapper (GAME.EXE 0x4233e0/0x423410/0x423440) calls exactly
// slots 3, 6 and 7, with the argument order above. The class is created
// lazily by 0x423360, which is reached from the MIX decryption path, not from
// registration.
//
// Encrypt/Decrypt are standard Blowfish-ECB, not copies. The game reaches the
// TIBSUN.MIX index as soon as WM_ACTIVATEAPP is delivered:
//
//   step 1881678  IBlowfish::SetKey(56, key=82060323...)  from 0x423404
//   step 1881870  ReadFile(8) at MIX offset 84
//   step 1881940  IBlowfish::Decrypt(8, src=2ce3171b...)  from 0x423469
//
// The key the game computes with its own RSA code is byte-for-byte the key
// tools/mix.py derives from the same 80-byte block. Decrypting the first index
// block with it yields count=17, body=75,094,416 -- the same index mix.py
// decodes. An identity copy instead left ciphertext, the PKStraw read returned
// 0, FUN_00547290 read a 6-byte header from uninitialised stack, and
// operator_new(-88560) exhausted the page table.
//
// The schedule is the standard one: the 18-word P-array and four 256-word
// S-boxes start as the first 1042 32-bit words of the fractional hex digits of
// pi, the key XORs the P-array cyclically (big-endian 4-byte words) and 521
// encryptions replace P and S. Blocks are big-endian. The known-answer vectors
// in tools/test_tools.py prove the tables and the round function; the TIBSUN
// index cross-check against tools/mix.py proves the whole path.
inline u8 *blowfish_key() { static u8 k[56]; return k; }
inline u32 &blowfish_key_len() { static u32 n = 0; return n; }

// The initial tables: 18 P words then 4*256 S words, i.e. the hex digits of pi.
// Generated from pi (not copied from BLOWFISH.DLL) by the Machin formula; the
// first words are 243f6a88 85a308d3 ... ec4e6c89, d1310ba6 ..., 3ac372e6.
inline const u32 *bf_init_words() {
    static const u32 w[1042] = {
    0x243f6a88u, 0x85a308d3u, 0x13198a2eu, 0x03707344u, 0xa4093822u, 0x299f31d0u,
    0x082efa98u, 0xec4e6c89u, 0x452821e6u, 0x38d01377u, 0xbe5466cfu, 0x34e90c6cu,
    0xc0ac29b7u, 0xc97c50ddu, 0x3f84d5b5u, 0xb5470917u, 0x9216d5d9u, 0x8979fb1bu,
    0xd1310ba6u, 0x98dfb5acu, 0x2ffd72dbu, 0xd01adfb7u, 0xb8e1afedu, 0x6a267e96u,
    0xba7c9045u, 0xf12c7f99u, 0x24a19947u, 0xb3916cf7u, 0x0801f2e2u, 0x858efc16u,
    0x636920d8u, 0x71574e69u, 0xa458fea3u, 0xf4933d7eu, 0x0d95748fu, 0x728eb658u,
    0x718bcd58u, 0x82154aeeu, 0x7b54a41du, 0xc25a59b5u, 0x9c30d539u, 0x2af26013u,
    0xc5d1b023u, 0x286085f0u, 0xca417918u, 0xb8db38efu, 0x8e79dcb0u, 0x603a180eu,
    0x6c9e0e8bu, 0xb01e8a3eu, 0xd71577c1u, 0xbd314b27u, 0x78af2fdau, 0x55605c60u,
    0xe65525f3u, 0xaa55ab94u, 0x57489862u, 0x63e81440u, 0x55ca396au, 0x2aab10b6u,
    0xb4cc5c34u, 0x1141e8ceu, 0xa15486afu, 0x7c72e993u, 0xb3ee1411u, 0x636fbc2au,
    0x2ba9c55du, 0x741831f6u, 0xce5c3e16u, 0x9b87931eu, 0xafd6ba33u, 0x6c24cf5cu,
    0x7a325381u, 0x28958677u, 0x3b8f4898u, 0x6b4bb9afu, 0xc4bfe81bu, 0x66282193u,
    0x61d809ccu, 0xfb21a991u, 0x487cac60u, 0x5dec8032u, 0xef845d5du, 0xe98575b1u,
    0xdc262302u, 0xeb651b88u, 0x23893e81u, 0xd396acc5u, 0x0f6d6ff3u, 0x83f44239u,
    0x2e0b4482u, 0xa4842004u, 0x69c8f04au, 0x9e1f9b5eu, 0x21c66842u, 0xf6e96c9au,
    0x670c9c61u, 0xabd388f0u, 0x6a51a0d2u, 0xd8542f68u, 0x960fa728u, 0xab5133a3u,
    0x6eef0b6cu, 0x137a3be4u, 0xba3bf050u, 0x7efb2a98u, 0xa1f1651du, 0x39af0176u,
    0x66ca593eu, 0x82430e88u, 0x8cee8619u, 0x456f9fb4u, 0x7d84a5c3u, 0x3b8b5ebeu,
    0xe06f75d8u, 0x85c12073u, 0x401a449fu, 0x56c16aa6u, 0x4ed3aa62u, 0x363f7706u,
    0x1bfedf72u, 0x429b023du, 0x37d0d724u, 0xd00a1248u, 0xdb0fead3u, 0x49f1c09bu,
    0x075372c9u, 0x80991b7bu, 0x25d479d8u, 0xf6e8def7u, 0xe3fe501au, 0xb6794c3bu,
    0x976ce0bdu, 0x04c006bau, 0xc1a94fb6u, 0x409f60c4u, 0x5e5c9ec2u, 0x196a2463u,
    0x68fb6fafu, 0x3e6c53b5u, 0x1339b2ebu, 0x3b52ec6fu, 0x6dfc511fu, 0x9b30952cu,
    0xcc814544u, 0xaf5ebd09u, 0xbee3d004u, 0xde334afdu, 0x660f2807u, 0x192e4bb3u,
    0xc0cba857u, 0x45c8740fu, 0xd20b5f39u, 0xb9d3fbdbu, 0x5579c0bdu, 0x1a60320au,
    0xd6a100c6u, 0x402c7279u, 0x679f25feu, 0xfb1fa3ccu, 0x8ea5e9f8u, 0xdb3222f8u,
    0x3c7516dfu, 0xfd616b15u, 0x2f501ec8u, 0xad0552abu, 0x323db5fau, 0xfd238760u,
    0x53317b48u, 0x3e00df82u, 0x9e5c57bbu, 0xca6f8ca0u, 0x1a87562eu, 0xdf1769dbu,
    0xd542a8f6u, 0x287effc3u, 0xac6732c6u, 0x8c4f5573u, 0x695b27b0u, 0xbbca58c8u,
    0xe1ffa35du, 0xb8f011a0u, 0x10fa3d98u, 0xfd2183b8u, 0x4afcb56cu, 0x2dd1d35bu,
    0x9a53e479u, 0xb6f84565u, 0xd28e49bcu, 0x4bfb9790u, 0xe1ddf2dau, 0xa4cb7e33u,
    0x62fb1341u, 0xcee4c6e8u, 0xef20cadau, 0x36774c01u, 0xd07e9efeu, 0x2bf11fb4u,
    0x95dbda4du, 0xae909198u, 0xeaad8e71u, 0x6b93d5a0u, 0xd08ed1d0u, 0xafc725e0u,
    0x8e3c5b2fu, 0x8e7594b7u, 0x8ff6e2fbu, 0xf2122b64u, 0x8888b812u, 0x900df01cu,
    0x4fad5ea0u, 0x688fc31cu, 0xd1cff191u, 0xb3a8c1adu, 0x2f2f2218u, 0xbe0e1777u,
    0xea752dfeu, 0x8b021fa1u, 0xe5a0cc0fu, 0xb56f74e8u, 0x18acf3d6u, 0xce89e299u,
    0xb4a84fe0u, 0xfd13e0b7u, 0x7cc43b81u, 0xd2ada8d9u, 0x165fa266u, 0x80957705u,
    0x93cc7314u, 0x211a1477u, 0xe6ad2065u, 0x77b5fa86u, 0xc75442f5u, 0xfb9d35cfu,
    0xebcdaf0cu, 0x7b3e89a0u, 0xd6411bd3u, 0xae1e7e49u, 0x00250e2du, 0x2071b35eu,
    0x226800bbu, 0x57b8e0afu, 0x2464369bu, 0xf009b91eu, 0x5563911du, 0x59dfa6aau,
    0x78c14389u, 0xd95a537fu, 0x207d5ba2u, 0x02e5b9c5u, 0x83260376u, 0x6295cfa9u,
    0x11c81968u, 0x4e734a41u, 0xb3472dcau, 0x7b14a94au, 0x1b510052u, 0x9a532915u,
    0xd60f573fu, 0xbc9bc6e4u, 0x2b60a476u, 0x81e67400u, 0x08ba6fb5u, 0x571be91fu,
    0xf296ec6bu, 0x2a0dd915u, 0xb6636521u, 0xe7b9f9b6u, 0xff34052eu, 0xc5855664u,
    0x53b02d5du, 0xa99f8fa1u, 0x08ba4799u, 0x6e85076au, 0x4b7a70e9u, 0xb5b32944u,
    0xdb75092eu, 0xc4192623u, 0xad6ea6b0u, 0x49a7df7du, 0x9cee60b8u, 0x8fedb266u,
    0xecaa8c71u, 0x699a17ffu, 0x5664526cu, 0xc2b19ee1u, 0x193602a5u, 0x75094c29u,
    0xa0591340u, 0xe4183a3eu, 0x3f54989au, 0x5b429d65u, 0x6b8fe4d6u, 0x99f73fd6u,
    0xa1d29c07u, 0xefe830f5u, 0x4d2d38e6u, 0xf0255dc1u, 0x4cdd2086u, 0x8470eb26u,
    0x6382e9c6u, 0x021ecc5eu, 0x09686b3fu, 0x3ebaefc9u, 0x3c971814u, 0x6b6a70a1u,
    0x687f3584u, 0x52a0e286u, 0xb79c5305u, 0xaa500737u, 0x3e07841cu, 0x7fdeae5cu,
    0x8e7d44ecu, 0x5716f2b8u, 0xb03ada37u, 0xf0500c0du, 0xf01c1f04u, 0x0200b3ffu,
    0xae0cf51au, 0x3cb574b2u, 0x25837a58u, 0xdc0921bdu, 0xd19113f9u, 0x7ca92ff6u,
    0x94324773u, 0x22f54701u, 0x3ae5e581u, 0x37c2dadcu, 0xc8b57634u, 0x9af3dda7u,
    0xa9446146u, 0x0fd0030eu, 0xecc8c73eu, 0xa4751e41u, 0xe238cd99u, 0x3bea0e2fu,
    0x3280bba1u, 0x183eb331u, 0x4e548b38u, 0x4f6db908u, 0x6f420d03u, 0xf60a04bfu,
    0x2cb81290u, 0x24977c79u, 0x5679b072u, 0xbcaf89afu, 0xde9a771fu, 0xd9930810u,
    0xb38bae12u, 0xdccf3f2eu, 0x5512721fu, 0x2e6b7124u, 0x501adde6u, 0x9f84cd87u,
    0x7a584718u, 0x7408da17u, 0xbc9f9abcu, 0xe94b7d8cu, 0xec7aec3au, 0xdb851dfau,
    0x63094366u, 0xc464c3d2u, 0xef1c1847u, 0x3215d908u, 0xdd433b37u, 0x24c2ba16u,
    0x12a14d43u, 0x2a65c451u, 0x50940002u, 0x133ae4ddu, 0x71dff89eu, 0x10314e55u,
    0x81ac77d6u, 0x5f11199bu, 0x043556f1u, 0xd7a3c76bu, 0x3c11183bu, 0x5924a509u,
    0xf28fe6edu, 0x97f1fbfau, 0x9ebabf2cu, 0x1e153c6eu, 0x86e34570u, 0xeae96fb1u,
    0x860e5e0au, 0x5a3e2ab3u, 0x771fe71cu, 0x4e3d06fau, 0x2965dcb9u, 0x99e71d0fu,
    0x803e89d6u, 0x5266c825u, 0x2e4cc978u, 0x9c10b36au, 0xc6150ebau, 0x94e2ea78u,
    0xa5fc3c53u, 0x1e0a2df4u, 0xf2f74ea7u, 0x361d2b3du, 0x1939260fu, 0x19c27960u,
    0x5223a708u, 0xf71312b6u, 0xebadfe6eu, 0xeac31f66u, 0xe3bc4595u, 0xa67bc883u,
    0xb17f37d1u, 0x018cff28u, 0xc332ddefu, 0xbe6c5aa5u, 0x65582185u, 0x68ab9802u,
    0xeecea50fu, 0xdb2f953bu, 0x2aef7dadu, 0x5b6e2f84u, 0x1521b628u, 0x29076170u,
    0xecdd4775u, 0x619f1510u, 0x13cca830u, 0xeb61bd96u, 0x0334fe1eu, 0xaa0363cfu,
    0xb5735c90u, 0x4c70a239u, 0xd59e9e0bu, 0xcbaade14u, 0xeecc86bcu, 0x60622ca7u,
    0x9cab5cabu, 0xb2f3846eu, 0x648b1eafu, 0x19bdf0cau, 0xa02369b9u, 0x655abb50u,
    0x40685a32u, 0x3c2ab4b3u, 0x319ee9d5u, 0xc021b8f7u, 0x9b540b19u, 0x875fa099u,
    0x95f7997eu, 0x623d7da8u, 0xf837889au, 0x97e32d77u, 0x11ed935fu, 0x16681281u,
    0x0e358829u, 0xc7e61fd6u, 0x96dedfa1u, 0x7858ba99u, 0x57f584a5u, 0x1b227263u,
    0x9b83c3ffu, 0x1ac24696u, 0xcdb30aebu, 0x532e3054u, 0x8fd948e4u, 0x6dbc3128u,
    0x58ebf2efu, 0x34c6ffeau, 0xfe28ed61u, 0xee7c3c73u, 0x5d4a14d9u, 0xe864b7e3u,
    0x42105d14u, 0x203e13e0u, 0x45eee2b6u, 0xa3aaabeau, 0xdb6c4f15u, 0xfacb4fd0u,
    0xc742f442u, 0xef6abbb5u, 0x654f3b1du, 0x41cd2105u, 0xd81e799eu, 0x86854dc7u,
    0xe44b476au, 0x3d816250u, 0xcf62a1f2u, 0x5b8d2646u, 0xfc8883a0u, 0xc1c7b6a3u,
    0x7f1524c3u, 0x69cb7492u, 0x47848a0bu, 0x5692b285u, 0x095bbf00u, 0xad19489du,
    0x1462b174u, 0x23820e00u, 0x58428d2au, 0x0c55f5eau, 0x1dadf43eu, 0x233f7061u,
    0x3372f092u, 0x8d937e41u, 0xd65fecf1u, 0x6c223bdbu, 0x7cde3759u, 0xcbee7460u,
    0x4085f2a7u, 0xce77326eu, 0xa6078084u, 0x19f8509eu, 0xe8efd855u, 0x61d99735u,
    0xa969a7aau, 0xc50c06c2u, 0x5a04abfcu, 0x800bcadcu, 0x9e447a2eu, 0xc3453484u,
    0xfdd56705u, 0x0e1e9ec9u, 0xdb73dbd3u, 0x105588cdu, 0x675fda79u, 0xe3674340u,
    0xc5c43465u, 0x713e38d8u, 0x3d28f89eu, 0xf16dff20u, 0x153e21e7u, 0x8fb03d4au,
    0xe6e39f2bu, 0xdb83adf7u, 0xe93d5a68u, 0x948140f7u, 0xf64c261cu, 0x94692934u,
    0x411520f7u, 0x7602d4f7u, 0xbcf46b2eu, 0xd4a20068u, 0xd4082471u, 0x3320f46au,
    0x43b7d4b7u, 0x500061afu, 0x1e39f62eu, 0x97244546u, 0x14214f74u, 0xbf8b8840u,
    0x4d95fc1du, 0x96b591afu, 0x70f4ddd3u, 0x66a02f45u, 0xbfbc09ecu, 0x03bd9785u,
    0x7fac6dd0u, 0x31cb8504u, 0x96eb27b3u, 0x55fd3941u, 0xda2547e6u, 0xabca0a9au,
    0x28507825u, 0x530429f4u, 0x0a2c86dau, 0xe9b66dfbu, 0x68dc1462u, 0xd7486900u,
    0x680ec0a4u, 0x27a18deeu, 0x4f3ffea2u, 0xe887ad8cu, 0xb58ce006u, 0x7af4d6b6u,
    0xaace1e7cu, 0xd3375fecu, 0xce78a399u, 0x406b2a42u, 0x20fe9e35u, 0xd9f385b9u,
    0xee39d7abu, 0x3b124e8bu, 0x1dc9faf7u, 0x4b6d1856u, 0x26a36631u, 0xeae397b2u,
    0x3a6efa74u, 0xdd5b4332u, 0x6841e7f7u, 0xca7820fbu, 0xfb0af54eu, 0xd8feb397u,
    0x454056acu, 0xba489527u, 0x55533a3au, 0x20838d87u, 0xfe6ba9b7u, 0xd096954bu,
    0x55a867bcu, 0xa1159a58u, 0xcca92963u, 0x99e1db33u, 0xa62a4a56u, 0x3f3125f9u,
    0x5ef47e1cu, 0x9029317cu, 0xfdf8e802u, 0x04272f70u, 0x80bb155cu, 0x05282ce3u,
    0x95c11548u, 0xe4c66d22u, 0x48c1133fu, 0xc70f86dcu, 0x07f9c9eeu, 0x41041f0fu,
    0x404779a4u, 0x5d886e17u, 0x325f51ebu, 0xd59bc0d1u, 0xf2bcc18fu, 0x41113564u,
    0x257b7834u, 0x602a9c60u, 0xdff8e8a3u, 0x1f636c1bu, 0x0e12b4c2u, 0x02e1329eu,
    0xaf664fd1u, 0xcad18115u, 0x6b2395e0u, 0x333e92e1u, 0x3b240b62u, 0xeebeb922u,
    0x85b2a20eu, 0xe6ba0d99u, 0xde720c8cu, 0x2da2f728u, 0xd0127845u, 0x95b794fdu,
    0x647d0862u, 0xe7ccf5f0u, 0x5449a36fu, 0x877d48fau, 0xc39dfd27u, 0xf33e8d1eu,
    0x0a476341u, 0x992eff74u, 0x3a6f6eabu, 0xf4f8fd37u, 0xa812dc60u, 0xa1ebddf8u,
    0x991be14cu, 0xdb6e6b0du, 0xc67b5510u, 0x6d672c37u, 0x2765d43bu, 0xdcd0e804u,
    0xf1290dc7u, 0xcc00ffa3u, 0xb5390f92u, 0x690fed0bu, 0x667b9ffbu, 0xcedb7d9cu,
    0xa091cf0bu, 0xd9155ea3u, 0xbb132f88u, 0x515bad24u, 0x7b9479bfu, 0x763bd6ebu,
    0x37392eb3u, 0xcc115979u, 0x8026e297u, 0xf42e312du, 0x6842ada7u, 0xc66a2b3bu,
    0x12754cccu, 0x782ef11cu, 0x6a124237u, 0xb79251e7u, 0x06a1bbe6u, 0x4bfb6350u,
    0x1a6b1018u, 0x11caedfau, 0x3d25bdd8u, 0xe2e1c3c9u, 0x44421659u, 0x0a121386u,
    0xd90cec6eu, 0xd5abea2au, 0x64af674eu, 0xda86a85fu, 0xbebfe988u, 0x64e4c3feu,
    0x9dbc8057u, 0xf0f7c086u, 0x60787bf8u, 0x6003604du, 0xd1fd8346u, 0xf6381fb0u,
    0x7745ae04u, 0xd736fcccu, 0x83426b33u, 0xf01eab71u, 0xb0804187u, 0x3c005e5fu,
    0x77a057beu, 0xbde8ae24u, 0x55464299u, 0xbf582e61u, 0x4e58f48fu, 0xf2ddfda2u,
    0xf474ef38u, 0x8789bdc2u, 0x5366f9c3u, 0xc8b38e74u, 0xb475f255u, 0x46fcd9b9u,
    0x7aeb2661u, 0x8b1ddf84u, 0x846a0e79u, 0x915f95e2u, 0x466e598eu, 0x20b45770u,
    0x8cd55591u, 0xc902de4cu, 0xb90bace1u, 0xbb8205d0u, 0x11a86248u, 0x7574a99eu,
    0xb77f19b6u, 0xe0a9dc09u, 0x662d09a1u, 0xc4324633u, 0xe85a1f02u, 0x09f0be8cu,
    0x4a99a025u, 0x1d6efe10u, 0x1ab93d1du, 0x0ba5a4dfu, 0xa186f20fu, 0x2868f169u,
    0xdcb7da83u, 0x573906feu, 0xa1e2ce9bu, 0x4fcd7f52u, 0x50115e01u, 0xa70683fau,
    0xa002b5c4u, 0x0de6d027u, 0x9af88c27u, 0x773f8641u, 0xc3604c06u, 0x61a806b5u,
    0xf0177a28u, 0xc0f586e0u, 0x006058aau, 0x30dc7d62u, 0x11e69ed7u, 0x2338ea63u,
    0x53c2dd94u, 0xc2c21634u, 0xbbcbee56u, 0x90bcb6deu, 0xebfc7da1u, 0xce591d76u,
    0x6f05e409u, 0x4b7c0188u, 0x39720a3du, 0x7c927c24u, 0x86e3725fu, 0x724d9db9u,
    0x1ac15bb4u, 0xd39eb8fcu, 0xed545578u, 0x08fca5b5u, 0xd83d7cd3u, 0x4dad0fc4u,
    0x1e50ef5eu, 0xb161e6f8u, 0xa28514d9u, 0x6c51133cu, 0x6fd5c7e7u, 0x56e14ec4u,
    0x362abfceu, 0xddc6c837u, 0xd79a3234u, 0x92638212u, 0x670efa8eu, 0x406000e0u,
    0x3a39ce37u, 0xd3faf5cfu, 0xabc27737u, 0x5ac52d1bu, 0x5cb0679eu, 0x4fa33742u,
    0xd3822740u, 0x99bc9bbeu, 0xd5118e9du, 0xbf0f7315u, 0xd62d1c7eu, 0xc700c47bu,
    0xb78c1b6bu, 0x21a19045u, 0xb26eb1beu, 0x6a366eb4u, 0x5748ab2fu, 0xbc946e79u,
    0xc6a376d2u, 0x6549c2c8u, 0x530ff8eeu, 0x468dde7du, 0xd5730a1du, 0x4cd04dc6u,
    0x2939bbdbu, 0xa9ba4650u, 0xac9526e8u, 0xbe5ee304u, 0xa1fad5f0u, 0x6a2d519au,
    0x63ef8ce2u, 0x9a86ee22u, 0xc089c2b8u, 0x43242ef6u, 0xa51e03aau, 0x9cf2d0a4u,
    0x83c061bau, 0x9be96a4du, 0x8fe51550u, 0xba645bd6u, 0x2826a2f9u, 0xa73a3ae1u,
    0x4ba99586u, 0xef5562e9u, 0xc72fefd3u, 0xf752f7dau, 0x3f046f69u, 0x77fa0a59u,
    0x80e4a915u, 0x87b08601u, 0x9b09e6adu, 0x3b3ee593u, 0xe990fd5au, 0x9e34d797u,
    0x2cf0b7d9u, 0x022b8b51u, 0x96d5ac3au, 0x017da67du, 0xd1cf3ed6u, 0x7c7d2d28u,
    0x1f9f25cfu, 0xadf2b89bu, 0x5ad6b472u, 0x5a88f54cu, 0xe029ac71u, 0xe019a5e6u,
    0x47b0acfdu, 0xed93fa9bu, 0xe8d3c48du, 0x283b57ccu, 0xf8d56629u, 0x79132e28u,
    0x785f0191u, 0xed756055u, 0xf7960e44u, 0xe3d35e8cu, 0x15056dd4u, 0x88f46dbau,
    0x03a16125u, 0x0564f0bdu, 0xc3eb9e15u, 0x3c9057a2u, 0x97271aecu, 0xa93a072au,
    0x1b3f6d9bu, 0x1e6321f5u, 0xf59c66fbu, 0x26dcf319u, 0x7533d928u, 0xb155fdf5u,
    0x03563482u, 0x8aba3cbbu, 0x28517711u, 0xc20ad9f8u, 0xabcc5167u, 0xccad925fu,
    0x4de81751u, 0x3830dc8eu, 0x379d5862u, 0x9320f991u, 0xea7a90c2u, 0xfb3e7bceu,
    0x5121ce64u, 0x774fbe32u, 0xa8b6e37eu, 0xc3293d46u, 0x48de5369u, 0x6413e680u,
    0xa2ae0810u, 0xdd6db224u, 0x69852dfdu, 0x09072166u, 0xb39a460au, 0x6445c0ddu,
    0x586cdecfu, 0x1c20c8aeu, 0x5bbef7ddu, 0x1b588d40u, 0xccd2017fu, 0x6bb4e3bbu,
    0xdda26a7eu, 0x3a59ff45u, 0x3e350a44u, 0xbcb4cdd5u, 0x72eacea8u, 0xfa6484bbu,
    0x8d6612aeu, 0xbf3c6f47u, 0xd29be463u, 0x542f5d9eu, 0xaec2771bu, 0xf64e6370u,
    0x740e0d8du, 0xe75b1357u, 0xf8721671u, 0xaf537d5du, 0x4040cb08u, 0x4eb4e2ccu,
    0x34d2466au, 0x0115af84u, 0xe1b00428u, 0x95983a1du, 0x06b89fb4u, 0xce6ea048u,
    0x6f3f3b82u, 0x3520ab82u, 0x011a1d4bu, 0x277227f8u, 0x611560b1u, 0xe7933fdcu,
    0xbb3a792bu, 0x344525bdu, 0xa08839e1u, 0x51ce794bu, 0x2f32c9b7u, 0xa01fbac9u,
    0xe01cc87eu, 0xbcc7d1f6u, 0xcf0111c3u, 0xa1e8aac7u, 0x1a908749u, 0xd44fbd9au,
    0xd0dadecbu, 0xd50ada38u, 0x0339c32au, 0xc6913667u, 0x8df9317cu, 0xe0b12b4fu,
    0xf79e59b7u, 0x43f5bb3au, 0xf2d519ffu, 0x27d9459cu, 0xbf97222cu, 0x15e6fc2au,
    0x0f91fc71u, 0x9b941525u, 0xfae59361u, 0xceb69cebu, 0xc2a86459u, 0x12baa8d1u,
    0xb6c1075eu, 0xe3056a0cu, 0x10d25065u, 0xcb03a442u, 0xe0ec6e0eu, 0x1698db3bu,
    0x4c98a0beu, 0x3278e964u, 0x9f1f9532u, 0xe0d392dfu, 0xd3a0342bu, 0x8971f21eu,
    0x1b0a7441u, 0x4ba3348cu, 0xc5be7120u, 0xc37632d8u, 0xdf359f8du, 0x9b992f2eu,
    0xe60b6f47u, 0x0fe3f11du, 0xe54cda54u, 0x1edad891u, 0xce6279cfu, 0xcd3e7e6fu,
    0x1618b166u, 0xfd2c1d05u, 0x848fd2c5u, 0xf6fb2299u, 0xf523f357u, 0xa6327623u,
    0x93a83531u, 0x56cccd02u, 0xacf08162u, 0x5a75ebb5u, 0x6e163697u, 0x88d273ccu,
    0xde966292u, 0x81b949d0u, 0x4c50901bu, 0x71c65614u, 0xe6c6c7bdu, 0x327a140au,
    0x45e1d006u, 0xc3f27b9au, 0xc9aa53fdu, 0x62a80f00u, 0xbb25bfe2u, 0x35bdd2f6u,
    0x71126905u, 0xb2040222u, 0xb6cbcf7cu, 0xcd769c2bu, 0x53113ec0u, 0x1640e3d3u,
    0x38abbd60u, 0x2547adf0u, 0xba38209cu, 0xf746ce76u, 0x77afa1c5u, 0x20756060u,
    0x85cbfe4eu, 0x8ae88dd8u, 0x7aaaf9b0u, 0x4cf9aa7eu, 0x1948c25cu, 0x02fb8a8cu,
    0x01c36ae4u, 0xd6ebe1f9u, 0x90d4f869u, 0xa65cdea0u, 0x3f09252du, 0xc208e69fu,
    0xb74e6132u, 0xce77e25bu, 0x578fdfe3u, 0x3ac372e6u,
    };
    return w;
}

inline u32 *bf_P() { static u32 p[18]; return p; }
inline u32 *bf_S() { static u32 s[1024]; return s; }
inline bool &bf_key_set() { static bool b = false; return b; }

// F(x) = ((S1[a] + S2[b]) ^ S3[c]) + S4[d], all additions mod 2^32.
inline u32 bf_F(u32 x) {
    const u32 *s = bf_S();
    return ((s[(x >> 24) & 0xffu] + s[256u + ((x >> 16) & 0xffu)]) ^
             s[512u + ((x >> 8) & 0xffu)]) + s[768u + (x & 0xffu)];
}

inline u32 bf_be32(u32 p) {
    return ((u32)ld8(p) << 24) | ((u32)ld8(p + 1u) << 16) |
           ((u32)ld8(p + 2u) << 8) | (u32)ld8(p + 3u);
}
inline void bf_put_be32(u32 p, u32 v) {
    st8(p, (u8)(v >> 24)); st8(p + 1u, (u8)(v >> 16));
    st8(p + 2u, (u8)(v >> 8)); st8(p + 3u, (u8)v);
}

inline void bf_encrypt_words(u32 &l, u32 &r) {
    const u32 *p = bf_P();
    u32 xl = l, xr = r;
    for (unsigned i = 0; i < 16u; ++i) {
        xl ^= p[i];
        xr ^= bf_F(xl);
        u32 t = xl; xl = xr; xr = t;
    }
    u32 t = xl; xl = xr; xr = t;
    xr ^= p[16]; xl ^= p[17];
    l = xl; r = xr;
}

// Decryption is the same Feistel walk with the P-array consumed in reverse
// (the DLL keeps a reversed copy at object +0x51; the order is what matters).
inline void bf_decrypt_words(u32 &l, u32 &r) {
    const u32 *p = bf_P();
    u32 xl = l, xr = r;
    for (unsigned i = 17u; i > 1u; --i) {
        xl ^= p[i];
        xr ^= bf_F(xl);
        u32 t = xl; xl = xr; xr = t;
    }
    u32 t = xl; xl = xr; xr = t;
    xr ^= p[1]; xl ^= p[0];
    l = xl; r = xr;
}

// The standard key schedule. A zero-length key leaves the object unkeyed: the
// DLL clears its initialised byte, and Encrypt/Decrypt then copy verbatim.
inline void bf_set_key(const u8 *key, u32 len) {
    u32 *p = bf_P();
    u32 *s = bf_S();
    const u32 *init = bf_init_words();
    for (unsigned i = 0; i < 18u; ++i) p[i] = init[i];
    for (unsigned i = 0; i < 1024u; ++i) s[i] = init[18u + i];
    if (len == 0u) { bf_key_set() = false; return; }
    u32 j = 0;
    for (unsigned i = 0; i < 18u; ++i) {
        u32 v = 0;
        for (unsigned k = 0; k < 4u; ++k) {
            v = (v << 8) | key[j];
            j = (j + 1u) % len;
        }
        p[i] ^= v;
    }
    u32 l = 0, r = 0;
    for (unsigned i = 0; i < 18u; i += 2u) {
        bf_encrypt_words(l, r);
        p[i] = l; p[i + 1u] = r;
    }
    for (unsigned i = 0; i < 1024u; i += 2u) {
        bf_encrypt_words(l, r);
        s[i] = l; s[i + 1u] = r;
    }
    bf_key_set() = true;
}

TS_BEGIN(bf_SetKey)
    u32 len = arg(1), key = arg(2);
    if (!key) {
        // The DLL's slot 3 returns E_POINTER for a NULL key and clears the
        // initialised flag (0x11001639).
        bf_key_set() = false;
        blowfish_key_len() = 0;
        stub_done(3, 0x80004003u);
    } else if ((int)len < 0 || len > 56u) {
        stub_done(3, 0x80070057u);   // E_INVALIDARG: the DLL checks 0..0x38
    } else {
        for (u32 i = 0; i < len; ++i) blowfish_key()[i] = ld8(key + i);
        blowfish_key_len() = len;
        bf_set_key(blowfish_key(), len);
        if (std::getenv("TS_FILE_TRACE"))
            std::fprintf(stderr, "win32stubs: IBlowfish::SetKey len=%u key=%02x%02x%02x%02x "
                                 "(caller 0x%08x, step %lu)\n", len,
                         ld8(key), ld8(key + 1u), ld8(key + 2u), ld8(key + 3u),
                         ld32(ESP.d), mach().steps);
        stub_done(3, 0u);
    }
TS_END

TS_BEGIN(bf_GetMaxKeyLength)
    u32 out = arg(1);
    if (!out) { stub_done(2, 0x80004003u); }
    else { st32(out, 56u); stub_done(2, 0u); }
TS_END

TS_BEGIN(bf_GetBlockSize)
    u32 out = arg(1);
    if (!out) { stub_done(2, 0x80004003u); }
    else { st32(out, 8u); stub_done(2, 0u); }
TS_END

TS_BEGIN(bf_Encrypt)
    u32 len = arg(1), src = arg(2), dst = arg(3);
    if (!src || !dst) { stub_done(4, 0x80004003u); }
    else if ((int)len < 0) { stub_done(4, 0x80070057u); }
    else {
        u32 i = 0;
        if (bf_key_set()) {
            // Consecutive 8-byte big-endian blocks; the tail (len % 8) is
            // copied verbatim, as the DLL does. Read both words before writing
            // so in == out works, which is the game's normal case.
            for (; i + 8u <= len; i += 8u) {
                u32 l = bf_be32(src + i), r = bf_be32(src + i + 4u);
                bf_encrypt_words(l, r);
                bf_put_be32(dst + i, l);
                bf_put_be32(dst + i + 4u, r);
            }
        }
        for (; i < len; ++i) st8(dst + i, ld8(src + i));
        stub_done(4, 0u);
    }
TS_END

TS_BEGIN(bf_Decrypt)
    u32 len = arg(1), src = arg(2), dst = arg(3);
    if (!src || !dst) { stub_done(4, 0x80004003u); }
    else if ((int)len < 0) { stub_done(4, 0x80070057u); }
    else {
        u32 i = 0;
        if (bf_key_set()) {
            for (; i + 8u <= len; i += 8u) {
                u32 l = bf_be32(src + i), r = bf_be32(src + i + 4u);
                bf_decrypt_words(l, r);
                bf_put_be32(dst + i, l);
                bf_put_be32(dst + i + 4u, r);
            }
        }
        if (std::getenv("TS_FILE_TRACE"))
            std::fprintf(stderr, "win32stubs: IBlowfish::Decrypt len=%u src=%02x%02x%02x%02x "
                                 "(caller 0x%08x, step %lu)\n", len,
                         ld8(src), ld8(src + 1u), ld8(src + 2u), ld8(src + 3u),
                         ld32(ESP.d), mach().steps);
        for (; i < len; ++i) st8(dst + i, ld8(src + i));
        stub_done(4, 0u);
    }
TS_END

void k_bf_SetKey();
void k_bf_GetMaxKeyLength();
void k_bf_GetBlockSize();
void k_bf_Encrypt();
void k_bf_Decrypt();

inline u32 make_blowfish_object() {
    static const char *const names[8] = {
        "IBlowfish::QueryInterface", "IBlowfish::AddRef", "IBlowfish::Release",
        "IBlowfish::SetKey", "IBlowfish::GetMaxKeyLength",
        "IBlowfish::GetBlockSize", "IBlowfish::Encrypt", "IBlowfish::Decrypt",
    };
    static const u8 nargs[8] = { 3, 1, 1, 3, 2, 2, 4, 4 };
    Fn over[8] = { 0, 0, 0, &k_bf_SetKey, &k_bf_GetMaxKeyLength,
                   &k_bf_GetBlockSize, &k_bf_Encrypt, &k_bf_Decrypt };
    static u32 obj = 0;
    if (!obj) obj = make_com_object(names, nargs, over, 8);
    return obj;
}

// A generic object for a CLSID the model does not know. It answers
// QueryInterface/AddRef/Release so a caller is never handed a null it
// dereferences, but it is not the class that was asked for and says so.
inline u32 make_generic_unknown() {
    static const char *const names[3] = {
        "IUnknown::QueryInterface", "IUnknown::AddRef", "IUnknown::Release",
    };
    static const u8 nargs[3] = { 3, 1, 1 };
    Fn over[3] = { 0, 0, 0 };
    static u32 obj = 0;
    if (!obj) obj = make_com_object(names, nargs, over, 3);
    return obj;
}

// CLSID 1440AD10-6AA8-11D1-B6F9-00A024DDAFD1 -- the class blowfish.dll
// registers. Its bytes, not a string: the model compares what the caller
// passed at 0x69e960.
inline const u8 *clsid_blowfish() {
    static const u8 b[16] = { 0x10,0xad,0x40,0x14, 0xa8,0x6a, 0xd1,0x11,
                               0xb6,0xf9,0x00,0xa0, 0x24,0xdd,0xaf,0xd1 };
    return b;
}
// IID E0113100-6A7C-11D1-B6F9-00A024DDAFD1 -- IBlowfish, from the game's
// QueryInterface at 0x4233ed (the IID lives at 0x6c0430).
inline const u8 *iid_blowfish() {
    static const u8 b[16] = { 0x00,0x31,0x11,0xe0, 0x7c,0x6a, 0xd1,0x11,
                               0xb6,0xf9,0x00,0xa0, 0x24,0xdd,0xaf,0xd1 };
    return b;
}
// IID_IUnknown -- 00000000-0000-0000-C000-000000000046, at 0x6bf5f8.
inline const u8 *iid_iunknown() {
    static const u8 b[16] = { 0x00,0x00,0x00,0x00, 0x00,0x00, 0x00,0x00,
                               0xc0,0x00,0x00,0x00, 0x00,0x00,0x00,0x46 };
    return b;
}

inline bool guid_bytes_are(u32 p, const u8 *b) {
    if (!p) return false;
    for (unsigned i = 0; i < 16u; ++i)
        if (ld8(p + i) != b[i]) return false;
    return true;
}

inline u32 make_com_for_clsid(u32 clsid) {
    if (guid_bytes_are(clsid, clsid_blowfish())) return make_blowfish_object();
    std::printf("win32stubs: CoCreateInstance(CLSID %08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x)"
                " -- no modelled class; handing back a generic IUnknown object\n",
                (unsigned)ld32(clsid), (unsigned)ld16(clsid + 4),
                (unsigned)ld16(clsid + 6), (unsigned)ld8(clsid + 8),
                (unsigned)ld8(clsid + 9), (unsigned)ld8(clsid + 10),
                (unsigned)ld8(clsid + 11), (unsigned)ld8(clsid + 12),
                (unsigned)ld8(clsid + 13), (unsigned)ld8(clsid + 14),
                (unsigned)ld8(clsid + 15));
    return make_generic_unknown();
}

TS_BEGIN(CoCreateInstance)
    // CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv). The old row
    // had no body: it returned S_OK and wrote NOTHING to ppv, so a caller that
    // checked the HRESULT then called through the uninitialised pointer. That
    // is exactly what FUN_005e87a0 does (OleRun, then QueryInterface), and it
    // is why the registration path failed even before the DLL export mattered.
    u32 clsid = arg(0);
    u32 riid = arg(3);
    u32 out = arg(4);
    if (!out) {
        stub_done(5, 0x80004003u);   // E_POINTER
    } else if (!guid_bytes_are(riid, iid_iunknown()) &&
               !guid_bytes_are(riid, iid_blowfish())) {
        st32(out, 0u);
        std::printf("win32stubs: CoCreateInstance -> E_NOINTERFACE: the model "
                    "has no object for that IID\n");
        stub_done(5, 0x80004002u);   // E_NOINTERFACE
    } else {
        u32 obj = make_com_for_clsid(clsid);
        st32(out, obj);
        stub_done(5, 0u);            // S_OK, and *ppv is a real object
    }
TS_END

// ===========================================================================
// Module export tables
// ===========================================================================
// The exports that are not in the import directory. blowfish.dll is loaded by
// name and its DllRegisterServer is called through GetProcAddress; comctl32's
// DllGetVersion is the same shape. Each is a registered stub with a measured
// argument count, so a caller that gets one back can call it and the stack
// stays balanced.
TS_BEGIN(blowfish_DllRegisterServer)
    std::printf("win32stubs: blowfish.dll!DllRegisterServer -> S_OK; the "
                "Blowfish class is registered in the model\n");
    stub_done(0, 0u);
TS_END

TS_BEGIN(blowfish_DllUnregisterServer)
    std::printf("win32stubs: blowfish.dll!DllUnregisterServer -> S_OK\n");
    stub_done(0, 0u);
TS_END

// S_FALSE: the module stays loaded. The real DLL answers this from its own
// object count; nothing in the model unloads it.
TS_RET(blowfish_DllCanUnloadNow, 0, 1u)

TS_BEGIN(blowfish_DllGetClassObject)
    // Not reached by this game: its COM calls go through CoCreateInstance,
    // which the model serves directly. Answering with a class factory that
    // does not exist would be a worse lie than the real error code.
    u32 out = arg(2);
    if (out) st32(out, 0u);
    std::printf("win32stubs: blowfish.dll!DllGetClassObject -> "
                "CLASS_E_CLASSNOTAVAILABLE (CoCreateInstance is served directly)\n");
    stub_done(3, 0x80040111u);
TS_END

inline ModuleDef *module_defs(unsigned *count) {
    static ExportDef blowfish_exports[] = {
        { "DllCanUnloadNow",     0, 1u,          &k_blowfish_DllCanUnloadNow,     0 },
        { "DllGetClassObject",   3, 0x80040111u, &k_blowfish_DllGetClassObject,   0 },
        { "DllRegisterServer",   0, 0u,          &k_blowfish_DllRegisterServer,   0 },
        { "DllUnregisterServer", 0, 0u,          &k_blowfish_DllUnregisterServer, 0 },
    };
    static ExportDef comctl32_exports[] = {
        { "DllGetVersion",       1, 0u,          &k_DllGetVersion,                0 },
    };
    static ModuleDef t[] = {
        { "blowfish.dll", blowfish_exports, 4 },
        { "comctl32.dll", comctl32_exports, 1 },
    };
    *count = 2;
    return t;
}

struct LoadedModule { u32 handle; char name[64]; const ModuleDef *def; };
inline LoadedModule *loaded_modules() { static LoadedModule m[32]; return m; }
inline unsigned &loaded_module_count() { static unsigned n = 0; return n; }

// A module handle's file name, for the resource reader. GetModuleHandleA
// answers 0x00400000 for "this image", and LoadLibraryA's handles are the
// rows loaded_modules() keeps.
inline const char *module_name_of(u32 handle) {
    if (handle == 0x00400000u) return "GAME.EXE";
    for (unsigned i = 0; i < loaded_module_count(); ++i)
        if (loaded_modules()[i].handle == handle) return loaded_modules()[i].name;
    return 0;
}

inline u32 module_load(const char *name) {
    register_module_exports();
    unsigned n = loaded_module_count();
    for (unsigned i = 0; i < n; ++i)
        if (same_dll(loaded_modules()[i].name, name))
            return loaded_modules()[i].handle;
    if (n >= 32u) return 0;
    unsigned ndefs = 0;
    ModuleDef *defs = module_defs(&ndefs);
    const ModuleDef *def = 0;
    for (unsigned i = 0; i < ndefs; ++i)
        if (same_dll(defs[i].name, name)) { def = &defs[i]; break; }
    LoadedModule &m = loaded_modules()[n];
    m.handle = 0x00500000u + n * 0x10u;
    unsigned j = 0;
    for (; name[j] && j + 1u < sizeof(m.name); ++j) m.name[j] = name[j];
    m.name[j] = 0;
    m.def = def;
    loaded_module_count() = n + 1u;
    if (def) {
        std::printf("win32stubs: LoadLibraryA(\"%s\") -> 0x%08x (%u modelled "
                    "export(s):", name, m.handle, def->nexports);
        for (unsigned i = 0; i < def->nexports; ++i)
            std::printf(" %s", def->exports[i].fn);
        std::printf(")\n");
    } else {
        // Keep the old behaviour -- a non-null handle -- but say what it is.
        std::printf("win32stubs: LoadLibraryA(\"%s\") -> 0x%08x -- no exports "
                    "are modelled for this module; GetProcAddress on it "
                    "resolves nothing\n", name, m.handle);
    }
    return m.handle;
}

inline u32 module_find_export(u32 handle, const char *fn) {
    register_module_exports();
    unsigned n = loaded_module_count();
    for (unsigned i = 0; i < n; ++i) {
        const LoadedModule &m = loaded_modules()[i];
        if (m.handle != handle) continue;
        if (!m.def) return 0;
        for (unsigned j = 0; j < m.def->nexports; ++j)
            if (!std::strcmp(m.def->exports[j].fn, fn))
                return m.def->exports[j].va;
        return 0;
    }
    // A handle that did not come from LoadLibraryA is the image itself
    // (GetModuleHandleA returns 0x00400000). Its exports are not modelled, so
    // fall back to the global import names -- the behaviour GetProcAddress had
    // before handles meant anything.
    return find_by_name(fn);
}

inline void register_module_exports() {
    // Idempotent, and called lazily from module_load/module_find_export rather
    // than from register_impl: register_impl is in the generated tail of this
    // header (tools/gen_win32_stubs.py owns everything after "// the import
    // table"), so a call placed there is removed the next time the table is
    // regenerated. This machinery lives in the hand-written head, which the
    // generator keeps byte-for-byte.
    static bool done = false;
    if (done) return;
    done = true;
    unsigned n = 0;
    ModuleDef *defs = module_defs(&n);
    for (unsigned i = 0; i < n; ++i) {
        for (unsigned j = 0; j < defs[i].nexports; ++j) {
            ExportDef &e = defs[i].exports[j];
            if (!e.real) continue;
            e.va = alloc_stub_va();
            ts_register(e.va, e.real);
        }
    }
}

inline u32 make_dd_palette() {
    // IDirectDrawPalette: QueryInterface/AddRef/Release/GetCaps/GetEntries/
    // SetEntries/Initialize. FUN_0046f670 checks only that CreatePalette returned
    // DD_OK, but it stores the out-pointer and the game dereferences it later, so
    // returning 0 while leaving *lplpDDPalette NULL moves the failure downstream
    // where it is much harder to read.
    static const char *const names[7] = {
        "IDirectDrawPalette::QueryInterface", "IDirectDrawPalette::AddRef",
        "IDirectDrawPalette::Release", "IDirectDrawPalette::GetCaps",
        "IDirectDrawPalette::GetEntries", "IDirectDrawPalette::SetEntries",
        "IDirectDrawPalette::Initialize",
    };
    static const u8 nargs[7] = { 3, 1, 1, 2, 4, 4, 4 };
    Fn over[7] = { 0 };
    static u32 obj = 0;
    if (!obj) obj = make_com_object(names, nargs, over, 7);
    return obj;
}


void k_sfs_Lock();
void k_sfs_GetSurfaceDesc();
void k_sfs_GetAttachedSurface();
inline u32 make_surface() {
    // IDirectDrawSurface, DirectX 6 slot order. Every slot is a stub; the two
    // that hand back memory are given real bodies because a caller that locks
    // a surface and draws into it must be handed somewhere to draw.
    static const char *const names[45] = {
        "IDirectDrawSurface::QueryInterface", "IDirectDrawSurface::AddRef",
        "IDirectDrawSurface::Release", "IDirectDrawSurface::AddAttachedSurface",
        "IDirectDrawSurface::AddOverlayDirtyRect", "IDirectDrawSurface::Blt",
        "IDirectDrawSurface::BltBatch", "IDirectDrawSurface::BltFast",
        "IDirectDrawSurface::DeleteAttachedSurface",
        "IDirectDrawSurface::EnumAttachedSurfaces",
        "IDirectDrawSurface::EnumOverlayZOrders", "IDirectDrawSurface::Flip",
        "IDirectDrawSurface::GetAttachedSurface", "IDirectDrawSurface::GetBltStatus",
        "IDirectDrawSurface::GetCaps", "IDirectDrawSurface::GetClipper",
        "IDirectDrawSurface::GetColorKey", "IDirectDrawSurface::GetDC",
        "IDirectDrawSurface::GetFlipStatus", "IDirectDrawSurface::GetOverlayPosition",
        "IDirectDrawSurface::GetPalette", "IDirectDrawSurface::GetPixelFormat",
        "IDirectDrawSurface::GetSurfaceDesc", "IDirectDrawSurface::Initialize",
        "IDirectDrawSurface::IsLost", "IDirectDrawSurface::Lock",
        "IDirectDrawSurface::ReleaseDC", "IDirectDrawSurface::Restore",
        "IDirectDrawSurface::SetClipper", "IDirectDrawSurface::SetColorKey",
        "IDirectDrawSurface::SetOverlayPosition", "IDirectDrawSurface::SetPalette",
        "IDirectDrawSurface::Unlock", "IDirectDrawSurface::UpdateOverlay",
        "IDirectDrawSurface::UpdateOverlayDisplay", "IDirectDrawSurface::UpdateOverlayZOrder",
        "IDirectDrawSurface::GetDDInterface", "IDirectDrawSurface::PageLock",
        "IDirectDrawSurface::PageUnlock", "IDirectDrawSurface::SetSurfaceDesc",
        "IDirectDrawSurface::SetPrivateData", "IDirectDrawSurface::GetPrivateData",
        "IDirectDrawSurface::FreePrivateData", "IDirectDrawSurface::GetUniquenessValue",
        "IDirectDrawSurface::ChangeUniquenessValue",
    };
    static const u8 nargs[45] = {
        3, 1, 1, 2, 2, 5, 3, 6, 3, 4, 4, 3, 3, 2, 2, 2, 3, 2, 2, 3,
        2, 2, 2, 3, 1, 5, 2, 1, 2, 3, 3, 2, 2, 4, 2, 3, 2, 2, 2, 3,
        4, 3, 2, 2, 1,
    };
    Fn over[45] = { 0 };
    over[25] = &k_sfs_Lock;          // Lock
    over[22] = &k_sfs_GetSurfaceDesc;
    over[12] = &k_sfs_GetAttachedSurface;
    // One vtable, one object per surface: slot_defs() is a fixed table, so the
    // 45 slot definitions are built once and every surface shares them.
    static u32 vt = 0;
    if (!vt) vt = make_com_vtable(names, nargs, over, 45);
    u32 obj = alloc_block(16u, 4u);
    st32(obj, vt);
    return obj;
}

TS_BEGIN(sfs_GetSurfaceDesc)
    // DDSURFACEDESC as THIS GAME reads it, taken from its own wrapper at
    // 0x4874c0 / 0x487ad0: +8 dwHeight, +12 dwWidth, +0x10 lPitch,
    // +0x24 lpSurface, +0x54 dwRGBBitCount. The wrapper computes
    // bytes-per-pixel as (*(u32*)(desc+0x54) + 7) >> 3 and requires 2 for
    // 16bpp; writing the bit count at +0x68 made that 0 and the game took its
    // "Direct X" error path.
    u32 p = arg(1);
    if (p) {
        for (u32 i = 0; i < 108u; ++i) st8(p + i, 0);
        st32(p + 0u, 108u);
        st32(p + 4u, 0x00000041u);
        st32(p + 8u, SCREEN_H);
        st32(p + 12u, SCREEN_W);
        st32(p + 16u, SCREEN_W * 2u);   // lPitch
        st32(p + 0x54u, SCREEN_BPP);    // ddpfPixelFormat.dwRGBBitCount
    }
    stub_done(2, 0u);
TS_END

TS_BEGIN(sfs_Lock)
    // Lock(this, lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent). COM is
    // __stdcall, so `this` is the FIRST STACK ARGUMENT, not ECX: five words are
    // pushed, and the stub must pop all five. It used to pop four, leaking four
    // bytes on every Lock; the leak walked the caller's frame until EBP was
    // zero and the game called through a null surface in its teardown. The
    // game's own call site (0x487b31) pushes hEvent first, then flags, desc,
    // rect and this, and has no add-esp cleanup, so the count is visible in
    // the binary.
    u32 p = arg(2);
    static u32 buffer = 0;
    if (!buffer) buffer = zero_block(SCREEN_W * SCREEN_H * 2u, 0x1000u);
    if (p) {
        for (u32 i = 0; i < 108u; ++i) st8(p + i, 0);
        st32(p + 0u, 108u);
        st32(p + 4u, 0x00000041u);
        st32(p + 8u, SCREEN_H);
        st32(p + 12u, SCREEN_W);
        st32(p + 16u, SCREEN_W * 2u);   // lPitch: the surface's stride in bytes
        st32(p + 0x24u, buffer);        // lpSurface: the wrapper reads +0x24
        st32(p + 0x54u, SCREEN_BPP);    // ddpfPixelFormat.dwRGBBitCount
    }
    stub_done(5, 0u);
TS_END

TS_BEGIN(sfs_GetAttachedSurface)
    u32 out = arg(2);
    if (out) st32(out, make_surface());
    stub_done(3, 0u);
TS_END

inline u32 make_directdraw() {
    // IDirectDraw, DirectX 6 slot order. Slots whose out-parameter the game
    // reads (display mode, caps, monitor) are the ones with real bodies.
    static const char *const names[25] = {
        "IDirectDraw::QueryInterface", "IDirectDraw::AddRef",
        "IDirectDraw::Release", "IDirectDraw::Compact",
        "IDirectDraw::CreateClipper", "IDirectDraw::CreatePalette",
        "IDirectDraw::CreateSurface", "IDirectDraw::DuplicateSurface",
        "IDirectDraw::EnumDisplayModes", "IDirectDraw::EnumSurfaces",
        "IDirectDraw::FlipToGDISurface", "IDirectDraw::GetCaps",
        "IDirectDraw::GetDisplayMode", "IDirectDraw::GetFourCCCodes",
        "IDirectDraw::GetGDISurface", "IDirectDraw::GetMonitorFrequency",
        "IDirectDraw::GetScanLine", "IDirectDraw::GetVerticalBlankStatus",
        "IDirectDraw::Initialize", "IDirectDraw::RestoreDisplayMode",
        "IDirectDraw::SetCooperativeLevel", "IDirectDraw::SetDisplayMode",
        "IDirectDraw::WaitForVerticalBlank",
        // 23 and 24 exist on the real interface and the game CALLS 23. Wine's
        // +ddraw trace of the retail binary shows GetAvailableVidMem right after
        // GetCaps, before SetCooperativeLevel. With a 23-slot table that call
        // read one entry past the end of the array and landed on whatever
        // followed it, so the game called a non-function and the failure showed
        // up much later as an argument holding a stub address.
        "IDirectDraw::GetAvailableVidMem", "IDirectDraw::GetDeviceIdentifier",
    };
    static const u8 nargs[25] = {
        3, 1, 1, 1, 4, 4, 4, 3, 5, 5, 1, 3, 2, 3, 2, 2, 2, 2, 2, 1, 3, 4, 3,
        4, 3,
    };
    Fn over[25] = { 0 };
    over[4] = &k_dd_CreateClipper;
    over[5] = &k_dd_CreatePalette;
    over[6] = &k_dd_CreateSurface;
    over[11] = &k_dd_GetCaps;
    over[12] = &k_dd_GetDisplayMode;
    over[15] = &k_dd_GetMonitorFrequency;
    over[16] = &k_dd_GetScanLine;
    over[17] = &k_dd_GetVerticalBlankStatus;
    over[23] = &k_dd_GetAvailableVidMem;
    static u32 vt = 0;
    if (!vt) vt = make_com_vtable(names, nargs, over, 25);
    u32 obj = alloc_block(16u, 4u);
    st32(obj, vt);
    return obj;
}

TS_BEGIN(DirectDrawCreate)
    u32 out = arg(1);
    if (out) st32(out, make_directdraw());
    stub_done(3, 0u);   // DD_OK
TS_END

TS_ORD_BEGIN(dsound_ord_2147483649, "DSOUND", 0x80000001u)   // DirectSoundCreate
    // The retail game on this machine has no audio device: it reports
    // "Unable to create Direct Sound Object. Proceeding with audio disabled."
    // and continues. The old body returned DS_OK and handed back an
    // IDirectDraw object, which is a lie twice over -- the game then built a
    // primary sound buffer through an interface that has no CreateSoundBuffer,
    // left the buffer pointer null, and released it, which is the call to
    // address 0 at 0x484839. A stub that fails the way the environment does is
    // the honest model, and it takes the game's own audio-disabled path.
    u32 out = arg(1);
    if (out) st32(out, 0u);
    std::printf("win32stubs: DirectSoundCreate -> DSERR_NODRIVER; audio is "
                "disabled, as it is in the retail environment here\n");
    stub_done_for("DSOUND", ord, 0x88780078u);   // DSERR_NODRIVER
TS_ORD_END

// the import table
//
// One row per import in bin/GAME.EXE's import directory, in directory order,
// followed by the exports reached only through GetProcAddress.
// `nargs` is the number of __stdcall arguments the callee owns and must pop;
// `ret` is the value a stub with no body of its own leaves in EAX; `real` is a
// hand-written body above, or 0 to use the named generic stub.
//
// Regenerate with tools/gen_win32_stubs.py -- the rows are data, and a wrong
// argument count is invisible until a caller's stack has already shifted.
struct ImportDef { const char *dll; const char *fn; u8 nargs; u32 ret; Fn real; };

inline const ImportDef *imports(unsigned *count) {
    static const ImportDef t[] = {
	{ "KERNEL32", "FileTimeToSystemTime", 2, 0x00000000u, &k_FileTimeToSystemTime },
	{ "KERNEL32", "FileTimeToLocalFileTime", 2, 0x00000000u, &k_FileTimeToLocalFileTime },
	{ "KERNEL32", "GetDateFormatA", 5, 0x00000000u, &k_GetDateFormatA },
	{ "KERNEL32", "GetTimeFormatA", 5, 0x00000000u, &k_GetTimeFormatA },
	{ "KERNEL32", "SystemTimeToFileTime", 2, 0x00000000u, &k_SystemTimeToFileTime },
	{ "KERNEL32", "QueryPerformanceFrequency", 1, 0x00000000u, &k_QueryPerformanceFrequency },
	{ "KERNEL32", "QueryPerformanceCounter", 1, 0x00000000u, &k_QueryPerformanceCounter },
	{ "KERNEL32", "EscapeCommFunction", 2, 0x00000000u, &k_EscapeCommFunction },
	{ "KERNEL32", "ClearCommBreak", 1, 0x00000000u, &k_ClearCommBreak },
	{ "KERNEL32", "CompareFileTime", 2, 0x00000000u, &k_CompareFileTime },
	{ "KERNEL32", "SetCommBreak", 1, 0x00000000u, &k_SetCommBreak },
	{ "KERNEL32", "SetFilePointer", 4, 0x00000000u, &k_SetFilePointer },
	{ "KERNEL32", "GetFileSize", 2, 0x00000000u, &k_GetFileSize },
	{ "KERNEL32", "FileTimeToDosDateTime", 3, 0x00000000u, &k_FileTimeToDosDateTime },
	{ "KERNEL32", "GetFileInformationByHandle", 2, 0x00000001u, &k_GetFileInformationByHandle },
	{ "KERNEL32", "SetFileTime", 4, 0x00000001u, &k_SetFileTime },
	{ "KERNEL32", "DosDateTimeToFileTime", 3, 0x00000000u, &k_DosDateTimeToFileTime },
	{ "KERNEL32", "lstrlen", 1, 0x00000000u, &k_lstrlen },
	{ "KERNEL32", "lstrcat", 2, 0x00000000u, &k_lstrcat },
	{ "KERNEL32", "lstrcpy", 2, 0x00000000u, &k_lstrcpy },
	{ "KERNEL32", "SetUnhandledExceptionFilter", 1, 0x00000000u, &k_SetUnhandledExceptionFilter },
	{ "KERNEL32", "ReadFile", 5, 0x00000001u, &k_ReadFile },
	{ "KERNEL32", "DeleteFileA", 1, 0x00000001u, &k_DeleteFileA },
	{ "KERNEL32", "GetFileTime", 4, 0x00000001u, &k_GetFileTime },
	{ "KERNEL32", "GlobalMemoryStatus", 1, 0x00000000u, &k_GlobalMemoryStatus },
	{ "KERNEL32", "SetVolumeLabelA", 2, 0x00000000u, &k_SetVolumeLabelA },
	{ "KERNEL32", "GetLogicalDriveStringsA", 2, 0x00000000u, &k_GetLogicalDriveStringsA },
	{ "KERNEL32", "ClearCommError", 3, 0x00000000u, &k_ClearCommError },
	{ "KERNEL32", "SetCommTimeouts", 2, 0x00000000u, &k_SetCommTimeouts },
	{ "KERNEL32", "SetCommState", 2, 0x00000000u, &k_SetCommState },
	{ "KERNEL32", "GetCommState", 2, 0x00000000u, &k_GetCommState },
	{ "KERNEL32", "GetTickCount", 0, 0x00000000u, &k_GetTickCount },
	{ "KERNEL32", "GetSystemTime", 1, 0x00000000u, &k_GetSystemTime },
	{ "KERNEL32", "SetupComm", 3, 0x00000000u, &k_SetupComm },
	{ "KERNEL32", "GetCommMask", 2, 0x00000000u, &k_GetCommMask },
	{ "KERNEL32", "GetCommModemStatus", 2, 0x00000000u, &k_GetCommModemStatus },
	{ "KERNEL32", "ResetEvent", 1, 0x00000001u, &k_ResetEvent },
	{ "KERNEL32", "GetOverlappedResult", 4, 0x00000001u, &k_GetOverlappedResult },
	{ "KERNEL32", "GetVersionExA", 1, 0x00000000u, &k_GetVersionExA },
	{ "KERNEL32", "GetExitCodeProcess", 2, 0x00000000u, &k_GetExitCodeProcess },
	{ "KERNEL32", "CreateProcessA", 10, 0x00000000u, &k_CreateProcessA },
	{ "KERNEL32", "SetEvent", 1, 0x00000001u, &k_SetEvent },
	{ "KERNEL32", "CreateDirectoryA", 2, 0x00000001u, &k_CreateDirectoryA },
	{ "KERNEL32", "GetCurrentDirectoryA", 2, 0x00000000u, &k_GetCurrentDirectoryA },
	{ "KERNEL32", "LockResource", 1, 0x00000000u, &k_LockResource },
	{ "KERNEL32", "LoadLibraryA", 1, 0x00000000u, &k_LoadLibraryA },
	{ "KERNEL32", "GetModuleHandleA", 1, 0x00000000u, &k_GetModuleHandleA },
	{ "KERNEL32", "GetProcAddress", 2, 0x00000000u, &k_GetProcAddress },
	{ "KERNEL32", "GetDiskFreeSpaceA", 5, 0x00000000u, &k_GetDiskFreeSpaceA },
	{ "KERNEL32", "FindNextFileA", 2, 0x00000000u, &k_FindNextFileA },
	{ "KERNEL32", "FindFirstFileA", 2, 0x00000000u, &k_FindFirstFileA },
	{ "KERNEL32", "FindClose", 1, 0x00000000u, &k_FindClose },
	{ "KERNEL32", "GetDriveTypeA", 1, 0x00000003u, &k_GetDriveTypeA },
	{ "KERNEL32", "CreateFileA", 7, 0x00000000u, &k_CreateFileA },
	{ "KERNEL32", "DeviceIoControl", 8, 0x00000000u, &k_DeviceIoControl },
	{ "KERNEL32", "GetLastError", 0, 0x00000000u, &k_GetLastError },
	{ "KERNEL32", "Sleep", 1, 0x00000000u, &k_Sleep },
	{ "KERNEL32", "CloseHandle", 1, 0x00000001u, &k_CloseHandle },
	{ "KERNEL32", "MultiByteToWideChar", 6, 0x00000000u, &k_MultiByteToWideChar },
	{ "KERNEL32", "InterlockedIncrement", 1, 0x00000000u, &k_InterlockedIncrement },
	{ "KERNEL32", "InterlockedDecrement", 1, 0x00000000u, &k_InterlockedDecrement },
	{ "KERNEL32", "DeleteCriticalSection", 1, 0x00000000u, &k_DeleteCriticalSection },
	{ "KERNEL32", "GetCurrentThreadId", 0, 0x00000000u, &k_GetCurrentThreadId },
	{ "KERNEL32", "InitializeCriticalSection", 1, 0x00000000u, &k_InitializeCriticalSection },
	{ "KERNEL32", "EnterCriticalSection", 1, 0x00000000u, &k_EnterCriticalSection },
	{ "KERNEL32", "WideCharToMultiByte", 8, 0x00000000u, &k_WideCharToMultiByte },
	{ "KERNEL32", "VirtualQuery", 3, 0x00000000u, &k_VirtualQuery },
	{ "KERNEL32", "VirtualProtect", 4, 0x00000000u, &k_VirtualProtect },
	{ "KERNEL32", "FlushInstructionCache", 3, 0x00000000u, &k_FlushInstructionCache },
	{ "KERNEL32", "WriteFile", 5, 0x00000001u, &k_WriteFile },
	{ "KERNEL32", "GetCurrentProcess", 0, 0xffffffffu, &k_GetCurrentProcess },
	{ "KERNEL32", "IsBadCodePtr", 1, 0x00000000u, &k_IsBadCodePtr },
	{ "KERNEL32", "IsBadHugeReadPtr", 2, 0x00000000u, &k_IsBadHugeReadPtr },
	{ "KERNEL32", "ExitProcess", 1, 0x00000000u, &k_ExitProcess },
	{ "KERNEL32", "AllocConsole", 0, 0x00000000u, &k_AllocConsole },
	{ "KERNEL32", "FreeConsole", 0, 0x00000000u, &k_FreeConsole },
	{ "KERNEL32", "WaitForSingleObject", 2, 0x00000000u, &k_WaitForSingleObject },
	{ "KERNEL32", "WaitForMultipleObjects", 4, 0x00000000u, &k_WaitForMultipleObjects },
	{ "KERNEL32", "ReleaseMutex", 1, 0x00000001u, &k_ReleaseMutex },
	{ "KERNEL32", "CreateMutexA", 3, 0x00000000u, &k_CreateMutexA },
	{ "KERNEL32", "FreeLibrary", 1, 0x00000001u, &k_FreeLibrary },
	{ "KERNEL32", "GetVolumeInformationA", 8, 0x00000000u, &k_GetVolumeInformationA },
	{ "KERNEL32", "GetModuleFileNameA", 3, 0x00000000u, &k_GetModuleFileNameA },
	{ "KERNEL32", "FindResourceA", 3, 0x00000000u, &k_FindResourceA },
	{ "KERNEL32", "LoadResource", 2, 0x00000000u, &k_LoadResource },
	{ "KERNEL32", "CreateEventA", 4, 0x00000000u, &k_CreateEventA },
	{ "KERNEL32", "PurgeComm", 2, 0x00000000u, &k_PurgeComm },
	{ "KERNEL32", "SetCurrentDirectoryA", 1, 0x00000001u, &k_SetCurrentDirectoryA },
	{ "KERNEL32", "OpenMutexA", 3, 0x00000000u, &k_OpenMutexA },
	{ "KERNEL32", "TlsGetValue", 1, 0x00000000u, &k_TlsGetValue },
	{ "KERNEL32", "WriteConsoleA", 5, 0x00000000u, &k_WriteConsoleA },
	{ "KERNEL32", "GetNumberOfConsoleInputEvents", 2, 0x00000000u, &k_GetNumberOfConsoleInputEvents },
	{ "KERNEL32", "TlsAlloc", 0, 0x00000000u, &k_TlsAlloc },
	{ "KERNEL32", "TlsSetValue", 2, 0x00000001u, &k_TlsSetValue },
	{ "KERNEL32", "TlsFree", 1, 0x00000001u, &k_TlsFree },
	{ "KERNEL32", "BackupSeek", 6, 0x00000000u, &k_BackupSeek },
	{ "KERNEL32", "LCMapStringA", 6, 0x00000000u, &k_LCMapStringA },
	{ "KERNEL32", "RaiseException", 4, 0x00000000u, &k_RaiseException },
	{ "KERNEL32", "HeapSize", 3, 0x00000000u, &k_HeapSize },
	{ "KERNEL32", "HeapReAlloc", 4, 0x00000000u, &k_HeapReAlloc },
	{ "KERNEL32", "FatalAppExitA", 2, 0x00000000u, &k_FatalAppExitA },
	{ "KERNEL32", "GetCommandLineA", 0, 0x00000000u, &k_GetCommandLineA },
	{ "KERNEL32", "GetStartupInfoA", 1, 0x00000000u, &k_GetStartupInfoA },
	{ "KERNEL32", "GetVersion", 0, 0x04000004u, &k_GetVersion },
	{ "KERNEL32", "RtlUnwind", 4, 0x00000000u, &k_RtlUnwind },
	{ "KERNEL32", "SetLastError", 1, 0x00000000u, &k_SetLastError },
	{ "KERNEL32", "GetCurrentThread", 0, 0xfffffffeu, &k_GetCurrentThread },
	{ "KERNEL32", "HeapAlloc", 3, 0x00000000u, &k_HeapAlloc },
	{ "KERNEL32", "PeekConsoleInputA", 4, 0x00000000u, &k_PeekConsoleInputA },
	{ "KERNEL32", "GetLocalTime", 1, 0x00000000u, &k_GetLocalTime },
	{ "KERNEL32", "GetTimeZoneInformation", 1, 0x00000000u, &k_GetTimeZoneInformation },
	{ "KERNEL32", "TerminateProcess", 2, 0x00000000u, &k_TerminateProcess },
	{ "KERNEL32", "HeapDestroy", 1, 0x00000000u, &k_HeapDestroy },
	{ "KERNEL32", "HeapCreate", 3, 0x00000000u, &k_HeapCreate },
	{ "KERNEL32", "CreateEventW", 4, 0x00000000u, &k_CreateEventW },
	{ "KERNEL32", "VirtualFree", 3, 0x00000000u, &k_VirtualFree },
	{ "KERNEL32", "VirtualAlloc", 4, 0x00000000u, &k_VirtualAlloc },
	{ "KERNEL32", "IsBadHugeWritePtr", 2, 0x00000000u, &k_IsBadHugeWritePtr },
	{ "KERNEL32", "SetHandleCount", 1, 0x00000000u, &k_SetHandleCount },
	{ "KERNEL32", "GetStdHandle", 1, 0x00000000u, &k_GetStdHandle },
	{ "KERNEL32", "GetFileType", 1, 0x00000002u, &k_GetFileType },
	{ "KERNEL32", "GetCPInfo", 2, 0x00000000u, &k_GetCPInfo },
	{ "KERNEL32", "GetACP", 0, 0x000004e4u, &k_GetACP },
	{ "KERNEL32", "GetOEMCP", 0, 0x000001b5u, &k_GetOEMCP },
	{ "KERNEL32", "SetConsoleCtrlHandler", 2, 0x00000000u, &k_SetConsoleCtrlHandler },
	{ "KERNEL32", "GetFileAttributesA", 1, 0x00000020u, &k_GetFileAttributesA },
	{ "KERNEL32", "GetCurrentProcessId", 0, 0x00000000u, &k_GetCurrentProcessId },
	{ "KERNEL32", "UnhandledExceptionFilter", 1, 0x00000001u, &k_UnhandledExceptionFilter },
	{ "KERNEL32", "FreeEnvironmentStringsA", 1, 0x00000000u, &k_FreeEnvironmentStringsA },
	{ "KERNEL32", "AddAtomW", 1, 0x00000000u, &k_AddAtomW },
	{ "KERNEL32", "LeaveCriticalSection", 1, 0x00000000u, &k_LeaveCriticalSection },
	{ "KERNEL32", "GetEnvironmentStrings", 0, 0x00000000u, &k_GetEnvironmentStrings },
	{ "KERNEL32", "CloseProfileUserMapping", 0, 0x00000000u, &k_CloseProfileUserMapping },
	{ "KERNEL32", "IsValidLocale", 2, 0x00000001u, &k_IsValidLocale },
	{ "KERNEL32", "IsValidCodePage", 1, 0x00000001u, &k_IsValidCodePage },
	{ "KERNEL32", "GetLocaleInfoA", 4, 0x00000000u, &k_GetLocaleInfoA },
	{ "KERNEL32", "EnumSystemLocalesA", 2, 0x00000000u, &k_EnumSystemLocalesA },
	{ "KERNEL32", "GetUserDefaultLCID", 0, 0x00000409u, &k_GetUserDefaultLCID },
	{ "KERNEL32", "GetStringTypeA", 5, 0x00000000u, &k_GetStringTypeA },
	{ "KERNEL32", "GetSystemTimeAsFileTime", 1, 0x00000000u, &k_GetSystemTimeAsFileTime },
	{ "KERNEL32", "FlushFileBuffers", 1, 0x00000000u, &k_FlushFileBuffers },
	{ "KERNEL32", "SetStdHandle", 2, 0x00000000u, &k_SetStdHandle },
	{ "KERNEL32", "CompareStringA", 6, 0x00000000u, &k_CompareStringA },
	{ "KERNEL32", "BackupSeek", 6, 0x00000000u, &k_BackupSeek },
	{ "KERNEL32", "SetEnvironmentVariableA", 2, 0x00000000u, &k_SetEnvironmentVariableA },
	{ "KERNEL32", "LocalFree", 1, 0x00000000u, &k_LocalFree },
	{ "KERNEL32", "HeapFree", 3, 0x00000000u, &k_HeapFree },
	{ "KERNEL32", "GetConsoleMode", 2, 0x00000000u, &k_GetConsoleMode },
	{ "KERNEL32", "SetConsoleMode", 2, 0x00000000u, &k_SetConsoleMode },
	{ "KERNEL32", "lstrlenW", 1, 0x00000000u, &k_lstrlenW },
	{ "KERNEL32", "SetEndOfFile", 1, 0x00000001u, &k_SetEndOfFile },
	{ "KERNEL32", "CreateEventW", 4, 0x00000000u, &k_CreateEventW },
	{ "KERNEL32", "ReadConsoleInputA", 4, 0x00000000u, &k_ReadConsoleInputA },
	{ "USER32", "DialogBoxIndirectParamA", 5, 0x00000000u, &k_DialogBoxIndirectParamA },
	{ "USER32", "DialogBoxParamA", 5, 0x00000000u, &k_DialogBoxParamA },
	{ "USER32", "GetKeyNameTextA", 4, 0x00000000u, &k_GetKeyNameTextA },
	{ "USER32", "DrawTextA", 5, 0x00000000u, &k_DrawTextA },
	{ "USER32", "ShowCursor", 1, 0x00000000u, &k_ShowCursor },
	{ "USER32", "InvalidateRect", 3, 0x00000000u, &k_InvalidateRect },
	{ "USER32", "SendMessageA", 4, 0x00000000u, &k_SendMessageA },
	{ "USER32", "GetFocus", 0, 0x00000000u, &k_GetFocus },
	{ "USER32", "GetDlgItem", 2, 0x00000000u, &k_GetDlgItem },
	{ "USER32", "EnableWindow", 2, 0x00000000u, &k_EnableWindow },
	{ "USER32", "GetWindowLongA", 2, 0x00000000u, &k_GetWindowLongA },
	{ "USER32", "SetWindowTextA", 2, 0x00000000u, &k_SetWindowTextA },
	{ "USER32", "UpdateWindow", 1, 0x00000000u, &k_UpdateWindow },
	{ "USER32", "ClientToScreen", 2, 0x00000000u, &k_ClientToScreen },
	{ "USER32", "GetClientRect", 2, 0x00000000u, &k_GetClientRect },
	{ "USER32", "DestroyWindow", 1, 0x00000000u, &k_DestroyWindow },
	{ "USER32", "GetKeyState", 1, 0x00000000u, &k_GetKeyState },
	{ "USER32", "ToAscii", 5, 0x00000000u, &k_ToAscii },
	{ "USER32", "MapVirtualKeyA", 2, 0x00000000u, &k_MapVirtualKeyA },
	{ "USER32", "GetAsyncKeyState", 1, 0x00000000u, &k_GetAsyncKeyState },
	{ "USER32", "GetSystemMetrics", 1, 0x00000000u, &k_GetSystemMetrics },
	{ "USER32", "SendDlgItemMessageA", 5, 0x00000000u, &k_SendDlgItemMessageA },
	{ "USER32", "GetWindowTextA", 3, 0x00000000u, &k_GetWindowTextA },
	{ "USER32", "MoveWindow", 6, 0x00000000u, &k_MoveWindow },
	{ "USER32", "AdjustWindowRectEx", 4, 0x00000000u, &k_AdjustWindowRectEx },
	{ "USER32", "GetMenu", 1, 0x00000000u, &k_GetMenu },
	{ "USER32", "SetRect", 5, 0x00000000u, &k_SetRect },
	{ "USER32", "SetWindowPos", 7, 0x00000000u, &k_SetWindowPos },
	{ "USER32", "PostMessageA", 4, 0x00000000u, &k_PostMessageA },
	{ "USER32", "ValidateRect", 2, 0x00000000u, &k_ValidateRect },
	{ "USER32", "SetWindowLongA", 3, 0x00000000u, &k_SetWindowLongA },
	{ "USER32", "CharToOemBuffA", 3, 0x00000000u, &k_CharToOemBuffA },
	{ "USER32", "ScreenToClient", 2, 0x00000000u, &k_ScreenToClient },
	{ "USER32", "GetWindowRect", 2, 0x00000000u, &k_GetWindowRect },
	{ "USER32", "DispatchMessageA", 1, 0x00000000u, &k_DispatchMessageA },
	{ "USER32", "TranslateMessage", 1, 0x00000000u, &k_TranslateMessage },
	{ "USER32", "GetDlgItemTextA", 4, 0x00000000u, &k_GetDlgItemTextA },
	{ "USER32", "WaitForInputIdle", 2, 0x00000000u, &k_WaitForInputIdle },
	{ "USER32", "GetTopWindow", 0, 0x00000000u, &k_GetTopWindow },
	{ "USER32", "GetForegroundWindow", 0, 0x00000000u, &k_GetForegroundWindow },
	{ "USER32", "LoadIconA", 2, 0x00000000u, &k_LoadIconA },
	{ "USER32", "SetActiveWindow", 1, 0x00000000u, &k_SetActiveWindow },
	{ "USER32", "RedrawWindow", 5, 0x00000000u, &k_RedrawWindow },
	{ "USER32", "GetWindowContextHelpId", 1, 0x00000000u, &k_GetWindowContextHelpId },
	{ "USER32", "WinHelpA", 4, 0x00000000u, &k_WinHelpA },
	{ "USER32", "ChildWindowFromPoint", 3, 0x00000000u, &k_ChildWindowFromPoint },
	{ "USER32", "LoadCursorA", 2, 0x00000000u, &k_LoadCursorA },
	{ "USER32", "SetCursor", 1, 0x00000000u, &k_SetCursor },
	{ "USER32", "CreateDialogParamA", 5, 0x00000000u, &k_CreateDialogParamA },
	{ "USER32", "PostQuitMessage", 1, 0x00000000u, &k_PostQuitMessage },
	{ "USER32", "FindWindowA", 2, 0x00000000u, &k_FindWindowA },
	{ "USER32", "GetDlgCtrlID", 1, 0x00000000u, &k_GetDlgCtrlID },
	{ "USER32", "GetCapture", 0, 0x00000000u, &k_GetCapture },
	{ "USER32", "SetCursorPos", 2, 0x00000000u, &k_SetCursorPos },
	{ "USER32", "SetFocus", 1, 0x00000000u, &k_SetFocus },
	{ "USER32", "ShowWindow", 2, 0x00000000u, &k_ShowWindow },
	{ "USER32", "EndDialog", 2, 0x00000000u, &k_EndDialog },
	{ "USER32", "GetActiveWindow", 0, 0x00000000u, &k_GetActiveWindow },
	{ "USER32", "SetForegroundWindow", 1, 0x00000000u, &k_SetForegroundWindow },
	{ "USER32", "CreateDialogIndirectParamA", 5, 0x00000000u, &k_CreateDialogIndirectParamA },
	{ "USER32", "SetDlgItemTextA", 3, 0x00000000u, &k_SetDlgItemTextA },
	{ "USER32", "BringWindowToTop", 1, 0x00000000u, &k_BringWindowToTop },
	{ "USER32", "DefWindowProcA", 4, 0x00000000u, &k_DefWindowProcA },
	{ "USER32", "CloseWindow", 1, 0x00000000u, &k_CloseWindow },
	{ "USER32", "GetDC", 1, 0x00000000u, &k_GetDC },
	{ "USER32", "ReleaseDC", 2, 0x00000000u, &k_ReleaseDC },
	{ "USER32", "MessageBoxA", 4, 0x00000000u, &k_MessageBoxA },
	{ "USER32", "LoadStringA", 4, 0x00000000u, &k_LoadStringA },
	{ "USER32", "MessageBoxIndirectA", 1, 0x00000000u, &k_MessageBoxIndirectA },
	{ "USER32", "wsprintfA", 0, 0x00000000u, &k_wsprintfA },
	{ "USER32", "CreateWindowExA", 12, 0x00000000u, &k_CreateWindowExA },
	{ "USER32", "CallWindowProcA", 5, 0x00000000u, &k_CallWindowProcA },
	{ "USER32", "GetNextDlgTabItem", 3, 0x00000000u, &k_GetNextDlgTabItem },
	{ "USER32", "GetUpdateRect", 3, 0x00000000u, &k_GetUpdateRect },
	{ "USER32", "IntersectRect", 3, 0x00000000u, &k_IntersectRect },
	{ "USER32", "SetTimer", 4, 0x00000000u, &k_SetTimer },
	{ "USER32", "GetCursorPos", 1, 0x00000000u, &k_GetCursorPos },
	{ "USER32", "EnumChildWindows", 3, 0x00000000u, &k_EnumChildWindows },
	{ "USER32", "GetWindow", 2, 0x00000000u, &k_GetWindow },
	{ "USER32", "GetClassNameA", 3, 0x00000000u, &k_GetClassNameA },
	{ "USER32", "SetCapture", 1, 0x00000000u, &k_SetCapture },
	{ "USER32", "GetParent", 1, 0x00000000u, &k_GetParent },
	{ "USER32", "ReleaseCapture", 0, 0x00000000u, &k_ReleaseCapture },
	{ "USER32", "PeekMessageA", 5, 0x00000000u, &k_PeekMessageA },
	{ "USER32", "RegisterClassA", 1, 0x00000000u, &k_RegisterClassA },
	{ "USER32", "IsWindowEnabled", 1, 0x00000000u, &k_IsWindowEnabled },
	{ "USER32", "TranslateAccelerator", 3, 0x00000000u, &k_TranslateAccelerator },
	{ "USER32", "GetMessageA", 4, 0x00000000u, &k_GetMessageA },
	{ "USER32", "IsDialogMessage", 2, 0x00000000u, &k_IsDialogMessage },
	{ "USER32", "KillTimer", 2, 0x00000000u, &k_KillTimer },
	{ "USER32", "WindowFromPoint", 2, 0x00000000u, &k_WindowFromPoint },
	{ "USER32", "RegisterHotKey", 4, 0x00000000u, &k_RegisterHotKey },
	{ "GDI32", "DeleteObject", 1, 0x00000000u, &k_DeleteObject },
	{ "GDI32", "CreateSolidBrush", 1, 0x00000000u, &k_CreateSolidBrush },
	{ "GDI32", "SetTextColor", 2, 0x00000000u, &k_SetTextColor },
	{ "GDI32", "SetBkColor", 2, 0x00000000u, &k_SetBkColor },
	{ "GDI32", "SetBkMode", 2, 0x00000000u, &k_SetBkMode },
	{ "GDI32", "GetTextColor", 1, 0x00000000u, &k_GetTextColor },
	{ "GDI32", "GetBkColor", 1, 0x00000000u, &k_GetBkColor },
	{ "GDI32", "GetBkMode", 1, 0x00000000u, &k_GetBkMode },
	{ "GDI32", "SelectObject", 2, 0x00000000u, &k_SelectObject },
	{ "GDI32", "GetStockObject", 1, 0x00000000u, &k_GetStockObject },
	{ "GDI32", "GetTextMetricsA", 2, 0x00000000u, &k_GetTextMetricsA },
	{ "GDI32", "TextOutA", 5, 0x00000000u, &k_TextOutA },
	{ "GDI32", "SetTextAlign", 2, 0x00000000u, &k_SetTextAlign },
	{ "GDI32", "CreateFontA", 14, 0x00000000u, &k_CreateFontA },
	{ "GDI32", "RestoreDC", 2, 0x00000000u, &k_RestoreDC },
	{ "GDI32", "CreateFontIndirectA", 1, 0x00000000u, &k_CreateFontIndirectA },
	{ "GDI32", "DPtoLP", 3, 0x00000000u, &k_DPtoLP },
	{ "GDI32", "SetWindowOrgEx", 4, 0x00000000u, &k_SetWindowOrgEx },
	{ "GDI32", "SetViewportOrgEx", 4, 0x00000000u, &k_SetViewportOrgEx },
	{ "GDI32", "ModifyWorldTransform", 3, 0x00000000u, &k_ModifyWorldTransform },
	{ "GDI32", "SetGraphicsMode", 2, 0x00000000u, &k_SetGraphicsMode },
	{ "GDI32", "SaveDC", 1, 0x00000000u, &k_SaveDC },
	{ "GDI32", "GetTextExtentPoint32A", 4, 0x00000000u, &k_GetTextExtentPoint32A },
	{ "GDI32", "CreateDIBSection", 6, 0x00000000u, &k_CreateDIBSection },
	{ "ADVAPI32", "RegCloseKey", 1, 0x00000000u, &k_RegCloseKey },
	{ "ADVAPI32", "RegQueryValueExA", 6, 0x00000002u, &k_RegQueryValueExA },
	{ "ADVAPI32", "RegEnumKeyExA", 8, 0x00000000u, &k_RegEnumKeyExA },
	{ "ADVAPI32", "RegOpenKeyExA", 5, 0x00000000u, &k_RegOpenKeyExA },
	{ "ADVAPI32", "RegSetValueExA", 6, 0x00000000u, &k_RegSetValueExA },
	{ "ADVAPI32", "RegCreateKeyExA", 9, 0x00000000u, &k_RegCreateKeyExA },
	{ "ADVAPI32", "RegDeleteKeyA", 2, 0x00000000u, &k_RegDeleteKeyA },
	{ "ADVAPI32", "RegQueryInfoKeyA", 12, 0x00000000u, &k_RegQueryInfoKeyA },
	{ "SHELL32", "FindExecutableA", 3, 0x00000000u, &k_FindExecutableA },
	{ "ole32", "StgCreateDocfile", 4, 0x00000000u, &k_StgCreateDocfile },
	{ "ole32", "CoRevokeClassObject", 1, 0x00000000u, &k_CoRevokeClassObject },
	{ "ole32", "OleInitialize", 1, 0x00000000u, &k_OleInitialize },
	{ "ole32", "CoRegisterClassObject", 5, 0x00000000u, &k_CoRegisterClassObject },
	{ "ole32", "OleUninitialize", 0, 0x00000000u, &k_OleUninitialize },
	{ "ole32", "CoDisconnectObject", 2, 0x00000000u, &k_CoDisconnectObject },
	{ "ole32", "StringFromGUID2", 3, 0x00000000u, &k_StringFromGUID2 },
	{ "ole32", "StgOpenStorage", 6, 0x00000000u, &k_StgOpenStorage },
	{ "ole32", "CoFileTimeNow", 1, 0x00000000u, &k_CoFileTimeNow },
	{ "ole32", "StringFromCLSID", 2, 0x00000000u, &k_StringFromCLSID },
	{ "ole32", "CLSIDFromString", 2, 0x00000000u, &k_CLSIDFromString },
	{ "ole32", "OleSaveToStream", 2, 0x00000000u, &k_OleSaveToStream },
	{ "ole32", "OleLoadFromStream", 3, 0x00000000u, &k_OleLoadFromStream },
	{ "ole32", "CoCreateInstance", 5, 0x00000000u, &k_CoCreateInstance },
	{ "ole32", "OleRun", 1, 0x00000000u, &k_OleRun },
	{ "OLEAUT32", "#2", 1, 0x00000000u, &k_oleaut_ord_2147483650 },
	{ "OLEAUT32", "#161", 2, 0x00000000u, &k_oleaut_ord_2147483809 },
	{ "OLEAUT32", "#6", 1, 0x00000000u, 0 },
	{ "OLEAUT32", "#201", 2, 0x00000000u, 0 },
	{ "OLEAUT32", "#12", 4, 0x00000000u, 0 },
	{ "OLEAUT32", "#202", 1, 0x00000000u, 0 },
	{ "OLEAUT32", "#9", 1, 0x00000000u, 0 },
	{ "OLEAUT32", "#8", 1, 0x00000000u, &k_oleaut_ord_2147483656 },
	{ "OLEAUT32", "#34", 2, 0x00000000u, 0 },
	{ "OLEAUT32", "#33", 4, 0x00000000u, 0 },
	{ "OLEAUT32", "#200", 2, 0x00000000u, 0 },
	{ "WINMM", "timeBeginPeriod", 1, 0x00000000u, &k_timeBeginPeriod },
	{ "WINMM", "timeSetEvent", 5, 0x00000000u, &k_timeSetEvent },
	{ "WINMM", "timeKillEvent", 1, 0x00000000u, &k_timeKillEvent },
	{ "WINMM", "timeGetTime", 0, 0x00000000u, &k_timeGetTime },
	{ "WINMM", "timeGetDevCaps", 2, 0x00000000u, &k_timeGetDevCaps },
	{ "WINMM", "timeEndPeriod", 1, 0x00000000u, &k_timeEndPeriod },
	{ "WSOCK32", "#17", 2, 0x00000000u, 0 },
	{ "WSOCK32", "#21", 1, 0x00000000u, 0 },
	{ "WSOCK32", "#20", 2, 0x00000000u, 0 },
	{ "WSOCK32", "#116", 0, 0x00000000u, 0 },
	{ "WSOCK32", "#101", 4, 0x00000000u, 0 },
	{ "WSOCK32", "#108", 0, 0x00000000u, 0 },
	{ "WSOCK32", "#7", 5, 0x00000000u, 0 },
	{ "WSOCK32", "#111", 0, 0x00000000u, 0 },
	{ "WSOCK32", "#11", 1, 0x00000000u, 0 },
	{ "WSOCK32", "#15", 1, 0x00000000u, 0 },
	{ "WSOCK32", "#14", 1, 0x00000000u, 0 },
	{ "WSOCK32", "#9", 1, 0x00000000u, 0 },
	{ "WSOCK32", "#115", 2, 0x00000000u, &k_wsock_ord_2147483763 },
	{ "WSOCK32", "#57", 5, 0x00000000u, 0 },
	{ "WSOCK32", "#52", 2, 0x00000000u, 0 },
	{ "WSOCK32", "#3", 1, 0x00000000u, 0 },
	{ "WSOCK32", "#23", 4, 0x00000000u, 0 },
	{ "WSOCK32", "#2", 3, 0x00000000u, 0 },
	{ "WSOCK32", "#8", 1, 0x00000000u, 0 },
	{ "DDRAW", "DirectDrawCreate", 3, 0x00000000u, &k_DirectDrawCreate },
	{ "DSOUND", "#1", 3, 0x00000000u, &k_dsound_ord_2147483649 },
	{ "VERSION", "GetFileVersionInfoSizeA", 2, 0x00000000u, &k_GetFileVersionInfoSizeA },
	{ "VERSION", "GetFileVersionInfoA", 4, 0x00000000u, &k_GetFileVersionInfoA },
	{ "VERSION", "VerQueryValueA", 4, 0x00000000u, &k_VerQueryValueA },
	{ "COMCTL32", "ImageList_DragShowNolock", 1, 0x00000001u, &k_ImageList_DragShowNolock },
	{ "COMCTL32", "#17", 0, 0x00000000u, &k_comctl32_ord_2147483665 },
	{ "COMCTL32", "ImageList_DragMove", 2, 0x00000001u, &k_ImageList_DragMove },
	{ "COMCTL32", "ImageList_DragEnter", 3, 0x00000001u, &k_ImageList_DragEnter },
	{ "COMCTL32", "ImageList_BeginDrag", 3, 0x00000001u, &k_ImageList_BeginDrag },
	{ "COMCTL32", "ImageList_Destroy", 1, 0x00000001u, &k_ImageList_Destroy },
	{ "COMCTL32", "ImageList_EndDrag", 0, 0x00000000u, &k_ImageList_EndDrag },
	{ "COMCTL32", "DllGetVersion", 1, 0x00000000u, &k_DllGetVersion },
    };
    *count = (unsigned)(sizeof(t) / sizeof(t[0]));
    return t;
}

inline unsigned long &count_of(const char *fn) {
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    for (unsigned i = 0; i < n; ++i)
        if (!std::strcmp(t[i].fn, fn)) return counts()[i];
    return counts()[n];   // everything unnamed shares the last bucket
}

inline bool same_dll(const char *a, const char *b);

// The definition of the forward declaration in the head: a stub body asks the
// table how many argument words it owns instead of repeating a count that only
// one place can check.
inline void stub_done_for(const char *dll, const char *fn, u32 value) {
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    for (unsigned i = 0; i < n; ++i)
        if (!std::strcmp(t[i].fn, fn) && same_dll(t[i].dll, dll)) {
            stub_done(t[i].nargs, value);
            return;
        }
    stub_done(0u, value);
}

inline u32 *stub_vas() { static u32 v[512]; return v; }

inline bool same_dll(const char *a, const char *b) {
    // "KERNEL32.dll" and "KERNEL32" are the same module. Case-insensitive,
    // because a loader may well have lowercased the name.
    for (;;) {
        char ca = *a, cb = *b;
        if (ca == '.' || !ca) ca = 0;
        if (cb == '.' || !cb) cb = 0;
        if (ca >= 'A' && ca <= 'Z') ca = (char)(ca - 'A' + 'a');
        if (cb >= 'A' && cb <= 'Z') cb = (char)(cb - 'A' + 'a');
        if (ca != cb) return false;
        if (!ca) return true;
        ++a; ++b;
    }
}

inline u32 find_by_name(const char *fn) {
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    for (unsigned i = 0; i < n; ++i)
        if (!std::strcmp(t[i].fn, fn)) return stub_vas()[i];
    return 0;
}

// A named generic stub. It knows its own row, so the call is named, counted,
// and pops exactly the arguments the real function owns. It is correct about
// the stack and about the return value -- and it is not silent: the FIRST call
// to each unimplemented import is printed, because "the program asked for
// something nothing implements" is the most useful line a boot log can carry,
// and printing every call would bury it under a frame loop.
inline bool *idx_logged() { static bool b[512]; return b; }

template<unsigned I> void idx_stub() {
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    (void)n;
    note(t[I].fn);
    if (!idx_logged()[I]) {
        idx_logged()[I] = true;
        std::printf("win32stubs: UNIMPLEMENTED %s!%s -> generic stub, "
                    "returns 0x%08x\n", t[I].dll, t[I].fn, (unsigned)t[I].ret);
    }
    stub_done(t[I].nargs, t[I].ret);
}

inline Fn idx_stub_fn(unsigned i) {
    static Fn f[342] = {
        &idx_stub<0>, &idx_stub<1>, &idx_stub<2>, &idx_stub<3>, &idx_stub<4>, &idx_stub<5>,
        &idx_stub<6>, &idx_stub<7>, &idx_stub<8>, &idx_stub<9>, &idx_stub<10>, &idx_stub<11>,
        &idx_stub<12>, &idx_stub<13>, &idx_stub<14>, &idx_stub<15>, &idx_stub<16>, &idx_stub<17>,
        &idx_stub<18>, &idx_stub<19>, &idx_stub<20>, &idx_stub<21>, &idx_stub<22>, &idx_stub<23>,
        &idx_stub<24>, &idx_stub<25>, &idx_stub<26>, &idx_stub<27>, &idx_stub<28>, &idx_stub<29>,
        &idx_stub<30>, &idx_stub<31>, &idx_stub<32>, &idx_stub<33>, &idx_stub<34>, &idx_stub<35>,
        &idx_stub<36>, &idx_stub<37>, &idx_stub<38>, &idx_stub<39>, &idx_stub<40>, &idx_stub<41>,
        &idx_stub<42>, &idx_stub<43>, &idx_stub<44>, &idx_stub<45>, &idx_stub<46>, &idx_stub<47>,
        &idx_stub<48>, &idx_stub<49>, &idx_stub<50>, &idx_stub<51>, &idx_stub<52>, &idx_stub<53>,
        &idx_stub<54>, &idx_stub<55>, &idx_stub<56>, &idx_stub<57>, &idx_stub<58>, &idx_stub<59>,
        &idx_stub<60>, &idx_stub<61>, &idx_stub<62>, &idx_stub<63>, &idx_stub<64>, &idx_stub<65>,
        &idx_stub<66>, &idx_stub<67>, &idx_stub<68>, &idx_stub<69>, &idx_stub<70>, &idx_stub<71>,
        &idx_stub<72>, &idx_stub<73>, &idx_stub<74>, &idx_stub<75>, &idx_stub<76>, &idx_stub<77>,
        &idx_stub<78>, &idx_stub<79>, &idx_stub<80>, &idx_stub<81>, &idx_stub<82>, &idx_stub<83>,
        &idx_stub<84>, &idx_stub<85>, &idx_stub<86>, &idx_stub<87>, &idx_stub<88>, &idx_stub<89>,
        &idx_stub<90>, &idx_stub<91>, &idx_stub<92>, &idx_stub<93>, &idx_stub<94>, &idx_stub<95>,
        &idx_stub<96>, &idx_stub<97>, &idx_stub<98>, &idx_stub<99>, &idx_stub<100>, &idx_stub<101>,
        &idx_stub<102>, &idx_stub<103>, &idx_stub<104>, &idx_stub<105>, &idx_stub<106>, &idx_stub<107>,
        &idx_stub<108>, &idx_stub<109>, &idx_stub<110>, &idx_stub<111>, &idx_stub<112>, &idx_stub<113>,
        &idx_stub<114>, &idx_stub<115>, &idx_stub<116>, &idx_stub<117>, &idx_stub<118>, &idx_stub<119>,
        &idx_stub<120>, &idx_stub<121>, &idx_stub<122>, &idx_stub<123>, &idx_stub<124>, &idx_stub<125>,
        &idx_stub<126>, &idx_stub<127>, &idx_stub<128>, &idx_stub<129>, &idx_stub<130>, &idx_stub<131>,
        &idx_stub<132>, &idx_stub<133>, &idx_stub<134>, &idx_stub<135>, &idx_stub<136>, &idx_stub<137>,
        &idx_stub<138>, &idx_stub<139>, &idx_stub<140>, &idx_stub<141>, &idx_stub<142>, &idx_stub<143>,
        &idx_stub<144>, &idx_stub<145>, &idx_stub<146>, &idx_stub<147>, &idx_stub<148>, &idx_stub<149>,
        &idx_stub<150>, &idx_stub<151>, &idx_stub<152>, &idx_stub<153>, &idx_stub<154>, &idx_stub<155>,
        &idx_stub<156>, &idx_stub<157>, &idx_stub<158>, &idx_stub<159>, &idx_stub<160>, &idx_stub<161>,
        &idx_stub<162>, &idx_stub<163>, &idx_stub<164>, &idx_stub<165>, &idx_stub<166>, &idx_stub<167>,
        &idx_stub<168>, &idx_stub<169>, &idx_stub<170>, &idx_stub<171>, &idx_stub<172>, &idx_stub<173>,
        &idx_stub<174>, &idx_stub<175>, &idx_stub<176>, &idx_stub<177>, &idx_stub<178>, &idx_stub<179>,
        &idx_stub<180>, &idx_stub<181>, &idx_stub<182>, &idx_stub<183>, &idx_stub<184>, &idx_stub<185>,
        &idx_stub<186>, &idx_stub<187>, &idx_stub<188>, &idx_stub<189>, &idx_stub<190>, &idx_stub<191>,
        &idx_stub<192>, &idx_stub<193>, &idx_stub<194>, &idx_stub<195>, &idx_stub<196>, &idx_stub<197>,
        &idx_stub<198>, &idx_stub<199>, &idx_stub<200>, &idx_stub<201>, &idx_stub<202>, &idx_stub<203>,
        &idx_stub<204>, &idx_stub<205>, &idx_stub<206>, &idx_stub<207>, &idx_stub<208>, &idx_stub<209>,
        &idx_stub<210>, &idx_stub<211>, &idx_stub<212>, &idx_stub<213>, &idx_stub<214>, &idx_stub<215>,
        &idx_stub<216>, &idx_stub<217>, &idx_stub<218>, &idx_stub<219>, &idx_stub<220>, &idx_stub<221>,
        &idx_stub<222>, &idx_stub<223>, &idx_stub<224>, &idx_stub<225>, &idx_stub<226>, &idx_stub<227>,
        &idx_stub<228>, &idx_stub<229>, &idx_stub<230>, &idx_stub<231>, &idx_stub<232>, &idx_stub<233>,
        &idx_stub<234>, &idx_stub<235>, &idx_stub<236>, &idx_stub<237>, &idx_stub<238>, &idx_stub<239>,
        &idx_stub<240>, &idx_stub<241>, &idx_stub<242>, &idx_stub<243>, &idx_stub<244>, &idx_stub<245>,
        &idx_stub<246>, &idx_stub<247>, &idx_stub<248>, &idx_stub<249>, &idx_stub<250>, &idx_stub<251>,
        &idx_stub<252>, &idx_stub<253>, &idx_stub<254>, &idx_stub<255>, &idx_stub<256>, &idx_stub<257>,
        &idx_stub<258>, &idx_stub<259>, &idx_stub<260>, &idx_stub<261>, &idx_stub<262>, &idx_stub<263>,
        &idx_stub<264>, &idx_stub<265>, &idx_stub<266>, &idx_stub<267>, &idx_stub<268>, &idx_stub<269>,
        &idx_stub<270>, &idx_stub<271>, &idx_stub<272>, &idx_stub<273>, &idx_stub<274>, &idx_stub<275>,
        &idx_stub<276>, &idx_stub<277>, &idx_stub<278>, &idx_stub<279>, &idx_stub<280>, &idx_stub<281>,
        &idx_stub<282>, &idx_stub<283>, &idx_stub<284>, &idx_stub<285>, &idx_stub<286>, &idx_stub<287>,
        &idx_stub<288>, &idx_stub<289>, &idx_stub<290>, &idx_stub<291>, &idx_stub<292>, &idx_stub<293>,
        &idx_stub<294>, &idx_stub<295>, &idx_stub<296>, &idx_stub<297>, &idx_stub<298>, &idx_stub<299>,
        &idx_stub<300>, &idx_stub<301>, &idx_stub<302>, &idx_stub<303>, &idx_stub<304>, &idx_stub<305>,
        &idx_stub<306>, &idx_stub<307>, &idx_stub<308>, &idx_stub<309>, &idx_stub<310>, &idx_stub<311>,
        &idx_stub<312>, &idx_stub<313>, &idx_stub<314>, &idx_stub<315>, &idx_stub<316>, &idx_stub<317>,
        &idx_stub<318>, &idx_stub<319>, &idx_stub<320>, &idx_stub<321>, &idx_stub<322>, &idx_stub<323>,
        &idx_stub<324>, &idx_stub<325>, &idx_stub<326>, &idx_stub<327>, &idx_stub<328>, &idx_stub<329>,
        &idx_stub<330>, &idx_stub<331>, &idx_stub<332>, &idx_stub<333>, &idx_stub<334>, &idx_stub<335>,
        &idx_stub<336>, &idx_stub<337>, &idx_stub<338>, &idx_stub<339>, &idx_stub<340>, &idx_stub<341>,
    };
    return i < 342u ? f[i] : 0;
}


// ---------------------------------------------------------------------------
// fallback
// ---------------------------------------------------------------------------
// An import this table does not know still has to be CALLABLE: writing 0 into
// its IAT slot would put the run back where it started, with every call
// landing on address 0. fallback_stub_va() therefore mints a fresh registered
// stub per unresolved import, named after the import so the miss is
// attributable, and returning 0 in EAX so it is visible. Each fallback gets its
// own VA, which is why the body is a template over a slot index rather than one
// shared function.
//
// TWO THINGS A FALLBACK MUST NOT DO.
//
// It must not be quiet. It says so when it is minted -- not only when it is
// called, because an import that is resolved and never called is still a hole
// in the model and the resolution is the moment it is attributable.
//
// It must not silently leave the modelled stack imbalanced. A __stdcall callee
// owns its argument words; a callee that pops none shifts every frame above it
// by four bytes per argument, and the caller then reads its own locals off by
// that much. So a fallback looks its own name up in the import table before it
// gives up: a spelling mismatch (the loader says "#161" where the table says
// "ord_2147483681") then costs nothing, because the count comes from the row.
// Only an import the table genuinely does not have ends up with an unknown
// count, and that case is counted and printed as what it is.
//
// The name comes from the last stub_va() call, because that is the only thing
// the shared interface carries. A caller that asks stub_va(dll, fn) and then
// fallback_stub_va() gets a fallback that names dll!fn.

struct FallbackDef { char name[80]; u8 nargs; bool known_arity; };
inline FallbackDef *fallback_defs() { static FallbackDef d[256]; return d; }
inline char *pending_query() { static char b[80]; return b; }

inline void remember_query(const char *dll, const char *fn) {
    char *b = pending_query();
    unsigned i = 0;
    if (dll) for (unsigned j = 0; dll[j] && i < 60u; ++j) b[i++] = dll[j];
    b[i++] = '!';
    if (fn) for (unsigned j = 0; fn[j] && i < 78u; ++j) b[i++] = fn[j];
    b[i] = 0;
}

template<unsigned I> void fallback_at() {
    const FallbackDef &d = fallback_defs()[I];
    state().fallbacks++;
    note(d.name[0] ? d.name : "(unnamed import)");
    if (d.known_arity) {
        // Loud, and correct about the stack: the row the name resolved to says
        // how many words the caller pushed.
        std::printf("win32stubs: NO STUB for import \"%s\" -- the fallback pops "
                    "%u argument word(s) from the import table and returns 0\n",
                    d.name[0] ? d.name : "(unnamed)", (unsigned)d.nargs);
    } else {
        state().unbalanced++;
        std::printf("win32stubs: NO STUB for import \"%s\" -- ARGUMENT COUNT "
                    "UNKNOWN, so the fallback cannot pop its arguments and ESP "
                    "drifts by 4 bytes per argument from here on\n",
                    d.name[0] ? d.name : "(unnamed)");
    }
    stub_done(d.nargs, 0u);   // 0 in EAX: unimplemented, and it says so
}

inline Fn fallback_fn(unsigned i) {
    static Fn f[256] = {
        &fallback_at<0>, &fallback_at<1>, &fallback_at<2>, &fallback_at<3>, &fallback_at<4>, &fallback_at<5>, &fallback_at<6>, &fallback_at<7>,
        &fallback_at<8>, &fallback_at<9>, &fallback_at<10>, &fallback_at<11>, &fallback_at<12>, &fallback_at<13>, &fallback_at<14>, &fallback_at<15>,
        &fallback_at<16>, &fallback_at<17>, &fallback_at<18>, &fallback_at<19>, &fallback_at<20>, &fallback_at<21>, &fallback_at<22>, &fallback_at<23>,
        &fallback_at<24>, &fallback_at<25>, &fallback_at<26>, &fallback_at<27>, &fallback_at<28>, &fallback_at<29>, &fallback_at<30>, &fallback_at<31>,
        &fallback_at<32>, &fallback_at<33>, &fallback_at<34>, &fallback_at<35>, &fallback_at<36>, &fallback_at<37>, &fallback_at<38>, &fallback_at<39>,
        &fallback_at<40>, &fallback_at<41>, &fallback_at<42>, &fallback_at<43>, &fallback_at<44>, &fallback_at<45>, &fallback_at<46>, &fallback_at<47>,
        &fallback_at<48>, &fallback_at<49>, &fallback_at<50>, &fallback_at<51>, &fallback_at<52>, &fallback_at<53>, &fallback_at<54>, &fallback_at<55>,
        &fallback_at<56>, &fallback_at<57>, &fallback_at<58>, &fallback_at<59>, &fallback_at<60>, &fallback_at<61>, &fallback_at<62>, &fallback_at<63>,
        &fallback_at<64>, &fallback_at<65>, &fallback_at<66>, &fallback_at<67>, &fallback_at<68>, &fallback_at<69>, &fallback_at<70>, &fallback_at<71>,
        &fallback_at<72>, &fallback_at<73>, &fallback_at<74>, &fallback_at<75>, &fallback_at<76>, &fallback_at<77>, &fallback_at<78>, &fallback_at<79>,
        &fallback_at<80>, &fallback_at<81>, &fallback_at<82>, &fallback_at<83>, &fallback_at<84>, &fallback_at<85>, &fallback_at<86>, &fallback_at<87>,
        &fallback_at<88>, &fallback_at<89>, &fallback_at<90>, &fallback_at<91>, &fallback_at<92>, &fallback_at<93>, &fallback_at<94>, &fallback_at<95>,
        &fallback_at<96>, &fallback_at<97>, &fallback_at<98>, &fallback_at<99>, &fallback_at<100>, &fallback_at<101>, &fallback_at<102>, &fallback_at<103>,
        &fallback_at<104>, &fallback_at<105>, &fallback_at<106>, &fallback_at<107>, &fallback_at<108>, &fallback_at<109>, &fallback_at<110>, &fallback_at<111>,
        &fallback_at<112>, &fallback_at<113>, &fallback_at<114>, &fallback_at<115>, &fallback_at<116>, &fallback_at<117>, &fallback_at<118>, &fallback_at<119>,
        &fallback_at<120>, &fallback_at<121>, &fallback_at<122>, &fallback_at<123>, &fallback_at<124>, &fallback_at<125>, &fallback_at<126>, &fallback_at<127>,
        &fallback_at<128>, &fallback_at<129>, &fallback_at<130>, &fallback_at<131>, &fallback_at<132>, &fallback_at<133>, &fallback_at<134>, &fallback_at<135>,
        &fallback_at<136>, &fallback_at<137>, &fallback_at<138>, &fallback_at<139>, &fallback_at<140>, &fallback_at<141>, &fallback_at<142>, &fallback_at<143>,
        &fallback_at<144>, &fallback_at<145>, &fallback_at<146>, &fallback_at<147>, &fallback_at<148>, &fallback_at<149>, &fallback_at<150>, &fallback_at<151>,
        &fallback_at<152>, &fallback_at<153>, &fallback_at<154>, &fallback_at<155>, &fallback_at<156>, &fallback_at<157>, &fallback_at<158>, &fallback_at<159>,
        &fallback_at<160>, &fallback_at<161>, &fallback_at<162>, &fallback_at<163>, &fallback_at<164>, &fallback_at<165>, &fallback_at<166>, &fallback_at<167>,
        &fallback_at<168>, &fallback_at<169>, &fallback_at<170>, &fallback_at<171>, &fallback_at<172>, &fallback_at<173>, &fallback_at<174>, &fallback_at<175>,
        &fallback_at<176>, &fallback_at<177>, &fallback_at<178>, &fallback_at<179>, &fallback_at<180>, &fallback_at<181>, &fallback_at<182>, &fallback_at<183>,
        &fallback_at<184>, &fallback_at<185>, &fallback_at<186>, &fallback_at<187>, &fallback_at<188>, &fallback_at<189>, &fallback_at<190>, &fallback_at<191>,
        &fallback_at<192>, &fallback_at<193>, &fallback_at<194>, &fallback_at<195>, &fallback_at<196>, &fallback_at<197>, &fallback_at<198>, &fallback_at<199>,
        &fallback_at<200>, &fallback_at<201>, &fallback_at<202>, &fallback_at<203>, &fallback_at<204>, &fallback_at<205>, &fallback_at<206>, &fallback_at<207>,
        &fallback_at<208>, &fallback_at<209>, &fallback_at<210>, &fallback_at<211>, &fallback_at<212>, &fallback_at<213>, &fallback_at<214>, &fallback_at<215>,
        &fallback_at<216>, &fallback_at<217>, &fallback_at<218>, &fallback_at<219>, &fallback_at<220>, &fallback_at<221>, &fallback_at<222>, &fallback_at<223>,
        &fallback_at<224>, &fallback_at<225>, &fallback_at<226>, &fallback_at<227>, &fallback_at<228>, &fallback_at<229>, &fallback_at<230>, &fallback_at<231>,
        &fallback_at<232>, &fallback_at<233>, &fallback_at<234>, &fallback_at<235>, &fallback_at<236>, &fallback_at<237>, &fallback_at<238>, &fallback_at<239>,
        &fallback_at<240>, &fallback_at<241>, &fallback_at<242>, &fallback_at<243>, &fallback_at<244>, &fallback_at<245>, &fallback_at<246>, &fallback_at<247>,
        &fallback_at<248>, &fallback_at<249>, &fallback_at<250>, &fallback_at<251>, &fallback_at<252>, &fallback_at<253>, &fallback_at<254>, &fallback_at<255>,
    };
    return f[i];
}


inline unsigned &fallback_slot() { static unsigned s = 0; return s; }

// "KERNEL32!CloseHandle" -> the row it names, or -1. One matching rule for
// stub_va() and the fallback, so the two cannot disagree about what a name
// means.
inline int find_row(const char *query) {
    char dll[64], fn[80];
    unsigned i = 0, j = 0;
    for (; query[i] && query[i] != '!' && i + 1u < sizeof(dll); ++i) dll[i] = query[i];
    dll[i] = 0;
    if (query[i] == '!') ++i;
    for (; query[i] && j + 1u < sizeof(fn); ++i) fn[j++] = query[i];
    fn[j] = 0;
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    for (unsigned k = 0; k < n; ++k)
        if (!std::strcmp(t[k].fn, fn) && same_dll(t[k].dll, dll)) return (int)k;
    return -1;
}

inline u32 fallback_impl() {
    unsigned i = fallback_slot()++ % 256u;
    FallbackDef &d = fallback_defs()[i];
    std::strncpy(d.name, pending_query(), sizeof(d.name) - 1);
    d.name[sizeof(d.name) - 1] = 0;
    // Ask the table how many words this import owns. A name the table has no
    // row for leaves the count unknown, and that is stated rather than guessed.
    int row = find_row(pending_query());
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    d.known_arity = row >= 0;
    d.nargs = row >= 0 ? t[row].nargs : 0u;
    state().fallback_names++;
    if (row >= 0) {
        std::printf("win32stubs: FALLBACK %s -- no stub of its own; the table's "
                    "%u argument word(s) are popped, EAX is 0\n",
                    d.name, (unsigned)d.nargs);
    } else {
        state().unbalanced_at_resolution++;
        std::printf("win32stubs: FALLBACK %s -- NOT IN THE IMPORT TABLE: its "
                    "argument count is unknown, so the modelled stack cannot be "
                    "kept balanced if it is called\n", d.name);
    }
    u32 va = alloc_stub_va();
    ts_register(va, fallback_fn(i));
    return va;
}

// ---------------------------------------------------------------------------
// registration
// ---------------------------------------------------------------------------
inline bool &registered() { static bool r = false; return r; }

inline void register_impl() {
    if (registered()) return;
    registered() = true;
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    unsigned generic = 0;
    for (unsigned i = 0; i < n; ++i) {
        u32 va = alloc_stub_va();
        stub_vas()[i] = va;
        ts_register(va, t[i].real ? t[i].real : idx_stub_fn(i));
        if (!t[i].real) ++generic;
    }
    // One line, once, at the moment the table is bound: how much of the import
    // layer is a real body and how much is a stub that only keeps the caller
    // alive. Each generic import also names itself on first call; this is the
    // count a reader needs before the first call happens.
    std::printf("win32stubs: %u imports registered: %u with a body, %u generic "
                "(a generic stub pops its table's arguments, returns its row's "
                "value, and says so on first call)\n", n, n - generic, generic);
}

inline u32 stub_va_impl(const char *dll, const char *fn) {
    remember_query(dll, fn);
    int row = find_row(pending_query());
    if (row < 0) return 0;
    state().resolved++;
    return stub_vas()[row];
}

inline unsigned long &count_for_impl(const char *dll, const char *fn) {
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    for (unsigned i = 0; i < n; ++i)
        if (!std::strcmp(t[i].fn, fn) && same_dll(t[i].dll, dll)) return counts()[i];
    return counts()[n];
}

}  // namespace w32

// ===========================================================================
// the shared interface
// ===========================================================================

// Register every stub in the dispatch table. Idempotent: the loader may call it
// from more than one place.
inline void register_win32_stubs() { w32::register_impl(); }

// The stub VA for a known import, or 0 so the caller can fall back. Records the
// name, because fallback_stub_va() has nowhere else to get it.
inline u32 stub_va(const char *dll, const char *fn) { return w32::stub_va_impl(dll, fn); }

// A callable stub for an import nothing implements: logs the name, returns 0.
inline u32 fallback_stub_va() { return w32::fallback_impl(); }

// Observable counters. `stub_calls` is every stub invocation; `fallbacks` is
// the subset that reached a fallback; `resolved` counts stub_va() hits.
inline unsigned stub_calls() { return (unsigned)w32::state().calls; }
inline unsigned stub_fallbacks() { return (unsigned)w32::state().fallbacks; }
inline unsigned stub_resolved() { return (unsigned)w32::state().resolved; }
inline unsigned stub_fallback_names() { return (unsigned)w32::state().fallback_names; }
inline const char *stub_last_name() { return w32::state().last; }

// Per-name call counts, for evidence rather than for the run.
inline unsigned stub_count_for(const char *dll, const char *fn) {
    return (unsigned)w32::count_for_impl(dll, fn);
}
inline unsigned stub_count_named(const char *fn) {
    return (unsigned)w32::count_of(fn);
}

// Print each stub call as it happens. Off by default: a full entry-point run
// makes thousands of them.
inline void stub_verbose(bool on) { w32::state().verbose = on; }

inline void stub_report() {
    unsigned n = 0;
    const w32::ImportDef *t = w32::imports(&n);
    unsigned real = 0;
    for (unsigned i = 0; i < n; ++i) if (t[i].real) ++real;
    unsigned unresolved = w32::fallback_slot();
    std::printf("win32stubs: %u imports; %u with a hand-written body; "
                "%u generic; %u stubs resolved; %u FALLBACKS; %lu calls; "
                "%lu fallback calls; %lu with an UNKNOWN argument count\n",
                n, real, n - real, (unsigned)w32::state().resolved, unresolved,
                w32::state().calls, w32::state().fallbacks,
                w32::state().unbalanced + w32::state().unbalanced_at_resolution);
    for (unsigned i = 0; i < n; ++i) {
        unsigned long c = w32::counts()[i];
        if (c) std::printf("  %8lu  %s!%s\n", c, t[i].dll, t[i].fn);
    }
    // An import that fell back is a hole in the table and the count alone does
    // not say which one, so name every fallback and say whether its stack
    // discipline is known.
    for (unsigned i = 0; i < unresolved; ++i) {
        const w32::FallbackDef &d = w32::fallback_defs()[i];
        std::printf("  FALLBACK  %s -- %s\n", d.name,
                    d.known_arity ? "argument count from the import table"
                                  : "ARGUMENT COUNT UNKNOWN: stack unbalanced");
    }
    // The generic rows by name. They are correct about the stack and about the
    // return value, and they are still unimplemented: a reader asking "what does
    // this model not do yet" should not have to call them all to find out.
    for (unsigned i = 0; i < n; ++i)
        if (!t[i].real)
            std::printf("  GENERIC   %s!%s (%u argument word(s))\n",
                        t[i].dll, t[i].fn, (unsigned)t[i].nargs);
    // COM methods are not imports, so the ones served by a generic slot stub
    // are reported from the slot table: a DirectDraw wrapper's Blt, Flip and
    // Unlock calls land there, and a DirectDraw phase needs to see them by
    // name. A slot with a hand-written body above counts under that body's
    // name in the import table's shared bucket instead.
    unsigned slots = w32::slot_next();
    for (unsigned i = 0; i < slots; ++i) {
        unsigned long c = w32::slot_counts()[i];
        if (c) std::printf("  %8lu  %s (vtable slot %u)\n", c,
                           w32::slot_defs()[i].name ? w32::slot_defs()[i].name : "com", i);
    }
}

}  // namespace ts

#endif  // TS_WIN32STUBS_H
