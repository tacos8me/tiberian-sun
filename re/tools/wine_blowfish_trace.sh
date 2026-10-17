#!/bin/bash
# Capture the retail game's cold-start blowfish.dll registration from Wine's
# own traces, for the path gdb attaches too late to see.
#
#   ./tools/wine_blowfish_trace.sh          # cold: delete the CLSID key first
#   ./tools/wine_blowfish_trace.sh --warm   # leave the key registered
#
# The registration loop runs in the first second of GAME.EXE init, before an
# out-of-process gdb attach can land, so it is read from WINEDEBUG instead:
# +relay (filtered to KERNEL32/OLE32/BLOWFISH by HKCU\Software\Wine\Debug\
# RelayInclude), +ole and +module. The full log is /tmp/blowfish_relay.log;
# only the lines that answer the question are printed.
#
# It also deletes the CLSID key for the cold run and reports the key afterward,
# which is the runtime proof that DllRegisterServer wrote it: the only writer of
# that subtree is BLOWFISH.DLL!DllRegisterServer.
set -e

ROOT=$(cd "$(dirname "$0")/.." && pwd)
GAMEDIR=${GAMEDIR:-$ROOT/../gamedir}
PREFIX=${WINEPREFIX:-$ROOT/../.wine32}
CD=${TSCD:-$ROOT/../work/cd1}
LOG=${LOG:-/tmp/blowfish_relay.log}
SECS=${SECS:-25}
WARM=0
KEY='HKCR\CLSID\{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}'

[ "$1" = "--warm" ] && WARM=1

export WINEPREFIX="$PREFIX" WINEARCH=win32

if [ ! -f "$GAMEDIR/GAME.EXE" ]; then
    echo "wine_blowfish_trace: no GAME.EXE in $GAMEDIR" >&2; exit 1
fi
if [ ! -f "$CD/TS1.DSK" ]; then
    echo "wine_blowfish_trace: no CD root at $CD" >&2; exit 1
fi

# d: must be a cdrom with label TS1, exactly as wine_run.sh --cd sets up.
ln -sfn "$CD" "$PREFIX/dosdevices/d:"
printf 'TS1\n' > "$CD/.windows-label"
WINEDEBUG=-all wine reg add 'HKLM\Software\Wine\Drives' /v 'd:' /d cdrom /f >/dev/null
wineserver -k 2>/dev/null || true
sleep 1

# RelayInclude is a function list, not a module list (the value Wine ships in
# RelayExclude is "ntdll.RtlEnterCriticalSection;kernel32.48;..."). Keeping it
# to the five calls that matter is what makes +relay usable here: unfiltered,
# this game emits hundreds of MB in the first seconds.
WINEDEBUG=-all wine reg add 'HKCU\Software\Wine\Debug' /v RelayInclude \
    /d 'kernel32.LoadLibraryA;kernel32.GetProcAddress;blowfish.DllRegisterServer;ole32.CoCreateInstance;ole32.OleRun' \
    /f >/dev/null

cleanup() {
    WINEDEBUG=-all wine reg delete 'HKCU\Software\Wine\Debug' /v RelayInclude /f \
        >/dev/null 2>&1 || true
    pkill -x wine 2>/dev/null || true
    [ -n "${XVFB_PID:-}" ] && kill "$XVFB_PID" 2>/dev/null || true
}
trap cleanup EXIT

if [ "$WARM" = "0" ]; then
    WINEDEBUG=-all wine reg delete "$KEY" /f >/dev/null 2>&1 || true
    echo "wine_blowfish_trace: CLSID key deleted (cold start)"
else
    echo "wine_blowfish_trace: CLSID key left in place (warm start)"
fi

Xvfb :81 -screen 0 640x480x16 >/dev/null 2>&1 &
XVFB_PID=$!
sleep 2
export DISPLAY=:81

cd "$GAMEDIR"
timeout "$SECS" env WINEDEBUG=+relay,+ole,+module wine GAME.EXE > "$LOG" 2>&1 || true

echo "wine_blowfish_trace: log $LOG ($(wc -l < "$LOG") lines)"
echo "=== LoadLibraryA / GetProcAddress / DllRegisterServer ==="
# Match the Ret lines by the game's return addresses (0x5e622d / 0x5e623f /
# 0x5e6245): they do not repeat the function name.
grep -nE 'LoadLibraryA|GetProcAddress|DllRegisterServer' "$LOG" \
    | grep -iE 'blowfish|DllRegisterServer|ret=005e622d|ret=005e623f|ret=005e6245' \
    | head -20
echo "=== CoCreateInstance calls and results ==="
grep -nE 'CoCreateInstance|OleRun' "$LOG" | head -30
echo "=== module load ==="
grep -nE 'blowfish.dll.*(Loaded|map_image)|Loaded module.*blowfish' "$LOG" | head -10
echo "=== registry key after run ==="
WINEDEBUG=-all wine reg query "$KEY" 2>&1 | head -6
WINEDEBUG=-all wine reg query "$KEY\\InprocServer32" 2>&1 | head -5
