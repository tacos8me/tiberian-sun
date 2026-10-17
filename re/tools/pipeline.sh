#!/usr/bin/env bash
# Full reconstruction pipeline: binary -> recovered data -> C++ headers.
#
# Reproduces the entire reconstruction from bin/GAME.EXE. Every stage is
# deterministic. Run from the re/ directory.
#
# Usage:  ./tools/pipeline.sh [--verify-only]
set -euo pipefail

cd "$(dirname "$0")/.."

# A nested out/out is what you get from running a stage with the output
# directory given as `out` while already inside out/. It happened once and left
# 40MB of stale, wrong artifacts committed for months, because nothing looked at
# them. Fail loudly instead.
if [ -d out/out ]; then
  echo "error: stray out/out/ directory -- a stage was run from the wrong cwd" >&2
  echo "       remove it (rm -rf out/out) and re-run" >&2
  exit 1
fi
BIN=bin/GAME.EXE
OUT=out
SRC=src/include

if [ ! -f "$BIN" ]; then
  echo "error: $BIN not found" >&2
  exit 1
fi

# The target is PE32 i386 and the gate compiles at -m32 (verify.py checks 1, 2
# and 9). Check it HERE, before any stage runs, rather than letting a host-ABI
# build answer a different question: a silent fallback to the host ABI is how
# an unlinkable tree survived here for months. -m32 needs the 32-bit crt and
# libc, so this links a program, not just a syntax check.
if ! echo 'int main(){return 0;}' | g++ -m32 -x c++ - -o /dev/null 2>/dev/null; then
  echo "error: g++ cannot compile and link 32-bit (-m32) code" >&2
  echo "       the target is PE32 i386; the host LP64 ABI is a different ABI" >&2
  echo "       install g++-multilib (Debian/Ubuntu: apt-get install g++-multilib)" >&2
  exit 1
fi

if [ "${1:-}" != "--verify-only" ]; then
  # Start from a clean generated tree: keep only the hand-written headers and
  # the prelude, so headers that are no longer generated do not linger.
  mkdir -p "$SRC"
  find "$SRC" -maxdepth 1 -name '*.h' -delete
  cp src/handwritten/*.h "$SRC"/ 2>/dev/null || true
  echo "== 0/10 clean generated tree =="
  # Test the tools, not just their output. verify.py checks artifacts; these
  # check the code that produces them, and every one is a regression test for a
  # defect that shipped.
  echo "== 0b/10 tool unit tests =="
  python3 tools/test_tools.py
  # The layout gate must be able to fail AND able to pass: the selftest runs it
  # against a fixture whose offsets are known, once correct and once displaced.
  python3 tools/gen_layout_check.py --selftest

  echo "== 1/10 RTTI recovery =="
  python3 tools/rtti.py "$BIN" "$OUT"

  echo "== 2/10 disassembly =="
  python3 tools/disasm.py "$BIN" "$OUT"

  echo "== 2b/10 machine code transcription =="
  # One canonical transcription per recovered function VA, plus a registration
  # unit so ts_call() dispatches. Independent of the headers: it is the code,
  # not the declarations.
  python3 tools/gen_code.py "$OUT" "$BIN" src/generated/code

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

  echo "== 8b/10 virtual naming =="
  # name_virtuals.py was previously NOT invoked by any stage, so
  # out/virtual_names.json survived only by accident and every clean
  # regeneration emitted vfunc_N for all 1,213 recoverable names.
  python3 tools/name_virtuals.py "$BIN" "$OUT"
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

# Generate .cpp function bodies for the member functions the reconstructed
# headers declare and the binary supplies both a function record and a
# recovered signature for. Must run AFTER the headers are final, since the
# emitted definitions copy the header declarations verbatim.
echo "== function bodies =="
python3 tools/gen_bodies.py "$OUT" "$SRC" src/generated src/generated/code
# Prove every generated TU compiles standalone against the reconstructed tree.
# bodies_all.cpp is a concatenation of the others, so it is left out of the
# per-file sweep and never compiled twice.
python3 tools/check_compile.py "generated bodies" "src/generated/*.cpp" \
        --exclude bodies_all.cpp

echo "== machine code TUs =="
python3 tools/check_compile.py "transcription TUs" "src/generated/code/*.cpp"

echo "== link the whole tree =="
# A per-file syntax check cannot see a forwarder that names a symbol nothing
# defines; only a link can. This links every transcription and every forwarder
# into one program, then runs the original entry point against the model.
python3 tools/link_test.py

echo
echo "== verification =="
python3 tools/verify.py "$SRC" build/shim

echo
echo "== binary-anchored member verification =="
# This is a GATE, not a report. It is one of only two checks whose input is the
# binary rather than another tool's output, so it is the one that can falsify
# the member data. The floor is set at 95% because the measured rate is 99.9%
# (3,571 of 3,575 claims confirmed, 4 refuted); a drop below it means a
# regression in gen_members.py, not noise.
python3 tools/verify_members_binary.py --fail-under 95

echo
echo "== metrics =="
python3 tools/harness.py metrics
