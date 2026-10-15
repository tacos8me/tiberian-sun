#!/usr/bin/env bash
# Full reconstruction pipeline: binary -> recovered data -> C++ headers.
#
# Reproduces the entire reconstruction from bin/GAME.EXE. Every stage is
# deterministic. Run from the re/ directory.
#
# Usage:  ./tools/pipeline.sh [--verify-only]
set -euo pipefail

cd "$(dirname "$0")/.."
BIN=bin/GAME.EXE
OUT=out
SRC=src/include

if [ ! -f "$BIN" ]; then
  echo "error: $BIN not found" >&2
  exit 1
fi

if [ "${1:-}" != "--verify-only" ]; then
  echo "== 1/9 RTTI recovery =="
  python3 tools/rtti.py "$BIN" "$OUT"

  echo "== 2/9 disassembly =="
  python3 tools/disasm.py "$BIN" "$OUT"

  echo "== 3/9 class manifest =="
  python3 tools/gen_manifest.py "$BIN" "$OUT"

  echo "== 4/9 direct-base reduction =="
  python3 tools/reduce_bases.py "$OUT"

  echo "== 5/9 data members =="
  python3 tools/gen_members.py "$BIN" "$OUT"

  echo "== 6/9 member naming =="
  python3 tools/name_members.py "$BIN" "$OUT"

  echo "== 7/9 signatures =="
  python3 tools/gen_signatures.py "$BIN" "$OUT"

  echo "== 8/9 header generation =="
  python3 tools/gen_headers.py "$BIN" "$OUT" "$SRC"

  echo "== 9/9 prelude reconciliation =="
  python3 tools/reconcile_prelude.py "$SRC"
  python3 tools/make_selfcontained.py "$SRC" build/shim
fi

echo
echo "== verification =="
python3 tools/verify.py "$SRC" build/shim
