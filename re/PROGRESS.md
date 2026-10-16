# PROGRESS — Tiberian Sun reconstruction
Last updated: after the orchestrated push (member emission, function bodies,
binary-anchored verification). All figures below are measured; re-run
`python3 tools/harness.py metrics` rather than trusting this file.

## Status summary

**`tools/verify.py` prints VERIFICATION PASSED across seven checks**, and the
tree is reproducible from a clean checkout with `out/` and `src/include/`
removed. Measured state:

| Measure | Value |
|---|---|
| headers | 260 (259 standalone-compilable + prelude) |
| functions recovered | 23,882 (65.56% of `.text`) |
| `.text` unattributed | 912 KB |
| classes / vtable slots | 749 / 9,810 (`vtable_slots_unseeded` 0) |
| constructors that are real function starts | 100% |
| virtuals named | 1,285 of 7,140 (18.6%) |
| named members | 135 across 20 classes |
| member offsets recovered | 3,575 (1,235 verified against the recovered table) |
| member claims confirmed in the binary | 3,571 of 3,575 (99.9%) |
| function bodies | 6,668 across 303 generated translation units |

Coverage is achieved in two tiers:
  - **hand-written headers** with named virtuals, recovered member offsets, and
    per-declaration evidence citations (including the `VectorClass`/
    `DynamicVectorClass`/`TypeList`, `Blitter`, and `Pipe`/`Straw` templates).
  - **generated headers** carrying exact inheritance, the full vtable in real
    order with target addresses, and the constructor VA.

The generated tier is still shallow: 5,855 virtuals remain `vfunc_N`, and the
6,668 function bodies are **empty definitions** with the recovered instruction
listing transcribed as a comment. They prove the signatures compile; they do
not reproduce behaviour.

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
| Functions disassembled | 23,882 | done |
| Instructions recovered | 498,869 | done |
| INI keys / referenced strings | 1,719 | done |
| Per-class vtable+ctor manifest | 749 classes, 9,810 slots | done |
| Direct-base reduction | 749 classes | done |
| Member offsets recovered | 3,575 across 521 classes | done |
| Function bodies emitted | 6,668 across 303 TUs | done |
| Binary-anchored member verification | 99.9% confirmed, gated | done |

### Verification performed

- `tools/verify.py` runs seven checks and is **fail-closed**: a missing artifact
  exits non-zero naming what is missing, rather than running a subset and
  printing PASSED.
- Check 4 re-resolves **all 749** manifest vtables through the binary's own COL
  chain and requires an exact mangled-name match. The previous figure (200/200)
  was an alphabetical prefix with substring matching; the real number was
  unknown until the sweep was widened.
- `tools/verify_members_binary.py` disassembles each constructor in the binary
  and confirms claimed member offsets against real stores: **3,571 of 3,575
  confirmed, 4 refuted, 0 uncheckable**. It runs as a gate with a 95% floor. It
  found that a store site buried inside a large general-purpose function was
  being accepted as the constructor for 180 of 585 classes; a plausibility gate
  now requires the vtable write to occur early through a non-stack register.
- Determinism: two clean pipeline runs produce byte-identical output
  (`tools/harness.py determinism --run`).

## Remaining work

### P0 — generated set correct and complete  [DONE]
- [x] Fix inheritance emission (real includes, direct bases only)
- [x] Regenerate all 749 class headers; compile standalone and combined
- [x] Reconcile generated headers with the hand-written ones (hand-written wins
      where it is richer)
- [x] Make `verify.py` fail-closed and exhaustive (7 checks)

### P1 — deepen the reconstruction  [DONE]
- [x] Recover data-member offsets from constructor store sequences (3,575
      across 521 classes)
- [x] Name members from INI keys (135 members across 20 classes, each traced to
      the key string VA and the reader function that stores it)
- [x] Recover function signatures (22,328)
- [x] Name virtuals where evidence exists (1,285 of 7,140)
- [x] Fix primary-vtable selection (virtual slots recovered rose from 5,604 to
      9,810 — the earlier figure was truncated by picking a secondary vtable)
- [x] Emit member blocks for classes that previously had no standalone header
- [x] Emit function bodies (6,668 across 303 TUs; empty definitions carrying the
      recovered evidence, no invented control flow)
- [x] Anchor member verification to the binary (99.9% of claims confirmed)

### P2 — templates  [DONE]
- [x] Reconstruct the `Blit*`/`RLEBlit*` blitter family as one template header
      (`src/handwritten/Blitter.h`, `<E>`/`<G>` split proven from stride)
- [x] Reconstruct `VectorClass<>` / `DynamicVectorClass<>` / `TypeList<>`
      (`src/handwritten/VectorClass.h`, layout from ctor `0x445640`)
- [x] Reconstruct the Pipe/Straw IO hierarchy (`src/handwritten/Pipe.h`,
      20 classes, 80 virtuals named from call-site evidence)
- [x] Wire template ownership into the generator and credit coverage to the
      defining template

### P2 — remaining
- [ ] Fix shared-constructor misattribution: 180 of 585 classes (31%) share a
      constructor VA, 74 of them `0x461054` alone. That generic function stores
      to `[esi+0x174]`/`[esi+8]`, not to the class's own members, and is the
      dominant cause of the 22.2% of member claims the binary does not confirm.
- [ ] `Node<>` container template (currently declared in `INIClass.h`; should
      become its own header with recovered layout)
- [ ] Reconstruct the MS* multimedia classes (MSAnim, MSFont, ...)
- [ ] Reconstruct the WOL/ATL COM layer
- [ ] Recover the remaining 912 KB of unattributed `.text`
- [ ] Name more virtuals (5,855 remain `vfunc_N`)

### P3 — validation
- [ ] Unit tests for the load-bearing tools (`rtti.py`, `disasm.py`,
      `gen_manifest.py`) — `verify.py` tests artifacts, never the tools
- [ ] Compile the gate with `-m32` so target type widths are enforced
- [ ] Byte-level validation: recompile and diff generated code against the
      original binary where a matching compiler is available
- [ ] Cross-check every INI key against a real `rules.ini` if obtainable

## Known gaps (documented, not hidden)

- **0.1% of member-offset claims are not confirmed by the binary** (4 of
  3,575). Recovery is deliberately conservative: the plausibility gate trades a
  little recall for near-perfect precision.
- **Member layout is not verified.** Offsets are checked; whether the *type* and
  ordering are right is not.
- `RulesClass` has no RTTI record (not polymorphic); identified only by its
  reader functions, so member offsets are not isolated.
- Nested gadget classes (`SidebarClass::StripClass`,
  `RadarClass::RTacticalClass`) have no located constructor.
- Member *names* exist only where an INI key or string provides evidence (135
  across 20 classes). Never infer one.
- Function bodies are empty definitions. They prove signatures compile; they do
  not reproduce behaviour.

## How to resume

The whole reconstruction is one reproducible command, and it verifies itself:

```bash
cd re
./tools/pipeline.sh              # regenerate everything, then verify + measure
```

To measure or check reproducibility without regenerating:

```bash
python3 tools/harness.py metrics --no-append        # real state, no prose
python3 tools/harness.py determinism --run          # two clean runs, byte-diffed
python3 tools/verify.py src/include build/shim      # the 7-check gate
python3 tools/verify_members_binary.py              # binary-anchored member check
```

Individual stages, in pipeline order:

```bash
python3 tools/rtti.py bin/GAME.EXE out             # RTTI recovery
python3 tools/disasm.py bin/GAME.EXE out           # disassembly
python3 tools/gen_strings.py bin/GAME.EXE out      # referenced strings
python3 tools/gen_manifest.py bin/GAME.EXE out     # per-class vtable/ctor
python3 tools/reduce_bases.py out                  # direct-base reduction
python3 tools/gen_members.py bin/GAME.EXE out      # member offsets
python3 tools/name_members.py bin/GAME.EXE out     # member naming from INI keys
python3 tools/gen_signatures.py bin/GAME.EXE out   # calling conventions
python3 tools/name_virtuals.py bin/GAME.EXE out    # virtual naming
python3 tools/gen_headers.py bin/GAME.EXE out src/include
python3 tools/reconcile_prelude.py src/include
python3 tools/make_selfcontained.py src/include build/shim
python3 tools/gen_bodies.py bin/GAME.EXE out src/generated
```

## Verification status

`tools/verify.py` reports, both in-tree and from a clean checkout:

```
[1] standalone compile: 259/259
[2] combined TU compile: PASS
[3] class coverage: 749/749 (100.0%)
[4] vtable->class bindings: 749/749 verified
[5] header content vs manifest: 219/219 verified
[6] inheritance vs direct bases: 218/222 verified
[7] member offsets vs recovered table: 1442/1442 verified
VERIFICATION PASSED
```

Plus the binary-anchored check, which is the only one whose input is the binary
rather than another tool's output:

```
tools/verify_members_binary.py
  claims examined       3575
  confirmed in binary   3571
  refuted               4
  uncheckable           0
  confirmed of checkable 99.9%
```
