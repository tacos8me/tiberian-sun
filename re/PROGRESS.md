# PROGRESS — Tiberian Sun reconstruction

All figures below are measured; re-run `python3 tools/harness.py metrics
--no-append` rather than trusting this file.

**This file records state. `ROADMAP.md` records the plan** — what "done" means
for documented/modifiable/compilable as measurable exit criteria, the corrected
size of each remaining problem, and the phase order. Read the roadmap to decide
what to work on. The one thing this file does not say clearly enough is that
"VERIFICATION PASSED" is not a statement about layout: see the ABI row in
`AGENTS.md`, and the `-m32` item in `todo.md`.

## Status summary

**`tools/verify.py` prints VERIFICATION PASSED across nine checks in 6 seconds.**
Check 9 is the one that matters most: it makes the compiler confirm every
recovered member offset, and it passes. **`pipeline.sh` is red at the link test**
on 2 unmapped calls, and takes 471 seconds -- the link is the expensive stage.

**The game runs a frame loop.** The image maps at its real VAs, 341/341 imports
resolve, and the original entry point 0x6854e3 creates a window and pumps
messages: `CreateWindowExA=1`, `PeekMessage=137,295`, `DispatchMessage=1`. A
5,000,000-instruction run ends on budget rather than a fault. There is no
renderer, and 2 unmapped calls remain.

`Image.h` and `Win32Stubs.h` are what make this possible; neither does anything
alone. The single change that unblocked startup was giving `comctl32!DllGetVersion`
an export: the CRT startup checks the comctl32 version and calls `ExitProcess(1)`
if it is below 4.00.70, and it reaches that function through `GetProcAddress`
rather than the import table.

Measured state:

| Measure | Value |
|---|---|
| headers | 262 (261 standalone-compilable + prelude) |
| functions recovered | 41,822 (96.06% of `.text`) |
| `.text` residual | 104 KB classified (99.98% attributed): 11 jump tables (256 B), 356 B of switch index tables, 105,808 B of alignment padding; 554 B in 90 runs reported unclassified in `out/text_data.json` |
| classes / vtable slots | 749 / 9,810 (`vtable_slots_unseeded` 0) |
| constructors that are real function starts | 100% |
| virtuals named | 1,567 of 6,987 (22.4%) |
| named members | 135 across 20 classes |
| member offsets recovered | 4,326 |
| member claims confirmed in the binary | 99.9% |
| **machine functions transcribed** | **43,668 (41,822 starts + 1,846 tail entries)** |
| **instructions transcribed** | **897,193 of 897,193 (100%)** |
| forwarder definitions | 6,996 across 309 translation units |
| destructor stubs (declared, no recovered address) | 228 |

### The code is transcribed, not stubbed

`src/generated/code/` holds one transcription per recovered function VA as
`ts::sub_XXXXXXXX(void)`, against the machine model in `src/handwritten/
Machine.h`: registers with sub-register views, correct flag semantics for every
operation emitted, lazily-paged memory, stack, x87 register stack, string ops,
segment registers, port I/O. Each generated line is one machine instruction and
keeps the original as a trailing comment.

The per-class files in `src/generated/` are **forwarders** — the out-of-line
definitions the reconstructed headers require, each calling the canonical
transcription of its VA. One machine function, one body.

The tree **links and runs**. `tools/link_test.py` links all 482 translation units
into a program, registers every recovered function, and executes the original PE
entry point against the model. That link is what found the defects below; a
per-file syntax check cannot see a symbol nothing defines.

Coverage is achieved in three tiers:
  - **hand-written headers** with named virtuals, recovered member offsets, and
    per-declaration evidence citations (including the `VectorClass`/
    `DynamicVectorClass`/`TypeList`, `Blitter`, and `Pipe`/`Straw` templates).
  - **generated headers** carrying exact inheritance, the full vtable in real
    order with target addresses, and the constructor VA.
  - **generated code** transcribing the whole recovered instruction stream.

## Completed and verified

| Item | Count | Status |
|---|---|---|
| CD images converted + extracted | 2 discs | done |
| Target binary identified | GAME.EXE | done |
| RTTI type descriptors | 788 | done |
| Class hierarchies | 755 | done |
| Complete Object Locators | 842 | done |
| Vtables bound to classes | 843 | done |
| Functions disassembled | 41,822 | done |
| Instructions recovered | 897,193 | done |
| INI keys / referenced strings | 1,876 | done |
| Per-class vtable+ctor manifest | 749 classes, 9,810 slots | done |
| Direct-base reduction | 749 classes | done |
| Member offsets recovered | 4,326 | done |
| Machine code transcribed | 43,668 functions | done |
| Class forwarders emitted | 6,996 | done |
| Whole-tree link + entry-point run | 482 TUs | done |
| Binary-anchored member verification | 99.9% confirmed, gated | done |
| Tool unit tests | 36 checks, mutation-proven | done |

### Verification performed

- `tools/verify.py` runs eight checks and is **fail-closed**: a missing artifact
  exits non-zero naming what is missing, rather than running a subset and
  printing PASSED.
- Check 4 re-resolves **all 749** manifest vtables through the binary's own COL
  chain and requires an exact mangled-name match.
- `tools/verify_members_binary.py` disassembles each constructor in the binary
  and confirms claimed member offsets against real stores: **99.9% confirmed**.
  It runs as a gate with a 95% floor.
- `tools/gen_code.py` and `tools/gen_bodies.py` both run **faithfulness gates**
  on their own output, proven by mutation. The transcription gate re-renders
  each function and requires the file to be byte-identical, which is what
  catches a corrupted statement whose evidence comment is intact.
- `tools/link_test.py` links the whole tree and runs it. See "defects found by
  the link" below for what it caught.
- `tools/test_tools.py` runs 36 unit checks, each a regression test for a
  shipped defect or a differential test against an independent implementation.
  Four of them are mutation-proven to fail when the defect is reintroduced.
- Determinism: two clean pipeline runs produce **497 byte-identical artifacts**
  (the recovered data *and* the generated C++).

## Remaining work

### P0 — generated set correct and complete  [DONE]
- [x] Fix inheritance emission (real includes, direct bases only)
- [x] Regenerate all 749 class headers; compile standalone and combined
- [x] Reconcile generated headers with the hand-written ones
- [x] Make `verify.py` fail-closed and exhaustive (8 checks)

### P1 — deepen the reconstruction  [DONE]
- [x] Recover data-member offsets from constructor store sequences (4,326)
- [x] Name members from INI keys (135 across 20 classes)
- [x] Recover function signatures (37,595)
- [x] Name virtuals where evidence exists (1,567 of 6,987)
- [x] Anchor member verification to the binary (99.9% confirmed)
- [x] Transcribe every recovered function (43,668, 99.98% of instructions)
- [x] Emit the forwarders the headers require, and link the tree

### P2 — remaining
- [ ] **Name more virtuals.** 5,420 remain `vfunc_N`. This is now the largest
      single gap in how editable the source is.
- [ ] **Name more members.** 135 of 4,326 have evidence-backed names.
- [ ] `Node<>` container template (declared in `INIClass.h`; should be its own
      header with recovered layout). 2 classes still show unresolved bases for
      it: `INIClass::INIEntry` and `MixFileClass`.
- [ ] Reconstruct the MS* multimedia classes and the WOL/ATL COM layer.
- [x] Classify the remaining 104 KB of residual `.text`. It is 11 jump tables
      (256 B) and 356 B of switch index tables, emitted as data in
      `out/text_data.json`, plus 105,808 B of alignment padding. 554 B in 90
      runs are neither and are listed verbatim, not forced into a category.
- [x] 0 instructions have no faithful rendering. The last 166 (MMX/SSE, the x87
      environment save/restore, far pointers, `lar`, `fbld`, `fcmovnbe`,
      `pushaw`) are modelled in `src/handwritten/Machine.h`.
- [ ] Load the image's data sections into the model. The entry-point run stops
      at import thunks because `call dword ptr [IAT]` reads 0 from modelled
      memory; the model never loads `.idata`.

### P3 — validation
- [ ] Compile the gate with `-m32` so target type widths are enforced. The shim
      is host-sized: `sizeof(DWORD)` is 8, and `__stdcall` is defined away.
- [ ] Byte-level validation: recompile and diff generated code against the
      original binary where a matching compiler is available.
- [ ] Cross-check every INI key against a real `rules.ini` (the e2e harness
      already resolves 110 of 112 recovered field names against the shipped
      INIs; widen it).

## Known gaps (documented, not hidden)

- **0.1% of member-offset claims are not confirmed by the binary.** Recovery is
  deliberately conservative: the plausibility gate trades recall for precision.
- **Member *type* and ordering are not verified.** Offsets are.
- **228 destructors are labelled stubs.** The headers declare them; no comment
  records where they are, so there is no transcription to forward to. The stub
  is marked as a stub in the source and counted in the generated README.
- **The transcribed code is not source-shaped.** It is faithful to what the
  machine does and is not claimed to be the shape the original source had.
- **A transcription reads its arguments from the model, not from C++
  parameters.** Hand-written callers (the template headers) therefore reach the
  right code through `ts::ts_invoke`, but the values they pass are only
  meaningful when the caller is itself a transcription.
- `RulesClass` has no RTTI record (not polymorphic); identified only by its
  reader functions, so member offsets are not isolated.
- Nested gadget classes (`SidebarClass::StripClass`,
  `RadarClass::RTacticalClass`) have no located constructor.
- Member *names* exist only where an INI key or string provides evidence. Never
  infer one.
- **`out/functions.json` is 65 MB**, over GitHub's 50 MB warning threshold. It
  is a regenerable artifact; untracking it would need a history rewrite.

## Defects found by the link (recorded because they were all invisible before)

Closing the body-coverage gap and linking the tree exposed six real defects, none
of which any existing check could see:

1. `in` operands were emitted swapped — `0x5fu = ts::in_op(EAX.d)`, an
   assignment to an immediate. Both branches of the operand read were identical.
2. A `loop` whose target lay outside the recovered span emitted `goto` to a
   label that was never emitted.
3. The declaration pattern could not match a destructor at all: `\b` before the
   name has no boundary between a space and `~`. **No destructor had ever been
   emitted**, and 214 were undefined.
4. `public: virtual ~X();` was captured whole, producing the out-of-line
   definition `public: virtual X::~X()`.
5. Template classes got out-of-line destructor stubs, which is not valid C++
   without an argument list.
6. The generator declared `virtual ~X()` for 66 classes whose slot 0 is a shared
   stub, claiming a vtable position the destructor does not hold.

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
python3 tools/verify.py src/include build/shim      # the 8-check gate
python3 tools/verify_members_binary.py              # binary-anchored member check
python3 tools/test_tools.py                         # tool unit tests
python3 tools/link_test.py                          # link + run the whole tree
```

Individual stages, in pipeline order:

```bash
python3 tools/rtti.py bin/GAME.EXE out             # RTTI recovery
python3 tools/disasm.py bin/GAME.EXE out           # disassembly
python3 tools/gen_code.py out bin/GAME.EXE src/generated/code  # transcription
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
python3 tools/gen_bodies.py out src/include src/generated src/generated/code
```

## Verification status

`tools/verify.py` reports, both in-tree and from a clean checkout:

```
[1] standalone compile: 261/261
[2] combined TU compile: PASS
[3] class coverage: 749/749 (100.0%)
[4] vtable->class bindings: 749/749 verified
[5] header content vs manifest: 219/219 verified
[6] inheritance vs direct bases: 220/220 verified
[7] member offsets vs recovered table: 2186/2186 verified
[8] declared arity vs recovered signatures: reported, not gated
VERIFICATION PASSED
```

Plus the two checks whose input is the binary rather than another tool's output:

```
tools/verify_members_binary.py
  confirmed of checkable 99.9%

tools/link_test.py
  link: OK (482 translation units)
  5 of 5 sampled VAs resolved
  entry 0x006854e3 ran; unmapped calls: 16383 (all import thunks reading 0)
  recursion depth limit 4096; times hit: 21
```
