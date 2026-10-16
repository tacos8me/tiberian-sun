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
| `tools/verify.py` | full verification gate |

Run order is encoded in `tools/pipeline.sh` (10 stages). Do not run stages by
hand out of order; `out/` artifacts feed forward.

`verify.py`'s coverage check credits a template instantiation to the header that
defines its template: 487 of the 749 manifest entries are instantiations
(`VectorClass`, `DynamicVectorClass`, `TClassFactory`, `TypeList`, `Blit*`), and
the template header legitimately defines them all. This is not a loosened gate —
do not satisfy it by emitting one header per instantiation.

## Build / verify

The gate is `tools/verify.py`, which checks standalone compilation of every
header, a combined translation unit, class coverage against the manifest, and
vtable→class bindings:

```bash
cd re
./tools/pipeline.sh            # regenerate everything, then verify
python3 tools/verify.py src/include build/shim
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
- Give reconstruction agents **tight, bounded scopes** and tell them explicitly
  not to write giant inline heredocs — one agent previously burned its entire
  budget that way and produced nothing.
- Parallel writers must target **disjoint file sets**.

## Current status

Last updated after the P0 tooling fixes: `VERIFICATION PASSED` with five checks,
and disassembly coverage roughly doubled.

**Done and verified.** RTTI recovery (788 type descriptors, 755 hierarchies,
843 vtables), disassembly (23,129 functions, 63.7% of `.text` bytes), full class
manifest (749 classes, 9,810 virtual slots, every class has a located
constructor that is a **real function entry point**), direct-base reduction,
member offsets, member names from INI keys (135 across 20 classes), signatures
(8,689 functions with calling conventions), and headers for all 749 classes with
1,610 named virtuals. The pipeline reproduces all of it from `bin/GAME.EXE`
alone and `tools/verify.py` passes **seven** checks: 259/259 standalone, combined
TU PASS, 749/749 coverage, 749/749 vtable bindings, 219/219 header content,
218/222 inheritance, 10/10 member offsets.

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

It still does **not** prove the disassembly is correct, and check 7 compares
only the handful of classes that actually emit offsets. Hand-written headers
are reported separately in check 6, not gated, because they are the
authoritative richer source and legitimately omit transitively-implied bases.

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
