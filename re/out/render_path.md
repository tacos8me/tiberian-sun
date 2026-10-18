# The mission render path: why no mission frame is presented

Measured 2026-10-18 on main `eb151bf` (page table growable), in the lane
worktree. Every claim below is a boot-report line, a gdb breakpoint hit, a
Ghidra decompile, or a decoded frame. The raw logs are named where each fact
comes from.

## Answer

The game **does** present a frame after the mission load starts: the GDI mission
**loading screen** (mech artwork, "ANALYZING COMBAT ZONE. TOPOGRAPHY..."), fully
drawn at 2.6e9 instructions and unchanged at 2.7e9.

It never presents a **mission** frame for three independent reasons:

1. **The framebuffer is destroyed in place.** The model's Win32 stub heap is an
   unbounded bump allocator starting at `0x50000000` (`Win32Stubs.h:80`). The
   DirectDraw surface pixel arena starts at `0x60000000` (`Rendering.h:116`).
   When the heap cursor crosses `0x60002000` (the primary surface's first
   pixel) at about **2.722e9** instructions, the load's heap traffic overwrites
   the primary surface pixel-for-pixel. The blue banded/grid frame is that
   overwrite: raw heap memory read as RGB565. It is not a loading screen, not
   an uninitialised surface, and not a partially drawn mission.
2. **The model is too slow to finish the load.** The isometric-map build
   (`DynamicVectorClass::Grow` at `0x0055b050` / `0x0055b09a`) completes around
   4.0e9 instructions, and the game then enters the tactical draw
   (`0x005d9990` -> `0x00593f30`). At **4,074,750,319 instructions** the
   900-second wall-clock watchdog fires: the model's throughput has fallen from
   ~26M instructions/s (at 4e9, 1.5 GB) to ~4.5M/s (at 4.07e9, 2.9 GB, 722,815
   page-table slots). The run is stopped inside a per-cell draw loop, not
   inside the loader.
3. **The tactical phase produces a black primary, 21 unmapped calls, and a
   context of coordinate/delta values.** Between 4.0e9 and the watchdog stop
   the game issues 3 more blits and 7,328 more locks (12 -> 15 blits, 769 ->
   8,097 locks) and the primary frame becomes **0 non-black pixels**. The same
   interval produces **21 unmapped calls** (2 targets below `0x1000`, 19
   elsewhere; 0 inside `.text`) that are absent at 4e9. A longer run (5.14e9
   actual instructions) shows the machine afterwards in `sub_00675ba0` with
   `ESP=0x00000844`, `ESI=0x000008ec`, `EDI=0x000008fc` and
   `EAX=0xffffff6c`: values that are coordinates and map deltas rather than
   pointers (whether the low ESP is a deliberate stack switch or corruption is
   not established). It adds no new coverage and no render calls for 1.07e9
   instructions. The unmapped targets are sliding 4-byte windows over a
   small-integer delta table (`0x000000db`, `0xffffff6a`..`0xffffff6d`), not
   code addresses. No mission pixels are presented at either stop.

A measurement defect also surfaced: `Machine::steps` is a 32-bit
`unsigned long`, and `boot_driver` clamps a budget above `0xffffffff` to
`4294967295` while `insn()` tests `steps > step_limit`. A budget at or above
`2^32` can therefore never fire, and the reported step count is modulo
`2^32`. The "4.5e9" run reports 845,440,651 after actually executing
**5,140,407,947** (`4,294,967,296 + 845,440,651`); only the watchdog stopped
it. Budgets below `2^32` (the 4e9 run) are unaffected.

The DirectDraw wiring is correct: the game never calls `Flip` (neither does the
retail binary), it presents by locking the primary and writing it, and the
model's slots and counters are the ones being called. The fix is an
address-space fix, not a rendering fix.

## 1. Measured timeline

All runs: `tools/input_scripts/esc_skip.txt`, `TS_CD` = CD1, `TS_ROOT` =
gamedir, `--frame` dump of the primary surface. The boot report's
`render:` line is the model's own counter set (`Rendering.h:246-249`).

| budget | stop | render counters | primary frame | non-black | stub-heap bump |
|---|---|---|---|---|---|
| 2.6e9 | `sub_0055b09a` (vector copy) | 8 surfaces, 12 blits, 0 fills, 769 locks, 0 flips | `out/render_2_6b.png` md5 `6cb0e1fff05f9250adda298e644f1266` | 290,399 | — |
| 2.7e9 | same | same | byte-identical to 2.6e9 (md5 `6cb0e1ff…`) | 290,399 | `0x5eaa1e90` |
| 2.7227e9 | same | same | `out/render_partial.png` md5 `a5440448c1f4fc057631a9280ec4ac80` | 197,813 | `0x60053760` |
| 3.2e9 | same | same | `out/render_3_2b.png` md5 `ab01ed73fe1682f3ceab242c4422416a` | 191,093 | `0x7c6a2b20` |
| 4.0e9 | `0x0055b0b8` `sub_0055b09a +0x1e` | same | byte-identical to 3.2e9 (md5 `ab01ed73…`) | 191,093 | (dest `0xac0431e0`) |
| 4.0747e9 | **watchdog** at `0x005940d8` `sub_00593f30 +0x1a8` (900 s) | 8 surfaces, **15 blits**, 0 fills, **8,097 locks**, 0 flips | `out/render_6b.png` md5 `fab3ec8fbf7b5c7de8041b90a78c6c31` | **0** | (722,815 pages) |
| 5.1404e9 actual (request 4.5e9, clamped, reported 845,440,651) | **watchdog** at `0x00675c35` `sub_00675ba0 +0x95` (1800 s) | same 15 / 0 / 8,097 / 0 | byte-identical to the 6b frame | **0** | (722,815 pages) |

Three facts fall out of the table:

* **The pre-tactical load issues no DirectDraw calls after the loading
  screen.** The counters are identical at 2.6, 3.2 and 4.0e9: no `Blt`,
  `Lock`, `Unlock` or `Flip` occurs between the loading screen and the end of
  the iso-map build.
* **The frame changes at the heap crossing, freezes, then changes again in the
  tactical phase.** 2.6e9 and 2.7e9 are byte-identical (the loading screen);
  3.2e9 and 4.0e9 are byte-identical (the heap pattern); 4.07e9 is all black
  after the tactical draw begins.
* **The tactical phase is where the wall clock runs out.** The 6e9-budget run
  stops at 4.0747e9 by watchdog after 900 s, with the page table at 722,815
  slots and 2.9 GB RSS. The instruction rate over that run is ~4.5M/s, against
  ~26M/s for the 4e9 run (152 s, 377,796 slots).

Commands (logs: `/tmp/render_gdb2.log`, `/tmp/render_state*.log`):

```
TS_CD=…/work/cd1 TS_ROOT=…/gamedir python3 tools/boot.py --no-build \
  --budget 2600000000 --timeout 600 \
  --input-script tools/input_scripts/esc_skip.txt --frame out/render_2_6b.png
```

The 4e9 report:

```
stop:   instruction budget exhausted at 0x0055b0b8 after 4000000001 instructions, depth 29
  in:   0x0055b0b8  sub_0055b09a +0x1e  (37 instructions, 102 bytes, src/generated/code/code_0091.cpp:7860)
  regs: eax=0x0000719d ecx=0x00007cb0 edx=0x000000bd ebx=0xffffff48
        esp=0x002ff07c ebp=0x00007cba esi=0x00711fd8 edi=0xac0431e0
render: 8 surfaces, 12 blits, 0 fills, 769 locks, 0 flips
pages:  377796 of 524288 slots (peak 377796, growths 4, refused 0)
```

`EBP=0x7cba` (31,930) is the new vector capacity, `ECX=0x7cb0` (31,920) the
count being copied, `EAX=0x719d` (29,085) the entry reached, `ESI=0x00711fd8`
the vector object, `EDI=0xac0431e0` the heap destination.

## 2. The eight surfaces

Read from `ts::rnd::surface_table()` at the frame dump (gdb, `render_state.gdb`;
`SURF[i]` layout from `Rendering.h:207-232`):

| # | COM object | pixels | size | pitch | role |
|---|---|---|---|---|---|
| 0 | `0x5060c450` | `0x60000000` | 64x64 | 128 | small offscreen |
| 1 | `0x5060c470` | `0x60002000` | 640x480 | 1280 | **primary** |
| 2 | `0x5060c4e4` | `0x60098000` | 472x480 | 944 | offscreen |
| 3 | `0x5060c504` | `0x60106a00` | 472x480 | 944 | offscreen |
| 4 | `0x5060c524` | `0x60175400` | 168x480 | 336 | offscreen |
| 5 | `0x5060c544` | `0x6019ca00` | 640x480 | 1280 | movie/loading surface |
| 6 | `0x5060c564` | `0x60232a00` | 640x480 | 1280 | offscreen |
| 7 | `0x53bfd5c0` | `0x602c8a00` | 67x108 | 134 | small offscreen |

All are 16bpp RGB565 (`R 0xf800 G 0x07e0 B 0x001f`). The arena ends at
`ts::rnd::mem_bump() = 0x602cc288`.

The seven `CREATE` lines gdb sees come from `ts::rnd::create_simple`; the eighth
surface (`0x53bfd5c0`) is created through a translation unit's own inline copy
of the same function, which the global breakpoint does not cover. This is a
gdb/symbol artifact, not a second allocator: all eight are in the one
`surface_table()` (`surface_n() = 8`, boot report `8 surfaces`).

## 3. What the game actually calls

### Blits: 12 in the load phase, 3 more in the tactical phase

Every one of the 12 is identical in shape (gdb `render_callers3.log`):

```
BLT dst=0x5060c470 drect=0x2ff374 src=0x5060c544 srect=0x2ff324 flags=0x1000000 fx=0
  #5 ts::sub::sub_00488029()
  #9 ts::sub::sub_00487cf0()
```

* destination = the **primary** `0x5060c470`;
* source = the 640x480 offscreen `0x5060c544`;
* `flags=0x01000000` = `DDBLT_WAIT`, no colour key, no colour fill;
* the call site is `0x0048823d call dword ptr [edx+0x14]` — **slot 5, Blt**
  (`src/generated/code/code_0037.cpp:4765`), inside the blit helper
  `sub_00488029` (`code_0037.cpp:4600`), called from the virtual
  `sub_00487cf0` (`code_0037.cpp:4256`).

The helper first calls `[edx+0x74]` (slot 29, `SetColorKey`) at `0x00488044`,
then `Blt`. The model's `k_sfs_Blt` is six words (`Win32Stubs.h:4936`), matches
the game's push count, and increments `stat_blits()` only after `find(dst)`
succeeds (`Rendering.h:557-561`). The count is real.

### Locks: 769 in the load phase (8,097 by the tactical phase)

Every lock goes through the game's own wrapper `sub_00487ad0`
(`code_0037.cpp:3944`) -> `IDirectDrawSurface::Lock` (slot 25); the caller
above the wrapper is the software surface copy/present routine
`sub_00675dc0` (`code_0162.cpp:719`, reached through `sub_00675d70`). A gdb
census at 2.7227e9 (`lockdist.log`) sees 762 of the 769:

| surface | locks |
|---|---|
| `0x5060c470` primary | 251 |
| `0x5060c544` 640x480 offscreen | 507 |
| the four other offscreens | 1 each (creation) |

The remaining 7 go through a TU-local inline copy of `ts::rnd::lock` that the
global breakpoint does not see; the boot counter (769) is authoritative.

This is the game's presentation path: it locks the offscreen to decode/draw a
frame, then locks the **primary** and writes it. That is why a frame can be
dumped at all, and why "no frame is presented" is not a missing `Flip`.

### Fill: 0 in this run; the game uses `Blt` colour-fill, not a Fill method

`stat_fills()` is incremented by `fill_rect_obj` (`Rendering.h:496`), which is
reached from `Blt` when `flags & DDBLT_COLORFILL` (`Rendering.h:563-573`). The
model's 0 fills means the game issued **no colour-fill `Blt`** in this run. The
retail trace does issue them (15 with `flags 0x1000400` before the menu), so
this is a real behavioural divergence, but not the blocker: the menu frame the
model renders is within 1.06/255 of retail on the pixels that differ
(`AGENTS.md`), so the missing fills do not leave a visible hole on this path.

### Flip: 0, and that is correct

`k_sfs_Flip` is wired at slot 11 (`Win32Stubs.h:4893`) and its body increments
`stat_flips()` (`Rendering.h:664`). The gdb breakpoint on `ts::rnd::flip` is
**never hit** in the 4e9 run. The retail trace has **0** `ddraw_surface1_Flip`
calls in 9,854 trace lines. The game presents by `Blt`/`Lock` into the primary,
never by flipping a back buffer. 0 flips is the game's behaviour, not a missing
slot or an uncounted call.

## 4. The blue banded/grid frame is heap memory, not a screen

Three measurements pin it:

1. **The boundary moves with the heap cursor.** At 2.7227e9 the stub-heap bump
   is `0x60053760`. The primary starts at `0x60002000` with pitch 1280, so the
   heap has consumed `(0x60053760 - 0x60002000) / 1280 = 260.7` rows.
   `out/render_partial.png` shows the heap pattern down to row ~259, a black
   zeroed block, and the untouched loading screen from row ~270 down. At
   2.7e9 (bump `0x5eaa1e90`, below the arena) the frame is the intact loading
   screen. At 3.2e9 (bump `0x7c6a2b20`, past the whole arena) it is all heap.
2. **The pixels are heap words.** Decoding the PNG (`out/render_3_2b.png`)
   gives RGB565 values of the form `0x0000`, `0xFFFF`, `0xFFE0`, and small
   integers — block headers, sizes and pointers, not image data. The vertical
   stripe/grid period is the 8-byte entry stride of the vector being copied
   (`0x0055b0be`: `mov ebx,[edx+eax*8-8]`).
3. **It freezes because the game stops drawing.** 3.2e9 and 4.0e9 are
   byte-identical, and no DirectDraw call occurs between 2.6e9 and 4.0e9. The
   primary is abandoned holding whatever the heap last wrote there until the
   tactical draw begins.

The loading screen itself is real and correct: `out/render_2_6b.png` shows the
GDI mech, the status panel ("ANALYZING COMBAT ZONE. TOPOGRAPHY...", "GATHERING
INTEL ON INVOLVED FACTIONS..."), and the GDI emblem — the game's own mission
loading screen. The 2.7e9 dump is byte-identical to it (md5 `6cb0e1ff…`).

## 5. Why the load is still running at 4e9

Ghidra decompiles of the load chain (`0x005c3e80` -> `0x005c4200` ->
`0x005a1a50` -> `0x005a35b0`):

* `FUN_005c4200` is the mission init ("Clearing old scenario",
  `0x006d77f4`); it calls `FUN_005a1a50()` (transcribed `sub_005a1a50`,
  `code_0109.cpp:647`) among the load stages, and updates a progress screen
  through `FUN_005d5cc0(0x1e … 0x62)`.
* `FUN_005a1a50` creates a new 640x480 surface (`FUN_00487310`) and calls
  `FUN_005a35b0()` (`sub_005a35b0`, `code_0109.cpp:4457`).
* `FUN_005a35b0` walks **22 layer vectors** at `param_1 + 0x1280` (24 bytes
  apart) and appends 8-byte `(x, y)` entries. Each vector grows by **10**
  entries at a time (`DynamicVectorClass`), so reaching ~32,000 entries costs
  ~3,200 reallocations, each copying the whole vector: O(n²).
* The grow itself is `sub_0055b050` (`code_0091.cpp:7804`) -> `malloc` at
  `0x00682997` -> copy loop `sub_0055b09a` (`code_0091.cpp:7860`, the loop at
  `0x0055b0b8`-`0x0055b0ce`) -> `free` at `0x0068298c`.

The 4e9 stop is the **second** layer vector (`0x00711fd8` = the first layer's
`0x00711fa8` + 0x30). The first layer's first >20,000 capacity grow was hit at
2.7227e9 (`BIGVEC capacity=20010 esi=0x711fa8`, `render_callers4.log`). The
cost is the game's own O(n²) vector growth executed one machine instruction at
a time by the model; it is not a stub defect and not a leak. This is the
"mission load is dominated by vector-growth reallocations" entry in `todo.md`.

**The iso-map build ends by ~4.0e9.** Between the 4.0e9 stop and the 4.0747e9
watchdog stop the game leaves the vector loop and enters the tactical draw:

* `FUN_005d9990` (transcribed `sub_005d9990`, `code_0121.cpp`) calls
  `FUN_00593f30` (`sub_00593f30`, `code_0105.cpp:6139`);
* `FUN_00593f30` loops over the visible tactical-map cell counts at
  `param_1+0x1514/0x1518/0x151c` and calls the shape blitter `FUN_00478f00`
  for each, then calls `FUN_005a0b90`;
* the watchdog EIP `0x005940d8` is inside one of those per-cell loops
  (`sub_00593f30 +0x1a8`), at call depth 100.

In that interval the render counters move for the first time since 2.6e9:
**15 blits** (was 12), **8,097 locks** (was 769), and the primary frame is
**all black** (0 non-black). The same interval produces **21 unmapped calls**
(2 below `0x1000`, 19 elsewhere; 0 inside `.text`) that are not present at
4e9.

A second, longer run (`--budget 4500000000`, clamped to `0xffffffff`, watchdog
1800 s) actually executes **5,140,407,947** instructions (the reported
845,440,651 is modulo 2^32) and stops in `sub_00675ba0` (`0x00675c35`, the
surface blitter) with:

```
regs: eax=0xffffff6c ecx=0x000000a8 edx=0x00000a00 ebx=0x0000000c
      esp=0x00000844 ebp=0x00000a00 esi=0x000008ec edi=0x000008fc
```

`ESP=0x844`, `ESI=0x8ec`, `EDI=0x8fc` are not stack or object addresses, and
`EAX=0xffffff6c` is one of the unmapped targets' values. The 16 sampled
unmapped targets are sliding 4-byte windows over a delta table:

```
0xffff6900 0x00dbffff 0x6a000000 0xffffff6a 0x0000dbff 0xff6b0000
0xdbffffff 0x000000db 0xffff6c00 0x00dbffff 0x6d000000 0xffffff6d
0x0000dbff 0xff6e0000 0xdbffffff 0x000000db
```

The most economical explanation is the same address-space collision: after the
heap cursor passed the arena, `heap_release` can put a freed block that lies
inside `0x60000000..0x602cc288` on the free list, and `heap_take` can hand it
to a later game object. Surface writes then overwrite that object's vtable or
fields, so the game reads map deltas as a vtable and calls them. The
alternative is an independent binding defect in the tactical draw. The test
that distinguishes them is 7a followed by the same run: if the 21 unmapped
calls disappear, they were the collision.

## 6. Retail comparison

`tools/oracle_render_retail.sh` (new, this lane) runs the retail `GAME.EXE`
under Wine with `WINEDEBUG=+ddraw`, dismisses the startup dialogs, and captures
a screenshot at each stage. The DirectDraw contract measured through the
startup movies and the main menu (`/tmp/retail_render2/ddraw.log`, 9,854
lines, 597 `surface_update_frontbuffer` calls):

| call | retail | model (4e9) |
|---|---|---|
| `IDirectDrawSurface::Blt` | **250** (15 colour-fill `0x1000400`, 235 copies `0x1000000`) | 12 |
| `IDirectDrawSurface::Lock` | **651** | 769 |
| `IDirectDrawSurface::Flip` | **0** | 0 |
| `CreateSurface` | 7 | 8 (the model creates one extra small surface) |

The retail copies are `dst (0,40) src=015F0FF0` x220 (movie frames into a
640x400 letterbox), `dst (0,0) src=015F0FF0` x14, and one self-blit; the
colour-fills are 14 `dst (0,0) src=00000000` plus one UI fill at
`(169,234)`. Presentation is `Blt` to the primary `015F02B8` followed by
`ddraw_surface_update_frontbuffer`. **No Flip anywhere.**

The counts are not stage-matched: the retail run reached the title/menu only,
while the model run went on through the campaign dialog, the GDI intro and into
the mission load. The qualitative contract is what matters and it matches:
blit/lock into the primary, no Flip.

**Retail input limit.** The retail cannot be driven past its title/menu with
the tooling here. `xdotool` clicks at all six measured menu button rectangles
(`(320,220) … (320,395)`), a `windowfocus`/`windowraise` first, and
`Return`/`Down+Return` all left the screen **byte-identical** (md5
`58cb9c0b…`) across 8 probes (`/tmp/retail_probe2`). The menu is a custom
DirectDraw screen with no child windows (`xwininfo`: the game window has one
640x480 child), so the synthetic events do not reach its hit-test. The
`--trace ddraw` comparison above is therefore the retail evidence available
for this lane; a retail mission-load trace needs a DirectInput-capable driver,
which does not exist in `tools/`.

## 7. The fix

### 7a. Framebuffer vs stub heap (the blue-frame defect)

The surface arena must not live in the stub heap's growth path. Two exact
options; either one fixes it.

**Option A (one line, `Rendering.h:116`). Put the arena below the heap.**

```cpp
// Was 0x60000000, which is inside the stub heap's unbounded growth path
// (Win32Stubs.h alloc_block starts at 0x50000000 and only moves up). The
// mission load's heap crossed 0x60002000 at ~2.722e9 instructions and
// overwrote the primary surface. 64 surfaces of 640x480x4 end at
// 0x14b00000, still below HEAP_BASE = 0x50000000.
inline u32 &mem_bump() { static u32 b = 0x10000000u; return b; }
```

`0x10000000` is clear of every other model region: image `0x400000..0x7b2e50`,
model stack `0x2ff000..0x300000`, import-thunk wrappers `0x00a00000`, stub heap
`0x50000000+`, synthetic stub VAs `0x7f000000+`. Worst case
`MAX_SURFACES(64) * 640*480*4 = 0x4b00000` ends at `0x14b00000`, 0x3b500000
below the heap base.

**Option B (durable invariant, `Win32Stubs.h` `alloc_block`). Reserve the
arena and make the allocator skip it.** This survives a future move of the
arena and does not depend on call ordering:

```cpp
enum { SURF_LO = 0x60000000u, SURF_HI = 0x68000000u };  // 128 MB, 64 surfaces
inline u32 alloc_block(u32 size, u32 align) {
    if (size == 0) size = 1;
    u32 &b = bump();
    b = (b + (align ? align - 1u : 0u)) & ~(align ? align - 1u : 0u);
    u32 p = b, end = p + size + 16u;
    if (p < SURF_HI && end > SURF_LO) { p = SURF_HI; end = p + size + 16u; }
    b = end;
    return p;
}
```

Option A is the smaller change and touches a file the heap lane is not
editing; Option B is the one to keep if `Rendering.h` ever moves the arena
again. Do not do both with different bases; pick one and make the other's
comment name it.

*Verification:* re-run the 2.7227e9 and 4e9 budgets. Success is
`frame md5 == 6cb0e1ff…` (the loading screen) at both, with the same render
counters (12 blits / 769 locks / 0 flips). The blue grid must not reappear at
any budget. `tools/test_runtime.py` already owns the surface-layer tests; add
a case that allocates >256 MB through the stub heap and asserts the primary's
pixel bytes are unchanged, so the regression cannot come back silently.

### 7b. The next blockers (after 7a)

In order:

1. **Fix the step counter before the next long run.** `Machine::steps` is a
   32-bit `unsigned long`, `boot_driver` clamps a budget above `0xffffffff`
   to `4294967295`, and `insn()` tests `steps > step_limit`. A budget at or
   above `2^32` never fires and the reported count wraps. Until this is fixed
   (make `steps`/`step_limit` 64-bit, or clamp below `2^32` and report the
   wrap), every run over 4.29e9 instructions is reported modulo 2^32 and is
   bounded only by the wall clock. This already affects the concurrent 12e9
   measurement.
2. **Throughput.** The 4.07e9 stop is the 900-second watchdog, not the
   instruction budget. The model is at 722,815 page-table slots / 2.9 GB RSS
   and ~4.5M instructions/s, down from ~26M/s at 4e9; the 1800-second run
   covers only 5.14e9. The iso-map build is done; the tactical draw is the
   phase that now needs the budget. The heap lane's free-list/coalescing work
   is aimed exactly at this (fewer fresh pages, lower RSS); if it does not
   restore the rate, profile `ts::page` with `perf record -g` as `AGENTS.md`
   describes.
3. **The 21 unmapped calls and the corrupted-looking context.** The targets
   are the delta table above, not code; after them the game runs 1.07e9
   instructions in `sub_00675ba0` with `ESP=0x844` and coordinate values in
   the pointer registers, adding no coverage and no render calls. Apply 7a and
   re-run: if the 21 calls and the odd context disappear, they were the arena
   collision (an object allocated into a freed arena-range block and then
   overwritten by surface writes). If they persist, capture the call sites
   (the out-of-text sample records the target only; a gdb breakpoint on the
   `unmapped_sample` store, or a small `Machine.h` instrumentation in a
   scratch clone, gives `m.eip`).
4. **The black primary.** At 4.07e9 the primary is 0 non-black while 3 new
   blits and 8,097 locks have run. Trace those 3 blits (dst/src/flags) and the
   lock targets: either the tactical draw presents to an offscreen that is
   never copied to the primary, or it clears the primary and the actual cell
   draw has not run yet at the watchdog. That is the same class of question as
   the original entry, now at the correct phase.

Do not chase the 0-fill divergence before 7a: the model's menu frame already
matches retail, and the missing 15 startup/UI colour-fills are not what blanks
the mission frame.

## 8. Evidence index

| file | what it is |
|---|---|
| `out/render_2_6b.png` | intact loading screen, md5 `6cb0e1ff…` (the 2.7e9 frame is byte-identical) |
| `out/render_partial.png` | heap boundary at row ~260, md5 `a5440448…` |
| `out/render_3_2b.png` | frozen heap pattern, md5 `ab01ed73…` (the 4.0e9 frame is byte-identical) |
| `out/render_6b.png` | black primary at the tactical phase, md5 `fab3ec8fbf7b5c7de8041b90a78c6c31` |
| `/tmp/render_gdb2.log` | 4e9 gdb trace: 7 `CREATE`, 12 `BLT`, 769 `LOCK`, 0 `FLIP` |
| `/tmp/render_state*.log` | surface table + `bump()` at 2.7 / 2.7227 / 3.2e9 |
| `/tmp/render_callers3.log`, `/tmp/render_callers4.log` | blit/lock/grow call chains |
| `/tmp/lockdist.log` | full lock census (762 of 769 visible) |
| `/tmp/render_45b_full.log` | 5.14e9 actual run: unmapped targets, wrapped step count, blitter stop |
| `/tmp/retail_render2/ddraw.log` | retail `+ddraw`, 250 Blt / 651 Lock / 0 Flip |
| `tools/oracle_render_retail.sh` | the retail driver used for the comparison |
