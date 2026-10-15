# Tiberian Sun — Disassembly to Source Reconstruction

## Result

The retail PC release of **Command & Conquer: Tiberian Sun** (Westwood Studios,
1999) has been disassembled and reconstructed into compilable C++ source.

**Verification status: all 42 reconstructed headers compile cleanly**
(`g++ -std=c++03 -fsyntax-only`), both individually and as a combined
translation unit. Every vtable→class binding was independently re-verified
against the binary's RTTI.

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
- PE32 i386, MSVC 6.0, ImageBase `0x400000`, EntryPoint `0x6854e3`
- `.text` 2.7 MB, timestamp 1999-07-28
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

Recovered:
- **788** type descriptors
- **755** class hierarchies with exact base-class displacements
- **842** Complete Object Locators
- **843** vtables bound to their owning class

### 2. Disassembly (`tools/disasm.py`)

Function boundaries were seeded from the entry point, vtable entries, and MSVC
prologue patterns, then extended by call-graph traversal with seed-boundary
termination.

Recovered:
- **9,057** functions
- **275,283** instructions
- Per-function callees and referenced strings

### 3. Schema recovery

**1,209** strings referenced by code were extracted, including **988
`rules.ini` field names** (`TiberiumChainReaction`, `HalfDamageSmokeLocation1`,
`Capturable`, `Unsellable`, `CustomRotor`, ...). These are the original struct
member names, recovered verbatim.

### 4. Source reconstruction

Three parallel workstreams reconstructed the class hierarchy, the
locomotion/command/AI subsystems, and the INI/map/display layer. Every
declaration cites its evidence (RTTI name, vtable VA, INI key, or function VA).

## Deliverables

```
re/
  bin/GAME.EXE              the target binary
  tools/rtti.py             RTTI recovery
  tools/demangle.py         MSVC name demangler
  tools/disasm.py           function/call-graph recovery
  out/classes.json          755 classes, 1291 edges, 843 vtables
  out/hierarchy.txt         readable class hierarchy
  out/vtables.txt           vtable -> class map
  out/functions.json        9057 functions with instructions
  out/ini_keys.json         1209 referenced strings
  src/include/*.h           42 reconstructed headers (7094 lines)
  src/include/SCHEMA.md     1374-row INI key -> class -> type table
  src/include/INDEX.md      core hierarchy index
  src/include/LOCOMOTION_INDEX.md
  src/include/MAP_INDEX.md
  build/tu_all.cpp          combined compile test
  build/shim/               minimal windows.h + shared prelude
```

## Verification

| Check | Result |
|---|---|
| All headers compile standalone | 42/42 pass |
| Combined translation unit compiles | pass |
| vtable→class bindings re-verified against RTTI | 17/17 correct |
| INI reader claims verified against binary strings | confirmed |
| Constructor addresses verified as real functions | confirmed |

Evidence density in the reconstructed source: 2,987 address citations,
802 INI key citations, 50 RTTI name citations. 975 virtuals that could not be
semantically identified are honestly named `vfunc_N` rather than guessed, and
247 items are explicitly marked `/* ? */`.

## Known gaps

- `RulesClass` has no RTTI record (it is not polymorphic); it is identified
  only by its reader functions, so its member offsets are not isolated.
- `TechnoClass` virtuals 64–227 and `ObjectClass` virtuals 64–104 have known
  target addresses but no standalone function records.
- Nested gadget classes (`SidebarClass::StripClass`, `RadarClass::RTacticalClass`)
  have no located constructor, so their member offsets are unresolved.
- Data-member *offsets* are exact where recovered; names are only assigned
  where an INI key or string provides evidence.

## Reproducing

```bash
cd re
python3 tools/rtti.py bin/GAME.EXE out      # RTTI recovery
python3 tools/disasm.py bin/GAME.EXE out    # disassembly
g++ -std=c++03 -fsyntax-only -Isrc/include -Ibuild/shim build/tu_all.cpp
```
