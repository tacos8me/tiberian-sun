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
inline u32 &bump() { static u32 b = 0x20000000u; return b; }
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
    char last[64];                  // name of the most recent stub called
    bool verbose;                   // print each stub call
    StubState() : calls(0), fallbacks(0), resolved(0), fallback_names(0),
                  verbose(false) { last[0] = 0; }
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
    ESP.d += 4u + nargs * 4u;
    EAX.d = value;
}

#define TS_BEGIN(w) void k_##w() { note(#w);
#define TS_END }
#define TS_RET(w, n, v) TS_BEGIN(w) stub_done(n, v); TS_END

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

// -- clock -------------------------------------------------------------------
// A monotonic millisecond counter. The original reads the real clock; a
// monotonic counter is what every caller actually depends on, and it makes a
// run deterministic, which a real clock would not.
inline u32 &tick() { static u32 t = 1000u; return t; }
inline u32 tick_advance(u32 ms) { tick() += ms; return tick(); }

// Defined with the table: name -> stub VA, ignoring the DLL.
inline u32 find_by_name(const char *fn);

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
    (void)arg(0);
    static u32 n = 0;
    stub_done(1, 0x00500000u + (n++));
TS_END

TS_RET(FreeLibrary, 1, 1u)

TS_BEGIN(GetProcAddress)
    // The returned address must be a registered stub or the caller's first
    // call lands on nothing. An unknown name gets the fallback, which is
    // registered and therefore callable, and which logs the miss.
    u32 va = find_by_name(ld_cstr(arg(1)));
    stub_done(2, va ? va : 0u);
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
    u32 p = alloc_block(size, 8u);
    st32(p, size);
    stub_done(3, p + 16u);
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
    // There is no way to unwind the modelled stack from here, so ExitProcess
    // returns and the frames above it run to their own `ret`. That is wrong
    // about control flow but it never loses the caller.
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
// There is no real file system behind the model. A file "exists" (so callers
// take the success path and stay alive) and reads as empty. That is a lie, but
// it is a consistent one: GetFileSize and SetFilePointer both say zero too, so
// nothing reads past an end that was never written.
inline u32 &file_serial() { static u32 n = 0; return n; }

TS_BEGIN(CreateFileA)
    stub_done(7, 0x00080000u + (file_serial()++));
TS_END

TS_BEGIN(ReadFile)
    u32 got = arg(3);
    if (got) st32(got, 0u);
    stub_done(5, 1u);
TS_END

TS_BEGIN(WriteFile)
    u32 written = arg(3);
    if (written) st32(written, arg(2));
    stub_done(5, 1u);
TS_END

TS_RET(CloseHandle, 1, 1u)
TS_RET(SetEndOfFile, 1, 1u)
TS_RET(FlushFileBuffers, 1, 1u)
TS_RET(FlushInstructionCache, 3, 1u)
TS_RET(DeleteFileA, 1, 1u)
TS_RET(SetFileTime, 4, 1u)
TS_RET(GetFileTime, 4, 1u)
TS_RET(GetFileInformationByHandle, 2, 1u)
TS_RET(GetFileAttributesA, 1, 0x20u)   // FILE_ATTRIBUTE_ARCHIVE

TS_BEGIN(GetFileSize)
    u32 high = arg(1);
    if (high) st32(high, 0u);
    stub_done(2, 0u);
TS_END

TS_BEGIN(SetFilePointer) stub_done(4, 0u); TS_END

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
TS_RET(FindResourceA, 3, 1u)   // a non-null HRSRC; nothing dereferences it

TS_BEGIN(LoadResource)
    static u32 block = 0;
    if (!block) block = zero_block(4096u, 4u);
    stub_done(2, block);
TS_END

TS_BEGIN(LockResource) stub_done(1, arg(0)); TS_END

// -- drives, volumes, filesystem information ---------------------------------
TS_RET(GetDriveTypeA, 1, 3u)   // DRIVE_FIXED

TS_BEGIN(GetDiskFreeSpaceA)
    st32(arg(1), 0x10000u); st32(arg(2), 512u);
    st32(arg(3), 0x10000u); st32(arg(4), 0x40000u);
    stub_done(5, 1u);
TS_END

TS_BEGIN(GetLogicalDriveStringsA)
    u32 buf = arg(1);
    if (buf) { st8(buf, 'C'); st8(buf + 1u, ':'); st8(buf + 2u, '\\'); st8(buf + 3u, 0); st8(buf + 4u, 0); }
    stub_done(2, 4u);
TS_END

TS_BEGIN(GetVolumeInformationA)
    u32 name = arg(1);
    if (name) c_strcpy(name, lit("TSVOL"));
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

enum { MAX_WINDOWS = 32, MAX_CLASSES = 16, MAX_MSGS = 128 };

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
    u32 proc;        // GWL_WNDPROC
    u32 userdata;    // GWL_USERDATA: where a program stores its `this`
    u32 style, exstyle, parent, id, atom, shown;
    i32 x, y, cx, cy;   // window rect, in screen coordinates
    char text[64];
};
inline WinRec *wins() { static WinRec w[MAX_WINDOWS]; return w; }

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
TS_RET(IsWindowEnabled, 1, 1u)
TS_RET(EnableWindow, 2, 1u)

TS_BEGIN(DestroyWindow)
    u32 hwnd = arg(0);
    WinRec *w = win_find(hwnd);
    if (w) {
        w->hwnd = 0u;
        if (wnd_handle() == hwnd) wnd_handle() = 0u;
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
TS_RET(SetForegroundWindow, 1, 1u)
TS_BEGIN(GetForegroundWindow) stub_done(0, wnd_handle()); TS_END
TS_BEGIN(GetActiveWindow)     stub_done(0, wnd_handle()); TS_END
TS_BEGIN(GetFocus)            stub_done(0, wnd_handle()); TS_END
TS_BEGIN(SetFocus)            stub_done(1, wnd_handle()); TS_END
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

TS_RET(EnumChildWindows, 3, 1u)
TS_RET(GetMenu, 1, 0x00060001u)
TS_RET(GetWindowContextHelpId, 1, 0u)
TS_RET(WaitForInputIdle, 2, 0u)
TS_RET(TranslateAccelerator, 3, 0u)
TS_RET(WinHelpA, 4, 1u)
TS_RET(LoadStringA, 5, 0u)
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

TS_RET(SetDlgItemTextA, 3, 1u)
TS_RET(GetDlgItemTextA, 4, 0u)
TS_RET(SendDlgItemMessageA, 5, 0u)
TS_RET(EndDialog, 2, 1u)
TS_RET(IsDialogMessage, 2, 0u)
TS_RET(GetDlgItem, 2, 0x00010003u)
TS_RET(GetNextDlgTabItem, 3, 0x00010003u)
TS_RET(GetDlgCtrlID, 1, 1u)

TS_BEGIN(MessageBoxA)
    // A message box is the program telling a human why it is about to stop.
    // Nothing here can show one, so the text goes to the run's log -- that log
    // is the only place the reason survives -- and the answer is IDOK, because
    // the alternative is a caller that treats the failure as fatal.
    const char *text = ld_cstr(arg(1));
    char cap[96];
    win_copy(cap, sizeof(cap), arg(2));
    std::printf("win32stubs: MessageBoxA(hwnd 0x%08x) \"%s\" / \"%s\" -> IDOK\n",
                arg(0), cap, text);
    stub_done(4, 1u);
TS_END

TS_RET(MessageBoxIndirectA, 1, 1u)

TS_BEGIN(DialogBoxParamA)
    std::printf("win32stubs: DialogBoxParamA(0x%08x, \"%s\") -> IDOK "
                "(no dialog is created)\n", arg(0), ld_cstr(arg(1)));
    stub_done(5, 1u);
TS_END
TS_RET(DialogBoxIndirectParamA, 5, 1u)
TS_RET(CreateDialogParamA, 5, 0x00010002u)
TS_RET(CreateDialogIndirectParamA, 5, 0x00010002u)

TS_BEGIN(SetWindowLongA)
    // Per window, and the previous value is returned: a subclassing wrapper
    // saves what it replaced and restores it later, and a game stores its own
    // `this` in GWL_USERDATA and reads it back inside its window procedure.
    u32 hwnd = arg(0), idx = arg(1), val = arg(2), prev = 0u;
    WinRec *w = win_find(hwnd);
    if (!w && hwnd == wnd_handle()) w = win_find(wnd_handle());
    if (w) {
        if (idx == 0xfffffffcu) { prev = w->proc; w->proc = val; wnd_proc() = val; }
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
        if (idx == 0xfffffffcu) r = w->proc;
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
    // to tell its own windows from a dialog's.
    u32 buf = arg(1), cch = arg(2), n = 0;
    const char *name = "";
    WinRec *w = win_find(arg(0));
    if (w) for (unsigned i = 0; i < class_n(); ++i)
        if (classes()[i].atom == w->atom) name = classes()[i].name;
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
    // it. It handles %s %c %d %u %x %X %p %%; anything else is copied through.
    u32 dst = arg(0);
    const char *fmt = ld_cstr(arg(1));
    u32 out = 0, ai = 2;
    char tmp[32];
    for (u32 i = 0; fmt[i]; ++i) {
        if (fmt[i] != '%') { st8(dst + out, (u8)fmt[i]); ++out; continue; }
        ++i;
        if (fmt[i] == '%') { st8(dst + out, '%'); ++out; continue; }
        bool pad0 = false;
        while (fmt[i] == '0' || fmt[i] == '-') { pad0 = pad0 || fmt[i] == '0'; ++i; }
        while (fmt[i] >= '0' && fmt[i] <= '9') ++i;
        if (fmt[i] == 'l') ++i;
        char c = fmt[i];
        if (c == 's') {
            u32 s = arg(ai++);
            while (ld8(s)) st8(dst + out++, ld8(s++));
        } else if (c == 'c') {
            st8(dst + out++, (u8)arg(ai++));
        } else if (c == 'd' || c == 'i' || c == 'u' || c == 'x' || c == 'X') {
            i32 v = (i32)arg(ai++);
            u32 base = (c == 'x' || c == 'X') ? 16u : ((c == 'u') ? 10u : 10u);
            bool neg = false;
            u32 uv;
            if (v < 0 && c != 'u' && c != 'x' && c != 'X') { neg = true; uv = (u32)(0 - (u32)v); }
            else uv = (u32)v;
            unsigned k = 0;
            do {
                u32 dg = uv % base;
                tmp[k++] = (char)(dg < 10u ? ('0' + dg) : ((c == 'X' ? 'A' : 'a') + dg - 10u));
                uv /= base;
            } while (uv && k < sizeof(tmp));
            if (neg) st8(dst + out++, '-');
            while (k) st8(dst + out++, (u8)tmp[--k]);
        } else if (c == 'p') {
            u32 v = arg(ai++);
            tmp[0] = '0'; tmp[1] = 'x';
            unsigned k = 2;
            for (int b = 7; b >= 0; --b) {
                u32 dg = (v >> (b * 4)) & 0xfu;
                tmp[k++] = (char)(dg < 10u ? ('0' + dg) : ('a' + dg - 10u));
            }
            for (unsigned j = 0; j < k; ++j) st8(dst + out++, (u8)tmp[j]);
        } else {
            st8(dst + out++, (u8)'%');
            if (c) st8(dst + out++, (u8)c);
        }
        (void)pad0;
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
TS_RET(SetViewportOrgEx, 3, 1u)
TS_RET(SetWindowOrgEx, 3, 1u)
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
TS_RET(dd_CreateClipper, 4, 0u)
TS_RET(dd_CreatePalette, 4, 0u)
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

TS_BEGIN(dd_GetCaps)
    // The game reads dwCaps/dwCaps2 to decide what it may ask for. Zeroing
    // them means "no hardware anything", which is true of this stub.
    st32(arg(1), 0u);
    st32(arg(2), 0u);
    stub_done(3, 0u);
TS_END

TS_BEGIN(dd_GetDisplayMode)
    // DDSURFACEDESC: +0 dwSize, +4 dwFlags, +8 dwHeight, +12 dwWidth,
    // +104 ddpfPixelFormat.dwRGBBitCount.
    u32 p = arg(1);
    for (u32 i = 0; i < 108u; ++i) st8(p + i, 0);
    st32(p + 0u, 108u);
    st32(p + 8u, 480u);
    st32(p + 12u, 640u);
    st32(p + 104u, 16u);
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

#define TS_ORD_BEGIN(dll, n) void k_##dll##_ord_##n() { note("ord_" #n);
#define TS_ORD_END }

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
// CoCreateInstance is handed a fake COM object rather than a null: a null is
// dereferenced immediately, a fake object at least answers AddRef/Release.
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
TS_BEGIN(oleaut_ord_2147483681)  // SysAllocString
    u32 s = arg(0);
    stub_done(1, s ? make_bstr(s, c_strlen(s)) : 0u);
TS_END
TS_BEGIN(oleaut_ord_2147483650)  // SysAllocStringLen
    u32 s = arg(0), n = arg(1);
    stub_done(2, s ? make_bstr(s, n) : 0u);
TS_END
TS_BEGIN(oleaut_ord_2147483656)  // SysStringLen
    u32 b = arg(0);
    stub_done(1, b ? ld32(b - 4u) / 2u : 0u);
TS_END
TS_RET(oleaut_ord_2147483654, 1, 0u)   // SysFreeString
TS_RET(oleaut_ord_2147483657, 1, 0u)   // VariantInit
TS_RET(oleaut_ord_2147483660, 1, 0u)   // VariantClear
TS_RET(oleaut_ord_2147483682, 2, 1u)   // SysReAllocStringLen
TS_RET(oleaut_ord_2147483809, 1, 0u)
TS_RET(oleaut_ord_2147483848, 4, 0u)   // VariantChangeType
TS_RET(oleaut_ord_2147483849, 2, 0u)   // VariantCopy
TS_RET(oleaut_ord_2147483850, 2, 0u)   // VariantCopyInd

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
TS_RET(comctl32_ord_2147483665, 1, 1u)

// -- VERSION / SHELL32 -------------------------------------------------------
TS_RET(GetFileVersionInfoSizeA, 2, 0u)   // no version resource, honestly
TS_RET(GetFileVersionInfoA, 4, 0u)
TS_RET(VerQueryValueA, 4, 0u)
TS_RET(FindExecutableA, 3, 1u)

// -- WSOCK32 -----------------------------------------------------------------
// WSAStartup is the one that matters: it must return 0 and fill WSADATA, or a
// caller that checks the result gives up on networking entirely.
TS_BEGIN(wsock_ord_2147483763)   // WSAStartup
    u32 p = arg(1);
    for (u32 i = 0; i < 0x190u; ++i) st8(p + i, 0);
    st16(p + 0u, 0x0101u);   // wVersion = 1.1
    st16(p + 2u, 0x0101u);   // wHighVersion
    c_strcpy(p + 4u, lit("Tiberian Sun WinSock"));
    st16(p + 0x18cu, 0u);
    st16(p + 0x18eu, 0u);
    stub_done(2, 0u);
TS_END
TS_RET(wsock_ord_2147483764, 0, 0u)   // WSACleanup
TS_RET(wsock_ord_2147483650, 0, 0u)   // WSACleanup
TS_RET(wsock_ord_2147483655, 1, 0u)   // WSASetLastError
TS_RET(wsock_ord_2147483656, 0, 0u)   // WSACancelBlockingCall
TS_RET(wsock_ord_2147483657, 0, 0u)   // WSAIsBlocking
TS_RET(wsock_ord_2147483659, 0, 0u)   // WSAUnhookBlockingHook
TS_RET(wsock_ord_2147483662, 5, 0u)   // WSAAsyncGetHostByName
TS_RET(wsock_ord_2147483663, 7, 0u)   // WSAAsyncGetHostByAddr
TS_RET(wsock_ord_2147483651, 7, 0u)   // WSAAsyncGetServByPort
TS_RET(wsock_ord_2147483665, 7, 0u)   // WSAAsyncGetServByPort
TS_RET(wsock_ord_2147483668, 4, 0u)   // WSAAsyncGetProtoByNumber
TS_RET(wsock_ord_2147483669, 6, 0u)   // WSAAsyncGetServByName
TS_RET(wsock_ord_2147483671, 4, 0u)   // WSAAsyncGetProtoByName
TS_RET(wsock_ord_2147483700, 3, 0u)   // gethostbyaddr
TS_RET(wsock_ord_2147483749, 5, 0u)   // WSAAsyncSelect
TS_RET(wsock_ord_2147483756, 0, 0u)
TS_RET(wsock_ord_2147483759, 0, 0u)   // WSAGetLastError
TS_RET(wsock_ord_2147483705, 2, 0u)   // gethostname

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

template<unsigned I> void slot_stub() {
    const SlotDef &d = slot_defs()[I];
    note(d.name ? d.name : "com");
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

// Build a fake COM object: an object block whose first dword points at a
// freshly allocated vtable. Each slot gets its own registered stub VA.
inline u32 make_com_object(const char *const *names, const u8 *nargs,
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
    u32 obj = alloc_block(16u, 4u);
    st32(obj, vt);
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
        2, 2, 2, 3, 1, 4, 2, 1, 2, 3, 3, 2, 2, 4, 2, 3, 2, 2, 2, 3,
        4, 3, 2, 2, 1,
    };
    Fn over[45] = { 0 };
    over[25] = &k_sfs_Lock;          // Lock
    over[22] = &k_sfs_GetSurfaceDesc;
    over[12] = &k_sfs_GetAttachedSurface;
    return make_com_object(names, nargs, over, 45);
}

TS_BEGIN(sfs_GetSurfaceDesc)
    // DDSURFACEDESC: +8 dwHeight, +12 dwWidth, +104 dwRGBBitCount.
    u32 p = arg(1);
    if (p) {
        for (u32 i = 0; i < 108u; ++i) st8(p + i, 0);
        st32(p + 0u, 108u);
        st32(p + 8u, 480u);
        st32(p + 12u, 640u);
        st32(p + 104u, 16u);
    }
    stub_done(2, 0u);
TS_END

TS_BEGIN(sfs_Lock)
    // Lock(this, lpDestRect, lpDDSurfaceDesc, dwFlags). Handing back a real
    // buffer is the whole point: a caller that locks and blits into lpSurface
    // then writes somewhere that exists.
    u32 p = arg(2);
    static u32 buffer = 0;
    if (!buffer) buffer = zero_block(640u * 480u * 2u, 0x1000u);
    if (p) {
        for (u32 i = 0; i < 108u; ++i) st8(p + i, 0);
        st32(p + 0u, 108u);
        st32(p + 8u, 480u);
        st32(p + 12u, 640u);
        st32(p + 16u, 1280u);        // lPitch = 640 * 2 bytes
        st32(p + 20u, buffer);       // lpSurface
        st32(p + 104u, 16u);
    }
    stub_done(4, 0u);
TS_END

TS_BEGIN(sfs_GetAttachedSurface)
    u32 out = arg(2);
    if (out) st32(out, make_surface());
    stub_done(3, 0u);
TS_END

inline u32 make_directdraw() {
    // IDirectDraw, DirectX 6 slot order. Slots whose out-parameter the game
    // reads (display mode, caps, monitor) are the ones with real bodies.
    static const char *const names[23] = {
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
    };
    static const u8 nargs[23] = {
        3, 1, 1, 1, 4, 4, 4, 3, 5, 5, 1, 3, 2, 3, 2, 2, 2, 2, 2, 1, 3, 4, 3,
    };
    Fn over[23] = { 0 };
    over[6] = &k_dd_CreateSurface;
    over[11] = &k_dd_GetCaps;
    over[12] = &k_dd_GetDisplayMode;
    over[15] = &k_dd_GetMonitorFrequency;
    over[16] = &k_dd_GetScanLine;
    over[17] = &k_dd_GetVerticalBlankStatus;
    return make_com_object(names, nargs, over, 23);
}

TS_BEGIN(DirectDrawCreate)
    u32 out = arg(1);
    if (out) st32(out, make_directdraw());
    stub_done(3, 0u);   // DD_OK
TS_END

TS_BEGIN(dsound_ord_2147483649)   // DirectSoundCreate
    u32 out = arg(1);
    if (out) st32(out, make_directdraw());
    stub_done(3, 0u);   // DS_OK
TS_END

// the import table
//
// One row per import in bin/GAME.EXE's import directory, in directory order.
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
	{ "USER32", "LoadStringA", 5, 0x00000000u, &k_LoadStringA },
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
	{ "GDI32", "SetWindowOrgEx", 3, 0x00000000u, &k_SetWindowOrgEx },
	{ "GDI32", "SetViewportOrgEx", 3, 0x00000000u, &k_SetViewportOrgEx },
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
	{ "ole32", "CoCreateInstance", 5, 0x00000000u, 0 },
	{ "ole32", "OleRun", 1, 0x00000000u, &k_OleRun },
	{ "OLEAUT32", "ord_2147483650", 2, 0x00000000u, &k_oleaut_ord_2147483650 },
	{ "OLEAUT32", "ord_2147483809", 1, 0x00000000u, &k_oleaut_ord_2147483809 },
	{ "OLEAUT32", "ord_2147483654", 1, 0x00000000u, &k_oleaut_ord_2147483654 },
	{ "OLEAUT32", "ord_2147483849", 2, 0x00000000u, &k_oleaut_ord_2147483849 },
	{ "OLEAUT32", "ord_2147483660", 1, 0x00000000u, &k_oleaut_ord_2147483660 },
	{ "OLEAUT32", "ord_2147483850", 2, 0x00000000u, &k_oleaut_ord_2147483850 },
	{ "OLEAUT32", "ord_2147483657", 0, 0x00000000u, &k_oleaut_ord_2147483657 },
	{ "OLEAUT32", "ord_2147483656", 0, 0x00000000u, &k_oleaut_ord_2147483656 },
	{ "OLEAUT32", "ord_2147483682", 2, 0x00000000u, &k_oleaut_ord_2147483682 },
	{ "OLEAUT32", "ord_2147483681", 1, 0x00000000u, &k_oleaut_ord_2147483681 },
	{ "OLEAUT32", "ord_2147483848", 4, 0x00000000u, &k_oleaut_ord_2147483848 },
	{ "WINMM", "timeBeginPeriod", 1, 0x00000000u, &k_timeBeginPeriod },
	{ "WINMM", "timeSetEvent", 5, 0x00000000u, &k_timeSetEvent },
	{ "WINMM", "timeKillEvent", 1, 0x00000000u, &k_timeKillEvent },
	{ "WINMM", "timeGetTime", 0, 0x00000000u, &k_timeGetTime },
	{ "WINMM", "timeGetDevCaps", 2, 0x00000000u, &k_timeGetDevCaps },
	{ "WINMM", "timeEndPeriod", 1, 0x00000000u, &k_timeEndPeriod },
	{ "WSOCK32", "ord_2147483665", 7, 0x00000000u, &k_wsock_ord_2147483665 },
	{ "WSOCK32", "ord_2147483669", 6, 0x00000000u, &k_wsock_ord_2147483669 },
	{ "WSOCK32", "ord_2147483668", 4, 0x00000000u, &k_wsock_ord_2147483668 },
	{ "WSOCK32", "ord_2147483764", 0, 0x00000000u, &k_wsock_ord_2147483764 },
	{ "WSOCK32", "ord_2147483749", 5, 0x00000000u, &k_wsock_ord_2147483749 },
	{ "WSOCK32", "ord_2147483756", 0, 0x00000000u, &k_wsock_ord_2147483756 },
	{ "WSOCK32", "ord_2147483655", 1, 0x00000000u, &k_wsock_ord_2147483655 },
	{ "WSOCK32", "ord_2147483759", 0, 0x00000000u, &k_wsock_ord_2147483759 },
	{ "WSOCK32", "ord_2147483659", 0, 0x00000000u, &k_wsock_ord_2147483659 },
	{ "WSOCK32", "ord_2147483663", 7, 0x00000000u, &k_wsock_ord_2147483663 },
	{ "WSOCK32", "ord_2147483662", 5, 0x00000000u, &k_wsock_ord_2147483662 },
	{ "WSOCK32", "ord_2147483657", 0, 0x00000000u, &k_wsock_ord_2147483657 },
	{ "WSOCK32", "ord_2147483763", 2, 0x00000000u, &k_wsock_ord_2147483763 },
	{ "WSOCK32", "ord_2147483705", 2, 0x00000000u, &k_wsock_ord_2147483705 },
	{ "WSOCK32", "ord_2147483700", 3, 0x00000000u, &k_wsock_ord_2147483700 },
	{ "WSOCK32", "ord_2147483651", 7, 0x00000000u, &k_wsock_ord_2147483651 },
	{ "WSOCK32", "ord_2147483671", 4, 0x00000000u, &k_wsock_ord_2147483671 },
	{ "WSOCK32", "ord_2147483650", 0, 0x00000000u, &k_wsock_ord_2147483650 },
	{ "WSOCK32", "ord_2147483656", 0, 0x00000000u, &k_wsock_ord_2147483656 },
	{ "DDRAW", "DirectDrawCreate", 3, 0x00000000u, &k_DirectDrawCreate },
	{ "DSOUND", "ord_2147483649", 3, 0x00000000u, &k_dsound_ord_2147483649 },
	{ "VERSION", "GetFileVersionInfoSizeA", 2, 0x00000000u, &k_GetFileVersionInfoSizeA },
	{ "VERSION", "GetFileVersionInfoA", 4, 0x00000000u, &k_GetFileVersionInfoA },
	{ "VERSION", "VerQueryValueA", 4, 0x00000000u, &k_VerQueryValueA },
	{ "COMCTL32", "ImageList_DragShowNolock", 1, 0x00000001u, &k_ImageList_DragShowNolock },
	{ "COMCTL32", "ord_2147483665", 1, 0x00000000u, &k_comctl32_ord_2147483665 },
	{ "COMCTL32", "ImageList_DragMove", 2, 0x00000001u, &k_ImageList_DragMove },
	{ "COMCTL32", "ImageList_DragEnter", 3, 0x00000001u, &k_ImageList_DragEnter },
	{ "COMCTL32", "ImageList_BeginDrag", 3, 0x00000001u, &k_ImageList_BeginDrag },
	{ "COMCTL32", "ImageList_Destroy", 1, 0x00000001u, &k_ImageList_Destroy },
	{ "COMCTL32", "ImageList_EndDrag", 0, 0x00000000u, &k_ImageList_EndDrag },
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
    static Fn f[341] = {
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
        &idx_stub<336>, &idx_stub<337>, &idx_stub<338>, &idx_stub<339>, &idx_stub<340>,
    };
    return i < 341u ? f[i] : 0;
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
// The name comes from the last stub_va() call, because that is the only thing
// the shared interface carries. A caller that asks stub_va(dll, fn) and then
// fallback_stub_va() gets a fallback that names dll!fn.

struct FallbackDef { char name[80]; u8 nargs; };
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
    // Loud: an import nothing implements is a hole in the model, and the only
    // moment it is attributable to a name is when it is called.
    std::printf("win32stubs: NO STUB for import \"%s\" -- the fallback returns "
                "0 and cannot pop its arguments\n",
                d.name[0] ? d.name : "(unnamed)");
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

inline u32 fallback_impl() {
    unsigned i = fallback_slot()++ % 256u;
    FallbackDef &d = fallback_defs()[i];
    std::strncpy(d.name, pending_query(), sizeof(d.name) - 1);
    d.name[sizeof(d.name) - 1] = 0;
    d.nargs = 0;   // an unknown import has an unknown argument count
    state().fallback_names++;
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
    for (unsigned i = 0; i < n; ++i) {
        u32 va = alloc_stub_va();
        stub_vas()[i] = va;
        ts_register(va, t[i].real ? t[i].real : idx_stub_fn(i));
    }
}

inline u32 stub_va_impl(const char *dll, const char *fn) {
    remember_query(dll, fn);
    unsigned n = 0;
    const ImportDef *t = imports(&n);
    for (unsigned i = 0; i < n; ++i) {
        if (!std::strcmp(t[i].fn, fn) && same_dll(t[i].dll, dll)) {
            state().resolved++;
            return stub_vas()[i];
        }
    }
    return 0;
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
    std::printf("win32stubs: %u imports; %u with a hand-written body; "
                "%u generic; %u stubs resolved; %lu calls; %lu fallbacks\n",
                n, real, n - real, (unsigned)w32::state().resolved,
                w32::state().calls, w32::state().fallbacks);
    for (unsigned i = 0; i < n; ++i) {
        unsigned long c = w32::counts()[i];
        if (c) std::printf("  %8lu  %s!%s\n", c, t[i].dll, t[i].fn);
    }
}

}  // namespace ts

#endif  // TS_WIN32STUBS_H
