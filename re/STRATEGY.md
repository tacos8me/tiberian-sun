# STRATEGY — the shortest credible path to a working, playable Tiberian Sun

Written against HEAD `8ce1ebe`; the figures below were re-measured on
2026-10-17 with the commands named in each row. Numbers quoted from a document
instead of measured say so. Anything I could not verify says **unverified**.
Re-measure before trusting this file: `python3 tools/harness.py metrics
--no-append`. The boot, link-test and oracle figures were re-measured after the
message-delivery and Blowfish-cipher merge (`boot.py`, `link_test.py`,
`wine_env.sh`); the full `pipeline.sh` run recorded in `d23d1bf` predates the
newly reachable shutdown code and was not re-run here.

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
| checks 1/3/4/5/6 | 263 headers, 749/749 classes, 749/749 bindings, 219/219, 220/220 | same |
| check 7 | 1,317/1,317 member offsets, 206 classes | same |
| check 8 (arity) | **331/1,116 match, 785 divergence**, reported not gated | same |
| check 9 (`offsetof`) | **3,073/3,073 compiler-confirmed** | same |
| recovered functions | 43,564 | `harness.py metrics` |
| instructions in `out/functions.json` | 848,822 | summed over `insns` |
| transcriptions emitted | 46,008 | `grep` over `src/generated/code` |
| `.text` | 96.09% as code, 99.99% attributed | metrics |
| classes / slots | 749 / 9,810, 0 unseeded | metrics |
| member offsets / named | 4,289 / **291** | metrics |
| virtual slots | **5,557** named `(class, slot)` pairs of 9,810; **917** distinct names (**281** semantic); headers declare 4,496 named + 2,458 `vfunc_N`; `metrics`' `virtual_names: 702` counts *classes with a naming record*, not names | metrics + `out/virtual_names.json` |
| indirect call sites | **18,056 of 60,449 (29.9%)** | walked `insns` |
| functions with no direct caller | **31,834 (73.1%)** | direct-edge graph from `callees` |
| functions reachable from entry `0x6854e3` by direct edges | **69 (0.16%)** | BFS over `callees` |
| functions declared by the reconstructed headers | **3,024 of 43,564 (6.9%)** | unique `sub_*` in `src/generated/*.cpp` |
| boot | **20,000,001 instructions** (budget) at `0x006844db` (`_exit`/atexit walk), **1,977 import calls, 99 distinct imports**, depth 16, **70 unmapped targets (10 in `.text`; 33 below `0x1000`, 27 elsewhere)**, message loop **reached and dispatching** (`CreateWindowExA=1`, `GetMessage=2`, `PeekMessage=5`, `DispatchMessage=2`), **51,604 distinct instructions / 4,535 of 46,008 functions entered**, primary surface `0x006beed8`=1, WinMain returns ~7.66M with ESP `0x11` | `tools/boot.py` (default `../gamedir`) |
| link test | **FAILED, exit 1**, 492 TUs, 20,000,001 instructions, **message loop reached** (`GetMessage=2`, `PeekMessage=5`, `DispatchMessage=2`), **10 unmapped calls inside `.text`**, 0 unmodelled | `tools/link_test.py` |
| boot strict gate | **exit 3**, 70 garbage call targets | `boot.py --no-build --strict` |
| retail oracle | reaches the **main menu**, 640x480, screenshot md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s | `tools/wine_env.sh` |
| Win32 surface | **341** PE import-directory thunks, all resolved; the stub table has **342** rows (the extra is `COMCTL32!DllGetVersion`, reached through `GetProcAddress`): **315 with a body, 27 generic** | `link_test.py` / boot |

In words:

1. **The binary is done.** Recovery, transcription and layout are off the
   critical path. A round spent there is a round spent on polish.
2. **The code has no name.** 73% of functions are reached only indirectly, the
   direct call graph reaches 69 functions from `main`, and the headers bind to
   3,024 of 43,564 functions. Whatever "named" means in the metrics, the tree a
   reader can *call* is 6.9% of the program.
3. **The runtime is the product.** What exists is a **20-million-instruction**
   run of the game's own startup that **delivers messages and reaches its own
   shutdown path**: `CreateWindowExA=1`, `GetMessage=2`, `PeekMessage=5`,
   `DispatchMessage=2`; the WndProc runs and `DAT_007b2e50` becomes 1. COM
   registration (`1580be3`) and message delivery (`f3011ca`) are fixed, and the
   Blowfish cipher is **implemented and proven** (`345aae7`: four known-answer
   vectors plus a byte-identical `tools/mix.py` cross-check on the shipped
   TIBSUN.MIX index). The run now consumes the 20M budget in the CRT exit path
   because WinMain returns with ESP `0x11`, and **70 unmapped teardown calls
   remain (10 in `.text`)**. The next content defect is the MIX index read
   (2 of 27 blocks).
4. **The Win32 layer is real where the game has been, and fiction where it has
   not** (`Win32Stubs.h`, read):
   - File I/O: **real** — a host root, case-insensitive paths, a 64-entry handle
     table, real `CreateFileA`/`ReadFile`/`WriteFile`/`SetFilePointer`/
     `GetFileSize`/`CloseHandle`. The old "there is no real file system" text is
     gone.
   - Input: `GetAsyncKeyState`/`GetKeyState` return 0. No key can arrive.
   - Timers: `timeSetEvent` returns 1 and never fires its callback.
   - Surfaces: `Lock` returns a real buffer that nothing reads; `Blt`/`Fill`/`Flip`
     are no-ops. There is no way to see a frame.
   - Strings: `LoadStringA` now has the correct arity (4) and returns 0; the
     MessageBox text is empty because the string table is empty at that point.

So: **structure recovered, instructions transcribed, layout compiler-confirmed,
and a runtime that has executed 20,000,001 instructions through message delivery
into its own shutdown path, on a Win32 layer that can load a file, create a
window and dispatch messages but cannot yet read the MIX index correctly, survive
shutdown, read a key, fire a timer, or show a pixel.**

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
   `46008 functions transcribed, 918438/918438 instructions modelled, 0
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
   `tools/mix.py` decodes both MIX index variants. What remains is routing
   `MixFileClass` through the real reader in the model, not inventing a file
   layer — and the model's MIX index read currently stops after **2 of 27
   blocks** (`0x547435`/`0x547426`), the next queued content defect. This was the
   largest missing block and it has moved.
4. **Surface memory, real blits, a frame dump.** → anything visual, and any
   judgement that the game works. Needs 3 to have content to draw.
5. **A virtual clock with timer callbacks.** → the logic tick. A
   `timeSetEvent` that never fires means timer-driven logic never runs.
6. **Input.** → "playable" rather than "a demo that plays itself".
7. **Fidelity at scale.** Unbounded in principle, bounded by the oracle: run
   both, diff at checkpoints, fix the first divergence. This is where the months
   are, and it is only tractable with 2.
8. **Speed.** 20,000,001 instructions in 1.1 s wall is order 18M instructions/s —
   and that includes image mapping, so treat it as an upper bound on cost, not a
   clean throughput. A 30 fps budget is ~330k instructions/frame. If a TS frame
   costs millions, the interpreter is 10–30× too slow. Measure before promising.

**Not on the path:** naming the 2,458 `vfunc_N` slots, source-shaping 46,008
transcriptions, the last 104 KB of `.text`, re-deriving member offsets. All four
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
- **Graphics now has an oracle, but a partial one.** `tools/wine_env.sh` gives a
  real main-menu screenshot (640x480, md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`),
  so a model frame can be diffed against it. The menu's buttons/text are not
  drawn in that screenshot because the PCX assets live inside a nested MIX
  entry, so the oracle cannot yet adjudicate the UI layer.
- **Sound stays silent without a mixer.** `DirectSoundCreate` now returns
  `DSERR_NODRIVER` (matching the retail no-audio environment), and no buffers are
  filled or mixed. Low priority, but do not discover it at the end.
- **The 27 generic stubs are silent holes**, and the project's own lesson
  applies: a stub returning a plausible wrong value is worse than one that fails.
  `LoadStringA` now has the correct arity but returns 0 because the string table
  is empty; the box renders blank and nothing says why.
- **The Blowfish cipher is real, and the model now implements it.** The retail
  oracle measures 9 `SetKey(56,key)` calls and 1,586 `Decrypt(8,in,out)` calls
  during startup, matching nine shipped MIX indexes; `345aae7` implements
  standard Blowfish-ECB and proves it against four known-answer vectors and a
  byte-identical `tools/mix.py` cross-check. The next content defect is the MIX
  index **read** (2 of 27 blocks), not the cipher.
- **The shutdown path is the new ESP defect.** WinMain returns at ~7.66M
  instructions with ESP `0x11` (entry `0x2fff84`); the atexit walk then consumes
  the remaining ~12.3M instructions. This is the same class as the original
  108-byte leak, on the exit path.
  Implement the cipher before trusting any MIX content path.
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
I/O is real); **message delivery and the Blowfish cipher are also done** (units
11 and 12 below). Units 6–10 remain, and **the MIX index read (unit 13) is the
next runtime blocker**.

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
   instructions; N of 46,008 recovered functions entered` and classifies unmapped
   targets (`below 0x1000`, `.rdata`, `elsewhere`). Measured now: 51,604 distinct
   instructions / 4,535 functions, **70 unmapped (10 in `.text`)**, `--strict`
   exits **3**. **Done** (the gate correctly reports the new teardown defect).
5. **Real file I/O.** Real `CreateFileA`/`ReadFile`/`WriteFile`/`SetFilePointer`/
   `GetFileSize`/`CloseHandle` over a host root, handles as real objects.
   **Done.**
6. **MIX in C++.** Port `tools/mix.py` (proven: both index variants decode,
   header+index+body equals file size) and route `MixFileClass` through it.
   *Exit:* the model opens `TIBSUN.MIX` and entry count and offsets match
   `mix.py` byte for byte. The cipher is done; the model's index read currently
   stops after 2 of 27 blocks (unit 13).
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
13. **Fix the MIX index read.** It stops after 2 of 27 blocks
    (`0x547435`/`0x547426`). *Exit:* the 17-entry index is read correctly and
    the game proceeds past the MIX header. **Open — the next blocker.**
14. **Fix the shutdown ESP corruption.** WinMain returns with ESP `0x11` (entry
    `0x2fff84`); the atexit walk runs away. *Exit:* WinMain returns with ESP
    where its caller left it. **Open.**
15. **Fix the null teardown calls / 10 in-text unmapped calls.** *Exit:*
    `boot.py --strict` exits 0 and the link test passes. **Open.**

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
   named pairs, **917** distinct names (**281** semantic), **6,954** header
   declarations (**4,496** named + **2,458** `vfunc_N`), **702** classes with a
   naming record. The distinction is stated once in `PROGRESS.md`; the old
   "1,500 / 8,280 / 3,409" mixed quantities and is retracted (`8467f84`).
5. **RESOLVED — `src/generated/README.md` contradicted itself.** The sentence now
   says what the number measures: **3,024 of 43,564 (6.94%) recovered functions
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
   caveat now says the opposite with measured counts: 46,008 transcriptions,
   918,438 instruction lines, 0 unmodelled, 492 TUs linked.
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
    `test_tools.py` (now 43 checks) and the import distinction (`8467f84`).
12. **OPEN — the framing error, which is the expensive one.** `ROADMAP.md` §2.3
    used to close with "for a decompilation that is the difference between 'it
    builds' and 'it is the game'". The sentence is gone and §4 now cross-
    references this file's argument, but the *strategic* disagreement is real
    and unresolved: the roadmap optimises the source artifact, this file
    optimises a playable runtime. Say which product you are building.
13. **RESOLVED — `AGENTS.md`'s "Current status" table carried stale counts.**
    It now uses the measured set: 43,564 spans (not 41,822), 96.09% code (not
    96.06%), 105,966 B unattributed classified as 12 jump tables / 264 B, 7
    switch index tables / 349 B, 104,959 B padding, 394 B in 55 runs (not 104 KB,
    11 jump tables / 256 B, 356 B, 105,808 B, 554 B in 90 runs); 9,810 vtable
    slots (not 9,780); 46,008 transcriptions / 918,438 instruction lines (not
    43,668 / 897,193); 264 headers (not 262); 2,444 tail entries (not 1,846;
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
    `d23d1bf`. **Superseded:** the newly reachable shutdown code reintroduces
    **10 in-text unmapped calls**, so the link test now fails (exit 1) and the
    pipeline would stop there. Item 22 tracks it.
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
20. **OPEN (new) — the MIX index read stops after 2 of 27 blocks**
    (`0x547435`/`0x547426`). The next content defect; `todo.md` has the
    reproduction and the pipe/control-flow hypotheses.
21. **OPEN (new) — shutdown corrupts ESP.** WinMain returns at ~7.66M
    instructions with ESP `0x11` (entry `0x2fff84`); the CRT atexit walk consumes
    the remaining ~12.3M instructions.
22. **OPEN (new) — null teardown calls produce 70 unmapped calls, 10 inside
    `.text`.** They did not disappear when the index decrypted. `boot.py
    --strict` exits 3 and the link test fails (exit 1).

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
