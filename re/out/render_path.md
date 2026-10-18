# The mission render path: why no mission frame is presented

Measured 2026-10-18 on main `eb151bf` (page table growable), in the lane
worktree. Every claim below is a boot-report line, a gdb breakpoint hit, a
Ghidra decompile, or a decoded frame. The raw logs are named where each fact
comes from.

## Answer

The game **does** present a frame after the mission load starts: the GDI mission
**loading screen** (mech artwork, "ANALYZING COMBAT ZONE. TOPOGRAPHY..."), fully
drawn at 2.6e9 instructions and unchanged at 2.7e9.

It never presents a **mission** frame for two independent reasons:

1. **The load is not finished.** At 4e9 instructions the game is still inside
   the isometric-map build, `DynamicVectorClass::Grow` at `0x0055b050` /
   `0x0055b09a`, copying ~32,000 8-byte entries for the second of 22 map
   layers. No DirectDraw call is made after 2.6e9 instructions, so no later
   frame can be presented at any of the budgets measured here.
2. **The framebuffer is destroyed in place.** The model's Win32 stub heap is an
   unbounded bump allocator starting at `0x50000000`
   (`Win32Stubs.h:80`). The DirectDraw surface pixel arena starts at
   `0x60000000` (`Rendering.h:116`). When the heap cursor crosses
   `0x60002000` (the primary surface's first pixel) at about **2.722e9**
   instructions, the load's heap traffic overwrites the primary surface
   pixel-for-pixel. The blue banded/grid frame is that overwrite: raw heap
   memory read as RGB565. It is not a loading screen, not an uninitialised
   surface, and not a partially drawn mission.

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

Two facts fall out of the table:

* **All rendering happens before 2.6e9.** The counters are identical at 2.6,
  3.2 and 4.0e9: the mission load issues no `Blt`, `Lock`, `Unlock` or `Flip`
  after the loading screen is drawn.
* **The frame changes exactly once, then freezes.** 2.6e9 and 2.7e9 are
  byte-identical (the loading screen). 3.2e9 and 4.0e9 are byte-identical (the
  heap pattern). The only interval in which the primary changes is the heap
  crossing, and nothing the game does afterwards writes it again.

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

### Blits: 12, all into the primary, none after 2.6e9

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

### Locks: 769, presentation is lock-and-write, not Flip

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
retail trace does issue them (14 with `flags 0x1000400` before the menu), so
this is a real behavioural divergence, but not the blocker: the menu frame the
model renders is within 1.06/255 of retail on the pixels that differ
(`AGENTS.md`), so the missing fills do not leave a visible hole on this path.

### Flip: 0, and that is correct

`k_sfs_Flip` is wired at slot 11 (`Win32Stubs.h:4893`) and its body increments
`stat_flips()` (`Rendering.h:664`). The gdb breakpoint on `ts::rnd::flip` is
**never hit** in the 4e9 run. The retail trace has **0** `ddraw_surface1_Flip`
calls in 6,176 trace lines. The game presents by `Blt`/`Lock` into the primary,
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
   byte-identical, and no DirectDraw call occurs after 2.6e9. The primary is
   abandoned holding whatever the heap last wrote there.

The loading screen itself is real and correct: `out/render_2_6b.png` and
`out/render_2_7b.png` are byte-identical and show the GDI mech, the status
panel ("ANALYZING COMBAT ZONE. TOPOGRAPHY...", "GATHERING INTEL ON INVOLVED
FACTIONS..."), and the GDI emblem — the game's own mission loading screen.

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
"mission load is dominated by vector-growth reallocations" entry in `todo.md`,
and it is the next blocker after the address-space fix.

## 6. Retail comparison

`tools/oracle_render_retail.sh` (new, this lane) runs the retail `GAME.EXE`
under Wine with `WINEDEBUG=+ddraw`, dismisses the startup dialogs, and captures
a screenshot at each stage. The DirectDraw contract measured through the
startup movies and the main menu (`/tmp/retail_render2/ddraw.log`, 6,176
lines):

| call | retail | model (4e9) |
|---|---|---|
| `IDirectDrawSurface::Blt` | **244** (14 colour-fill `0x1000400`, 230 copies `0x1000000`) | 12 |
| `IDirectDrawSurface::Lock` | **380** | 769 |
| `IDirectDrawSurface::Flip` | **0** | 0 |
| `CreateSurface` | 7 | 8 (the model creates one extra small surface) |

The retail copies are `dst (0,40) src=015F0FF0` x220 (movie frames into a
640x400 letterbox), `dst (0,0) src=015F0FF0` x9, and one self-blit; the
colour-fills are 14 `dst (0,0) src=00000000 flags=0x1000400`. Presentation is
`Blt` to the primary `015F02B8` followed by
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

### 7b. The next blocker (after 7a)

The mission load's instruction cost. With the framebuffer protected, the run
still needs a budget large enough to finish the 22-layer iso-map build; at 4e9
it is in layer 2. This is not a defect in the model's Windows layer: the
retail executes the same O(n²) growth and finishes in well under a second on
real hardware, while the model pays ~8 interpreted instructions per 8-byte
entry copy. Measure the completion budget with a single long run (the heap
lane is already running 12e9); then either accept the budget for a
mission-frame demonstration or reduce the per-instruction cost (the heap
free-list work reduces memory and page-table pressure but **not** the copy
count).

Do not chase the 0-fill divergence before 7a: the model's menu frame already
matches retail, and the missing 14 startup colour-fills are not what blanks
the mission frame.

## 8. Evidence index

| file | what it is |
|---|---|
| `out/render_2_6b.png` | intact loading screen, md5 `6cb0e1ff…` (the 2.7e9 frame is byte-identical) |
| `out/render_partial.png` | heap boundary at row ~260, md5 `a5440448…` |
| `out/render_3_2b.png` | frozen heap pattern, md5 `ab01ed73…` (the 4.0e9 frame is byte-identical) |
| `/tmp/render_gdb2.log` | 4e9 gdb trace: 7 `CREATE`, 12 `BLT`, 769 `LOCK`, 0 `FLIP` |
| `/tmp/render_state*.log` | surface table + `bump()` at 2.7 / 2.7227 / 3.2e9 |
| `/tmp/render_callers3.log`, `/tmp/render_callers4.log` | blit/lock/grow call chains |
| `/tmp/lockdist.log` | full lock census (762 of 769 visible) |
| `/tmp/retail_render2/ddraw.log` | retail `+ddraw`, 244 Blt / 380 Lock / 0 Flip |
| `tools/oracle_render_retail.sh` | the retail driver used for the comparison |
