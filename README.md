# Tiberian Sun — Disassembly to Source

A reverse-engineering project that takes the retail PC release of
**Command & Conquer: Tiberian Sun** (Westwood Studios, 1999) and reconstructs
it as compilable, documented C++ source — and, increasingly, executes the
original program's own instruction stream against a machine model.

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
- **The menu is not selectable yet.** Scripted input is delivered and consumed,
  but exit needs `0x5898f0` to return 1 (`[0x7b0d38]` nonzero and != 5, currently
  0) and `0x7b2ab8` has only its menu-init store; the button/text PCX assets are
  in the nested MIX entry `0xa8548fd9` (317 entries), so the click points are
  probes. The game does **not** leave the menu.
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
| tool tests | 75 checks, several mutation-proven; `test_runtime.py` 101 more (rendering/input) |
| boot (`TS_CD`) | message loop reached and dispatching; real Blowfish-ECB; MIX/CD path correct; real LANGUAGE.DLL dialogs; **renders the main menu**; 20M stop `0x00488800` (55,774 / 4,764), 1.65B no input `0x0045fa8b` (77,619 / 6,132), 1.9B scripted `0x0055f78e` (77,982 / 6,150); 0 unmapped, 0 stack imbalances; current blocker is menu selection |
| frame | `re/out/frame.png`: 640x480, md5 `0fdf2636c2643ce2510ae67fb9ca9e26`, 254,610 / 307,200 non-black; 1.06/255 mean channel difference from retail on differing pixels |

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

At larger budgets the model reaches the menu: no input at `--budget 1650000000`
stops at `0x0045fa8b` (77,619 / 6,132); the default scripted input at
`--budget 1900000000 --timeout 600` stops in the menu message pump `0x0055f78e`
with `GetMessage=11`, `PeekMessage=15,207,219`, `DispatchMessage=11` (77,982 /
6,150). `--frame out/frame.png` dumps the rendered menu; the committed frame is
640x480, md5 `0fdf2636c2643ce2510ae67fb9ca9e26`, with 254,610 / 307,200
non-black pixels, and differs from the retail screenshot by a mean channel
difference of 1.06/255 on the pixels that differ.

Real Blowfish-ECB is implemented and proven against `tools/mix.py` (the shipped
TIBSUN.MIX first index block decrypts byte-identically), the MIX index read is
fixed (27/27 blocks, 17/17 records), the dialog layer reads LANGUAGE.DLL's own
templates, and narrow-width flags are derived from the operand width. The menu
renders but is **not selectable**: `0x5898f0` needs `[0x7b0d38]` nonzero and
!= 5 (currently 0), `0x7b2ab8` has only its menu-init store, and the button PCX
assets are in the nested MIX entry `0xa8548fd9`, so the scripted clicks are
probes. The game does not leave the menu. The shutdown ESP defect is dormant.

The model has diagnostics for this work: `TS_TRACE=0xADDR`, `TS_PEEK=0xADDR`,
`TS_SCAN=start,count`, `TS_WATCH=0xADDR`, `TS_FILE_TRACE=1`, plus `bad_calls`,
`stack_imbalances` and a per-callee ESP tracker. "What is ESP here?" is answered
by running the machine, not by counting pushes by hand.

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
