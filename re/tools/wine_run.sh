#!/bin/bash
# Run the REAL GAME.EXE under Wine -- the only oracle this project has.
#
# Everything else here checks the reconstruction against the reconstruction. This
# runs the retail binary and reports what it does, which is the only way to know
# whether the model's behaviour is the game's behaviour.
#
#   ./tools/wine_run.sh                    # plain run, 90s, headless
#   ./tools/wine_run.sh --trace ddraw      # WINEDEBUG=+ddraw, the DirectDraw calls
#   ./tools/wine_run.sh --trace file,reg --secs 20
#   ./tools/wine_run.sh --dismiss      # walk the startup dialog chain
#   ./tools/wine_run.sh --shot out.png # grab the screen with ffmpeg
#   ./tools/wine_run.sh --cd work/cd1 --screen 640x480x16 --dismiss --shot /tmp/ts.png
#                                      # give it the CD and a display mode it can use
#   ./tools/wine_env.sh                # the same, with the right defaults
#
# Prerequisites (Ubuntu 24.04, done once):
#   sudo dpkg --add-architecture i386 && sudo apt-get update
#   sudo apt-get install -y wine32:i386 xvfb
#   # The default prefix is wow64-only and CANNOT run a 32-bit exe: it fails with
#   # "failed to load syswow64\ntdll.dll". A 32-bit prefix needs WINEARCH=win32,
#   # and Wine 9 refuses that in wow64 mode unless wine32:i386 is installed.
#
# It found, in one run, that the retail game reaches DirectDraw surface creation
# and dies there -- and that its IDirectDraw calls include slot 23
# (GetAvailableVidMem), which our COM vtable was one entry too short for.
#
# WHAT IT DOES HERE, measured:
#
#   without --cd: creates a "Tiberian Sun" window, shows "Warning - Unable to
#   create Direct Sound Object. Proceeding with audio disabled.", then "Error -
#   Unable to set the video mode." and exits.
#
#   with --cd work/cd1 --screen 640x480x16: dismisses the DirectSound warning,
#   plays WWLOGO.VQA and STARTUP.VQA from the CD, and reaches the main menu
#   (the GDI/Nod wheel), which stays up for as long as the game is left running.
#   Two things are load-bearing, both measured from GAME.EXE 0x471cd0 and the
#   Wine traces:
#
#   1. The X screen must already be 640x480x16. The game asks DirectDraw for
#      640x400x16 then 640x480x16 (ddraw trace). Xvfb has no mode list, so the
#      only SetDisplayMode that can succeed is one matching the screen it was
#      started with; anything else makes Wine's NtUserChangeDisplaySettings
#      return -2 and the game shows "Unable to set the video mode".
#   2. The CD must be a DRIVE_CDROM (type 5) whose volume label is TS%d and whose
#      root holds TS%d.DSK. A directory-backed drive is DRIVE_FIXED by default
#      and the game never probes it (trace: GetDriveTypeW d: -> 3, no D:\TS1.dsk
#      open). --cd maps d:, writes .windows-label=TS1, sets
#      HKLM\Software\Wine\Drives "d:"="cdrom" and restarts wineserver, because
#      mountmgr reads that key once at driver init.
#
# No Westwood install registry key is needed: with the CD present the game reads
# RULES.INI/ART.INI/etc. from the CD or its MIX files and never queries
# SOFTWARE\Westwood\Tiberian Sun on this path (verified with --trace reg).
#
# FOR AN INSTRUCTION-LEVEL ORACLE (tools/oracle.py, not yet written), three
# things bite and each cost a run:
#   1. Attaching needs /proc/sys/kernel/yama/ptrace_scope=0 (it is 1 here).
#      `sudo sysctl -w kernel.yama.ptrace_scope=0`.
#   2. Do NOT break at the instruction AFTER ExitProcess -- 0x684529. Wine's
#      ExitProcess terminates the process and never returns, so that breakpoint is
#      never hit. Break at the call itself, 0x684523.
#   3. `gdb --args wine ...` fails ("not in executable format") because
#      /usr/bin/wine is a shell wrapper. The real loader is /usr/lib/wine/wine.
#      Attaching to the running GAME.EXE pid is the more reliable route anyway,
#      because it guarantees the PE is mapped at its preferred base first.
# Verify the mapping before trusting any address: /proc/<pid>/maps should show
#   00400000-00698000 r-xp ... GAME.EXE
set -e

ROOT=$(cd "$(dirname "$0")/.." && pwd)
PREFIX=${WINEPREFIX:-$ROOT/../.wine32}
GAMEDIR=${GAMEDIR:-$ROOT/../gamedir}
SECS=90
TRACE=""
DISMISS=0
SHOT=""
SHOT_DELAY=0
CD=""
SCREEN=640x480x16

while [ $# -gt 0 ]; do
    case "$1" in
        --trace) TRACE="$2"; shift 2 ;;
        --secs)  SECS="$2";  shift 2 ;;
        --dismiss) DISMISS=1; shift ;;
        --shot)  SHOT="$2";  shift 2 ;;
        --shot-delay) SHOT_DELAY="$2"; shift 2 ;;
        --cd)    CD="$2";    shift 2 ;;
        --screen) SCREEN="$2"; shift 2 ;;
        *) echo "usage: $0 [--trace CHANNEL] [--secs N] [--dismiss] [--shot PNG]" >&2
           echo "          [--shot-delay N] [--cd DIR] [--screen WxHxD]" >&2
           exit 2 ;;
    esac
done

case "$SCREEN" in
    [0-9]*x[0-9]*x[0-9]*) ;;
    *) echo "wine_run: --screen must be WxHxD, e.g. 640x480x16" >&2; exit 2 ;;
esac

if [ ! -d "$GAMEDIR" ] || [ ! -f "$GAMEDIR/GAME.EXE" ]; then
    echo "wine_run: no game directory at $GAMEDIR" >&2
    echo "  create it from the CD image: cp -r work/cd1/INSTALL/. gamedir/" >&2
    exit 1
fi
if [ ! -d "$PREFIX" ]; then
    echo "wine_run: no 32-bit prefix at $PREFIX" >&2
    echo "  WINEARCH=win32 WINEPREFIX=$PREFIX wineboot -i" >&2
    exit 1
fi

if [ -n "$CD" ]; then
    if [ ! -d "$CD" ]; then
        echo "wine_run: no CD tree at $CD" >&2
        echo "  --cd wants the extracted CD root, the directory holding TS1.DSK" >&2
        exit 1
    fi
    CD=$(cd "$CD" && pwd)
    if [ ! -f "$CD/TS1.DSK" ]; then
        echo "wine_run: $CD is not a Tiberian Sun CD root (no TS1.DSK)" >&2
        exit 1
    fi
    if [ -e "$PREFIX/dosdevices/d:" ] && [ ! -L "$PREFIX/dosdevices/d:" ]; then
        echo "wine_run: $PREFIX/dosdevices/d: exists and is not a symlink" >&2
        exit 1
    fi
    if ! ln -sfn "$CD" "$PREFIX/dosdevices/d:"; then
        echo "wine_run: cannot map d: in $PREFIX/dosdevices" >&2
        exit 1
    fi
    # The game requires the volume label to be TS%d (GAME.EXE 0x471cd0 formats
    # "TS%d" and compares it with GetVolumeInformationA). mountmgr reads a
    # directory drive's label from .windows-label in the mount root.
    if ! printf 'TS1\n' > "$CD/.windows-label"; then
        echo "wine_run: cannot write $CD/.windows-label; the game needs volume label TS1" >&2
        exit 1
    fi
    # mountmgr reads HKLM\Software\Wine\Drives once at driver init and caches the
    # result, so set it and restart the server. Without it d: is DRIVE_FIXED (3)
    # and the game never looks for its CD on it (measured: GetDriveTypeW d: -> 3).
    if ! WINEPREFIX="$PREFIX" WINEARCH=win32 WINEDEBUG=-all wine reg add \
            "HKLM\\Software\\Wine\\Drives" /v "d:" /d "cdrom" /f >/dev/null 2>&1; then
        echo "wine_run: could not set the d: drive type to cdrom in $PREFIX" >&2
        exit 1
    fi
    WINEPREFIX="$PREFIX" wineserver -k 2>/dev/null || true
    sleep 1
    echo "wine_run: CD $CD -> d: (label TS1, drive type cdrom)"
fi

export WINEPREFIX="$PREFIX" WINEARCH=win32
if [ -n "$TRACE" ]; then
    # Accept "file,registry" as well as "+file,+reg": Wine wants one prefix per
    # channel, and its registry channel is called reg, not registry.
    TRACE_ENV=""
    IFS=',' read -r -a _channels <<< "$TRACE"
    for _ch in "${_channels[@]}"; do
        _ch=${_ch#+}; _ch=${_ch#-}
        [ "$_ch" = "registry" ] && _ch=reg
        TRACE_ENV="${TRACE_ENV:+$TRACE_ENV,}+$_ch"
    done
    export WINEDEBUG="$TRACE_ENV"
else
    export WINEDEBUG=-all
fi

cd "$GAMEDIR"
# -a picks a free X display; the game needs one even when nothing is watching,
# because it creates a real window and asks DirectDraw for a cooperative level.
if [ "$DISMISS" = "1" ] || [ -n "$SHOT" ]; then
    for _t in Xvfb xdpyinfo xwininfo; do
        command -v "$_t" >/dev/null || { echo "wine_run: missing $_t" >&2; exit 1; }
    done
    [ "$DISMISS" = "1" ] && { command -v xdotool >/dev/null || { echo "wine_run: missing xdotool" >&2; exit 1; }; }
    [ -n "$SHOT" ] && { command -v ffmpeg >/dev/null || { echo "wine_run: missing ffmpeg" >&2; exit 1; }; }
fi

set +e
if [ "$DISMISS" = "1" ] || [ -n "$SHOT" ]; then
    # Run on a private display so the dialogs can be found and dismissed while
    # the game is still alive. The screen geometry matters: the game asks for
    # 640x400x16 then 640x480x16, and Xvfb cannot change modes, so the only
    # SetDisplayMode that succeeds is one matching this screen (see the header).
    DISP=77
    while [ "$DISP" -lt 100 ]; do
        if [ -e "/tmp/.X${DISP}-lock" ] && xdpyinfo -display ":$DISP" >/dev/null 2>&1; then
            DISP=$((DISP + 1)); continue
        fi
        [ -e "/tmp/.X${DISP}-lock" ] && { DISP=$((DISP + 1)); continue; }
        break
    done
    Xvfb ":$DISP" -screen 0 "$SCREEN" >/dev/null 2>&1 &
    xvfb_pid=$!
    sleep 2
    if ! xdpyinfo -display ":$DISP" >/dev/null 2>&1; then
        echo "wine_run: Xvfb :$DISP did not start at $SCREEN" >&2
        kill "$xvfb_pid" 2>/dev/null
        exit 1
    fi
    export DISPLAY=":$DISP"
    timeout "$SECS" wine GAME.EXE > /tmp/wine_run.log 2>&1 &
    game_pid=$!
    sleep 20
    dismissed=0
    if [ "$DISMISS" = "1" ]; then
        for _ in 1 2 3 4 5 6 7 8; do
            # Only modal dialogs: narrower than 500 px. The main game window is
            # SCREEN-sized, and sending Return to it would press a menu button.
            small=$(xwininfo -root -tree 2>/dev/null | awk '
                /"Tiberian Sun"/ {
                    if (match($0, /0x[0-9a-f]+/)) id = substr($0, RSTART, RLENGTH);
                    if (match($0, /[0-9]+x[0-9]+\+/)) {
                        split(substr($0, RSTART, RLENGTH), g, "x");
                        if (g[1] + 0 < 500) { print id; exit }
                    }
                }')
            [ -z "$small" ] && break
            xdotool key --window "$small" Return 2>/dev/null
            dismissed=$((dismissed + 1))
            sleep 2
        done
        echo "wine_run: dismissed $dismissed startup dialog(s)"
        echo "wine_run: windows now"
        xwininfo -root -tree 2>/dev/null | grep -E '"Tiberian Sun"' | sed 's/^/  /'
    fi
    if [ -n "$SHOT" ]; then
        [ "$SHOT_DELAY" -gt 0 ] 2>/dev/null && sleep "$SHOT_DELAY"
        ffmpeg -loglevel error -f x11grab -video_size "${SCREEN%x*}" -i ":$DISP" \
               -frames:v 1 -y "$SHOT" 2>/dev/null
        shot_bytes=$(stat -c%s "$SHOT" 2>/dev/null || echo 0)
        echo "wine_run: screenshot $SHOT ($shot_bytes bytes)"
        if [ "$shot_bytes" -lt 20000 ]; then
            echo "  WARNING: a few KB means a blank screen -- the game is not drawing a menu"
        fi
    fi
    wait "$game_pid" 2>/dev/null
    rc=$?
    kill "$xvfb_pid" 2>/dev/null
else
    timeout "$SECS" xvfb-run -a -s "-screen 0 $SCREEN" wine GAME.EXE > /tmp/wine_run.log 2>&1
    rc=$?
fi
set -e

echo "wine_run: exit=$rc (124 = still running when the timeout hit, i.e. a loop)"
if [ "$rc" -eq 124 ]; then
    echo "  the game did NOT crash: it was still running after ${SECS}s"
fi
if grep -q "Unhandled page fault\|page fault on" /tmp/wine_run.log 2>/dev/null; then
    echo "  FAULT:"
    grep -m2 -E "page fault" /tmp/wine_run.log | sed 's/^/    /'
fi
if grep -q "Unable to set the video mode" /tmp/wine_run.log 2>/dev/null; then
    echo "  BLOCKER: DirectDraw SetDisplayMode failed. Xvfb cannot change modes;"
    echo "  start it at the mode the game asks for: --screen 640x480x16"
fi
if [ -n "$CD" ]; then
    if grep -q "TS1.dsk" /tmp/wine_run.log 2>/dev/null; then
        echo "  CD: the game probed D:\\TS1.dsk"
    elif [ -n "$TRACE" ]; then
        echo "  CD: no D:\\TS1.dsk probe in the trace -- it did not accept the CD"
    fi
fi
if [ -n "$TRACE" ]; then
    echo "  trace: /tmp/wine_run.log ($(wc -l < /tmp/wine_run.log) lines)"
else
    grep -vE "ALSA|snd_|pcm\.c|fixme:ntdll" /tmp/wine_run.log | head -20 | sed 's/^/  /'
fi
