# todo.md — Tiberian Sun reconstruction work list

Prioritized, evidence-backed work items. Each states what is wrong, how it was
verified, and what "done" means. Ordered by (value × certainty) ÷ risk.

Status legend: `[ ]` open · `[~]` in progress · `[x]` done.

**This file is the defect ledger. `re/ROADMAP.md` is the plan.** todo.md records
what is broken and how it was verified; ROADMAP.md states what "done" means, how
big each remaining problem actually is once the raw counts are corrected for
inheritance and for what is not code, and in what order the phases go. Read the
roadmap first if you are deciding what to work on; read this file before
touching anything.

**Measure before you trust any number here.** Run
`python3 tools/harness.py metrics --no-append`. The figures below were
re-measured on 2026-10-17; prose goes stale, measurements do not. Where a number
is quoted it is because the *change* matters, not because the value is current.

Naming quantities are easy to conflate, so state them the same way everywhere:
**9,810** vtable `(class, slot)` instances; **5,557** named pairs; **917**
distinct name strings (**281** semantic, excluding destructors and
QI/AddRef/Release); **6,957** virtual declarations in the headers (**4,499**
named + **2,458** `vfunc_N`); **702** classes with a naming record (this is what
`harness.py metrics` prints as `virtual_names` -- it is not a count of distinct
names). `PROGRESS.md` states the same distinction once in full.

---

## P0 — correctness of the transcription  [COMPLETE]

### [x] Spans that do not end on a transfer drop their continuation
**Fixed.** Every span that would otherwise fall through now emits a tail
continuation to a registered VA. Measured with
`python3 tools/gen_code.py out bin/GAME.EXE src/generated/code --check`:
**12,383 spans continue into another span by tail transfer**; the 15 that do not
are itemised (2 land inside another span's instruction, 13 land in bytes never
recovered as code). The same run reports `46008 functions transcribed,
918438/918438 instructions modelled (100.00%), 0 unmodelled`, and the
faithfulness gate passes instruction-for-instruction.

`tools/gen_code.py` fails the run when a span falls through onto an instruction
boundary inside recovered code with nothing registered there. The check is
proven by mutation (removing a continuation makes it report the defect and exit
1).

**Retraction, kept because it explains the defect.** This file used to carry the
same title twice: once `[x]` and once `[~]`. The `[~]` copy was the pre-fix
description — "1,513 spans end on a conditional branch and 8,937 on a
non-transfer instruction, about 10,450 spans incomplete" over 43,497
transcriptions — and its numbers are historical, not current. The two entries are
now one.

How it presented: a boot fault at `rep stos` 0x0068d781 with ECX=0x08000004
dwords. Chain: `calloc(1,0x74)` at 0x68794e -> 0x689c3d -> `_heap_alloc`
0x688bd0 -> sub_00688c9a, a 6-instruction span ending on `je 0x688cbe`. The
fall-through was dropped, so 0x688ed4's epilogue never ran and calloc was
returned into with EAX=EBX=0x20000010 -- the heap header pointer -- which it then
used as both `dst` and `count` for a 512 MB memset.

**Why the old gate passed over it.** The faithfulness gate proves *instruction
coverage*: every instruction appears exactly once, in order, with matching
operands, and the rendering round-trips. It did not prove *control-flow
coverage*. A tree could satisfy it completely while ~10,450 spans had no
continuation. That is the gap the control-flow gate closes.

### [x] Recovery missed ~1,700 entry points that the game actually calls
**Found by running the code, not by reading it.** The disassembler's prologue
heuristics had never recovered entry points with no prologue (a 13-byte leaf like
`push [0x834c60]; call [0x698108]; ret`) or ones that begin inside alignment fill.
The boot called them, and the link test reported 13.1M unmapped calls inside
`.text`.

`tools/disasm.py` now recovers entry points to a FIXPOINT from three evidence
sources, all read out of the binary: data-section code-pointer tables (a run of
>= 2 dwords pointing into .text whose peers are recovered starts), table entries
landing in a gap, and spans that begin in alignment fill. Every seed was CALLED by
the original program, so it is evidence, not a guess.

Current measured state: **43,557 spans, 45,947 transcriptions, 912,879/912,879
instruction lines modelled at 100%**, 0 unmodelled, `.text` code coverage
**95.86%** (the disasm correction, below). The historical progression quoted in
the previous version of this entry (41,822 -> 43,564 spans; 897,193 -> 918,438
instructions) is kept only as history.

The two residual in-`.text` targets this entry used to name — `0x6844df` (from
`0x5e7317`) and `0x46f3f8` (from `0x674bf6`), both RETURN ADDRESSES of indirect
calls — disappeared with the 108-byte stack-leak fix. A later teardown
regression (10 in-text unmapped calls) was fixed by `47d26eb`; the link test
passes and `boot.py --strict` exits 0.
*Done when:* that is fixed and the link test passes. **Met.**

### [x] The 32 ordinal imports could not pop their arguments
The stub table keyed an ordinal by its raw ILT value (`"ord_2147483681"`) while
the loader asks for `"#161"`, so all 32 silently fell back to a stub with
`nargs=0` -- a stub that cannot pop. Every such call left the stack imbalanced.
Now **341 of 341 PE import-directory thunks resolve** (`link_test.py`: `imports:
341 across 13 DLLs`, `341 of 341 IAT slots written`, `0 with no stub`), 0
fallbacks. The stub table has **342 rows** because it also carries
`COMCTL32!DllGetVersion`, which is reached through `GetProcAddress`, not the
import directory; `register_win32_stubs()` reports 315 with a body and 27
generic. Argument counts are derived from the recovered instruction stream
(count the pushes reaching each `call [slot]`, stop at a branch
target/call/ret/pop/ESP-adjust, skip the prologue); 12 measured, 20 with no call
site tagged `"doc"` and printed as unmeasured. `--stack-check`: 341 driven, 0
imbalanced.

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
**Superseded, and the old description is retracted.** The bodies are no longer
empty definitions. `src/generated/code/` holds 45,947 transcriptions
(43,557 recovered function starts + 2,390 tail entries), one machine instruction
per generated line, 912,879 instruction lines in all; the per-class `.cpp` files
are 6,962 forwarders across 310 translation units that call the canonical
transcription of their VA (3,024 distinct machine functions, 3,938 aliases). The
tree links (492 TUs) and runs the original entry point. The project reproduces
behaviour as a machine transcription; it is not source-shaped C++ (see P3).

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
Done. Measured now: **43,557 spans**, **95.86%** of `.text` as recovered code,
**112,401 B (110 KB)** unattributed and classified as data/padding (the
switch-table correction moved 213 tables out of code; see below). The tool
recorded gap-recovered bodies' callees but never fed them back, never followed
conditional jumps, and read jump tables only from initial seeds. Made the
worklist a fixpoint. Verified the 1,019 inner entry points are real: 1,004 are
branch/call targets, only 15 unreferenced.

### [x] Declare members at the width the constructor stores
Done. Every member was emitted as a 4-byte `unsigned long`; one written with
`mov byte ptr [eax+0xc], 1` swallowed every offset inside that span. Check 7 went
1,235 -> 2,032 offsets across 165 -> 237 classes.

### [x] Fix the IPersistStream type inconsistency and the COM vtable evidence
Done. AbstractClass.h now matches the binary and the prelude
(HRESULT/__stdcall/ULARGE_INTEGER*); slot 0 is no longer mislabelled a
destructor (0x405790 is a shared E_POINTER stub and slot 0 of 66 classes); slot 6
is declared; LocomotionClass's Load/Save/GetSizeMax citations are corrected.
verify.py gained check 8, reporting declared-vs-recovered arity (measured now:
**331/1116 match, 785 divergences**) with the `this`-counting caveat documented.
**A duplicate of this item in P3 stayed `[ ]` after the fix; it has been
collapsed into this entry.**

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

### [x] Decrypt the encrypted MIX header
**Solved.** The headerless TD/RA1 variant was already correct (SETUP.MIX: 61
entries, exact size identity). The TS/RA2 encrypted variant now decodes too.

The earlier attempt failed on two details, both of which produce plausible-looking
key material that decrypts nothing:
  * the exponent is the public 0x10001, not the private key from keys.ini;
  * the key block is decoded in `a+1`-byte pieces where `a = (bitlen-2)/8` and
    each piece yields only `a` bytes. Copying `a+1` bytes out gives an 80-byte
    key instead of the correct 56, and the wrong byte alignment.
The decrypted index is a `u16` count then a `u32` body size -- 6 bytes, not a
`u32` count at 0 -- then 12-byte records, padded to a Blowfish block. Flags bit
0x00010000 adds a 20-byte SHA1 trailer.

The proof is not that the numbers look plausible. It is that header + padded
index + body size equals the file size exactly, which holds on TIBSUN.MIX,
MULTI.MIX, SCORES.MIX and MOVIES02.MIX with every record in bounds and the
bodies contiguous. TIBSUN.MIX then yields a 151-byte entry whose contents are
the `[PublicKey]` line naming the modulus used to decrypt it, so the round trip
closes on itself.

Consequence: `tools/extract_realdata.py` no longer needs hand-computed byte
ranges; the offsets come from the index.

### [x] Classify the remaining ~103 KB of `.text`
**Done.** The earlier guess ("roughly 95% of it is real code") was wrong, and
reading the bytes showed why:

```
gap 0x663820 224B: c62a6600 4c376600 db2c6600 8e2a6600 ...   <- jump table
gap 0x45fef0  64B: c7fe4500 e3fe4500 d0fe4500 dafe4500 ...   <- jump table
```

They are **switch tables and alignment padding**. Measured in
`out/text_data.json`: 112,401 B unattributed, classified as **213 static switch
tables (6,388 B)**, 102 switch index tables (145 B), 105,492 B of alignment
padding in 12,671 runs, and 376 B in 46 runs reported verbatim rather than forced
into a category. 99.99% of `.text` is attributed as code or data; 95.86% is
recovered code.
*Done when:* coverage is >=99.5% with every remaining byte classified. **The
"coverage" here means attributed, not code: the 99.5% code target is not met and
is not achievable, because the remainder is data.**

### [x] Load the image's data sections into the machine model — retracted as a defect
**The premise was false.** `src/handwritten/Image.h` copies every PE section's
raw extent into modelled memory at its real VA and writes a stub VA into every
one of the **341** IAT slots. `tools/link_test.py` reports `imports: 341 across
13 DLLs`, `resolve_imports() ... 341 of 341 IAT slots written`, and `341 imports
bound, 0 with no stub`; it does **not** stop at a null call target. The stub
table has 342 rows because it also carries `COMCTL32!DllGetVersion`, reached
through `GetProcAddress` rather than the import directory.

The empty `MessageBoxA` text is `LoadStringA` returning 0 (see the stub table),
not a missing section. The "16,383 calls read 0" figure described the pre-Image.h
model and is retracted.
*Done when:* the entry-point run reports no call target of 0x00000000. **Met;
this item should not be worked on.**

### [x] 166 instructions have no faithful rendering
**Done: 0 unmodelled.** The former 166 — MMX/SSE (`movq`, `psrlw`, `movd`,
`pxor`, `packuswb`, `punpcklbw`, `punpckhwd`, `psrad`, `pmulhw`), the x87
environment save/restore (`fnsave`, `frstor`, `fnstenv`, `fldenv`), far
pointers (`les`, `lds`) and `lar` — are rendered against the machine model.
Measured: `python3 tools/gen_code.py out bin/GAME.EXE src/generated/code
--check` prints `46008 functions transcribed, 918438/918438 instructions
modelled (100.00%), 0 unmodelled`, and `grep ts::unmodelled
src/generated/code/*.cpp` returns nothing. Two limits of the *model* remain
(`lar` has no descriptor table; `fnsave`/`fnstenv` write zero into the x87
pointer fields), stated in `Machine.h`.

### [ ] Name more virtuals
**The denominator in this item was wrong twice and is now measured once.** The
old "1,500 named (15.3%), 8,280 unnamed, 3,409 distinct" mixed three different
quantities and is retracted. Measured on 2026-10-17:

- **9,810** vtable `(class, slot)` instances in the manifest.
- **5,557** of those carry a name in `out/virtual_names.json` (56.6%).
- **917** distinct name strings; **281** excluding destructors and
  QI/AddRef/Release.
- The generated headers declare **6,957** virtuals: **4,499 named** (64.7%) and
  **2,458 `vfunc_N`**. Headers declare only what a class introduces or
  overrides, which is why 6,957 is smaller than 9,810.
- `harness.py metrics` prints `virtual_names: 702`; that key counts **classes
  with a naming record**, not distinct names. Do not quote it as a name count.

The old "3,409 distinct" was an estimate of slot instances after inheritance
deduplication, not a name count, and is not used as a denominator.

Leverage is the whole game here — naming a base names its subtree (re-derived
from `out/class_manifest.json` and `out/direct_bases.json`; the metric is the
base's slot count times (subclasses + 1)):

| Class | slots | subclasses | slot-instances covered |
|---|---|---|---|
| `ObjectClass` | 105 | 20 | 2,205 |
| `AbstractClass` | 24 | 68 | 1,656 |
| `TechnoClass` | 214 | 5 | 1,284 |
| `MissionClass` | 133 | 7 | 1,064 |
| `RadioClass` | 135 | 6 | 945 |

The top 18 bases cover 13,011 slot-instances. An inheritance-propagation engine
is the prerequisite: 917 distinct names currently cover 5,557 named pairs, and
each new base name can cover its whole subtree.
*Done when:* >=60% of the 6,957 header virtual declarations are named, each
citing its rule and the concrete signal it keyed on. Do not loosen a rule to
raise the ratio.

### [ ] Mine the unused `rules.ini` keys
1,876 strings recovered; 291 members named across 20 classes (8.3% of the 3,526
non-vptr members). `tools/e2e_rules.py` resolves 280 of 291 (96.2%) against the
shipped INIs. The reader-trace mechanism in `name_members.py` works and is
proven.
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
`tools/test_tools.py`: **94 checks passed** (re-measured for this pass). Not
golden-count assertions -- every one is a regression test for a defect that
actually shipped, or a differential test against an independent implementation.
Four are proven to fail when the defect is reintroduced (mutating
`gen_bodies.DECL`, swapping the `in` operands, removing the `loop` tail
handling, removing the immediate mask each produce failures naming the right
check). Wired into the pipeline as stage 0b.

### [x] Compile the gate with `-m32`
**Done and gating.** `verify.py` compiles every header and the combined TU at
`-m32` and fails closed when `g++-multilib` is absent. Measured this session:

```
[1] standalone compile: 265/265
[2] combined TU compile: PASS
[9] compiler-confirmed member offsets: PASS (3073/3073 asserted)
```

**Retraction, kept because the two stale claims were both real defects once.**
An earlier copy of this entry was `[~]` and quoted the LP64 host build: 2,313
`unsigned long` declarations, `offsetof(AbstractClass, m_000) == 20`, 1,053 of
4,326 member entries being vptr stores emitted as data members. Those figures
described the pre-fix tree and are not current. The current recovered table has
763 vptr stores (620 at offset 0) out of 4,289 entries; the headers omit them and
check 9 confirms 3,073/3,073 expressible offsets. The old "262/262 headers,
482 TUs" measurements are likewise superseded (263/263 and 492 TUs).

### [x] Run against real game data
`realdata/` holds the shipped control files extracted from the retail archive
(`rules.ini`, `art.ini`, `ai.ini`, `sound.ini`, `theme.ini`, `BATTLE.INI`,
`MISSION.INI`; byte ranges and section counts in `realdata/README.md`);
`tools/e2e_rules.py` drives the reconstructed model with them and
`tools/extract_realdata.py` regenerates them from the MIX archive. Measured this
session: `tools/e2e_rules.py` resolves **280 of 291 (96.2%)** recovered member
names against the shipped data; 14 are documented only in the files' own comment
headers. This is the first external validation of the recovered names, offsets
and readers.

### [x] Declare the base classes two hand-written headers omit
`AnimClass` omitted `StageClass` (RTTI: mdisp 76) and was fixed: StageClass now
has its own header, since its definition lived inside `TechnoClass.h`, which no
other class included. `HouseClass` turned out to be CORRECT -- the checker could
not read a multi-line base clause. Check 6 is now 220/220 with no divergence.

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

### [x] Decrypt the encrypted TS MIX index
**Solved, and this entry was a stale duplicate of the P2 item above.** The
headerless TD/RA1 variant was already exact (`SETUP/SETUP.MIX`, 61 entries), and
the TS/RA2 encrypted variant now decodes too: header + padded index + body size
equals the file size on TIBSUN.MIX, MULTI.MIX, SCORES.MIX and MOVIES02.MIX, and
TIBSUN.MIX yields the `[PublicKey]` entry naming the modulus used to decrypt it.
`tools/mix.py probe <archive>` remains available for diagnostics.

### [ ] Byte-level validation against the original binary
Recompile and diff where a matching compiler is available.

---

## P4 — booting the game  [IN PROGRESS]

### [x] Load the image and resolve the imports
`src/handwritten/Image.h` maps the PE's sections at their real VAs and writes a
stub VA into every one of the **341** PE import-directory IAT slots.
`src/handwritten/Win32Stubs.h` supplies those stubs. Measured: `link_test.py`
reports `341 of 341 IAT slots written` and `341 imports bound, 0 with no stub`.
(The historical "16,383 unmapped calls became 0" described the pre-Image.h
model.) The PE details that would have silently corrupted it, all handled and
read from the file: no `.reloc` (mapped at preferred base, so the transcriptions'
absolute immediates stay valid), no bound imports, `OriginalFirstThunk == 0` for
KERNEL32/USER32, and `.data` VirtualSize > SizeOfRawData.
*Done when:* the entry point no longer reads a null call target. **Met.**

### [x] Fix the page table that was silently corrupting long runs
`PageTable` had 8192 slots. The game's memset needs 32,768 pages, so the table
filled and `page()` returned one shared dummy page for every new address -- every
distinct page overwrote the same 4 KB -- while linear probing degraded to
O(8192) per lookup. The only symptom was "it got slow"; every long run would have
been quietly wrong. Found with `perf` (99.67% in `ts::page`). Now 32,768 slots and
exhaustion is a hard reported fault.
*Done when:* a full table is a reported fault, not silent aliasing. **Met.**

### [x] Get past the 512 MB memset
Fixed, and the fix was two separate defects:
  - `ret` did not pop the return address, so every call leaked 4 bytes and ESP
    drifted until code read a stale `[esp]` as a return address.
  - Spans that did not end on a transfer dropped their continuation, so ~10,450
    bodies returned with their frame abandoned.
Both are recorded above. The memset itself was a symptom of the second.

**Retraction, kept because the diagnostic was useful.** A second copy of this
item was marked `[ ]` and described the fault at `rep stos` 0x0068d781 with
ECX=0x08000004 dwords, `calloc(n,4)` at 0x6845b4 and the bad count in the global
at 0x836280. That was the pre-fix state; the two defects above are the fix. The
duplicate entry has been collapsed into this one.

### [x] The 108-byte stack leak across the atexit walk — fixed
The CRT's atexit walk over the 3,311-entry static-constructor table leaked
**108 bytes**: ESP was 0x002ff56c at `push %esi` (0x6844cf) and 0x002ff4fc at the
matching `pop %esi` (0x6844ea). `pop %esi` therefore loaded a return address,
ESI became 0x6844df, and `call *%esi` at 0x5e72cf called into `ExitProcess`
instead of the function whose result decides whether the "Direct X" error box
appears. The fix is `ts_call_ptr` delegating to `ts_call_at` (`Machine.h:863`),
so the two call paths balance the same way. Measured at that commit: the game
reached its own error box with `MessageBoxA` type **0x30**, called
**`ExitProcess(1)`**, and stopped after **7,038,765 instructions** with **0
unmapped calls inside `.text`**. The old corrupted values (`0x6844df` for `1`,
`0x00a00201` for `0x30`) were gone; exit 1 was the game's legitimate failure
path, not a corrupted one. The later COM fix (`1580be3`) carried the run past
that error box to the message loop (20,000,001 instructions).

### [x] `jmp_indirect` was a no-op — fixed
`Machine.h:1368` is now `inline void jmp_indirect(u32 target) { ts_tail(target); }`.
Before the fix it discarded the target, so an indirect jump fell through to the
next instruction instead of transferring. Found by the boot lane while chasing
the leak.

### [x] Conditional `rep`/`repne` `scas`/`cmps` zeroed ECX — fixed
`Machine.h:895-902` now leave ECX holding the count that did not run. The
sequence `repne scasb; not ecx` depends on it: zeroing ECX made `not ecx` return
the wrong length. The unconditional repeats still legitimately finish with
ECX == 0.

### [x] `LoadStringA`'s arity was 5 — fixed
`Win32Stubs.h:1634` is `TS_RET(LoadStringA, 4, 0u)` and the table row is 4
(`gen_win32_stubs.py:102`). The body used to pop 5, shifting the caller's stack.
`gen_win32_stubs.py` now **refuses to write** when a `TS_RET` body's count and
`NARGS` disagree, so this class of defect fails the generator instead of
shipping. The MessageBox text is still empty, but now because the string table
is empty at that point, not because the stack was wrong.

### [x] The link test's `ExitProcess` stub returned — fixed
`build/ts_link_test.cpp` set `g_halt` but not `m.halt`, so `insn()` did not
longjmp and the run walked past the fatal exit; that is why it printed
`MESSAGE LOOP: reached` after a fatal exit — the same retracted false positive
`boot_driver` had already fixed. Commit `db612d1` makes it call `ts::fault(...)`.
Measured at `db612d1`: `link_test.py` exits **0**, reports `MESSAGE LOOP: not
reached (no GetMessage/PeekMessage call)`, 7,038,765 instructions, 2 unmapped
calls (0 in `.text`), `ts_link_test: OK`. The COM fix later made the loop
genuinely reach: measured now `MESSAGE LOOP: reached -- PeekMessage 695115,
DispatchMessage 0`, 20,000,001 instructions, 0 unmapped, `ts_link_test: OK`.

### [x] `boot.py`'s fresh-checkout header sync — fixed
`60dac16` runs reconcile + self-contained after copying the hand-written headers,
so a fresh checkout no longer gets 140 raw headers copied over the reconciled
ones. Measured: `python3 tools/boot.py` builds (0 of 492 units compiled on a warm
cache) and reports.

### [x] `blowfish.dll!DllRegisterServer` fails, so the game takes its error box — fixed
**Was the boot stop, measured.** The game reaches its own `MessageBoxA` at
`0x5e72fb` (`type=0x30`, `ExitProcess(1)`). The box is guarded by the byte at
`-0x5f4(%ebp)`, which is set at `0x5e6216` from `FUN_005e87a0` (a
`CoCreateInstance` wrapper) and, in the fallback, at `0x5e625b`/`0x5e626b`.

The fallback path is the failure:

```
0x5e6227  push 0x6d99d4                 ; "blowfish.dll"
0x5e622b  call LoadLibraryA
0x5e6233  push 0x6d9b34                 ; "DllRegisterServer"
0x5e6239  call GetProcAddress
0x5e623f  test %eax,%eax
0x5e6241  je 0x5e625b                   ; not found -> set the error flag
```

The model's `LoadLibraryA("blowfish.dll")` returns a fake handle, and
`GetProcAddress(handle, "DllRegisterServer")` returns 0 because no stub exists
for it. The flag becomes 1, and the box appears after init. The retail game has
`BLOWFISH.DLL` in `gamedir` and its export registers the Westwood class
`{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}` (`0x69e960`; a second CLSID at
`0x69e970` is `{B45A4A80-86DA-11D1-B706-00A024DDAFD1}`).

Two related defects in the same path:
- `CoCreateInstance` is a **generic** row (`Win32Stubs.h:2975`, `real=0`), so it
  returns `S_OK` but never writes its out pointer, contrary to the comment at
  `Win32Stubs.h:2199` ("handed a fake COM object rather than a null"). The
  wrapper then calls `OleRun`/`QueryInterface` through an uninitialised pointer.
- `GetProcAddress` cannot resolve an export on a DLL the model only pretends to
  load; `DllRegisterServer` is exactly that case.

**Fixed in `1580be3`.** `LoadLibraryA` now returns a module handle that names
the module, with a per-module export table; `blowfish.dll` exports
`DllCanUnloadNow`/`DllGetClassObject`/`DllRegisterServer`/`DllUnregisterServer`.
`CoCreateInstance` writes an `IBlowfish` object for CLSID `1440AD10-…` whose
eight-slot vtable was recovered from `BLOWFISH.DLL` (object vtable
`0x1101e480`); slots 3–7 are `SetKey`, `GetMaxKeyLength` (56), `GetBlockSize`
(8), `Encrypt`, `Decrypt`. `IDirectDraw::CreateClipper` and
`DirectSoundCreate` no longer return `S_OK` without writing their out pointers
(the latter now returns `DSERR_NODRIVER`, matching the retail no-audio
environment). A regression test (`win32-module-exports`) covers handle
identity, export isolation, `*ppv` being written, the IBlowfish methods and ESP
balance; it is mutation-proven.

Measured at that commit: the registration flag stays 0, the `0x5e72fb` error box
never fires, there is no `ExitProcess`, the primary-surface check passes
(`TS_PEEK: 0x006beed8 = 1`), and the boot **reaches the message loop**:
`PeekMessage=695,115`, `CreateWindowExA=1`, `DispatchMessageA=0`, 0 unmapped
calls, `boot.py --strict` exits 0.

**Superseded by later commits:** message delivery (`f3011ca`) made the loop
dispatch, the Blowfish cipher (`345aae7`) replaced the identity copy, and the
MIX-read/teardown fixes carried the run into the dialog loop, and the later
dialog/narrow-flag fixes carried it to the intro-movie pump at `0x00488800`,
with **0 unmapped calls** and `--strict` exiting 0.

### [x] Write `DDSURFACEDESC` fields where the game reads them
The game's `DSurface` wrapper (`0x4874c0`/`0x487ad0`) reads `dwRGBBitCount` at
`desc+0x54` and `lpSurface` at `desc+0x24`, and computes bytes-per-pixel as
`(bpp + 7) >> 3`; it requires 2 for 16bpp. The stubs wrote the bit count at
`+0x68` and `lpSurface` at `+0x14`, so the primary-surface check saw 0. Fixed in
`138718a` against the game's own code. This did not move the current stop
(COM registration fails earlier), but it is the next runtime check.

### [x] Reach a window and a message loop — reached
**Reached in `1580be3`.** `CreateWindowExA=1` and `PeekMessage=695,115`; the
loop at `0x0055f78a` (`sub_0055f780`, the PeekMessage pump) is the game's own.
At that commit the loop was idle (`DispatchMessageA=0`, `GetMessage=0`), which
the next entry fixed: `f3011ca` delivers `WM_ACTIVATEAPP`, so the loop now
dispatches and the game leaves the spin.

This entry previously said "Reached. CreateWindowExA=1, PeekMessage=137,295"
and was false, for an instructive reason: the model let `ExitProcess` return,
so execution walked past a fatal exit and the "loop" was one the real game
never ran. `ExitProcess` now halts the run in both `boot_driver` and the link
test, and the current measurement is from a run that never calls it.

The unlock on this path is worth keeping: `comctl32!DllGetVersion` was reached
through `GetProcAddress` rather than the import table, so there was no stub, the
version read as 0, the CRT's version check failed and the game called
`ExitProcess(1)` before initialising anything. One export reporting 4.72 fixed
it. The same class of defect (`GetProcAddress` on a module the model only
pretended to load) blocked `blowfish.dll!DllRegisterServer` until `1580be3`.

Measured at that commit with the project's `../gamedir`: **43,845 distinct
instructions; 4,142 of 46,008 recovered functions entered**; 0 unmapped calls;
`boot.py --strict` exited 0. Without `../gamedir` the counts differ slightly
(the `CreateFileA("\\.\MONO")` path changes); use the project gamedir for the
canonical figure. **Superseded:** message delivery (`f3011ca`) and Blowfish
(`345aae7`) moved the stop to the CRT exit path and reintroduced teardown
unmapped calls; see the entries below.

### [x] Deliver queued messages so the game runs frames — fixed
**Was the blocker, measured.** The message loop at `0x0055f780` spun on
`DAT_007b2e50`:

```asm
5e77b0: mov al, ds:0x7b2e50
5e77b5: test al, al
5e77b7: je   0x5e77ab        ; call FUN_0055f780 again
```

`DAT_007b2e50` is "app active". The game's WndProc sets it at `0x65bd2a` from
the `WM_ACTIVATEAPP` (`0x1c`) `wParam`; the dispatch is at `0x65bcaf` and the
handler at `0x65bd00`. The model's `PeekMessageA` always returns 0, so no
`WM_ACTIVATEAPP` is ever delivered, `DispatchMessageA` is never called, and the
flag never changes.

**Fixed in `f3011ca`.** `CreateWindowExA` now queues `WM_ACTIVATEAPP`
(`0x001c`, `wParam=1`) after the synchronous `WM_CREATE`; `RegisterClassA`
already recorded the WndProc, and the existing `PeekMessageA`/`GetMessageA`/
`DispatchMessageA`/`SendMessageA` bodies were correct but unreachable because
nothing was ever queued. A mutation-proven test drives every USER32 stub
through `ts_call_at` and checks the `MSG` layout, `PM_REMOVE`/`PM_NOREMOVE`,
WndProc dispatch and the `__stdcall` pop convention.

Measured: the game's WndProc at `0x65ba70` runs, `DAT_007b2e50 = 1` at
`0x65bd2a`, and the run leaves the spin: `GetMessage=1`, `PeekMessage=3`,
`DispatchMessage=1`, `CreateWindowExA=1`; **1,882,051 instructions**, coverage
49,635 / 4,455 functions. The next stop is the Blowfish identity copy (below):
the TIBSUN.MIX index is read and "decrypted" to ciphertext, so a wrapped
`operator_new` exhausts the page table at `0x00a006b0`.

**Secondary, not blocking:** the DirectSound warning text is the literal
`"%s\n\n%s"`, so the `sprintf`-wrapper path (`0x682aae` → `0x68667c`) or
`LoadStringA` is not producing formatted text. The box renders with the format
string; fix it when the content path is stable.

**Downstream:** the newly reachable code makes indirect calls with
`this == NULL` (`ESI=0`) at `0x4b312b`, `0x4b319b`, `0x4b31a4`, `0x487d16`,
`0x5e7c7e` (and later `0x5f1083`, `0x5f1006`, `0x5e7cfe`, `0x5e8489`) —
teardown of objects whose construction failed after the undecryptable index.
They did **not** disappear when the cipher was fixed; the separate teardown
entry below tracks them. `boot.py --strict` exits non-zero (currently **3**).

### [x] Implement the Blowfish cipher the MIX indexes need — fixed
**Measured against the retail binary, and it was not optional.** The object that
`blowfish.dll` registers (`IBlowfish`, CLSID
`{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}`, vtable `0x1101e480`, 8 entries) is a
real cipher the game uses during startup: `SetKey(56, key)` **9 times** and
`Decrypt(8, in, out)` **1586 times**. The nine 56-byte keys are byte-identical
to the keys derived from nine shipped MIX indexes (TIBSUN.MIX, three TIBSUN
sub-archives, MAPS01, MULTI, SCORES, MOVIES01), and 1586 is exactly the sum of
those index sizes in 8-byte blocks.

Before the fix, the model's `IBlowfish::Encrypt`/`Decrypt` copied the block and
logged once that no cipher was modelled. If the game reached MIX content loading,
every index decrypted to garbage and the failure looked like a corrupt archive
rather than a missing cipher — exactly the "plausible wrong value" failure this
project keeps finding.

**The current boot stop, measured** (after `f3011ca`):

```
step 1881678  IBlowfish::SetKey(56, key=82060323...)   from 0x423404
step 1881870  ReadFile(8) at MIX offset 84
step 1881940  IBlowfish::Decrypt(8, ciphertext)        from 0x423469
step 1882051  HeapAlloc(0xfffea610)                    from 0x68511c
```

The 56-byte key the game computes with its own RSA code is byte-for-byte the
key `tools/mix.py` derives from the same 80-byte block (`82060323...`), and
decrypting the first index block with it yields a valid index (`count=17`,
body `75,094,416`). The identity copy leaves ciphertext, so the PKStraw read
returns 0; `FUN_00547290` then reads its 6-byte header from uninitialised stack
(`count=0xe32c`), and `operator_new(-88560)` wraps to a ~4 GB allocation that
exhausts the model's page table at `0x00a006b0`.

**Fixed in `345aae7`.** Standard Blowfish-ECB in slots 3/6/7: pi-derived
P-array and S-boxes, the standard key schedule, 16-round Feistel, big-endian
blocks, `len/8` blocks plus a verbatim tail, in-place operation, and
`SetKey(NULL)` -> `E_POINTER`. Proven by the four standard known-answer vectors,
round-trip/block/tail/stack tests, and — the strongest evidence — a
byte-identical cross-check against `tools/mix.py` on the shipped TIBSUN.MIX
index: key `82060323...`, ciphertext `2ce3171bdbecb40e` decrypts to
`110090d97904849a` (count 17, body 75,094,416). That vector is now a permanent
regression test; `tools/test_tools.py` now runs **70** checks.

Measured at that commit: the page-table exhaustion at `0x00a006b0` was
**gone**. The later MIX-read, disasm, tracker, `idiv`, dialog and narrow-flag
fixes moved the stop to the intro-movie pump at `0x00488800`; the current
measurement is `GetMessage=1 PeekMessage=5 DispatchMessage=1 CreateWindowExA=1`,
coverage 55,774 / 4,764 of 45,947, **0 unmapped, 0 imbalances**, `--strict` exit
0. The
game's own `SetKey(56, 82060323...)` and the correct decryptions are visible in
`TS_FILE_TRACE`.

*Spec (kept for the MIX read path):* `re/out/blowfish_oracle.md`, produced by
`re/tools/oracle_blowfish.py` and `re/tools/wine_blowfish_trace.sh`.

### [x] The MIX index read stops after 2 of 27 blocks — fixed
**Was the next measurable defect, after `345aae7`.** The game read only **2
index blocks (16 bytes)** instead of 27. The root cause was **not** the pipe or
the cipher: `Buffer::Read` at `0x423620` loops correctly, reading 8-byte
Blowfish blocks and copying each into the caller's buffer with `memcpy`
(`0x683bb0`). The `memcpy` takes an **unaligned-destination jump table**; for
`dest & 3 == 2` it jumps to entry 2, `0x683c4c`.

`0x683c4c` is an instruction boundary inside the recovered span at `0x683c49`,
but it was **not a registered function start**. `ts::jmp_indirect` dispatches by
exact VA, `ts_lookup(0x683c4c)` returned null, so the tail dispatch recorded an
unregistered call and returned — **abandoning memcpy's frame**: EBP/EDI/ESI
(12 bytes) and the return address (4 bytes) stayed on the modelled stack, and
ESI still held the source pointer. The next loop iteration at `0x4236a7` used
`ESI = 0x2fefbc` (the memcpy source) as its `this`, so the read never continued.

**Fixed in `1c0db7e`.** `tools/gen_code.py` now discovers static switch-table
targets from the image and registers every entry that is an instruction boundary
of a recovered span as a tail entry: **43 added, 46,008 -> 46,051
transcriptions, 919,493 instructions**. No `emit_x86.py`, `Machine.h` or stub
change was needed. A mutation-proven regression test is in `13726a9`.

Measured: all **27 index blocks** are read (file position 84 -> 300) and the
**17 records match `tools/mix.py` 17/17** (offset - 300, size equal). The game
then reads entry bodies: 544 8-byte reads, file position past 22 MB. New stop:
budget exhausted at `0x0055fd1c` (`sub_0055fcf0 +0x2c`, a vector/array copy
loop), depth 23, **ESP `0x002fee5c` (healthy)**, no fault, no `ExitProcess`;
message loop `GetMessage=1 PeekMessage=13,607 DispatchMessage=1`; coverage
52,011 / 4,616 of 46,051; **5 unmapped calls, 0 inside `.text`** (the known null
teardown sites). At a 60M budget it stays in the same steady state, so it is not
a corrupted loop. The shutdown/ESP path is no longer reached.

### [x] The recovery decoded memcpy's jump tables as code — fixed
**Was latent, measured by the lane that fixed the short read.** The jump tables
at `0x683c10` and `0x683c20` were decoded as code, creating false spans at
`0x683c14`, `0x683c89` and `0x683cf6`, so entry 1 of each table was not an
instruction boundary and could not be registered. Any `memcpy` with
`dest & 3 == 1` and `len >= 4` would have abandoned its frame.

**Fixed in `9e5703f` + `ad62afc`.** `tools/disasm.py` derives switch-table data
regions from evidence only (a recovered `jmp dword ptr [reg*4 + base]`, then a
run of at least two `.text` code pointers; `base+4` is tried for the unreachable
index-0 hole). Recovery is a fixpoint over the seed and data sets, and the gap
seeder/`_recover` stop at data. The binary has **three** hole tables, not two:
`0x683c10` (entry 1 `0x683c20`), `0x683d98` (entry 1 `0x683da8`) and
`0x68ce40` (entry 1 `0x68ce50`). All three entry-1 addresses are now function
starts, transcribed (14, 12 and 14 instructions), declared and registered.

Counts: recovered functions 43,564 -> **43,557**; transcriptions 46,051 ->
**45,947**; instruction lines 919,493 -> **912,879** (100% modelled); `.text`
code 96.09% -> **95.86%**; jump tables 264 B/12 -> **6,388 B/213**. Every one of
the 3,186 instruction addresses that left the transcription was classified: 2,581
inside classified tables, 249 re-decoded from corrected boundaries, 356 fill/
table bytes. No real code was lost. The pipeline ran **exit 0** (8m51s), the
faithfulness gate verified all 45,947 functions, the link passed, and
`verify.py` printed `VERIFICATION PASSED`. `tools/test_tools.py` is now **68
checks** (`test_jump_table_data`, mutation-proven).

### [ ] Shutdown corrupts ESP before the CRT exit path — not reached now
**Measured before the MIX-read fix; the current run never gets here.** After
`1c0db7e` the game reads the index and entry bodies and stays in its own loop;
it no longer posts `WM_DESTROY` within the 20M/60M budget, so this path is
dormant, not fixed. Keep it on the list: it will matter as soon as shutdown is
reached again.

**Original measurement, separate from the MIX read.** WinMain returned at ~7.66M
instructions with ESP `0x11`; it was `0x2fff84` at entry. The CRT's atexit walk
then consumes the remaining ~12.3M instructions. This is the same class as the
original 108-byte leak, but on the shutdown path: find the call or tail
continuation that does not restore ESP, using the per-callee ESP tracker
(`TS_TRACE`, `leak_va`/`leak_delta`) rather than counting pushes.
**The walk itself, decompiled** (`FUN_00684486`, Ghidra MCP):

```c
if (param_2 == 0) {
  if ((DAT_00836290 != 0) && (puVar1 = DAT_0083628c - 4, DAT_00836290 <= puVar1)) {
    do {
      if ((code *)*puVar1 != 0) (*(code *)*puVar1)();
      puVar1--;
    } while (DAT_00836290 <= puVar1);
  }
  ...
}
```

`DAT_0083628c` is the table end and `DAT_00836290` the start. Nine of the ten
in-text unmapped targets come from the walk's `call eax` at `0x6844dd`, and they
are **return addresses**, not constructor pointers: `0x6844df`, `0x5e867e`,
`0x5e864a`, `0x682995`, `0x683fce`, `0x5e7d68`, `0x6864ba`, `0x46f8e9`. The
table has been populated with return addresses (the same signature as the
original 108-byte leak), so the corruption happens before or during `_onexit`
registration. Peek `TS_PEEK=0x83628c,0x836290` and scan the table with
`TS_SCAN` before assuming the walk is at fault.

**The table is heap memory, not an atexit table.** `TS_PEEK` at the end of the
run:

```
0x0083628c = 0x50006f28   (end)
0x00836290 = 0x50006c70   (start)   -> 174 entries
```

At the stop (`0x006844db`, the walk), `ebx = 0x50006f24`, `eax = 0`,
`esp = 0xfffffff1`, `ebp = 0x005e7d68`. Watching the first entry
(`TS_WATCH=0x50006c70`) reports **one write of `0x00000020` from `0x0068ce13`**
— a heap header, not a function pointer. The bounds point at a reused heap
block, so the walk is calling whatever the heap holds. The ESP corruption
(`0xfffffff1` at the stop, `0x2fff84` at WinMain entry) is the root cause; the
walk is the symptom.

*Next:* first fix the stack-imbalance tracker (the entry below), because the
current false positive hides the callee that moves ESP; then trace WinMain's
return (`0x5e5b80` entry, caller at `0x6855b1`) and the `_onexit` registration
path. Do not chase the walk's targets individually.
*Done when:* WinMain returns with ESP where its caller left it and the CRT exit
path completes (or faults at a named instruction).

### [x] The 5 null-pointer teardown calls — fixed
**Was the remaining runtime defect after `1c0db7e`.** The boot reported **5
unmapped calls, 0 inside `.text`**: `0x4b312b`, `0x4b319b`, `0x4b31a4`,
`0x487d16`, `0x5e7c7e`. All five had **one root cause**: the DirectDraw surface
COM vtable declared `Lock` with **4** stack arguments, but COM is `__stdcall`
and `this` is the first stack argument — the game pushes **5**
(`hEvent, flags, desc, rect, this`) at `0x487b31` with no `add esp` cleanup.
Each Lock leaked 4 bytes; the leak walked `0x487ad0`'s frame, EBP reached 0, the
"Allocating new surfaces" function took its skip branch, and the global
AlternateSurface `[0x714ed4]` was left null. The five calls then went through
that null surface (or through EBP=0).

**Fixed in `47d26eb`.** `IDirectDrawSurface::Lock` now pops 5. The retail oracle
corroborates: at `0x5e7857` the retail's `[0x714ed4]` is non-null and it executes
the fallback allocation at `0x4dfa0b`; the model now does both
(`0x714ed4 = 0x50007600`).

Result: **`boot.py --strict` exits 0**, **0 unmapped calls**, **0 stack
imbalances**. New stop: **divide overflow at `0x00422a5b`** (`idiv ecx`,
`ECX=0xffffffff`) after 16,318,827 instructions — a model defect, not the game
(see the `idiv` entry below).

### [x] The stack-imbalance tracker flags correct `__stdcall` stubs — fixed
**Was a diagnostic false positive at 111,682 stub calls.** `ts_call_at`
(`Machine.h`) compared ESP after a call to the ESP before it, but a correct
`__stdcall` stub ends at `esp_before + 4*nargs`.

**Fixed in `d047496`.** `stub_done` records the popped argument count and the
call depth; `ts_call_at` compares a **direct** stub call against
`esp_before + 4*nargs` and still requires `esp_before` for a recovered callee.
The depth test was necessary — without it, recovered callees that call a stub
internally still produced 54,527 false positives. Measured: **111,682 -> 0**.
Mutation proof: a stub body leaving ESP 4 bytes off reports 10,703 real
imbalances, and `--stack-check` still catches a body/table arity disagreement.
`tools/boot.py` now parses and forwards the bare `STACK IMBALANCE` header.

### [x] Signed `idiv` raises a false divide overflow — fixed
**Was the boot stop, and it was a model defect.** At `0x00422a59` the
instruction is `idiv ecx` with `ECX = 0xffffffff`. The true quotient for
`EDX:EAX = 0xffffffffffffffff`, `ECX = -1` is 1, but `ts::idiv32`/`divmod64`
(`src/handwritten/Machine.h`) ignored its `sign` flag, divided the unsigned
value by `-d` (=1), and raised "divide overflow".

**Fixed in `16e21fa`.** `divmod64` now honors `sign`: the signed path divides
magnitudes and applies signs (quotient truncated toward zero, remainder takes the
dividend's sign, `#DE` only for a zero divisor or a quotient outside
`[-2^31, 2^31-1]`), which also keeps `INT64_MIN / -1` out of the host's
undefined signed-division case. `div32`/`idiv32` are one-line wrappers. The
recovered stream contains only 32-bit `div`/`idiv` (no 8/16-bit forms), so there
is no live 8/16-bit defect. Tests: `test_machine_div` (the boot case, sign
combinations, overflow, divide-by-zero, and a 200,000-pair differential sweep)
and `test_win32_surface_lock` (slot 25, five arguments, ESP returns at
`caller_ESP + 0x14`), both mutation-proven. `test_tools.py` is now **94 checks**.

Measured with `TS_ROOT=.../gamedir`: the idiv is passed. New stop is **budget
exhaustion at `0x00488800` after 20,000,001 instructions** with `TS_CD`, depth
19, inside `sub_0048875c +0xa4`; `GetMessage=1 PeekMessage=5 DispatchMessage=1
CreateWindowExA=1`; coverage **55,774 / 4,764 of 45,947**; **0 unmapped calls,
0 stack imbalances**; `--strict` exits 0.

### [x] The game is alive in a dialog-setup loop — fixed
**Was the runtime state: no fault, no corruption, and no progress.** At 200M and
400M budgets the counters scaled exactly while coverage froze at 52,858 /
4,643 — a stable ~200M-instruction cycle. The dialog layer was fake:
`CreateDialogIndirectParamA` was `TS_RET(...,5,0x00010002)` (no window, no
`WM_INITDIALOG`), `GetDlgItem` returned a constant, and `PeekMessageA` always
returned 0, so `DispatchMessage` stayed at 1.

**Fixed in `f576607` + `c81c1f5`.** `Win32Stubs.h` now reads the module's own
PE `.rsrc` from disk: `FindResourceA`/`LoadResource`/`LoadStringA` serve
LANGUAGE.DLL's **60 dialog templates and string table**. `CreateDialogIndirectParamA`
builds the dialog record and its template controls, sends `WM_INITDIALOG`
(`wParam=0`, `lParam=dwInitParam`) *before returning*, and queues a scripted
`WM_COMMAND` for the default button; `GetDlgItem` returns stable per-control
handles (0 for an undeclared ID); `Set/GetWindowLongA` carry `DWLP_USER`;
`EnableWindow`/`IsWindowEnabled`/`ShowWindow`/`SetFocus`/`SetForegroundWindow`
keep state; `DestroyWindow`/`EndDialog` free children; `EnumChildWindows`,
`Set/GetDlgItemTextA`, `SendDlgItemMessageA`, `GetDlgCtrlID`, `GetClassNameA`
are real. A CD drive model (`TS_CD` maps `D:` to a host root, `DRIVE_CDROM`,
label `TS1`) was added, with a `win32-dialog` test (**71 checks at that commit;
now 72**).

**The infinite loop had a second cause:** `SetViewportOrgEx`/`SetWindowOrgEx`
take **4** arguments, and the stubs popped 3. Two calls leaked 8 bytes inside
`FUN_00659e90`, whose epilogue restored a stale EBX (`0xff`); `FUN_0057a260`
uses EBX as its null sentinel on the window-record hash walk and spun forever.

Measured: **`DispatchMessage=157`, `GetMessage=157`, `PeekMessage=1,411`**;
coverage **56,011 / 4,813**; stop `0x0057b6d1` (window-record hash insert);
0 unmapped, 0 imbalances, `--strict` exits 0. The dialog now shows its real
shipped text ("Please insert a Tiberian Sun CD into the CD-ROM drive."), and
what remains is the game's **genuine CD-absence retry**, which dispatches
messages. The next root cause is the narrow-width flag defect below.

### [x] Narrow-width flag helpers are wrong — fixed
**Was the next root cause after the dialog fix.** `Machine.h`'s `f_sub`/`f_add`/
`f_inc`/`f_dec`/`f_log` derived SF/OF from bit 31, while the 8/16-bit operations
passed a zero-extended result, so every `JLE`/`JG` on a byte was wrong. Two
measured consequences: `FUN_0068667c` (the game's own vsprintf) copied formats
literally (`GetDriveTypeA("%c:\")`, `CreateFileA("EXPAND%02d.MIX")`), and
`FUN_004a7600` (GetCDClass) stopped its `c:`..`z:` scan after `c`, so
`Force_CD_Available` re-prompted forever.

**Fixed in `f2e6ca9`.** `Machine.h` now has width-aware
`f_add_w`/`f_sub_w`/`f_inc_w`/`f_dec_w`/`f_log_w`; all 8/16-bit
arithmetic/comparison feeds the width, 32-bit is unchanged. `test_tools.py` is
now **72** checks and compares every flag against the CPU's own flags via inline
asm, including the `0x7f`/`0x80` boundary and the vsprintf `%` classification;
mutation-proven (18 assertions fail with the old derivation).

**`19ee16c`** then fixed the generator's hard-coded
`SetViewportOrgEx`/`SetWindowOrgEx` arity (4, not 3); the regenerated table diff
is exactly those two rows and `--stack-check` is 342/0.

Measured with `TS_CD` (the modelled `D:`): the CD path is correct end-to-end —
`CreateFileA("D:\TS1.dsk")` succeeds, `GetDriveTypeA` no longer receives `%c:\`,
and the game enumerates `EXPAND99..EXPAND00` (100 files), proving `%02d`. At 20M
the stop is `0x00488800` (`sub_0048875c +0xa4`), coverage **55,774 / 4,764**; at
80M the stop is `0x0064e156` (`sub_0064e14d +0x9`), depth 24, `GetMessage=1
PeekMessage=312,674 DispatchMessage=1 CreateWindowExA=1`, coverage **61,817 /
5,171**; 0 unmapped, 0 imbalances, `--strict` exits 0.

### [x] The intro-movie playback pump — fixed (frozen clock)
**Was the current blocker.** At 80M the stop was `0x0064e156`
(`sub_0064e14d +0x9`) inside the intro-movie pump `sub_0064dff0`
(loop `0x64e0b8..0x64e242`), with coverage frozen at 61,817.

**The stream callback at `+0x54` was a red herring.** It is `source[5]` of the
player's stream-source struct, initialized from the static table at `0x6e0850`
and never written by any instruction. The retail stream under Wine
(`STREAM=0x01a0e800` at `0x67ca4a`) also has `+0x54 = 0`, with `+0x84=8`,
`+0x88=1`, `+0x8c=2` — byte-for-byte the model's state. `sub_0067c920` treats a
null callback as "no frame callback" and continues into the decoder.

**The real cause was the model clock.** `tick()` was a static counter that only
`Sleep` advanced, and the movie pump polls `timeGetTime` in a busy loop and never
sleeps. The VQA pacer `sub_006786a0` compares `FUN_0067d580()` (which reads
`timeGetTime`) with `(frame+1)*60/fps`; frozen time returns **-10 ("too early")**
at `0x678804` forever, never reaching `0x678811`, the branch that hands audio to
the output callback and clears the descriptor's done bit (`[desc+0x14]=0`). The
decoder's top check then returns **-9** at `0x67c9ba` forever. Measured before:
`0x678811` hit once, `0x678804` hundreds of thousands of times.

**Fixed in `65f8606`.** `tick() = tick_base() + mach().steps/10000`, with `Sleep`
still advancing the base: deterministic, about 3x real time. A mutation-proven
`win32-clock` test is in `a2083d5`; `test_tools.py` is now **73 checks**.

Measured with `TS_CD`:
- **80M:** stop `0x0067ccb7`, coverage 61,986 / 5,178, `PeekMessage=23,002`.
- **200M:** the decoder runs and the first movie reaches its **last frame
  (130/130)**; stop `0x0067e0ab` in the VQA codec.
- **400M:** SHA-1 in the RNG seeder (`FUN_004dbe90`, "Init random number").
- **800M:** line-clip/draw `FUN_00676030` (`0x00676101`), coverage 67,144.
- **1.6B:** stop `0x004f698d`, coverage **78,070 / 6,166**. The game has opened
  **TIBSUN.MIX (75 MB)** and probed **`TITLE.PCX`/`LOCAL.MIX`** — main-menu asset
  initialization.

### [x] Main-menu asset init spins in a node list — fixed (Blt arity)
**Was the current blocker, after the movies complete.** At 1.6B the run stopped
at `0x004f698d`; a 3.2B budget reached the same EIP with identical coverage
78,070 / 6,166 and no new message pumping. The node list looked self-linked.

**The list was a symptom.** `IDirectDrawSurface::Blt` takes **6** words
(`this, dest, src, srcrect, flags, fx`); the slot stub popped **5**, so each of
the **210 Blt** calls before the menu leaked 4 bytes — **840 bytes of ESP
drift**. That drift zeroed two stack-relative fields at the menu transition:
`0x4db807` in `FUN_004da790` calls `FUN_005da290(1)`, which calls
`FUN_005da880` twice with different `this` (`0x712254`, `0x712628`); both
`[this+0x30]` fields read 0, so the second call re-inserted already-linked
nodes. `FUN_004b2f90 -> FUN_004f6a20 -> FUN_004f69b0` then returned the node
being inserted, setting its next pointer to itself, and `FUN_004f6980` walked
the self-loop forever.

**Fixed in `df6eb6b`** (Blt pops 6; `BltFast` too), with a mutation-proven test
in `59de3f2`. `dd_GetDisplayMode` now writes bpp at `+0x54` and caps at `+0x68`.
Isolation: Blt=6 alone removes the cycle; Blt=5 with the display-mode fix
reintroduces it exactly. The fixed model hits `0x4da790` once and never reaches
`0x5da880`, matching the retail probe.

**New stop: the model reaches the main menu.** At 1.6B it stops at
`0x004dd3d9` (`sub_004dd37f +0x5a`) inside `FUN_004dd2a0`, the **main-menu
input loop**; `GetMessage=1 PeekMessage=9,523,645 DispatchMessage=1
CreateWindowExA=1`; coverage 77,459 / 6,121; 0 unmapped, 0 imbalances. The
coverage is 611 instructions lower than the buggy run because the corrupted
list path is no longer executed. The retail probe hits the same loop
(`0x4dd2a0`, `0x4dd3d9`, key poll `0x4f1ca0`).

*Next:* integrate `Rendering.h`/`Input.h` so the menu renders and input can
leave the loop (the entry below).

### [x] Standalone rendering/input layer — built and tested
**Built ahead of the menu.** `Rendering.h` provides per-surface memory with
pitch and 16bpp RGB565, `Lock`/`Unlock`, `Blt`/`BltFast`/`Fill`/`Flip`,
`GetSurfaceDesc`/`GetCaps`/`GetPixelFormat`, palette support, and a PNG/PPM
frame dump. `Input.h` provides scripted key/mouse state, cursor, and synthesized
messages. Both are standalone and do not yet touch `Win32Stubs.h`.

Retail measurements (`+ddraw` trace and gdb at `0x4883f5`): **210 Blt** (197
copies flags `0x01000000`, 13 colour fills `0x01000400`), **195 Lock**
(`0x1`), 7 `CreateSurface`, **0 Flip, 0 SetPalette, 0 GetPixelFormat** before
the menu. Every surface is 16bpp RGB565 (`f800/07e0/001f`), pitch `width*2`;
primary 640x480, offscreen 64x64 / 472x480 / 168x480 / 640x480. The startup
colour fills use `dwFillColor = 0` (clear to black).

Tests: `tools/test_runtime.py` -> **RUNTIME TEST PASSED** (99 selftest checks,
PNG decoded independently, PPM pixel checked, two mutations caught). Synthetic
640x480 frame: `out/frame_selftest.png` md5 `4e2d4e579162a2a669caa2f49b75b16d`.
The exact integration patch is in `tools/test_runtime.py`'s docstring.

### [x] Integrate Rendering.h/Input.h into Win32Stubs.h — done
**The model renders the game's main menu.** The layer is wired through
`Win32Stubs.h`: `make_surface`/`dd_CreateSurface`/`dd_GetDisplayMode` go through
`ts::rnd`; surface slots 5 (Blt, 6 words), 7 (BltFast, 6), 11 (Flip, 3), 14
(GetCaps), 21 (GetPixelFormat), 22 (GetSurfaceDesc), 25 (Lock), 29
(SetColorKey), 31 (SetPalette), 32 (Unlock) have real bodies; input functions
and `PeekMessageA` go through `ts::input`; `SCREEN_W/H` are 640x480; `--frame`
dumps the primary surface.

Measured:
- `out/frame.png` (and `out/frame_input.png`): 640x480, md5
  `0fdf2636c2643ce2510ae67fb9ca9e26`, **254,610 / 307,200 non-black pixels**.
  It is the real main-menu art (title logo, GDI/Nod wheel, emblems).
- Retail comparison (`/tmp/retail.png`, md5 `58cb9c0b…`): 102,836 pixels differ
  at all (33.48%) but the **mean absolute channel difference is 1.06/255**;
  only **506 pixels differ by more than 8**, all in the bottom-right retail
  version text the model does not draw. The rest is RGB565 quantization.
- Boot, default scripted input, 1.9B: New Campaign clicked, menu exits result 1,
  `RT_DIALOG` 148 created at step 1,688,503,557; stop `0x0045fa8e` in its modal
  loop; `GetMessage=7 PeekMessage=14,572,933 DispatchMessage=7`; 78,504 / 6,191.
- Exit Game script, 1.9B: `ExitProcess(0)` at `0x00684529` after 1,707,666,895
  instructions; 86,400 / 6,654; 0 unmapped, 0 imbalances.
- Gates: `VERIFICATION PASSED`, **94** tool tests, 101 runtime checks,
  `--strict` exits 0.

**Scripted input is delivered but does not leave the menu.** Ten messages are
synthesized and dispatched; the WndProc feeds the game's key queue
(`0x710ca0`) and the menu loop consumes them. But exit needs `0x5898f0` to
return 1, which requires `[0x7b0d38]` nonzero and != 5 (it is 0), and the
selection flag `0x7b2ab8` has only its menu-init store (`0x4da7b8`). No menu
item was selected.

### [x] Menu-item selection and the nested-MIX button art — fixed
**The model leaves the main menu and opens "Select Campaign:".** LANGUAGE.DLL
`RT_DIALOG` **226** is the main menu: template 204x147 DLU, `DS_SETFONT` 8pt
"MS Sans Serif"; dialog units map by the measured 6x13 base unit
(`MulDiv(x,6,4)`, `MulDiv(y,13,8)`), so the dialog is 306x239, centred at
(167,120) and moved to **(167,187)**. Its six `BS_OWNERDRAW` buttons (class atom
0x80) and result codes:

| id | control | client rect (x,y,cx,cy) | screen rect | result |
|---|---|---|---|---|
| 0x617 (1559) | New Campaign | 56,20,195,29 | (223,207)-(418,236) | 1 |
| 0x619 (1561) | Load Mission | 56,54,195,29 | (223,241)-(418,270) | 2 |
| 0x61A (1562) | Multiplayer Game | 56,88,195,29 | (223,275)-(418,304) | 3 |
| 0x3EB (1003) | Intro / Sneak Peek | 56,122,195,29 | (223,309)-(418,338) | 4 |
| 0x55C (1372) | Options | 56,156,195,29 | (223,343)-(418,372) | 5 |
| 0x3EE (1006) | Exit Game | 56,190,195,29 | (223,377)-(418,406) | 6 |

The game subclasses each button at `0x57b2c0` (`FUN_0057a260`,
`SetWindowLongA(GWL_WNDPROC)`), lays it out on custom message `0x497`
(`FUN_0057e7c0`), and the dialog procedure `0x4dd670` handles only `WM_COMMAND`,
writing the result code through the `GWL_USERDATA` pointer `FUN_004dd2a0`
installs. The model now performs press/release -> `BN_CLICKED` in
`dialog_mouse`, so the click reaches the game's own `WM_COMMAND` path.

Measured (default timeline: `mousemove 320 220` + click):
- The click hits button `0x20003`, queues `WM_COMMAND id=1559`; the menu loop
  exits with result 1 and the game creates `LANGUAGE.DLL` `RT_DIALOG` **148,
  "Select Campaign:"** at step **1,688,503,557**.
- At 1.9e9 the stop is the budget at `0x0045fa8e` (`FUN_0045f960`, front-end
  frame) inside that dialog's modal loop: `PeekMessage=14,572,933`,
  `DispatchMessage=7`, input-target `0x2000b`, **0 unmapped**. Window table:
  dialog 148 at (135,187 370x243) with "Select Campaign:", listbox 1109,
  Cancel, OK, Slider3, Difficulty, Harder. (This was the state before the
  listbox layer; the mission-start entry below supersedes it.)
- Clicking **Exit Game** (screen 320,391) instead reaches `ExitProcess(0)` at
  `0x00684529` after 1,707,666,895 instructions — the game leaves the menu and
  exits.
- Frames: `/tmp/campaign_final.png` (menu background, 254,610 non-black; the
  renderer does not rasterise the dialog), `/tmp/exit.png` (0 non-black).

**Correction:** `0x7b2ab8` and `0x7b0d38` do not change. `0x7b2ab8` is the
front-end-active flag (still 1) and `0x7b0d38` the game mode (still 0 because
the game is still in the front-end). The selection signal is the dialog
procedure's result code through `GWL_USERDATA`, read by `FUN_004dd2a0`.

**The nested MIX is mounted and read.** `TIBSUN.MIX` entry `0xa8548fd9`
(18,044,736 B at offset 22,111,980) is a 317-entry archive containing 111 PCX
files, including the 176x29 button art. `TS_FILE_TRACE` shows the nested header
(4-byte flags, 80-byte key block, 8-byte index blocks — the Blowfish index
decrypt) and nested bodies at ~27.5M. The art is still not painted because the
model never sends `WM_PAINT` to child controls (`UpdateWindow` paints only the
passed window; `InvalidateRect` is a no-op), so the button subclass paint path
never runs. The retail also draws no buttons in this environment (its `+win`
trace shows the controls created, visible and laid out; the screenshot has no
buttons), so this is a separate failure that does not block selection.

### [x] The "Select Campaign:" listbox is empty — fixed; the mission starts
**The model now selects a campaign and starts the GDI mission.**

The list source is the shipped **`BATTLE.INI`** (inside `TIBSUN.MIX`, committed
as `re/realdata/BATTLE.INI`):

```
[Battles]
1=GDI1
2=NOD1
[GDI1] Description=GDI Campaign - Evolutionary Response
[NOD1] Description=Brotherhood of Nod Campaign - Deus ex Kane
```

The game's dialog procedure `0x004db8b0` (RT_DIALOG 148) feeds the listbox in
`WM_INITDIALOG`: for `DAT_007b0b38` entries it calls
`SendMessageA(listbox 1109, LB_ADDSTRING, 0, campaign[i]+0x268)` and then
`LB_SETCURSEL 0`; the strings are the campaigns' `Description` values (slot 25
of the campaign class at `0x004467d0`). **OK** (id 1) reads `LB_GETCURSEL` and
`TBM_GETPOS` (trackbar 1295).

Implemented in `Win32Stubs.h`: the full listbox message layer (ADD/INSERT/
DELETE/RESETCONTENT, GETCOUNT, GETTEXT/GETTEXTLEN, GETCURSEL/SETCURSEL/GETSEL/
SETSEL, FINDSTRING/EXACT, ITEMFROMPOINT, click-to-select with `LBN_SELCHANGE`)
and the trackbar (`TBM_SETRANGE/SETPOS/GETPOS`). `FindFirstFileA` now enumerates
the real host, and a read-only shipped-data root (`TS_DATA`, default `realdata`)
serves `BATTLE.INI`, since the CD's INSTALL tree has none.

Measured (default timeline: New Campaign, row 1 NOD1, row 0 GDI1, OK; with the
game's own `MixFileClass` lookup, `GDI1A.MAP` is in **MAPS01.MIX on CD1** and
`NOD1A.MAP` in **MAPS02.MIX on CD2**, so the default ends on GDI):
- After OK the campaign dialog is destroyed, `GDI1A.MAP` is read, and the **GDI
  campaign intro movie plays**. At 2.1e9 the stop is the budget inside the LCW
  video decoder (`0x0067dc4f`, `sub_0067daa0`), `pages-used 9029/32768`.
- The frame is the GDI intro title ("SEPTEMBER 2, 2030 / GDI SPACE STATION
  PHILADELPHIA"), **194,131 non-black pixels**.
- ESC at model-ms 192,000 skips the movie. At that time the load then faulted at
  step **2,580,698,410** at `0x00a006b0` with the fixed table exhausted
  (32768/32768); the growable page table and heap fix supersede that fault — the
  load now runs to 479B and fails on the wild-call defect recorded above.
- Frames: `/tmp/default_mission.png` (GDI intro), `/tmp/gdi_mission.png`
  (live-action briefing), `/tmp/gdi_skip.png` (290,399 non-black).
- Gates: `verify.py` PASSED, `test_tools.py` **94**, `test_runtime.py` 101,
  `--strict` 0, link OK.

### [x] The page table is exhausted during the mission load — fixed
**Was the current blocker.** `PageTable::SLOTS` was 32,768 (128 MB); the
`GDI1A.MAP` load touched every slot and faulted at `0x00a006b0` with
`pages-used 32768/32768`.

**The demand is legitimate and measured.** With temporary instrumentation
(per-page first-toucher, 64 MB region histogram, HeapAlloc size/caller
attribution) on the ESC-skip timeline:
- Peak **182,757 pages at 3.2e9 (748 MB, 3 growths)**; **377,796 pages at 4e9
  (1.44 GiB, 4 growths)**; growth is linear (~1 page per 4,100 instructions) and
  no fault occurs.
- **97%** of pages are touched inside `KERNEL32!HeapAlloc`; `VirtualAlloc` is
  3,568; 791 are pre-`insn()` startup pages.
- Of 7,353 fresh-page HeapAlloc calls (726,109,836 B), **5,664 calls /
  679,369,440 B (93.5%) come from the `DynamicVectorClass` resize at
  `0x0055b050`** (`code_0091.cpp:7808`): it allocates `capacity*8`, copies and
  frees the old buffer; sizes run 1,200-273,680 B.
- It is **not** a reuse failure: 7,859 HeapAlloc / 6,836 HeapFree, and the free
  list length was 0 at all 44 samples. A growing reallocation cannot fit the
  block it just freed; the bump cursor advances by the sum of intermediates.
  The retail peak for that vector is one allocation (<=273,680 B). The model has
  no decommit, so its footprint is every page ever touched, not the game's peak
  live set — legitimate for a 4 GiB address-space model.

**Fixed in `27e90dc`.** `PageTable` is now a growable open-addressed table:
doubles at 75% load up to 2^21 slots, rehashes deterministically in old-array
order, and **still refuses with a reported fault** (`page table exhausted`,
counted in `refused`) instead of aliasing. `page_in()` exposes the lookup for
tests; `page()` wraps the singleton. `boot_driver`/`boot.py` report
`used/cap/peak/growths/refused`. `test_page_table` covers growth past 32,768
with a byte-exact 40,000-page round-trip, reported exhaustion with live pages
untouched, stable identity across rehash, distinct host pages, and deterministic
layout; mutation-proven both ways. `test_tools.py` is **84 checks**.

Measured on committed code (4B budget, ESC at 192,000): stop `0x0055b0b8` in
`sub_0055b09a`, depth 29; **pages 377,796 of 524,288 (peak 377,796, growths 4,
refused 0)**; `GetMessage=19 PeekMessage=13,549,425 DispatchMessage=19`;
0 unmapped, `--strict` 0; render 8 surfaces, 12 blits, 0 fills, 769 locks,
0 flips; frame 640x480, 191,093 non-black.

**The mission still does not render** — see the two entries below.

### [x] The mission load is dominated by vector-growth reallocations — fixed
**Was measured as 5,664 `DynamicVectorClass` reallocations at `0x0055b050`,
93.5% of fresh-page HeapAlloc bytes (679 MB cumulative), driving the 1.44 GiB
footprint.** The model had no decommit, and the old heap could not reuse a
block whose successor was live.

**Fixed in `5d65211`..`7998c7a`.** The heap is now a contiguous,
address-ordered block chain with a doubly linked free list threaded through
free payloads: `HeapFree` coalesces with a free predecessor/successor;
`HeapReAlloc` grows in place into a free successor or the arena end and only
copies when it must; allocation splits put remainders back on the list.
`alloc_block` also reserves `[0x60000000,0x68000000)` so the surface arena can
never be overwritten. Semantics are unchanged (live blocks never overlap,
handed-out blocks read zero, `HeapSize`/contents preserved). Tests: **89
checks**, including `test_win32_heap` (realloc-in-place, coalescing, no
aliasing, contents, loader growth bounded, live-flag after absorbing a
successor) and `test_win32_heap_arena` (real primary surface + probe in the
reserved window + >256 MB allocation, both unchanged); both mutation-proven. A
60,000-op randomized stress test found 0 failures.

Measured at 4B: pages **377,796 (1.44 GiB) -> 12,809 (50.0 MB)**, growths
**4 -> 0**, 11,696 of 12,253 allocations reused (95.5%), peak live 30.3 MB;
the frame is the real GDI loading screen, md5 `6cb0e1ff…` (290,399 non-black),
byte-identical to `out/render_2_6b.png`. At 30B: 63,031 of 63,598 reused
(99.1%), footprint flat at 14,111 pages (55 MB).

### [x] Interpreter speed — 12-14x, target exceeded
**Was ~28.6M instructions/s; now ~400M/s, with byte-identical machine state.**
Measured on the mission-load run (`TS_ROOT`/`TS_CD`/`TS_DATA`, `esc_skip.txt`):

| budget | before | after | instructions/s |
|---|---|---|---|
| 4B | 137.1 s | **11.5 s** | 29.2M -> **347.8M** (11.9x) |
| 20B | 698 s | **50.2 s** | 28.65M -> **398.4M** (13.9x) |
| 30B | 1,048 s | **74.4 s** | 28.6M -> **403.2M** (14.1x) |
| 50B | — | 123.0 s | 406.5M |
| 100B | — | 243.2 s | 411.2M |

State is byte-identical at every budget: same stop EIP, registers, pages,
coverage and frame. Changes (all semantics-preserving): build at `-O2
-fno-pie`/`-no-pie`; a 256-entry direct-mapped page cache (positive hits only;
blocks never move on resize); single-page wide `ld16/st16/ld32/st32` with one
lookup + `memcpy` when not straddling a page; `insn()`'s cold path moved to
`noinline insn_slow()`; inlined page-cache hit, `page()` and flag helpers;
`count_of()` caches the last import name instead of `strcmp`-scanning 342 rows
per stub call. Rejected as measured-slower/larger: `always_inline` on `insn`
(234 MB binary) and on `ld32/st32` (114 MB, 5% for 2.2x size).

A perf regression test (`test_machine_perf`) pins the hot path: a 40M-iteration
workload, wide-vs-byte checksum `88188416`, page straddles, the watchpoint path,
a 20M/s floor, a cached-path >=1.4x `page_in_slow` ratio (measured 1.73-1.78),
and that `boot.py` keeps `-O2`/`-fno-pie`; mutation-proven. `test_tools.py` is
**94 checks**; `verify.py` PASSED, `test_runtime.py` 101, link OK, `--strict` 0.

### [ ] The 479B mission-load run goes wild
**The 1.2e12-budget run got much further, then faulted.** It ran
**478,970,534,425 instructions (479B)** and stopped at `0x00422de0`:

```
why:  page table exhausted: page allocation failed
eip:  0x00422ddd  add edi, dword ptr [edx + 0x10]
regs: eax=0x9caa856e ecx=0x9caa856f edx=0x9d65508c ebx=0x67e7da4d
      esp=0xf15d5f7c ebp=0xc3c48f90 esi=0x00000011 edi=0xc21638ed
pages: 1,024,211 of 2,097,152 slots (peak 1,024,211, growths 6, refused 1)
```

The registers are wild across the top of the 4 GiB address space. The run made
**7,771,444 unmapped calls**: **7 inside `.text`**, all targeting `0x00606060`
from `sub_004f3b40` (`0x004f3b5f`/`0x004f3b6d`); 7,673,235 below `0x1000`; 98,202
elsewhere. Coverage 158,971 / 9,630; render 9 surfaces, 171 blits, 10,484
locks, 0 fills/flips; **the frame is 0 non-black (black)**.

This is a new defect, not the O(n²) estimate failing: the loader ran far past
100B, then a pointer table or stack went wild and the game called through
`0x606060` (a repeated `0x60` byte pattern) and millions of below-`0x1000`
targets. The page-table refusal is one symptom; the first wild call predates
it.

**Context on the O(n²) loader** (still the cost): 22 `DynamicVectorClass`
objects at `this+0x1280` (base `0x710cc8`, stride `0x18`) in `sub_005a35b0`
(loop `0x5a35cb`..`0x5a3760`), called from `sub_005a1ba8` at `0x5a1bb8`; each
append goes through `SetCount` `0x55b050` -> `0x55b09a` and copies the whole
buffer. At 100B it was on vector 7/22 (160,070 entries). The old "tactical draw
at ~4.07B" was an artifact of the heap collision.

*Next:* find the **first** wild call, not the flood. Add a diagnostic for the
page-table refusal address; `TS_TRACE` `sub_004f3b40` and `0x00422de0`;
`TS_WATCH` the pointer table/stack slots that produce `0x606060` and the
below-`0x1000` targets. Determine whether (a) `0x606060` is a jump-table target
that should be registered (like the earlier switch-table fix), (b) the loader is
appending unbounded entries and overflowing a buffer, or (c) the 4 GiB address
space is genuinely exhausted by the model's arena/heap. Fix the first proven
cause, then re-run at ~500B (about 20 min at 400M/s).
*Done when:* the 479B wild-call flood is explained and gone, and the run reaches
the tactical phase or a new named stop with a non-black frame if it renders.

### [x] The mission writes pixels but never presents a frame — explained and fixed
**The blue banded/grid frame was raw heap memory.** The game *does* present the
mission **loading screen** at 2.6e9 (`out/render_2_6b.png`, md5 `6cb0e1ff…`,
290,399 non-black). It never presents a mission frame because the stub heap
bumps from `0x50000000` while `Rendering.h` placed surface pixels at
`0x60000000`: by ~2.722e9 instructions the heap had consumed 260.7 rows of the
primary. `out/render_partial.png` shows the boundary exactly — heap pattern
down to row ~259, a black zeroed block, then the intact loading screen.

Measured render path (`out/render_path.md`): 8 surfaces, all 16bpp RGB565;
primary `0x5060c470` at pixels `0x60002000` (640x480), 640x480 offscreen
`0x5060c544` at `0x6019ca00`, plus six others. In the load phase: 12 blits, all
`dst=primary`, `src=offscreen`, flags `0x01000000`, slot 5 Blt, call site
`0x0048823d` (`sub_00488029` from `sub_00487cf0`); 769 locks via
`sub_00487ad0` (gdb census: 251 primary, 507 offscreen, 4 one-time; caller
`sub_00675dc0`). **0 fills and 0 flips are the game's behaviour, not missing
slots**: it uses Blt colour-fill (none in this run; retail issues 15), Flip slot
11 is wired and counted, the gdb breakpoint is never hit, and the retail
`+ddraw` trace has 0 `Flip` in 9,854 lines. Presentation is Blt/Lock into the
primary plus `surface_update_frontbuffer` in retail.

**Fixed:** `Rendering.h`'s arena now starts at `0x10000000` (worst case 64
surfaces ends at `0x14b00000`), clear of the image, stack, heap and stub VAs.
**Verified:** the 2.7227e9 run after the arena move stops at `0x0055b0b8` with
67,169 / 131,072 pages (2 growths), 12 blits, 769 locks, and a frame whose md5
is **`6cb0e1ff…`** — byte-identical to the committed `out/render_2_6b.png`
loading screen (290,399 non-black). The heap no longer overwrites the
framebuffer. The heap lane is adding a durable reservation of
`[0x60000000,0x68000000)` in `alloc_block` plus a >256 MB allocation test that
asserts the primary pixels are unchanged.

**Measurement defect fixed:** `Machine.steps`/`step_limit`/`fault_steps` were
32-bit on the `-m32` build, so budgets ≥2^32 never fired and long runs reported
a wrapped count. They are 64-bit now; `boot_driver` parses `--budget` with
`strtoull` and prints `%llu`. The earlier "4.5e9" run actually executed
5,140,407,947 instructions and reported 845,440,651.

### [ ] The tactical phase is black and has 21 delta-table calls — not reached with correct memory
**The earlier measurement was taken under the heap collision and does not
reproduce.** With the arena/heap fixes, the loader is still in
`sub_0055b09a` at 30B instructions (see the entry above); it has not reached the
tactical draw (`sub_005d9990` → `sub_00593f30`). The old run reported 15 blits,
8,097 locks, a 0-non-black primary, 21 unmapped delta-table calls
(`0x000000db`, `0xffffff6a..6d`), and a 5.14e9 stop in `sub_00675ba0` with
`ESP=0x844`, `ESI=0x8ec`, `EAX=0xffffff6c`. Treat those as symptoms of the
corrupted loader buffers until re-measured with correct memory.

*Next:* after the speed work lets the load complete, re-measure this phase. If
the 21 delta-table calls persist, they are a real defect (an object overwritten
or a missing registration); if they vanish, they were the collision.
*Done when:* the tactical phase is reached with correct memory and either
renders a non-black frame or names the next defect precisely.

### [ ] The VQA player's frame-wait is stub-bound
**Secondary; skipping works.** The movie player's frame-wait loop makes ~68M
`PeekMessageA` calls in 600 s, so model time barely advances during movies. The
clock now moves with executed instructions, but the movie pacing still consumes
the budget. The committed `re/tools/input_scripts/esc_skip.txt` is the practical
route to the mission load until the clock/pacing model is improved.

### [x] The first unmapped calls — fixed
**The two in-`.text` targets from the leak era are gone.** `0x6844df` and
`0x46f3f8` disappeared with the stack-leak fix; the link test then reported
**0 unmapped calls inside `.text`** and passed.

Two outside-`.text` targets remained:
- `0x002fffe8` -- a stack address (the entry ESP is 0x002ffffc), so a data slot
  held a stack pointer where a code pointer was expected.
- `0xa000f9b6` -- inside the synthetic traced-thunk region (`kWrapperBase =
  0x00a00000`) but not a registered thunk, so it was a computed stub address
  that nothing bound.

Both were the same class: a stub that returned `S_OK` without writing its out
pointer. `1580be3` fixed `IDirectDraw::CreateClipper` (the `0x4876e4` call to
address 0) and `DirectSoundCreate` (the `0x484839` call to address 0; it now
returns `DSERR_NODRIVER`, matching the retail no-audio environment). That made
the boot report 0 unmapped calls and `boot.py --strict` exit 0 **at that
commit**.

**Reopened and re-closed.** After `f3011ca`/`345aae7` the boot reported **70
unmapped calls, 10 inside `.text`**; `47d26eb` fixed the single root cause (the
`IDirectDrawSurface::Lock` arity), and the boot now reports **0 unmapped calls,
0 stack imbalances**, `boot.py --strict` exits 0, and the link test passes.
See "The 5 null-pointer teardown calls — fixed" above.

### [x] Real file I/O
The stub layer used to make every file "exist" and read as empty, with
`GetFileSize`/`SetFilePointer` agreeing, so the lie was internally consistent and
the game could never load `rules.ini`, `TIBSUN.MIX` or the CD. Now: a host root
(`TS_ROOT`, default `../gamedir`), case-insensitive per-component path resolution
(Windows is case-insensitive; the game asks for `rules.ini` when the disk may hold
`RULES.INI`), a 64-entry handle table over `std::FILE*`, and real `CreateFileA` /
`ReadFile` / `WriteFile` / `SetFilePointer` / `GetFileSize` / `CloseHandle` /
`GetFileAttributesA` / `DeleteFileA`. A miss returns INVALID_HANDLE_VALUE and says
so, so a caller takes its real failure path instead of reading zeroes.
First run: `CreateFileA("\\.\MONO")` -- the sound device name.
*Note:* this did not itself move the stop; the instruction count rose from
1,011,817 to 7,038,765 when the stack leak was fixed, to 20,000,001 (the
budget, at the message loop) when COM registration was fixed, to the CRT exit
path once message delivery and Blowfish landed, to the dialog-setup loop after
the MIX-read and teardown fixes, to the intro-movie pump after the dialog and
narrow-flag fixes, through the **rendered main menu** and the campaign dialog,
and now into the **GDI intro movie** after the listbox/mission-start work
landed, then (with ESC) into the mission loader, which runs to **479B**
instructions before a wild-call fault. Use `TS_CD` to model `D:`.

### [x] Make the boot a one-command measurement
`tools/boot.py` builds (cached), runs with a budget and a timeout, and prints
instructions, depth, stop/fault EIP with instruction bytes, import calls by name,
the last N call targets, coverage, unmapped-target classification and the render
counters. Measured:
`python3 tools/boot.py` (with `TS_CD`) reports **20,000,001 instructions** (budget
exhausted at `0x00488800`, depth 19, `sub_0048875c +0xa4`), **message loop
reached and dispatching** (`CreateWindowExA=1`, `GetMessage=1`, `PeekMessage=5`,
`DispatchMessage=1`), **55,774 distinct instructions / 4,764 of 45,947 functions
entered**, and **0 unmapped calls, 0 stack imbalances**. With the default
scripted input at `--budget 2100000000` (`--timeout 600`) the timeline clicks New
Campaign, the menu exits with result 1, `RT_DIALOG` 148 is created at step
1,688,503,557, its listbox is populated from `BATTLE.INI`, GDI1 is selected and
OK starts the mission (`GDI1A.MAP` from `MAPS01.MIX`, then the GDI intro), and
the stop is `0x0067dc4f` in the LCW decoder (`GetMessage=17`,
`PeekMessage=13,565,102`, `DispatchMessage=17`; 84,130 / 6,431; pages used
11,058 / 32,768). An ESC-skip script at `--budget 3000000000` faults at
`0x00a006b0` after 2,580,698,410 instructions with the page table exhausted at
32,768 / 32,768 (139,223 / 8,678). The Exit Game script reaches `ExitProcess(0)`
at `0x00684529` after 1,707,666,895 instructions (86,400 / 6,654). `--frame`
dumps the primary surface. `--no-build --strict` is the gate:
it exits **0**. `--json`, `--watch`, `--imports N`, `--budget N`, `--rebuild`,
`--no-build`, `--input-script` and the `TS_CD` drive model are documented in the
tool's header.
*Done when:* `python3 tools/boot.py` reproduces the boot and reports it. **Met.**

## Standing caveats

- `VERIFICATION PASSED` means the tree compiles and that the generated headers
  agree with the recovered tables. It does **not** prove the disassembly is
  correct. Check 4 reads the binary, check 9 makes the compiler adjudicate
  layout, and `verify_members_binary.py` is a separate binary-anchored gate.
- The link test now **passes** (exit 0, 492 TUs, 20,000,001 instructions,
  message loop reached and dispatching); the boot has **0 unmapped calls, 0
  stack imbalances**, and `boot.py --strict` exits 0.
- **Message delivery is fixed** (`f3011ca`) and the **dialog layer is real**
  (`f576607` + `c81c1f5`): `CreateDialogIndirectParamA` reads LANGUAGE.DLL's own
  dialog templates, sends `WM_INITDIALOG`, and `DispatchMessage` runs. The
  narrow-width flag defect is fixed (`f2e6ca9`) and the CD path works end-to-end
  with `TS_CD` (`D:\TS1.dsk`, `EXPAND99..00`).
- **The model renders the real main menu.** `out/frame.png` is committed
  (640x480, md5 `0fdf2636c2643ce2510ae67fb9ca9e26`, 254,610 / 307,200 non-black
  pixels). Against `/tmp/retail.png` (`58cb9c0b…`), 102,836 pixels differ at all
  (33.48%), the mean absolute channel difference is **1.06/255 over the differing
  pixels** (0.35 over the whole frame), and only 506 pixels differ by more than
  8, all in the retail version text the model does not draw. The rest is RGB565
  quantization.
- **The menu is selectable and the game leaves it.** `DLGTEMPLATEEX` is parsed,
  the six `BS_OWNERDRAW` buttons have real rectangles, and press/release →
  `BN_CLICKED` reaches the game's own hit-test and `WM_COMMAND` path. New
  Campaign opens `RT_DIALOG` 148 "Select Campaign:" at step 1,688,503,557; Exit
  Game reaches `ExitProcess(0)` after 1,707,666,895 instructions.
- **The campaign list works and the GDI mission starts.** Dialog 148's listbox
  is populated from the shipped `BATTLE.INI` `[Battles]` (`1=GDI1, 2=NOD1`), the
  listbox/trackbar message layer is implemented, GDI1 is selected, and the game
  reads `GDI1A.MAP` from `MAPS01.MIX` and plays the GDI intro (frame
  `re/out/mission.png`, 640x480, 194,131 non-black, md5
  `ccdb46d448a90e487e513bc1daf2ff8b`).
- **The current blocker is the page table, and the mission load does not
  complete.** The default timeline stops at `0x0067dc4f` with 11,058 / 32,768
  pages used; an ESC-skip script faults at `0x00a006b0` (the `HeapAlloc` thunk)
  after 2,580,698,410 instructions with the table exhausted at 32,768 / 32,768.
  `PageTable::SLOTS` is the limit. The VQA frame-wait path is also stub-bound, so
  movie pacing is not retail pacing. The nested MIX (`0xa8548fd9`, 317 entries)
  is mounted and read; the button art is not painted (no `WM_PAINT` to children;
  retail does not draw it either).
- **The Blowfish cipher is implemented and proven** (`345aae7`), and the MIX
  index read is fixed: all 27 blocks are read and the 17 records match
  `tools/mix.py` 17/17.
- The retail oracle reaches the **main menu** (`tools/wine_env.sh`), but the
  main-menu buttons/text are not drawn because the PCX assets live inside a
  nested MIX entry, and there is no audio.
- Member offsets are 99.9% confirmed against the binary, but **layout is not
  verified**: whether the types and ordering are right is unchecked.
- Member names exist only where an INI key or string provides evidence. Never
  infer one.
- **"Function bodies are empty" was false and is retracted.** Every recovered
  function is transcribed in `src/generated/code/`: **45,947** functions
  (**43,557** starts + **2,390** tail entries), **912,879** instruction lines,
  one machine instruction per line against `ts::` in `Machine.h`. The per-class
  `.cpp` files are forwarders to those transcriptions, and `tools/link_test.py`
  links 492 translation units and runs the original entry point. What the
  project does not yet reproduce is *source-shaped* C++ (ROADMAP P3), not
  behaviour.
