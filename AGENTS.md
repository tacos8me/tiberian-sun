# AGENTS.md — Tiberian Sun reverse engineering project

Guidance for agents working in this repository.

## What this project is

A full disassembly of **Command & Conquer: Tiberian Sun** (Westwood Studios,
1999, retail PC release) back into compilable C++ source.

The goal is **complete, documented, change-ready source** — not a partial
sketch. "It compiles" is not the bar; "every class is present, every claim is
traceable to binary evidence, and a human can confidently edit it" is the bar.

## Repository layout

```
ts1.bin ts2.bin ts1.cue ts2.cue   raw MODE2/2352 CD images (input, gitignored)
work/                             extracted ISO trees (derived, gitignored)
re/
  bin/GAME.EXE                    the target binary (SHA256 in REPORT.md)
  tools/                          recovery + generation tooling
  tools/pipeline.sh               one-command full regeneration
  out/                            recovered data (JSON, text)
  src/include/                    generated headers (do not hand-edit)
  src/handwritten/                hand-written, evidence-backed headers
  src/handwritten/Machine.h       the x86 machine model the transcribed code runs on
  src/handwritten/Rendering.h     per-surface RGB565 memory, Blt/Fill/Flip/Lock,
                                  and a PNG/PPM frame dump
  src/handwritten/Input.h         scripted key/mouse state and synthesized messages
  src/handwritten/Image.h         maps the PE's sections at their real VAs and
                                  writes a stub VA into every IAT slot
  src/handwritten/Win32Stubs.h    what the 341 imported Windows functions (plus
                                  the one GetProcAddress-only export) DO, so
                                  `call dword ptr [IAT]` reaches code
  build/image_selftest.cpp        load the image and resolve imports, standalone
  build/boot_driver.cpp           the boot harness behind tools/boot.py
  tools/ghidra_scripts/           Ghidra post-scripts (DecompileAt.java)
  tools/wine_run.sh               run the RETAIL binary under Wine (headless,
                                  --trace CHANNEL, --dismiss, --shot PNG,
                                  --cd DIR, --screen WxHxD)
  tools/wine_env.sh               the same with the CD and 640x480x16 defaults
                                  the game needs to reach its main menu
  tools/oracle.py                 attach gdb to the retail binary and print the
                                  real registers at an address, for diffing
                                  against the model's TS_TRACE
  STRATEGY.md                     the shortest credible path to a working game,
                                  measured and ranked. Read with ROADMAP.md
ghidra-proj/                      analysed Ghidra project (gitignored, 544 MB);
                                  recreate with tools/ghidra.sh's documented command
  src/generated/code/             the transcription: one ts::sub_XXXXXXXX(void)
                                  per recovered function VA, plus the dispatch
                                  registration unit and recovered.h
  src/generated/                  declaration-shaped forwarders, one .cpp per class
  src/generated/README.md         generated coverage figures (regenerated, not prose)
  build/                          compile harness + platform shim
  build/ts_link_test.cpp          links the whole tree and runs the entry point
  REPORT.md                       results and verification status
  PROGRESS.md                     current state and remaining work
  ROADMAP.md                      the remaining trajectory: what "done" means,
                                  how big each remaining problem really is, and
                                  the order the phases go in. READ THIS FIRST
                                  when deciding what to work on.
  CONTEXT.md                      briefing for reconstruction agents
```

`src/generated/` is 80 MB of generated C++. It is committed on purpose: it is
the artifact a reader comes for, and regenerating it takes four minutes. Never
hand-edit it — `src/generated/code/recovered.h` declares every transcription and
is generated too.

## Ground rules

1. **Evidence or it doesn't go in.** Every declaration must cite its source:
   RTTI descriptor VA, vtable VA, constructor VA, INI key string, or function
   VA. Never invent a member, name, or type.
2. **Mark uncertainty explicitly.** Use `/* ? */` for anything inferred but not
   proven. Use `vfunc_N` for virtuals whose semantics aren't established.
   Honest gaps are worth more than confident guesses.
3. **Verify against the binary, not against the previous agent's output.**
   Re-resolve vtable→class bindings through the COL chain. Re-read the bytes.
4. **Keep the build green.** `build/tu_all.cpp` must compile and the whole
   generated tree must **link** after every change. A per-file syntax check
   cannot see a forwarder naming a symbol nothing defines; the tree did not link
   for months because every check compiled one file at a time. Run
   `./tools/pipeline.sh` — it links and runs the result.
5. **Regenerate, don't hand-patch, generated files.** If a header is produced by
   a tool in `tools/`, fix the tool and re-run it. Anything you want to keep must
   live in `src/handwritten/`; the pipeline wipes `src/include/` and restores
   the hand-written set, which always wins over a generated header.
6. **The pipeline must be reproducible from a clean checkout.** Run
   `./tools/pipeline.sh` against an empty `out/` and `src/include/` before
   claiming a change works. A stage that depends on an artifact no tool
   produces is a bug.

## Toolchain

| Tool | Purpose |
|---|---|
| `tools/rtti.py` | MSVC 6 RTTI recovery → classes, vtables, COLs |
| `tools/demangle.py` | MSVC mangled-name demangler |
| `tools/disasm.py` | function boundary + call-graph recovery |
| `tools/gen_strings.py` | referenced-string / INI-key extraction |
| `tools/gen_manifest.py` | per-class vtable/ctor manifest |
| `tools/reduce_bases.py` | transitive → direct base reduction |
| `tools/gen_members.py` | data-member offset recovery from ctors |
| `tools/name_members.py` | member naming from INI-key store sites |
| `tools/gen_signatures.py` | calling convention / arity recovery |
| `tools/name_virtuals.py` | virtual naming from call-site and string evidence; a pipeline stage |
| `tools/gen_headers.py` | header emission from the manifest |
| `tools/reconcile_prelude.py` | prelude/header base reconciliation |
| `tools/make_selfcontained.py` | add missing includes per header |
| `tools/verify.py` | full verification gate (9 checks, fail-closed, compiled at the target ABI) |
| `tools/harness.py` | measure real state (`metrics`) and prove reproducibility (`determinism`) |
| `tools/gen_code.py` | the canonical transcription of every recovered function, plus the registration unit and tail entries; mutation-proven faithfulness gate (`--check`) |
| `tools/gen_bodies.py` | the declaration-shaped forwarders the reconstructed headers require, de-duplicated by function VA, gated to name a defined transcription |
| `tools/emit_x86.py` | the operand grammar and per-mnemonic rendering used by both generators; the only place that knows the disassembler's operand text |
| `tools/link_test.py` | link the whole tree into a program and run it; the only check that can see an undefined symbol |
| `tools/check_compile.py` | compile a set of generated TUs standalone and fail if any does |
| `tools/gen_win32_stubs.py` | generates `Win32Stubs.h` from the image's own import directory, so the stub table cannot drift from the **341 PE thunks** across 13 DLLs (309 by name, 32 by ordinal). The table holds **342 rows**: the 341 imports plus `COMCTL32!DllGetVersion`, which the CRT reaches through `GetProcAddress`, not the import directory. Resolution is a property of the 341 IAT slots (`Image.h`: `341 of 341 IAT slots written`); do not write "342/342 resolved". `--evidence` derives argument counts from the recovered instruction stream and reports agreement per ordinal; `--stack-check` drives the table. It **refuses to write** when a `TS_RET` body's argument count and `NARGS` disagree, so an arity defect fails the generator instead of shipping. `19ee16c` corrected `SetViewportOrgEx`/`SetWindowOrgEx` to 4 arguments; the regenerated table diff is exactly those two rows and `--stack-check` is 342/0 |
| `tools/boot.py` | one command that builds (cached), boots the image and reports it -- imports called by name, fault/stop EIP with instruction bytes, containing function and generated source line, message-loop state, **coverage** (`distinct instructions; N of 45,947 recovered functions entered`) and **unmapped-target classification** (`below 0x1000`, `.rdata`, `elsewhere`). `--strict` exits non-zero while any garbage call target remains (currently **0**; exit 0); `--json`, `--watch`, `--imports N`, `--budget N`, `--rebuild`, `--no-build`, `--frame PNG` (dump the primary surface) and `--input-script FILE` (default: the built-in timeline). `TS_ROOT` selects the host root for ordinary files; **`TS_CD`** models `D:` (a `DRIVE_CDROM` labelled `TS1`) so the game can find `D:\TS1.dsk` and its MIX files |
| `tools/ghidra.sh` | decompile the function containing each given address, from a saved Ghidra project (seconds, no re-analysis) |
| `tools/wine_run.sh` | run the **retail** `GAME.EXE` under Wine, headless: `--trace CHANNEL`, `--dismiss` the startup dialog chain, `--shot PNG` with `--shot-delay N`, `--cd DIR` (maps `d:`, labels it `TS1`, sets the drive type to cdrom) and `--screen WxHxD`. The only oracle this project has |
| `tools/wine_env.sh` | one command that runs the retail game in the environment it needs to reach its **main menu**: Xvfb at `640x480x16` and a `DRIVE_CDROM` `d:` labelled `TS1` holding `TS1.DSK`. Wraps `wine_run.sh --cd ... --screen 640x480x16 --dismiss --shot`. Measured: GDI/Nod title screen, 640x480, screenshot md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s |
| `tools/oracle.py` | attach gdb to the retail binary, break at an address, **parse `info registers` and the stack top** into ESP/EBP/EBX/EAX/ECX/EDX, and `--compare` diff them against the model at the same address (`--budget`, `--secs`, `--gdb-timeout`) |
| `tools/oracle_blowfish.py` | the measured retail spec for `blowfish.dll` → `out/blowfish_oracle.md`: vtable, object layout, the 9 `SetKey` / 1,586 `Decrypt` call counts, and the nine-index corroboration |
| `tools/wine_blowfish_trace.sh` | the Wine/gdb harness behind `oracle_blowfish.py` |
| `tools/test_tools.py` | unit tests for the load-bearing tools; every one is a regression test for a shipped defect |
| `tools/test_runtime.py` | **101** checks for the rendering and input layers (`Rendering.h`, `Input.h`), mutation-proven; includes a standalone frame selftest and the integration contract |
| `tools/verify_members_binary.py` | re-derive member offsets from the binary's own code; the only member check not trusting `out/` |
| `tools/extract_realdata.py` | extract the shipped INI control files from `TIBSUN.MIX` with disjoint byte ranges |
| `tools/e2e_rules.py` | drive the reconstructed model with real shipped data and check recovered field names against it |
| `tools/mix.py` | MIX archive reader. Both index variants decode: the headerless TD/RA1 one, and the TS/RA2 one with the RSA key block, the Blowfish-encrypted index and the SHA1 trailer. Validated by the header+index+body filling the file to the byte on all four encrypted archives on these CDs, and by TIBSUN.MIX yielding keys.ini -- the file containing the key that decrypted it |
| `tools/gen_layout_check.py` | emit `offsetof` assertions for every recovered member offset and compile them at `-m32`; the only layout check the compiler adjudicates. Gated as verify.py check 9 |

### Tools that are not pipeline stages

These are run by hand. They are not broken, but nothing regenerates their output
and no check covers it:

| Tool | What it is for |
|---|---|
| `tools/ctor.py` | member-init offsets + string refs for a chosen class; the exploratory form of `gen_members.py` |
| `tools/mapdump.py` | vtable entries + ctor offsets for the map/display classes |
| `tools/vtdump.py` | full evidence dump for the map/display/UI layer |
| `tools/gen_ini_headers.py` | INI/config + weapon/warhead/anim/bullet type headers |
| `tools/gen_schema.py` | emits `src/include/SCHEMA.md` from `tools/ini_map.json` |

**Sharp edge:** `gen_schema.py` writes `src/include/SCHEMA.md`, and
`pipeline.sh` then copies `src/handwritten/SCHEMA.md` over it. The hand-written
file wins, so running `gen_schema.py` and then the pipeline silently discards
its output. If you change `ini_map.json`, run `gen_schema.py` *after* the
pipeline, or move the result into `src/handwritten/`.

Run order is encoded in `tools/pipeline.sh`. Do not run stages by hand out of
order; `out/` artifacts feed forward. The stages are: 0 clean tree, 0b tool unit
tests, 1 RTTI, 2 disassembly, 2b machine-code transcription, 3 strings,
4 class manifest, 5 direct bases, 6 members, 7 member naming, 8 signatures,
8b virtual naming, 9 header generation, 10 prelude reconciliation, then the
forwarders, the per-TU compile sweep, the whole-tree link, verification and
metrics.

`verify.py`'s coverage check credits a template instantiation to the header that
defines its template: 487 of the 749 manifest entries are instantiations
(`VectorClass`, `DynamicVectorClass`, `TClassFactory`, `TypeList`, `Blit*`), and
the template header legitimately defines them all. This is not a loosened gate —
do not satisfy it by emitting one header per instantiation.

## Build / verify

The gate is `tools/verify.py`. It runs nine checks: standalone compilation of
every header at the target ABI (`-m32`), a combined translation unit at `-m32`,
class coverage against the manifest, exhaustive vtable→class bindings resolved
through the binary's own RTTI COL chain, header-content-vs-manifest,
inheritance-vs-direct-bases, member offsets-vs-recovered-table,
declared-vs-recovered virtual arity, and `offsetof` assertions that make the
compiler confirm every recovered member offset. It is fail-closed: a missing
artifact makes it exit non-zero naming what is missing rather than running a
subset and printing PASSED, and it exits non-zero when `g++` cannot compile
`-m32` rather than falling back to the host's LP64 ABI.

**The target ABI is a prerequisite, not a preference.** The target is PE32
i386, so the gate compiles at `-m32`: on the host's LP64 ABI `sizeof(void*)`
is 8, `long` is 8 bytes, and `__stdcall`/`__thiscall` are defined away, so a
host-ABI build answers a question the game does not ask. Install
`g++-multilib` (Debian/Ubuntu: `apt-get install g++-multilib`); without it
`tools/pipeline.sh` stops at its prerequisite check and `tools/verify.py`
exits naming the missing toolchain. This is deliberate — a silent skip is
exactly how an unlinkable tree survived here for months.

```bash
cd re
./tools/pipeline.sh            # regenerate everything, then verify
python3 tools/verify.py src/include build/shim
python3 tools/verify_members_binary.py   # offsets re-derived from the binary
python3 tools/test_tools.py              # unit tests for the tools themselves
python3 tools/link_test.py               # link the whole tree and run it
python3 tools/harness.py metrics --no-append   # real measured state
python3 tools/harness.py determinism --run     # two clean runs, byte-diffed
```

The pipeline runs all of these. A change is not done until `VERIFICATION
PASSED` is printed, every generated translation unit compiles, and the tree
links.

Every header must also compile standalone:

```bash
for h in src/include/*.h; do
  g++ -std=c++03 -fsyntax-only -Isrc/include -Ibuild/shim -include "$h" -x c++ /dev/null || echo "FAIL $h"
done
```

## Booting the game

The tree does not just compile and link -- it executes the original program. The
two halves that make that possible are `Image.h` and `Win32Stubs.h`, and neither
does anything alone: the transcribed code reads its call target out of the
modelled IAT, so an IAT full of zeroes makes every `call dword ptr [IAT]` land on
address 0 and the run stops at the first one.

What the boot does today, measured:

```
$ TS_CD=../work/cd1 python3 tools/boot.py
image:  3227799 bytes in 4 sections; 341 of 341 imports resolved, 341 traced
stop:   instruction budget exhausted at 0x00488800 after 20000001 instructions, depth 19
why:    instruction budget exhausted (sub_0048875c +0xa4)
  CreateFileA("D:\TS1.dsk") -> .../TS1.DSK (19 bytes)
  CreateFileA("EXPAND99.MIX") ... CreateFileA("EXPAND00.MIX")   (100 files)
  MessageBoxA(... type 0x00000030) called from 0x00484456 -> IDOK
message loop: reached   (GetMessage=1 PeekMessage=5 DispatchMessage=1
                         CreateWindowExA=1)
unmapped calls: 0
coverage: 55774 distinct instructions; 4764 of 45947 recovered functions entered
```

At larger budgets the model selects a campaign and starts the GDI mission:

```
default scripted input (New Campaign, NOD1, GDI1, OK),
--budget 2100000000 --timeout 600:
  stop 0x0067dc4f (LCW video decoder, sub_0067daa0), depth 28
  GetMessage=17 PeekMessage=13565102 DispatchMessage=17 CreateWindowExA=1
  coverage 84130 / 6431; pages-used 9029 of 32768
  frame re/out/mission.png: 640x480, 194131 non-black pixels

same timeline + ESC at model-ms 192000, --budget 4000000000:
  stop 0x0055b0bb (budget) after 4000000001 instructions, depth 29
  coverage 139250 / 8682; pages 12809 of 32768
  render 8 surfaces, 12 blits, 769 locks, 0 flips
  frame /tmp/mission_4b.png: 640x480, 290399 non-black,
  md5 6cb0e1fff05f9250adda298e644f1266 = committed re/out/render_2_6b.png
  (the GDI loading screen)
```

The interpreter is **12-14x faster** (~400M instructions/s; measured 11.7 s at
4B, 50.2 s at 20B, 243.9 s at 100B) with byte-identical machine state: build at
`-O2 -fno-pie`/`-no-pie`, a 256-entry positive-hit page cache, wide single-page
`ld16/st16/ld32/st32`, a `noinline insn_slow()` cold path, inlined page/flag
helpers and a cached import-name lookup. `test_machine_perf` pins the hot path.

So: the game starts, initialises, shows the expected DirectSound warning, creates
its window, **delivers `WM_ACTIVATEAPP`**, reads the CD (`TS_CD` models `D:`),
builds its real dialogs from LANGUAGE.DLL's templates, plays past the intro,
**renders the real main menu**, **selects the GDI campaign, starts the mission,
and plays the GDI intro movie**. The committed `re/out/frame.png` is 640x480,
md5 `0fdf2636c2643ce2510ae67fb9ca9e26`, with **254,610 / 307,200 non-black
pixels**; it shows the title logo, GDI/Nod wheel and emblems. Against the retail
screenshot (`/tmp/retail.png`, md5 `58cb9c0b…`), **102,836 pixels differ at all
(33.48%)**, but the mean absolute channel difference is **1.06/255 over the
differing pixels** (0.35 over the whole frame) and only **506 pixels differ by
more than 8**, all in the bottom-right retail version text the model does not
draw; the rest is RGB565 quantization. The intro frame is the GDI title
("SEPTEMBER 2, 2030 / GDI SPACE STATION PHILADELPHIA"), 194,131 non-black.

After ESC, the mission load runs far past the intro: the loader's own O(n²)
`DynamicVectorClass` growth (22 vectors at `this+0x1280`, `sub_005a35b0`, each
append through `SetCount` `0x55b050` → `0x55b09a`) dominates the cost. At a 100B
budget it is on vector 7/22 with **160,070 entries** (`TS_PEEK` at `0x712068`:
count and capacity both `0x27146`, growth step 10) and has not finished. A
1.2e12-budget run reached **478,970,534,425 instructions (479B)** and then
**faulted at `0x00422de0`** with `page table exhausted: page allocation failed`
(1,024,211 / 2,097,152 pages, 1 refused) and **7,771,444 unmapped calls** — 7 in
`.text`, all `0x606060` from `sub_004f3b40`, 7,673,235 below `0x1000`, 98,202
elsewhere — with wild registers across the top of the 4 GiB space and a **black
frame** (9 surfaces, 171 blits, 10,484 locks, 0 fills/flips). That is a new
defect (a pointer table or stack going wild), not the O(n²) estimate: the loader
got far past 100B before it happened. A 500B-budget reproduction this session
faulted at the same EIP after **478,968,556,708** instructions with 1,023,748
pages and 7,734,028 unmapped calls (7 in `.text`, same `0x606060` call sites),
so the flood reproduces in character but the exact length varies slightly
between runs. **The tactical phase is not reached and no mission frame is
rendered.** `tools/boot.py --strict` exits **0** and the link test passes.

**The menu is selectable, and the game leaves it.** LANGUAGE.DLL `RT_DIALOG` 226
is the menu (306x239 at (167,187)); its six `BS_OWNERDRAW` buttons are:

| id | control | screen rect | result |
|---|---|---|---|
| 0x617 (1559) | New Campaign | (223,207)-(418,236) | 1 |
| 0x619 (1561) | Load Mission | (223,241)-(418,270) | 2 |
| 0x61A (1562) | Multiplayer Game | (223,275)-(418,304) | 3 |
| 0x3EB (1003) | Intro / Sneak Peek | (223,309)-(418,338) | 4 |
| 0x55C (1372) | Options | (223,343)-(418,372) | 5 |
| 0x3EE (1006) | Exit Game | (223,377)-(418,406) | 6 |

The model performs press/release → `BN_CLICKED` in `dialog_mouse`, so the click
reaches the game's own hit-test and `WM_COMMAND` path. The default timeline
clicks New Campaign: the menu exits with result 1 and the game creates
`RT_DIALOG` **148, "Select Campaign:"** at step **1,688,503,557**. Clicking Exit
Game instead reaches **`ExitProcess(0)`** at `0x00684529` after **1,707,666,895**
instructions.

**Correction to an earlier claim:** `0x7b2ab8` and `0x7b0d38` do **not** change.
`0x7b2ab8` is the front-end-active flag (still 1) and `0x7b0d38` the game mode
(still 0 because the game is still in the front-end). The selection signal is the
dialog procedure's result code through `GWL_USERDATA`.

**The campaign list works and the mission starts.** The list source is the
shipped `BATTLE.INI` (inside `TIBSUN.MIX`, committed as `re/realdata/BATTLE.INI`):
`[Battles] 1=GDI1, 2=NOD1` with `Description` strings. The game's dialog
procedure `0x004db8b0` feeds listbox 1109 via `LB_ADDSTRING` in `WM_INITDIALOG`
and sets `LB_SETCURSEL 0`; OK reads `LB_GETCURSEL` and `TBM_GETPOS` (trackbar
1295). The model implements the full listbox/trackbar layer, real `FindFirstFileA`
host enumeration, and a read-only `TS_DATA` root (default `realdata`) that serves
`BATTLE.INI`. The default timeline selects NOD1 then GDI1 and clicks OK; after OK
the campaign dialog is destroyed and **`GDI1A.MAP`** is read from **MAPS01.MIX on
CD1** (`NOD1A.MAP` is in MAPS02.MIX on CD2). The **GDI intro movie plays**: at
2.1e9 the stop is the LCW video decoder at `0x0067dc4f` (`sub_0067daa0`),
`pages-used 9029/32768`, and the frame is the GDI intro title ("SEPTEMBER 2,
2030 / GDI SPACE STATION PHILADELPHIA"), **194,131 non-black pixels**
(`re/out/mission.png`).

**The nested MIX is mounted and read.** `TIBSUN.MIX` entry `0xa8548fd9`
(18,044,736 B at offset 22,111,980) is a 317-entry archive containing 111 PCX
files, including the button art. The art is not painted because the model never
sends `WM_PAINT` to child controls; the retail also draws no buttons in this
environment, so this is separate and non-blocking.

**The current blocker is the 479B wild-call fault during the mission load.** ESC
at model-ms 192,000 skips the movie and the loader runs; the page table is now
dynamically growable (2,097,152-page cap) and no longer the fixed 32,768-slot
limit. At 4B the load is clean (`0x0055b0bb`, 12,809 pages, 12 blits, loading
screen); at 100B it is still on vector 7/22; the 1.2e12-budget run faulted at
`0x00422de0` after 478,970,534,425 instructions with the wild-call flood above.
A diagnosis lane is finding the **first** wild call, not the flood. The
**mission load does not complete**, the tactical phase is not reached, and no
mission is rendered.

**A correction that matters more than any of this.** An earlier version of this
file reported `message loop: reached` with `PeekMessage=137,295`. That was wrong,
and the way it was wrong is worth keeping: the model let `ExitProcess` *return*,
so the frames above it ran to their own `ret` and execution continued past a fatal
exit. Everything observed after that point was a loop the real game never ran.

**And this block kept reporting the retracted number for a while after the
correction was written** — the correction sat ten lines below a measurement that
contradicted it. If you are reading a figure here that disagrees with
`python3 tools/boot.py`, the tool is right and this file is stale.

**The defect that explained it, and its fix.** Before the fix, the exit
arguments were stub addresses rather than the numbers the code pushed:
`ExitProcess(0x6844df)` where the code passed `1`, and a `MessageBoxA` type of
`0x00a00201` (inside the import-thunk region) where the code passed `0x30`.
Tracing the CRT's atexit walk over the 3,311-entry static-constructor table
showed why:

```
at 0x6844cf (push %esi, the atexit walk):  esp=0x002ff56c
at 0x6844ea (pop  %esi, the same walk):    esp=0x002ff4fc
```

**108 bytes — 27 dwords — leaked across the walk.** After it, `pop %esi` read a
return address instead of the saved register, ESI became `0x6844df`, and

```asm
5e72cf: call *%esi        ; goes to an address inside ExitProcess
5e72d1: mov  -0x5f4(%ebp),%bl
5e72d7: test %bl,%bl
5e72d9: je   0x5e7306     ; zero would skip the error box; it is not zero
```

so the game showed its "Direct X" error box with corrupted arguments and exited.
**Fixed:** `ts_call_ptr` now delegates to `ts_call_at` (`Machine.h:863`), so the
two call paths balance the same way. Measured after the fix: `ExitProcess(1)`,
`MessageBoxA` type `0x30`, and 0 unmapped calls inside `.text`. The two model
defects that surfaced alongside it are also fixed: `jmp_indirect` was a no-op
(now `ts_tail`) and conditional `rep`/`repne` `scas`/`cmps` zeroed ECX instead of
leaving the count that did not run.

**The next blocker was COM registration, and it is fixed (`1580be3`).** The game
registers Westwood's `BLOWFISH.DLL` class `{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}`
through `GetProcAddress(handle, "DllRegisterServer")`, but the model's
`LoadLibraryA` returned an anonymous handle and `GetProcAddress` could not resolve
an export on a module it only pretended to load. Now `LoadLibraryA` returns a
named module handle with a per-module export table; `blowfish.dll` exports
`DllRegisterServer`/`DllUnregisterServer` (S_OK), `DllGetClassObject` and
`DllCanUnloadNow`; `CoCreateInstance` writes an `IBlowfish` object whose
eight-slot vtable was recovered from `BLOWFISH.DLL` (slots 3–7 are `SetKey`,
`GetMaxKeyLength`=56, `GetBlockSize`=8, `Encrypt`, `Decrypt`);
`IDirectDraw::CreateClipper` writes a real clipper; and `DirectSoundCreate`
returns `DSERR_NODRIVER` (no audio, matching retail). A
`win32-module-exports` regression test covers handle identity, export isolation,
`*ppv` being written, the IBlowfish methods and ESP balance. (`test_tools.py`
runs **94** checks now; the module and interpreter-perf tests are among the
additions.)

**Message delivery is fixed (`f3011ca`).** `CreateWindowExA` queues
`WM_ACTIVATEAPP` (`0x001c`, `wParam=1`) after the synchronous `WM_CREATE`;
`RegisterClassA` records the WndProc; `PeekMessageA`/`GetMessageA` return the
`MSG`; `DispatchMessageA` calls the WndProc. Measured at that commit: the
WndProc at `0x65ba70` runs, `DAT_007b2e50` becomes 1 at `0x65bd2a`, and the game
leaves the spin — `GetMessage=1`, `PeekMessage=3`, `DispatchMessage=1`,
`CreateWindowExA=1`, **1,882,051 instructions**, coverage 49,635 / 4,455
functions. A mutation-proven USER32 test covers the `MSG` layout,
`PM_REMOVE`/`PM_NOREMOVE`, WndProc dispatch and the `__stdcall` pop.

**The Blowfish cipher is implemented and proven (`345aae7`).** The retail oracle
(`re/out/blowfish_oracle.md`, produced by `tools/oracle_blowfish.py` and
`tools/wine_blowfish_trace.sh`) measured the registered object as a real cipher:
`SetKey(56,key)` **9 times** and `Decrypt(8,in,out)` **1,586 times** during
startup, with the nine keys byte-identical to the keys derived from nine shipped
MIX indexes. The model now implements standard Blowfish-ECB in
`IBlowfish::SetKey`/`Encrypt`/`Decrypt` (pi-derived P-array and S-boxes, the
standard key schedule, 16-round Feistel, big-endian blocks, `len/8` blocks plus a
verbatim tail, in-place operation). It is proven by the four standard
known-answer vectors and a byte-identical cross-check against `tools/mix.py` on
the shipped TIBSUN.MIX index: key `82060323...`, ciphertext
`2ce3171bdbecb40e` decrypts to `110090d97904849a` (count 17, body 75,094,416).
The Blowfish-era page-table exhaustion at `0x00a006b0` is gone; the table now
grows, and the mission load instead reaches 479B and faults on a wild-call flood
(see below).

**The MIX index read is fixed (`1c0db7e`), and the fix exposed a disasm defect
(`9e5703f` + `ad62afc`).** The short read was not the pipe or the cipher: the
unaligned-destination `memcpy` jumps into entry 2 of a switch table at
`0x683c4c`, which was an instruction boundary but not a registered function
start, so `jmp_indirect` returned and abandoned memcpy's frame. The generator now
registers every static switch-table entry that is an instruction boundary as a
tail entry, and `tools/disasm.py` classifies switch tables from evidence only.
Result: **213 static switch tables / 6,388 B** are data (was 12 / 264 B),
including three hole tables (`0x683c10`, `0x683d98`, `0x68ce40`) whose entry-1
addresses are now function starts. Recovered functions **43,557** (was 43,564),
transcriptions **45,947** (was 46,051), instruction lines **912,879**, `.text`
code **95.86%**; every one of the 3,186 instruction addresses that left the
transcription was classified (2,581 inside classified tables, 249 re-decoded
from corrected boundaries, 356 fill/table bytes). All **27 index blocks** are
read and the 17 records match `tools/mix.py` 17/17.

**The stack tracker and the null teardown calls are fixed (`d047496` +
`47d26eb`).** `stub_done` records the popped argument count and `ts_call_at`
compares a direct stub call against `esp_before + 4*nargs`; **111,682 false
stack imbalances → 0**. The five null teardown calls had one root cause:
`IDirectDrawSurface::Lock` was declared with 4 stack arguments but COM is
`__stdcall` and `this` is the first stack argument, so the game pushes **5**;
each call leaked 4 bytes until EBP reached 0 and AlternateSurface was left null.
`Lock` now pops 5. Measured: **0 unmapped calls, 0 stack imbalances**,
`boot.py --strict` exits **0**, and the link test passes.

**Signed `idiv` is fixed (`16e21fa`).** `divmod64` now has true signed x86
semantics (magnitudes + signs, correct overflow/divide-by-zero, `INT64_MIN / -1`
handled), so the old divide-overflow stop is passed. `test_tools.py` runs **77**
checks, including signed-division vectors, a 200,000-pair differential sweep and
the `IDirectDrawSurface::Lock` 5-argument regression.

**The dialog layer is real (`f576607` + `c81c1f5`), and narrow-width flags are
fixed (`f2e6ca9` + `19ee16c`).** `Win32Stubs.h` reads the module's own PE `.rsrc`
from disk: `FindResourceA`/`LoadResource`/`LoadStringA` serve LANGUAGE.DLL's
**60 dialog templates and string table**. `CreateDialogIndirectParamA` builds the
dialog record and its template controls, sends `WM_INITDIALOG`, and queues a
scripted `WM_COMMAND`; `GetDlgItem` returns stable per-control handles; the
window-state calls are real. A CD drive model (`TS_CD` maps `D:` to a host root,
`DRIVE_CDROM`, label `TS1`) was added. `SetViewportOrgEx`/`SetWindowOrgEx` take
**4** arguments (the stubs popped 3, leaking 8 bytes and corrupting EBX);
`19ee16c` fixes the generator's table so a regeneration cannot restore it
(`--stack-check` 342/0). `Machine.h` now has width-aware
`f_add_w`/`f_sub_w`/`f_inc_w`/`f_dec_w`/`f_log_w`, so all 8/16-bit
arithmetic/comparison derives SF/OF/ZF/CF/AF/PF from the operand width. The
tests compare every flag against the CPU's own flags via inline asm, including
the `0x7f`/`0x80` boundary and the vsprintf `%` classification; mutation-proven
(18 assertions fail with the old derivation). `test_tools.py` is **75** checks.

**Rendering and input are integrated (`5031e44` etc.), and the model renders the
real main menu.** `Win32Stubs.h` routes surfaces through `ts::rnd`
(`Rendering.h`: per-surface memory, pitch, 16bpp RGB565, `Lock`/`Unlock`,
`Blt` (6 words), `BltFast`, `Fill`, `Flip`, descriptors, palette) and input
through `ts::input` (`Input.h`: scripted key/mouse state, cursor, synthesized
messages). `SCREEN_W/H` are 640x480 and `--frame` dumps the primary surface.
The committed `re/out/frame.png` is 640x480, md5
`0fdf2636c2643ce2510ae67fb9ca9e26`, with **254,610 / 307,200 non-black pixels**;
it shows the title logo, GDI/Nod wheel and emblems. Against the retail screenshot
(`/tmp/retail.png`, md5 `58cb9c0b…`), **102,836 pixels differ at all (33.48%)**,
the mean absolute channel difference is **1.06/255 over the differing pixels**
(0.35 over the whole frame), and only **506 pixels differ by more than 8**, all
in the bottom-right retail version text the model does not draw; the rest is
RGB565 quantization. `test_runtime.py` runs **101** checks, mutation-proven.

**The campaign list works and the mission starts.** The list source is shipped
`BATTLE.INI` (`[Battles] 1=GDI1, 2=NOD1`, with `Description` strings); the game's
dialog procedure feeds listbox 1109 via `LB_ADDSTRING` and sets `LB_SETCURSEL 0`,
and OK reads `LB_GETCURSEL`/`TBM_GETPOS`. The model implements the full
listbox/trackbar layer, real `FindFirstFileA` enumeration and a read-only
`TS_DATA` root. The default timeline selects NOD1 then GDI1 and clicks OK;
`GDI1A.MAP` is read from MAPS01.MIX and the **GDI intro movie plays** (at 2.1e9
the stop is the LCW decoder `0x0067dc4f`, `pages-used 9029/32768`; the frame is
the GDI title, 194,131 non-black). ESC skips the movie; the mission load then
runs on the game's own O(n²) vector growth (at 100B it is on vector 7/22 with
160,070 entries) and a 1.2e12-budget run faulted at 479B instructions at
`0x00422de0` with 7,771,444 unmapped calls and a black frame. The **tactical
phase is not reached and no mission is rendered.** `test_tools.py` is **94**
checks. The interpreter is **12-14x faster** (~400M instructions/s) with
byte-identical machine state.

**Why it used to exit immediately, and the shape of the answer.** The CRT startup
reads the comctl32 version and, if it is below 4.00.70, shows an error box and
calls `ExitProcess(1)`. It reaches `DllGetVersion` through `GetProcAddress`, not
through the import table, so there was no stub for it, the version read as 0, and
the game exited before initialising anything. It was telling us why it would not
start, in a message box whose text is unreadable because the string table is empty
at that point. One export reporting 4.72 fixed it.

The lesson generalises: **the game's own failure paths are the best diagnostic
there is**, but only if the thing it fails on is modelled well enough to be
believed. A stub that returns 0 where the real Windows returns a version does not
produce an error -- it produces the game taking a legitimate-looking branch that
exits.

Three things are worth knowing before touching this, because each cost real time
to find:

- **The model's page table must be big enough for the game's working set.** It was
  8192 slots; the game's memset needs 32,768 pages; a full table made `page()`
  return one shared dummy page for every new address, so every distinct page
  silently overwrote the same 4 KB while linear probing degraded to O(8192) per
  lookup. The only symptom was "it got slow". Exhaustion is now a hard fault.
- **`perf record -g -p <pid> -- sleep 8`** is how that was found (`ts::page` at
  99.67%), and it is the fastest way to find the next one. `kernel.perf_event_paranoid`
  must be 1.
- **A stub that lies is worse than a stub that fails.** An unimplemented import
  goes through a fallback that logs its name and returns 0, and every call is
  counted, so an unimplemented import is visible rather than silently plausible.
  The same applies to a stub that returns a *plausible* wrong value: returning 0
  from `DllGetVersion` did not look like an error, it looked like an old DLL.
- **The model has diagnostics; use them instead of reading disassembly.**
  `TS_TRACE=0xADDR` records ESP/ESI/EDI/EAX/ECX/EDX at the first 8 executions of an
  address; `TS_PEEK=0xADDR[,..]` reads globals at the end of a run (which state is
  the game in?); `TS_SCAN=start,count` reports table entries that are not `.text`
  VAs (a corrupted table); `TS_WATCH=0xADDR` gives the EIP of the first non-zero
  byte store to an address. `bad_calls` names calls whose target is outside `.text`
  with no stub, plus the call site; `stack_imbalances` catches a stub whose
  argument count is wrong; the leak tracker names callees that leave ESP moved.
  "Is ESP right at this push?" is a question the machine answers in one run and
  counting pushes by hand answers wrongly.
- **Know which gate is slow.** `tools/verify.py` is **6 seconds**; `pipeline.sh`
  is **471 seconds** and the cost is the whole-tree compile and link. Iterate with
  `tools/boot.py` (sub-second after a cached build) and `verify.py`, and spend the
  471s only when something is ready to land. Most of the time lost in this project
  was lanes running the full pipeline to check a six-line change.
- **Use the oracle before believing the model.** `tools/wine_env.sh` runs the
  retail binary in the environment it needs; `tools/oracle.py 0xADDR` reads its
  real registers. The first oracle runs said the retail game exited with code 01
  early — but that was the *environment*, not the game: with the CD and a
  640x480x16 screen it reaches its main menu. Check what the real binary does
  before spending a day on a "divergence", and make sure the environment is not
  the thing that is failing.
- **A stub that says a file exists and reads as empty is the worst kind of lie.**
  The file layer did exactly that: every file "existed", `GetFileSize` said 0 and
  `SetFilePointer` agreed, so the lie was internally consistent and nothing ever
  read past an end that was never written. The game cannot load `rules.ini`,
  `TIBSUN.MIX` or the CD, so it exits during init, and no amount of DirectDraw work
  changes that. It is now real file I/O over a host root with case-insensitive
  path resolution.
- **Use Ghidra for semantics.** `./tools/ghidra.sh 0x478e60 0x684446` prints the
  decompiled C for those addresses in seconds against a saved, already-analysed
  project. Reading `objdump` to work out what a function is *for* is the slowest
  thing you can do here; the decompiler answers it directly.

## Key technical facts

- Target is PE32 i386, MSVC 6.0, ImageBase `0x400000`, `.text` at `0x401000`.
- RTTI is intact. `type_info` layout: `+0x00` vftable, `+0x04` spare,
  `+0x08` inline mangled name. The `type_info` vftable is at `0x6a7d04`.
- Complete Object Locator layout in this binary: `+0x00` signature, `+0x04`
  offset, `+0x08` reserved, `+0x0c` pTypeDescriptor, `+0x10` pClassDescriptor.
  The COL pointer sits immediately **before** the vtable.
- RTTI base-class arrays list **transitive** bases. `out/direct_bases.json`
  holds the reduced direct-base set; use that for C++ inheritance.
- INI key strings are the original struct member names. `out/ini_keys.json`.

## Subagent notes

- Background/async subagents require `@earendil-works/chord`, which is **not** a
  declared dependency of `@earendil-works/pi-coding-agent` and is not installed
  by default. It is not resolved from the user-level
  `~/.pi/agent/npm/node_modules`. Install it into the host package itself:

  ```bash
  cd "$(npm root -g)/@earendil-works/pi-coding-agent"
  npm install --no-save @earendil-works/chord@1.1.0
  ```

  Without it **every** subagent launch fails immediately with
  `does not provide @earendil-works/chord, @earendil-works/chord/context`, and
  `async: false` does **not** help — that error is a missing module, not a
  background-job problem. Verify with
  `node -e "console.log(require.resolve('@earendil-works/chord/context'))"`
  from that directory (a bare `require.resolve('@earendil-works/chord')` fails
  with `ERR_PACKAGE_PATH_NOT_EXPORTED`, which is expected and harmless: the
  package is ESM-only and exports only subpaths).
- **Run long multi-lane workflows `async: true`, and raise the per-child
  deadline.** Two separate caps bite: a blocking workflow (`async: false`) has a
  30-minute wall-clock limit, and every child has a 30-minute default timeout of
  its own. `async: true` removes the parent deadline but **not** the child one —
  pass `timeoutMs` on the workflow call (e.g. `5400000` for 90 minutes) when
  lanes will run `pipeline.sh` (~20 minutes now) plus analysis. Passing
  `async: false` to get a same-turn result cost six lanes' work in one run.
- **A lane reporting "completed" has not necessarily committed anything.** Three
  separate W5 attempts reported completion with a clean worktree and a branch
  with zero commits; its artifact was the *unexecuted* `write` call text. Always
  verify independently before assuming work landed:
  `git log --oneline main..<lane-branch>`, `git worktree list`, and
  `git status` inside the lane's worktree. Recoverable partial output lives in
  `/tmp/pi-subagents-uid-*/async-subagent-runs/<run-id>/` (`output-0.log`,
  `status.json`) and the session artifacts directory.
- **A lane whose final action is one very large `write` may never execute it.**
  If a file is more than a few hundred lines, either split it into several
  writes or do it yourself with targeted edits. After three failed delegations of
  the same 400-line rewrite, doing it directly with anchored edits was faster and
  reliable.
- **Use few lanes.** Two well-scoped lanes finished more than six did. Six lanes
  also multiply the merge work and the chance of cross-lane conflicts in
  generated output. Prefer 2–3.
- Give reconstruction agents **tight, bounded scopes** and tell them explicitly
  not to write giant inline heredocs — one agent previously burned its entire
  budget that way and produced nothing.
- Parallel writers must target **disjoint file sets**. Worktree isolation
  (`isolation: "worktree"`) prevents file collisions but does **not** preserve
  work: a lane killed before its first commit loses everything. Instruct every
  lane to **commit immediately after editing, before any long verification run**,
  and to report the commit hash.
- **Beware lanes that commit to `main` rather than their worktree.** Two lanes in
  one round did this, so their work appeared in the main history while their
  branches stayed empty. Check `git log` on `main` as well as the lane branches
  before concluding a lane produced nothing.
- A lane that changes a *generator* can break the tree in ways no lane owns, and
  the damage can be silent. Two examples from this repo: a lane that taught
  `gen_headers.py` to emit member blocks for headerless classes created 424
  duplicate headers including a `GScreenClass.h` that redefined a prelude-owned
  type; and the same emitter's member-block logic silently **deleted** ~593
  declaration lines across 43 headers over successive runs. Treat generator
  changes as high-risk, require the lane to run the full pipeline and
  `verify.py`, and **re-verify the merged result yourself**.
- Lanes will correctly refuse to document a broken tree, and will report defects
  in each other's work. Expect questions like "which state should the docs
  describe?" — decide the target state explicitly rather than letting a lane
  guess, and read the cross-lane findings rather than filing them.

## Current status

Last updated after the interpreter-speed and 479B-fault work. Every figure here
was re-measured on 2026-10-18 with `python3 tools/harness.py metrics --no-append`,
`python3 tools/verify.py src/include build/shim`,
`python3 tools/verify_members_binary.py`, `python3 tools/test_tools.py`,
`python3 tools/test_runtime.py`,
`python3 tools/gen_code.py out bin/GAME.EXE src/generated/code --check`,
`python3 tools/link_test.py` (with `TS_ROOT`/`TS_CD`/`TS_DATA`), and direct
counts over `out/` and `src/generated/`, plus `python3 tools/boot.py` (20M; 2.1B
with the default timeline; 4B, 20B and 100B with the ESC-skip script),
`python3 tools/boot.py --no-build --strict`, and `tools/wine_env.sh`. Pages and
render counters are printed by `boot.py`; the vector fields at 100B are read
with `TS_PEEK` through `./build/boot_driver`. Re-run these rather than trusting
this list. The boot and frame figures below were re-measured after the
interpreter-speed merge; the 479B fault figures are the coordinator's
1.2e12-budget run, recorded in `todo.md`.

**How far off "complete" is this project.** The layers are far apart. Read this
before quoting any single number. `ROADMAP.md` states each of these as a
measurable exit criterion rather than a percentage.

| Layer | State | Note |
|---|---|---|
| **ABI / layout** | **reproduced, compiler-confirmed** | everything compiles `-m32` at the target ABI, and check 9 makes the compiler confirm every recovered member offset a header can express: **3,073/3,073**. The 1,216 offsets with no assertion are itemised by reason (743 vptr stores the compiler inserts itself, 413 past the declared extent, 44 undeclared, 10 no member offset, 6 headerless) |
| Binary recovery | **effectively closed** | **43,557** functions/spans, **95.86%** of `.text` as recovered code; **112,401 B (110 KB)** unattributed, classified in `out/text_data.json` as **213** static switch tables (6,388 B), **102** switch index tables (145 B), **105,492 B** of alignment padding, and **376 B in 46 runs** reported verbatim rather than forced into a category. 99.99% of `.text` is attributed as code or data. |
| Structural reconstruction | **strong** | 749/749 classes, **9,810** vtable slots, 0 unseeded, every constructor a real entry point |
| Code transcription | **complete** | **43,557** recovered function spans, transcribed as **45,947** functions (43,557 starts + **2,390** tail entries) and **912,879** instruction lines, 100% modelled, 0 unmodelled |
| Verification | **9 checks green, link test green** | `verify.py` passes all nine (265/265, 749/749, 749/749, 219/219, 220/220, 1317/1317, check 9 PASS; about 6 s last timed). `link_test.py` exits **0** (`link: OK`, 492 TUs, message loop reached and dispatching with the data roots set, 0 unmapped). `test_tools.py` runs **94** checks and `test_runtime.py` **101** (mutation-proven). A full `./tools/pipeline.sh` ran **exit 0** in the disasm-correction lane (8m51s, per `todo.md`); it was not re-run in this worktree |
| **Semantics** | **~9%** | 5,557 of 9,810 slots carry a name (56.6%), but only **917 distinct names** and **281 distinct semantic** ones — most of the coverage is shared-stub propagation. 291 members named across 20 classes; `e2e_rules.py` resolves 96.2% of them against the shipped `rules.ini` |
| **Modifiability** | **minimal** | there is still no hand-written `.cpp` in the tree; every function is generated machine transcription, so the editable surface is the hand-written headers |
| **Behaviour** | **starts the GDI campaign and plays the intro; the mission load runs to 479B and faults on a wild-call flood** | the image maps at its real VAs, **341/341** PE import-directory thunks resolve (the stub table has 342 rows; the extra is `COMCTL32!DllGetVersion`, reached through `GetProcAddress`). With `TS_CD`/`TS_DATA` the game reads the CD, builds its real LANGUAGE.DLL dialogs, plays past the intro, **renders the main menu**, and the default timeline clicks New Campaign (six `BS_OWNERDRAW` buttons, real rectangles) → `RT_DIALOG` 148 "Select Campaign:" at step 1,688,503,557. The campaign list comes from shipped `BATTLE.INI` (`[Battles] 1=GDI1, 2=NOD1`); the model's listbox/trackbar layer feeds it and OK selects GDI. `GDI1A.MAP` is read from MAPS01.MIX and the **GDI intro movie plays**: at 2.1e9 the stop is the LCW decoder `0x0067dc4f`, `pages-used 9029/32768`, frame `re/out/mission.png` (194,131 non-black, the GDI title). The interpreter runs at ~400M instructions/s (12-14x faster, byte-identical state). ESC skips the movie; the loader's own O(n²) vector growth dominates (at 100B it is on vector 7/22 with 160,070 entries), and a 1.2e12-budget run **faulted at 479B instructions at `0x00422de0`** with 7,771,444 unmapped calls (7 in `.text`, all `0x606060` from `sub_004f3b40`), 1,024,211/2,097,152 pages and a black frame. **0 unmapped calls and 0 stack imbalances at 20M/2.1B/4B/100B**; `boot.py --strict` exits **0**. **The tactical phase is not reached and no mission is rendered.** The retail binary reaches its main menu under `tools/wine_env.sh` |

So: *the binary is explained, its instructions are transcribed, the layout is
compiler-confirmed, and the game's own startup now runs cleanly through real
dialogs, the CD check, the intro movie, a rendered and selectable main menu, the
campaign list, and into the GDI campaign's intro movie.* The model creates a
window, dispatches messages, reads the MIX index and the CD, renders the menu
(`out/frame.png`, within 1.06/255 of retail on the pixels that differ), clicks
New Campaign and OK through the game's own hit-test, reads `GDI1A.MAP`, and
plays the GDI intro (`re/out/mission.png`, 194,131 non-black). But the mission
load runs to 479B instructions and then **faults on a wild-call flood at
`0x00422de0`**, so **the tactical phase is not reached and no mission is
rendered**. The binding constraint is no longer recovery, transcription or
layout — all three are done. It is **semantics, source-shaping and the model's
runtime (the mission loader, the wild pointer/stack, dialog painting, timers,
input-driven play)**, and the honest headline is that the source is still barely
named and not editable as source.

Two corrections that matter when sizing the rest:

- **The naming denominators are three different quantities.** The old "3,409
  distinct slots" was an estimate of slot instances after inheritance
  deduplication, not a name count, and is retracted. Measured: **9,810** vtable
  `(class, slot)` instances; **5,557** named pairs (56.6%); **917** distinct
  name strings (**281** semantic, excluding destructors and QI/AddRef/Release);
  **6,957** virtual declarations in the headers (**4,499** named + **2,458**
  `vfunc_N`); **702** classes with a naming record (this is what
  `harness.py metrics` prints as `virtual_names` — not a count of names).
  65.1% of slot instances are inherited, so naming a base names its subtree;
  the top 18 bases cover 13,011 slot-instances.
- **`-m32` is in the gate today** (`g++-multilib` installed): **265/265** headers
  compile at the target ABI, the tree links (492 TUs) and the entry point runs.
  This item is closed; check 9 confirms 3,073/3,073 offsets.

**The whole recovered instruction stream is transcribed.** `tools/gen_code.py`
renders each recovered function into C++ against the `ts::` machine model in
`src/handwritten/Machine.h` — registers, flags, memory and control flow are
modelled, so the generated code does what the machine does. Each generated line
is one machine instruction and keeps the original as a trailing comment. It emits
one canonical `ts::sub_XXXXXXXX(void)` per recovered function VA (**43,557**),
plus **2,390 tail entries**: addresses a cross-function branch targets that are
not function starts, which were previously dispatch gaps. That is **45,947**
transcriptions and **912,879** instruction lines in all. (The harness's
`shared_tail_entry_points` = **1,000** is a *different* quantity: entry points
that sit strictly inside another recovered body.) The per-class files in
`src/generated/` are forwarders across **310** translation units that call
those, so one machine function has one body.

`tools/link_test.py` links all **492** translation units into a program, registers
every recovered function, and executes the original PE entry point against the
model. **That link is not decoration**: a per-file syntax check cannot see a
forwarder naming a symbol nothing defines, and the tree did not link for months
because no check looked.

Honest limits, all stated in `src/generated/README.md`:

- 0 instructions have no faithful rendering. The 166 that once did (MMX/SSE, the
  x87 environment save/restore, far pointers, `lar`, `fbld`, `fcmovnbe`,
  `pushaw`) are now modelled: **912,879 of 912,879 instruction lines** are
  modelled and the transcription gate still verifies every function
  instruction-for-instruction. Two limits of the *model* remain, stated where
  they live in `Machine.h`: `lar` reports the flat ring-3 code segment because
  the model has no descriptor table, and `fnsave`/`fnstenv` write zero into the
  two x87 pointer fields because the model does not track them.
- **229** destructors are **labelled stubs**. The headers declare them; no
  comment records where they are, so there is no transcription to forward to.
- **The entry-point run does not stop at import thunks.** `Image.h` maps every
  PE section's raw extent and writes a stub VA into all **341** IAT slots;
  `link_test.py` reports `341 of 341 IAT slots written` and `0 with no stub`.
  The old "stops at import thunks / reads 0" claim described the pre-`Image.h`
  model and is retracted.

It is **not** source-shaped decompilation: the generated code is faithful to what
the machine does and is not claimed to be the shape the original source had.
A transcription reads its arguments from the model, not from C++ parameters, so
hand-written callers reach the right code through `ts::ts_invoke` but the values
they pass are only meaningful when the caller is itself a transcription.

**Done and verified.** RTTI recovery (788 type descriptors, 755 hierarchies,
843 vtables), disassembly (**43,557** functions, **95.86%** of `.text`,
`vtable_slots_unseeded` 0, **213** switch tables / 6,388 B as data), class
manifest (749 classes, **9,810** slots, every constructor a **real function
entry point**), direct-base reduction, **4,289** member offsets recovered (4,285
= 99.9% confirmed against the binary by disassembling the constructors),
**4,499** named virtual declarations and **5,557** named `(class, slot)` pairs
from **917** distinct names, **291** member names from INI keys, headers for all
749 classes, and the whole recovered instruction stream transcribed (**45,947**
functions including **2,390** tail entries, **6,962** forwarders across **310**
translation units).

The pipeline reproduces all of it from `bin/GAME.EXE` alone and
`tools/verify.py` runs **nine** checks: **265/265** standalone at `-m32`,
combined TU PASS at `-m32`, 749/749 coverage, 749/749 vtable bindings, 219/219
header content, 220/220 inheritance, **1,317/1,317** member offsets across
**206** classes, check 8 reporting declared-vs-recovered virtual arity
(**331/1,116** match, 785 divergence; reported, not gated), and check 9
compiling `offsetof` assertions for the recovered member offsets at `-m32`,
which confirms **3,073/3,073** asserted offsets. The transcription
faithfulness gate verifies all **45,947** functions instruction-for-instruction,
and `tools/test_tools.py` runs **77** checks with four mutation-proven.
`tools/harness.py determinism --run` last measured **497** artifacts
byte-identical across two clean runs — the recovered data *and* the generated
C++ (*not re-run in this session*).

**Real shipped game data is committed** under `realdata/` (rules.ini, ai.ini,
art.ini, sound.ini, theme.ini, BATTLE.INI, MISSION.INI, ~1 MB), extracted from
`INSTALL/TIBSUN.MIX`. `tools/e2e_rules.py` drives the reconstructed model with it
and checks the recovered field names against shipped keys: **280 of 291
(96.2%)** resolve; 14 are documented only in the files' comment headers. This is the only
validation that compares the reconstruction against something outside the
project's own artifacts. It is committed deliberately so a fresh clone can run
it without the 2.7 GB of CD images.

**Closing a coverage gap is a test, not just more output.** Widening the
transcription from 2,845 functions to all 43,557 — and then linking the result —
exposed six defects that every existing check had passed over, including one that
had prevented a single destructor from ever being emitted in the life of the
project. Both of those moves are the same move: make the check reach code it did
not reach before. Prefer it to adding a new assertion about code already covered.

**How to measure instead of guessing.** `python3 tools/harness.py metrics`
prints the real state and appends to `out/metrics.jsonl`;
`python3 tools/harness.py determinism --run` proves two clean runs are
byte-identical. Use these before and after any change.

**Read `ROADMAP.md` to decide what to work on, and `todo.md` before touching
anything.** The roadmap states what "done" means for *documented*, *modifiable*
and *compilable* as measurable exit criteria, corrects the raw counts for what
they actually mean (9,810 slot instances, 5,557 named pairs, 917 distinct names,
4,499 named + 2,458 `vfunc_N` header declarations; 65% of slot instances are
inherited), and gives the phase order. todo.md is the
defect ledger and records what is broken and how it was verified. Do not read the
status above as "everything is fine".

**What `VERIFICATION PASSED` does and does not mean.** The gate is now
fail-closed: a missing artifact makes it exit non-zero naming what is missing,
rather than running a subset of checks and printing PASSED. It validates that
the tree compiles, that every manifest vtable re-resolves to its class through
the binary's own RTTI COL chain (all 749, exhaustively), that headers cite
matching vtable evidence, and that generated inheritance and member offsets
agree with the recovered tables.

It still does **not** prove the disassembly is correct. Two specific gaps to
keep in mind:

- Check 4, check 9 and `tools/verify_members_binary.py` are the only checks
  anchored outside the tool chain: check 4 reads the binary, check 9 makes the
  compiler adjudicate layout. Checks 5–7 compare the tree against other tools'
  output (`out/class_manifest.json`, `out/direct_bases.json`,
  `out/members.json`), so a bug in `gen_manifest.py` would make the manifest and
  the headers wrong *together* and those checks would still pass.
- `tools/verify_members_binary.py` disassembles each constructor in the binary
  and confirms claimed offsets against real stores. It examines 637 classes and
  **4,289 claims**, confirms **99.9%** (**4,285 confirmed, 4 refuted, 0
  uncheckable**) and runs as a **gate** with a 95% floor. The 4 refuted are two
  offsets in each of `ScoreTimeClass` and `ScorePrintClass`. It caught a real
  defect earlier: a store site buried inside a large general-purpose function was
  being accepted as the constructor, which misattributed member claims for 180 of
  585 classes. A plausibility gate now requires the vtable write to occur early
  in the function through a non-stack register, which cut false claims from 1,052
  to 4.
- Check 7 covers **206 classes and 1,317 offsets** (it previously skipped
  hand-written headers and reported only 6 classes). It verifies the
  machine-generated `m_<hex>`/`vptr_<hex>` declarations against the recovered
  table. It deliberately does **not** gate *curated* hand-written names
  (`vptr`, `NextStage`, `m_Field474`): those carry their own ctor evidence and
  are often richer than the automated table, so a disagreement is reported
  rather than treated as an error.
- Check 8 reports declared-vs-recovered virtual arity: **331 of 1,116** agree,
  **785** diverge. It is **reported, not gated**, because `nargs` counts `this` for COM
  `__stdcall` methods and not for `__thiscall`, so it is a signal rather than
  ground truth. It already caught a real case (`DisplayClass.h`'s
  `TacticalClass::vfunc_4()` declared with 0 arguments where `0x4f6a20` is
  `__thiscall nargs=1`).
- **The whole tree is linked, not just compiled.** `tools/link_test.py` links
  every transcription and every forwarder into one program, registers the
  recovered functions, and runs the original PE entry point against the model.
  A per-file syntax check cannot see a forwarder naming a symbol nothing
  defines; the tree did not link for months and nothing noticed. If you add a
  generator, add it to the link — that is where the defects are.
- `tools/gen_code.py` and `tools/gen_bodies.py` run **faithfulness gates** on
  their own output. The transcription gate has two independent checks: every instruction of every transcribed function must appear
  exactly once, in order, with matching operands; and the generated code must be
  byte-identical to what re-rendering the recovered instructions would produce.
  The second check exists because the first cannot see a corrupted *statement* —
  a mutated operand leaves its comment untouched. Both are proven by mutation:
  deleting an instruction gives `1/4 records match`, swapping two gives
  `2/4 records match`, and corrupting `push32(0x19u)` to `0x1au` with the comment
  intact gives `code does not match the recovered instructions at line 0`. All
  three exit 1. Semantic equivalence with the original cannot be proven by
  *compiling* it (there is no MSVC 6 toolchain, only g++), so faithful rendering
  is the strongest claim available from the generator alone.

  **But it can be checked against the running original, and that is new.**
  `tools/wine_run.sh` runs the retail `GAME.EXE` under Wine on this host, at its
  preferred base, headless, with `WINEDEBUG` channels. That is a differential
  oracle at instruction granularity -- the thing this file used to say could not
  exist. Wine's `+ddraw` trace is what exposed the 23-vs-25 slot `IDirectDraw`
  vtable: it listed the retail call sequence, and `GetAvailableVidMem` was a slot
  we did not have.

  **What it actually does here, verified with a screenshot:** `tools/wine_env.sh`
  gives the game a `DRIVE_CDROM` `d:` labelled `TS1` holding `TS1.DSK` and an X
  screen at `640x480x16` (the mode the game asks DirectDraw for). Measured this
  session: it dismisses the DirectSound warning, plays `WWLOGO.VQA` and
  `STARTUP.VQA`, and reaches the **main menu** (the GDI/Nod wheel), which stays
  up — the game was still running after 60 s, screenshot
  `/tmp/ts.png` at 640x480, md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`. The earlier
  claim that the oracle "gives up early" is **false** and retracted. Remaining
  oracle limits: the main-menu buttons/text are not drawn because the PCX assets
  live inside a nested MIX entry, and there is no audio. Do not read "runs under
  Wine" as "is playable under Wine", but do read it as "reaches its own menu".
- **Test the tools, not only their output.** `tools/verify.py` checks artifacts;
  a tool can be wrong in a way that produces self-consistent output and nothing
  notices. `tools/test_tools.py` holds 94 checks, each a regression test for a
  defect that shipped or a differential test against an independent
  implementation, and four of them are mutation-proven to fail when the defect
  is reintroduced. If a test cannot fail, it is not worth the runtime.
- **A gate that has never failed is not a gate.** Every gate in this repo is now
  proven by reintroducing the defect it is meant to catch and confirming it
  fails. When you add one, break the thing on purpose and watch it fail — the
  two-check transcription gate was written because a mutation showed the
  record-only check could not see a corrupted statement.
- **Closing a coverage gap finds bugs in code that was already there.** Extending
  the transcription from 2,845 functions to all 43,557 exposed six defects in the
  emitter and generator, including one that had prevented every destructor from
  ever being emitted. Widening scope is a test, not just more output.
- **Never `git stash push -u -- <path>` to get a clean tree for a test.** `git
  stash pop` does not restore the index: it unstages the paths it touched and
  leaves them reverted *in the index*, so the next `git commit` records the
  previous commit's content for those paths while the working tree holds the new
  content. This silently shipped a mixed tree twice — once leaving a 40 MB
  deleted duplicate in the repo, once shipping the pre-change generated files
  alongside their replacements. To test a clean tree, clone to a temp directory
  (`git clone --depth 1 file://$PWD /tmp/x`) and run there.
- **After every commit that touches generated files, read one back out of HEAD**
  (`git show HEAD:<path> | head`). The pipeline regenerates everything, so a
  stale committed artifact is invisible to any test that rebuilds — it only
  shows up when someone reads the repo instead of rebuilding it. `git status`
  after the push is what caught both incidents above.
- **Do not leave `__pycache__` behind.** A stale `.pyc` silently shadowed an
  edited tool during development of the tests above: the source said one thing,
  the imported module said another, and the mutation checks "passed" against the
  old code. `tools/test_tools.py` sets `sys.dont_write_bytecode = True`.

Member **offsets** are the one piece of layout that IS verified against the
binary: `tools/verify_members_binary.py` disassembles each constructor and
confirms the claimed displacement appears in a real store, at 99.9% (4,285 of
4,289, 4 refuted, 0 uncheckable). What is **not** verified is member *type* and
*ordering* — whether a 4-byte slot is really an `int` rather than a pointer, or
whether two members are the right way round. Hand-written inheritance is reported
separately in check 6, not gated, because those headers are the authoritative
richer source and legitimately omit transitively-implied bases.

**Known open defects** are listed in `todo.md` with evidence. The source-side
largest are **2,458 header virtual declarations remaining `vfunc_N`** (5,557
`(class, slot)` pairs carry a name, but only 917 names are distinct), **3,235 of
3,526 non-vptr members unnamed**, and **the transcription is not source-shaped**.

The runtime blocker is the **479B wild-call fault during the mission load**. The
campaign list works and the mission starts (GDI, from `BATTLE.INI`), and the GDI
intro movie plays. ESC skips it and the loader runs: the page table is now
dynamically growable, and the cost is the game's own O(n²) vector growth (at 100B
it is on vector 7/22 with 160,070 entries). A 1.2e12-budget run reached
478,970,534,425 instructions and faulted at `0x00422de0` with 1,024,211 /
2,097,152 pages, **7,771,444 unmapped calls** (7 in `.text`, all `0x606060` from
`sub_004f3b40`; 7,673,235 below `0x1000`; 98,202 elsewhere) and a **black
frame**. The loader got far past 100B; the fault is a new defect (a pointer
table/stack going wild), and a diagnosis lane is finding the first wild call.
The **tactical phase is not reached and no mission is rendered.** The VQA
frame-wait is stub-bound (~68M `PeekMessageA` in 600 s), so ESC is the practical
route meanwhile; it is secondary.

The shutdown ESP corruption (WinMain returns with ESP `0x11`) is **dormant, not
fixed**: the current run never reaches shutdown. Keep it queued for when it does.

The slot-0 mislabel is fixed (the generator detects shared slot-0 stubs and
declares the destructor without claiming a vtable position).

**Do not quote `VERIFICATION PASSED` as evidence of correctness beyond that.**

**P2 templates, recovered and integrated.** Three hand-written template headers
were added under `src/handwritten/` after recovering the layouts from the
binary and corroborating them across instantiations:

- `VectorClass.h` — `VectorClass<T>`, `DynamicVectorClass<T>`, `TypeList<T>`.
  Layout confirmed from ctor `0x445640` and dtor `0x445570` (offsets +4 data,
  +8 count, +0xc active, +0xd allocated gating the free; +0x10 capacity,
  +0x14 growth step default 10). Covers 309 manifest instantiations.
- `Blitter.h` — the `Blit*`/`RLEBlit*` family, 110 instantiations across 97
  template names, with the `<E>`/`<G>` pixel-format split proven from operand
  width (stride 1 vs 2).
- `Pipe.h` — the 20-class Pipe/Straw IO hierarchy with all 80 virtuals named
  from call-site evidence (LCW, LZO, PKZIP, Blowfish, SHA-1, base64).

`tools/gen_headers.py` skips manifest entries owned by those headers (536
entries skipped), maps a template name to the header that actually defines it
(includes name `VectorClass.h`/`INIClass.h`, not a nonexistent
`DynamicVectorClass.h`/`Node.h`), and forward-declares MSVC-leaked formal
template parameters (the `H` in `VectorClass<H>`) instead of inventing a type.
`tools/verify.py` credits class coverage to the template that defines an
instantiation rather than demanding a file per instantiation, which is what the
487 template-argument manifest entries require.

**The generated tree is a build artifact of the tools.** A stale
`src/include/` will fail verification even when every tool is correct; the
headers must be regenerated, never hand-edited. Run `./tools/pipeline.sh` and
let it rewrite `src/include/` rather than patching a header in place — a
hand-patch is lost on the next run.

**Known remaining gaps.** `Node<>` is still declared inside `INIClass.h` rather
than its own header, and two classes still show unresolved bases for it
(`INIClass::INIEntry`, `MixFileClass`). The MS* multimedia classes and the
WOL/ATL COM layer are not reconstructed. The template headers' members are
defined as forwarders through `ts::ts_invoke` rather than as hand-written C++,
because several of them tail-jump and the recorded span is not the whole method.

See `PROGRESS.md` and `todo.md` for the live work list.
