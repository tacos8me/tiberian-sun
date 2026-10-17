# STRATEGY — the shortest credible path to a working, playable Tiberian Sun

Written against HEAD `8ce1ebe`. Every figure was produced by a command in this
session; the command is named. Numbers quoted from a document instead of measured
say so. Anything I could not verify says **unverified**. Re-measure before
trusting this file: `python3 tools/harness.py metrics --no-append`.

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
| virtual slots | 4,496 named, 2,458 `vfunc_N`, **702 distinct names** | metrics |
| indirect call sites | **18,054 of 60,449 (29.9%)** | walked `insns` |
| functions with no direct caller | **31,834 (73.1%)** | direct-edge graph from `callees` |
| functions reachable from entry `0x6854e3` by direct edges | **69 (0.16%)** | BFS over `callees` |
| functions declared by the reconstructed headers | **3,024 of 43,564 (6.9%)** | unique `sub_*` in `src/generated/*.cpp` |
| boot | 1,011,817 instructions, 0.1 s, 721 import calls, 53 distinct imports, depth 15, **34 unmapped indirect targets (0 in `.text`)**, message loop **not reached** | `tools/boot.py` |
| Win32 surface | 342 registered, **315 with a body, 27 generic** | boot |

In words:

1. **The binary is done.** Recovery, transcription and layout are off the
   critical path. A round spent there is a round spent on polish.
2. **The code has no name.** 73% of functions are reached only indirectly, the
   direct call graph reaches 69 functions from `main`, and the headers bind to
   3,024 of 43,564 functions. Whatever "named" means in the metrics, the tree a
   reader can *call* is 6.9% of the program.
3. **The runtime is the product.** What exists is a 1.01-million-instruction run
   of the game's own startup that ends in 34 indirect calls into garbage
   (`0x00000011` repeatedly, `.rdata` addresses like `0x006a4850`) — the machine
   saying a value is not a function pointer.
4. **The Win32 layer is fiction exactly where it matters** (`Win32Stubs.h`, read):
   - File I/O: *"There is no real file system ... A file 'exists' ... and reads as
     empty. That is a lie, but it is a consistent one"* (578). No `rules.ini`, no
     `.mix`, no `.shp`, no map, no sound — ever.
   - Input: `GetAsyncKeyState`/`GetKeyState` return 0 (1467). No key can arrive.
   - Timers: `timeSetEvent` returns 1 and never fires its callback (2098).
   - Surfaces: `Lock` returns a real 800x600x16 buffer (2381) that nothing ever
     reads; `Blt`/`Fill`/`Flip` are no-ops. There is no way to see a frame.
   - Strings: `LoadStringA` returns 0 (1465) — which is why the boot's MessageBox
     has empty text. (Its pointer `0x715524` also lies past `.data`'s raw extent,
     so it would read zero in a real process too.)

So: **structure recovered, instructions transcribed, layout compiler-confirmed,
and a runtime that has executed ~1.2× the 848,822 static instructions through a
depth-15 slice of startup, on a Win32 layer that cannot load a file, read a key,
fire a timer, or show a pixel.**

### The fact the docs do not contain

```
$ wine --version                -> wine-9.0
$ /proc/<GAME.EXE>/maps         -> 00400000-00698000 r-xp ... GAME.EXE
$ xdotool getwindowname         -> "Tiberian Sun" (two windows)
screenshot                      -> the game's own dialog: "Warning - Unable to
                                   create Direct Sound Object. Proceeding with
                                   audio disabled."
```

The retail binary **runs under Wine here, maps at exactly 0x00400000, creates its
windows, and reaches its own DirectSound init path.** Every recovered VA is live
in a real process: `gdb -p`, `break *0xVA`, read EAX/ESP/EBP and 64 bytes of
stack, compare with the model at the same VA. That is a differential oracle at
instruction granularity. `AGENTS.md` asserts the opposite — *"Semantic
equivalence with the original cannot be proven here (there is no MSVC 6
toolchain, only g++)"* — true about compiling, wrong about observing.

Caveat: Wine is not Windows, so it cannot adjudicate a *Win32 stub's* behaviour.
It can adjudicate the x86 model, because that is the real CPU running the same
bytes.

---

## 2. The critical path, ranked by downstream fan-out

`→` means "prerequisite for".

1. **Instruction-level model fidelity** (control flow complete, ESP right, flags
   right). → everything. The retracted `PeekMessage=137,295` is proof: a model
   bug makes every downstream observation fiction. `gen_code.py` now emits
   `ts_tail` continuations (10,477 spans, doc-quoted — I did not re-run the
   generator) and `ret` pops, but **no gate fails when a span ends with neither a
   transfer nor a registered continuation.** That is why this defect class
   recurs.
2. **The Wine oracle.** → every later fidelity claim, including 1. Cheap, proven
   to work, and it turns "the arguments look wrong" into a diff.
3. **Real file I/O + MIX + a virtual file system.** → every content path: INI,
   art, maps, audio, movies. Without it the game cannot step past init however
   correct the CPU is. **Absent from `ROADMAP.md` entirely**; the largest missing
   block.
4. **Surface memory, real blits, a frame dump.** → anything visual, and any
   judgement that the game works. Needs 3 to have content to draw.
5. **A virtual clock with timer callbacks.** → the logic tick. A
   `timeSetEvent` that never fires means timer-driven logic never runs.
6. **Input.** → "playable" rather than "a demo that plays itself".
7. **Fidelity at scale.** Unbounded in principle, bounded by the oracle: run
   both, diff at checkpoints, fix the first divergence. This is where the months
   are, and it is only tractable with 2.
8. **Speed.** 1,011,817 instructions in 0.1 s wall is order 10M instructions/s —
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
- **Naming is the roadmap's biggest item and is worth nothing here.** 702 names
  already cover 4,496 slots; naming the rest changes no byte of execution.
  Defensible only as documentation.
- **Source-shaping has no honest exit test.** ROADMAP P3 wants "≥50% of
  instructions in the 500 highest-centrality functions". There is no centrality
  that survives measurement: 69 functions reachable from `main`, 73% with no
  direct caller, 30% of call sites indirect. A lifter's only real check is
  behavioural equivalence — which needs the oracle. Months, unbounded, and it
  makes nothing run.
- **Graphics has no oracle except the one you can now build.** Without a
  screenshot diff, "the renderer works" is a human squinting at a PNG.
- **Sound stays silent without a mixer.** `DirectSoundCreate` returns a fake
  object; buffers are never filled or mixed. Low priority, but do not discover it
  at the end.
- **The 27 generic stubs are silent holes**, and the project's own lesson
  applies: a stub returning a plausible wrong value is worse than one that fails.
  `LoadStringA` returning 0 is exactly that — the box renders blank and nothing
  says why.
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

Each has an exit criterion a command can decide. Units 1–2 are in flight.

1. **Wine oracle (`tools/oracle.py`).** `xvfb-run wine GAME.EXE` in
   `work/cd1/INSTALL`, wait for the "Tiberian Sun" window, dismiss dialogs with
   `xdotool`, attach `gdb`, `break *0xVA`, print registers + 64 bytes of stack.
   Feasibility verified (wine 9.0, image at 0x400000, windows created, Xvfb and
   xdotool present). *Exit:* `python3 tools/oracle.py 0x00684529` prints the real
   ESP/EBP/EBX at the `ExitProcess` call site beside the model's, and `--diff`
   exits non-zero when they differ.
2. **The stack imbalance (owned by the parent).** With 1 available it is one
   breakpoint: real ESP vs model ESP at `0x00684529`. *Exit:* `ExitProcess`
   receives `1` and `MessageBoxA` type `0x30`; the 34 garbage call targets fall to
   0 or each is named.
3. **Gate control-flow completeness.** `gen_code.py` already counts
   continuations, overlaps and defects; fail the pipeline unless every span ends
   on a transfer or a registered continuation, and prove the gate by deleting one.
   *Exit:* 0 spans with neither, and exit 1 when a continuation is removed.
4. **Make the boot report coverage.** Distinct functions entered, distinct basic
   blocks, and a hard fail on any indirect target below `0x1000` or inside
   `.rdata`. Today it reports 34 garbage targets and no coverage figure. *Exit:*
   `boot.py` prints "functions entered: N of 43,564", with 0 garbage targets or
   each named.
5. **Real file I/O.** `CreateFileA`, `ReadFile`, `WriteFile`, `SetFilePointer`,
   `GetFileSize`, `FindFirstFileA`/`FindNextFileA`, `CloseHandle` over a root
   directory, handles as real objects. *Exit:* a unit test opens
   `TIBSUN.MIX` and reads 1,024 bytes at offset 0, and the game's own INI reader
   returns >0 bytes for `rules.ini` during a boot.
6. **MIX in C++.** Port `tools/mix.py` (proven: both index variants decode,
   header+index+body equals file size) and route `MixFileClass` through it.
   *Exit:* the model opens `TIBSUN.MIX` and entry count and offsets match
   `mix.py` byte for byte.
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

---

## 5. What the docs now get wrong

Ordered by damage. Same failure mode the project already caught once: prose that
measurement does not support.

1. **`AGENTS.md`'s "What the boot does today, measured" block is the retracted
   result** — `budget 5000000`, `instruction budget exhausted`,
   `message loop: reached (PeekMessage=137295)`. Measured now: budget
   20,000,000, stop at `0x00684529` after 1,011,817 instructions, **message loop
   not reached**. The correction sits ten lines below the false block, so the
   first thing a reader learns about the boot is a claim the same file retracts.
2. **`ROADMAP.md` P4 says "4.2 is DONE ... the game runs a frame loop ...
   `PeekMessage=137,295`"** — the same retracted claim, still shaping the plan's
   ordering and its "what is left is not 'does it run'" paragraph.
3. **`ROADMAP.md` §2.3 and all of P0 are a completed phase described as the
   top priority.** It calls the vptr-as-member defect "the real defect", quotes
   `offsetof(AbstractClass, m_000) == 20`, and makes "make compilable mean the
   game's ABI" the start of the trajectory. Measured: `AbstractClass.h` omits
   vptr stores ("the compiler supplies the vptr"), check 9 confirms 3,073/3,073,
   and the gate already compiles `-m32`. §2.2's ABI figures ("2,313 `unsigned
   long`", "ObjectClass 248 bytes") are equally stale.
4. **The virtual-naming denominator is wrong in `ROADMAP.md` §2.4 and
   `todo.md`**: "1,500 named (15.3%), 8,280 unnamed, 3,409 distinct". Metrics:
   4,496 named, 2,458 `vfunc_N`, **702 distinct**, 9,810 slots. `AGENTS.md` gives
   a third set ("5,557 named, 917 distinct, 281 semantic"). Three files, three
   answers, one measurement.
5. **`src/generated/README.md` contradicts itself and hides the most important
   number in the project**: "6.94% of the recovered functions have a
   transcription; the rest ... are transcribed in `code/` regardless". Every
   recovered function is transcribed (46,008 bodies). What 6.94% measures is that
   **only 3,024 of 43,564 functions are declared by the reconstructed headers** —
   the real modifiability ceiling, stated nowhere.
6. **`todo.md` is internally contradictory.** Two entries titled "Spans that do
   not end on a transfer drop their continuation" (one `[x]`, one `[~]`, same
   defect). "Get past the 512 MB memset" is both `[x]` and `[ ]`. "Reach a window
   and a message loop" is both `[x]` and `[ ]`. "166 instructions have no faithful
   rendering" is `[ ]` while `PROGRESS.md` says 0 unmodelled. "Compile the gate
   with `-m32`" is `[~]` though it is done and gating. "Fix the IPersistStream
   type inconsistency" is `[ ]` in P3 and `[x]` in P2.
7. **`todo.md`'s "Load the image's data sections into the machine model" rests on
   a false premise** — it says "nothing ever copies `.idata`/`.rdata` in".
   `Image.h` copies every section's raw extent into modelled memory and writes a
   stub VA into all 341 IAT slots; boot reports 341 of 341 resolved. The empty
   MessageBox text is `LoadStringA` returning 0, not a missing section.
8. **`todo.md`'s standing caveat "Function bodies are empty. The project
   reproduces structure, not behaviour."** is false and contradicts the same
   file's own 46,008 bodies / 918,438 instructions.
9. **Import counts disagree three ways**: `AGENTS.md` "342/342 resolve", boot
   "341 of 341 resolved, 341 traced", stub table "342 registered". One is a
   different quantity; the docs never say which.
10. **Verification figures drift inside single files.** `AGENTS.md` quotes check
    9 as 3109/3109 (measured 3,073/3,073) and check 7 as both 1317/1317 and
    2186/2186 in different paragraphs. Check 8 is quoted at 478/1,172 and
    measured at **331/1,116** — it is getting *worse* as headers change, and
    nobody is watching a check that does not gate.
11. **`CONTEXT.md` calls `verify.py` "the 8-check fail-closed gate"** (it is 9)
    and lists tooling as if current.
12. **The framing error, which is the expensive one.** `ROADMAP.md` §2.3 closes
    with "for a decompilation that is the difference between 'it builds' and 'it
    is the game'". Measured: the game's own instructions already run; that
    difference is the runtime — file I/O, surfaces, timers, input — none of which
    appears anywhere in the roadmap.

---

## 6. If you only do three things

1. Build the Wine oracle. Hours of work, and it makes every other claim in this
   project checkable instead of arguable.
2. Give the model a file system. Until then no amount of CPU fidelity gets the
   game past its own data load.
3. Say which product you are building. If it is *a playable game*, use Wine and
   stop. If it is *our own code running the game*, ROADMAP P2 and P3 are not on
   the path and should be demoted to "documentation, later".
