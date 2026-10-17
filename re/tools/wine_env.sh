#!/bin/bash
# One command that runs the REAL game with the environment it needs to reach its
# main menu, and screenshots it.
#
#   ./tools/wine_env.sh                 # 100s run, screenshot /tmp/ts.png
#   ./tools/wine_env.sh --secs 60       # any wine_run.sh option is passed through
#   SHOT=/tmp/menu.png ./tools/wine_env.sh
#
# It is a thin wrapper over tools/wine_run.sh --cd ... --screen 640x480x16.
# The two settings are not preferences, they are what the game asks for:
#
#   * Display mode. GAME.EXE calls SetDisplayMode(640x400x16) and then
#     SetDisplayMode(640x480x16) (WINEDEBUG=+ddraw). Xvfb has no mode list, so
#     Wine can only satisfy a request that matches the screen Xvfb was started
#     with; on 1024x768 both calls fail (NtUserChangeDisplaySettings returns -2)
#     and the game exits with "Error - Unable to set the video mode."
#
#   * CD. The game looks for its CD only on a DRIVE_CDROM drive whose volume
#     label is TS%d and whose root contains TS%d.DSK (GAME.EXE 0x471cd0). It
#     opens D:\TS1.dsk once d: is declared cdrom with label TS1. Without it the
#     menu draws black and the game sits there; the trace shows every UI PCX
#     open failing and no D:\TS1.dsk probe.
#
# No SOFTWARE\Westwood\Tiberian Sun install key is required: the game never
# queries it on this path (WINEDEBUG=+reg). It reads RULES.INI/ART.INI/etc. from
# the CD root or from its MIX files.
#
# Missing gamedir, Wine prefix, CD tree or TS1.DSK makes this exit non-zero with
# the path it wanted. It never falls back to a "close enough" environment.
set -e

ROOT=$(cd "$(dirname "$0")/.." && pwd)
GAMEDIR=${GAMEDIR:-$ROOT/../gamedir}
PREFIX=${WINEPREFIX:-$ROOT/../.wine32}
CD=${TSCD:-$ROOT/../work/cd1}
SECS=${SECS:-100}
SHOT=${SHOT:-/tmp/ts.png}
SHOT_DELAY=${SHOT_DELAY:-20}

exec "$ROOT/tools/wine_run.sh" \
    --cd "$CD" \
    --screen 640x480x16 \
    --dismiss \
    --shot "$SHOT" \
    --shot-delay "$SHOT_DELAY" \
    --secs "$SECS" \
    "$@"
