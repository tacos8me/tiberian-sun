#!/bin/bash
# Drive the RETAIL GAME.EXE under Wine through the render path this lane is
# investigating: main menu -> New Campaign -> GDI1 -> OK -> GDI intro movie ->
# ESC -> mission load.  Capture WINEDEBUG=+ddraw and a screenshot at each stage.
#
# This is an oracle script, not a pipeline stage.  It exists because the model's
# own counters (Rendering.h) say what the model's DirectDraw layer did, and the
# only way to know whether that matches the game is to watch the game.
#
#   ./tools/oracle_render_retail.sh [OUTDIR]
#
# Defaults: OUTDIR=/tmp/retail_render, WINEPREFIX=../.wine32,
# GAMEDIR=../gamedir, CD=../work/cd1.  Override with the environment.
#
# What it does NOT do: it does not prove the game reached the mission.  It
# screenshots every stage so the claim can be checked by looking at the frames
# and by grepping the +ddraw log for CreateSurface/Blt/Lock/Flip after the
# campaign dialog.  Stage names in the log: menu, campaign, movie, load1, load2.
set -e

ROOT=$(cd "$(dirname "$0")/.." && pwd)
OUT=${1:-/tmp/retail_render}
PREFIX=${WINEPREFIX:-$ROOT/../.wine32}
GAMEDIR=${GAMEDIR:-$ROOT/../gamedir}
CD=${CD:-$ROOT/../work/cd1}
SECS=${SECS:-240}
DISP=${DISP:-78}

for f in "$GAMEDIR/GAME.EXE" "$CD/TS1.DSK"; do
    [ -e "$f" ] || { echo "oracle_render_retail: missing $f" >&2; exit 1; }
done
for t in Xvfb xdpyinfo xwininfo xdotool ffmpeg wine; do
    command -v "$t" >/dev/null || { echo "oracle_render_retail: missing $t" >&2; exit 1; }
done

mkdir -p "$OUT"

# d: must be a DRIVE_CDROM labelled TS1.  Same setup as tools/wine_run.sh,
# because the game's GetCDClass/Force_CD_Available path requires it.
ln -sfn "$CD" "$PREFIX/dosdevices/d:"
printf 'TS1\n' > "$CD/.windows-label"
WINEPREFIX="$PREFIX" WINEARCH=win32 WINEDEBUG=-all wine reg add \
    "HKLM\\Software\\Wine\\Drives" /v "d:" /d "cdrom" /f >/dev/null 2>&1
WINEPREFIX="$PREFIX" wineserver -k 2>/dev/null || true
sleep 1

Xvfb ":$DISP" -screen 0 640x480x16 >/dev/null 2>&1 &
XVFB_PID=$!
sleep 2
export DISPLAY=":$DISP"
xdpyinfo >/dev/null || { echo "oracle_render_retail: Xvfb :$DISP failed" >&2; kill $XVFB_PID; exit 1; }

shot() { ffmpeg -loglevel error -f x11grab -video_size 640x480 -i ":$DISP" \
                 -frames:v 1 -y "$OUT/$1.png" 2>/dev/null || true; }
smallwin() {
    xwininfo -root -tree 2>/dev/null | awk '
        /"Tiberian Sun"/ {
            if (match($0, /0x[0-9a-f]+/)) id = substr($0, RSTART, RLENGTH);
            if (match($0, /[0-9]+x[0-9]+\+/)) {
                split(substr($0, RSTART, RLENGTH), g, "x");
                if (g[1] + 0 < 500) { print id; exit }
            }
        }'
}
dismiss() {   # press Return on each small modal dialog, N times
    for _ in $(seq 1 "$1"); do
        w=$(smallwin)
        [ -z "$w" ] && break
        xdotool key --window "$w" Return 2>/dev/null || true
        sleep 2
    done
}

export WINEPREFIX="$PREFIX" WINEARCH=win32 WINEDEBUG=+ddraw
cd "$GAMEDIR"
timeout "$SECS" wine GAME.EXE > "$OUT/ddraw.log" 2>&1 &
GAME_PID=$!

cleanup() { kill "$GAME_PID" 2>/dev/null || true; WINEPREFIX="$PREFIX" wineserver -k 2>/dev/null || true; kill "$XVFB_PID" 2>/dev/null || true; }
trap cleanup EXIT

sleep 20
dismiss 8
echo "oracle_render_retail: startup dialogs dismissed; windows:"
xwininfo -root -tree 2>/dev/null | grep -E '"Tiberian Sun"' | sed 's/^/  /' || true

# The menu is only up after the startup movies have finished.  Measured: at 32 s
# the screen is still a movie frame; at 60 s it is the GDI/Nod wheel.  Wait for
# the wheel, do not guess from the startup-dialog count.
sleep 40
shot menu
echo "oracle_render_retail: menu captured"

# New Campaign: the button's screen rect is (223,207)-(418,236) (todo.md).
xdotool mousemove 320 220 click 1
sleep 3
shot campaign
dismiss 2
sleep 3
shot campaign_after_ok

# The GDI intro movie starts here; ESC skips it (the model's esc_skip script).
sleep 12
shot movie
xdotool key Escape
sleep 2
shot after_esc

# Mission load.  Screenshot twice: the load is long in the model, and the
# retail frame at these points is what the model's frames should be compared to.
sleep 30
shot load1
sleep 30
shot load2

echo "oracle_render_retail: done; screenshots and ddraw.log in $OUT"
