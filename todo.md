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
QI/AddRef/Release); **6,954** virtual declarations in the headers (**4,496**
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

Current measured state: 43,564 spans, 46,008 transcriptions, 918,438/918,438
instructions modelled at 100%, 0 unmodelled, `.text` code coverage 96.09%. The
historical progression quoted in the previous version of this entry (41,822 ->
43,564 spans; 897,193 -> 918,438 instructions) is kept only as history.

The two residual in-`.text` targets this entry used to name — `0x6844df` (from
`0x5e7317`) and `0x46f3f8` (from `0x674bf6`), both RETURN ADDRESSES of indirect
calls — disappeared with the 108-byte stack-leak fix, and the link test passed
until the newly reachable shutdown code reintroduced **10 in-text unmapped
calls** (a separate teardown defect, queued in P4).
*Done when:* that is fixed and the link test passes. **Met for the recovery
defect; reopened as a teardown defect.**

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
empty definitions. `src/generated/code/` holds 46,008 transcriptions
(43,564 recovered function starts + 2,444 tail entries), one machine instruction
per generated line, 918,438 instruction lines in all; the per-class `.cpp` files
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
Done. Measured now: **43,564 spans**, **96.09%** of `.text` as recovered code,
**105,966 B (103 KB)** unattributed and classified as data/padding. The tool
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
`out/text_data.json`: 105,966 B unattributed, classified as 12 jump tables
(264 B), 7 switch index tables (349 B), 104,959 B of alignment padding in 12,606
runs, and 394 B in 55 runs reported verbatim rather than forced into a category.
99.99% of `.text` is attributed as code or data; 96.09% is recovered code.
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
- The generated headers declare **6,954** virtuals: **4,496 named** (64.7%) and
  **2,458 `vfunc_N`**. Headers declare only what a class introduces or
  overrides, which is why 6,954 is smaller than 9,810.
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
*Done when:* >=60% of the 6,954 header virtual declarations are named, each
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
`tools/test_tools.py`: **43 checks passed** (measured this session). Not
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
[1] standalone compile: 263/263
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
dispatch, and the Blowfish cipher (`345aae7`, below) replaced the identity copy.
The boot now stops in the CRT exit path with 70 unmapped calls (10 in `.text`);
see the open teardown entry.

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
regression test; `tools/test_tools.py` runs **43** checks.

Measured: the page-table exhaustion at `0x00a006b0` is **gone**. The run now
consumes the full 20,000,001-instruction budget in the CRT exit path at
`0x006844db` (`sub_00684486`, `_exit`/atexit walk); message loop
`GetMessage=2 PeekMessage=5 DispatchMessage=2 CreateWindowExA=1`; coverage
51,604 / 4,535 of 46,008. The game's own `SetKey(56, 82060323...)` and the
correct decryptions of the first index blocks are visible in `TS_FILE_TRACE`.

*Spec (kept for the MIX read path):* `re/out/blowfish_oracle.md`, produced by
`re/tools/oracle_blowfish.py` and `re/tools/wine_blowfish_trace.sh`.

### [ ] The MIX index read stops after 2 of 27 blocks
**The next measurable defect, after `345aae7`.** The game reads the TIBSUN.MIX
header and only **2 index blocks (16 bytes)** instead of 27. The 204-byte
record-table read at `0x547435` returns just one further 8-byte block: the pipe
chain (`0x5932d8 -> 0x5f1070 -> 0x423660`) moves one block per call, and
`0x547426` calls it once without looping or checking the count, so the 17-entry
table is built from ~10 valid bytes plus stale memory.

At step 1,886,937 the game posts `WM_DESTROY` (caller `0x5e7d29`, shutdown block
`sub_005e7cd9`), drains the loop, and WinMain (`0x5e5b80`, called from `entry`
at `0x6855b1`) returns at ~7.66M instructions. The CRT exit path is then entered
with ESP already corrupted (`0x11`; it was `0x2fff84` at entry), so the atexit
walk runs away for the remaining ~12.3M instructions.

*Next:* decide whether the short read is a model defect in the pipe/straw layer
or a transcription/control-flow defect at `0x547426` (the call should consume
204 bytes, not 8). Use `TS_FILE_TRACE=1`, `TS_TRACE` at `0x547426`/`0x547435`,
and compare against the retail sequence. Do not assume the cipher is still at
fault; it is proven against `mix.py`.
*Done when:* the 17-entry index is read correctly and the game proceeds past
the MIX header, with the next stop named.

### [ ] Shutdown corrupts ESP before the CRT exit path
**Measured, and separate from the MIX read.** WinMain returns at ~7.66M
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

### [ ] Null-pointer teardown calls and the 10 in-text unmapped calls
**Measured after `345aae7`; they did not disappear when the index decrypted.**
The original five `this == NULL` sites still fire twice each: `0x4b312b`,
`0x4b319b`, `0x4b31a4`, `0x487d16`, `0x5e7c7e`; new null sites are `0x5f1083`,
`0x5f1006`, `0x5e7cfe`, `0x5e8489`. The boot now reports **70 unmapped calls,
10 inside `.text`** (it was 5, none in `.text`), and `boot.py --strict` exits
non-zero. They are a teardown defect, not a consequence of the undecryptable
index.

**The 10 in-text targets split into two groups:**
- **9 are the CRT atexit walk** at `0x6844dd`, calling return addresses stored
  in the table (`0x6844df`, `0x5e867e`, `0x5e864a`, `0x682995`, `0x683fce`,
  `0x5e7d68`, `0x6864ba`, `0x46f8e9`). They are a symptom of the shutdown ESP
  corruption above; fix that first and re-measure before chasing them.
- **1 is `0x683c4c` from `0x423679`** (`sub_00423660 +0x19`), in the MIX/pipe
  read path. That one belongs with the short-index-read lane.

*Next:* fix the shutdown ESP corruption, re-run `boot.py --strict`, and see how
many remain. Then name the caller and the object whose construction failed for
each survivor; fix the null `this` or the missing registration.
*Done when:* 0 in-text unmapped calls and each null site named or fixed.

### [ ] The stack-imbalance tracker flags correct `__stdcall` stubs
**A diagnostic false positive, measured at 2,069 stub calls after `f3011ca`.**
`ts_call_at` (`src/handwritten/Machine.h`) compares ESP after a call to the ESP
before it, but a correct `__stdcall` stub ends at `esp_before + 4*nargs`
because it pops its own arguments. Every correct stub therefore looks like a
leak, and the real signal is buried. The project's stub-stack probe and the new
USER32 test's explicit `ESP == caller_ESP + 4*nargs` check are the reliable
arity gates today.

*Fix:* let the stub record the argument count it popped (a field set from
`stub_done`, or an expected-delta hook), and compare a stub call against
`esp_before + 4*nargs`; non-stub calls still require `esp_before`. Keep the
mutation that proves a wrong arity is caught.
*Done when:* a boot reports 0 false imbalances, and deliberately changing one
import's arity still fails the gate. This touches `Machine.h` and `stub_done`
in `Win32Stubs.h`, so it is sequenced behind the Blowfish change.

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

**Reopened by the newly reachable code.** After `f3011ca`/`345aae7` the boot
reports **70 unmapped calls, 10 inside `.text`** (33 below `0x1000`, 27
elsewhere); the link test **fails** on the 10 in-text targets, and
`boot.py --strict` exits non-zero (3). They are a teardown defect, tracked in
the open entry above ("Null-pointer teardown calls and the 10 in-text unmapped
calls"), not a regression of the `1580be3` fix.

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
budget, at the message loop) when COM registration was fixed, and the stop moved
to the CRT exit path (`0x006844db`) once message delivery and the Blowfish
cipher landed.

### [x] Make the boot a one-command measurement
`tools/boot.py` builds (cached), runs with a budget and a timeout, and prints
instructions, depth, stop/fault EIP with instruction bytes, import calls by name,
the last N call targets, coverage and unmapped-target classification. Measured:
`python3 tools/boot.py` builds (0 of 492 units compiled on a warm cache) and
reports **20,000,001 instructions** (budget exhausted at `0x006844db`,
`sub_00684486`, the `_exit`/atexit walk), **message loop reached and
dispatching** (`CreateWindowExA=1`, `GetMessage=2`, `PeekMessage=5`,
`DispatchMessage=2`), **51,604 distinct instructions / 4,535 of 46,008
functions entered**, and **70 unmapped calls (10 inside `.text`; 33 below
`0x1000`, 27 elsewhere)**. `--no-build --strict` is the gate: it exits
**non-zero (3)** while those remain. `--json`, `--watch`, `--imports N`,
`--budget N`, `--rebuild` and `--no-build` are documented in the tool's header.
*Done when:* `python3 tools/boot.py` reproduces the boot and reports it. **Met.**

## Standing caveats

- `VERIFICATION PASSED` means the tree compiles and that the generated headers
  agree with the recovered tables. It does **not** prove the disassembly is
  correct. Check 4 reads the binary, check 9 makes the compiler adjudicate
  layout, and `verify_members_binary.py` is a separate binary-anchored gate.
- The link test now **fails** (exit 1, 492 TUs, 20,000,001 instructions,
  message loop reached and dispatching) on **10 unmapped calls inside `.text`**;
  `boot.py --strict` exits **3**. They are the teardown defect queued above.
- **Message delivery is fixed** (`f3011ca`): `WM_ACTIVATEAPP` is queued, the
  WndProc at `0x65ba70` runs, `DAT_007b2e50` becomes 1, and the game leaves the
  `PeekMessage` spin. The current stop is the CRT exit path (`0x006844db`) with
  WinMain having returned at ~7.66M instructions with ESP `0x11` — the shutdown
  ESP corruption queued above.
- **The Blowfish cipher is implemented and proven** (`345aae7`): standard
  Blowfish-ECB, four known-answer vectors plus a byte-identical `tools/mix.py`
  cross-check on the shipped TIBSUN.MIX index (`2ce3171bdbecb40e` →
  `110090d97904849a`, count 17, body 75,094,416). The next content defect is the
  MIX index read (2 of 27 blocks), queued above.
- The retail oracle reaches the **main menu** (`tools/wine_env.sh`), but the
  main-menu buttons/text are not drawn because the PCX assets live inside a
  nested MIX entry, and there is no audio.
- Member offsets are 99.9% confirmed against the binary, but **layout is not
  verified**: whether the types and ordering are right is unchecked.
- Member names exist only where an INI key or string provides evidence. Never
  infer one.
- **"Function bodies are empty" was false and is retracted.** Every recovered
  function is transcribed in `src/generated/code/`: 46,008 functions
  (43,564 starts + 2,444 tail entries), 918,438 instruction lines, one machine
  instruction per line against `ts::` in `Machine.h`. The per-class `.cpp` files
  are forwarders to those transcriptions, and `tools/link_test.py` links 492
  translation units and runs the original entry point. What the project does not
  yet reproduce is *source-shaped* C++ (ROADMAP P3), not behaviour.
