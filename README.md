# Tiberian Sun — Disassembly to Source

A reverse-engineering project that takes the retail PC release of
**Command & Conquer: Tiberian Sun** (Westwood Studios, 1999) and reconstructs
it as compilable, documented C++ source.

## Status

The reconstructed source **compiles**: 259/259 headers compile standalone, the
combined translation unit passes, and 303 generated translation units containing
6,668 function bodies compile with `-std=c++03`.

It does **not** yet reproduce behaviour. The bodies are empty definitions
carrying the recovered instruction listing as a comment — no control flow or
expression is invented, because the binary does not carry enough information to
justify one. What exists is a verified structural skeleton, not a working game.

| Measure | Value |
|---|---|
| classes / vtable slots | 749 / 9,810 (all slots seeded) |
| constructors that are real function entry points | 100% |
| functions recovered | 23,882 (65.6% of `.text`) |
| `.text` still unattributed | 912 KB |
| function bodies emitted | 6,668 across 303 translation units |
| virtuals named | 1,285 of 7,140 (18.6%) |
| member offsets recovered | 3,575 (99.9% confirmed against the binary) |
| named members | 135 across 20 classes |
| verification gate | 7 checks, fail-closed, passing |

**Measure, don't trust prose.** `python3 tools/harness.py metrics` prints the
real state; these figures drift as the tools improve.

## Verifying

```bash
cd re
./tools/pipeline.sh                            # regenerate everything, then verify
python3 tools/verify.py src/include build/shim # the 7-check gate
python3 tools/verify_members_binary.py         # re-derives offsets from the binary
python3 tools/harness.py determinism --run     # two clean runs, byte-diffed
```

## Running against real game data

`realdata/` holds `rules.ini` and `ai.ini` extracted from the retail archive.
The reconstruction is driven with them end to end:

```bash
python3 tools/e2e_rules.py ../realdata/rules.ini ../realdata/ai.ini
```

Of the 135 member names recovered from the binary's string table, **115 (85.2%)
resolve to real keys in shipped data** — the first evidence that the recovered
names, offsets and reader functions are correct against something outside the
project's own artifacts. Sample output:

```
BuildingTypeClass +1276 Adjacent = '2'   [GATECH]       reader 0x43f250
BulletTypeClass   +292  Airburst = 'yes' [MultiMissile] reader 0x445ba0
ObjectTypeClass   +104  Strength = '230' [HVR]          reader 0x572610
```

The 20 unresolved names are understood: `CustomRotor`, `Unsellable` and
`Degenerates` appear only in *comments* in `ai.ini` because `art.ini`'s bodies
are compressed in the archive. The strings exist in the binary, so the recovery
is faithful — the dataset simply cannot confirm them.

The gate is fail-closed: a missing artifact exits non-zero naming what is
missing, rather than running a subset of checks and reporting success. Only two
checks are anchored to the binary itself; the rest compare the tree against
recovered data, so they can agree with each other while both are wrong. That
distinction is documented in [AGENTS.md](AGENTS.md) and matters when reading
`VERIFICATION PASSED`.

## What was recovered

- **755** classes with exact inheritance, from intact MSVC 6 RTTI
- **843** vtables bound to their owning class, re-resolved exhaustively through
  the binary's own Complete Object Locator chain
- **23,882** functions / 498,869 instructions
- **3,575** member offsets, each confirmed against a real store instruction in
  the binary (99.9%)
- **1,719** referenced strings, including `rules.ini` field names — the original
  struct member names, recovered verbatim

## Layout

```
re/
  bin/GAME.EXE        target binary
  tools/              recovery, generation and verification tooling
  out/                recovered data (JSON)
  src/include/        reconstructed C++ headers
  src/generated/      generated function bodies
  src/handwritten/    evidence-backed headers that take precedence
  build/              compile harness
  REPORT.md           results
  PROGRESS.md         live status and remaining work
todo.md               prioritized work list
```

## Method

The binary is stripped of symbols but retains complete MSVC 6 RTTI. The
`type_info` and Complete Object Locator layouts were determined empirically from
the binary rather than assumed, and a recursive MSVC name demangler was written
for the observed mangling grammar. Every declaration in the reconstructed source
cites its evidence: an RTTI descriptor address, a vtable address, a constructor
address, an INI key string, or a function address.

Uncertainty is marked explicitly (`/* ? */`, `vfunc_N`) rather than guessed at,
and known gaps are recorded rather than hidden — see the caveats at the end of
[todo.md](todo.md) and `PROGRESS.md`.

## Contributing / working on this

Read [AGENTS.md](AGENTS.md) first — ground rules, toolchain, and the technical
facts about the binary. [todo.md](todo.md) is the prioritized work list.

Two operational notes learned the hard way: worktree-isolated agents must
**commit** their own work or a timed-out workflow loses it, and a change to a
generator can break the tree in ways no single agent owns.

## Legal

Tiberian Sun and its assets are © 1999 Electronic Arts / Westwood Studios.
This repository contains only reconstructed source and derived data; the
original game binaries are included solely as the reference target.
