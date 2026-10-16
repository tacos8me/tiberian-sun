# todo.md — Tiberian Sun reconstruction work list

Prioritized, evidence-backed work items. Each states what is wrong, how it was
verified, and what "done" means. Ordered by (value × certainty) ÷ risk.

Status legend: `[ ]` open · `[~]` in progress · `[x]` done.

**Measure before you trust any number here.** Run
`python3 tools/harness.py metrics --no-append`. The figures below were measured
at the commit that last touched this file; prose goes stale, measurements do
not. Where a number is quoted it is because the *change* matters, not because
the value is current.

---

## P0 — correctness of the gate and the tools  [COMPLETE]

### [x] Fix `tools/disasm.py` prologue scan (dead `pats` list)
`find_prologues` defined five prologue patterns and scanned only the hardcoded
`55 8b ec` sequence. Now scans every pattern with a preceding-byte boundary
check, plus frame-less `__thiscall`/`__stdcall` prologues. Coverage 30.5% →
49.2% at the time.

### [x] Fix `tools/disasm.py` vtable seeding truncation
The seeding loop `break`-ed on the first non-`.text` vtable entry, truncating
821 of 843 vtables and leaving `TechnoClass` with 146 of 214 slots unseeded.
Also removed a 64-slot cap affecting 21 classes. `vtable_slots_unseeded` is now
**0**.

### [x] Correct the "constructor" citations
`gen_manifest.py` recorded the vtable-store *immediate* and labelled it a
constructor: 0 of 179 sampled were function starts. Now maps store sites to
enclosing recovered functions. **100% of 2,640 constructor VAs are real
function starts.**

### [x] Make `verify.py` fail-closed and exhaustive
Deleting `out/` previously ran 2 of 5 checks and still printed
`VERIFICATION PASSED`. Now exits non-zero naming missing artifacts. Check 4
covers all 749 entries through the binary's COL chain with exact mangled-name
matching (the old 200/200 was an alphabetical prefix with substring matching).

### [x] Add header-content, inheritance and member-offset checks (5, 6, 7)
Verified by mutation: a fabricated vtable VA, a fabricated slot target, a
mutated base clause and a mutated member offset each fail with a precise
diagnostic.

### [x] Anchor member verification to the binary
`tools/verify_members_binary.py` disassembles each constructor and confirms
claimed offsets against real stores. **3,571 of 3,575 confirmed (99.9%), 4
refuted, 0 uncheckable**, enforced as a gate. This is one of only two checks
whose input is the binary.

### [x] Fix glob-order nondeterminism
Ownership of a template name depended on filesystem enumeration order.
`tools/harness.py determinism --run` now reports 12 artifacts byte-identical
across two clean runs.

### [x] Resolve the orphaned `out/virtual_names.json`
`name_virtuals.py` existed, worked, and was invoked by no stage, so every clean
regeneration lost all 1,213 names. Now pipeline stage 8b, and `gen_headers.py`
fails loudly if the artifact is missing.

### [x] Fix shared stale-block accumulation in `gen_headers.py`
A class already carrying a member banner was skipped, so blocks from earlier
runs survived after the recovery stopped producing those offsets. Now excised
and replaced; re-running is idempotent.

---

## P1 — depth: emit and verify evidence already recovered  [COMPLETE]

### [x] Emit member offsets for classes with no standalone header
2,668 records across 385 classes were computed then discarded. Emitted member
declarations rose from ~1,966 to 2,743.

### [x] Generate `.cpp` function bodies
6,668 bodies across 303 translation units. Bodies are **empty definitions**
carrying the recovered instruction listing, size and calling convention as a
comment — no invented control flow. They prove signatures compile; they do not
reproduce behaviour.

### [x] Recover more unattributed `.text`
Gap-recovery over already-recorded branch targets. 12,784 → 23,882 functions;
`.text` coverage 49.2% → 65.56%.

### [x] Widen check 7 to hand-written headers
It skipped them entirely and reported 6 classes while 2,743 declarations were
emitted. Now 156 classes / 1,442 offsets, with curated names reported rather
than gated.

---

## P2 — the substantive remaining work

### [x] Fix shared-constructor misattribution in `gen_members.py`
180 of 585 classes shared a constructor VA, 74 of them `0x461054` alone -- a
general-purpose function starting mid-flow (`push 0`) that writes through
`[esi+0x174]`, not through the class's own members. Fixed with a plausibility
gate: the vtable write must occur early in the function and through a non-stack
register. Window chosen empirically against the binary-anchored verifier
(12 -> 1,682 claims/0 false; 40 -> 2,658/2; 100 -> 3,184/2; 200 -> 3,571/4;
beyond 200 recall flattens). False claims fell from 1,052 to 4 and the check is
now a gate with a 95% floor.

### [x] Recover the unattributed `.text`
Done: 65.6% -> **96.06%** coverage, 23,882 -> 41,822 functions, 912 KB -> 104 KB
unattributed. The tool recorded gap-recovered bodies' callees but never fed them
back, never followed conditional jumps, and read jump tables only from initial
seeds. Made the worklist a fixpoint. Verified the 1,019 inner entry points are
real: 1,004 are branch/call targets, only 15 unreferenced.

### [x] Declare members at the width the constructor stores
Done. Every member was emitted as a 4-byte `unsigned long`; one written with
`mov byte ptr [eax+0xc], 1` swallowed every offset inside that span. Check 7 went
1,235 -> 2,032 offsets across 165 -> 237 classes.

### [x] Fix the IPersistStream type inconsistency and the COM vtable evidence
Done. AbstractClass.h now matches the binary and the prelude
(HRESULT/__stdcall/ULARGE_INTEGER*); slot 0 is no longer mislabelled a
destructor (0x405790 is a shared E_POINTER stub and slot 0 of 66 classes); slot 6
is declared; LocomotionClass's Load/Save/GetSizeMax citations are corrected.
verify.py gained check 8, reporting declared-vs-recovered arity (478/1172 match,
694 divergences) with the `this`-counting caveat documented.

### [x] Wire the dead naming rules
Done. R9 QueryInterface/AddRef/Release was defined but never called; R2 Read_INI
read an artifact no tool produced. Named virtuals 1,287 -> 1,522 (22.0%) with
out/virtual_name_evidence.json giving every name its rule and signal.
MapSeedClass now reports Width/NumPlayers/... instead of "RandomMap" ten times.

### [x] De-duplicate bodies and add the faithfulness gate
Done directly after W5 failed to commit three times (its large `write` never
executed). Bodies are keyed on the function VA: the first declarer carries the
transcription, every other declaring name becomes an explicit ALIAS. The README
now reports member definitions (6,670), DISTINCT functions (2,845) and aliases
(3,825) instead of one inflated number.

The faithfulness gate is real and PROVEN BY MUTATION: generation asserts every
instruction appears exactly once, in order, with matching bytes; deleting one
instruction from AbstractClass.cpp makes `--check` exit 1 naming
`0x006829a5 (AbstractClass::QueryInterface): 1/4 records match`. Building the
gate exposed two bugs in the gate itself (`\s+` matching newlines, and a
non-unique banner where MapSeedClass::Read_INI is declared twice). All 303
generated TUs compile; the pipeline has `set -euo pipefail`, so it is a hard
stop.

### [x] Fix the gen_headers.py member-block 2-cycle
The "already declared in the class body" set was collected from the whole body,
including the machine block a previous run had appended, so a re-run dropped its
own members and left only a comment. That deleted ~593 declaration lines across
43 headers and flipped check 7 between 2032/237 and 1716/241. The set is now
collected from the body truncated at the machine banner, so only curated
declarations count. Verified idempotent: two consecutive runs give a
byte-identical src/handwritten/. Check 7 is now 2186/2186 across 248 classes.

### [x] gen_headers.py crash outside pipeline stage order
`UnboundLocalError: prelude_owned` (appended to ~line 610, bound ~line 840) --
it crashed whenever run with an already-reconciled ts_prelude.h present. Fixed.

### [x] The slot-0 mislabel exists in ~66 more headers
0x405790 is a shared E_POINTER stub labelled `virtual ~X(); // [0]` in 66
generated and 14 hand-written headers. Fixed at the source: gen_headers.py now
computes which slot-0 addresses are shared by more than one class and, for
those, emits the destructor without claiming a vtable position and names slot 0
for what it is. The hand-written headers were fixed the same way. The 14
destructors that had been forwarding to the stub are now labelled stubs
instead of pointing at the wrong address.

### [ ] Decrypt the encrypted MIX header (deferred, low value)
tools/mix.py now reads the headerless TD/RA1 variant correctly (SETUP.MIX: 61
entries, exact size identity) and fails explicitly on the encrypted TS variant
rather than reporting garbage. The published Westwood RSA/Blowfish scheme was
implemented and the search space explored without a self-consistent table.
Deferred: every INI-based verification works without it.

### [ ] Recover the remaining 104 KB of `.text`
Roughly 95% of it is real code, not padding. Jump tables, SEH handlers and
`.data`-resident code are the identified untouched sources.
*Done when:* unattributed bytes approach padding-only and the residual is
reported with its padding fraction.

### [ ] Load the image's data sections into the machine model
`tools/link_test.py` runs the original PE entry point against the model and it
stops at the first import: `call dword ptr [IAT]` reads 0, because the model's
lazily-paged memory starts empty and nothing ever copies `.idata`/`.rdata` in.
16,383 of the entry point's calls are this. Loading the sections would let the
import table resolve to real addresses and make the run far more meaningful.
*Done when:* the entry-point run reports no call target of 0x00000000.

### [ ] 166 instructions have no faithful rendering
Emitted as `ts::unmodelled(va, "mnem")` rather than approximated: MMX/SSE
(`movq`, `psrlw`, `movd`, `pxor`, `packuswb`, `punpcklbw`, `punpckhwd`,
`psrad`, `pmulhw`), the x87 environment save/restore (`fnsave`, `frstor`,
`fnstenv`, `fldenv`), far pointers (`les`, `lds`) and `lar`. MMX/SSE is the
largest group and would need 64-bit register state in the model.
*Done when:* the count reaches zero, or every remaining mnemonic is argued for
in the generated README.

### [ ] Name more virtuals
5,420 of 6,987 remain `vfunc_N` (22.4% named). Existing rules cover a fifth of
manifest slots; further progress needs new evidence rules, not better plumbing.
Candidate rules: unique string-literal reference, callee-set match against a
named base virtual, slot index against a known MSVC base layout.
*Done when:* the named ratio rises with every new name citing its rule and the
concrete signal it keyed on. Do not loosen a rule to raise the ratio.

### [ ] Mine the unused `rules.ini` keys
1,876 strings recovered; only 135 members named across 20 classes. The
reader-trace mechanism in `name_members.py` works and is proven.
*Done when:* distinct named members rise substantially, each citing key VA and
reader VA.

### [ ] `Node<>` as its own header
Currently declared inside `INIClass.h`; should have its own header with the
layout recovered from the binary.

### [ ] MS* multimedia classes and the WOL/ATL COM layer
Not reconstructed. Both are large surfaces; scope them before starting.

---

## P3 — verification and validation

### [x] Unit tests for the load-bearing tools
`tools/test_tools.py`: 36 checks. Not golden-count assertions -- every one is a
regression test for a defect that actually shipped, or a differential test
against an independent implementation. Four are proven to fail when the defect
is reintroduced (mutating `gen_bodies.DECL`, swapping the `in` operands,
removing the `loop` tail handling, removing the immediate mask each produce
failures naming the right check). Wired into the pipeline as stage 0b.

### [ ] Compile the gate with `-m32`
The shim is host-sized: on this LP64 host `sizeof(DWORD)` is 8 and `sizeof(GUID)`
is 24, against 4 and 16 in the PE32 target, and `__stdcall` is defined away
entirely. So the calling-convention data recovered for 22,328 functions is not
exercised by the build. `-m32 -fsyntax-only` works here and needs no multilib.
*Done when:* a deliberately mis-sized shim type fails the gate.

### [x] Run against real game data
`realdata/` holds the shipped control files extracted from the retail archive
(`rules.ini`, `art.ini`, `ai.ini`, `sound.ini`, `theme.ini`, `BATTLE.INI`,
`MISSION.INI`; byte ranges and section counts in `realdata/README.md`);
`tools/e2e_rules.py` drives the reconstructed model with them and
`tools/extract_realdata.py` regenerates them from the MIX archive. 101 of 112
recovered member names (90.2%) resolve to keys the shipped data sets, and 9
more are named in the files' own comment headers -- 110 (98.2%) confirmed by
either class, reported separately because a comment proves the name exists, not
that a value is read. This is the first external validation of the recovered
names, offsets and readers. The two unconfirmed names are `RandomMap` and
`Inaccuate`, the latter being the binary's own misspelling of `Inaccurate`.

### [x] Declare the base classes two hand-written headers omit
`AnimClass` omitted `StageClass` (RTTI: mdisp 76) and was fixed: StageClass now
has its own header, since its definition lived inside `TechnoClass.h`, which no
other class included. `HouseClass` turned out to be CORRECT -- the checker could
not read a multi-line base clause. Check 6 is now 220/220 with no divergence.

### [ ] Fix the IPersistStream type inconsistency
`AbstractClass.h` declares `IPersistStream` with `long`/`unsigned long*`, while
`ts_prelude.h`, `LocomotionClass.h` and real COM use `HRESULT`/`ULARGE_INTEGER*`.
Same vtable shape so no layout impact, but it makes
`LocomotionClass::GetSizeMax` hide the base's and accounts for one of 204
`-Woverloaded-virtual` warnings.
*Done when:* one definition exists and the source is type-consistent.

### [ ] Eliminate the 204 -Woverloaded-virtual warnings
Every one is `-Woverloaded-virtual`, which fires when a derived class declares a
virtual with the same NAME but a different signature as a base's. The dominant
cause is the `vfunc_N` placeholder scheme: slot N of a derived vtable is an
unrelated function to slot N of a base's, but they share a name, so g++ warns.
The gate compiles with zero diagnostics at its current warning level, so this is
latent. Decide whether to fix the naming, add `using Base::vfunc_N;`, or gate at
a documented warning level -- but do not leave it unexplained.
*Done when:* the warnings are either gone or the gate states why they are
expected.

### [ ] Verify member *layout*, not just offsets
Offsets are checked; whether types and ordering are right is not.

### [x] Recover `art.ini` and the remaining MIX bodies
The earlier claim that art.ini's bodies were compressed was **wrong**: only the
MIX *index* is encrypted, and every file body is in the clear. All seven control
files are now extracted with explicit byte ranges (`tools/extract_realdata.py`,
inventory in `realdata/README.md`), which lifted member-name confirmation from
85.2% to 98.2% and left only 2 names unconfirmed.

### [ ] Decrypt the encrypted TS MIX index
`tools/mix.py` reads the headerless TD/RA1 variant exactly (`SETUP/SETUP.MIX`,
61 entries) and fails *explicitly* on the encrypted TS variant rather than
reporting garbage. The published Westwood RSA/Blowfish scheme is implemented but
does not yet produce a self-consistent index for the archives on these CDs;
`python3 tools/mix.py probe <archive>` prints every key derivation and the bytes
it produces. Closing this would give exact entry sizes for every file in
TIBSUN.MIX instead of the content-derived ranges realdata currently uses.

### [ ] Byte-level validation against the original binary
Recompile and diff where a matching compiler is available.

---

## Standing caveats

- `VERIFICATION PASSED` means the tree compiles and that the generated headers
  agree with the recovered tables. It does **not** prove the disassembly is
  correct, and only check 4 and the binary member verifier are anchored to the
  binary.
- Member offsets are 99.9% confirmed against the binary, but **layout is not
  verified**: whether the types and ordering are right is unchecked.
- Member names exist only where an INI key or string provides evidence. Never
  infer one.
- Function bodies are empty. The project reproduces structure, not behaviour.
