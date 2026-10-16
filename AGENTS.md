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
  build/                          compile harness + platform shim
  REPORT.md                       results and verification status
  PROGRESS.md                     current state and remaining work
  CONTEXT.md                      briefing for reconstruction agents
```

## Ground rules

1. **Evidence or it doesn't go in.** Every declaration must cite its source:
   RTTI descriptor VA, vtable VA, constructor VA, INI key string, or function
   VA. Never invent a member, name, or type.
2. **Mark uncertainty explicitly.** Use `/* ? */` for anything inferred but not
   proven. Use `vfunc_N` for virtuals whose semantics aren't established.
   Honest gaps are worth more than confident guesses.
3. **Verify against the binary, not against the previous agent's output.**
   Re-resolve vtable→class bindings through the COL chain. Re-read the bytes.
4. **Keep the build green.** `build/tu_all.cpp` must compile after every change.
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
| `tools/gen_headers.py` | header emission from the manifest |
| `tools/reconcile_prelude.py` | prelude/header base reconciliation |
| `tools/make_selfcontained.py` | add missing includes per header |
| `tools/verify.py` | full verification gate (8 checks, fail-closed) |
| `tools/harness.py` | measure real state (`metrics`) and prove reproducibility (`determinism`) |
| `tools/gen_bodies.py` | emit `.cpp` member definitions, de-duplicated by function VA, with a mutation-proven faithfulness gate (`--check`) |
| `tools/verify_members_binary.py` | re-derive member offsets from the binary's own code; the only member check not trusting `out/` |
| `tools/extract_realdata.py` | extract the shipped INI control files from `TIBSUN.MIX` with disjoint byte ranges |
| `tools/e2e_rules.py` | drive the reconstructed model with real shipped data and check recovered field names against it |
| `tools/mix.py` | MIX archive reader (reads the headerless TD/RA1 variant; the encrypted TS header is not yet decrypted and fails explicitly) |

Run order is encoded in `tools/pipeline.sh`. Do not run stages by hand out of
order; `out/` artifacts feed forward. The stages are: 0 clean tree, 1 RTTI,
2 disassembly, 3 strings, 4 class manifest, 5 direct bases, 6 members,
7 member naming, 8 signatures, 8b virtual naming, 9 header generation,
10 prelude reconciliation, then function bodies, verification and metrics.

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
python3 tools/harness.py metrics --no-append   # real measured state
python3 tools/harness.py determinism --run     # two clean runs, byte-diffed
```

A change is not done until `VERIFICATION PASSED` is printed.

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

Last updated after the coverage, naming, archive and body-de-duplication work.
Every figure here is from `python3 tools/harness.py metrics`; re-run it rather
than trusting this list.

**How far off "complete" is this project.** Three layers, and they are far
apart. Read this before quoting any single number.

| Layer | State | Note |
|---|---|---|
| Binary recovery | **~96%** | 41,822 functions, 96.06% of `.text` attributed; 104 KB left |
| Structural reconstruction | **~95%** | 749/749 classes, 9,810 vtable slots, 0 unseeded, every constructor a real entry point |
| Verification | **strong but bounded** | 8 checks, fail-closed, 2 anchored to the binary, determinism proven |
| **Semantics** | **~20%** | 22% of virtuals named, ~3% of members named |
| **Behaviour** | **0%** | all 6,670 bodies are EMPTY definitions |

So: *the binary is nearly explained; the source is a fifth explained; nothing
runs.* If "complete decompilation" means compilable, documented, change-ready
source that reproduces the game, the honest figure is **~35–40%**. The single
largest gap is function bodies — the shadow-register transcription was designed
and never landed.

**Done and verified.** RTTI recovery (788 type descriptors, 755 hierarchies,
843 vtables), disassembly (41,822 functions, 96.06% of `.text`,
`vtable_slots_unseeded` 0), class manifest (749 classes, 9,810 slots, every
constructor a **real function entry point**), direct-base reduction, 4,326 member
offsets recovered (99.9% confirmed against the binary by disassembling the
constructors), 6,670 member definitions across 303 generated translation units
(2,845 distinct functions; the rest are explicit aliases), 1,522 named virtuals
with a per-name evidence file, 135 member names from INI keys, and headers for
all 749 classes.

The pipeline reproduces all of it from `bin/GAME.EXE` alone and
`tools/verify.py` passes **eight** checks: 260/260 standalone, combined TU PASS,
749/749 coverage, 749/749 vtable bindings, 219/219 header content, 220/220
inheritance, 2186/2186 member offsets across 248 classes, and check 8 reporting
declared-vs-recovered virtual arity (reported, not gated). The gen_bodies
faithfulness gate verifies 2,845 distinct functions instruction-for-instruction.
`tools/harness.py determinism --run` reports 13 artifacts byte-identical across
two clean runs.

**Real shipped game data is committed** under `realdata/` (rules.ini, ai.ini,
art.ini, sound.ini, theme.ini, BATTLE.INI, MISSION.INI, ~1 MB), extracted from
`INSTALL/TIBSUN.MIX`. `tools/e2e_rules.py` drives the reconstructed model with it
and checks the recovered field names against shipped keys: 110 of 112 resolve
(101 as `key=value`, 9 only in the documented comment header). This is the only
validation that compares the reconstruction against something outside the
project's own artifacts. It is committed deliberately so a fresh clone can run
it without the 2.7 GB of CD images.

**How to measure instead of guessing.** `python3 tools/harness.py metrics`
prints the real state and appends to `out/metrics.jsonl`;
`python3 tools/harness.py determinism --run` proves two clean runs are
byte-identical. Use these before and after any change.

**Read `todo.md` before starting work.** It is the prioritized, evidence-backed
work list, and it records defects found by the last review that are *not* yet
fixed. Do not read the status above as "everything is fine".

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
  and confirms claimed offsets against real stores. It confirms **99.9%**
  (3,571 of 3,575 claims, 4 refuted, 0 uncheckable) and runs as a **gate** with
  a 95% floor. It caught a real defect: a store site buried inside a large
  general-purpose function was being accepted as the constructor, which
  misattributed member claims for 180 of 585 classes. A plausibility gate now
  requires the vtable write to occur early in the function through a non-stack
  register, which cut false claims from 1,052 to 4.
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
- `tools/gen_bodies.py` runs a **faithfulness gate** on its own output: every
  instruction of every transcribed function must appear exactly once, in order,
  with matching bytes. It is proven by mutation — deleting one instruction from
  `AbstractClass.cpp` makes `--check` exit 1 naming
  `0x006829a5 (AbstractClass::QueryInterface): 1/4 records match`. Semantic
  equivalence cannot be proven here (there is no MSVC 6 toolchain, only g++), so
  faithful rendering is the strongest claim available and it is checkable.

Member **offsets** are the one piece of layout that IS verified against the
binary: `tools/verify_members_binary.py` disassembles each constructor and
confirms the claimed displacement appears in a real store, at 99.9% (3,571 of
3,575, 4 refuted). What is **not** verified is member *type* and *ordering* —
whether a 4-byte slot is really an `int` rather than a pointer, or whether two
members are the right way round. Hand-written inheritance is reported separately
in check 6, not gated, because those headers are the authoritative richer source
and legitimately omit transitively-implied bases.

**Known open defects** are listed in `todo.md` with evidence; the two most
substantial are the slot-0 mislabel (a shared `E_POINTER` stub is labelled
`virtual ~X(); // [0]` in ~66 generated and hand-written headers, of which only
`AbstractClass` has been fixed) and the fact that every function body is still
empty.

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
than its own header. The MS* multimedia classes, the WOL/ATL COM layer, and
`.cpp` stub bodies are not yet reconstructed.

See `PROGRESS.md` for the live work list.
