# STRATEGY — the shortest credible path to a working, playable Tiberian Sun

Written against HEAD `8ce1ebe`; the figures below were re-measured on
2026-10-17 with the commands named in each row. Numbers quoted from a document
instead of measured say so. Anything I could not verify says **unverified**.
Re-measure before trusting this file: `python3 tools/harness.py metrics
--no-append`. The boot, frame, link-test and oracle figures were re-measured
after the campaign-selection/mission-start merge (`boot.py` with `TS_CD` at 20M
and 2.1B, plus the ESC-skip script, `link_test.py`, `test_runtime.py`,
`wine_env.sh`); the full `pipeline.sh` run recorded in `todo.md` (exit 0, 8m51s)
was not re-run here.

This is not a status report (`PROGRESS.md` is). It is the argument about what to
do next and what it costs.

---

## 0. The answer in one paragraph

The project is not blocked on recovery, transcription, layout, naming or
source-shaping. All five are done or are busywork. It is blocked on **a runtime**:
a machine model and Win32/DirectDraw surface faithful enough to execute the game's
own content pipeline. The roadmap points at the other thing — "modifiable source"
— so its two largest items (naming virtuals, source-shaping) move the playability
needle by zero. And the biggest unclaimed asset in this repository is that **the
retail game already runs on this machine under Wine, at its preferred base
address**, which gives the project the bit-exact differential oracle its
documentation says cannot exist.

---

## 1. Where it actually is

| Measure | Value | How |
|---|---|---|
| `verify.py` | **PASSED**, 9 checks, ~6 s | `tools/verify.py src/include build/shim` |
| checks 1/3/4/5/6 | 265 headers, 749/749 classes, 749/749 bindings, 219/219, 220/220 | same |
| check 7 | 1,317/1,317 member offsets, 206 classes | same |
| check 8 (arity) | **331/1,116 match, 785 divergence**, reported not gated | same |
| check 9 (`offsetof`) | **3,073/3,073 compiler-confirmed** | same |
| recovered functions | 43,557 | `harness.py metrics` |
| instructions in `out/functions.json` | 845,093 | summed over `insns` |
| transcriptions emitted | 45,947 (912,879 instruction lines) | `grep` over `src/generated/code` |
| `.text` | 95.86% as code, 99.99% attributed; 213 switch tables / 6,388 B | metrics |
| classes / slots | 749 / 9,810, 0 unseeded | metrics |
| member offsets / named | 4,289 / **291** | metrics |
| virtual slots | **5,557** named `(class, slot)` pairs of 9,810; **917** distinct names (**281** semantic); headers declare 4,499 named + 2,458 `vfunc_N`; `metrics`' `virtual_names: 702` counts *classes with a naming record*, not names | metrics + `out/virtual_names.json` |
| indirect call sites | **18,056 of 60,449 (29.9%)** | walked `insns` |
| functions with no direct caller | **31,834 (73.1%)** | direct-edge graph from `callees` |
| functions reachable from entry `0x6854e3` by direct edges | **69 (0.16%)** | BFS over `callees` |
| functions declared by the reconstructed headers | **3,024 of 43,557 (6.9%)** | unique `sub_*` in `src/generated/*.cpp` |
| boot (20M, `TS_CD`) | **20,000,001 instructions** (budget) at `0x00488800` (`sub_0048875c +0xa4`), depth 19, **0 unmapped targets, 0 stack imbalances**, message loop **reached and dispatching** (`CreateWindowExA=1`, `GetMessage=1`, `PeekMessage=5`, `DispatchMessage=1`), **55,774 distinct instructions / 4,764 of 45,947 functions entered**; CD read (`D:\TS1.dsk`, `EXPAND99..00`); primary surface `0x006beed8`=1 | `tools/boot.py` (`TS_CD=../work/cd1`) |
| boot (2.1B, default timeline) | New Campaign, NOD1, GDI1, OK; `GDI1A.MAP` read; GDI intro plays; stop `0x0067dc4f` (LCW decoder), depth 28, `GetMessage=17`, `PeekMessage=13,565,102`, `DispatchMessage=17`, coverage **84,130 / 6,431**, `pages-used 9,029/32,768`; frame 194,131 non-black | same (`--timeout 600`) |
| boot (4B, ESC skip) | stop `0x0055b0bb` (budget) after 4,000,000,001 instructions, depth 29; coverage **139,250 / 8,682**; pages **12,809/32,768**; 8 surfaces, 12 blits, 769 locks, 0 flips; frame 290,399 non-black (loading screen, md5 `6cb0e1ff…` = `re/out/render_2_6b.png`); **11.7 s wall** | same |
| boot (100B, ESC skip) | stop `0x0055b0bc` (budget); pages **16,343/32,768**; loader on vector 7/22 with **160,070 entries**; **243.9 s wall** | same |
| boot (500B, ESC skip) | **tactical HUD renders**: budget at **500,000,000,001** instructions, EIP `0x00543669` (3x4 matrix·vector transform), depth 100; pages **12,637/32,768** (refused 0); **0 unmapped**; coverage **158,667 / 9,607**; `GetMessage=19`, `DispatchMessage=19`, `PeekMessage=23,117,313`; 8 surfaces, **18,691,958 blits, 46,753,350 locks**; frame `re/out/frame_500b.png` md5 `4fc1cec7…`, **87,794 non-black** (Options, credits 5000, GDI radar eagle, build tabs/grid); **map viewport still black** | same |
| boot (1.2e12, ESC skip) | historical, pre-`e5d7039`: fault at `0x00422de0` after 478,970,534,425 instructions, 1,024,211/2,097,152 pages, 7,771,444 unmapped calls, black frame; caused by the x87 D8 emitter bug (scale 108 instead of 1.125) and **does not reproduce** after the fix | same |
| interpreter | **~400M instructions/s** (12-14x the old ~28.6M/s), byte-identical machine state; 4B 11.7 s, 20B 50.2 s, 100B 243.9 s | `test_machine_perf` + boot |
| frame | `re/out/frame.png`: 640x480, md5 `0fdf2636c2643ce2510ae67fb9ca9e26`, 254,610 / 307,200 non-black; vs retail: 102,836 pixels differ (33.48%), mean abs channel diff **1.06/255** over differing pixels (0.35 over frame), 506 pixels >8 | `tools/boot.py --frame` |
| link test | **OK, exit 0**, 492 TUs, 20,000,001 instructions, **message loop reached** (`GetMessage=1`, `PeekMessage=5`, `DispatchMessage=1`), 0 unmapped, 0 unmodelled | `tools/link_test.py` |
| boot strict gate | **exit 0**, 0 garbage call targets | `boot.py --no-build --strict` |
| retail oracle | reaches the **main menu**, 640x480, screenshot md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s | `tools/wine_env.sh` |
| Win32 surface | **341** PE import-directory thunks, all resolved; the stub table has **342** rows (the extra is `COMCTL32!DllGetVersion`, reached through `GetProcAddress`): **315 with a body, 27 generic** | `link_test.py` / boot |

In words:

1. **The binary is done.** Recovery, transcription and layout are off the
   critical path. A round spent there is a round spent on polish.
2. **The code has no name.** 73% of functions are reached only indirectly, the
   direct call graph reaches 69 functions from `main`, and the headers bind to
   3,024 of 43,557 functions. Whatever "named" means in the metrics, the tree a
   reader can *call* is 6.9% of the program.
3. **The runtime is the product.** What exists is a run of the game's own
   startup that **delivers messages, reads the CD and the MIX index, builds its
   real dialogs, renders and selects the main menu, populates the campaign list,
   starts the GDI campaign, and plays its intro movie**: at 2.1B the stop is the
   LCW decoder `0x0067dc4f` (`pages-used 9,029/32,768`), with the GDI title
   frame (194,131 non-black). `re/out/frame.png` is 640x480 and within
   **1.06/255** mean channel difference of the retail screenshot on the pixels
   that differ. COM registration, message delivery, the dialog layer,
   narrow-width flags, the Blowfish cipher, the stack tracker, the teardown
   calls, signed `idiv`, rendering, input, menu selection, the listbox/trackbar
   layer and the mission start are all fixed. The interpreter is **12-14x
   faster** (~400M instructions/s) with byte-identical state. The 479B
   out-of-bounds write was the **x87 emitter** (D8 one-register forms rendered
   as pop forms; 901 instructions), fixed in `e5d7039`; after the fix the
   tactical zoom is 1.125, the map surface 99x108, the visible rect
   `35,28,23,43`, and there are 0 OOB stores. A 500B run now **renders the
   tactical HUD** (`re/out/frame_500b.png`, 87,794 non-black) with 0 unmapped
   calls, 12,637 pages, 18,691,958 blits and 46,753,350 locks. The **map
   viewport is still black**; only the HUD renders, so the mission map does
   **not** render yet.
4. **The Win32 layer is real where the game has been, and fiction where it has
   not** (`Win32Stubs.h`, read):
   - File I/O: **real** — a host root, case-insensitive paths, a 64-entry handle
     table, real `CreateFileA`/`ReadFile`/`WriteFile`/`SetFilePointer`/
     `GetFileSize`/`CloseHandle`. The old "there is no real file system" text is
     gone.
   - Input: **real and scripted** (`Input.h`): key/mouse state, cursor and
     synthesized messages from a deterministic timeline; the default timeline
     delivers 10 messages and the game's key queue consumes them.
   - Timers: `timeSetEvent` returns 1 and never fires its callback.
   - Surfaces: **real** (`Rendering.h`): per-surface RGB565 memory, `Lock`,
     `Blt`/`BltFast`/`Fill`/`Flip`, descriptors and palette; `--frame` dumps the
     primary surface. The main menu renders.
   - Strings: `LoadStringA` has the correct arity (4) and reads LANGUAGE.DLL's
     string table; the DirectSound warning text is still the format string.

So: **structure recovered, instructions transcribed, layout compiler-confirmed,
and a runtime that has executed 2.1 billion instructions through message
delivery, the CD/MIX path, real dialogs, the intro movie, a rendered and
selectable main menu, the campaign list, and into the GDI campaign's intro movie,
on a Win32 layer that can load a file, create a window, dispatch messages, read
the MIX index, build dialogs, take scripted input, hit-test a real button,
populate a listbox, start a mission, and render the in-game tactical HUD — but
cannot yet render the map viewport (it is still black), fire a timer, or play a
sound.**

### The fact the docs do not contain

```
$ ./tools/wine_env.sh
$ /proc/<GAME.EXE>/maps         -> 00400000-00698000 r-xp ... GAME.EXE
$ xdotool getwindowname         -> "Tiberian Sun" (640x480)
screenshot                      -> the GDI/Nod main menu, 640x480
                                   md5 58cb9c0bb64a1bf11c6e9b28e96eca11
                                   still running after 60 s (exit 124 = a loop)
```

The retail binary **runs under Wine here, maps at exactly 0x00400000, and reaches
its main menu.** `tools/wine_env.sh` is the reproduction: an X screen at
640x480x16 (the mode the game asks DirectDraw for) and a `DRIVE_CDROM` `d:`
labelled `TS1` holding `TS1.DSK`. It dismisses the DirectSound warning and plays
`WWLOGO.VQA`/`STARTUP.VQA`. Every recovered VA is live in a real process:
`gdb -p`, `break *0xVA`, read EAX/ESP/EBP and 64 bytes of stack, compare with the
model at the same VA. That is a differential oracle at instruction granularity.
`AGENTS.md` used to assert the opposite — *"Semantic equivalence with the
original cannot be proven here (there is no MSVC 6 toolchain, only g++)"* — true
about compiling, wrong about observing. The remaining oracle limits: the menu's
buttons/text are not drawn (the PCX assets live inside a nested MIX entry) and
audio is absent.

Caveat: Wine is not Windows, so it cannot adjudicate a *Win32 stub's* behaviour.
It can adjudicate the x86 model, because that is the real CPU running the same
bytes.

---

## 2. The critical path, ranked by downstream fan-out

`→` means "prerequisite for".

1. **Instruction-level model fidelity** (control flow complete, ESP right, flags
   right). → everything. The retracted `PeekMessage=137,295` is proof: a model
   bug makes every downstream observation fiction. `gen_code.py` now emits
   `ts_tail` continuations — measured this session:
   `45947 functions transcribed, 912879/912879 instruction lines modelled, 0
   unmodelled`; **12,383 spans continue by tail transfer**, 2 overlap another
   span's instruction and 13 land in unrecovered bytes. The control-flow gate
   now **fails** on a span that ends with neither a transfer nor a registered
   continuation, and is proven by mutation. This item is closed for the startup
   path; **the shutdown path is now the open one** — WinMain returns with ESP
   `0x11` and the CRT atexit walk runs away (queued in `todo.md`).
2. **The Wine oracle.** → every later fidelity claim, including 1. **Done and
   past startup:** `tools/wine_env.sh` reaches the retail main menu (640x480,
   still running after 60 s). `tools/oracle.py` reads the real registers and
   `--compare` diffs them against the model. It turns "the arguments look wrong"
   into a diff.
3. **Real file I/O + MIX + a virtual file system.** → every content path: INI,
   art, maps, audio, movies. **File I/O is now real** (`Win32Stubs.h`: a host
   root, case-insensitive path resolution, a 64-entry handle table, real
   `CreateFileA`/`ReadFile`/`WriteFile`/`SetFilePointer`/`GetFileSize`), and
   `tools/mix.py` decodes both MIX index variants. The model's MIX index read is
   **fixed** (`1c0db7e`): all 27 blocks are read and the 17 records match
   `tools/mix.py` 17/17. What remains is routing `MixFileClass` through the real
   reader, not inventing a file layer. This was the largest missing block and it
   has moved.
4. **Surface memory, real blits, a frame dump.** → anything visual, and any
   judgement that the game works. Needs 3 to have content to draw.
5. **A virtual clock with timer callbacks.** → the logic tick. A
   `timeSetEvent` that never fires means timer-driven logic never runs.
6. **Input.** → "playable" rather than "a demo that plays itself".
7. **Fidelity at scale.** Unbounded in principle, bounded by the oracle: run
   both, diff at checkpoints, fix the first divergence. This is where the months
   are, and it is only tractable with 2.
8. **Speed.** 20,000,001 instructions in 1.65 s wall is order 12M instructions/s —
   and that includes image mapping, so treat it as an upper bound on cost, not a
   clean throughput. A 30 fps budget is ~330k instructions/frame. If a TS frame
   costs millions, the interpreter is 10–30× too slow. Measure before promising.
9. **The MS*/AVI movie layer — done.** The model plays past the intro, reaches
   the menu, starts the GDI campaign, and plays the GDI intro movie: at 2.1B the
   stop is the LCW decoder `0x0067dc4f` (84,130 / 6,431, `pages-used
   9,029/32,768`, 194,131 non-black frame). The rendering/input integration
   (`Rendering.h`, `Input.h`) is what made the menu visible and clickable.
10. **Menu-item selection — done.** `DLGTEMPLATEEX` is parsed, the six
    `BS_OWNERDRAW` buttons have real rectangles, and press/release →
    `BN_CLICKED` reaches the game's own hit-test and `WM_COMMAND` path. New
    Campaign opens `RT_DIALOG` 148 "Select Campaign:"; Exit Game reaches
    `ExitProcess(0)`. The nested MIX (`0xa8548fd9`, 317 entries) is mounted and
    read.
11. **Campaign list and mission start — done.** Shipped `BATTLE.INI`
    (`[Battles] 1=GDI1, 2=NOD1`) feeds listbox 1109 through the model's full
    listbox/trackbar layer; OK selects GDI, `GDI1A.MAP` is read from MAPS01.MIX,
    and the GDI intro movie plays (2.1B stop `0x0067dc4f`, 84,130 / 6,431).
12. **The 479B wild call — root-caused to the x87 emitter and fixed
    (`e5d7039`).** `tools/emit_x86.py` rendered the D8 one-register forms as pop
    forms, so `fdiv st(1)` wrote `ST(1)=ST(1)/ST(0)` instead of
    `ST(0)=ST(0)/ST(1)`; 901 non-pop one-operand x87 instructions were affected.
    `FUN_005a1bd0`'s tactical zoom became 108 instead of 1.125, producing the
    bogus `2125x4176` visible rect and the out-of-bounds vptr write. After the
    fix: scale 1.125 (`0x3f900000`), map surface 99x108, visible rect
    `35,28,23,43`, vptr `0x006a5dfc`, 0 OOB stores.
13. **The tactical HUD renders — first in-game frame.** The 500B run stops at the
    budget at `0x00543669`, depth 100, pages 12,637/32,768 (refused 0), **0
    unmapped calls**, coverage 158,667/9,607, 8 surfaces with 18,691,958 blits
    and 46,753,350 locks. `re/out/frame_500b.png` (87,794 non-black) shows
    Options, credits 5000, the GDI radar eagle, and the build tabs/grid.
14. **The black map viewport — the current blocker.** Only the HUD renders; the
    map does not. Trace the terrain/map blits (destination surfaces, the primary
    map rectangle now that the visible rect is `35,28,23,43` on a 99x108
    surface) with per-surface counters and dumps; do not paint the map by hand.
    The mission map does **not** render yet.
15. **The interpreter — done.** 12-14x faster (~400M instructions/s vs
    ~28.6M/s) with byte-identical machine state; `test_machine_perf` pins the
    hot path and is mutation-proven.

**Not on the path:** naming the 2,458 `vfunc_N` slots, source-shaping 45,947
transcriptions, the last 110 KB of `.text`, re-deriving member offsets. All four
serve the *source artifact*. They are the right work only if the deliverable is
documented source; ROADMAP P2 and P3 have no consumer in the playability goal.

---

## 3. The hard truths

- **"Decompile it, compile the result into a game" is not a plan.** 93% of
  functions have no declaration, the 7% that do are transcriptions that read
  arguments from `ts::` registers rather than C++ parameters, and there is no
  MSVC 6. The model is what runs. Any plan ending in "then it runs as native
  code" is fantasy.
- **Playable Tiberian Sun already exists on this machine; it is called Wine.** If
  the literal deliverable is "a playable TS", the shortest path is Wine
  configuration plus input plumbing — hours. Be explicit about which product you
  are building: *a playable game* (use Wine) or *our own code running the game*
  (build the runtime). The repo has never said, and that ambiguity is why the
  roadmap optimises for a third thing.
- **Naming is the roadmap's biggest item and is worth nothing here.** 917
  distinct names already cover 5,557 `(class, slot)` pairs; naming the remaining
  2,458 `vfunc_N` header declarations changes no byte of execution. Defensible
  only as documentation.
- **Source-shaping has no honest exit test.** ROADMAP P3 wants "≥50% of
  instructions in the 500 highest-centrality functions". There is no centrality
  that survives measurement: 69 functions reachable from `main`, 73% with no
  direct caller, 30% of call sites indirect. A lifter's only real check is
  behavioural equivalence — which needs the oracle. Months, unbounded, and it
  makes nothing run.
- **Graphics now has an oracle and a matching frame.** `tools/wine_env.sh` gives
  a real main-menu screenshot (640x480, md5
  `58cb9c0bb64a1bf11c6e9b28e96eca11`), and the model's committed
  `re/out/frame.png` (md5 `0fdf2636…`) differs from it by a **1.06/255** mean
  channel difference on the pixels that differ (0.35 over the whole frame), with
  only 506 pixels >8. The menu's buttons/text are not drawn in either because
  the PCX assets live inside a nested MIX entry, so the oracle cannot yet
  adjudicate the UI controls.
- **Sound stays silent without a mixer.** `DirectSoundCreate` now returns
  `DSERR_NODRIVER` (matching the retail no-audio environment), and no buffers are
  filled or mixed. Low priority, but do not discover it at the end.
- **The 27 generic stubs are silent holes**, and the project's own lesson
  applies: a stub returning a plausible wrong value is worse than one that fails.
  `LoadStringA` now has the correct arity but returns 0 because the string table
  is empty; the box renders blank and nothing says why.
- **The Blowfish cipher and the MIX index read are fixed.** The retail oracle
  measures 9 `SetKey(56,key)` calls and 1,586 `Decrypt(8,in,out)` calls during
  startup, matching nine shipped MIX indexes; `345aae7` implements standard
  Blowfish-ECB and proves it against four known-answer vectors and a
  byte-identical `tools/mix.py` cross-check. `1c0db7e` fixes the index read: all
  27 blocks are read and the 17 records match `mix.py` 17/17.
- **The dialog layer is real (`f576607` + `c81c1f5`), and the CD path works.**
  `Win32Stubs.h` reads LANGUAGE.DLL's 60 dialog templates and string table from
  the module's own `.rsrc`; `CreateDialogIndirectParamA` builds the record,
  sends `WM_INITDIALOG`, and `DispatchMessage` runs. `TS_CD` models `D:`
  (`DRIVE_CDROM`, label `TS1`), and the game reads `D:\TS1.dsk` and enumerates
  `EXPAND99..00`. Narrow-width flags (`f2e6ca9`) and the generator's
  `SetViewportOrgEx`/`SetWindowOrgEx` arity (`19ee16c`) are fixed too.
- **Rendering, input, the movie path, menu selection, the mission start and the
  tactical HUD work; the map viewport does not.** The model plays past the intro,
  renders and selects the real menu, populates the campaign list from shipped
  `BATTLE.INI`, and OK starts the GDI campaign: `GDI1A.MAP` is read and the GDI
  intro movie plays (2.1B stop `0x0067dc4f`, 194,131 non-black frame). The 479B
  out-of-bounds write was the x87 emitter (D8 one-register forms; 901
  instructions), fixed in `e5d7039`; the 500B run then renders the **tactical
  HUD** (`re/out/frame_500b.png`, 87,794 non-black) with 0 unmapped calls. **The
  map viewport is still black**, so the mission map does not render. The nested
  MIX (`0xa8548fd9`, 317 entries) is mounted and read; the button art is not
  painted (no `WM_PAINT` to children; retail does not draw it either).
- **The VQA frame-wait is stub-bound.** ~68M `PeekMessageA` calls in 600 s, so
  model time barely advances during movies; ESC is the practical route to the
  mission load until the clock/pacing model is improved.
- **The shutdown ESP defect is dormant.** WinMain returned at ~7.66M
  instructions with ESP `0x11` (entry `0x2fff84`) before the MIX-read fix; the
  current run never reaches shutdown. It is the same class as the original
  108-byte leak, on the exit path.
- **x87 is right; FPU control state is unverified.** `Machine.h` keeps the x87
  stack in host `long double` (80-bit here), which is correct. The rounding-mode
  and precision-control bits of the FPU control word are **unverified**; a game
  that changes them diverges slowly and looks like an AI bug.
- **Do not touch networking.** 18 WSOCK32 imports and the WOL/ATL COM layer are
  irrelevant to skirmish against the AI, the only "playable" that matters.
- **Every new resource repeats the page-table lesson.** The 8,192-slot table
  corrupted long runs silently. Surfaces, file handles, timer ids and per-surface
  memory can all reintroduce exactly that. Make exhaustion a reported fault.

---

## 4. The next ten units of work

Each has an exit criterion a command can decide. **Units 1–5 are done** (the
Wine oracle reaches the main menu, the stack leak is fixed, the control-flow gate
is in `gen_code.py`, boot reports coverage and target classification, and file
I/O is real); **message delivery, the Blowfish cipher, the MIX index read, the
stack tracker, the teardown calls, signed `idiv`, the dialog layer, narrow-width
flags, the generator arity, the MS*/AVI movie path, rendering, input, menu
selection and the campaign list/mission start are also done** (units 11–25
below), as are **the growable page table, the heap reuse, the interpreter
speedup, the 479B run, its diagnosis and the x87 fix, and the tactical HUD (units
26–30)**. Units 6–10 remain, and **the black map viewport (unit 31) is the
current blocker**.

1. **Wine oracle (`tools/oracle.py`).** `tools/wine_env.sh` runs the retail game
   with the CD and a 640x480x16 screen; `oracle.py` attaches gdb, breaks at an
   address, and prints registers + 64 bytes of stack. *Exit:* the oracle reaches
   the main menu and `--compare` diffs the registers against the model.
   **Done.**
2. **The stack imbalance (owned by the parent).** Fixed: `ts_call_ptr` delegates
   to `ts_call_at`; the model reached its message loop. *Exit:* met; a **new
   shutdown ESP defect** (`0x11` on WinMain return) is unit 14.
3. **Gate control-flow completeness.** `gen_code.py` fails when a span ends with
   neither a transfer nor a registered continuation, proven by mutation.
   **Done.**
4. **Make the boot report coverage.** `boot.py` prints `coverage: N distinct
   instructions; N of 45,947 recovered functions entered` and classifies unmapped
   targets (`below 0x1000`, `.rdata`, `elsewhere`). Measured now (with `TS_CD`):
   55,774 distinct instructions / 4,764 functions at 20M, 84,130 / 6,431 at 2.1B,
   **0 unmapped**, `--strict` exits **0**. **Done.**
5. **Real file I/O.** Real `CreateFileA`/`ReadFile`/`WriteFile`/`SetFilePointer`/
   `GetFileSize`/`CloseHandle` over a host root, handles as real objects.
   **Done.**
6. **MIX in C++.** Port `tools/mix.py` (proven: both index variants decode,
   header+index+body equals file size) and route `MixFileClass` through it.
   *Exit:* the model opens `TIBSUN.MIX` and entry count and offsets match
   `mix.py` byte for byte. The cipher and the index read are done (all 27 blocks,
   17/17 records); routing `MixFileClass` through the reader remains.
7. **Framebuffer.** Per-surface memory, real `Blt`/`BltFast`/`Fill`/`Flip`,
   `boot.py --dump-frame=N` writing a PNG. *Exit:* a non-blank PNG whose palette
   and dimensions match a Wine screenshot of the same screen, reported as a
   pixel diff.
8. **Virtual clock.** One clock advanced by `Sleep`, `timeGetTime`,
   `QueryPerformanceCounter` and instruction count; `timeSetEvent` fires its
   callback when due. *Exit:* 300 iterations of the game's own loop, each
   advancing 1/30 s, frame counter reaching 300.
9. **Input.** A key/mouse table fed by a script (SDL later); `GetAsyncKeyState`,
   `GetKeyState`, `GetKeyboardState`, `GetCursorPos` read it; `PeekMessageA`
   synthesises matching messages. *Exit:* a scripted click at (400,300) moves the
   game off its main menu, visible in the frame dump and the import trace.
10. **Speed.** `perf record` the driver exactly as the page-table bug was found,
    then batch/specialise hot opcodes. *Exit:* ≥100M instructions/s on the same
    run, measured with `--budget`, not inferred from wall time.

After 10 the project has either a visible, input-driven, data-loading game — a
defensible "playable" — or a precise list of divergences from Wine, which is the
only honest way to close the rest.

**Units added since the list was written, in order:**

11. **Deliver queued messages.** `CreateWindowExA` queues `WM_ACTIVATEAPP`, the
    WndProc runs, `DAT_007b2e50` becomes 1. **Done (`f3011ca`).** Measured at
    that commit: 1,882,051 instructions, `GetMessage=1`, `PeekMessage=3`,
    `DispatchMessage=1`, coverage 49,635 / 4,455.
12. **Implement the Blowfish cipher.** Standard Blowfish-ECB, proven by four
    known-answer vectors and a byte-identical `tools/mix.py` cross-check.
    **Done (`345aae7`).** The page-table exhaustion at `0x00a006b0` is gone.
13. **Fix the MIX index read.** **Done (`1c0db7e`):** the root cause was the
    unaligned `memcpy` jump table; all 27 blocks are read and the 17 records
    match `tools/mix.py` 17/17.
14. **Fix the shutdown ESP corruption.** WinMain returns with ESP `0x11` (entry
    `0x2fff84`). **Dormant:** the current run never reaches shutdown. *Exit:*
    WinMain returns with ESP where its caller left it.
15. **Fix the null teardown calls / 10 in-text unmapped calls.** **Done
    (`47d26eb`):** one root cause, the `IDirectDrawSurface::Lock` arity; 0
    unmapped, `boot.py --strict` exits 0, link test passes.
16. **Fix the stack-imbalance tracker.** **Done (`d047496`):** 111,682 false
    imbalances -> 0.
17. **Fix signed `idiv`.** **Done (`16e21fa`):** true signed x86 semantics.
18. **Implement the dialog layer.** **Done (`f576607` + `c81c1f5`):** LANGUAGE.DLL's
    60 templates and string table are read from the module's `.rsrc`;
    `CreateDialogIndirectParamA` builds the record and controls, sends
    `WM_INITDIALOG`, and `DispatchMessage` runs. A CD drive model (`TS_CD`) was
    added.
19. **Fix the generator's `SetViewportOrgEx`/`SetWindowOrgEx` arity.** **Done
    (`19ee16c`):** 4, not 3; `--stack-check` is 342/0.
20. **Fix narrow-width flags.** **Done (`f2e6ca9`):** width-aware
    `f_add_w`/`f_sub_w`/`f_inc_w`/`f_dec_w`/`f_log_w`; the vsprintf `%` and the
    CD-drive scan are correct, and the flags are compared against the CPU's own.
    `test_tools.py` is 101 checks.
21. **Make the CD path work.** **Done:** with `TS_CD`, `CreateFileA("D:\TS1.dsk")`
    succeeds, `GetDriveTypeA` no longer receives `%c:\`, and `EXPAND99..00` are
    enumerated.
22. **Implement the MS*/AVI movie layer.** **Done:** the model plays past the
    intro, reaches the menu, starts the GDI campaign, and plays its intro movie
    (2.1B stop `0x0067dc4f`, 84,130 / 6,431).
23. **Integrate rendering and input.** **Done (`5031e44` etc.):** `Rendering.h`
    and `Input.h` are wired through `Win32Stubs.h`; the model renders the real
    main menu (`re/out/frame.png`, 1.06/255 from retail on differing pixels);
    `test_runtime.py` is 101 checks.
24. **Make the menu selectable.** **Done (`8474345` … `fa3e2a0`):** the six
    `BS_OWNERDRAW` buttons have real rectangles; press/release → `BN_CLICKED`
    reaches the game's own hit-test and `WM_COMMAND` path. New Campaign opens
    `RT_DIALOG` 148 "Select Campaign:"; Exit Game reaches `ExitProcess(0)`.
    `0x7b2ab8`/`0x7b0d38` do not change; the signal is the dialog result through
    `GWL_USERDATA`.
25. **Implement listbox/combobox/trackbar content and the mission start.**
    **Done (`1966cc1` + `4b7ef9b`):** the list source is shipped `BATTLE.INI`;
    the model implements the full listbox/trackbar layer, real `FindFirstFileA`
    enumeration and a read-only `TS_DATA` root. OK selects GDI, `GDI1A.MAP` is
    read, and the GDI intro movie plays (2.1B stop `0x0067dc4f`, 84,130 / 6,431,
    194,131 non-black frame).
26. **Make the page table growable and reuse freed heap blocks.** **Done:** the
    table grows past 32,768 (2,097,152-page cap) with stable page identity, and
    the heap coalesces/reallocs in place. At 4B the mission load uses 12,809
    pages instead of 377,796 (50 MB instead of 1.44 GiB), 95.5% of allocations
    reused.
27. **Speed up the interpreter.** **Done (`9243e93` … `6421f30`):** 12-14x
    faster (~400M instructions/s) with byte-identical machine state; 4B in
    11.7 s, 20B in 50.2 s, 100B in 243.9 s. `test_machine_perf` pins the hot
    path.
28. **Run the load past 100B.** **Done:** a 1.2e12-budget run reached
    478,970,534,425 instructions and faulted at `0x00422de0` with the page
    table refusing a page (1,024,211/2,097,152), 7,771,444 unmapped calls and a
    black frame. The loader got far past 100B; the tactical phase was **not**
    reached.
29. **Find the first wild call in the 479B run.** **Done (`6a31165`):** it is
    deterministic at step 478,300,935,316 — `call [edx+0x2c]` at `0x005fa96b` →
    `0x46000062`, EDX=`0x6a5dfe` = TerrainClass vtable `0x6a5dfc` + 2.
    `FUN_005a34e0`'s cell-seen bitmap setter wrote 34,062 B before its bitmap
    (index `67*(-4068)+65 = -272,491`) at `0x005a3558`, corrupting the vptr. The
    page refusal is the 32-bit process's ~4.2 GB address space, not a table cap.
30. **Find the zoom/scale divergence.** **Done (`e5d7039`):** the x87 emitter
    rendered D8 one-register forms as pop forms, so `fdiv st(1)` computed the
    wrong destination; `FUN_005a1bd0`'s tactical zoom became 108 instead of
    1.125. The fix covers 901 non-pop one-operand x87 instructions and 951
    regenerated lines. After: scale 1.125, map surface 99x108, visible rect
    `35,28,23,43`, vptr intact, 0 OOB stores.
31. **Render the map viewport.** **Open — the current blocker.** The 500B run
    renders the tactical HUD (`re/out/frame_500b.png`, 87,794 non-black) with 0
    unmapped calls, but the map area is black. Add per-surface blit/lock
    counters and surface dumps; find the terrain blits' destination and whether
    the map is an offscreen surface never composited, or the primary map
    rectangle is clipped/scrolled wrongly. *Exit:* the map viewport renders
    terrain/tiles in the dumped frame, or the next precise renderer defect is
    named.

---

## 5. What the docs got wrong — status after the 2026-10-17 measurement sweep

Ordered by damage. Same failure mode the project already caught once: prose that
measurement does not support. Each item is now marked **RESOLVED** (with the
corrected statement and the commit/date) or **OPEN** (with what is still false).
The first batch of corrections is commit `8467f84`; `todo.md`, `REPORT.md` and
this section are carried by `be04940`; the AGENTS.md figure sweep is `27e7f59`
(all 2026-10-17).

1. **RESOLVED — `AGENTS.md`'s retracted boot block.** The false
   `budget 5000000 / message loop: reached (PeekMessage=137295)` block is gone;
   the file now reports the measured stop and `message loop: not-reached`
   (commit `b968950`, swept again in `c858273`; figures superseded by item 17).
2. **RESOLVED — `ROADMAP.md` P4's "4.2 is DONE ... frame loop".** The section now
   states 4.2 is **not** done, records the retraction and the 108-byte leak, and
   reports the measured boot (item 17).
3. **RESOLVED — `ROADMAP.md` §2.2/§2.3/P0 described a completed phase as the top
   priority.** §2.2 is now marked RESOLVED and its 2,313 `unsigned long` /
   `ObjectClass` 248-byte figures are explicitly retracted as LP64-host history.
   §2.3 is marked RESOLVED: the headers omit vptr stores, check 9 confirms
   **3,073/3,073**, and P0 is marked `[RESOLVED]` (`8467f84`).
4. **RESOLVED — the virtual-naming denominator.** `ROADMAP.md` §2.4, `todo.md`
   and this file now use one measured set: **9,810** slot instances, **5,557**
   named pairs, **917** distinct names (**281** semantic), **6,957** header
   declarations (**4,499** named + **2,458** `vfunc_N`), **702** classes with a
   naming record. The distinction is stated once in `PROGRESS.md`; the old
   "1,500 / 8,280 / 3,409" mixed quantities and is retracted (`8467f84`).
5. **RESOLVED — `src/generated/README.md` contradicted itself.** The sentence now
   says what the number measures: **3,024 of 43,557 (6.94%) recovered functions
   are declared by the reconstructed headers**, so a forwarder exists for them;
   the other 40,540 are transcribed in `code/` but no header declares them. The
   generator (`tools/gen_bodies.py`) was fixed, not the output (`8467f84`).
6. **RESOLVED — `todo.md` was internally contradictory.** The duplicate
   "Spans that do not end on a transfer" entries are one `[x]` entry; "512 MB
   memset" is one `[x]`; "Reach a window and a message loop" is one `[ ]`;
   "166 instructions have no faithful rendering" is `[x]` (0 unmodelled);
   "Compile the gate with `-m32`" is `[x]`; "Fix the IPersistStream type
   inconsistency" is one `[x]` entry. Each retracted copy keeps a one-line note
   saying what was stale.
7. **RESOLVED — `todo.md`'s data-section premise was false.** `Image.h` copies
   every section's raw extent and writes a stub VA into all **341** IAT slots;
   `link_test.py` reports `341 of 341 IAT slots written` and `0 with no stub`.
   The entry is retracted and marked done; the empty MessageBox text is
   `LoadStringA` returning 0.
8. **RESOLVED — "Function bodies are empty" was false.** `todo.md`'s standing
   caveat now says the opposite with measured counts: 45,947 transcriptions,
   912,879 instruction lines, 0 unmodelled, 492 TUs linked.
9. **RESOLVED — the import counts are now distinguished.** **341** PE
   import-directory thunks (309 by name, 32 by ordinal, 13 DLLs), all resolved;
   **342** stub-table rows (the extra is `COMCTL32!DllGetVersion`, reached
   through `GetProcAddress`); "traced" is the count of imports actually called.
   Stated in `PROGRESS.md` and used consistently in `AGENTS.md` and `CONTEXT.md`
   (`8467f84`).
10. **RESOLVED — verification figures drifted inside single files.**
    `AGENTS.md` now quotes check 7 as **1,317/1,317 across 206 classes**, check 8
    as **331/1,116 match, 785 divergence**, member verification as **4,285/4,289,
    4 refuted**, and check 9 as **3,073/3,073** with the current unasserted
    breakdown (743 vptr, 413 past extent, 44 undeclared, 10 no offset, 6
    headerless = 1,216). Measured this session.
11. **RESOLVED — `CONTEXT.md` called `verify.py` "the 8-check gate".** It now
    says nine, describes check 9, and lists `link_test.py` (492 TUs),
    `test_tools.py` (now 101 checks) and the import distinction (`8467f84`).
12. **OPEN — the framing error, which is the expensive one.** `ROADMAP.md` §2.3
    used to close with "for a decompilation that is the difference between 'it
    builds' and 'it is the game'". The sentence is gone and §4 now cross-
    references this file's argument, but the *strategic* disagreement is real
    and unresolved: the roadmap optimises the source artifact, this file
    optimises a playable runtime. Say which product you are building.
13. **RESOLVED — `AGENTS.md`'s "Current status" table carried stale counts.**
    It now uses the measured set: 43,557 spans (not 41,822), 95.86% code (not
    96.06%), 112,401 B unattributed classified as 213 static switch tables /
    6,388 B, 102 switch index tables / 145 B, 105,492 B padding, 376 B in 46
    runs (not 104 KB, 11 jump tables / 256 B, 356 B, 105,808 B, 554 B in 90
    runs); 9,810 vtable slots (not 9,780); 45,947 transcriptions / 912,879
    instruction lines (not 43,668 / 897,193); 266 headers (not 262); 2,390 tail entries (not 1,846;
    `shared_tail_entry_points` = 1,080 is a different quantity and is now
    labelled as such); 492 TUs (not 482); 4,289 member claims, 4,285 confirmed
    (not 4,326 / 4,322); check 7 1,317 offsets across 206 classes (not 2,186
    across 248); 2,458 `vfunc_N` and 291 named members (not 5,420 unnamed
    virtuals / ~4,190 unnamed members); 43 tool tests (not 36). The
    "3,409 distinct slots" claim is retracted and replaced with the measured
    slot-instance/named-pair/distinct-name set. Commit `27e7f59`.
14. **RESOLVED — `tools/boot.py` did not build on a fresh checkout.**
    `60dac16` runs reconcile + self-contained after `sync_handwritten()` copies
    the hand-written headers, so the fresh-checkout build no longer fails on
    missing COM types. Measured: `boot.py --rebuild` compiles 492/492 units and
    reports the boot.
15. **RESOLVED — "pipeline red at the link test" was false at that commit.**
    The link test's `ExitProcess` stub returned instead of halting, which
    produced the same retracted `MESSAGE LOOP: reached` artifact as
    `boot_driver`. `db612d1` wires `m.halt`, and the link test then exited **0**
    with the message loop genuinely reached from a run that never calls
    `ExitProcess`. A full `./tools/pipeline.sh` run completed exit 0 in
    `d23d1bf`. A later teardown regression (10 in-text unmapped calls) was fixed
    by `47d26eb`; the link test passes again (item 22).
16. **RESOLVED — the oracle "gives up early" was false.** `tools/wine_env.sh`
    reaches the retail **main menu** (640x480, screenshot md5
    `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s). The remaining
    limits are stated: main-menu buttons/text are not drawn (PCX assets inside a
    nested MIX entry) and audio is absent.
17. **RESOLVED — the boot's corrupted exit arguments were a model defect, not a
    stub lie.** The 108-byte atexit leak is fixed (`ts_call_ptr` delegates to
    `ts_call_at`); `jmp_indirect` and conditional `rep`/`repne` `scas`/`cmps`
    were also fixed. Measured at that commit: `MessageBoxA` type `0x30` on the
    DirectSound warning, 0 unmapped calls, `--strict` exit 0. (The later
    shutdown path reintroduced unmapped teardown calls; item 22.)
18. **RESOLVED — the message loop is reached.** `1580be3` fixed COM
    registration (named module handles and per-module exports; `blowfish.dll`
    exports `DllRegisterServer`/`DllUnregisterServer`/`DllGetClassObject`/
    `DllCanUnloadNow`; `CoCreateInstance` writes an `IBlowfish`;
    `IDirectDraw::CreateClipper` writes a clipper; `DirectSoundCreate` returns
    `DSERR_NODRIVER`). Message delivery is also fixed (`f3011ca`): measured
    1,882,051 instructions, `GetMessage=1`, `PeekMessage=3`, `DispatchMessage=1`,
    the WndProc at `0x65ba70` runs and `DAT_007b2e50` becomes 1. The loop is no
    longer idle.
19. **RESOLVED — the Blowfish cipher is implemented and proven.** The retail
    oracle (`re/out/blowfish_oracle.md`, `tools/oracle_blowfish.py`,
    `tools/wine_blowfish_trace.sh`) measured `SetKey(56,key)` **9 times** and
    `Decrypt(8,in,out)` **1,586 times** during startup, matching nine shipped
    MIX indexes. `345aae7` implements standard Blowfish-ECB, proven by four
    known-answer vectors and a byte-identical `tools/mix.py` cross-check on the
    shipped TIBSUN.MIX index (`2ce3171bdbecb40e` → `110090d97904849a`, count 17,
    body 75,094,416). The page-table exhaustion at `0x00a006b0` is gone.
20. **RESOLVED — the MIX index read is fixed (`1c0db7e`).** The root cause was
    the unaligned `memcpy` jump table, not the pipe or cipher; all 27 blocks are
    read and the 17 records match `tools/mix.py` 17/17.
21. **RESOLVED — the disasm switch-table correction (`9e5703f` + `ad62afc`).**
    213 static switch tables (6,388 B) are data; recovered functions 43,557,
    transcriptions 45,947, instruction lines 912,879, `.text` code 95.86%. Every
    lost instruction was classified.
22. **RESOLVED — the stack tracker and the null teardown calls.** `d047496`
    makes `ts_call_at` compare direct stub calls against `esp_before + 4*nargs`
    (**111,682 false imbalances -> 0**); `47d26eb` fixes the one root cause of
    the five null teardown calls (`IDirectDrawSurface::Lock` popped 4, not 5).
    Measured: 0 unmapped, `--strict` exits 0, link test passes.
23. **RESOLVED — signed `idiv` (`16e21fa`).** `divmod64` has true signed x86
    semantics; the old divide-overflow stop is passed.
24. **RESOLVED — the dialog layer (`f576607` + `c81c1f5`).** LANGUAGE.DLL's 60
    dialog templates and string table are read from the module's `.rsrc`;
    `CreateDialogIndirectParamA` builds the record and controls, sends
    `WM_INITDIALOG`, and `DispatchMessage` runs. A CD drive model (`TS_CD`) was
    added.
25. **RESOLVED — narrow-width flags (`f2e6ca9`) and generator arity
    (`19ee16c`).** Width-aware flag derivation fixes the vsprintf `%` and the
    CD-drive scan; `SetViewportOrgEx`/`SetWindowOrgEx` take 4 arguments and
    `--stack-check` is 342/0. `test_tools.py` is 101 checks.
26. **RESOLVED — the intro-movie path and the rendered main menu.** The model
    plays past the intro and renders the menu: `re/out/frame.png` (md5
    `0fdf2636…`, 254,610 / 307,200 non-black) differs from the retail screenshot
    by **1.06/255** mean channel difference on the pixels that differ, with only
    506 pixels >8.
27. **RESOLVED — menu selection.** `DLGTEMPLATEEX` is parsed and the six
    `BS_OWNERDRAW` buttons have real rectangles; press/release → `BN_CLICKED`
    reaches the game's own hit-test and `WM_COMMAND` path. New Campaign exits the
    menu with result 1 and opens `RT_DIALOG` 148 "Select Campaign:" at step
    1,688,503,557; Exit Game reaches `ExitProcess(0)` at `0x00684529` after
    1,707,666,895 instructions. `0x7b2ab8`/`0x7b0d38` do not change; the signal
    is the dialog result through `GWL_USERDATA`.
28. **RESOLVED — the campaign list and mission start (`1966cc1` + `4b7ef9b`).**
    Shipped `BATTLE.INI` (`[Battles] 1=GDI1, 2=NOD1`) feeds listbox 1109; the
    model implements the full listbox/trackbar layer, `FindFirstFileA` host
    enumeration and a read-only `TS_DATA` root. OK selects GDI, `GDI1A.MAP` is
    read from MAPS01.MIX, and the GDI intro movie plays (2.1B stop
    `0x0067dc4f`, `pages-used 9,029/32,768`, 194,131 non-black frame).
29. **RESOLVED — the page table and the heap.** The table grows (2,097,152-page
    cap) with stable page identity; the heap coalesces and reallocs in place.
    At 4B the mission load uses 12,809 pages instead of 377,796, 95.5% of
    allocations reused.
30. **RESOLVED — the interpreter is 12-14x faster (`9243e93` … `6421f30`).**
    ~400M instructions/s with byte-identical machine state; 4B in 11.7 s, 100B
    in 243.9 s. `test_machine_perf` pins the hot path.
31. **RESOLVED — the 479B wild call is diagnosed (`6a31165`).** The first wild
    call is deterministic at step 478,300,935,316: `call [edx+0x2c]` at
    `0x005fa96b` → `0x46000062`, EDX=`0x6a5dfe` = TerrainClass vtable `0x6a5dfc`
    + 2. `FUN_005a34e0`'s cell-seen bitmap setter wrote 34,062 B before its
    bitmap (index `67*(-4068)+65 = -272,491`) at `0x005a3558`, corrupting the
    vptr. The page refusal is the 32-bit process's ~4.2 GB address space, not a
    table cap; the flood's termination varies between runs, the game state does
    not. This was the pre-fix state; the flood no longer reproduces after
    `e5d7039`, and the 500B run reaches the tactical HUD.
32. **RESOLVED — the visible rect's coordinate space (`e5d7039`).** The x87
    emitter rendered D8 one-register forms as pop forms, so `fdiv st(1)`
    computed `ST(1)=ST(1)/ST(0)`; the tactical zoom became 108 instead of 1.125,
    producing the bogus `2125x4176` rect and the OOB vptr write. 901 non-pop
    one-operand x87 instructions were affected. After: scale 1.125, map surface
    99x108, visible rect `35,28,23,43`, vptr intact, 0 OOB stores.
33. **RESOLVED — the tactical HUD renders (500B).** Budget at 500,000,000,001,
    EIP `0x00543669`, 0 unmapped, pages 12,637/32,768, coverage 158,667/9,607,
    18,691,958 blits, 46,753,350 locks; `re/out/frame_500b.png` (87,794
    non-black) shows the HUD.
34. **OPEN (new) — the map viewport is black.** Only the HUD renders; the
    mission map does not. Trace the terrain/map blits (destination surfaces,
    primary map rectangle) with per-surface counters and dumps; do not paint the
    map by hand. **The current blocker.**
35. **OPEN (new) — the VQA frame-wait is stub-bound.** ~68M `PeekMessageA` calls
    in 600 s; ESC is the practical route to the mission load meanwhile.
36. **OPEN (new) — shutdown corrupts ESP, dormant.** WinMain returned with ESP
    `0x11` before the MIX-read fix; the current run never reaches shutdown.

---

## 6. If you only do three things

1. Build the Wine oracle. **Done** (`tools/wine_env.sh` reaches the retail main
   menu; `tools/oracle.py` reads its registers). It makes every other claim
   checkable instead of arguable.
2. Give the model a file system. **Done** (`Win32Stubs.h` real file I/O over a
   host root, plus `tools/mix.py` decoding both index variants). The remaining
   step is routing `MixFileClass` through the real reader.
3. Say which product you are building. If it is *a playable game*, use Wine and
   stop. If it is *our own code running the game*, ROADMAP P2 and P3 are not on
   the path and should be demoted to "documentation, later". **Still open, and
   still the most expensive ambiguity in the repository.**
