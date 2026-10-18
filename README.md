# Tiberian Sun — Disassembly to Source

A reverse-engineering project that takes the retail PC release of
**Command & Conquer: Tiberian Sun** (Westwood Studios, 1999) and reconstructs
it as compilable, documented C++ source — and, increasingly, executes the
original program's own instruction stream against a machine model.

## Progress screenshot

![The reconstructed model rendering the retail Tiberian Sun tactical HUD and the revealed base area](re/out/frame_500b_objects.png)

*The model's own 640x480 render: the in-game tactical HUD (Options, credits, the
GDI radar eagle, build tabs and the build grid) plus the revealed base area at
the bottom of the map viewport — terrain, the GDI construction yard and a unit
(93,404 non-black pixels). The rest of the viewport is shroud: 34 of 1,681 cells
are seen, which is the next thing to fix. The main menu
(`re/out/frame.png`), the GDI intro (`re/out/mission.png`), the mission loading
screen (`re/out/render_2_6b.png`) and the earlier HUD-only frame
(`re/out/frame_500b.png`) are committed alongside it.*

## Status

The reconstructed source **compiles, links and runs**. The binary is recovered,
its whole instruction stream is transcribed, the compiler confirms the recovered
memory layout at the target 32-bit ABI, and the original entry point executes
against a model of the x86 machine it was written for.

It is **not yet a playable game**, and the source is not yet source-shaped:

- The model boots the image at its real VAs, resolves all 341 PE import-directory
  thunks, reaches the game's own **message loop**, decrypts the TIBSUN.MIX index
  with a real Blowfish-ECB implementation (all 27 index blocks, 17/17 records
  matching `tools/mix.py`), reads the CD (`TS_CD` models `D:`), builds its real
  LANGUAGE.DLL dialogs, plays past the intro, and **renders the real main
  menu**. The committed `re/out/frame.png` is 640x480, md5
  `0fdf2636c2643ce2510ae67fb9ca9e26`, with 254,610 / 307,200 non-black pixels;
  against the retail screenshot, 102,836 pixels differ (33.48%) but the mean
  absolute channel difference is **1.06/255** on the pixels that differ, and only
  506 pixels differ by more than 8 (the retail version text). The run has **0
  unmapped calls and 0 stack imbalances**, and `boot.py --strict` exits 0.
- **The menu is selectable, and the game leaves it.** `DLGTEMPLATEEX` is parsed;
  the six `BS_OWNERDRAW` buttons have real rectangles and the click goes through
  the game's own hit-test and `WM_COMMAND` path. New Campaign exits the menu with
  result 1 and opens `RT_DIALOG` **148, "Select Campaign:"** at step
  **1,688,503,557**; Exit Game reaches **`ExitProcess(0)`** after 1,707,666,895
  instructions. The nested MIX (`0xa8548fd9`, 317 entries) is mounted and read.
- **The campaign list works and the GDI mission starts.** Dialog 148's listbox is
  populated from the shipped `BATTLE.INI` `[Battles]` (`1=GDI1, 2=NOD1`), the
  listbox/trackbar message layer is implemented, GDI1 is selected, and the game
  reads `GDI1A.MAP` from `MAPS01.MIX` and **plays the GDI intro** (frame
  `re/out/mission.png`, 640x480, 194,131 non-black, md5
  `ccdb46d448a90e487e513bc1daf2ff8b`). The button art is not painted (no
  `WM_PAINT` to children; retail does not draw it either).
- **The interpreter is 12-14x faster (~400M instructions/s) with byte-identical
  machine state.** Measured on the mission-load run: 11.7 s at 4B (341.9M/s),
  50.2 s at 20B (398.4M/s), 243.9 s at 100B (410.0M/s); the old rate was ~28.6M/s.
  The changes are semantics-preserving (`-O2 -fno-pie`, a positive-hit page
  cache, wide single-page loads/stores, an out-of-line cold path, inlined
  page/flag helpers, a cached import-name lookup), and `test_machine_perf` pins
  the hot path.
- **The tactical HUD renders — the first in-game frame.** The 479B out-of-bounds
  write was the **x87 emitter**: `tools/emit_x86.py` rendered the D8
  one-register forms as pop forms, so `fdiv st(1)` wrote `ST(1)=ST(1)/ST(0)`
  instead of `ST(0)=ST(0)/ST(1)` (901 non-pop one-operand x87 instructions
  affected; the 692 pop forms were right). `FUN_005a1bd0`'s tactical zoom used
  it and produced scale **108** instead of **1.125**, so `FUN_005a4080` built the
  bogus `2125x4176` visible rect and `FUN_005a34e0` wrote 34,062 bytes before its
  bitmap, on a TerrainClass vptr. **Fixed in `e5d7039`** (emitter + 951
  regenerated x87 lines + tests + a clean-tree `gen_headers` unblock). After the
  fix: scale **1.125** (`0x3f900000`), map surface **99x108**, visible rect
  **35, 28, 23, 43**, vptr **`0x006a5dfc`**, **0** out-of-bounds stores.
- **The 500B run renders the tactical HUD.** It stops at the budget after
  **500,000,000,001 instructions** at `0x00543669` (a 3x4 matrix·vector transform
  in the renderer), depth 100, pages **12,637 / 32,768** (refused 0), **0
  unmapped calls**, coverage **158,667 / 9,607**, message loop dispatching
  (`GetMessage=19`, `DispatchMessage=19`, `PeekMessage=23,117,313`), 8 surfaces
  with **18,691,958 blits and 46,753,350 locks**. The frame
  `re/out/frame_500b.png` (640x480, md5 `4fc1cec7…`, **87,794 non-black**) shows
  Options, credits 5000, the GDI radar eagle, and the build tabs/grid. An
  independent 500B-budget reproduction was stopped by its watchdog at
  372,912,291,926 instructions under host contention but matched the pages,
  coverage, 0 unmapped calls and a byte-identical frame.
- **The scenario world is created, and the map viewport now shows the base.**
  Scenario object creation rejected every entry because two stubs lied.
  `TlsGetValue` always returned 0 and `TlsSetValue` stored nothing, so the CRT's
  `_getptd` block never persisted and `FUN_006837ea` (strtok) returned only the
  first token of each `owner,type,...` line; `FUN_0063e650(NULL)` was -1 and the
  entry was dropped. `CLSIDFromString` also wrote 16 zero bytes and returned
  S_OK, so every `Locomotor={...}` became CLSID_NULL, `CoCreateInstance` handed
  back a generic 3-slot IUnknown, and its `call [vtable+0xc]` at `0x630e2d`
  called 0 — leaking its arguments and walking ESP over `FUN_0063b470`'s saved
  EBP, after which mission init called the infantry and structures readers with
  `this=0x9a80`. Both are fixed (real TLS slots; real GUID parsing plus the
  game's own `IClassFactory` from `CoRegisterClassObject`). The world registries
  now hold **4 ObjectClass, 21 infantry, 36 buildings, 7 houses**;
  `TS_MAPCELLS` reports **34 of 1,681 cells seen**. The remaining black is
  **shroud**, not a missing render: the revealed patch is clipped to the bottom
  edge of the 472x480 viewport instead of centred on the base, so the next work
  is the shroud-reveal radius (cell `+0xa4` seen/fog bits) and the tactical
  camera origin/visible-rect computation, not the renderer. The VQA frame-wait
  path is also stub-bound, so movie pacing is not retail pacing.
- **The GDI loading screen renders.** At a 4B budget the ESC-skip run stops at
  `0x0055b0bb` with 12,809 pages, 12 blits and 769 locks; the frame is 640x480,
  290,399 non-black, md5 `6cb0e1fff05f9250adda298e644f1266`, byte-identical to
  the committed `re/out/render_2_6b.png`.
- The **retail** game reaches its **main menu** under Wine on this host; that is
  the project's external oracle.
- Every function body is generated machine transcription, one instruction per
  line, not the C++ the original authors wrote. The editable surface is the
  hand-written headers.

| Measure | Value |
|---|---|
| classes / vtable slots | 749 / 9,810 (all slots seeded) |
| functions recovered | 43,557 spans, 95.86% of `.text`; 99.99% attributed as code or data; 213 static switch tables / 6,388 B |
| instruction stream transcribed | 45,947 functions (43,557 starts + 2,390 tail entries), 912,879 instruction lines, 0 unmodelled |
| forwarders / link units | 6,962 across 310 units; the whole tree links as 492 TUs |
| member offsets recovered | 4,289 (4,285 confirmed against the binary, 99.9%; 4 refuted) |
| named members | 291 across 20 classes |
| virtuals named | 5,557 of 9,810 slot pairs (56.6%); 917 distinct names (281 semantic) |
| headers | 266 total, 265 compile standalone at `-m32` |
| verification gate | 9 checks, fail-closed, passing; check 9 confirms 3,073/3,073 offsets |
| tool tests | 101 checks, several mutation-proven; `test_runtime.py` 110 more (rendering/input) |
| interpreter | ~400M instructions/s (12-14x the old ~28.6M/s), byte-identical state; 4B in 11.7 s, 100B in 243.9 s |
| boot (`TS_CD`) | message loop reached and dispatching; real Blowfish-ECB; MIX/CD path correct; real LANGUAGE.DLL dialogs; **renders and selects the main menu**; campaign list from `BATTLE.INI`; **starts the GDI mission and plays its intro**; 20M stop `0x00488800` (55,774 / 4,764), 2.1B stop `0x0067dc4f` (84,130 / 6,431; pages 9,029 / 32,768), 4B ESC-skip stop `0x0055b0bb` (139,250 / 8,682; pages 12,809), 20B ESC-skip stop `0x0041398c`, pages 13,698, coverage 230,640 / 13,415, 57,074 blits / 254,041 locks, **500B ESC-skip: budget at `0x0042322c`, 0 unmapped, coverage 230,658 / 13,417, 1,942,509 blits / 5,971,443 locks**, Exit Game reaches `ExitProcess(0)` after 1,707,666,895 instructions (86,400 / 6,654); 0 unmapped at 20M/2.1B/4B/20B/500B; the world is populated (4 ObjectClass / 21 infantry / 36 buildings / 7 houses) and 34 of 1,681 cells are seen, so the remaining black viewport is shroud, not a missing render |
| frame | `re/out/frame_500b_objects.png`: 640x480, md5 `e788f40b…`, 93,404 non-black, the tactical HUD plus the revealed base area (terrain, GDI construction yard, a unit). `re/out/frame_500b.png` (HUD only, md5 `4fc1cec7…`, 87,794 non-black), `re/out/frame.png` (main menu, md5 `0fdf2636…`, 254,610 non-black; 1.06/255 mean channel difference from retail on differing pixels), `re/out/mission.png` (GDI intro title, 194,131 non-black) and `re/out/render_2_6b.png` (loading screen, 290,399 non-black, md5 `6cb0e1ff…`) |

**Measure, don't trust prose.** `python3 tools/harness.py metrics` prints the
real state; these figures drift as the tools improve. `AGENTS.md` records the
ground rules and the honest limits of each number.

## Verifying

```bash
cd re
./tools/pipeline.sh                            # regenerate everything, then verify
python3 tools/verify.py src/include build/shim # the 9-check gate (about 6 s)
python3 tools/verify_members_binary.py         # re-derives offsets from the binary
python3 tools/link_test.py                     # links 492 TUs and runs the entry point
python3 tools/test_tools.py                    # tests the tools themselves
python3 tools/test_runtime.py                  # rendering/input layer tests
python3 tools/harness.py metrics --no-append   # measured state
python3 tools/harness.py determinism --run     # two clean runs, byte-diffed
```

The gate is fail-closed: a missing artifact exits non-zero naming what is
missing, rather than running a subset and reporting success. Check 4 reads the
binary, check 9 makes the compiler adjudicate layout, and
`verify_members_binary.py` is a separate binary-anchored gate; the others
compare the tree against recovered data, so they can agree with each other while
both are wrong. That distinction matters when reading `VERIFICATION PASSED`.

## Booting the original program

```bash
cd re
TS_CD=../work/cd1 python3 tools/boot.py          # model D: as a TS1 CD-ROM
python3 tools/boot.py --strict                   # exit non-zero if a garbage target remains
```

`TS_ROOT` selects the host root for ordinary files; `TS_CD` maps `D:` to a host
directory as a `DRIVE_CDROM` labelled `TS1`, which is what lets the game find
`D:\TS1.dsk` and its MIX files. The image maps at `0x400000` (no `.reloc`), all
341 import-directory thunks resolve to stubs, and the original entry point
`0x6854e3` runs. The current measured stop:

```
message loop: reached  (GetMessage=1 PeekMessage=5 DispatchMessage=1 CreateWindowExA=1)
stop: instruction budget exhausted at 0x00488800 after 20,000,001 instructions
why:  the run is in the intro/menu path (no fault, no exit)
coverage: 55,774 distinct instructions; 4,764 of 45,947 recovered functions
unmapped calls: 0 (0 inside .text); stack imbalances: 0
```

At `--budget 2100000000 --timeout 600` the default scripted input clicks New
Campaign: the menu exits with result 1, `RT_DIALOG` **148, "Select Campaign:"**
is created at step **1,688,503,557**, its listbox is populated from the shipped
`BATTLE.INI` `[Battles]` (`1=GDI1, 2=NOD1`), GDI1 is selected, OK starts the
mission, and the game reads `GDI1A.MAP` from `MAPS01.MIX` and plays the GDI
intro. The stop is `0x0067dc4f` (LCW decode during playback) at the
2.1-billion-instruction budget (`GetMessage=17`, `PeekMessage=13,565,102`,
`DispatchMessage=17`; 84,130 / 6,431; pages used **9,029 / 32,768**). The intro
frame captured to `re/out/mission.png` is 640x480, 194,131 non-black, md5
`ccdb46d448a90e487e513bc1daf2ff8b`. A script that clicks Exit Game instead
reaches **`ExitProcess(0)`** at `0x00684529` after **1,707,666,895** instructions
(86,400 / 6,654). `--frame out/frame.png` dumps the rendered menu; the committed
frame is 640x480, md5 `0fdf2636c2643ce2510ae67fb9ca9e26`, with 254,610 / 307,200
non-black pixels, and differs from the retail screenshot by a mean channel
difference of 1.06/255 on the pixels that differ.

With `--input-script tools/input_scripts/esc_skip.txt` the intro is skipped and
the mission load runs. At `--budget 4000000000` it stops at `0x0055b0bb` after
4,000,000,001 instructions (11.7 s wall) with 12,809 pages, 12 blits, 769 locks
and a 640x480 loading screen (290,399 non-black, md5
`6cb0e1fff05f9250adda298e644f1266` = `re/out/render_2_6b.png`). At 100B it is
still on the loader's vector 7/22 (160,070 entries) after 243.9 s. At
`--budget 500000000000` it reaches the **tactical HUD**: budget at
500,000,000,001 instructions, EIP `0x00543669` (a 3x4 matrix·vector transform in
the renderer), depth 100, pages **12,637 / 32,768** (refused 0), **0 unmapped
calls**, coverage **158,667 / 9,607**, `GetMessage=19`, `DispatchMessage=19`,
`PeekMessage=23,117,313`, 8 surfaces with **18,691,958 blits and 46,753,350
locks**. `--frame` dumps `re/out/frame_500b.png` (640x480, md5 `4fc1cec7…`,
87,794 non-black): Options, credits 5000, the GDI radar eagle, and the build
tabs/grid. **The map viewport is still black** — the world is empty and fully
shrouded (see below); the HUD is the furthest rendered frame.

The 479B out-of-bounds write that preceded this was the **x87 emitter**:
`tools/emit_x86.py` rendered the D8 one-register forms as pop forms, so
`fdiv st(1)` wrote `ST(1)=ST(1)/ST(0)` instead of `ST(0)=ST(0)/ST(1)` (901
non-pop one-operand x87 instructions affected). `FUN_005a1bd0`'s tactical zoom
used it and produced scale **108** instead of **1.125**, so `FUN_005a4080` built
the bogus `2125x4176` visible rect and `FUN_005a34e0` wrote 34,062 bytes before
its bitmap, on a TerrainClass vptr. Fixed in `e5d7039` (emitter + 951
regenerated x87 lines + tests + a clean-tree `gen_headers` unblock). After the
fix: scale 1.125 (`0x3f900000`), map surface 99x108, visible rect `35,28,23,43`,
vptr `0x006a5dfc`, **0** out-of-bounds stores. The earlier 1.2e12-budget flood
(7,771,444 unmapped calls, 1,024,211 / 2,097,152 pages, black frame) is
historical and does not reproduce.

Real Blowfish-ECB is implemented and proven against `tools/mix.py` (the shipped
TIBSUN.MIX first index block decrypts byte-identically), the MIX index read is
fixed (27/27 blocks, 17/17 records), the dialog layer reads LANGUAGE.DLL's own
templates, and narrow-width flags are derived from the operand width. The menu
renders and is selectable through the game's own hit-test and `WM_COMMAND` path;
the nested MIX (`0xa8548fd9`, 317 entries) is mounted and read. The campaign
list is populated from the shipped `BATTLE.INI`, GDI1 is selected, the game
starts the mission and plays the GDI intro, and the 500B run renders the
**tactical HUD**. **The current blocker is scenario object creation**: the black
map is not a renderer defect. The map surface is blitted to the primary every
frame and every tactical pass executes, but `TS_OBJECTS` is **n=0** and
`TS_MAPCELLS` shows 1,681 cells allocated with 12 tile entries and **0
revealed**, so the shroud paints the viewport black. `FUN_0063b470` (`[Units]`)
and `FUN_0063b8f0` (structures) ran, but every entry was rejected (owner-house
`FUN_004c7470` → -1, type `FUN_0063e650` → -1, `FUN_00630cc0` null, or placement
`0xc4` → 0). A lane is tracing that path; do not inject objects by hand. The
479B out-of-bounds write is fixed (x87 D8 one-register forms; 901 instructions),
and the earlier flood no longer occurs (0 unmapped calls at 500B). The VQA
frame-wait path is also stub-bound, so movie pacing is not retail pacing; the
button art is not painted (no `WM_PAINT` to children; retail does not draw it
either). The shutdown ESP defect is dormant.

The model has diagnostics for this work: `TS_TRACE=0xADDR`, `TS_PEEK=0xADDR`,
`TS_SCAN=start,count`, `TS_WATCH=0xADDR`, `TS_WATCH_RING`, `TS_STOP_UNMAPPED=1`,
`TS_HEAP_AT`, `TS_FOLLOW`, `TS_FUNC_ENTERED`, `--surfaces`, `--surface-dump`,
`TS_BLITLOG`, `TS_EXEC_CHECK`, `TS_OBJECTS`, `TS_MAPCELLS`, `TS_FILE_TRACE=1`,
plus `bad_calls`, `stack_imbalances` and a per-callee ESP tracker. "What is ESP
here?" is answered by running the machine, not by counting pushes by hand.

## The retail oracle

```bash
cd re
./tools/wine_env.sh              # retail GAME.EXE -> main menu, screenshot
python3 tools/oracle.py 0xADDR   # real registers at an address, for diffing
python3 tools/oracle_blowfish.py # the Blowfish registration/decrypt path
```

`wine_env.sh` starts Xvfb at `640x480x16` (the mode the game asks DirectDraw
for) and maps a `DRIVE_CDROM` `d:` with volume label `TS1` holding `TS1.DSK`.
With that environment the retail game plays its CD intro movies and reaches the
GDI/Nod main menu (verified screenshot, md5 `58cb9c0b…`, still running after
60 s). This is the only oracle that is not the project checking itself; use it
before believing the model.

## Running against real game data

`realdata/` holds the shipped control files extracted from the retail archive
(`rules.ini`, `art.ini`, `ai.ini`, `sound.ini`, `theme.ini`, `BATTLE.INI`,
`MISSION.INI` — inventory in [realdata/README.md](realdata/README.md)). The
reconstructed field names are driven with them end to end:

```bash
cd re
python3 tools/e2e_rules.py ../realdata/*.ini ../realdata/*.INI
```

Measured: 142 recovered member names; 121 are set as `Name=value` in the shipped
data, 10 more appear only in the files' comment headers, and 11 remain
unconfirmed — 131 (92.3%) confirmed by one class or the other. The two evidence
classes are reported separately because a comment proves a name exists, not that
a value is read. `--typos` reports names whose spelling differs from the shipped
documentation (the binary's own `Inaccuate` is the known case).

`tools/mix.py` decodes both shipped MIX index variants (the headerless TD/RA1
form and the TS/RA2 RSA + Blowfish-encrypted form); `tools/extract_realdata.py`
regenerates `realdata/` from `TIBSUN.MIX`.

## What was recovered

- **43,557** function spans, 95.86% of `.text`; the residual 110 KB is
  classified as **213 static switch tables (6,388 B)**, 102 switch index tables
  and alignment padding.
- **45,947** transcriptions covering **912,879** instructions, all modelled.
- **749** classes with exact inheritance from intact MSVC 6 RTTI, and **9,810**
  vtable slots, every one bound through the binary's own Complete Object Locator
  chain.
- **4,289** member offsets recovered from constructor stores; **4,285** confirmed
  against the binary by disassembling those constructors.
- **1,876** referenced strings, including `rules.ini` field names — the original
  struct member names, recovered verbatim.
- A working machine model (`src/handwritten/Machine.h`), a PE image mapper and
  import resolver (`Image.h`), a per-surface RGB565 rendering layer
  (`Rendering.h`), a scripted input layer (`Input.h`), and a
  Win32/DirectDraw/DirectSound/COM stub layer (`Win32Stubs.h`) faithful enough
  to run the game's startup and render its main menu.

## Layout

```
re/
  bin/GAME.EXE        target binary
  tools/              recovery, generation, verification and boot tooling
  tools/pipeline.sh   one-command regeneration
  out/                recovered data (JSON) and the Blowfish oracle report
  src/include/        reconstructed C++ headers (generated; do not hand-edit)
  src/handwritten/    evidence-backed headers, including the machine model
  src/generated/      forwarders; src/generated/code/ is the transcription
  build/              compile harness and boot driver
  REPORT.md           results and verification status
  PROGRESS.md         live status
  ROADMAP.md          what "done" means and the phase order
  STRATEGY.md         the shortest path to a working game
realdata/             shipped INI files extracted from TIBSUN.MIX
todo.md               prioritized, evidence-backed work list
```

## Method

The binary is stripped of symbols but retains complete MSVC 6 RTTI. The
`type_info` and Complete Object Locator layouts were determined empirically from
the binary rather than assumed, and a recursive MSVC name demangler was written
for the observed mangling grammar. Every declaration in the reconstructed source
cites its evidence: an RTTI descriptor address, a vtable address, a constructor
address, an INI key string, or a function address.

Uncertainty is marked explicitly (`/* ? */`, `vfunc_N`) rather than guessed at,
and known gaps are recorded rather than hidden. Two rules from this project are
worth repeating: a gate that has never failed is not a gate, and a stub that
returns a plausible wrong value is worse than one that fails loudly.

## Contributing / working on this

Read [AGENTS.md](AGENTS.md) first — ground rules, toolchain, and the technical
facts about the binary. [ROADMAP.md](re/ROADMAP.md) says what is left and in what
order; [todo.md](todo.md) is the defect ledger.

Two operational notes learned the hard way: worktree-isolated agents must
**commit** their own work or a timed-out workflow loses it, and a change to a
generator can break the tree in ways no single agent owns. Run
`./tools/pipeline.sh` before claiming a generator change works.

## Legal

Tiberian Sun and its assets are © 1999 Electronic Arts / Westwood Studios.
This repository contains only reconstructed source and derived data; the
original game binaries are included solely as the reference target.
