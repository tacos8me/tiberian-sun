# PROGRESS — Tiberian Sun reconstruction

Last updated: after initial RTTI/disassembly recovery and first header pass.

## Status summary

**Class coverage is now complete: all 749 classes recovered from RTTI have a
header, and the entire set compiles** — 692 headers, 27,026 lines, verified
both standalone (692/692) and as a combined translation unit.

Coverage is achieved in two tiers:
  - **96 classes** have hand-written headers with named virtuals, recovered
    member offsets, and per-declaration evidence citations.
  - **653 classes** have generated headers carrying exact inheritance, the full
    vtable in real order with target addresses, and the constructor VA.

The generated tier is mechanically correct but shallow: virtuals are named
`vfunc_N` and data members are not yet emitted. Deepening that tier is the
current work (see P1 below).

## Completed and verified

| Item | Count | Status |
|---|---|---|
| CD images converted + extracted | 2 discs | done |
| Target binary identified | GAME.EXE | done |
| RTTI type descriptors | 788 | done |
| Class hierarchies | 755 | done |
| Complete Object Locators | 842 | done |
| Vtables bound to classes | 843 | done |
| Functions disassembled | 9,057 | done |
| Instructions recovered | 275,283 | done |
| INI keys / referenced strings | 1,209 | done |
| Per-class vtable+ctor manifest | 749 classes, 5,604 slots | done |
| Direct-base reduction | 749 classes | done |

### Verification performed

- All 17 sampled vtable→class bindings re-resolved through the COL chain
  against the binary's RTTI: **17/17 correct**.
- INI reader claim at `0x61f590` confirmed to reference real `TechnoTypeClass`
  fields (`TypeImmune`, `WalkRate`, `MoveToShroud`, `IsTrain`).
- Constructor addresses confirmed to be real function starts.
- Hand-written headers compile standalone and combined (42/42).

## In progress

**Systematic header generation.** `tools/gen_manifest.py` and
`tools/gen_headers.py` now emit a header for every class with a vtable
(749 classes). Two known defects must be fixed before this replaces the
hand-written set:

1. **Inheritance uses forward declarations.** Generated headers emit
   `class Base;` then `class Derived : public Base` — illegal for an
   incomplete type. Fix: emit real definitions and `#include` the direct base
   headers, or emit a single combined header.
2. **Transitive bases included.** RTTI lists transitive bases; C++ must not
   repeat them. `out/direct_bases.json` has the reduced set — the generator
   must use it.

## Remaining work

### P0 — make the generated set correct and complete  [DONE]
- [x] Fix inheritance emission (real includes, direct bases only)
- [x] Regenerate all 749 class headers
- [x] Compile the full set; fix fallout
- [x] Reconcile generated headers with the hand-written ones (hand-written
      wins where it is richer; generator fills the remaining 653 classes)
- [x] All 692 headers compile standalone and combined

### P1 — deepen the reconstruction  [NEXT]
- [ ] Recover data-member offsets from constructor store sequences
- [ ] Name members from INI keys / strings per class
- [ ] Recover function signatures (arg counts, `__thiscall` vs `__stdcall`)
- [ ] Emit `.cpp` stubs with real bodies where feasible
- [ ] Promote generated virtuals to named ones where strings/callees identify
      their purpose

### P2 — coverage beyond classes
- [ ] Reconstruct the ~58 `Blit*` blitter templates as one template header
- [ ] Reconstruct `TypeList<>` / `VectorClass<>` / `Node<>` container templates
- [ ] Reconstruct the Pipe/Straw IO hierarchy (20 classes)
- [ ] Reconstruct the MS* multimedia classes (MSAnim, MSFont, ...)
- [ ] Reconstruct the WOL/ATL COM layer

### P3 — validation
- [ ] Byte-level validation: recompile and diff generated code against the
      original binary where a matching compiler is available
- [ ] Cross-check every INI key against a real `rules.ini` if obtainable
- [ ] Verify vtable slot counts against the binary for all 749 classes

## Known gaps (documented, not hidden)

- `RulesClass` has no RTTI record (not polymorphic); identified only by its
  reader functions, so member offsets are not isolated.
- `TechnoClass` virtuals 64–227 and `ObjectClass` virtuals 64–104 have target
  addresses but no standalone function records.
- Nested gadget classes (`SidebarClass::StripClass`,
  `RadarClass::RTacticalClass`) have no located constructor.
- Member *offsets* are exact where recovered; *names* only where an INI key or
  string provides evidence.

## How to resume

```bash
cd re
python3 tools/rtti.py bin/GAME.EXE out
python3 tools/disasm.py bin/GAME.EXE out
python3 tools/gen_manifest.py bin/GAME.EXE out
python3 tools/gen_headers.py bin/GAME.EXE out src/include
g++ -std=c++03 -fsyntax-only -Isrc/include -Ibuild/shim build/tu_all.cpp
```
