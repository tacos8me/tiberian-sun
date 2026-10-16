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

### [ ] Recover the remaining 912 KB of unattributed `.text`
Roughly 95% of it is real code, not padding. Jump tables, SEH handlers and
`.data`-resident code are the identified untouched sources.
*Done when:* unattributed bytes approach padding-only and the residual is
reported with its padding fraction.

### [ ] Name more virtuals
5,855 of 7,140 remain `vfunc_N` (18.6% named). Existing rules cover 12.4% of
manifest slots; further progress needs new evidence rules, not better plumbing.
Candidate rules: unique string-literal reference, callee-set match against a
named base virtual, slot index against a known MSVC base layout.
*Done when:* the named ratio rises with every new name citing its rule and the
concrete signal it keyed on. Do not loosen a rule to raise the ratio.

### [ ] Mine the unused `rules.ini` keys
1,719 strings recovered; only 135 members named across 20 classes. The
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

### [ ] Unit tests for the load-bearing tools
No test directory exists. `verify.py` and the binary verifier test artifacts,
never the tools. Add golden-count assertions for `rtti.py`, `disasm.py` and
`gen_manifest.py` so a silent recovery regression fails a test rather than only
changing a number.
*Done when:* a deliberate off-by-one in `rtti.py` fails a test.

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
