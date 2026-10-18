# ROADMAP — the remaining trajectory to documented, modifiable, compilable source

Written after a full audit of the tree. Every figure below was measured, not
estimated; the commands that produce them are named. Re-measure before trusting
this file (`python3 tools/harness.py metrics --no-append`).

Figures in this file were re-measured on 2026-10-17 with `harness.py metrics`,
`verify.py`, `verify_members_binary.py`, `test_tools.py`, `boot.py` (and
`--no-build --strict`), `link_test.py`, `wine_env.sh`, and direct counts over
`out/` and `src/generated/`. The boot and recovery figures were re-measured
after the dialog, narrow-width-flag and generator-arity merges; the full
`pipeline.sh` run recorded in `todo.md` (exit 0, 8m51s) was not re-run here.

The point of this document is to say **what is actually left**, what the
*smallest* honest size of that work is, and in what order it can be done — not to
list everything that could conceivably be improved.

---

## 1. What "done" has to mean

The goal is a Tiberian Sun that is **documented**, **modifiable** and
**compilable**. Those are three different claims and they are nowhere near
equally satisfied. Stated so they can be checked:

| Claim | Done means | Measurable exit criterion |
|---|---|---|
| **Compilable** | The tree builds as 32-bit code with the game's ABI, and the compiler itself confirms the recovered memory layout | `tools/verify.py` compiles everything `-m32` and fails unless every recovered member offset is confirmed by `offsetof` |
| **Documented** | Every class, virtual and member is either named with cited evidence or explicitly marked unknown *with a reason* | 0 declarations without an evidence record; the `vfunc_N` remainder carries a per-slot reason |
| **Modifiable** | A human changes a function's *behaviour* by editing C++ statements, rebuilds, and sees the change — without touching machine instructions | ≥50% of instructions in the 500 highest-centrality functions absorbed into recovered C++ statements, each passing a differential test |

Nothing below counts as done until one of those commands changes its output.

---

## 2. Where the tree actually is

### 2.1 Recovery is effectively closed — this is not the problem any more

`.text` is 2,712,101 bytes. 112,401 bytes (110 KB) are unattributed. I read the
bytes in the largest gaps rather than assuming:

```
gap 0x663820 224B: c62a6600 4c376600 db2c6600 8e2a6600 ...   <- jump table
gap 0x45fef0  64B: c7fe4500 e3fe4500 d0fe4500 dafe4500 ...   <- jump table
```

Those are **switch tables**, plus alignment padding. The largest gap in the whole
binary is 224 bytes and there are **zero gaps larger than 512 bytes**. So the
"4% unattributed" is not unknown code — it is data living inside `.text`.
Function discovery is done.

| Item | State (measured) |
|---|---|
| Functions recovered | 43,557 spans (`harness.py metrics`: `functions`), 2,599,700 bytes = 95.86% of `.text` as recovered code |
| Residual `.text` | 112,401 B unattributed, classified in `out/text_data.json`: **213 static switch tables (6,388 B)**, 102 switch index tables (145 B), 105,492 B of alignment padding in 12,671 runs, and 376 B in 46 runs reported verbatim. 99.99% of `.text` is attributed as code or data. |
| Instructions in `out/functions.json` | 845,093 static instructions across 43,557 spans |
| Instructions with no faithful rendering | 0 (`grep ts::unmodelled src/generated/code` → 0) |
| Member offsets refuted by the binary | 4 of 4,289 (`ScoreTimeClass`, `ScorePrintClass`); 4,285 confirmed (99.9%) |

**Do not spend the next round on disassembly.** It is finished.

### 2.2 The ABI gap — RESOLVED, and recorded here as history

This phase is closed. The gate compiles at the target ABI (`-m32`), and the
toolchain requirement is documented in `AGENTS.md`: `verify.py` exits non-zero
when `g++-multilib` is absent rather than silently falling back to LP64.

The figures this section used to quote — 2,313 `unsigned long` declarations and
`ObjectClass` at 248 bytes against 164 — described the LP64 host build before
the gate asked for `-m32`. They are **retracted as current measurements**, not
replaced with different numbers for the same claim: the host-width problem they
described no longer exists in the build the gate runs. Measured now:

```
verify.py -m32 standalone headers : 263/263
verify.py -m32 combined TU        : PASS
```

### 2.3 The vptr-as-data-member defect — RESOLVED

An earlier version of this section called this "the real defect" and quoted
`offsetof(AbstractClass, m_000) == 20` against a recovered offset of 0. That was
a real defect, and the record is kept because it is instructive: the recovered
table contains vtable-pointer stores (763 of 4,289 entries, 620 of them at
offset 0), and emitting each as an ordinary data member made C++ count the vptr
twice and displace every member after it.

It is fixed. The generated headers **omit vptr stores** — the compiler supplies
the vptr — and check 9 now compiles an `offsetof` assertion for every recovered
offset a header can express:

```
[9] compiler-confirmed member offsets: PASS
    coverage: 3073/4289 recovered offsets asserted
    compiler-confirmed: 3073/3073
```

The 1,216 offsets with no assertion are itemised by reason in the check output
(743 vptr stores, 413 offsets past the declared extent, 44 undeclared, 10 with
no member offset, 6 headerless). The tree's memory layout is now enforced by the
compiler, not asserted in prose.

### 2.4 Semantics — the bulk of what is left, and smaller than it looks

**One measured set, and the three quantities it contains.** Earlier versions of
this file and `todo.md` used "1,500 named, 8,280 unnamed, 3,409 distinct"; that
mixed quantities and is retracted. Measured on 2026-10-17:

```
vtable (class, slot) instances in the manifest        9,810
named (class, slot) pairs in out/virtual_names.json   5,557  (56.6%)
distinct name strings among those pairs                 917
  excluding destructors and QI/AddRef/Release            281
classes with at least one naming record                  702
virtual declarations in the generated headers          6,954
  named declarations                                   4,496  (64.7%)
  vfunc_N declarations                                 2,458
```

The distinction, stated once: a **slot instance** is a `(class, slot)` pair in
the manifest (9,810). A **named pair** is one of those that carries a name
(5,557). A **distinct name** is a unique name string (917; 281 if destructors
and COM boilerplate are excluded). The headers declare only the virtuals a class
introduces or overrides, so they contain 6,954 declarations — fewer than 9,810
slot instances because 65.1% of slot instances are inherited. `harness.py
metrics` prints `virtual_names: 702`; that key counts **classes with a naming
record**, not distinct name strings, and should not be quoted as a name count.
The old "3,409 distinct" was an estimate of slot instances after inheritance
deduplication, not a name count, and is not used as a denominator here.

And the leverage is extreme — naming a base names its whole subtree. Re-derived
from `out/class_manifest.json` and `out/direct_bases.json` this session; the
metric is the base's slot count times (subclasses + 1):

| Class | slots | subclasses | slot-instances covered |
|---|---|---|---|
| `ObjectClass` | 105 | 20 | 2,205 |
| `AbstractClass` | 24 | 68 | 1,656 |
| `TechnoClass` | 214 | 5 | 1,284 |
| `MissionClass` | 133 | 7 | 1,064 |
| `RadioClass` | 135 | 6 | 945 |
| `FootClass` | 236 | 3 | 944 |
| `AbstractTypeClass` | 27 | 28 | 783 |
| `GadgetClass` | 31 | 20 | 651 |
| `ObjectTypeClass` | 40 | 14 | 600 |

The top 18 bases cover 13,011 slot-instances. The naming problem is the 2,458
`vfunc_N` declarations the headers still carry (and the 5,557 named pairs that
could be given better evidence), not 9,810. That is tractable, and it is where
the next documentation rounds belong.

Members: 4,289 recovered entries, 3,526 after removing 763 vptr stores, **291
named across 20 classes (8.3% of non-vptr members)**.

### 2.5 Modifiability — the hardest gap, and the one with no partial credit

**There is not one hand-written `.cpp` file in the tree.** The transcription is
45,947 generated functions (43,557 recovered function starts + 2,390 tail
entries), one instruction per generated line, 912,879 instruction lines in all:

```cpp
ts::push32(0x19u);          //   0x006829a5  push 0x19
ts::mov_r32(EAX.d, 0u);     //   0x006829a7  mov eax, 0
```

This is faithful and gated, and it is **not source**. A human can read it against
the recovered listing line for line, but cannot meaningfully *edit* it. The
hand-editable surface today is 49 headers under `src/handwritten/`; the other
headers under `src/include/` (264 total) and all the code are regenerated.

So: the tree is compilable and increasingly documented, and it is only
*marginally* modifiable. This is the long pole and it needs a different
technique from everything done so far — pattern recognition over the 848,822
static instructions rather than more recovery.

---

## 3. The trajectory

### P0 — Make "compilable" mean the game's ABI  [RESOLVED]

*Kept as history. Every item below is done and gating.*

- **0.0** Stop emitting members as `unsigned long`. **Done.** The figures this
  item used to quote (2,313 of 2,919) were measured on the LP64 host build and
  are retracted; they are not current.
- **0.1** Gate on `-m32`, fail closed when multilib is absent. **Done.**
  `verify.py` exits non-zero naming the missing toolchain.
- **0.2** Stop emitting vptr stores as data members. **Done.** The headers omit
  them; the recovered table still records 763 vptr stores (620 at offset 0),
  which is evidence, not declarations.
- **0.3** Generate `offsetof` assertions for every recovered member offset and
  gate on them. **Done.** Check 9 confirms **3,073/3,073** asserted offsets at
  `-m32`.

Exit met: `tools/verify.py` fails unless the compiler confirms every recovered
member offset it can express at `-m32`.

### P1 — Close recovery and hygiene

*Bounded and mostly mechanical.*

- **1.1** Emit the residual `.text` as data and classify the padding. **Done:**
  `tools/disasm.py` classifies it into 213 static switch tables (6,388 B), 102
  switch index tables (145 B), 105,492 B of padding and 376 B in 46 unclassified runs, and
  writes `out/text_data.json`.
- **1.2** Implement the unmodelled instructions. **Done:** 0 unmodelled; the
  model additions are in `src/handwritten/Machine.h`.
- **1.3** Fix the 4 refuted offsets. **Open:** `ScoreTimeClass` and
  `ScorePrintClass`, two offsets each, still refuted by
  `tools/verify_members_binary.py`.
- **1.4** Locate the destructors, or mark each explicitly rather than labelling
  it a stub. **Partly done:** 229 destructors are emitted as labelled stubs and
  counted in `src/generated/README.md`.
- **1.5** Get `out/functions.json` (67,670,646 bytes, over GitHub's 50 MB
  warning) out of the repository — release asset or compressed.
- **1.6** Decrypt the encrypted TS MIX header. **Done:** `tools/mix.py` decodes
  both index variants; header + padded index + body size equals the file size
  on all four encrypted archives.

Exit: `.text` coverage ≥99.5% with the remainder classified; 0 unmodelled; 0
labelled stubs; no tracked file over 50 MB. *Not met:* coverage is 95.86% as
code (99.99% attributed), and 229 stubs remain.

### P2 — Name the tree

*Bounded in size: 2,458 `vfunc_N` header declarations, 917 distinct names (281
semantic), 3,526 non-vptr members. This is the semantics layer, and it is where
council rounds fit — one lane per evidence source, with disjoint files.*

- **2.1** Build the inheritance-propagation engine: a name established on a base
  propagates to every override, with the evidence chain recorded per slot. 5,557
  `(class, slot)` pairs already carry a name; the engine is what turns 917
  distinct names into more of those pairs.
- **2.2** Work the leverage table in §2.4 in order, starting with `ObjectClass`,
  `AbstractClass`, `TechnoClass`, `MissionClass`, `RadioClass`, `FootClass`.
- **2.3** Evidence sources to mine, in rough order of yield: call-site argument
  and return shape; cross-references to the 1,876 recovered INI keys; the string
  table; the existing 917 names; RTTI. `tools/e2e_rules.py` validates names
  against shipped data — 280 of 291 (96.2%) resolve — and that validation is
  what keeps this honest.
- **2.4** Fix the arity check (`nargs` counts `this` for COM `__stdcall` but not
  for `__thiscall`) so check 8 can become a gate instead of a signal. Measured
  today: **331/1,116 match, 785 divergence**.
- **2.5** Member naming from INI store sites, extending `name_members.py` beyond
  the current 20 classes / 291 names.

Exit: ≥60% of the 6,954 header virtual declarations named with a per-name
evidence record; arity check gated; ≥50% of the 3,526 non-vptr members named.

### P3 — Source-shaping (the long pole)

*Open-ended in effort; it needs a new technique, and it is what "modifiable"
actually requires.*

- **3.1** Recover real signatures — parameters, types, return — from
  prologue/epilogue shape and call sites, not just arity.
- **3.2** Build an idiom lifter: recognise the standard MSVC 6 sequences
  (this-relative access, virtual dispatch through the vtable, switch tables,
  string and memcpy idioms) and emit C++ statements instead of instructions.
- **3.3** Build the check that makes lifting trustworthy: a lifted function must
  reach the **same register and memory state** as its transcription over random
  inputs. Without this, a lifter is a plausible-looking guess generator — and
  this project's whole method is that a claim is checkable.
- **3.4** Order by call-graph centrality, not alphabetically. Lift what a reader
  needs to understand the game first.

Exit: ≥50% of instructions in the 500 highest-centrality functions absorbed into
recovered statements, every lifted function passing the differential test.

### P4 — Documentation and reproduction

- **4.1** Per-class documentation: for any class, why every declaration is
  believed. The evidence already exists in `out/`; it is not yet readable.
- **4.2** The end-to-end test: load the real game data, run the entry point, and
  compare something observable against the original.

Exit: a documented command that shows the model doing something the original
does, and that fails if it stops doing it.

**4.2 is still NOT done, but message delivery now works and the run reaches the
CRT exit path.** A previous version of this file claimed the game "runs a frame
loop" with `PeekMessage=137,295`. That was retracted: the model let
`ExitProcess` return, so execution continued past a fatal exit and the "loop"
was one the real game never ran. The current claim is from a run that never
calls `ExitProcess`.

What is true, measured after the dialog, narrow-width-flag and generator-arity
merges: the game maps at its real VAs and resolves **341/341** PE
import-directory thunks (309 by name, 32 by ordinal, across 13 DLLs). The stub
table holds 342 rows because it also carries `COMCTL32!DllGetVersion`, which is
not an import — the CRT reaches it through `GetProcAddress`. With `TS_CD` (the
modelled `D:`) the CD path is correct end-to-end: `CreateFileA("D:\TS1.dsk")`
succeeds, `GetDriveTypeA` no longer receives `%c:\`, and the game enumerates
`EXPAND99..EXPAND00` (100 files). The dialogs are real (LANGUAGE.DLL's 60
templates and string table). At the default 20M budget the stop is `0x00488800`
(`sub_0048875c +0xa4`), depth 19, `CreateWindowExA=1`, `GetMessage=1`,
`PeekMessage=5`, `DispatchMessage=1`, coverage **55,798 / 4,767**. At
`--budget 80000000` the stop is `0x0064e156` (`sub_0064e14d +0x9`), depth 24,
`PeekMessage=312,674`, coverage **61,817 / 5,171**, frozen from 40M. **0 unmapped
calls, 0 stack imbalances**; `boot.py --strict` exits **0** and the link test
passes. The only message box is the expected DirectSound warning at `0x484456`
(`type=0x30`), and the primary-surface check passes (`TS_PEEK=0x006beed8` → 1).

**The recovery was corrected (`9e5703f` + `ad62afc`).** 213 static switch tables
are now data (was 12), including three hole tables (`0x683c10`, `0x683d98`,
`0x68ce40`). Recovered functions **43,557**, transcriptions **45,947**,
instruction lines **912,879**, `.text` code **95.86%**, jump tables **6,388 B /
213**. Every lost instruction was classified. This also fixed the MIX index read:
all 27 blocks are read and the 17 records match `tools/mix.py` 17/17.

**COM registration is fixed (`1580be3`).** The game registers Westwood's
`BLOWFISH.DLL` class `{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}`; `LoadLibraryA` now
returns a named module handle with a per-module export table, `blowfish.dll`
exports `DllRegisterServer`/`DllUnregisterServer`/`DllGetClassObject`/
`DllCanUnloadNow`, `CoCreateInstance` writes an `IBlowfish` object with an
eight-slot vtable recovered from `BLOWFISH.DLL`, `IDirectDraw::CreateClipper`
writes a real clipper, and `DirectSoundCreate` returns `DSERR_NODRIVER` (no
audio, matching retail).

**Message delivery (`f3011ca`), the Blowfish cipher (`345aae7`), the stack
tracker (`d047496`), the null teardown calls (`47d26eb`) and signed `idiv`
(`16e21fa`) are fixed.** `CreateWindowExA` queues `WM_ACTIVATEAPP`; the WndProc
runs and the game leaves the spin. `IBlowfish` is standard Blowfish-ECB, proven
by four known-answer vectors and a byte-identical `tools/mix.py` cross-check on
the shipped TIBSUN.MIX index (`2ce3171bdbecb40e` → `110090d97904849a`, count 17,
body 75,094,416); the page-table exhaustion at `0x00a006b0` is gone. The stack
tracker went from **111,682 false imbalances to 0**, and the five null teardown
calls had one root cause — `IDirectDrawSurface::Lock` popped 4 arguments instead
of 5 — now fixed, so the boot has **0 unmapped calls** and `--strict` exits 0.

**The dialog layer is real (`f576607` + `c81c1f5`), and narrow-width flags are
fixed (`f2e6ca9` + `19ee16c`).** `Win32Stubs.h` reads LANGUAGE.DLL's own PE
`.rsrc` (60 dialog templates and string table); `CreateDialogIndirectParamA`
builds the record and controls, sends `WM_INITDIALOG`, and `DispatchMessage`
runs. A CD drive model (`TS_CD`) was added. `SetViewportOrgEx`/`SetWindowOrgEx`
take 4 arguments (the stubs popped 3, leaking 8 bytes); `19ee16c` fixes the
generator table (`--stack-check` 342/0). `Machine.h` has width-aware
`f_add_w`/`f_sub_w`/`f_inc_w`/`f_dec_w`/`f_log_w`, so 8/16-bit flags are correct;
the tests compare them against the CPU's own flags. `test_tools.py` is 72 checks.

**The current blocker is the intro-movie playback pump.** At 80M the stop is
`0x0064e156` (`sub_0064e14d +0x9`) inside `sub_0064dff0` (loop
`0x64e0b8..0x64e242`). The VQA decoder never executes: the stream object
(`ESI=0x508991e0`) has `[+0x150]=0x18`, so `sub_0067c920` skips setup, reads the
frame callback `[+0x54]` (NULL), and returns -1, which the pump retries.
`TS_WATCH` on `+0x54` records no store. The stream is otherwise alive (read
callback `+0x44=0x64e920`, buffer `+0x764`, dimensions 800x480). This is the
unreconstructed MS*/AVI layer, not a missing import. The movie path is **not
working**, and the menu is not reached.

**The oracle premise is now inverted.** `tools/wine_env.sh` gives the retail
binary an X screen at 640x480x16 and a `DRIVE_CDROM` `d:` labelled `TS1` holding
`TS1.DSK`; measured this session, it dismisses the DirectSound warning, plays
`WWLOGO.VQA`/`STARTUP.VQA`, and reaches its **main menu**, still running after
60 s (screenshot md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`). The earlier claim that
it "gives up early" is retracted. Remaining oracle limits: main-menu buttons/text
are not drawn because the PCX assets live inside a nested MIX entry, and there is
no audio. So the model reaches the loop but not the menu, and the oracle is the
target.

Ranked next steps (see `STRATEGY.md` for the full plan and exit criteria):
  1. Implement the MS*/AVI movie-stream layer the intro pump expects: make the
     VQA decoder (`0x679090`/`sub_00679360`) execute, which means giving the
     stream object a frame callback at `[+0x54]` and the state `sub_0067c920`
     expects, then frame timing and surface presentation. Exit: the decoder
     executes, coverage advances past 61,817, and the next stop is named.
  2. Make the model reach the main menu: real rendering (per-surface memory and
     real `Blt`/`Flip`), the nested-MIX PCX assets, and the stubs the menu path
     needs. The retail oracle now shows the target.
  3. Instruction-level differential: `tools/oracle.py 0xADDR --compare` at
     checkpoints the model and the retail binary both reach.
  4. Keep the shutdown ESP defect (`0x11` on WinMain return) queued: it is
     dormant now but will matter as soon as shutdown is reached again.

**The lesson worth keeping from this phase.** The game exits immediately unless
`comctl32!DllGetVersion` reports >= 4.00.70, and it reaches that function through
`GetProcAddress`, not the import table -- so there was no stub, the version read
as 0, and the game took its own legitimate-looking failure path. It was telling us
why it would not start the whole time, in a message box whose text is unreadable
because the string table is empty at that point. A stub returning a plausible
wrong value is harder to diagnose than one that fails loudly.

The honest measure of progress here is not "does it run" but **how far into the
game's own code it gets before it needs something we have not modelled**. The
measured figure is **20,000,001 instructions** (the budget) ending in the
intro-movie playback pump at `0x00488800` (and `0x0064e156` at an 80M budget),
after the dialog, CD, Blowfish and narrow-flag fixes. Before the stack-leak fix
it was 1,011,817 instructions ending in `ExitProcess(1)`.
Each stop is a precise, actionable finding rather than a wall, because the
harness reports the stop EIP, the instruction bytes there, the containing
function and generated source line, and the ordered import calls -- and
`tools/oracle.py` can read the same registers from the retail binary.

Two lessons from getting this far, both recorded in AGENTS.md:

- **The model's page table was silently corrupting long runs.** 8192 slots could
  not hold the game's working set, and a full table made `page()` return one
  shared dummy page for every new address. Every distinct page overwrote the same
  4 KB. The only symptom was that it got slow. Found with `perf`, not by reading
  code.
- **A stub that lies is worse than a stub that fails.** Unimplemented imports go
  through a logged fallback and every call is counted, so a gap is visible.

---

## 4. Critical path, and what is deliberately not on it

**P0 → P1 → P2 → P3.** P0 is resolved. P1 is small and mostly closed. P2 is
bounded in item count and is where parallel rounds pay. P3 is the long pole.
`STRATEGY.md` argues that for *playability* none of P2/P3 is on the path; this
file is the source-artifact roadmap, and the two are deliberately different
products.

Not on the critical path, and should not be worked on: more disassembly
(§2.1), more class discovery (749/749 with every constructor a real entry
point), more header coverage.

## 5. Honest risks

- **P3 may not recover the original source's shape, only *a* correct shape.**
  Lifting cannot prove it found the author's intent. The honest claim is
  behavioural equivalence with the machine, and the differential test is the
  strongest available evidence for it. This limit will not go away.
- **P0.2 required redesigning member emission, not patching it** — and that is
  what happened. The headers omit vptr stores and check 9 confirms the offsets
  the compiler can express (3,073/3,073). The recovered table still records 763
  vptr stores; they are evidence for the constructor scan, not declarations.
- **`-m32` becomes a build requirement.** Anyone without `g++-multilib` cannot
  pass the gate. That is the correct trade — a silent skip is what produced the
  unlinkable tree — but it must be documented, not discovered.
- **Naming effort is not linear.** The first names on `ObjectClass` are cheap
  (string cross-references); the last ones in a 214-slot class may each need
  reading a function. The leverage table in §2.4 is a *floor*, not a promise.
- **Everything here assumes the recovered offsets are right.** 4 of 4,289 are
  refuted (99.9% confirmed). P0.3 did not show the table is systematically
  wrong, so P2's member work is not blocked.
