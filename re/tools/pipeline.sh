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
  # Start from a clean generated tree: keep only the hand-written headers and
  # the prelude, so headers that are no longer generated do not linger.
  mkdir -p "$SRC"
  find "$SRC" -maxdepth 1 -name '*.h' -delete
  cp src/handwritten/*.h "$SRC"/ 2>/dev/null || true
  echo "== 0/10 clean generated tree =="
  echo "== 1/10 RTTI recovery =="
  python3 tools/rtti.py "$BIN" "$OUT"

  echo "== 2/10 disassembly =="
  python3 tools/disasm.py "$BIN" "$OUT"

  echo "== 3/10 referenced strings =="
  python3 tools/gen_strings.py "$BIN" "$OUT"

  echo "== 4/10 class manifest =="
  python3 tools/gen_manifest.py "$BIN" "$OUT"

  echo "== 5/10 direct-base reduction =="
  python3 tools/reduce_bases.py "$OUT"

  echo "== 6/10 data members =="
  python3 tools/gen_members.py "$BIN" "$OUT"

  echo "== 7/10 member naming =="
  python3 tools/name_members.py "$BIN" "$OUT"

  echo "== 8/10 signatures =="
  python3 tools/gen_signatures.py "$BIN" "$OUT"

  echo "== 9/10 header generation =="
  # Hand-written headers are the authoritative, richer versions for the classes
  # they cover. Restore them after generation so they always win.
  python3 tools/gen_headers.py "$BIN" "$OUT" "$SRC"
  if [ -d src/handwritten ]; then
    cp src/handwritten/*.h "$SRC"/ 2>/dev/null || true
    cp src/handwritten/*.md "$SRC"/ 2>/dev/null || true
    echo "restored hand-written headers"
  fi

  echo "== 10/10 prelude reconciliation =="
  # Shim headers for types defined inside another hand-written header.
  cat > "$SRC/AbstractTypeClass.h" <<'SHIM'
#pragma once
// AbstractTypeClass is defined in ObjectTypeClass.h (hand-written header).
// This shim exists so generated headers can include it by name.
#include "ObjectTypeClass.h"
SHIM
  cat > "$SRC/GenericNode.h" <<'SHIM'
#pragma once
// GenericNode is defined in INIClass.h (hand-written header).
#include "INIClass.h"
SHIM

  python3 tools/reconcile_prelude.py "$SRC"
  python3 tools/make_selfcontained.py "$SRC" build/shim
fi

# Regenerate the combined translation unit from what actually exists.
python3 - <<'PY'
import glob, os
hs = sorted(os.path.basename(p) for p in glob.glob("src/include/*.h")
            if os.path.basename(p) != "ts_prelude.h")
with open("build/tu_all.cpp", "w") as f:
    f.write("// Translation unit including every reconstructed header.\n")
    f.write("// Proves the whole set is self-consistent, compilable C++.\n")
    for h in hs:
        f.write(f'#include "{h}"\n')
    f.write("int main() { return 0; }\n")
print(f"combined TU: {len(hs)} headers")
PY

echo
echo "== verification =="
python3 tools/verify.py "$SRC" build/shim
