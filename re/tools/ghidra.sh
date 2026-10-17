#!/bin/bash
# Query the pre-analysed Ghidra project. Seconds per call, no re-analysis.
#   ./tools/ghidra.sh 0x5e7294 0x674be0
set -e
GHIDRA=${GHIDRA:-/opt/ghidra/ghidra_12.1.4_PUBLIC}
ROOT=$(cd "$(dirname "$0")/.." && pwd)
exec "$GHIDRA/support/analyzeHeadless" "/mnt/nvme-1/projects/tiberian/ghidra-proj" TS \
     -process GAME.EXE -noanalysis \
     -scriptPath "$ROOT/tools/ghidra_scripts" \
     -postScript DecompileAt.java "$@" 2>&1 | grep -vE "^INFO|^WARN|^Using|openjdk|OpenJDK"
