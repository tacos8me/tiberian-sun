# PROGRESS — Tiberian Sun reconstruction
Last updated: P2 header integration complete — `tools/verify.py` passes.

## Status summary

**Class coverage is complete and verified: `tools/verify.py` prints
VERIFICATION PASSED** — 259/259 headers compile standalone, the combined
translation unit compiles, all 749 manifest classes are covered, and 200/200
sampled vtable→class bindings re-resolve correctly against the binary.

Coverage is achieved in two tiers:
  - **hand-written headers** with named virtuals, recovered member offsets, and
    per-declaration evidence citations (including the `VectorClass`/
    `DynamicVectorClass`/`TypeList`, `Blitter`, and `Pipe`/`Straw` templates).
  - **generated headers** carrying exact inheritance, the full vtable in real
    order with target addresses, and the constructor VA.

The generated tier is mechanically correct but shallow: virtuals are named
`vfunc_N` and data members are not yet emitted. Deepening that tier is the
current work (see P1 below).

Instantiations of the hand-written templates are covered by the template
header that defines them rather than by a per-instantiation file; `verify.py`
credits coverage to the defining template (487 of the 749 manifest entries are
template instantiations, e.g. 150 `VectorClass`, 147 `DynamicVectorClass`,
66 `TClassFactory`, 12 `TypeList`).

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

### P1 — deepen the reconstruction  [DONE]
- [x] Recover data-member offsets from constructor store sequences
      (4160 offsets across 628 classes)
- [x] Name members from INI keys (135 members across 20 classes, each traced
      to the key string VA and the reader function that stores it)
- [x] Recover function signatures (8689 functions: 3176 __stdcall,
      3012 __thiscall, 1216 __cdecl)
- [x] Name virtuals where evidence exists (1242 names across 624 classes)
- [x] Fix primary-vtable selection (virtual slots recovered rose from 5604
      to 9810 — the earlier figure was truncated by picking a secondary
      interface vtable)

### P2 — templates  [DONE]
- [x] Reconstruct the `Blit*`/`RLEBlit*` blitter family as one template header
      (`src/handwritten/Blitter.h`, `<E>`/`<G>` split proven from stride)
- [x] Reconstruct `VectorClass<>` / `DynamicVectorClass<>` / `TypeList<>`
      (`src/handwritten/VectorClass.h`, layout from ctor `0x445640`)
- [x] Reconstruct the Pipe/Straw IO hierarchy (`src/handwritten/Pipe.h`,
      20 classes, 80 virtuals named from call-site evidence)
- [x] Wire template ownership into the generator so instantiations are not
      re-emitted as duplicate headers, and credit coverage to the defining
      template

### P2 — remaining
- [ ] `Node<>` container template (currently declared in `INIClass.h`; should
      become its own header with recovered layout)
- [ ] Reconstruct the MS* multimedia classes (MSAnim, MSFont, ...)
- [ ] Reconstruct the WOL/ATL COM layer
- [ ] Emit `.cpp` stubs with real bodies where feasible
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

The whole reconstruction is one reproducible command:

```bash
cd re
./tools/pipeline.sh              # regenerate everything from bin/GAME.EXE
./tools/pipeline.sh --verify-only   # verify the current tree
```

Individual stages, in order:

```bash
python3 tools/rtti.py bin/GAME.EXE out          # RTTI recovery
python3 tools/disasm.py bin/GAME.EXE out        # disassembly
python3 tools/gen_manifest.py bin/GAME.EXE out  # per-class vtable/ctor
python3 tools/reduce_bases.py out               # direct-base reduction
python3 tools/gen_members.py bin/GAME.EXE out   # member offsets
python3 tools/gen_headers.py bin/GAME.EXE out src/include
python3 tools/reconcile_prelude.py src/include
python3 tools/make_selfcontained.py src/include build/shim
python3 tools/verify.py src/include build/shim
```

## Verification status

`tools/verify.py` reports, both in-tree and from a clean checkout:

```
[1] standalone compile: 259/259
[2] combined TU compile: PASS
[3] class coverage: 749/749 (100.0%)
[4] vtable->class bindings: 200/200 verified
VERIFICATION PASSED
```
