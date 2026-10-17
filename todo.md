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
instructions modelled at 100%, 0 unmodelled, `.text` code coverage 96.09%, and
**0 unmapped calls inside `.text`** (link test). The historical progression
quoted in the previous version of this entry (41,822 -> 43,564 spans; 897,193 ->
918,438 instructions) is kept only as history.

The two residual in-`.text` targets this entry used to name — `0x6844df` (from
`0x5e7317`) and `0x46f3f8` (from `0x674bf6`), both RETURN ADDRESSES of indirect
calls — disappeared with the 108-byte stack-leak fix. The link test now passes;
the 2 remaining unmapped calls are outside `.text` (see the P4 entry).
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
`tools/test_tools.py`: **40 checks passed** (measured this session). Not
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
so the two call paths balance the same way. Measured after the fix: the game
reaches its own error box with `MessageBoxA` type **0x30**, calls
**`ExitProcess(1)`**, and stops after **7,038,765 instructions** with **0
unmapped calls inside `.text`**. The old corrupted values (`0x6844df` for `1`,
`0x00a00201` for `0x30`) are gone; exit 1 is now the game's legitimate failure
path, not a corrupted one.

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
Measured: `link_test.py` exits **0**, reports `MESSAGE LOOP: not reached (no
GetMessage/PeekMessage call)`, 7,038,765 instructions, 2 unmapped calls (0 in
`.text`), `ts_link_test: OK`.

### [x] `boot.py`'s fresh-checkout header sync — fixed
`60dac16` runs reconcile + self-contained after copying the hand-written headers,
so a fresh checkout no longer gets 140 raw headers copied over the reconciled
ones. Measured: `python3 tools/boot.py` builds (0 of 492 units compiled on a warm
cache) and reports.

### [ ] `blowfish.dll!DllRegisterServer` fails, so the game takes its error box
**The current boot stop, measured.** The game reaches its own `MessageBoxA` at
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

*Next:* give `LoadLibraryA`/`GetProcAddress` a named export table for
`blowfish.dll` (at minimum `DllRegisterServer`/`DllUnregisterServer` returning
`S_OK`), and make `CoCreateInstance` write a fake COM object with a real vtable.
Then boot; the next stop should be the primary-surface check, whose descriptor
offsets are already fixed (`138718a`: bpp at `+0x54`, `lpSurface` at `+0x24`).
If the game actually calls Blowfish methods to decrypt MIX data, those methods
need behaviour, not just `S_OK`.
*Done when:* the registration flag stays 0 and the run reaches the next precise
stop (or the message loop).

### [x] Write `DDSURFACEDESC` fields where the game reads them
The game's `DSurface` wrapper (`0x4874c0`/`0x487ad0`) reads `dwRGBBitCount` at
`desc+0x54` and `lpSurface` at `desc+0x24`, and computes bytes-per-pixel as
`(bpp + 7) >> 3`; it requires 2 for 16bpp. The stubs wrote the bit count at
`+0x68` and `lpSurface` at `+0x14`, so the primary-surface check saw 0. Fixed in
`138718a` against the game's own code. This did not move the current stop
(COM registration fails earlier), but it is the next runtime check.

### [ ] Reach a window and a message loop
**NOT reached.** This entry previously said "Reached. CreateWindowExA=1,
PeekMessage=137,295" -- that was false, and it was false for an instructive
reason: the model let `ExitProcess` return, so execution walked past a fatal exit
and the "loop" was one the real game never ran. `ExitProcess` now halts the run
in both `boot_driver` and the link test, and the measurement is
`GetMessage=0 PeekMessage=0 DispatchMessage=0 CreateWindowExA=0`.

The one real unlock on this path is worth keeping: `comctl32!DllGetVersion` was
reached through `GetProcAddress` rather than the import table, so there was no
stub, the version read as 0, the CRT's version check failed and the game called
`ExitProcess(1)` before initialising anything. One export reporting 4.72 fixed it.

Measured coverage now: **44,246 distinct instructions; 4,211 of 46,008 recovered
functions entered**; 2,046 import calls over 341 traced imports (64 distinct);
2,046 calls over 25 distinct imports in the link test. The next stubs that
matter: `RegisterClassA`, `CreateWindowExA`, `PeekMessageA` (must return 0 when
there is no message -- that is what keeps a game loop running), `GetMessageA`
(must not return 0 immediately, which looks like WM_QUIT), `GetSystemMetrics`,
`DirectDrawCreate` (must return a plausible object with a vtable, because the
game's own wrapper calls through it).
*Done when:* the run reaches a message loop and keeps executing frames rather
than returning.

### [ ] The 2 unmapped calls, now outside `.text`
**The two in-`.text` targets are gone.** `0x6844df` and `0x46f3f8` disappeared
with the stack-leak fix; the link test now reports **0 unmapped calls inside
`.text`** and passes. Two unmapped targets remain, both outside `.text`:

- `0x002fffe8` -- a stack address (the entry ESP is 0x002ffffc), so a data slot
  holds a stack pointer where a code pointer was expected.
- `0xa000f9b6` -- inside the synthetic traced-thunk region (`kWrapperBase =
  0x00a00000`) but not a registered thunk, so it is a computed stub address
  that nothing bound.

`python3 tools/boot.py --strict` exits **4** on them
(`boot: --strict: 2 garbage call target(s) remain`); the boot reports
`unmapped targets: below 0x1000=0 .rdata=0 elsewhere=2`.
*Done when:* both are named or gone, and `boot.py --strict` exits 0.

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
1,011,817 to 7,038,765 only when the stack leak was fixed.

### [x] Make the boot a one-command measurement
`tools/boot.py` builds (cached), runs with a budget and a timeout, and prints
instructions, depth, faults with EIP and instruction bytes, import calls by name,
the last N call targets, and (new) coverage and unmapped-target classification.
Measured: `python3 tools/boot.py` builds (0 of 492 units compiled on a warm
cache), runs in 0.2 s wall, and reports 7,038,765 instructions, message loop not
reached, 44,246 distinct instructions, 4,211 of 46,008 functions entered, and 2
unmapped calls (0 in `.text`). `--strict` is the gate: it exits 4 while those 2
remain. `--json`, `--watch`, `--imports N`, `--budget N`, `--rebuild` and
`--no-build` are documented in the tool's header.
*Done when:* `python3 tools/boot.py` reproduces the boot and reports it. **Met.**

## Standing caveats

- `VERIFICATION PASSED` means the tree compiles and that the generated headers
  agree with the recovered tables. It does **not** prove the disassembly is
  correct. Check 4 reads the binary, check 9 makes the compiler adjudicate
  layout, and `verify_members_binary.py` is a separate binary-anchored gate.
- The link test now **passes** (exit 0, 492 TUs, message loop not reached), but
  the boot still has **2 unmapped calls outside `.text`**
  (`0x002fffe8`, `0xa000f9b6`); `boot.py --strict` exits 4 on them.
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
