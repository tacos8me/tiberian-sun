# PROGRESS — Tiberian Sun reconstruction

All figures below were re-measured on 2026-10-18, after merging the TLS/CLSID
fixes that populate the scenario world and the multimedia-timer fix that centres
the tactical view, with `python3 tools/harness.py metrics
--no-append`, `python3 tools/verify.py src/include build/shim`,
`python3 tools/verify_members_binary.py`, `python3 tools/test_tools.py`,
`python3 tools/test_runtime.py`, `python3 tools/boot.py` with `TS_CD` (20M, 2.1B
with the default campaign timeline, the ESC-skip script at 4B/20B/100B/500B, and
`--strict`), `python3 tools/link_test.py`, `tools/wine_env.sh`, and direct counts
over `out/` and `src/generated/`. `TS_ROOT` selects the ordinary file root;
`TS_CD` models the `D:` CD drive (`DRIVE_CDROM`, label `TS1`); `TS_DATA`
(default `realdata`) serves shipped control files such as `BATTLE.INI`. From a
worktree pass absolute `TS_ROOT`/`TS_CD`; without `TS_ROOT` the game cannot find
its ordinary files and takes its own failure path (`ExitProcess` at ~7.3M
instructions), which is environment, not a model regression. Re-run those rather
than trusting this file.

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
PeekMessage 5, DispatchMessage 1`, 0 unmapped. `test_tools.py` runs **106**
checks (including the mutation-proven `win32-tls`, `win32-clsid` and
`win32-timer`) and
`test_runtime.py` **110** (mutation-proven). A full
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

**The scenario now creates its world (`c61f060` + `8163162`).** At mission init
the scenario INI holds `[Houses] 7`, `[Units] 3`, `[Infantry] 17`,
`[Structures] 36`. The first unit resolves owner `"Nod"` → house 1, type
`"BGGY"` → `0x18`, and placement slot `0xc4` → 1; all 3 unit placements and all
17 infantry placements return 1. The authoritative world registries after the
run hold **4 ObjectClass (`0x77bbe0`), 21 infantry (`0x7b0c08`), 36 buildings
(`0x7b2c48`) and 7 houses (`0x7afce0`)**. `TS_OBJECTS` also prints `n=0` because
it reads `0x7b2d98`, a per-frame renderer list that is legitimately empty at an
arbitrary stop — that number is **not** the world. Two constant-return stubs had
prevented creation: `TlsGetValue`/`TlsSetValue` never persisted the CRT's
`_getptd` block, so `FUN_006837ea` (strtok) returned only the first token and
every scenario entry lost its type; and `CLSIDFromString` wrote 16 zero bytes, so
every `Locomotor={...}` became `CLSID_NULL` and `CoCreateInstance` returned a
generic 3-slot `IUnknown` whose slot-3 call at `0x630e2d` was 0, leaking `0x54`
bytes of arguments over `FUN_0063b470`'s saved EBP (`this` became `0x9a80`).
`CoRegisterClassObject` was also a no-op; `CoCreateInstance` now delegates to the
game's own `IClassFactory`. The pre-timer 500B ESC-skip frame
`re/out/frame_500b_objects.png` (640x480, md5
`e788f40b2db184885b9f25dab79380b0`, **93,404 non-black**) showed the tactical HUD
plus the revealed base area in the viewport's bottom-right corner; it is
superseded by `re/out/frame_500b_centered.png`.

**The tactical view is now centred — the multimedia timer was the cause
(`f1b4bc6`).** `timeSetEvent` was a constant return (`TS_RET(..., 1)`): it
returned a timer id and never invoked the callback. `WWMouseClass` installs a
16 ms periodic callback (`0x6725d0`) that reads `GetCursorPos` and stores the
result at mouse `+0x18`/`+0x1c`; with no callback the modelled pointer stayed
`(0,0)`. The tactical input handler `FUN_005cf850` read `(0,0)` as "the pointer
is outside the screen" and issued an up-left scroll command every frame
(direction 7, accelerating speed); the scroll ran to the map clamp `(-820,988)`,
the visible rect clamped to the map minimum `(35,28,23,43)`, and the revealed
area rendered in the viewport's bottom-right corner. A timer table plus
`timer_pump()` now dispatches due callbacks from `PeekMessageA`/`GetMessageA`
with the real five-argument `TIMECALLBACK` signature, periodic/one-shot handling
and `timeKillEvent`. `win32-timer` is mutation-proven (removing the dispatch
fails it).

Measured at 20B ESC-skip: the stop is the budget at `0x00695dad` after
**20,000,000,001** instructions, depth 100; `GetMessage=19`,
`PeekMessage=13,760,224`, `DispatchMessage=19`; pages **13,698 / 32,768**;
coverage **231,260 / 13,433**; 8 surfaces, **25,913 blits / 156,314 locks**;
**0 unmapped**. WWMouse position **(355,397)**, display scroll **(-408,1248)**
(the game's centring target, not the clamp), visible rect **(53,45,23,43)**,
map-surface non-black **96,220** (was 12,703), primary **176,809** non-black.
The 500B ESC-skip run has **0 unmapped**, pages **13,698**, coverage
**231,365 / 13,437**, and `re/out/frame_500b_centered.png` (640x480, md5
`e8ba8219…`, **176,796 non-black**) shows terrain, structures, units and a
Tiberium field with the HUD and the mouse cursor. The remaining black at the top
and right is unexplored shroud, correct at mission start.

**Correction: "34 cells seen" was a `TS_MAPCELLS` windowing defect.** It scanned
only the INI's 48x63 `[Map] Size` window. The whole 512x512 array shows
**5,985 allocated, 193 tiles, 1,124 seen**, seen bbox (34,57)-(83,102). The
camera target is the shipped `[Basic] HomeCell=98` / `[Waypoints] 98=76068` →
cell (68,76), decoded by `0x5c802a` and applied by `0x47661d`; that is scenario
data, not a defect, and the GDI construction yard at (28,59) is not the camera
target. The reveal pipeline runs (`0x4732b0` executed 1,634 times from the
per-object pass). `FogOfWar=no` and `ShroudGrow=no` in the shipped `rules.ini`,
so the fog/shroud-regrow passes correctly do not run. The render offset was
**not** the transform: the renderer draws at `world - (scroll - half_viewport)`
and maps the camera cell exactly to the viewport centre.

**The next work item is scripted tactical input.** Select a unit and issue a
move order, then verify its `CoordStruct` (`ObjectClass +0x40`, 256 leptons/cell)
changes; also check whether the sidebar build buttons respond. Do not claim
playability.

**The interpreter is 12-14x faster, with byte-identical machine state.**
Measured on the mission-load run (ESC-skip): 4B in **11.7 s** (341.9M
instructions/s), 20B in **50.2 s** (398.4M/s), 100B in **243.9 s** (410.0M/s);
the old rate was ~28.6M/s. The changes are semantics-preserving: `-O2
-fno-pie`/`-no-pie`, a 256-entry positive-hit page cache, wide single-page
`ld16/st16/ld32/st32`, a `noinline insn_slow()` cold path, inlined page and flag
helpers, and a cached import-name lookup. `test_machine_perf` pins the hot path
and is mutation-proven.

Boot states, all with **0 unmapped calls and 0 stack imbalances**. The
20M/2.1B/4B/100B rows predate the multimedia-timer fix; the 20B/500B rows are
post-fix:
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
- 20B with ESC (after the timer fix): stop `0x00695dad` (budget) after
  **20,000,000,001** instructions, depth 100; `GetMessage=19`,
  `PeekMessage=13,760,224`, `DispatchMessage=19`; pages **13,698 / 32,768**;
  coverage **231,260 / 13,433**; 8 surfaces, **25,913 blits / 156,314 locks**;
  **0 unmapped**; WWMouse `(355,397)`, scroll `(-408,1248)`, visible rect
  `(53,45,23,43)`, map surface 96,220 non-black, primary 176,809.
- 500B with ESC (after the timer fix): **0 unmapped**, pages **13,698**,
  coverage **231,365 / 13,437**, message loop dispatching; frame
  `re/out/frame_500b_centered.png`, md5 `e8ba8219…`, **176,796 non-black** —
  terrain, structures, units and a Tiberium field, with the HUD and cursor.
- Historical (pre-timer): 20B stopped at `0x0041398c` with coverage
  230,640 / 13,415, 57,074 blits / 254,041 locks; 500B stopped at `0x0042322c`
  with coverage 230,658 / 13,417, 1,942,509 blits / 5,971,443 locks and
  `re/out/frame_500b_objects.png` (93,404 non-black, the revealed area in the
  viewport's bottom-right corner).
`boot.py --strict` exits **0**. The only message box is the expected DirectSound
warning at `0x484456` (`type=0x30`); the primary-surface check passes
(`TS_PEEK=0x006beed8` → `1`).

**Historical: the tactical HUD renders, and the 479B out-of-bounds write is
fixed.** The
root cause was the **x87 emitter**: `tools/emit_x86.py` rendered the D8
one-register forms as pop forms, so `fdiv st(1)` wrote `ST(1)=ST(1)/ST(0)`
instead of `ST(0)=ST(0)/ST(1)` (901 non-pop one-operand x87 instructions
affected; the 692 pop forms were right). `FUN_005a1bd0`'s tactical zoom used it
and stored scale **108** instead of **1.125** at `[0x710d28+0x1490]`, so
`FUN_005a4080` built the bogus `2125x4176` visible rect and `FUN_005a34e0`
wrote 34,062 bytes before its bitmap, on a TerrainClass vptr. Fixed in
`e5d7039` (emitter + 951 regenerated x87 lines + tests + a clean-tree
`gen_headers` unblock). After the fix: scale **1.125** (`0x3f900000`), map
surface **99x108**, visible rect **35, 28, 23, 43**, vptr **`0x006a5dfc`**, **0**
out-of-bounds stores.

**Historical: the first in-game frame (pre-timer).** Budget at
**500,000,000,001** instructions, EIP `0x00543669` (`sub_00543620`, a 3x4
matrix·vector transform in the renderer), depth 100; pages **12,637 / 32,768**
(refused 0); **0 unmapped calls**; coverage **158,667 / 9,607**; message loop
dispatching (`GetMessage=19`, `DispatchMessage=19`, `PeekMessage=23,117,313`);
8 surfaces with **18,691,958 blits and 46,753,350 locks**.
`re/out/frame_500b.png` (640x480, md5 `4fc1cec7…`, **87,794 non-black**) showed
Options, credits 5000, the GDI radar eagle, and the build tabs/grid. An
independent 500B-budget reproduction was stopped by the 1800 s watchdog at
372,912,291,926 instructions (host contention) but matched pages (12,637),
coverage (158,667 / 9,607), 0 unmapped and a **byte-identical frame**. This is
superseded by the timer fix.

**The black is unexplored shroud, not a renderer defect.** The per-surface trace
shows the 472x480 map surface blitted to the primary once per frame
(`last_src=0x5060c4e4`; the two map surfaces swap on scroll), the sidebar
composited, and the radar 99x108 surface **0 non-black**. `TS_EXEC_CHECK` shows
every tactical pass executing (`0x4b2ff0` map render, `0x5f6e50`,
`0x5f6360`/`0x5f8c00` terrain, `0x452ab0` tile draw, `0x478f00` shape blit,
`0x5f90e0`/`0x452330`/`0x5fa220` shroud, `0x5a0b90`/`0x5a4080` radar). The whole
512x512 cell array shows **5,985 allocated, 193 tiles, 1,124 seen** (bbox
(34,57)-(83,102)), with heights decoded, so `IsoMapPack5` works. The reveal
pipeline runs (`0x4732b0` executed 1,634 times from the per-object pass);
`FogOfWar=no` and `ShroudGrow=no` in the shipped `rules.ini`, so those passes
correctly do not run. `FUN_00543620` executes with sane callers; no frame was
hand-painted.

**The next work item is scripted tactical input, not the render offset or the
reveal radius.** Select a unit and issue a move order, then verify its
`CoordStruct` (`ObjectClass +0x40`, 256 leptons/cell) changes; also check whether
the sidebar build buttons respond. Do not claim playability. The VQA frame-wait
is stub-bound (~68M `PeekMessageA` in 600 s), so ESC is the practical route
meanwhile; it is secondary.

**Superseded diagnosis, kept as history.** Before `c61f060` + `8163162` the
world was genuinely empty: `TS_OBJECTS` read **n=0, array=NULL**, `TS_MAPCELLS`
showed 1,681 cells with 12 tile entries and **0 revealed**, and every
`[Units]`/`[Structures]` entry was rejected (owner-house `FUN_004c7470` → -1,
type `FUN_0063e650` → -1, `FUN_00630cc0` null, placement `0xc4` → 0). The cause
was two constant-return stubs (TLS and CLSID/class-factory), described above.
Note that `n=0` was never evidence of the empty world: it reads the per-frame
renderer list `0x7b2d98`, not the class registries. The earlier 479B flood
(7,771,444 unmapped calls, 1,024,211 / 2,097,152 pages, black frame) is also
historical and no longer occurs.

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
two rows and `--stack-check` is 342/0. `test_tools.py` runs **106** checks,
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
| **boot (20B, ESC skip, after the timer fix)** | **20,000,000,001 instructions** (budget) at `0x00695dad`, depth 100; `GetMessage=19`, `PeekMessage=13,760,224`, `DispatchMessage=19`; pages **13,698 / 32,768**; coverage **231,260 / 13,433**; 8 surfaces, **25,913 blits / 156,314 locks**; **0 unmapped**; WWMouse `(355,397)`, scroll `(-408,1248)`, visible rect `(53,45,23,43)`, map surface 96,220 non-black, primary 176,809 |
| **boot (500B, ESC skip, after the timer fix)** | **500,000,000,001 instructions** (budget); **0 unmapped**; pages **13,698**; coverage **231,365 / 13,437**; message loop dispatching; frame `re/out/frame_500b_centered.png` md5 `e8ba8219…`, **176,796 non-black** — terrain, structures, units, Tiberium, HUD and cursor |
| **world state (after TLS/CLSID)** | registries: **ObjectClass 4 (`0x77bbe0`), infantry 21 (`0x7b0c08`), buildings 36 (`0x7b2c48`), houses 7 (`0x7afce0`)**; scenario INI at mission init: `[Houses] 7`, `[Units] 3`, `[Infantry] 17`, `[Structures] 36`; first unit owner `"Nod"` → house 1, type `"BGGY"` → `0x18`, placement `0xc4` → 1; `TS_MAPCELLS` over the whole 512x512 array: **5,985 allocated, 193 tiles, 1,124 seen**, seen bbox (34,57)-(83,102); camera target `HomeCell=98` → cell (68,76); `TS_OBJECTS` still prints `n=0` because `0x7b2d98` is a per-frame renderer list, not the world |
| **boot (20B/500B, pre-timer, historical)** | 20B stopped at `0x0041398c`, coverage 230,640 / 13,415, 57,074 blits / 254,041 locks; 500B stopped at `0x0042322c`, coverage 230,658 / 13,417, 1,942,509 blits / 5,971,443 locks; frame `re/out/frame_500b_objects.png` (93,404 non-black), revealed area in the viewport's bottom-right corner. Superseded by `f1b4bc6` |
| **world state (6B, ESC skip, pre-fix)** | superseded: `TS_OBJECTS` **n=0, array=NULL**; `TS_MAPCELLS` **alloc=1,681, tiles=12, seen=0, fog=0**; all `[Units]`/`[Structures]` entries rejected. Fixed by `c61f060` + `8163162` |
| **boot (1.2e12 budget, ESC skip)** | historical, pre-x87-fix: first wild call deterministic at step 478,300,935,316; flood faulted at `0x00422de0` after 478,970,534,425 instructions with 1,024,211 / 2,097,152 pages and 7,771,444 unmapped calls; **does not reproduce after `e5d7039`** |
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
| Tool unit tests | 106 checks, mutation-proven | done |
| Runtime tests | 110 checks (rendering/input), mutation-proven | done |

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
  and at 100B it is still on loader vector 7/22 (160,070 entries). After the
  timer fix, 20B stops at the budget at `0x00695dad` (coverage 231,260 / 13,433,
  0 unmapped, WWMouse `(355,397)`, scroll `(-408,1248)`, visible rect
  `(53,45,23,43)`), and 500B has **0 unmapped calls**, pages 13,698, coverage
  231,365 / 13,437, and renders `re/out/frame_500b_centered.png` (176,796
  non-black: terrain, structures, units, Tiberium, HUD, cursor). The scenario's
  3 units, 17 infantry and 36 structures are created; the whole 512x512 cell
  array shows 5,985 allocated / 193 tiles / 1,124 seen, and the remaining black
  is unexplored shroud, correct at mission start. The pre-fix 1.2e12 flood no
  longer reproduces.
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
- `tools/test_tools.py` runs **106** unit checks, each a regression test for a
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
- [x] **The 479B wild call is diagnosed (`6a31165`).** The first wild call is
      deterministic at step **478,300,935,316** (three runs agree):
      `call [edx+0x2c]` at `0x005fa96b` → `0x46000062`, EDX=`0x6a5dfe` =
      TerrainClass vtable `0x6a5dfc` + 2. `FUN_005a34e0`'s cell-seen bitmap
      setter wrote 34,062 B before its bitmap (index `67*(-4068)+65 = -272,491`)
      at `0x005a3558`, corrupting the vptr. The page refusal is the 32-bit
      process's ~4.2 GB address space, not a table cap; the flood's termination
      varies, the game state does not.
- [x] **The x87 D8 emitter form is fixed (`e5d7039`).** `tools/emit_x86.py`
      rendered the D8 one-register forms as pop forms, so `fdiv st(1)` wrote
      `ST(1)=ST(1)/ST(0)` instead of `ST(0)=ST(0)/ST(1)`; 901 non-pop
      one-operand x87 instructions were affected. `FUN_005a1bd0`'s tactical
      zoom produced scale 108 instead of 1.125, which built the bogus
      `2125x4176` visible rect and caused the out-of-bounds vptr write. After
      the fix: scale 1.125 (`0x3f900000`), map surface 99x108, visible rect
      `35,28,23,43`, vptr `0x006a5dfc`, 0 OOB stores.
- [x] **The tactical HUD renders (500B run, pre-TLS/CLSID).** Budget at
      500,000,000,001, EIP `0x00543669`, depth 100, pages 12,637/32,768,
      **0 unmapped**, coverage 158,667/9,607, 8 surfaces with 18,691,958 blits
      and 46,753,350 locks; frame `re/out/frame_500b.png` (87,794 non-black)
      shows the HUD. After the object-creation fixes the same budget renders
      `frame_500b_objects.png` (93,404 non-black) with the revealed base area.
- [x] **The black map is explained: it is shroud, not a missing render
      (`707c96d`, superseded in part).** The renderer is correct: the map surface
      is blitted to the primary every frame, every tactical pass executes, and no
      frame was hand-painted. The original diagnosis also read `TS_OBJECTS` n=0
      as an empty world; that was a per-frame renderer list (`0x7b2d98`), not the
      world. The world *was* empty at the time, for the TLS/CLSID reasons below.
      `FUN_00543620` runs with sane callers.
- [x] **Scenario object creation is fixed (`c61f060` + `8163162`).** Two stubs
      caused the empty world. `TlsGetValue`/`TlsSetValue` were constant returns,
      so the CRT's `_getptd` block never persisted and `FUN_006837ea` (strtok)
      returned only the first token; every scenario entry lost its type.
      `CLSIDFromString` wrote 16 zero bytes, so every `Locomotor={...}` became
      `CLSID_NULL`; `CoCreateInstance` returned a generic 3-slot `IUnknown` whose
      slot-3 call at `0x630e2d` was 0, leaking arguments and walking ESP `0x54`
      bytes over `FUN_0063b470`'s saved EBP (`this` became `0x9a80`).
      `CoRegisterClassObject` was a no-op; `CoCreateInstance` now delegates to
      the game's own `IClassFactory`. Measured: `[Units] 3`, `[Infantry] 17`,
      `[Structures] 36`, all 3 unit and all 17 infantry placements return 1;
      registries 4 ObjectClass, 21 infantry, 36 buildings, 7 houses.
- [x] **The tactical view is centred (`f1b4bc6`).** `timeSetEvent` was a
      constant return and never dispatched its callback, so WWMouse's position
      stayed `(0,0)`; the tactical input handler read that as off-screen and
      scrolled up-left every frame to the map clamp. A timer table plus
      `timer_pump()` now dispatches due callbacks from
      `PeekMessageA`/`GetMessageA` (real five-argument `TIMECALLBACK` signature,
      periodic/one-shot, `timeKillEvent`). Measured at 20B: WWMouse `(355,397)`,
      scroll `(-408,1248)`, visible rect `(53,45,23,43)`, map surface 96,220
      non-black (was 12,703), primary 176,809. The 500B frame
      `re/out/frame_500b_centered.png` (176,796 non-black) shows terrain,
      structures, units and Tiberium with the HUD and cursor. `win32-timer` is
      mutation-proven; `test_tools.py` is 106 checks.
- [x] **The revealed-cell count is corrected.** The old "34 seen" figure was a
      `TS_MAPCELLS` windowing defect that scanned only the INI's 48x63 `[Map]
      Size` window. The whole 512x512 array shows **5,985 allocated, 193 tiles,
      1,124 seen** (bbox (34,57)-(83,102)). The camera target is the shipped
      `[Basic] HomeCell=98` / `[Waypoints] 98=76068` → cell (68,76), decoded by
      `0x5c802a` and applied by `0x47661d`; scenario data, not a defect.
- [ ] **Scripted tactical input — the next work item.** Select a unit and issue
      a move order, then verify its `CoordStruct` (`ObjectClass +0x40`, 256
      leptons/cell) changes; also check whether the sidebar build buttons
      respond. Do not claim playability.
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
- **The tactical map renders centred, the world is populated, and the remaining
  black is unexplored shroud.** The 479B out-of-bounds write was the x87 D8
  emitter form (901 instructions), fixed in `e5d7039`; after the fix the
  tactical zoom is 1.125, the map surface 99x108, the vptr intact and there are
  0 OOB stores. The TLS/CLSID fixes (`c61f060` + `8163162`) create the
  scenario's 3 units, 17 infantry and 36 structures; the registries hold 4
  ObjectClass, 21 infantry, 36 buildings and 7 houses. The multimedia-timer fix
  (`f1b4bc6`) centres the view: the 20B run has WWMouse `(355,397)`, scroll
  `(-408,1248)`, visible rect `(53,45,23,43)`, map surface 96,220 non-black
  (was 12,703) and primary 176,809; the 500B run renders
  `re/out/frame_500b_centered.png` (176,796 non-black: terrain, structures,
  units, Tiberium, HUD and cursor) with 0 unmapped calls. The black is not a
  renderer defect: the map surface is blitted every frame and every tactical
  pass executes. The whole 512x512 cell array shows **5,985 allocated, 193
  tiles, 1,124 seen** (bbox (34,57)-(83,102)); the camera target is the shipped
  `HomeCell=98` → cell (68,76), not a defect. `TS_OBJECTS` n=0 is the per-frame
  renderer list `0x7b2d98`, not the world.
- **The next work item is scripted tactical input.** Select a unit and issue a
  move order, then verify its `CoordStruct` (`ObjectClass +0x40`, 256
  leptons/cell) changes; also check whether the sidebar build buttons respond.
  This is not yet proven, and the model is not a playable game.
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
  --budget 500000000000, ESC skip (HISTORICAL, pre-TLS/CLSID): stop
    0x00543669 (3x4 matrix*vector transform), depth 100; pages
    12,637/32,768, refused 0; 0 unmapped; coverage 158,667 / 9,607;
    GetMessage=19 DispatchMessage=19 PeekMessage=23,117,313; 8 surfaces,
    18,691,958 blits, 46,753,350 locks
    frame: re/out/frame_500b.png, md5 4fc1cec7..., 87,794 non-black
          = the tactical HUD (Options, credits 5000, GDI radar, build tabs)
  --budget 6000000000, ESC skip (HISTORICAL, pre-TLS/CLSID),
    TS_OBJECTS=1 TS_MAPCELLS=1 --surfaces:
    objects: n=0 array=0x00000000   (per-frame list, not the world)
    mapcells: alloc=1681 tiles=12 seen=0 fog=0; heights 604/671/562 decode
    surfaces: primary non-black 87794 (HUD); map 472x480 non-black 7080
              (rows 0-22); radar 99x108 non-black 0
    TS_EXEC_CHECK: all 11 tactical passes executed; all 5 reveal/sight
              updates (0x476000, 0x4760f0, 0x477670, 0x513bf0, 0x476180) not
  --budget 20000000000, ESC skip (after the timer fix): stop 0x00695dad
    (budget) after 20,000,000,001 instructions, depth 100;
    GetMessage=19 PeekMessage=13,760,224 DispatchMessage=19;
    pages 13,698/32,768; coverage 231,260 / 13,433; 8 surfaces,
    25,913 blits, 156,314 locks; 0 unmapped; WWMouse (355,397);
    scroll (-408,1248); visible rect (53,45,23,43); map surface 96,220
    non-black; primary 176,809
  --budget 500000000000, ESC skip (after the timer fix): 0 unmapped;
    pages 13,698; coverage 231,365 / 13,437; message loop dispatching
    frame: re/out/frame_500b_centered.png, md5 e8ba8219..., 176,796
          non-black = terrain, structures, units, Tiberium, HUD, cursor;
          the remaining black is unexplored shroud
    mapcells (whole 512x512 array): alloc=5985 tiles=193 seen=1124;
          seen bbox (34,57)-(83,102); camera target HomeCell=98 -> (68,76)
    registries: ObjectClass 4, infantry 21, buildings 36, houses 7
  --budget 20000000000 / 500000000000, ESC skip (pre-timer, historical):
    20B stop 0x0041398c, coverage 230,640 / 13,415, 57,074 blits,
    254,041 locks; 500B stop 0x0042322c, coverage 230,658 / 13,417,
    1,942,509 blits, 5,971,443 locks; frame re/out/frame_500b_objects.png
    (93,404 non-black, revealed area in the viewport's bottom-right corner)
  --budget 1200000000000, ESC skip (1.2e12), historical pre-e5d7039:
    fault at 0x00422de0 after 478,970,534,425 instructions; pages
    1,024,211/2,097,152; 7,771,444 unmapped calls; black frame; does not
    reproduce after the x87 D8 fix

tools/test_tools.py
  TOOL TESTS PASSED (106 checks; win32-tls, win32-clsid and win32-timer
  mutation-proven)

tools/test_runtime.py
  RUNTIME TEST PASSED (110 checks, mutation-proven); frame dump produced

re/out/frame_500b_centered.png
  640x480, md5 e8ba8219..., 176,796 non-black: terrain, structures, units
  and a Tiberium field, with the tactical HUD and the mouse cursor; the
  remaining black at the top and right is unexplored shroud

re/out/frame_500b_objects.png (historical, pre-timer)
  640x480, md5 e788f40b2db184885b9f25dab79380b0, 93,404 non-black: the
  tactical HUD plus the revealed base area drawn in the viewport's
  bottom-right corner

re/out/frame_500b.png (historical, pre-TLS/CLSID)
  640x480, md5 4fc1cec7..., 87,794 non-black: the tactical HUD
  (Options, credits 5000, GDI radar eagle, build tabs/grid); map viewport black

re/out/frame.png
  640x480, md5 0fdf2636c2643ce2510ae67fb9ca9e26, 254,610 / 307,200 non-black
  vs /tmp/retail.png (58cb9c0b...): 102,836 pixels differ (33.48%),
  mean abs channel diff 1.06/255 over differing pixels, 506 pixels >8

tools/wine_env.sh
  retail game: main menu, 640x480, md5 58cb9c0bb64a1bf11c6e9b28e96eca11
  still running after 60 s (exit 124 = a loop, not a crash)
```
