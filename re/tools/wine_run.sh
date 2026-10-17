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
set -e

ROOT=$(cd "$(dirname "$0")/.." && pwd)
PREFIX=${WINEPREFIX:-$ROOT/../.wine32}
GAMEDIR=${GAMEDIR:-$ROOT/../gamedir}
SECS=90
TRACE=""

while [ $# -gt 0 ]; do
    case "$1" in
        --trace) TRACE="$2"; shift 2 ;;
        --secs)  SECS="$2";  shift 2 ;;
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
timeout "$SECS" xvfb-run -a wine GAME.EXE > /tmp/wine_run.log 2>&1
rc=$?
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
