# PROGRESS — Tiberian Sun reconstruction

All figures below were re-measured on 2026-10-18, after merging the campaign
selection/mission-start work, with `python3 tools/harness.py metrics --no-append`,
`python3 tools/verify.py src/include build/shim`,
`python3 tools/verify_members_binary.py`, `python3 tools/test_tools.py`,
`python3 tools/test_runtime.py`, `python3 tools/boot.py` with `TS_CD` (20M, 2.1B
with the default campaign timeline, the ESC-skip script, and `--strict`),
`python3 tools/link_test.py`, `tools/wine_env.sh`, and direct counts over `out/`
and `src/generated/`. `TS_ROOT` selects the ordinary file root; `TS_CD` models
the `D:` CD drive (`DRIVE_CDROM`, label `TS1`); `TS_DATA` (default `realdata`)
serves shipped control files such as `BATTLE.INI`. Re-run those rather than
trusting this file.

**This file records state. `ROADMAP.md` records the plan** — what "done" means
for documented/modifiable/compilable as measurable exit criteria, the corrected
size of each remaining problem, and the phase order. Read the roadmap to decide
what to work on.

## Status summary

**`tools/verify.py` prints VERIFICATION PASSED across nine checks.** Check 9 is
the one that matters most: it makes the compiler confirm every recovered member
offset it can express, and it passes 3,073/3,073. **The link stage is green**:
measured this session, `link: OK (492 translation units)`, `ts_link_test: OK`,
exit **0**, 20,000,001 instructions, `MESSAGE LOOP: reached -- GetMessage 1,
PeekMessage 5, DispatchMessage 1`, 0 unmapped. `test_tools.py` runs **94**
checks and `test_runtime.py` **101** (mutation-proven). A full
`./tools/pipeline.sh` ran **exit 0** in the disasm-correction lane (8m51s,
recorded in `todo.md`); it was not re-run in this worktree. The link was the
expensive stage when last timed (about 471 s).

**The model renders the real main menu, and the menu is selectable.** The image
maps at its real VAs and all **341/341** PE import-directory thunks are written
to stubs (`link_test.py`: `imports: 341 across 13 DLLs`, `resolve_imports() ...
341 of 341 IAT slots written`, `traced thunks: 341 imports bound, 0 with no
stub`). With `TS_CD` (the modelled `D:`), the CD path is correct end-to-end
(`D:\TS1.dsk`, `EXPAND99..EXPAND00`, 100 files). The dialogs are real
(LANGUAGE.DLL's 60 templates and string table), and rendering/input are
integrated through `ts::rnd` (`Rendering.h`) and `ts::input` (`Input.h`):
surfaces are per-surface RGB565 memory, `Blt`/`BltFast`/`Fill`/`Flip`/`Lock` are
real, `SCREEN_W/H` are 640x480, and `--frame` dumps the primary surface. The
committed `re/out/frame.png` is 640x480, md5
`0fdf2636c2643ce2510ae67fb9ca9e26`, with **254,610 / 307,200 non-black pixels**;
it shows the title logo, GDI/Nod wheel and emblems. Against the retail screenshot
(`/tmp/retail.png`, md5 `58cb9c0b…`), **102,836 pixels differ at all (33.48%)**,
the mean absolute channel difference is **1.06/255 over the differing pixels**
(0.35 over the whole frame), and only **506 pixels differ by more than 8**, all
in the bottom-right retail version text the model does not draw; the rest is
RGB565 quantization.

**The model leaves the menu and opens "Select Campaign:".** LANGUAGE.DLL
`RT_DIALOG` 226 is the menu (306x239 at (167,187)) with six `BS_OWNERDRAW`
buttons: New Campaign (1559, result 1), Load Mission (1561, 2), Multiplayer Game
(1562, 3), Intro/Sneak Peek (1003, 4), Options (1372, 5), Exit Game (1006, 6).
The model performs press/release → `BN_CLICKED` in `dialog_mouse`, so the click
goes through the game's own hit-test and `WM_COMMAND` path. The default timeline
clicks New Campaign: the menu exits with result 1 and the game creates
`RT_DIALOG` **148, "Select Campaign:"** at step **1,688,503,557**. Clicking Exit
Game instead reaches **`ExitProcess(0)`** at `0x00684529` after **1,707,666,895**
instructions. **Correction:** `0x7b2ab8` (front-end-active) and `0x7b0d38` (game
mode) do not change; the selection signal is the dialog procedure's result code
through `GWL_USERDATA`.

**The campaign list works and the mission starts.** The list source is the
shipped `BATTLE.INI` (inside `TIBSUN.MIX`, committed as `re/realdata/BATTLE.INI`):
`[Battles] 1=GDI1, 2=NOD1` with `Description` strings. The game's dialog
procedure `0x004db8b0` feeds listbox 1109 via `LB_ADDSTRING` in `WM_INITDIALOG`
and sets `LB_SETCURSEL 0`; OK reads `LB_GETCURSEL` and `TBM_GETPOS` (trackbar
1295). The model implements the full listbox/trackbar layer, real `FindFirstFileA`
host enumeration, and a read-only `TS_DATA` root (default `realdata`) serving
`BATTLE.INI`. The default timeline selects NOD1 then GDI1 and clicks OK; the
campaign dialog is destroyed, **`GDI1A.MAP`** is read from **MAPS01.MIX on CD1**
(`NOD1A.MAP` is in MAPS02.MIX on CD2), and the **GDI intro movie plays**. At 2.1e9
the stop is the LCW video decoder at `0x0067dc4f` (`sub_0067daa0`),
`pages-used 9029/32768`; the frame is the GDI intro title ("SEPTEMBER 2, 2030 /
GDI SPACE STATION PHILADELPHIA"), **194,131 non-black pixels**
(`re/out/mission.png`). The nested MIX (`TIBSUN.MIX` entry `0xa8548fd9`,
317 entries, 111 PCX) is mounted and read; the button art is not painted because
no `WM_PAINT` reaches the child controls (retail does not draw it either).

**The interpreter is 12-14x faster, with byte-identical machine state.**
Measured on the mission-load run (ESC-skip): 4B in **11.7 s** (341.9M
instructions/s), 20B in **50.2 s** (398.4M/s), 100B in **243.9 s** (410.0M/s);
the old rate was ~28.6M/s. The changes are semantics-preserving: `-O2
-fno-pie`/`-no-pie`, a 256-entry positive-hit page cache, wide single-page
`ld16/st16/ld32/st32`, a `noinline insn_slow()` cold path, inlined page and flag
helpers, and a cached import-name lookup. `test_machine_perf` pins the hot path
and is mutation-proven.

Boot states, all with **0 unmapped calls and 0 stack imbalances**:
- 20M: stop `0x00488800`, `GetMessage=1`, `PeekMessage=5`, `DispatchMessage=1`,
  coverage **55,774 / 4,764**, pages **3,538 / 32,768**.
- 2.1B default timeline (New Campaign, NOD1, GDI1, OK): stop `0x0067dc4f` (LCW
  decoder), `GetMessage=17`, `PeekMessage=13,565,102`, `DispatchMessage=17`,
  coverage **84,130 / 6,431**, `pages-used 9,029/32,768`.
- 4B with ESC at model-ms 192,000: stop `0x0055b0bb` (budget) after 4,000,000,001
  instructions, depth 29, coverage **139,250 / 8,682**, pages **12,809 / 32,768**;
  render 8 surfaces, 12 blits, 0 fills, 769 locks, 0 flips; frame
  `/tmp/mission_4b.png` 640x480, **290,399 non-black**, md5
  `6cb0e1fff05f9250adda298e644f1266` = committed `re/out/render_2_6b.png` (the GDI
  loading screen).
- 100B with ESC: stop `0x0055b0bc` (budget), pages **16,343 / 32,768**, coverage
  unchanged; the loader is on vector 7/22 with **160,070 entries** (`TS_PEEK`
  `0x712068`: count and capacity `0x27146`, growth step 10).
`boot.py --strict` exits **0**. The only message box is the expected DirectSound
warning at `0x484456` (`type=0x30`); the primary-surface check passes
(`TS_PEEK=0x006beed8` → `1`).

**The current blocker is a wild-call fault at 479B instructions.** The page table
is dynamically growable now (2,097,152-page cap), so the old fixed 32,768-slot
exhaustion at `0x00a006b0` is gone. The loader's own O(n²) `DynamicVectorClass`
growth (22 vectors at `this+0x1280`, `sub_005a35b0`; each append through
`SetCount` `0x55b050` → `0x55b09a`) dominates the cost. A 1.2e12-budget run
reached **478,970,534,425 instructions** and faulted at `0x00422de0` with
`page table exhausted: page allocation failed` (**1,024,211 / 2,097,152** pages,
1 refused) and **7,771,444 unmapped calls** — 7 in `.text`, all `0x606060` from
`sub_004f3b40` (`0x004f3b5f`/`0x004f3b6d`); 7,673,235 below `0x1000`; 98,202
elsewhere — with wild registers across the top of the 4 GiB space and a **black
frame** (9 surfaces, 171 blits, 10,484 locks, 0 fills/flips; coverage
158,971 / 9,630). The loader got far past 100B, so this is a new defect (a
pointer table or stack going wild), not the O(n²) estimate. **The tactical phase
is not reached and no mission is rendered.** A diagnosis lane is finding the
first wild call. The VQA frame-wait is stub-bound (~68M `PeekMessageA` in 600 s),
so ESC is the practical route meanwhile; it is secondary.

**The recovery was corrected (`9e5703f` + `ad62afc`).** 213 static switch tables
are now data (was 12), including three hole tables (`0x683c10`, `0x683d98`,
`0x68ce40`) whose entry-1 addresses are now function starts. Counts: recovered
functions **43,557** (was 43,564); transcriptions **45,947** (was 46,051);
instruction lines **912,879**; `.text` code **95.86%**; jump tables **6,388 B /
213**. Every one of the 3,186 instruction addresses that left the transcription
was classified (2,581 inside classified tables, 249 re-decoded from corrected
boundaries, 356 fill/table bytes). The MIX index read is fixed as a consequence:
all **27 blocks** are read and the 17 records match `tools/mix.py` 17/17.

**The stack tracker and the null teardown calls are fixed (`d047496` +
`47d26eb`).** `stub_done` records the popped argument count and `ts_call_at`
compares a direct stub call against `esp_before + 4*nargs`: **111,682 false
stack imbalances → 0**. The five null teardown calls had one root cause:
`IDirectDrawSurface::Lock` was declared with 4 stack arguments instead of 5
(`this` + 4), leaking 4 bytes per call until EBP reached 0 and AlternateSurface
was left null. `Lock` now pops 5. Measured: **0 unmapped calls, 0 stack
imbalances**, `boot.py --strict` exits 0, link test passes.

**Signed `idiv` is fixed (`16e21fa`).** `divmod64` now has true signed x86
semantics (magnitudes + signs, correct overflow/divide-by-zero, `INT64_MIN / -1`
handled), so the old divide-overflow stop is passed.

**Narrow-width flags are fixed (`f2e6ca9`).** `Machine.h` now has width-aware
`f_add_w`/`f_sub_w`/`f_inc_w`/`f_dec_w`/`f_log_w`; all 8/16-bit
arithmetic/comparison feeds the operand width, and 32-bit is unchanged. Before
the fix, `JLE`/`JG` on a byte was wrong, so the game's own vsprintf copied
formats literally (`GetDriveTypeA("%c:\")`, `CreateFileA("EXPAND%02d.MIX")`) and
the CD-drive scan stopped after `c`. The tests now compare every flag against the
CPU's own flags via inline asm, including the `0x7f`/`0x80` boundary and the
vsprintf `%` classification; mutation-proven (18 assertions fail with the old
derivation). `19ee16c` then fixed the generator's `SetViewportOrgEx`/
`SetWindowOrgEx` arity (4, not 3); the regenerated table diff is exactly those
two rows and `--stack-check` is 342/0. `test_tools.py` runs **94** checks,
including signed-division vectors, a 200,000-pair differential sweep and the
`IDirectDrawSurface::Lock` 5-argument regression.

**Earlier fixes still in force:** COM registration (`1580be3`), message delivery
(`f3011ca`), the Blowfish cipher (`345aae7`, proven by four known-answer vectors
and the `tools/mix.py` cross-check), the 108-byte atexit leak (`ts_call_ptr` →
`ts_call_at`), `jmp_indirect` (now `ts_tail`) and conditional `rep`/`repne`
`scas`/`cmps` (they now leave the count that did not run). The shutdown ESP
corruption (WinMain returning with ESP `0x11`) is **dormant, not fixed**: the
current run never reaches shutdown.

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

**The oracle now reaches the retail game's main menu.** `tools/wine_env.sh`
runs the retail binary under Wine with the two things it needs: an X screen at
**640x480x16** (the mode it asks DirectDraw for) and a `DRIVE_CDROM` `d:` with
volume label `TS1` holding `TS1.DSK`. Measured this session: it dismisses the
DirectSound warning, plays `WWLOGO.VQA` and `STARTUP.VQA`, and reaches the
GDI/Nod title screen — 640x480, screenshot md5
`58cb9c0bb64a1bf11c6e9b28e96eca11`, **still running after 60 s** (exit 124 from
the timeout, i.e. a loop, not a crash). The earlier claim that the oracle "gives
up early" is **false** and retracted. Remaining limits: the main-menu
buttons/text are not drawn because the PCX assets live inside a nested MIX
entry, and there is no audio. `tools/oracle.py` reads the retail registers with
gdb and `--compare` diffs them against the model. The `+ddraw` trace also
exposed our 23-slot `IDirectDraw` vtable against the real 25 slots (the game
calls slot 23, `GetAvailableVidMem`).

**File I/O is real.** The stub layer used to make every file "exist" and read as
empty, with `GetFileSize` and `SetFilePointer` agreeing -- consistent and useless,
because the game could never load `rules.ini`, `TIBSUN.MIX` or the CD. It is now a
real filesystem behind a host root (`TS_ROOT`, default `../gamedir`) with
case-insensitive per-component path resolution, a 64-entry handle table and real
`CreateFileA`/`ReadFile`/`WriteFile`/`SetFilePointer`/`GetFileSize`/`CloseHandle`/
`GetFileAttributesA`/`DeleteFileA`. `IDirectDrawPalette` is a real object too.

**This paragraph previously claimed a message loop. That was false.** The model
let `ExitProcess` return, so execution continued past a fatal exit and the
"loop" was one the real game never ran. `ExitProcess` now halts the run in both
`boot_driver` and `ts_link_test.cpp`; the link test's stub had the same defect
(it set `g_halt` but not `m.halt`) and was fixed in `db612d1`. **The loop is now
genuinely reached and dispatching, and the claim is backed by a run that never
calls `ExitProcess`:** `CreateWindowExA=1`, `GetMessage=1`, `PeekMessage=5`,
`DispatchMessage=1`; the run now ends on the 20,000,001-instruction budget at
`0x00488800` (the 20M stop; at 2.1B the model renders and selects the menu,
starts the GDI campaign, and plays the GDI intro). The old retracted
`PeekMessage=137,295` is
kept as history, not as evidence. The corrupted-argument defect that the first
halt exposed is also fixed: the atexit walk leaked **108 bytes (27 dwords)**, so
ESI held a return address; `ts_call_ptr` now delegates to `ts_call_at`, and the
exit arguments were `ExitProcess(1)` and `MessageBoxA` type `0x30`, the values
the code pushed.

`Image.h` and `Win32Stubs.h` are what make this possible; neither does anything
alone. The single change that unblocked startup was giving `comctl32!DllGetVersion`
an export: the CRT startup checks the comctl32 version and calls `ExitProcess(1)`
if it is below 4.00.70, and it reaches that function through `GetProcAddress`
rather than the import table.

Measured state:

| Measure | Value (command) |
|---|---|
| headers | 266 (265 standalone-compilable + prelude) (`harness.py metrics`; `verify.py` check 1) |
| functions recovered | 43,557 spans (`harness.py metrics`: `functions`) |
| `.text` | 2,712,101 bytes; 2,599,700 covered as code = **95.86%**; **99.99% attributed as code or data**. Residual: 213 static switch tables (6,388 B), 102 switch index tables (145 B), 105,492 B of alignment padding, 376 B in 46 unclassified runs (`out/text_data.json`) |
| classes / vtable slots | 749 / 9,810 (`vtable_slots_unseeded` 0) |
| constructors that are real function starts | 100.0% (`ctors_are_function_starts`) |
| virtual declarations in headers | 6,957 = 4,499 named (64.7%) + 2,458 `vfunc_N` |
| named `(class, slot)` pairs | 5,557 of 9,810 (56.6%); 917 distinct names (281 semantic); 702 classes carry a naming record (`out/virtual_names.json`) |
| named members | 291 across 20 classes |
| member offsets recovered | 4,289 (763 are vptr stores; 3,526 real members) |
| member claims confirmed in the binary | 4,285 of 4,289 (99.9%, 4 refuted) (`verify_members_binary.py`) |
| signatures recovered | 43,557 (`harness.py metrics`) |
| **machine functions transcribed** | **45,947 (43,557 recovered starts + 2,390 tail entries)** |
| **instruction lines transcribed** | **912,879; 0 unmodelled** (`grep ts::unmodelled` → 0) |
| static instructions in `out/functions.json` | 845,093 across 43,557 spans |
| forwarder definitions | 6,962 across 310 class translation units; 3,024 distinct machine functions, 3,938 aliases (`src/generated/README.md`) |
| destructor stubs (declared, no recovered address) | 229 |
| **boot (20M, TS_CD)** | **20,000,001 instructions** (budget exhausted) at `0x00488800` (`sub_0048875c +0xa4`), depth 19; `CreateWindowExA=1`, `GetMessage=1`, `PeekMessage=5`, `DispatchMessage=1`; CD read (`D:\TS1.dsk`, `EXPAND99..00`); only `MessageBoxA` is the DirectSound warning at `0x484456` (`type=0x30`); **0 unmapped calls, 0 stack imbalances**; coverage **55,774 distinct instructions / 4,764 of 45,947 functions**; primary surface `TS_PEEK=0x006beed8` → 1 |
| **boot (2.1B, default timeline)** | **2,100,000,001 instructions** at `0x0067dc4f` (LCW video decoder), depth 28; `GetMessage=17`, `PeekMessage=13,565,102`, `DispatchMessage=17`; coverage **84,130 / 6,431**; `pages-used 9,029/32,768`; GDI intro frame 194,131 non-black |
| **boot (4B, ESC skip)** | **4,000,000,001 instructions** (budget) at `0x0055b0bb`, depth 29; coverage **139,250 / 8,682**; pages **12,809 / 32,768**; 8 surfaces, 12 blits, 769 locks, 0 flips; frame `/tmp/mission_4b.png` **290,399 non-black**, md5 `6cb0e1ff…` = `re/out/render_2_6b.png` (loading screen); **11.7 s wall** |
| **boot (100B, ESC skip)** | **100,000,000,001 instructions** (budget) at `0x0055b0bc`; pages **16,343 / 32,768**; loader on vector 7/22 with **160,070 entries** (`TS_PEEK` `0x712068`); **243.9 s wall** |
| **boot (1.2e12 budget, ESC skip)** | **fault at `0x00422de0` after 478,970,534,425 instructions**; `why: page table exhausted: page allocation failed`; pages **1,024,211 / 2,097,152** (1 refused); **7,771,444 unmapped calls** (7 in `.text`, all `0x606060` from `sub_004f3b40`; 7,673,235 below `0x1000`; 98,202 elsewhere); coverage 158,971 / 9,630; black frame (9 surfaces, 171 blits, 10,484 locks, 0 fills/flips); **tactical phase not reached** |
| **interpreter speed** | **~400M instructions/s** (12-14x the old ~28.6M/s), byte-identical state; 4B 11.7 s, 20B 50.2 s, 100B 243.9 s |
| **frame** | `re/out/frame.png`: 640x480, md5 `0fdf2636c2643ce2510ae67fb9ca9e26`, **254,610 / 307,200 non-black**; vs retail `/tmp/retail.png`: 102,836 pixels differ (33.48%), mean abs channel diff **1.06/255** over differing pixels (0.35 over frame), 506 pixels differ by >8 (retail version text) |
| boot strict gate | exits **0**: 0 garbage call targets (`boot.py --no-build --strict`) |
| link test | **OK**, exit 0, 492 TUs, 20,000,001 instructions, **message loop reached** (`GetMessage=1`, `PeekMessage=5`, `DispatchMessage=1`), 0 unmapped, 0 unmodelled (`link_test.py`) |
| retail oracle | reaches the **main menu**, 640x480, md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s (`wine_env.sh`) |

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
| Functions disassembled | 43,557 spans | done |
| Instructions recovered | 845,093 static (912,879 transcribed lines) | done |
| INI keys / referenced strings | 1,876 | done |
| Per-class vtable+ctor manifest | 749 classes, 9,810 slots | done |
| Direct-base reduction | 749 classes | done |
| Member offsets recovered | 4,289 | done |
| Machine code transcribed | 45,947 functions | done |
| Class forwarders emitted | 6,962 | done |
| Whole-tree link + entry-point run | 492 TUs | done |
| Binary-anchored member verification | 4,285/4,289 (99.9%), gated | done |
| Tool unit tests | 94 checks, mutation-proven | done |
| Runtime tests | 101 checks (rendering/input), mutation-proven | done |

### Verification performed

- `tools/verify.py` runs **nine** checks (about 6 s last timed) and is
  **fail-closed**: a missing artifact exits non-zero naming what is missing,
  rather than running a subset and printing PASSED.
- `tools/boot.py` (with `TS_CD`) runs the image for **20,000,001 instructions**
  (the budget, at `0x00488800`), reports the stop, import calls, coverage and
  unmapped-target classification, and **reaches the message loop with dispatch**
  (`CreateWindowExA=1`, `GetMessage=1`, `PeekMessage=5`, `DispatchMessage=1`).
  `--strict` exits **0** with 0 garbage targets and 0 stack imbalances. At
  `--budget 2100000000` the default timeline selects the GDI campaign, reads
  `GDI1A.MAP`, and plays the GDI intro: stop `0x0067dc4f` (LCW decoder),
  coverage 84,130 / 6,431, `pages-used 9,029/32,768`, frame 194,131 non-black.
  With ESC at model-ms 192,000 the mission load runs at **~400M instructions/s**
  (12-14x faster, byte-identical state); at 4B it stops at `0x0055b0bb` with
  12,809 pages and a 290,399-non-black loading screen (`re/out/render_2_6b.png`),
  and at 100B it is still on loader vector 7/22 (160,070 entries). A 1.2e12
  budget run faulted at 479B instructions at `0x00422de0` with 7,771,444 unmapped
  calls and a black frame; the tactical phase is not reached.
- `tools/link_test.py` links **492** translation units, runs the entry point,
  and **passes** (exit 0); with `TS_ROOT`/`TS_CD`/`TS_DATA` the message loop is
  reached and dispatching, with 0 unmapped calls.
- `tools/wine_env.sh` runs the **retail** binary to its main menu (640x480,
  screenshot md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s).
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
- `tools/test_tools.py` runs **94** unit checks, each a regression test for a
  shipped defect or a differential test against an independent implementation.
  Several are mutation-proven to fail when the defect is reintroduced,
  including the interpreter hot-path checksum and page-cache ratio.
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
- [x] Recover function signatures (43,557)
- [x] Name virtuals where evidence exists (4,499 named declarations; 5,557 named `(class, slot)` pairs; 917 distinct names)
- [x] Anchor member verification to the binary (4,285/4,289, 99.9% confirmed)
- [x] Transcribe every recovered function (45,947 functions, 912,879 instruction lines, 0 unmodelled)
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
- [x] Classify the residual `.text`. It is **213 static switch tables (6,388 B)**
      and **102 switch index tables (145 B)**, emitted as data in
      `out/text_data.json`, plus **105,492 B** of alignment padding. **376 B in
      46 runs** are neither and are listed verbatim, not forced into a category.
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

### P4 — booting the game  [IN PROGRESS]
- [x] Fix the 108-byte atexit stack leak. `ts_call_ptr` now delegates to
      `ts_call_at`; measured `ExitProcess(1)` and `MessageBoxA` type `0x30`.
- [x] Fix `jmp_indirect` (was a no-op; now `ts_tail`) and conditional
      `rep`/`repne` `scas`/`cmps` (they zeroed ECX instead of leaving the count
      that did not run).
- [x] Fix `LoadStringA`'s arity (5 -> 4) and make `gen_win32_stubs.py` refuse to
      write when a `TS_RET` body and `NARGS` disagree.
- [x] Fix the link test's `ExitProcess` stub (it returned; now it faults).
- [x] Fix `boot.py`'s fresh-checkout header sync (`60dac16`).
- [x] Remove the unmapped calls. `1580be3` fixed
      `IDirectDraw::CreateClipper` and `DirectSoundCreate` (`DSERR_NODRIVER`);
      a later teardown regression (70/10) was fixed by `47d26eb`
      (`IDirectDrawSurface::Lock` arity). The boot now reports **0 unmapped
      calls** and `boot.py --strict` exits **0**.
- [x] **Reach a window and a message loop.** Reached (`1580be3`).
- [x] **Deliver queued messages (`f3011ca`).** `CreateWindowExA` queues
      `WM_ACTIVATEAPP`; the WndProc at `0x65ba70` runs, `DAT_007b2e50` becomes 1,
      and the game leaves the spin. Measured at that commit: 1,882,051
      instructions, `GetMessage=1`, `PeekMessage=3`, `DispatchMessage=1`,
      coverage 49,635 / 4,455.
- [x] **Implement the Blowfish cipher (`345aae7`).** Standard Blowfish-ECB,
      proven by four known-answer vectors and a byte-identical `tools/mix.py`
      cross-check on the shipped TIBSUN.MIX index (`2ce3171bdbecb40e` →
      `110090d97904849a`, count 17, body 75,094,416). The page-table exhaustion
      at `0x00a006b0` is gone.
- [x] **The MIX index read (`1c0db7e`).** The root cause was the unaligned
      `memcpy` jump table, not the pipe or cipher. All **27 blocks** are read and
      the 17 records match `tools/mix.py` 17/17.
- [x] **The disasm switch-table correction (`9e5703f` + `ad62afc`).** 213 tables
      (6,388 B) are data; three hole tables' entry-1 addresses are function
      starts. Counts: 43,557 functions, 45,947 transcriptions, 912,879
      instruction lines, 95.86% code.
- [x] **The stack tracker (`d047496`)** and **the 5 null teardown calls
      (`47d26eb`).** 111,682 false imbalances -> 0; `Lock` now pops 5; 0 unmapped
      calls.
- [x] **Signed `idiv` (`16e21fa`).** `divmod64` has true signed x86 semantics.
- [x] **The dialog layer (`f576607` + `c81c1f5`).** LANGUAGE.DLL's 60 dialog
      templates and string table are read from the module's own `.rsrc`;
      `CreateDialogIndirectParamA` builds the record and controls, sends
      `WM_INITDIALOG`, and `DispatchMessage` runs. A CD drive model (`TS_CD`)
      was added.
- [x] **Narrow-width flags (`f2e6ca9`).** Width-aware
      `f_add_w`/`f_sub_w`/`f_inc_w`/`f_dec_w`/`f_log_w`; the vsprintf `%` and
      the CD-drive scan are correct; flags are compared against the CPU's own.
- [x] **Generator arity (`19ee16c`).** `SetViewportOrgEx`/`SetWindowOrgEx` take
      4; `--stack-check` is 342/0.
- [x] **Rendering and input integration (`5031e44` etc.).** Surfaces route
      through `ts::rnd`, input through `ts::input`, `SCREEN_W/H` are 640x480,
      and `--frame` dumps the primary surface. The model renders the real main
      menu: `re/out/frame.png` (md5 `0fdf2636…`, 254,610 / 307,200 non-black;
      1.06/255 mean difference from retail on the pixels that differ).
- [x] **Menu-item selection (`8474345` … `fa3e2a0`).** `DLGTEMPLATEEX` is parsed;
      the menu's six `BS_OWNERDRAW` buttons have real rectangles and the click
      goes through the game's own hit-test and `WM_COMMAND` path. New Campaign
      opens `RT_DIALOG` 148 "Select Campaign:" at step 1,688,503,557; Exit Game
      reaches `ExitProcess(0)` at `0x00684529` after 1,707,666,895 instructions.
      The nested MIX (`0xa8548fd9`, 317 entries) is mounted and read.
- [x] **Campaign list and mission start (`1966cc1` + `4b7ef9b`).** The list
      source is shipped `BATTLE.INI` (`[Battles] 1=GDI1, 2=NOD1`); the model
      implements the full listbox/trackbar layer, `FindFirstFileA` host
      enumeration and a read-only `TS_DATA` root. OK selects GDI, `GDI1A.MAP` is
      read from MAPS01.MIX, and the GDI intro movie plays (2.1B stop
      `0x0067dc4f`, `pages-used 9,029/32,768`, frame 194,131 non-black).
- [x] **The page table grows and the heap reuses freed blocks.** `PageTable`
      grows past 32,768 (2,097,152-page cap) with stable page identity; the heap
      coalesces and reallocs in place. At 4B the mission load uses **12,809**
      pages instead of 377,796 (50 MB instead of 1.44 GiB), 11,696 of 12,253
      allocations reused (95.5%).
- [x] **The interpreter is 12-14x faster (`9243e93` … `6421f30`).** ~400M
      instructions/s with byte-identical machine state; 4B in 11.7 s, 20B in
      50.2 s, 100B in 243.9 s. `test_machine_perf` pins the hot path.
- [ ] **The 479B mission-load run goes wild — the current blocker.** A 1.2e12
      budget run reached 478,970,534,425 instructions and faulted at
      `0x00422de0` with the page table refusing a page (1,024,211 / 2,097,152)
      and **7,771,444 unmapped calls** (7 in `.text`, all `0x606060` from
      `sub_004f3b40`; 7,673,235 below `0x1000`; 98,202 elsewhere), wild
      registers and a black frame. The loader got far past 100B; this is a new
      defect (a pointer table/stack going wild). The tactical phase is **not
      reached** and no mission is rendered. A diagnosis lane is finding the
      first wild call.
- [ ] **The VQA player's frame-wait is stub-bound — secondary.** ~68M
      `PeekMessageA` calls in 600 s; ESC is the practical route to the mission
      load meanwhile.
- [ ] **Shutdown corrupts ESP — dormant.** WinMain returned with ESP `0x11`
      before the MIX-read fix; the current run never reaches shutdown. Keep it
      queued.

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
- **The boot has 0 unmapped calls and 0 stack imbalances**; `boot.py --strict`
  exits 0 and the link test passes.
- **The model starts the GDI campaign and plays its intro.** The menu renders
  and is selectable (`re/out/frame.png`: 640x480, md5 `0fdf2636…`, 254,610 /
  307,200 non-black; versus retail, 102,836 pixels differ (33.48%), mean
  absolute channel difference 1.06/255 over the differing pixels, 506 pixels
  >8). New Campaign opens `RT_DIALOG` 148 at step 1,688,503,557; the campaign
  list comes from shipped `BATTLE.INI` and OK selects GDI. `GDI1A.MAP` is read
  from MAPS01.MIX and the GDI intro plays (stop `0x0067dc4f`, frame 194,131
  non-black). The nested MIX (`0xa8548fd9`, 317 entries) is mounted and read.
- **The mission load does not complete: the 479B run goes wild.** ESC skips the
  movie; the loader's own O(n²) vector growth dominates (at 100B it is on vector
  7/22 with 160,070 entries). A 1.2e12-budget run reached 478,970,534,425
  instructions and faulted at `0x00422de0` with 1,024,211 / 2,097,152 pages,
  **7,771,444 unmapped calls** (7 in `.text`, all `0x606060` from
  `sub_004f3b40`; 7,673,235 below `0x1000`; 98,202 elsewhere) and a black
  frame. The page table is growable; this is a new pointer-table/stack defect.
  The tactical phase is not reached and no mission is rendered.
- **The VQA frame-wait is stub-bound.** ~68M `PeekMessageA` calls in 600 s, so
  ESC is the practical route to the mission load meanwhile.
- **Shutdown corrupts ESP, but the path is dormant.** WinMain returned with ESP
  `0x11` before the MIX-read fix; the current run never reaches shutdown.
- **The Blowfish cipher, the MIX index read and the dialog layer are fixed.**
  All 27 index blocks are read (17/17 records match `mix.py`), and the game
  builds its real LANGUAGE.DLL dialogs. The CD path is correct with `TS_CD`. The
  retail oracle reaches the main menu, but the menu's buttons/text are not drawn
  (PCX assets inside a nested MIX entry) and audio is absent.
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
[1] standalone compile: 265/265
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
  link: OK (492 translation units); ts_link_test: OK (exit 0)
  5 of 5 sampled VAs resolved
  imports: 341 across 13 DLLs; 341 of 341 IAT slots written; 341 bound, 0 with no stub
  entry 0x006854e3 ran; 20,000,001 instructions (budget)
  0 unmapped calls; 0 unmodelled
  MESSAGE LOOP: reached -- GetMessage 1, PeekMessage 5, DispatchMessage 1

tools/boot.py  (TS_CD=../work/cd1)
  20,000,001 instructions (budget exhausted at 0x00488800)
  CreateWindowExA=1 GetMessage=1 PeekMessage=5 DispatchMessage=1
  CreateFileA("D:\TS1.dsk") ok; EXPAND99..EXPAND00 enumerated
  only MessageBoxA: DirectSound warning at 0x484456 (type 0x30)
  message loop: reached and dispatching; unmapped calls: 0; stack imbalances: 0
  coverage: 55,774 distinct instructions; 4,764 of 45,947 recovered functions entered
  primary surface: TS_PEEK 0x006beed8 = 1
  --no-build --strict: exit 0
  --budget 2100000000, default timeline (New Campaign, NOD1, GDI1, OK):
    stop 0x0067dc4f (LCW decoder); GetMessage=17 PeekMessage=13,565,102
    DispatchMessage=17; 84,130 / 6,431; pages-used 9,029/32,768
    frame: re/out/mission.png, 640x480, 194,131 non-black
  --budget 4000000000, same timeline + ESC at model-ms 192000:
    stop 0x0055b0bb (budget) after 4,000,000,001 instructions; 11.7 s wall
    coverage 139,250 / 8,682; pages 12,809/32,768; 12 blits, 769 locks
    frame: /tmp/mission_4b.png, 290,399 non-black, md5 6cb0e1ff...
          = re/out/render_2_6b.png (GDI loading screen)
  --budget 100000000000, ESC skip: stop 0x0055b0bc; pages 16,343/32,768;
    loader on vector 7/22 with 160,070 entries (TS_PEEK 0x712068); 243.9 s
  --budget 1200000000000, ESC skip (1.2e12): fault at 0x00422de0 after
    478,970,534,425 instructions; pages 1,024,211/2,097,152 (1 refused);
    7,771,444 unmapped calls; black frame; tactical phase NOT reached

tools/test_runtime.py
  RUNTIME TEST PASSED (101 checks, mutation-proven); frame dump produced

re/out/frame.png
  640x480, md5 0fdf2636c2643ce2510ae67fb9ca9e26, 254,610 / 307,200 non-black
  vs /tmp/retail.png (58cb9c0b...): 102,836 pixels differ (33.48%),
  mean abs channel diff 1.06/255 over differing pixels, 506 pixels >8

tools/wine_env.sh
  retail game: main menu, 640x480, md5 58cb9c0bb64a1bf11c6e9b28e96eca11
  still running after 60 s (exit 124 = a loop, not a crash)
```
