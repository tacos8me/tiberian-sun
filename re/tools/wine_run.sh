#!/bin/bash
# Run the REAL GAME.EXE under Wine -- the only oracle this project has.
#
# Everything else here checks the reconstruction against the reconstruction. This
# runs the retail binary and reports what it does, which is the only way to know
# whether the model's behaviour is the game's behaviour.
#
#   ./tools/wine_run.sh                    # plain run, 90s, headless
#   ./tools/wine_run.sh --trace ddraw      # WINEDEBUG=+ddraw, the DirectDraw calls
#   ./tools/wine_run.sh --trace relay --secs 20
#   ./tools/wine_run.sh --dismiss      # walk the startup dialog chain
#   ./tools/wine_run.sh --shot out.png # grab the screen with ffmpeg
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
# WHAT IT DOES HERE, verified with a screenshot: creates a 1024x768 "Tiberian Sun"
# window, shows "Warning - Unable to create Direct Sound Object. Proceeding with
# audio disabled.", walks a second dialog, then exits. There is no audio device,
# no CD mounted and no install registry, so the retail game gives up early too.
# Mounting the CD and satisfying the install is what would extend this past init.
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

while [ $# -gt 0 ]; do
    case "$1" in
        --trace) TRACE="$2"; shift 2 ;;
        --secs)  SECS="$2";  shift 2 ;;
        --dismiss) DISMISS=1; shift ;;
        --shot)  SHOT="$2";  shift 2 ;;
        *) echo "usage: $0 [--trace CHANNEL] [--secs N]" >&2; exit 2 ;;
    esac
done

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

export WINEPREFIX="$PREFIX" WINEARCH=win32
if [ -n "$TRACE" ]; then
    export WINEDEBUG="+$TRACE"
else
    export WINEDEBUG=-all
fi

cd "$GAMEDIR"
# -a picks a free X display; the game needs one even when nothing is watching,
# because it creates a real window and asks DirectDraw for a cooperative level.
set +e
if [ "$DISMISS" = "1" ] || [ -n "$SHOT" ]; then
    # Run on a private display so the dialogs can be found and dismissed while
    # the game is still alive. The retail game walks a CHAIN of startup dialogs;
    # one Return dismisses one and reveals the next.
    Xvfb :77 -screen 0 1024x768x16 >/dev/null 2>&1 &
    xvfb_pid=$!
    sleep 2
    export DISPLAY=:77
    timeout "$SECS" wine GAME.EXE > /tmp/wine_run.log 2>&1 &
    game_pid=$!
    sleep 20
    if [ "$DISMISS" = "1" ]; then
        for _ in 1 2 3 4 5 6 7 8; do
            small=$(xwininfo -root -tree 2>/dev/null | grep '"Tiberian Sun"' \
                    | grep -vE "1024x768" | grep -oE "0x[0-9a-f]+" | head -1)
            [ -z "$small" ] && break
            xdotool key --window "$small" Return 2>/dev/null
            sleep 2
        done
        echo "wine_run: windows now"
        xwininfo -root -tree 2>/dev/null | grep -E '"Tiberian Sun"' | sed 's/^/  /'
    fi
    if [ -n "$SHOT" ]; then
        ffmpeg -loglevel error -f x11grab -video_size 1024x768 -i :77 \
               -frames:v 1 -y "$SHOT" 2>/dev/null
        echo "wine_run: screenshot $SHOT ($(stat -c%s "$SHOT" 2>/dev/null || echo 0) bytes; a few KB means a blank screen)"
    fi
    wait "$game_pid" 2>/dev/null
    rc=$?
    kill "$xvfb_pid" 2>/dev/null
else
    timeout "$SECS" xvfb-run -a wine GAME.EXE > /tmp/wine_run.log 2>&1
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
if [ -n "$TRACE" ]; then
    echo "  trace: /tmp/wine_run.log ($(wc -l < /tmp/wine_run.log) lines)"
else
    grep -vE "ALSA|snd_|pcm\.c|fixme:ntdll" /tmp/wine_run.log | head -20 | sed 's/^/  /'
fi
