# Tiberian Sun Reverse Engineering — Shared Context

## Target
`re/bin/GAME.EXE` — Command & Conquer: Tiberian Sun (Westwood Studios, 1999).
- SHA256 `2941b01998da0cd2bb0b7db30125d4130e3876a74d6cc5588b630c6d5fac5a12`
- PE32 i386, MSVC 6.0, ImageBase 0x400000, EntryPoint 0x6854e3
- Sections: .text 0x401000 (2.7MB), .rdata 0x698000, .data 0x6bb000, .rsrc 0x837000
- Original source path embedded in binary: `D:\Projects\TSun\Code\`
- Stripped of symbols, but full MSVC RTTI is present.

## Recovered artifacts (in `re/out/`)

Counts are approximate and drift as the tools improve; run
`python3 tools/harness.py metrics --no-append` for the current figures. The
figures in this file were re-measured on 2026-10-17.

- `classes.json` — RTTI classes, inheritance edges, object locators, vtables
- `hierarchy.txt` — human-readable class hierarchy with base-class displacements
- `vtables.txt` — vtable VA -> class binding
- `functions.json` — recovered functions with full instruction listings,
  callees and string references
- `class_manifest.json` — per-class vtable, constructor and base set
- `members.json` — member offsets recovered from constructor store sequences
- `signatures.json` — calling conventions and arity
- `ini_keys.json` — strings referenced by code, including `rules.ini` field names
## Tools (in `re/tools/`)

The full pipeline is `./tools/pipeline.sh`. The main tools:

- `rtti.py` — RTTI recovery
- `demangle.py` — MSVC RTTI name demangler
- `disasm.py` — function boundary + call graph recovery
- `gen_manifest.py` — per-class vtable, constructor and base set
- `gen_members.py` / `name_members.py` — member offsets, and names from INI keys
- `gen_signatures.py` / `name_virtuals.py` — calling conventions and virtual names
- `gen_headers.py` — header emission
- `gen_code.py` — the canonical transcription of every recovered function into
  `src/generated/code/`, against the machine model in `src/handwritten/Machine.h`
- `gen_bodies.py` — the declaration-shaped forwarders the headers require
- `emit_x86.py` — the operand grammar and per-mnemonic rendering both use
- `verify.py` — the **9-check** fail-closed gate, compiled at the target ABI
  (`-m32`); check 9 makes the compiler confirm recovered member offsets with
  `offsetof`
- `verify_members_binary.py` — re-derives member offsets from the binary's own
  code (4,285 of 4,289 confirmed, 99.9%)
- `link_test.py` — links all 492 translation units into a program and runs it;
  measured **OK, exit 0**, message loop reached, 0 unmapped
- `test_tools.py` — 101 unit tests for the tools themselves, including
  `test_machine_perf`, which pins the interpreter hot path (40M-iteration
  workload, wide-vs-byte checksum `88188416`, page straddles, a 20M/s floor, a
  cached-path ratio of 1.73-1.78, and the `-O2`/`-fno-pie` build flags), and
  the x87-form tests that caught the D8 one-register emitter bug
- `test_runtime.py` — 110 checks for the rendering/input layers (`Rendering.h`,
  `Input.h`), mutation-proven; includes a standalone frame selftest
- `harness.py` — measures real state (`metrics`) and proves reproducibility
  (`determinism`)
- `gen_win32_stubs.py` — generates `Win32Stubs.h` from the PE import directory
  (341 imports across 13 DLLs; the stub table has 342 rows because it also
  carries `COMCTL32!DllGetVersion`, reached through `GetProcAddress`). It
  refuses to write when a `TS_RET` body's argument count disagrees with `NARGS`;
  `--evidence` reports per-ordinal agreement.
- `boot.py` — builds and runs the image against the model. `TS_ROOT` selects the
  ordinary file root; **`TS_CD`** models `D:` (`DRIVE_CDROM`, label `TS1`);
  `--frame` dumps the primary surface and `--input-script` selects a timeline
  (the default timeline is built in; `TS_INPUT_SCRIPT=` disables it). Measured
  with `TS_CD`: 20M stops at `0x00488800` (55,774 / 4,764); the default script at
  2.1B clicks New Campaign, the menu exits with result 1, `RT_DIALOG` 148
  "Select Campaign:" is created at step 1,688,503,557, its listbox is populated
  from the shipped `BATTLE.INI` `[Battles]` (`1=GDI1, 2=NOD1`), GDI1 is selected
  and OK starts the mission: the game reads `GDI1A.MAP` from `MAPS01.MIX` and
  plays the GDI intro. The stop is `0x0067dc4f` (LCW decode during intro
  playback; `GetMessage=17`, `PeekMessage=13,565,102`, `DispatchMessage=17`;
  84,130 / 6,431; pages used **9,029 / 32,768**). The intro frame is
  `re/out/mission.png` (640x480, 194,131 non-black, md5
  `ccdb46d448a90e487e513bc1daf2ff8b`). The ESC-skip script
  (`tools/input_scripts/esc_skip.txt`) skips the movie and loads the mission at
  **~400M instructions/s** (12-14x the old rate): 4B in 11.7 s, 20B in 50.2 s,
  100B in 243.9 s, byte-identical state. At 4B the stop is `0x0055b0bb` (budget)
  with 12,809 pages and the GDI loading screen (290,399 non-black, md5
  `6cb0e1fff05f9250adda298e644f1266` = `re/out/render_2_6b.png`); at 100B it is
  on vector 7/22 with 160,070 entries. At 500B it stops at the budget after
  500,000,000,001 instructions at `0x00543669`, with 0 unmapped calls, pages
  12,637/32,768, coverage 158,667/9,607, 18,691,958 blits and 46,753,350 locks,
  and renders the **tactical HUD** (`re/out/frame_500b.png`, 87,794 non-black).
  At 6B, `TS_OBJECTS` is **n=0, array=NULL** and `TS_MAPCELLS` shows **1,681
  cells allocated, 12 tile entries, 0 revealed, 0 fogged** (heights decode);
  `--surfaces` gives primary 87,794 non-black (HUD), map 472x480 non-black 7,080
  (rows 0-22), radar 99x108 **0**; `TS_EXEC_CHECK` shows all 11 tactical passes
  executing and all 5 reveal/sight updates not. An independent 500B reproduction
  was watchdog-stopped at 372,912,291,926 instructions but matched pages,
  coverage, 0 unmapped and a byte-identical frame. The Exit Game script reaches
  `ExitProcess(0)` at `0x00684529` after 1,707,666,895 instructions
  (86,400 / 6,654). CD read (`D:\TS1.dsk`, `EXPAND99..00`); only `MessageBoxA`
  is the DirectSound warning at `0x484456` (`type=0x30`); **0 unmapped calls, 0
  stack imbalances** at 20M/2.1B/4B/100B; `--no-build --strict` exits **0**.
  `re/out/frame.png` is the rendered main menu (640x480, md5 `0fdf2636…`,
  254,610 / 307,200 non-black).
- `wine_run.sh` / `wine_env.sh` — run the **retail** binary under Wine.
  `wine_env.sh` supplies the CD (`d:`, label `TS1`, `TS1.DSK`) and a
  640x480x16 screen; measured, the retail game reaches its **main menu**
  (screenshot md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s).
- `oracle.py` — attach gdb to the retail binary, parse its registers and stack,
  and `--compare` against the model at the same address
- `oracle_blowfish.py` / `wine_blowfish_trace.sh` — the tooling behind
  `out/blowfish_oracle.md`, the measured retail spec for `blowfish.dll`. It
  establishes that the registered object is a **real Blowfish cipher the game
  uses**: 9 `SetKey(56,key)` and 1,586 `Decrypt(8,in,out)` calls during startup,
  matching nine shipped MIX indexes. The model **implements standard
  Blowfish-ECB** (`345aae7`), proven by four known-answer vectors and a
  byte-identical `tools/mix.py` cross-check on the shipped TIBSUN.MIX index
  (`2ce3171bdbecb40e` → `110090d97904849a`, count 17, body 75,094,416).

**Current runtime blocker** (queued in `todo.md`): **scenario object creation
produces zero objects**. The model renders and selects the main menu (the six
`BS_OWNERDRAW` buttons have real rectangles; press/release → `BN_CLICKED`
reaches the game's own hit-test), New Campaign opens `RT_DIALOG` 148 at step
1,688,503,557, the listbox is populated from the shipped `BATTLE.INI`
`[Battles]`, GDI1 is selected, and the game reads `GDI1A.MAP` from `MAPS01.MIX`
and plays the GDI intro. The 479B out-of-bounds write was the **x87 emitter**
(D8 one-register forms rendered as pop forms; 901 instructions), fixed in
`e5d7039`; after the fix the tactical zoom is 1.125, the map surface 99x108, the
visible rect `35,28,23,43`, the vptr `0x006a5dfc`, and there are 0 OOB stores. A
500B run renders the **tactical HUD** (`re/out/frame_500b.png`, 87,794
non-black: Options, credits 5000, GDI radar eagle, build tabs/grid) with 0
unmapped calls, 12,637 pages, 18,691,958 blits and 46,753,350 locks. The black
map is **not** a renderer defect: the map surface is blitted to the primary every
frame and every tactical pass executes, but `TS_OBJECTS` is **n=0** and
`TS_MAPCELLS` shows 1,681 cells allocated with 12 tile entries and **0
revealed**, so the shroud paints the viewport black. `FUN_0063b470` (`[Units]`)
and `FUN_0063b8f0` (structures) ran, but every entry was rejected (owner-house
`FUN_004c7470` → -1, type `FUN_0063e650` → -1, `FUN_00630cc0` null, or placement
`0xc4` → 0). A lane is tracing that path; do not inject objects by hand. **The
mission map and units do not render.** The VQA frame-wait path is also
stub-bound, so movie pacing is not retail pacing. The nested MIX (`0xa8548fd9`,
317 entries) is mounted and read; the button art is not painted (no `WM_PAINT`
to children; retail does not draw it either). Rendering, input, the movie path,
the dialog layer, CD path, narrow-width flags, MIX index read, Blowfish cipher,
menu selection, the campaign list, the mission start and the x87 fix are done;
the interpreter is 12-14x faster; the shutdown ESP corruption is dormant.

**Measure, do not trust prose.** `python3 tools/harness.py metrics --no-append`
reports the real state; `python3 tools/verify.py src/include build/shim` is the
gate.
## Key facts for reconstruction
1. **RTTI gives exact class names and inheritance.** Use `hierarchy.txt`.
2. **INI keys are the original struct field names.** e.g. `TiberiumChainReaction`,
   `HalfDamageSmokeLocation1`, `Capturable`, `Unsellable`, `CustomRotor`.
   These come from `rules.ini` parsing code and map to `TechnoTypeClass` /
   `BuildingTypeClass` / `UnitTypeClass` members.
3. **Vtable layout gives virtual function order.** Each vtable entry is a
   function VA in `functions.json`.
4. **MSVC 6 calling convention**: `__thiscall` for member functions (this in ECX),
   `__stdcall` for API, `__cdecl` for varargs.
5. **Known original module names** from embedded strings:
   `Connect.CPP`, `DSAudio.Cpp`, `Queue.CPP`, `wwmous.cpp`.

## Class hierarchy anchors (verified from RTTI)
```
AbstractClass
  -> AbstractTypeClass
       -> ObjectTypeClass
            -> TechnoTypeClass
                 -> BuildingTypeClass, InfantryTypeClass, UnitTypeClass,
                    AircraftTypeClass, VesselTypeClass
       -> TerrainTypeClass, SmudgeTypeClass, OverlayTypeClass, ...
  -> ObjectClass
       -> TechnoClass
            -> BuildingClass, InfantryClass, UnitClass, AircraftClass, VesselClass
       -> TerrainClass, SmudgeClass, OverlayClass, AnimClass, BulletClass
```
Locomotion classes: DriveLocomotionClass, FlyLocomotionClass, WalkLocomotionClass,
JumpjetLocomotionClass, HoverLocomotionClass, MechLocomotionClass,
TeleportLocomotionClass, TunnelLocomotionClass, DropPodLocomotionClass.

## The machine model

`src/handwritten/Machine.h` models the x86 machine the recovered code ran on:
registers with sub-register views, flag semantics for every operation emitted,
lazily-paged memory, the stack, the x87 register stack, string ops, segment
registers and port I/O. Transcribed bodies read their arguments from this model,
**not** from their C++ parameter lists — the parameter list is the reconstructed
interface, the model is where the values actually are. Hand-written callers reach
recovered code through `ts::ts_invoke`; generated forwarders call the
transcription by name and read the result from `ts::ret_eax()`.

Two more hand-written headers are **our model, not recovered data**:
`Rendering.h` (`ts::rnd`: per-surface RGB565 memory, pitch, `Lock`/`Unlock`,
`Blt`/`BltFast`/`Fill`/`Flip`, descriptors, palette, PNG/PPM frame dump) and
`Input.h` (`ts::input`: scripted key/mouse state, cursor, synthesized messages).
`Win32Stubs.h` routes the surface and input calls through them, which is what
lets the model render the main menu and take scripted input.

## Output conventions
- Reconstructed headers go in `re/src/include/`
- Generated code goes in `re/src/generated/` — never hand-edit it
- Reconstructed sources go in `re/src/`
- Every reconstructed declaration must cite its evidence (RTTI name, vtable VA,
  INI key, or string VA) in a comment.
- Do NOT invent members that have no evidence. Mark uncertain ones `/* ? */`.
