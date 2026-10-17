#!/usr/bin/env python3
"""Generate the import table at the tail of src/handwritten/Win32Stubs.h.

Win32Stubs.h is a hand-written header -- it is copied verbatim into src/include
by the pipeline and nothing regenerates it. But its tail is 341 data rows, one
per import in bin/GAME.EXE's import directory, and typing those by hand invites
a wrong argument count that no check would catch. So the tail is generated here
and the head is left alone.

The split point is the line `// the import table`: everything before it is the
hand-written part (helpers, stub bodies, the COM-object machinery) and is kept
byte-for-byte; everything from it on is rewritten. Running this twice is
therefore safe.

    python3 tools/gen_win32_stubs.py

It reads the import directory straight out of bin/GAME.EXE, so a name that is
not in the binary cannot appear in the table.
"""

import re
import struct
import sys

HDR = "src/handwritten/Win32Stubs.h"
BIN = "bin/GAME.EXE"
SPLIT = "\n// the import table\n"

# Arguments each import owns and pops. Correct counts matter: a stub that pops
# the wrong number of __stdcall arguments shifts every frame above it. The
# return value only has to keep the caller alive, so it is usually 0.
NARGS = {
    # kernel32
    "AddAtomW": 1, "AllocConsole": 0, "BackupSeek": 6, "ClearCommBreak": 1,
    "ClearCommError": 3, "CloseHandle": 1, "CloseProfileUserMapping": 0,
    "CompareFileTime": 2, "CompareStringA": 6, "CreateDirectoryA": 2,
    "CreateEventA": 4, "CreateEventW": 4, "CreateFileA": 7, "CreateMutexA": 3,
    "CreateProcessA": 10, "DeleteCriticalSection": 1, "DeleteFileA": 1,
    "DeviceIoControl": 8, "DosDateTimeToFileTime": 3, "EnterCriticalSection": 1,
    "EnumSystemLocalesA": 2, "EscapeCommFunction": 2, "ExitProcess": 1,
    "FatalAppExitA": 2, "FileTimeToDosDateTime": 3, "FileTimeToLocalFileTime": 2,
    "FileTimeToSystemTime": 2, "FindClose": 1, "FindFirstFileA": 2,
    "FindNextFileA": 2, "FindResourceA": 3, "FlushFileBuffers": 1,
    "FlushInstructionCache": 3, "FreeConsole": 0, "FreeEnvironmentStringsA": 1,
    "FreeLibrary": 1, "GetACP": 0, "GetCPInfo": 2, "GetCommMask": 2,
    "GetCommModemStatus": 2, "GetCommState": 2, "GetCommandLineA": 0,
    "GetConsoleMode": 2, "GetCurrentDirectoryA": 2, "GetCurrentProcess": 0,
    "GetCurrentProcessId": 0, "GetCurrentThread": 0, "GetCurrentThreadId": 0,
    "GetDateFormatA": 5, "GetDiskFreeSpaceA": 5, "GetDriveTypeA": 1,
    "GetEnvironmentStrings": 0, "GetExitCodeProcess": 2, "GetFileAttributesA": 1,
    "GetFileInformationByHandle": 2, "GetFileSize": 2, "GetFileTime": 4,
    "GetFileType": 1, "GetLastError": 0, "GetLocalTime": 1, "GetLocaleInfoA": 4,
    "GetLogicalDriveStringsA": 2, "GetModuleFileNameA": 3, "GetModuleHandleA": 1,
    "GetNumberOfConsoleInputEvents": 2, "GetOEMCP": 0, "GetOverlappedResult": 4,
    "GetProcAddress": 2, "GetStartupInfoA": 1, "GetStdHandle": 1,
    "GetStringTypeA": 5, "GetSystemTime": 1, "GetSystemTimeAsFileTime": 1,
    "GetTickCount": 0, "GetTimeFormatA": 5, "GetTimeZoneInformation": 1,
    "GetUserDefaultLCID": 0, "GetVersion": 0, "GetVersionExA": 1,
    "GetVolumeInformationA": 8, "GlobalAlloc": 2, "GlobalFree": 1,
    "GlobalMemoryStatus": 1, "HeapAlloc": 3, "HeapCreate": 3, "HeapDestroy": 1,
    "HeapFree": 3, "HeapReAlloc": 4, "HeapSize": 3,
    "InitializeCriticalSection": 1, "InterlockedDecrement": 1,
    "InterlockedIncrement": 1, "IsBadCodePtr": 1, "IsBadHugeReadPtr": 2,
    "IsBadHugeWritePtr": 2, "IsValidCodePage": 1, "IsValidLocale": 2,
    "LCMapStringA": 6, "LeaveCriticalSection": 1, "LoadLibraryA": 1,
    "LoadResource": 2, "LocalAlloc": 2, "LocalFree": 1, "LockResource": 1,
    "MultiByteToWideChar": 6, "OpenMutexA": 3, "PeekConsoleInputA": 4,
    "PurgeComm": 2, "QueryPerformanceCounter": 1, "QueryPerformanceFrequency": 1,
    "RaiseException": 4, "ReadConsoleInputA": 4, "ReadFile": 5, "ReleaseMutex": 1,
    "ResetEvent": 1, "RtlUnwind": 4, "SetCommBreak": 1, "SetCommState": 2,
    "SetCommTimeouts": 2, "SetConsoleCtrlHandler": 2, "SetConsoleMode": 2,
    "SetCurrentDirectoryA": 1, "SetEndOfFile": 1, "SetEnvironmentVariableA": 2,
    "SetEvent": 1, "SetFilePointer": 4, "SetFileTime": 4, "SetHandleCount": 1,
    "SetLastError": 1, "SetStdHandle": 2, "SetUnhandledExceptionFilter": 1,
    "SetVolumeLabelA": 2, "SetupComm": 3, "Sleep": 1, "SystemTimeToFileTime": 2,
    "TerminateProcess": 2, "TlsAlloc": 0, "TlsFree": 1, "TlsGetValue": 1,
    "TlsSetValue": 2, "UnhandledExceptionFilter": 1, "VirtualAlloc": 4,
    "VirtualFree": 3, "VirtualProtect": 4, "VirtualQuery": 3,
    "WaitForMultipleObjects": 4, "WaitForSingleObject": 2,
    "WideCharToMultiByte": 8, "WriteConsoleA": 5, "WriteFile": 5,
    "lstrcat": 2, "lstrcpy": 2, "lstrlen": 1, "lstrlenW": 1,
    # user32
    "AdjustWindowRectEx": 4, "BringWindowToTop": 1, "CallWindowProcA": 5,
    "CharToOemBuffA": 3, "CharNextA": 1, "CharPrevA": 2,
    "ChildWindowFromPoint": 3, "ClientToScreen": 2, "CloseWindow": 1,
    "CreateDialogIndirectParamA": 5, "CreateDialogParamA": 5,
    "CreateWindowExA": 12, "DefWindowProcA": 4, "DestroyWindow": 1,
    "DialogBoxIndirectParamA": 5, "DialogBoxParamA": 5, "DispatchMessageA": 1,
    "DrawTextA": 5, "EnableWindow": 2, "EndDialog": 2, "EnumChildWindows": 3,
    "FindWindowA": 2, "GetActiveWindow": 0, "GetAsyncKeyState": 1,
    "GetCapture": 0, "GetClassNameA": 3, "GetClientRect": 2, "GetCursorPos": 1,
    "GetDC": 1, "GetDlgCtrlID": 1, "GetDlgItem": 2, "GetDlgItemTextA": 4,
    "GetFocus": 0, "GetForegroundWindow": 0, "GetKeyNameTextA": 4,
    "GetKeyState": 1, "GetMenu": 1, "GetMessageA": 4, "GetNextDlgTabItem": 3,
    "GetParent": 1, "GetSystemMetrics": 1, "GetTopWindow": 0, "GetUpdateRect": 3,
    "GetWindow": 2, "GetWindowContextHelpId": 1, "GetWindowLongA": 2,
    "GetWindowRect": 2, "GetWindowTextA": 3, "IntersectRect": 3,
    "InvalidateRect": 3, "IsDialogMessage": 2, "IsWindow": 1,
    "IsWindowEnabled": 1, "KillTimer": 2, "LoadCursorA": 2, "LoadIconA": 2,
    "LoadStringA": 5, "MapVirtualKeyA": 2, "MessageBoxA": 4,
    "MessageBoxIndirectA": 1, "MoveWindow": 6, "PeekMessageA": 5,
    "PostMessageA": 4, "PostQuitMessage": 1, "RedrawWindow": 5,
    "RegisterClassA": 1, "RegisterHotKey": 4, "ReleaseCapture": 0,
    "ReleaseDC": 2, "ScreenToClient": 2, "SendDlgItemMessageA": 5,
    "SendMessageA": 4, "SetActiveWindow": 1, "SetCapture": 1, "SetCursor": 1,
    "SetCursorPos": 2, "SetDlgItemTextA": 3, "SetFocus": 1,
    "SetForegroundWindow": 1, "SetRect": 5, "SetTimer": 4, "SetWindowLongA": 3,
    "SetWindowPos": 7, "SetWindowTextA": 2, "ShowCursor": 1, "ShowWindow": 2,
    "ToAscii": 5, "TranslateAccelerator": 3, "TranslateMessage": 1,
    "UpdateWindow": 1, "ValidateRect": 2, "WaitForInputIdle": 2, "WinHelpA": 4,
    "WindowFromPoint": 2, "wsprintfA": 0,
    # gdi32
    "CreateDIBSection": 6, "CreateFontA": 14, "CreateFontIndirectA": 1,
    "CreateSolidBrush": 1, "DPtoLP": 3, "DeleteObject": 1, "DeleteDC": 1,
    "GetBkColor": 1, "GetBkMode": 1, "GetStockObject": 1, "GetTextColor": 1,
    "GetTextExtentPoint32A": 4, "GetTextMetricsA": 2, "ModifyWorldTransform": 3,
    "RestoreDC": 2, "SaveDC": 1, "SelectObject": 2, "SetBkColor": 2,
    "SetBkMode": 2, "SetGraphicsMode": 2, "SetTextAlign": 2, "SetTextColor": 2,
    "SetViewportOrgEx": 3, "SetWindowOrgEx": 3, "TextOutA": 5, "BitBlt": 9,
    "StretchBlt": 11, "PatBlt": 6, "Rectangle": 7, "CreateCompatibleDC": 1,
    "CreateCompatibleBitmap": 3,
    # advapi32, shell32
    "RegCloseKey": 1, "RegCreateKeyExA": 9, "RegDeleteKeyA": 2,
    "RegEnumKeyExA": 8, "RegOpenKeyExA": 5, "RegQueryInfoKeyA": 12,
    "RegQueryValueExA": 6, "RegSetValueExA": 6, "FindExecutableA": 3,
    # ole32
    "CLSIDFromString": 2, "CoCreateInstance": 5, "CoDisconnectObject": 2,
    "CoFileTimeNow": 1, "CoRegisterClassObject": 5, "CoRevokeClassObject": 1,
    "OleInitialize": 1, "OleLoadFromStream": 3, "OleRun": 1,
    "OleSaveToStream": 2, "OleUninitialize": 0, "StgCreateDocfile": 4,
    "StgOpenStorage": 6, "StringFromCLSID": 2, "StringFromGUID2": 3,
    # winmm, comctl32, version, ddraw
    "timeBeginPeriod": 1, "timeEndPeriod": 1, "timeGetDevCaps": 2,
    "timeGetTime": 0, "timeKillEvent": 1, "timeSetEvent": 5,
    "ImageList_BeginDrag": 3, "ImageList_Destroy": 1, "ImageList_DragEnter": 3,
    "ImageList_DragMove": 2, "ImageList_DragShowNolock": 1,
    "ImageList_EndDrag": 0, "GetFileVersionInfoA": 4,
    "GetFileVersionInfoSizeA": 2, "VerQueryValueA": 4, "DirectDrawCreate": 3,
    # ordinals
    "ord_2147483649": 3, "ord_2147483650": 1, "ord_2147483651": 7,
    "ord_2147483654": 1, "ord_2147483655": 1, "ord_2147483656": 0,
    "ord_2147483657": 0, "ord_2147483659": 0, "ord_2147483660": 1,
    "ord_2147483662": 5, "ord_2147483663": 7, "ord_2147483665": 1,
    "ord_2147483668": 4, "ord_2147483669": 6, "ord_2147483671": 4,
    "ord_2147483681": 1, "ord_2147483682": 2, "ord_2147483700": 3,
    "ord_2147483705": 2, "ord_2147483749": 5, "ord_2147483756": 0,
    "ord_2147483759": 0, "ord_2147483763": 2, "ord_2147483764": 0,
    "ord_2147483809": 1, "ord_2147483848": 4, "ord_2147483849": 2,
    "ord_2147483850": 2,
}

# An ordinal means different things in different DLLs, so these few override.
NARGS_DLL = {
    ("WSOCK32.dll", "ord_2147483650"): 0,
    ("WSOCK32.dll", "ord_2147483651"): 7,
    ("WSOCK32.dll", "ord_2147483665"): 7,
    ("COMCTL32.dll", "ord_2147483665"): 1,
    ("DSOUND.dll", "ord_2147483649"): 3,
    ("OLEAUT32.dll", "ord_2147483650"): 2,
}

# The value a stub leaves in EAX. 0 is the safe default; these are the ones
# where 0 would read as failure to a caller that checks.
RET = {
    "FreeLibrary": 1, "IsValidCodePage": 1, "IsValidLocale": 1, "TlsFree": 1,
    "TlsSetValue": 1, "SetEvent": 1, "ResetEvent": 1, "ReleaseMutex": 1,
    "WriteFile": 1, "ReadFile": 1, "CloseHandle": 1, "WaitForSingleObject": 0,
    "WaitForMultipleObjects": 0, "GetVersion": 0x04000004, "GetACP": 1252,
    "GetOEMCP": 437, "GetUserDefaultLCID": 0x0409, "GetDriveTypeA": 3,
    "SetCurrentDirectoryA": 1, "CreateDirectoryA": 1, "DeleteFileA": 1,
    "SetEndOfFile": 1, "SetFileTime": 1, "GetFileTime": 1,
    "GetFileInformationByHandle": 1, "GetOverlappedResult": 1,
    "UnhandledExceptionFilter": 1, "OleInitialize": 0, "OleUninitialize": 0,
    "RegQueryValueExA": 2, "ImageList_Destroy": 1, "ImageList_BeginDrag": 1,
    "ImageList_DragEnter": 1, "ImageList_DragMove": 1,
    "ImageList_DragShowNolock": 1, "GetFileType": 2, "GetCurrentProcess": -1,
    "GetCurrentThread": -2, "GetFileAttributesA": 0x20,
}

DLL_SHORT = {
    "KERNEL32.dll": "KERNEL32", "USER32.dll": "USER32", "GDI32.dll": "GDI32",
    "ADVAPI32.dll": "ADVAPI32", "SHELL32.dll": "SHELL32", "ole32.dll": "ole32",
    "OLEAUT32.dll": "OLEAUT32", "WINMM.dll": "WINMM", "WSOCK32.dll": "WSOCK32",
    "DDRAW.dll": "DDRAW", "DSOUND.dll": "DSOUND", "VERSION.dll": "VERSION",
    "COMCTL32.dll": "COMCTL32",
}
# An ordinal stub's C++ name carries its DLL, because the same ordinal number
# appears in more than one of these.
ORD_PREFIX = {"OLEAUT32.dll": "oleaut", "WSOCK32.dll": "wsock",
              "COMCTL32.dll": "comctl32", "DSOUND.dll": "dsound"}


def read_imports(path):
    """The import directory, straight out of the binary."""
    d = open(path, "rb").read()
    pe = struct.unpack_from("<I", d, 0x3C)[0]
    nsec = struct.unpack_from("<H", d, pe + 6)[0]
    optsz = struct.unpack_from("<H", d, pe + 20)[0]
    secs = []
    for i in range(nsec):
        e = pe + 24 + optsz + i * 40
        vs, va, rs, ro = struct.unpack_from("<IIII", d, e + 8)
        secs.append((va, vs, ro, rs))

    def r2o(rva):
        for va, vs, ro, rs in secs:
            if va <= rva < va + max(vs, rs):
                return ro + (rva - va)
        raise ValueError("rva 0x%x is in no section" % rva)

    def cstr(off):
        return d[off:d.index(b"\0", off)].decode("latin1")

    imp = struct.unpack_from("<II", d, pe + 24 + 104)[0]
    out = []
    i = 0
    while True:
        o = r2o(imp + i * 20)
        oft, _ts, _fc, namerva, ft = struct.unpack_from("<IIIII", d, o)
        if oft == 0 and namerva == 0:
            break
        dll = cstr(r2o(namerva))
        fns = []
        j = 0
        while True:
            v = struct.unpack_from("<I", d, r2o(oft or ft) + j * 4)[0]
            if v == 0:
                break
            fns.append("ord_%d" % v if v & 0x80000000 else cstr(r2o(v) + 2))
            j += 1
        out.append((dll, fns))
        i += 1
    return out


def main():
    src = open(HDR).read()
    head = src.split(SPLIT)[0]
    # The bodies are written with the TS_RET/TS_BEGIN/TS_ORD_BEGIN macros, so
    # the symbol names only exist after expansion: collect the macro arguments.
    real = set("k_" + n for n in re.findall(r"TS_RET\(([A-Za-z0-9_]+)\s*,", head))
    real |= set("k_" + n for n in re.findall(r"TS_BEGIN\(([A-Za-z0-9_]+)\)", head))
    real |= set("k_%s_ord_%s" % (d, n) for d, n in
                re.findall(r"TS_ORD_BEGIN\(([A-Za-z0-9_]+)\s*,\s*([0-9]+)\)", head))
    real.discard("k_w")   # the `#define TS_BEGIN(w)` line itself
    imports = read_imports(BIN)

    rows, missing = [], []
    for dll, fns in imports:
        for fn in fns:
            n = NARGS_DLL.get((dll, fn), NARGS.get(fn))
            if n is None:
                missing.append("%s!%s" % (dll, fn))
                n = 0
            r = RET.get(fn, 0)
            if fn.startswith("ord_") and dll in ORD_PREFIX:
                sym = "k_%s_%s" % (ORD_PREFIX[dll], fn)
            else:
                sym = "k_" + fn
            ref = ("&" + sym) if sym in real else "0"
            rows.append('\t{ "%s", "%s", %u, 0x%08xu, %s },'
                        % (DLL_SHORT[dll], fn, n, r & 0xffffffff, ref))
    if missing:
        print("WARNING: no argument count for %s" % ", ".join(missing))

    withreal = sum(1 for r in rows if not r.rstrip(",").endswith(" 0 }"))
    print("imports %d; with a hand-written body %d; generic %d"
          % (len(rows), withreal, len(rows) - withreal))

    out = []
    out.append("""
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
    static const ImportDef t[] = {""")
    out.append("\n".join(rows))
    out.append("""    };
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
                    "returns 0x%08x\\n", t[I].dll, t[I].fn, (unsigned)t[I].ret);
    }
    stub_done(t[I].nargs, t[I].ret);
}
""")

    refs = ["&idx_stub<%d>," % i for i in range(len(rows))]
    body = "\n".join("        " + " ".join(refs[i:i + 6])
                     for i in range(0, len(refs), 6))
    out.append("inline Fn idx_stub_fn(unsigned i) {\n    static Fn f[%d] = {\n%s\n"
               "    };\n    return i < %du ? f[i] : 0;\n}\n"
               % (len(rows), body, len(rows)))

    out.append("""
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
    std::printf("win32stubs: NO STUB for import \\"%s\\" -- the fallback returns "
                "0 and cannot pop its arguments\\n",
                d.name[0] ? d.name : "(unnamed)");
    stub_done(d.nargs, 0u);   // 0 in EAX: unimplemented, and it says so
}
""")

    refs = ["&fallback_at<%d>," % i for i in range(256)]
    body = "\n".join("        " + " ".join(refs[i:i + 8])
                     for i in range(0, 256, 8))
    out.append("inline Fn fallback_fn(unsigned i) {\n    static Fn f[256] = {\n%s\n"
               "    };\n    return f[i];\n}\n" % body)

    out.append("""
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
                "%u generic; %u stubs resolved; %lu calls; %lu fallbacks\\n",
                n, real, n - real, (unsigned)w32::state().resolved,
                w32::state().calls, w32::state().fallbacks);
    for (unsigned i = 0; i < n; ++i) {
        unsigned long c = w32::counts()[i];
        if (c) std::printf("  %8lu  %s!%s\\n", c, t[i].dll, t[i].fn);
    }
    // COM methods are not imports, so the ones served by a generic slot stub
    // are reported from the slot table: a DirectDraw wrapper's Blt, Flip and
    // Unlock calls land there, and a DirectDraw phase needs to see them by
    // name. A slot with a hand-written body above counts under that body's
    // name in the import table's shared bucket instead.
    unsigned slots = w32::slot_next();
    for (unsigned i = 0; i < slots; ++i) {
        unsigned long c = w32::slot_counts()[i];
        if (c) std::printf("  %8lu  %s (vtable slot %u)\\n", c,
                           w32::slot_defs()[i].name ? w32::slot_defs()[i].name : "com", i);
    }
}

}  // namespace ts

#endif  // TS_WIN32STUBS_H
""")

    open(HDR, "w").write(head + "\n".join(out))


if __name__ == "__main__":
    sys.exit(main())
