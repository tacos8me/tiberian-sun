# Tiberian Sun — Disassembly to Source Reconstruction

## Result

The retail PC release of **Command & Conquer: Tiberian Sun** (Westwood Studios,
1999) has been disassembled and reconstructed into a compilable C++ class
declaration tree.

**Verification status: `tools/verify.py` prints `VERIFICATION PASSED` with nine
checks.** 265 of the 266 headers under `src/include/` compile standalone at the
target ABI (`g++ -m32 -std=c++03 -fsyntax-only`; `ts_prelude.h` is the shared
prelude and is included rather than compiled on its own), and the combined
translation unit `build/tu_all.cpp` compiles. Every vtable→class binding in the
class manifest was re-verified by re-reading the binary's own RTTI Complete
Object Locator chain. Check 9 makes the compiler confirm every recovered member
offset a header can express: **3,073/3,073**.

The recovered **machine code is transcribed**, not stubbed: 45,947 functions
(43,557 recovered function starts + 2,390 tail entries), 912,879 of 912,879
instruction lines (100%, 0 unmodelled), rendered against the machine model in
`src/handwritten/Machine.h`. The whole tree — 492 translation units — **links
into a program**, and that program runs the original PE entry point against the
model: with `TS_CD` (the modelled `D:`), it reads the CD (`D:\TS1.dsk`,
`EXPAND99..00`), builds its real LANGUAGE.DLL dialogs, plays past the intro, and
**renders the real main menu**. The committed `re/out/frame.png` is 640x480, md5
`0fdf2636c2643ce2510ae67fb9ca9e26`, with **254,610 / 307,200 non-black pixels**
(title logo, GDI/Nod wheel, emblems). Against the retail screenshot
(`/tmp/retail.png`, md5 `58cb9c0b…`), **102,836 pixels differ at all (33.48%)**,
the mean absolute channel difference is **1.06/255 over the differing pixels**
(0.35 over the whole frame), and only **506 pixels differ by more than 8**, all
in the bottom-right retail version text the model does not draw; the rest is
RGB565 quantization.

**The menu is selectable, and the game leaves it.** LANGUAGE.DLL `RT_DIALOG` 226
is the menu with six `BS_OWNERDRAW` buttons: New Campaign (1559, result 1), Load
Mission (1561, 2), Multiplayer Game (1562, 3), Intro/Sneak Peek (1003, 4),
Options (1372, 5), Exit Game (1006, 6). `DLGTEMPLATEEX` is parsed and the model
performs press/release → `BN_CLICKED` in `dialog_mouse`, so the click reaches the
game's own hit-test and `WM_COMMAND` path. The default timeline clicks New
Campaign: the menu exits with result 1 and `RT_DIALOG` **148, "Select
Campaign:"** is created at step **1,688,503,557**. Clicking Exit Game instead
reaches **`ExitProcess(0)`** at `0x00684529` after **1,707,666,895** instructions.
`0x7b2ab8` (front-end-active) and `0x7b0d38` (game mode) do not change; the
selection signal is the dialog procedure's result code through `GWL_USERDATA`.

Boot states, all with **0 unmapped calls and 0 stack imbalances**:
- 20M: stop `0x00488800`, coverage **55,774 / 4,764**.
- default scripted input, 1.9B: stop `0x0045fa8e` in the Select Campaign dialog's
  modal loop, `GetMessage=7`, `PeekMessage=14,572,933`, `DispatchMessage=7`,
  coverage **78,504 / 6,191**.
- Exit Game script, 1.9B: `ExitProcess(0)` at `0x00684529` after 1,707,666,895
  instructions, `GetMessage=6`, `PeekMessage=11,216,667`, `DispatchMessage=6`,
  coverage **86,400 / 6,654**.
`boot.py --strict` exits 0 and `tools/link_test.py` passes (exit 0). A full
`./tools/pipeline.sh` ran exit 0 in the disasm-correction lane (8m51s, per
`todo.md`); it was not re-run here.

The **current blocker is the empty "Select Campaign:" listbox**. Dialog 148 is
up (135,187 370x243) with listbox 1109, Cancel, OK, Slider3, Difficulty and
Harder, but the model implements no listbox/combobox/trackbar content
(`LB_ADDSTRING`/`LB_GETCURSEL` are no-ops), so nothing can be selected and **no
mission starts**. The nested MIX (`0xa8548fd9`, 317 entries, 111 PCX) is mounted
and read, but the button art is not painted because no `WM_PAINT` reaches the
child controls; the retail also draws no buttons here.

The **retail** binary, run under Wine with the CD and a 640x480x16 screen
(`tools/wine_env.sh`), reaches its **main menu** (screenshot md5
`58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s). That is the
differential oracle: the model renders and selects the same menu and reaches the
campaign dialog.

The retail oracle also measured what `blowfish.dll` registers
(`re/out/blowfish_oracle.md`, `tools/oracle_blowfish.py`,
`tools/wine_blowfish_trace.sh`): the object is a **real Blowfish cipher the game
uses**, with 9 `SetKey(56,key)` calls and 1,586 `Decrypt(8,in,out)` calls during
startup, matching nine shipped MIX indexes. The model **implements standard
Blowfish-ECB** (`345aae7`), proven by the four standard known-answer vectors and
a byte-identical `tools/mix.py` cross-check on the shipped TIBSUN.MIX index
(`2ce3171bdbecb40e` → `110090d97904849a`, count 17, body 75,094,416). The MIX
index read is fixed as well: all 27 blocks are read and the 17 records match
`tools/mix.py` 17/17.

The recovery was corrected (`9e5703f` + `ad62afc`): **213 static switch tables
(6,388 B)** are now data (was 12), including three hole tables whose entry-1
addresses are function starts; `.text` code coverage is **95.86%**. Every
instruction that left the transcription was classified.

**What that does *not* mean is stated under [Verification](#verification)
below.** In short: the gate proves the declaration tree compiles, that the
manifest's vtable bindings match the binary, and that the tree links. It does
not prove the disassembly is correct, it does not verify data-member *types* or
ordering, and the transcription is **not source-shaped** — it is faithful to
what the machine does and is not claimed to be the shape the original source
had. Measured naming: 4,499 of 6,957 header virtual declarations are named
(64.7%; 2,458 remain `vfunc_N`), representing 5,557 named `(class, slot)` pairs
and 917 distinct names (281 semantic); 291 of 3,526 non-vptr members are named
(8.3%).

## Input

The working directory contained two raw CD images, not an installed game:

| File | Format | Size |
|---|---|---|
| `ts1.bin` / `ts1.cue` | MODE2/2352 raw CD image | 740 MiB |
| `ts2.bin` / `ts2.cue` | MODE2/2352 raw CD image | 739 MiB |

Both discs were converted to ISO 9660 (stripping the 24-byte sector headers)
and extracted. Both discs carry an identical `INSTALL/GAME.EXE`, so there is
exactly one build to reverse.

**Target:** `GAME.EXE`
- SHA256 `2941b01998da0cd2bb0b7db30125d4130e3876a74d6cc5588b630c6d5fac5a12`
  (`sha256sum bin/GAME.EXE`)
- PE32 i386, MSVC 6.0, ImageBase `0x400000`, EntryPoint `0x6854e3`
- `.text` 2,712,101 bytes (2.6 MiB), PE timestamp 1999-07-29 01:52:58 UTC
- Stripped of symbols, but **full MSVC RTTI is present**
- Original source path embedded in the binary: `D:\Projects\TSun\Code\`

## Method

### 1. RTTI recovery (`tools/rtti.py`, `tools/demangle.py`)

The binary retains complete MSVC 6 RTTI. The `type_info` layout was determined
empirically from the binary:

```
+0x00  vftable ptr -> type_info vftable (0x6a7d04)
+0x04  spare
+0x08  inline mangled name
```

A recursive MSVC name demangler was written to handle the observed grammar
(`?$Template@Arg@@`, `AV`/`AU`/`BV`/`BU` type codes, pointer/reference args).

Recovered (`python3 tools/rtti.py bin/GAME.EXE out`):
- **788** type descriptors
- **755** class hierarchies with exact base-class displacements
- **842** Complete Object Locators
- **843** vtables bound to their owning class

### 2. Disassembly (`tools/disasm.py`)

Function boundaries were seeded from the entry point, vtable entries, and MSVC
prologue patterns, then extended by call-graph traversal with seed-boundary
termination, plus a gap-recovery pass over already-recorded branch targets.

Recovered (`python3 tools/disasm.py bin/GAME.EXE out`):
- **43,557** functions/spans, covering **2,599,700 bytes = 95.86%** of `.text`
  (`python3 tools/harness.py metrics --no-append`)
- **845,093** static instructions
- Per-function callees and referenced strings
- **112,401 B (110 KB)** of `.text` remains unattributed, classified in
  `out/text_data.json` as **213 static switch tables (6,388 B)**, 102 switch
  index tables (145 B), 105,492 B of alignment padding, and 376 B in 46
  unclassified runs; 99.99% of `.text` is attributed as code or data

Function sizes are measured as merged intervals, not summed per-function sizes:
**1,000** entry points sit inside another recovered body (`shared_tail_entry_points`)
and would be double-counted otherwise.

### 3. Schema recovery

**1,876** strings referenced by code were extracted (`out/ini_keys.json`), of
which **1,319** match the `CamelCase` identifier shape used for INI keys
(`TiberiumChainReaction`, `HalfDamageSmokeLocation1`, `Capturable`,
`Unsellable`, `CustomRotor`, ...). These are the original struct member names,
recovered verbatim.

```
python3 tools/harness.py metrics --no-append
#   ini_keys          1876
python3 tools/gen_strings.py bin/GAME.EXE out
```

`out/ini_keys.json` maps each string to the function VAs that reference it.
Only a fraction is yet mapped onto declared members (see Known gaps).

### 4. Source reconstruction

Class headers are emitted from a per-class manifest (`tools/gen_manifest.py`)
and deepened by three further recovery passes:

- **member offsets** from constructor store sequences
  (`tools/gen_members.py`, `tools/name_members.py`): **4,289** member-offset
  records recovered (763 vptr stores, 3,526 real members), **291** named across
  20 classes where an INI key or string provides evidence;
- **calling conventions and arity** (`tools/gen_signatures.py`): **43,557**
  signatures (`__thiscall` 11,478, `__stdcall` 10,728, `__cdecl` 9,642, and
  11,716 with no convention recovered);
- **virtual naming** (`tools/name_virtuals.py`): **4,499** named virtual
  declarations (64.7% of the 6,957 the harness counts) and **5,557** named
  `(class, slot)` pairs, from **917** distinct names (**281** semantic). The
  remaining **2,458** declarations are honestly named `vfunc_N` rather than
  guessed. (`tools/harness.py metrics` reports `virtuals_named` and
  `virtuals_unnamed`; its `virtual_names: 702` key counts classes with a naming
  record, not names.)

Every declaration cites its evidence (RTTI name, vtable VA, constructor VA,
INI key, or function VA). The `.cpp` bodies are no longer empty: every recovered
function is transcribed in `src/generated/code/` and every header declaration
has a forwarder in `src/generated/` (see [Verification](#verification)).

## Deliverables

```
re/
  bin/GAME.EXE              the target binary
  tools/                    recovery + generation tooling
  tools/pipeline.sh         one-command full regeneration + verification
  tools/harness.py          metrics + determinism harness
  out/classes.json          755 classes, 1291 edges, 842 locators, 843 vtables
  out/hierarchy.txt         readable class hierarchy
  out/vtables.txt           vtable -> class map (843 entries)
  out/functions.json        43,557 functions with 845,093 instructions, callees, refs
  out/ini_keys.json         1,876 referenced strings -> referencing functions
  out/class_manifest.json   749 classes, 9,810 virtual slots
  out/members.json          4,289 member-offset records
  out/signatures.json       43,557 signatures
  src/include/*.h           266 headers (265 standalone-compilable + prelude)
  src/generated/code/       45,947 machine-code transcriptions + registration
  src/generated/*.cpp       6,962 forwarders across 310 translation units
  src/handwritten/          hand-written, evidence-backed headers
  src/include/SCHEMA.md     1374-row INI key -> class -> type table
  src/include/INDEX.md      core hierarchy index
  src/include/LOCOMOTION_INDEX.md
  src/include/MAP_INDEX.md
  build/tu_all.cpp          combined compile test
  build/ts_link_test.cpp    links the whole tree and runs the entry point
  build/shim/               minimal windows.h + shared prelude
```

`src/include/` is a **build artifact of the tools**: `tools/pipeline.sh`
deletes it and regenerates it from `bin/GAME.EXE`, restoring
`src/handwritten/` on top (the hand-written set always wins). Never hand-edit
a header under `src/include/`.

## Verification

`python3 tools/verify.py src/include build/shim` runs nine checks and is
fail-closed: a missing artifact under `out/` makes it exit non-zero naming what
is missing, rather than running a subset and printing `VERIFICATION PASSED`.
Current output:

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

**How much each check is worth:**

| Check | What it proves | Anchored to |
|---|---|---|
| 1 | every header parses standalone | the compiler |
| 2 | the whole set is self-consistent C++ | the compiler |
| 3 | every manifest class has a defining header | the manifest |
| 4 | every manifest vtable re-resolves to its class through the binary's RTTI COL chain | **the binary** |
| 5 | each header's cited vtable VA and per-slot targets match the manifest | manifest output |
| 6 | emitted base clauses match `out/direct_bases.json` | tool output |
| 7 | emitted member offsets match `out/members.json` | tool output |
| 9 | every recovered member offset a header can express is confirmed by `offsetof` at `-m32` | **the compiler** |

Only **checks 4 and 9** are anchored outside the tool chain: check 4 reads the
target binary, check 9 makes the compiler adjudicate the layout. Checks 5–7
compare the generated tree against other tools' output, so they detect generator
regressions and header/manifest disagreement, but they cannot detect an error
common to a recovery tool and the generator that consumes it. Check 3 has the
same limitation. `tools/verify_members_binary.py` is a separate binary-anchored
gate: 4,285 of 4,289 claims confirmed (99.9%, 4 refuted).

**Explicit non-claims:**

- **The disassembly is not verified.** No check re-decodes function boundaries
  against an independent source. Every downstream artifact inherits any
  boundary error in `tools/disasm.py`.
- **Data-member layout is verified against the compiler, not against the
  original source.** Check 7 compares 1,317 emitted offsets across 206 classes
  against `out/members.json` (itself produced by `tools/gen_members.py`), and
  check 9 makes the compiler confirm 3,073/3,073 expressible offsets at `-m32`.
  `tools/verify_members_binary.py` independently confirms 4,285 of 4,289 claims
  against stores in the binary's constructors. What is *not* verified is member
  *type* and *ordering* — whether a 4-byte slot is an `int` rather than a
  pointer, or whether two members are the right way round.
- **The declaration tree alone does not reproduce behaviour** — but it is no
  longer alone. `src/generated/code/` transcribes the whole recovered
  instruction stream and `tools/link_test.py` links and runs it; what the
  declaration tree proves on its own is that the declarations are
  self-consistent.
- **A link is the only check that can see an undefined symbol.** The tree did
  not link for months because every check compiled one file at a time. Adding
  the link found six defects, including one that had prevented every destructor
  in the project from ever being emitted.
- Check 6 now reports 220/220; the four divergences were resolved.

Evidence density in the reconstructed headers, measured this session: **10,288**
address citations, **473** quoted INI-key citations, **378** RTTI citations.
**2,458** virtual declarations could not be semantically identified and are
named `vfunc_N` rather than guessed (126 distinct `vfunc_N` numbers);
**1,811** items are explicitly marked `/* ? */`.

```
grep -rhoE '0x[0-9a-fA-F]{6,8}' src/include/*.h | wc -l            # 10288
grep -rhoE 'INI "[^"]+"'        src/include/*.h | wc -l            # 473
grep -rho  'RTTI'               src/include/*.h | wc -l            # 378
grep -rhoE 'vfunc_[0-9]+'       src/include/*.h | sort -u | wc -l  # 126
grep -rhoF '/* ? */'            src/include/*.h | wc -l            # 1811
grep -rh 'virtual' src/include/*.h | grep -c 'vfunc_'              # 2458
```

## Known gaps

- **The transcription is not source-shaped.** 45,947 functions are transcribed
  faithfully against a machine model, but a transcription is not a
  reconstruction: it is what the machine does, not the shape the original
  source had. 0 instructions have no faithful rendering
  (`grep ts::unmodelled src/generated/code` returns nothing).
- **229 destructors are labelled stubs.** The headers declare them; no comment
  records where they are, so there is no transcription to forward to.
- **The entry-point run does not stop at import thunks.** `Image.h` maps every
  section's raw extent and writes a stub VA into all 341 IAT slots;
  `link_test.py` reports `341 of 341 IAT slots written` and `0 with no stub`.
  The old "16,383 calls read 0" claim described the pre-`Image.h` model and is
  retracted.
- **The model renders and selects the main menu, and opens "Select Campaign:".**
  The frame is `re/out/frame.png` (md5 `0fdf2636…`, 254,610 / 307,200 non-black;
  versus retail, 102,836 pixels differ (33.48%), mean absolute channel
  difference 1.06/255 over the differing pixels, 506 pixels >8). New Campaign
  exits the menu with result 1 and creates `RT_DIALOG` 148 at step 1,688,503,557;
  Exit Game reaches `ExitProcess(0)` after 1,707,666,895 instructions. The
  nested MIX (`0xa8548fd9`, 317 entries) is mounted and read.
- **The "Select Campaign:" listbox is empty.** No listbox/combobox/trackbar
  content is implemented (`LB_ADDSTRING`/`LB_GETCURSEL` are no-ops), so no
  campaign can be selected and **no mission starts**. The button art is not
  painted (no `WM_PAINT` to children; retail does not draw it either).
- **The boot has 0 unmapped calls and 0 stack imbalances**; the link test passes
  and `boot.py --strict` exits 0.
- **The Blowfish cipher, the MIX index read and the dialog layer are fixed.**
  All 27 index blocks are read (17/17 records match `mix.py`), and the game
  builds its real LANGUAGE.DLL dialogs. The CD path is correct with `TS_CD`.
- **Shutdown corrupts ESP, but the path is dormant.** WinMain returned with ESP
  `0x11` before the MIX-read fix; the current run never reaches shutdown.
- **Member layout is verified where expressible; types and ordering are not.**
  Check 9 confirms 3,073/3,073 offsets at `-m32`, and the binary-anchored
  verifier confirms 4,285 of 4,289; member *types* and ordering are not checked.
- **1,876 referenced strings are recovered, but only 291 members across 20
  classes are named from them** (8.3% of 3,526 non-vptr members).
- **`RulesClass` has no RTTI record** (it is not polymorphic); it is identified
  only by its reader functions, so its member offsets are not isolated.
- **`Node<>` is still declared inside `INIClass.h`** rather than in its own
  header.
- **The MS\* multimedia classes, the WOL/ATL COM layer, and the nested gadget
  classes are incomplete.** `SidebarClass::StripClass` and
  `RadarClass::RTacticalClass` have constructors located but their member
  offsets are not isolated.
- **Member offsets are exact where recovered; names only where an INI key or
  string provides evidence.** Do not infer a name.
- **`out/functions.json` is 67,670,646 bytes (~65 MB)**, over GitHub's 50 MB
  warning threshold. It is a regenerable artifact; untracking it would need a
  history rewrite. (The
  stray `out/out/` duplicate that sat in the repo for months has been removed,
  and `tools/pipeline.sh` now fails loudly if it reappears.)

## Reproducing

The whole reconstruction is one reproducible command:

```bash
cd re
./tools/pipeline.sh              # regenerate everything from bin/GAME.EXE, then verify
./tools/pipeline.sh --verify-only   # verify the current tree
```

Individual stages, in order (see `tools/pipeline.sh` for the exact sequence):

```bash
python3 tools/rtti.py bin/GAME.EXE out          # RTTI recovery
python3 tools/disasm.py bin/GAME.EXE out        # disassembly
python3 tools/gen_strings.py bin/GAME.EXE out   # referenced strings / INI keys
python3 tools/gen_manifest.py bin/GAME.EXE out  # per-class vtable/ctor
python3 tools/reduce_bases.py out               # direct-base reduction
python3 tools/gen_members.py bin/GAME.EXE out   # member offsets
python3 tools/name_members.py bin/GAME.EXE out  # member naming from INI keys
python3 tools/gen_signatures.py bin/GAME.EXE out
python3 tools/name_virtuals.py bin/GAME.EXE out
python3 tools/gen_headers.py bin/GAME.EXE out src/include
python3 tools/reconcile_prelude.py src/include
python3 tools/make_selfcontained.py src/include build/shim
python3 tools/verify.py src/include build/shim
```

Measure the current state, or check pipeline determinism:

```bash
python3 tools/harness.py metrics --no-append   # all figures quoted above
python3 tools/harness.py determinism --run     # two clean runs must be byte-identical
```
