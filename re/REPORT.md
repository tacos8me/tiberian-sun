# Tiberian Sun — Disassembly to Source Reconstruction

## Result

The retail PC release of **Command & Conquer: Tiberian Sun** (Westwood Studios,
1999) has been disassembled and reconstructed into a compilable C++ class
declaration tree.

**Verification status: `tools/verify.py` prints `VERIFICATION PASSED` with seven
checks.** 259 of the 260 headers under `src/include/` compile standalone
(`g++ -std=c++03 -fsyntax-only`; `ts_prelude.h` is the shared prelude and is
included rather than compiled on its own), and the combined translation unit
`build/tu_all.cpp` compiles. Every vtable→class binding in the class manifest
was re-verified by re-reading the binary's own RTTI Complete Object Locator
chain.

**What that does *not* mean is stated under [Verification](#verification)
below.** In short: the gate proves the declaration tree compiles and that the
manifest's vtable bindings match the binary. It does not prove the disassembly
is correct, it does not verify data-member layout anywhere, and there are still
**zero `.cpp` function bodies** — the project reproduces *structure*, not
*behaviour*.

## Input

The working directory contained two raw CD images, not an installed game:

| File | Format | Size |
|---|---|---|
| `ts1.bin` / `ts1.cue` | MODE2/2352 raw CD image | 740 MiB |
| `ts2.bin` / `ts2.cue` | MODE2/2352 raw CD image | 739 MiB |

Both discs were converted to ISO 9660 (stripping the 24-byte sector headers)
and extracted. Both discs carry an identical `INSTALL/GAME.EXE`, so there is
exactly one build to reverse.

**Target:** `GAME.EXE`
- SHA256 `2941b01998da0cd2bb0b7db30125d4130e3876a74d6cc5588b630c6d5fac5a12`
  (`sha256sum bin/GAME.EXE`)
- PE32 i386, MSVC 6.0, ImageBase `0x400000`, EntryPoint `0x6854e3`
- `.text` 2,712,101 bytes (2.6 MiB), PE timestamp 1999-07-29 01:52:58 UTC
- Stripped of symbols, but **full MSVC RTTI is present**
- Original source path embedded in the binary: `D:\Projects\TSun\Code\`

## Method

### 1. RTTI recovery (`tools/rtti.py`, `tools/demangle.py`)

The binary retains complete MSVC 6 RTTI. The `type_info` layout was determined
empirically from the binary:

```
+0x00  vftable ptr -> type_info vftable (0x6a7d04)
+0x04  spare
+0x08  inline mangled name
```

A recursive MSVC name demangler was written to handle the observed grammar
(`?$Template@Arg@@`, `AV`/`AU`/`BV`/`BU` type codes, pointer/reference args).

Recovered (`python3 tools/rtti.py bin/GAME.EXE out`):
- **788** type descriptors
- **755** class hierarchies with exact base-class displacements
- **842** Complete Object Locators
- **843** vtables bound to their owning class

### 2. Disassembly (`tools/disasm.py`)

Function boundaries were seeded from the entry point, vtable entries, and MSVC
prologue patterns, then extended by call-graph traversal with seed-boundary
termination, plus a gap-recovery pass over already-recorded branch targets.

Recovered (`python3 tools/disasm.py bin/GAME.EXE out`):
- **23,129** functions, covering **63.66%** of `.text` bytes
  (`python3 tools/harness.py metrics --no-append`)
- **572,378** instructions
- Per-function callees and referenced strings
- **963 KB** of `.text` remains unattributed

Function sizes are measured as merged intervals, not summed per-function sizes:
**193** entry points sit inside another recovered body (shared jump tails) and
would be double-counted otherwise.

### 3. Schema recovery

**1,719** strings referenced by code were extracted, of which **1,335** are
INI-key-like `CamelCase` identifiers (`TiberiumChainReaction`,
`HalfDamageSmokeLocation1`, `Capturable`, `Unsellable`, `CustomRotor`, ...).
These are the original struct member names, recovered verbatim.

```
python3 tools/gen_strings.py bin/GAME.EXE out
#   referenced strings: 1719
#   INI-key-like      : 1335
```

`out/ini_keys.json` maps each string to the function VAs that reference it.
Only a fraction is yet mapped onto declared members (see Known gaps).

### 4. Source reconstruction

Class headers are emitted from a per-class manifest (`tools/gen_manifest.py`)
and deepened by three further recovery passes:

- **member offsets** from constructor store sequences
  (`tools/gen_members.py`, `tools/name_members.py`): **4,634** member-offset
  records recovered, **135** named across 20 classes where an INI key or
  string provides evidence;
- **calling conventions and arity** (`tools/gen_signatures.py`): **21,649**
  signatures (`__stdcall` 7,727, `__thiscall` 4,933, `__cdecl` 3,455, and
  5,534 with no convention recovered);
- **virtual naming** (`tools/name_virtuals.py`): **1,285** virtuals named from
  call-site evidence (**18.6%** of the 6,922 `virtual` declarations the harness
  counts). The remaining **5,637** are honestly named `vfunc_N` rather than
  guessed. (`tools/harness.py metrics` reports `virtuals_named` and
  `virtuals_unnamed`.)

Every declaration cites its evidence (RTTI name, vtable VA, constructor VA,
INI key, or function VA). No `.cpp` function bodies exist yet.

## Deliverables

```
re/
  bin/GAME.EXE              the target binary
  tools/                    recovery + generation tooling
  tools/pipeline.sh         one-command full regeneration + verification
  tools/harness.py          metrics + determinism harness
  out/classes.json          755 classes, 1291 edges, 842 locators, 843 vtables
  out/hierarchy.txt         readable class hierarchy
  out/vtables.txt           vtable -> class map (843 entries)
  out/functions.json        23,129 functions with instructions, callees, refs
  out/ini_keys.json         1,719 referenced strings -> referencing functions
  out/class_manifest.json   749 classes, 9,810 virtual slots
  out/members.json          4,634 member-offset records
  out/signatures.json       21,649 signatures
  src/include/*.h           260 headers (259 standalone-compilable + prelude)
  src/handwritten/          45 hand-written, evidence-backed headers
  src/include/SCHEMA.md     1374-row INI key -> class -> type table
  src/include/INDEX.md      core hierarchy index
  src/include/LOCOMOTION_INDEX.md
  src/include/MAP_INDEX.md
  build/tu_all.cpp          combined compile test (includes, no bodies)
  build/shim/               minimal windows.h + shared prelude
```

`src/include/` is a **build artifact of the tools**: `tools/pipeline.sh`
deletes it and regenerates it from `bin/GAME.EXE`, restoring
`src/handwritten/` on top (the hand-written set always wins). Never hand-edit
a header under `src/include/`.

## Verification

`python3 tools/verify.py src/include build/shim` runs seven checks and is
fail-closed: a missing artifact under `out/` makes it exit non-zero naming what
is missing, rather than running a subset and printing `VERIFICATION PASSED`.
Current output:

```
[1] standalone compile: 259/259
[2] combined TU compile: PASS
[3] class coverage: 749/749 (100.0%)
[4] vtable->class bindings: 749/749 verified
[5] header content vs manifest: 219/219 verified
[6] inheritance vs direct bases: 218/222 verified
    (4 hand-written headers diverge; reported, not gated)
[7] member offsets vs recovered table: 10/10 verified (6 classes emit offsets)
VERIFICATION PASSED
```

**How much each check is worth:**

| Check | What it proves | Anchored to |
|---|---|---|
| 1 | every header parses standalone | the compiler |
| 2 | the whole set is self-consistent C++ | the compiler |
| 3 | every manifest class has a defining header | the manifest |
| 4 | every manifest vtable re-resolves to its class through the binary's RTTI COL chain | **the binary** |
| 5 | each header's cited vtable VA and per-slot targets match the manifest | manifest output |
| 6 | emitted base clauses match `out/direct_bases.json` | tool output |
| 7 | emitted member offsets match `out/members.json` | tool output |

Only **check 4** reads the target binary independently. Checks 5–7 compare the
generated tree against other tools' output, so they detect generator
regressions and header/manifest disagreement, but they cannot detect an error
common to a recovery tool and the generator that consumes it. Check 3 has the
same limitation.

**Explicit non-claims:**

- **The disassembly is not verified.** No check re-decodes function boundaries
  against an independent source. Every downstream artifact inherits any
  boundary error in `tools/disasm.py`.
- **Data-member layout is not verified anywhere.** Check 7 compares only the 6
  classes that actually emit offsets (10 offsets total) against
  `out/members.json`, which is itself produced by `tools/gen_members.py` from
  the same constructor scan. Member offsets are exact where recovered and
  their provenance is recorded, but nothing cross-checks them against the
  binary's access patterns.
- **Behaviour is not reproduced.** `build/tu_all.cpp` is 262 lines of
  `#include` plus `int main() { return 0; }`. There are **zero** `.cpp`
  function bodies. The tree proves the declarations are self-consistent, not
  that any recovered function does what the original did.
- Check 6 reports 4 hand-written headers whose base clauses diverge from
  `out/direct_bases.json`. These are legitimate (hand-written headers omit
  transitively-implied bases), so they are reported and not gated.

Evidence density in the reconstructed headers: **9,657** address citations,
**473** quoted INI-key citations, **384** RTTI citations. **239** distinct
virtuals could not be semantically identified and are named `vfunc_N` rather
than guessed; **1,002** items are explicitly marked `/* ? */`.

```
grep -rhoE '0x[0-9a-fA-F]{6,8}' src/include/*.h | wc -l   # 9657
grep -rhoE 'INI "[^"]+"'        src/include/*.h | wc -l   # 473
grep -rho  'RTTI'               src/include/*.h | wc -l   # 384
grep -rhoE 'vfunc_[0-9]+'       src/include/*.h | sort -u | wc -l   # 239
grep -rhoF '/* ? */'            src/include/*.h | wc -l   # 1002
```

## Known gaps

- **No function bodies.** Zero `.cpp` files exist. Bodies are recommended to
  land only after boundary recovery is trustworthy, since every boundary bug
  would be baked into every regenerated body.
- **Member layout is unverified** (see Verification). 4,634 member-offset
  records are recovered but only 10 reach a header through a verified check;
  2,544 records across 394 classes are computed and discarded because those
  classes have no standalone header.
- **~877 INI keys are unmapped.** 1,719 referenced strings are recovered but
  only 135 members across 20 classes are named from them.
- **`RulesClass` has no RTTI record** (it is not polymorphic); it is identified
  only by its reader functions, so its member offsets are not isolated.
- **`Node<>` is still declared inside `INIClass.h`** rather than in its own
  header.
- **The MS\* multimedia classes, the WOL/ATL COM layer, and the nested gadget
  classes are incomplete.** `SidebarClass::StripClass` and
  `RadarClass::RTacticalClass` have constructors located but their member
  offsets are not isolated.
- **Member offsets are exact where recovered; names only where an INI key or
  string provides evidence.** Do not infer a name.
- **`out/out/` is a stray committed duplicate** of `out/` from an earlier
  mistyped invocation; it is not produced by any pipeline stage and can be
  deleted.

## Reproducing

The whole reconstruction is one reproducible command:

```bash
cd re
./tools/pipeline.sh              # regenerate everything from bin/GAME.EXE, then verify
./tools/pipeline.sh --verify-only   # verify the current tree
```

Individual stages, in order (see `tools/pipeline.sh` for the exact sequence):

```bash
python3 tools/rtti.py bin/GAME.EXE out          # RTTI recovery
python3 tools/disasm.py bin/GAME.EXE out        # disassembly
python3 tools/gen_strings.py bin/GAME.EXE out   # referenced strings / INI keys
python3 tools/gen_manifest.py bin/GAME.EXE out  # per-class vtable/ctor
python3 tools/reduce_bases.py out               # direct-base reduction
python3 tools/gen_members.py bin/GAME.EXE out   # member offsets
python3 tools/name_members.py bin/GAME.EXE out  # member naming from INI keys
python3 tools/gen_signatures.py bin/GAME.EXE out
python3 tools/name_virtuals.py bin/GAME.EXE out
python3 tools/gen_headers.py bin/GAME.EXE out src/include
python3 tools/reconcile_prelude.py src/include
python3 tools/make_selfcontained.py src/include build/shim
python3 tools/verify.py src/include build/shim
```

Measure the current state, or check pipeline determinism:

```bash
python3 tools/harness.py metrics --no-append   # all figures quoted above
python3 tools/harness.py determinism --run     # two clean runs must be byte-identical
```
