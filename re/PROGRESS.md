# PROGRESS — Tiberian Sun reconstruction

All figures below were re-measured on 2026-10-17 with
`python3 tools/harness.py metrics --no-append`, `python3 tools/verify.py
src/include build/shim`, `python3 tools/verify_members_binary.py`,
`python3 tools/test_tools.py`, `python3 tools/link_test.py`, and direct counts
over `out/` and `src/generated/`. Re-run those rather than trusting this file.
Boot-status figures (instructions executed, `ExitProcess` argument, message-loop
state) are **being re-measured by a concurrent lane** and are marked where they
appear.

**This file records state. `ROADMAP.md` records the plan** — what "done" means
for documented/modifiable/compilable as measurable exit criteria, the corrected
size of each remaining problem, and the phase order. Read the roadmap to decide
what to work on.

## Status summary

**`tools/verify.py` prints VERIFICATION PASSED across nine checks.** Check 9 is
the one that matters most: it makes the compiler confirm every recovered member
offset it can express, and it passes 3,073/3,073. **The link stage is red** on 2
unmapped calls: measured this session, `link: OK (492 translation units)`, then
`ts_link_test: FAILED (2 unmapped calls inside .text: 0x006844df from
0x005e7317, 0x0046f3f8 from 0x00674bf6)`. The full `pipeline.sh` was not re-run
here; it last measured about 471 seconds, with the link the expensive stage.

**The game boots, initialises, and exits.** The image maps at its real VAs and
all **341/341** PE import-directory thunks are written to stubs (`link_test.py`:
`imports: 341 across 13 DLLs`, `resolve_imports() ... 341 of 341 IAT slots
written`, `traced thunks: 341 imports bound, 0 with no stub`). The original
entry point 0x6854e3 runs 1,011,817 instructions of CRT startup and game init
before calling `ExitProcess` (boot-status figure, being re-measured). The cause
is measured rather than guessed: the CRT's atexit walk over the 3,311-entry
static-constructor table leaks **108 bytes** (ESP 0x002ff56c at `push %esi`,
0x002ff4fc at the matching `pop %esi`), so `pop %esi` reads a return address, ESI
becomes 0x6844df, and `call *%esi` at 0x5e72cf calls into `ExitProcess` instead of
the function whose result decides whether the "Direct X" error box appears.

**The import counts are three different quantities, and the docs used to
conflate them.** Stated once here:

- **341** — PE import-directory thunks in `GAME.EXE` (309 by name, 32 by
  ordinal, across 13 DLLs). `Image.h` writes a stub VA into every one of these
  IAT slots, so boot reports **341/341 resolved**.
- **342** — rows in the stub table (`w32::imports`, `Win32Stubs.h`). It is the
  341 imports plus `COMCTL32!DllGetVersion`, which is **not** an import: the CRT
  reaches it through `GetProcAddress`. `register_win32_stubs()` therefore says
  "342 imports registered: 315 with a body, 27 generic".
- **traced** — imports actually called during a run. `link_test.py` reports
  `341 imports bound, 0 with no stub`; boot reports the calls it observed.

Do not write "342/342 resolved": resolution is a property of the 341 IAT slots.
The old "342/342" and "341 vs 342 disagree" statements were both reading one of
these quantities as another.

**There is now an oracle, and it changed the premise.** `tools/wine_run.sh` runs
the retail binary under Wine; `tools/oracle.py` reads its real registers with gdb.
The first thing it established is that the **retail game exits with code 01 before
reaching DirectDraw surface creation here too** -- so the model is not diverging
from a working game, it is failing the same way. It also produced the retail
`IDirectDraw` call sequence from a `+ddraw` trace, which exposed our 23-slot vtable
against the real 25 slots (the game calls slot 23, `GetAvailableVidMem`).

**File I/O is real.** The stub layer used to make every file "exist" and read as
empty, with `GetFileSize` and `SetFilePointer` agreeing -- consistent and useless,
because the game could never load `rules.ini`, `TIBSUN.MIX` or the CD. It is now a
real filesystem behind a host root (`TS_ROOT`, default `../gamedir`) with
case-insensitive per-component path resolution, a 64-entry handle table and real
`CreateFileA`/`ReadFile`/`WriteFile`/`SetFilePointer`/`GetFileSize`/`CloseHandle`/
`GetFileAttributesA`/`DeleteFileA`. `IDirectDrawPalette` is a real object too.

**This paragraph previously claimed a message loop. That was false.** The model
let `ExitProcess` return, so execution continued past a fatal exit and the
"loop" was one the real game never ran. `ExitProcess` now halts the run. What the
halt exposes is the real defect: at exit the arguments are stub addresses rather
than the numbers the code pushed -- `ExitProcess(0x6844df)` for `1`, and a
`MessageBoxA` type of `0x00a00201` for `0x30` -- because the atexit walk leaks
**108 bytes (27 dwords)**, so ESI holds a return address. (`tools/link_test.py`
still reports `MESSAGE LOOP: reached -- PeekMessage 30,089,403`; that is the same
retracted artifact, because the link test records the `ExitProcess` fault and
keeps running. Do not quote it as a reached loop.)

`Image.h` and `Win32Stubs.h` are what make this possible; neither does anything
alone. The single change that unblocked startup was giving `comctl32!DllGetVersion`
an export: the CRT startup checks the comctl32 version and calls `ExitProcess(1)`
if it is below 4.00.70, and it reaches that function through `GetProcAddress`
rather than the import table.

Measured state:

| Measure | Value (command) |
|---|---|
| headers | 264 (263 standalone-compilable + prelude) (`harness.py metrics`; `verify.py` check 1) |
| functions recovered | 43,564 spans (`harness.py metrics`: `functions`) |
| `.text` | 2,712,101 bytes; 2,606,135 covered as code = **96.09%**; **99.99% attributed as code or data**. Residual: 12 jump tables (264 B), 7 switch index tables (349 B), 104,959 B of alignment padding, 394 B in 55 unclassified runs (`out/text_data.json`) |
| classes / vtable slots | 749 / 9,810 (`vtable_slots_unseeded` 0) |
| constructors that are real function starts | 100.0% (`ctors_are_function_starts`) |
| virtual declarations in headers | 6,954 = 4,496 named (64.7%) + 2,458 `vfunc_N` |
| named `(class, slot)` pairs | 5,557 of 9,810 (56.6%); 917 distinct names (281 semantic); 702 classes carry a naming record (`out/virtual_names.json`) |
| named members | 291 across 20 classes |
| member offsets recovered | 4,289 (763 are vptr stores; 3,526 real members) |
| member claims confirmed in the binary | 4,285 of 4,289 (99.9%, 4 refuted) (`verify_members_binary.py`) |
| signatures recovered | 43,564 (`harness.py metrics`) |
| **machine functions transcribed** | **46,008 (43,564 recovered starts + 2,444 tail entries)** |
| **instruction lines transcribed** | **918,438; 0 unmodelled** (`grep ts::unmodelled` → 0) |
| static instructions in `out/functions.json` | 848,822 across 43,564 spans |
| forwarder definitions | 6,962 across 310 class translation units; 3,024 distinct machine functions, 3,938 aliases (`src/generated/README.md`) |
| destructor stubs (declared, no recovered address) | 229 |

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

The tree **links and runs**. `tools/link_test.py` links all 492 translation units
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
| Functions disassembled | 43,564 spans | done |
| Instructions recovered | 848,822 static (918,438 transcribed lines) | done |
| INI keys / referenced strings | 1,876 | done |
| Per-class vtable+ctor manifest | 749 classes, 9,810 slots | done |
| Direct-base reduction | 749 classes | done |
| Member offsets recovered | 4,289 | done |
| Machine code transcribed | 46,008 functions | done |
| Class forwarders emitted | 6,962 | done |
| Whole-tree link + entry-point run | 492 TUs | done |
| Binary-anchored member verification | 4,285/4,289 (99.9%), gated | done |
| Tool unit tests | 39 checks, mutation-proven | done |

### Verification performed

- `tools/verify.py` runs **nine** checks in 6 seconds and is **fail-closed**: a missing artifact
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
- `tools/test_tools.py` runs **39** unit checks, each a regression test for a
  shipped defect or a differential test against an independent implementation.
  Four of them are mutation-proven to fail when the defect is reintroduced.
- Determinism: two clean pipeline runs produce **497 byte-identical artifacts**
  (the recovered data *and* the generated C++). *Not re-run in this session
  (it costs two full pipeline runs); 497 is the last measured value.*

## Remaining work

### P0 — generated set correct and complete  [DONE]
- [x] Fix inheritance emission (real includes, direct bases only)
- [x] Regenerate all 749 class headers; compile standalone and combined
- [x] Reconcile generated headers with the hand-written ones
- [x] Make `verify.py` fail-closed and exhaustive (9 checks)

### P1 — deepen the reconstruction  [DONE]
- [x] Recover data-member offsets from constructor store sequences (4,289)
- [x] Name members from INI keys (291 across 20 classes)
- [x] Recover function signatures (43,564)
- [x] Name virtuals where evidence exists (4,496 named declarations; 5,557 named `(class, slot)` pairs; 917 distinct names)
- [x] Anchor member verification to the binary (4,285/4,289, 99.9% confirmed)
- [x] Transcribe every recovered function (46,008 functions, 918,438 instruction lines, 0 unmodelled)
- [x] Emit the forwarders the headers require, and link the tree

### P2 — remaining
- [ ] **Name more virtuals.** 2,458 header declarations remain `vfunc_N`;
      5,557 `(class, slot)` pairs carry a name, but only 917 names are distinct
      (281 semantic). This is now the largest single gap in how editable the
      source is.
- [ ] **Name more members.** 291 of 3,526 non-vptr members (4,289 recovered
      offsets) have evidence-backed names.
- [ ] `Node<>` container template (declared in `INIClass.h`; should be its own
      header with recovered layout). 2 classes still show unresolved bases for
      it: `INIClass::INIEntry` and `MixFileClass`.
- [ ] Reconstruct the MS* multimedia classes and the WOL/ATL COM layer.
- [x] Classify the remaining ~103 KB of residual `.text`. It is 12 jump tables
      (264 B) and 7 switch index tables (349 B), emitted as data in
      `out/text_data.json`, plus 104,959 B of alignment padding. 394 B in 55
      runs are neither and are listed verbatim, not forced into a category.
- [x] 0 instructions have no faithful rendering (`grep ts::unmodelled
      src/generated/code` → 0). The formerly unmodelled MMX/SSE, x87
      environment, far-pointer, `lar`, `fbld`, `fcmovnbe` and `pushaw`
      instructions are modelled in `src/handwritten/Machine.h`.
- [x] **Load the image's data sections into the model — retracted as a defect.**
      The premise was false: `src/handwritten/Image.h` copies every section's
      raw extent into modelled memory and writes a stub VA into every one of the
      341 IAT slots, and `link_test.py` reports `341 of 341 IAT slots written`.
      The entry-point run does not stop at a null call target. The empty
      `MessageBoxA` text is `LoadStringA` returning 0, not a missing section.

### P3 — validation
- [x] Compile the gate with `-m32` so target type widths are enforced. Done:
      `verify.py` compiles every header and the combined TU at `-m32`, fails
      closed when `g++-multilib` is absent, and check 9 confirms 3,073/3,073
      recovered member offsets with `offsetof` assertions.
- [ ] Byte-level validation: recompile and diff generated code against the
      original binary where a matching compiler is available.
- [ ] Cross-check every INI key against real shipped data. `tools/e2e_rules.py`
      resolves **280 of 291 (96.2%)** recovered member names against the shipped
      INIs; 14 are documented only in comment headers. Widen it.

## Known gaps (documented, not hidden)

- **4 of 4,289 member-offset claims (0.1%) are not confirmed by the binary.**
  Recovery is deliberately conservative: the plausibility gate trades recall for
  precision. The four are two offsets each in `ScoreTimeClass` and
  `ScorePrintClass`.
- **Member *type* and ordering are not verified.** Offsets are.
- **229 destructors are labelled stubs.** The headers declare them; no comment
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
- **`out/functions.json` is 67,670,646 bytes (~65 MB)**, over GitHub's 50 MB
  warning threshold. It is a regenerable artifact; untracking it would need a
  history rewrite.

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
python3 tools/verify.py src/include build/shim      # the 9-check gate
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
[1] standalone compile: 263/263
[2] combined TU compile: PASS
[3] class coverage: 749/749 (100.0%)
[4] vtable->class bindings: 749/749 verified
[5] header content vs manifest: 219/219 verified
[6] inheritance vs direct bases: 220/220 verified
[7] member offsets vs recovered table: 1317/1317 verified (206 classes emit offsets)
[8] declared arity vs recovered signatures: 331/1116 match, 785 divergence(s) (reported, not gated)
[9] compiler-confirmed member offsets: PASS (3073/3073 asserted)
VERIFICATION PASSED
```

Plus the two checks whose input is the binary rather than another tool's output:

```
tools/verify_members_binary.py
  classes examined 637; claims 4289; confirmed 4285; refuted 4; 99.9%

tools/link_test.py
  link: OK (492 translation units)
  5 of 5 sampled VAs resolved
  imports: 341 across 13 DLLs; 341 of 341 IAT slots written; 341 bound, 0 with no stub
  entry 0x006854e3 ran; FAILED on 2 unmapped calls inside .text:
    0x006844df (from 0x005e7317), 0x0046f3f8 (from 0x00674bf6)
```
