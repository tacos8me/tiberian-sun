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
  src/handwritten/Image.h         maps the PE's sections at their real VAs and
                                  writes a stub VA into every IAT slot
  src/handwritten/Win32Stubs.h    what the 341 imported Windows functions DO, so
                                  `call dword ptr [IAT]` reaches code
  build/image_selftest.cpp        load the image and resolve imports, standalone
  build/boot_driver.cpp           the boot harness behind tools/boot.py
  tools/ghidra_scripts/           Ghidra post-scripts (DecompileAt.java)
  tools/wine_run.sh               run the RETAIL binary under Wine (headless,
                                  --trace CHANNEL, --dismiss, --shot PNG)
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
| `tools/gen_win32_stubs.py` | generates `Win32Stubs.h` from the image's own import directory, so the stub table cannot drift from the 341 thunks; `--evidence` derives argument counts from the recovered instruction stream |
| `tools/boot.py` | one command that builds (cached: 0.4s), boots the image and reports it -- imports called by name, fault EIP with instruction bytes, containing function and generated source line, and whether a message loop was reached |
| `tools/ghidra.sh` | decompile the function containing each given address, from a saved Ghidra project (seconds, no re-analysis) |
| `tools/wine_run.sh` | run the **retail** `GAME.EXE` under Wine, headless: plain, `--trace ddraw`, `--dismiss` the startup dialog chain, `--shot` a PNG. The only oracle this project has |
| `tools/oracle.py` | attach gdb to the retail binary, break at an address, print real ESP/EBP/EBX/EAX and the stack top; `--compare` diffs against the model at the same address |
| `tools/test_tools.py` | unit tests for the load-bearing tools; every one is a regression test for a shipped defect |
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
$ python3 tools/boot.py            # 0.4s build (cached), 0.2s run
image:  3227799 bytes in 4 sections; 342 of 342 imports resolved, 342 traced
stop:   fault at 0x00684529 after 1,011,817 instructions, depth 15
why:    the program called ExitProcess
message loop: not-reached   (GetMessage=0 PeekMessage=0 DispatchMessage=0
                             CreateWindowExA=0)
```

So: the game starts, initialises, and exits. It does **not** reach a message
loop, and it creates no window.

**A correction that matters more than any of this.** An earlier version of this
file reported `message loop: reached` with `PeekMessage=137,295`. That was wrong,
and the way it was wrong is worth keeping: the model let `ExitProcess` *return*,
so the frames above it ran to their own `ret` and execution continued past a fatal
exit. Everything observed after that point was a loop the real game never ran.

**And this block kept reporting the retracted number for a while after the
correction was written** — the correction sat ten lines below a measurement that
contradicted it. If you are reading a figure here that disagrees with
`python3 tools/boot.py`, the tool is right and this file is stale.

**The defect that explains it, now measured.** At the exit the arguments are stub
addresses rather than the numbers the code pushed: `ExitProcess(0x6844df)` where
the code passed `1`, and a `MessageBoxA` type of `0x00a00201` (inside the
import-thunk region) where the code passed `0x30`. Tracing the CRT's atexit walk
over the 3,311-entry static-constructor table shows why:

```
at 0x6844cf (push %esi, the atexit walk):  esp=0x002ff56c
at 0x6844ea (pop  %esi, the same walk):    esp=0x002ff4fc
```

**108 bytes — 27 dwords — leaked across the walk.** After it, `pop %esi` reads a
return address instead of the saved register, ESI becomes `0x6844df`, and

```asm
5e72cf: call *%esi        ; goes to an address inside ExitProcess
5e72d1: mov  -0x5f4(%ebp),%bl
5e72d7: test %bl,%bl
5e72d9: je   0x5e7306     ; zero would skip the error box; it is not zero
```

so the game shows its "Direct X" error box and exits. The corrupted exit code, the
corrupted MessageBox type and the error box are all downstream of those 108 bytes.
The leak is inside one or more of the 3,311 constructors; the diagnostic that names
it must compare ESP at a constructor's own entry against ESP at its `ret`, because
a plain "ESP must return where it started" check also flags `_alloca_probe`
(`0x684230`), which moves ESP by design with `mov %ecx,%esp`.

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
- **Use the oracle before believing the model.** `tools/wine_run.sh` runs the
  retail binary; `tools/oracle.py 0xADDR` reads its real registers. The first thing
  the oracle ever said was that the retail game exits with code 01 before
  DirectDraw surface creation here too -- which invalidated the premise that the
  model was diverging from a *working* game. Check what the real binary does before
  spending a day on a "divergence".
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

Last updated after the transcription, whole-tree-link and gap-closing work.
Every figure here is from `python3 tools/harness.py metrics`; re-run it rather
than trusting this list.

**How far off "complete" is this project.** The layers are far apart. Read this
before quoting any single number. `ROADMAP.md` states each of these as a
measurable exit criterion rather than a percentage.

| Layer | State | Note |
|---|---|---|
| **ABI / layout** | **reproduced, compiler-confirmed** | everything compiles `-m32` at the target ABI, and check 9 makes the compiler confirm every recovered member offset a header can express: 3109/3109. The 1217 offsets with no assertion are itemised by reason (743 vptr stores the compiler inserts itself, 413 past the declared extent, 82 undeclared, 13 no member offset, 6 headerless) |
| Binary recovery | **effectively closed** | 41,822 functions, 96.06% of `.text` as recovered code; the remaining 104 KB is classified — 11 jump tables (256 B) and 356 B of switch index tables emitted as data, 105,808 B of alignment padding, and 554 B in 90 runs reported verbatim rather than forced into a category (`out/text_data.json`) |
| Structural reconstruction | **strong** | 749/749 classes, 9,780 vtable slots, 0 unseeded, every constructor a real entry point |
| Code transcription | **complete** | 43,668 functions, 897,193 of 897,193 instructions (100%) |
| Verification | **9 checks green, pipeline red** | `verify.py` passes all nine in **6 seconds** (263/263, 749/749, 749/749, 219/219, 220/220, 1317/1317, check 9 PASS). `pipeline.sh` is red at the **link test** on 2 unmapped calls, and takes **471s** — the link is the expensive stage, not verification |
| **Semantics** | **~9%** | 5,557 of 9,810 slots carry a name (56.6%), but only **917 distinct names** and **281 distinct semantic** ones — most of the coverage is shared-stub propagation. 291 members named across 20 classes; `e2e_rules.py` resolves 96.2% of them against the shipped `rules.ini` |
| **Modifiability** | **minimal** | there is still no hand-written `.cpp` in the tree; every function is generated machine transcription, so the editable surface is the hand-written headers |
| **Behaviour** | **boots, then exits** | the image maps at its real VAs, 342/342 imports resolve, and the game runs **1,011,817 instructions** of its own CRT startup and init before calling `ExitProcess`. The cause is measured: the atexit walk leaks **108 bytes** across the static-constructor table, so ESI holds a return address and `call *%esi` at `0x5e72cf` goes to garbage. The **retail binary does the same thing here** (exits code 01, before DirectDraw surface creation), so this is not a divergence from a working game |

So: *the binary is explained, its instructions are transcribed, the layout is
compiler-confirmed, and the game's own startup now executes.* It is not a
playable game: there is no window, no message loop, and no renderer. The binding
constraint is no longer recovery, transcription or layout — all three are done.
It is **semantics, source-shaping and the boot**, and the honest headline is that
the source is still barely named and not editable as source.

Two corrections that matter when sizing the rest:

- **The virtual-naming problem is 3,409 distinct slots, not 9,780.** 65.1% of
  (class, slot) pairs are inherited from a base. The top 18 bases cover 13,011
  slot-instances, so naming effort on `ObjectClass`/`AbstractClass`/`TechnoClass`
  and their peers is worth roughly 20 slots per name.
- **`-m32` works here today** (`g++-multilib` installed): 262/262 headers
  compile, the tree links in 1m37s and the entry point runs. The gate just never
  asked. Putting it in the gate is the highest-value small change available.

**The whole recovered instruction stream is transcribed.** `tools/gen_code.py`
renders each recovered function into C++ against the `ts::` machine model in
`src/handwritten/Machine.h` — registers, flags, memory and control flow are
modelled, so the generated code does what the machine does. Each generated line
is one machine instruction and keeps the original as a trailing comment. It emits
one canonical `ts::sub_XXXXXXXX(void)` per function VA, plus 1,846 **tail
entries**: addresses a cross-function branch targets that are not function
starts, which were previously dispatch gaps. The per-class files in
`src/generated/` are **forwarders** that call those, so one machine function has
one body.

`tools/link_test.py` links all 482 translation units into a program, registers
every recovered function, and executes the original PE entry point against the
model. **That link is not decoration**: a per-file syntax check cannot see a
forwarder naming a symbol nothing defines, and the tree did not link for months
because no check looked.

Honest limits, all stated in `src/generated/README.md`:

- 0 instructions have no faithful rendering. The last 166 (MMX/SSE, the x87
  environment save/restore, far pointers, `lar`, `fbld`, `fcmovnbe`, `pushaw`)
  are rendered against the machine model: 897,193 of 897,193 instructions are
  modelled and the transcription gate still verifies every function
  instruction-for-instruction. Two limits of the *model* remain, stated where
  they live in `Machine.h`: `lar` reports the flat ring-3 code segment because
  the model has no descriptor table, and `fnsave`/`fnstenv` write zero into the
  two x87 pointer fields because the model does not track them.
- 228 destructors are **labelled stubs**. The headers declare them; no comment
  records where they are, so there is no transcription to forward to.
- The entry-point run stops at import thunks, because the model never loads the
  image's data sections and `call dword ptr [IAT]` therefore reads 0.

It is **not** source-shaped decompilation: the generated code is faithful to what
the machine does and is not claimed to be the shape the original source had.
A transcription reads its arguments from the model, not from C++ parameters, so
hand-written callers reach the right code through `ts::ts_invoke` but the values
they pass are only meaningful when the caller is itself a transcription.

**Done and verified.** RTTI recovery (788 type descriptors, 755 hierarchies,
843 vtables), disassembly (41,822 functions, 96.06% of `.text`,
`vtable_slots_unseeded` 0), class manifest (749 classes, 9,810 slots, every
constructor a **real function entry point**), direct-base reduction, 4,326 member
offsets recovered (99.9% confirmed against the binary by disassembling the
constructors), 1,567 named virtuals with a per-name evidence file, 135 member
names from INI keys, headers for all 749 classes, and the whole recovered
instruction stream transcribed (43,668 functions including 1,846 tail entries,
6,996 forwarders across 309 translation units).

The pipeline reproduces all of it from `bin/GAME.EXE` alone and
`tools/verify.py` runs **nine** checks: 261/261 standalone at `-m32`, combined TU
PASS at `-m32`, 749/749 coverage, 749/749 vtable bindings, 219/219 header
content, 220/220 inheritance, 2186/2186 member offsets across 248 classes,
check 8 reporting declared-vs-recovered virtual arity (reported, not gated),
and check 9 compiling `offsetof` assertions for the recovered member offsets at
`-m32`, which currently confirms **1 of 3,090** — the vptr-emitted-as-a-data-
member defect (ROADMAP P0.2), not a broken check. The transcription
faithfulness gate verifies all 43,668 functions instruction-for-instruction, and
`tools/test_tools.py` runs 36 checks with four mutation-proven.
`tools/harness.py determinism --run` reports **497** artifacts byte-identical
across two clean runs — the recovered data *and* the generated C++.

**Real shipped game data is committed** under `realdata/` (rules.ini, ai.ini,
art.ini, sound.ini, theme.ini, BATTLE.INI, MISSION.INI, ~1 MB), extracted from
`INSTALL/TIBSUN.MIX`. `tools/e2e_rules.py` drives the reconstructed model with it
and checks the recovered field names against shipped keys: 110 of 112 resolve
(101 as `key=value`, 9 only in the documented comment header). This is the only
validation that compares the reconstruction against something outside the
project's own artifacts. It is committed deliberately so a fresh clone can run
it without the 2.7 GB of CD images.

**Closing a coverage gap is a test, not just more output.** Widening the
transcription from 2,845 functions to all 41,822 — and then linking the result —
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
they actually mean (the virtual-naming problem is 3,409 distinct slots, not
9,810, because 65% are inherited), and gives the phase order. todo.md is the
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

- Check 4 and `tools/verify_members_binary.py` are the only checks anchored to
  the binary. Checks 5–7 compare the tree against other tools' output
  (`out/class_manifest.json`, `out/direct_bases.json`, `out/members.json`), so a
  bug in `gen_manifest.py` would make the manifest and the headers wrong
  *together* and those checks would still pass.
- `tools/verify_members_binary.py` disassembles each constructor in the binary
  and confirms claimed offsets against real stores. It examines 637 classes and
  4,326 claims, confirms **99.9%** (4,322 confirmed, 4 refuted, 0 uncheckable)
  and runs as a **gate** with a 95% floor. The 4 refuted are two offsets in each
  of `ScoreTimeClass` and `ScorePrintClass`. It caught a real defect earlier: a
  store site buried inside a large general-purpose function was being accepted
  as the constructor, which misattributed member claims for 180 of 585 classes.
  A plausibility gate now requires the vtable write to occur early in the
  function through a non-stack register, which cut false claims from 1,052 to 4.
- Check 7 covers **248 classes and 2,186 offsets** (it previously skipped
  hand-written headers and reported only 6 classes). It verifies the
  machine-generated `m_<hex>`/`vptr_<hex>` declarations against the recovered
  table. It deliberately does **not** gate *curated* hand-written names
  (`vptr`, `NextStage`, `m_Field474`): those carry their own ctor evidence and
  are often richer than the automated table, so a disagreement is reported
  rather than treated as an error.
- Check 8 reports declared-vs-recovered virtual arity: 478 of 1,172 agree, 694
  diverge. It is **reported, not gated**, because `nargs` counts `this` for COM
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

  **What it actually does here, verified with a screenshot:** it creates a
  1024x768 "Tiberian Sun" window, shows `Warning - Unable to create Direct Sound
  Object. Proceeding with audio disabled.`, walks a second dialog after that is
  dismissed, and then **exits**. So the oracle is real but bounded: it reports the
  retail call sequence up to the point where the game gives up, and it gives up
  early in this environment because there is no audio device, no CD mounted and no
  install registry. Do not read "runs under Wine" as "is playable under Wine".
  Mounting the CD and satisfying the install is the work needed to extend the
  oracle past startup, and it is worth doing before trusting any claim here about
  what the game does after init.
- **Test the tools, not only their output.** `tools/verify.py` checks artifacts;
  a tool can be wrong in a way that produces self-consistent output and nothing
  notices. `tools/test_tools.py` holds 36 checks, each a regression test for a
  defect that shipped or a differential test against an independent
  implementation, and four of them are mutation-proven to fail when the defect
  is reintroduced. If a test cannot fail, it is not worth the runtime.
- **A gate that has never failed is not a gate.** Every gate in this repo is now
  proven by reintroducing the defect it is meant to catch and confirming it
  fails. When you add one, break the thing on purpose and watch it fail — the
  two-check transcription gate was written because a mutation showed the
  record-only check could not see a corrupted statement.
- **Closing a coverage gap finds bugs in code that was already there.** Extending
  the transcription from 2,845 functions to all 41,822 exposed six defects in the
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
confirms the claimed displacement appears in a real store, at 99.9% (4,322 of
4,326, 4 refuted, 0 uncheckable). What is **not** verified is member *type* and
*ordering* — whether a 4-byte slot is really an `int` rather than a pointer, or
whether two members are the right way round. Hand-written inheritance is reported
separately in check 6, not gated, because those headers are the authoritative
richer source and legitimately omit transitively-implied bases.

**Known open defects** are listed in `todo.md` with evidence. The two largest
now are: **5,420 virtuals and ~4,190 members are unnamed** — the source compiles,
links and runs, but a reader still meets `vfunc_247` more often than a name; and
**the transcription is not source-shaped**, so a function reads as one machine
instruction per line rather than as the C++ the original author wrote. The
slot-0 mislabel is fixed (the generator now detects shared slot-0 stubs and
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
