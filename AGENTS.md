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
| `tools/verify.py` | full verification gate (8 checks, fail-closed) |
| `tools/harness.py` | measure real state (`metrics`) and prove reproducibility (`determinism`) |
| `tools/gen_code.py` | the canonical transcription of every recovered function, plus the registration unit and tail entries; mutation-proven faithfulness gate (`--check`) |
| `tools/gen_bodies.py` | the declaration-shaped forwarders the reconstructed headers require, de-duplicated by function VA, gated to name a defined transcription |
| `tools/emit_x86.py` | the operand grammar and per-mnemonic rendering used by both generators; the only place that knows the disassembler's operand text |
| `tools/link_test.py` | link the whole tree into a program and run it; the only check that can see an undefined symbol |
| `tools/check_compile.py` | compile a set of generated TUs standalone and fail if any does |
| `tools/test_tools.py` | unit tests for the load-bearing tools; every one is a regression test for a shipped defect |
| `tools/verify_members_binary.py` | re-derive member offsets from the binary's own code; the only member check not trusting `out/` |
| `tools/extract_realdata.py` | extract the shipped INI control files from `TIBSUN.MIX` with disjoint byte ranges |
| `tools/e2e_rules.py` | drive the reconstructed model with real shipped data and check recovered field names against it |
| `tools/mix.py` | MIX archive reader (reads the headerless TD/RA1 variant; the encrypted TS header is not yet decrypted and fails explicitly) |

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

The gate is `tools/verify.py`. It runs eight checks: standalone compilation of
every header, a combined translation unit, class coverage against the manifest,
exhaustive vtable→class bindings resolved through the binary's own RTTI COL
chain, header-content-vs-manifest, inheritance-vs-direct-bases, member
offsets-vs-recovered-table, and declared-vs-recovered virtual arity. It is
fail-closed: a missing artifact makes it exit non-zero naming what is missing
rather than running a subset and printing PASSED.

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
| **ABI / layout** | **not reproduced** | host build is LP64 and the reconstructed classes model the vptr as a data member, so the compiled structs do not have the game's offsets. `-m32` fixes the width problem and is already proven to compile, link and run; the vptr problem is the real one |
| Binary recovery | **effectively closed** | 41,822 functions, 96.06% of `.text`; the remaining 104 KB is verified jump tables and padding, not unknown code |
| Structural reconstruction | **strong** | 749/749 classes, 9,780 vtable slots, 0 unseeded, every constructor a real entry point |
| Code transcription | **complete** | 43,668 functions, 897,027 of 897,193 instructions (99.98%) |
| Verification | **strong but bounded** | 8 checks, fail-closed, 2 anchored to the binary, whole-tree link, determinism over 497 artifacts |
| **Semantics** | **~5%** | 497 of 9,780 slots have a genuinely semantic virtual name (1,500 total minus 600 destructors and 403 COM boilerplate); 135 of 3,273 real members named |
| **Modifiability** | **minimal** | there is no hand-written `.cpp` in the tree; every function is generated machine transcription, so the editable surface is 47 headers |
| Behaviour | **partial** | the tree links and the original entry point runs against the model; it stops at import thunks |

So: *the binary is explained, its instructions are transcribed, and the tree
links and runs — but the source is barely named and not editable as source.* The
binding constraint is no longer recovery (finished) or transcription (finished).
It is **semantics and source-shaping**, and the honest headline is that the
largest single piece of remaining work has not been started.

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

- 166 instructions (0.02%) have no faithful rendering and are emitted as
  `ts::unmodelled(va, "mnem")` rather than approximated: MMX/SSE, the x87
  environment save/restore (`fnsave`/`frstor`/`fnstenv`/`fldenv`), far pointers
  (`les`/`lds`) and `lar`.
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
`tools/verify.py` passes **eight** checks: 261/261 standalone, combined TU PASS,
749/749 coverage, 749/749 vtable bindings, 219/219 header content, 220/220
inheritance, 2186/2186 member offsets across 248 classes, and check 8 reporting
declared-vs-recovered virtual arity (reported, not gated). The transcription
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
  three exit 1. Semantic equivalence with the original cannot be proven here
  (there is no MSVC 6 toolchain, only g++), so faithful rendering is the strongest
  claim available — and it is checkable.
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
