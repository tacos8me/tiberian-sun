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
  out/                            recovered data (JSON, text)
  src/include/                    reconstructed C++ headers
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
   a tool in `tools/`, fix the tool and re-run it.

## Toolchain

| Tool | Purpose |
|---|---|
| `tools/rtti.py` | MSVC 6 RTTI recovery → classes, vtables, COLs |
| `tools/demangle.py` | MSVC mangled-name demangler |
| `tools/disasm.py` | function boundary + call-graph recovery |
| `tools/gen_manifest.py` | per-class vtable/ctor manifest |
| `tools/gen_headers.py` | header emission from the manifest |

Run order: `rtti.py` → `disasm.py` → `gen_manifest.py` → `gen_headers.py`.

## Build / verify

```bash
cd re
g++ -std=c++03 -fsyntax-only -Isrc/include -Ibuild/shim build/tu_all.cpp
```

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

- Background/async subagents require `@earendil-works/chord`. It is not part of
  the default install; `1.1.0` is installed under
  `pi-coding-agent/node_modules/@earendil-works/chord`. If async children fail
  with a chord error, run the workflow with `async: false`.
- Give reconstruction agents **tight, bounded scopes** and tell them explicitly
  not to write giant inline heredocs — one agent previously burned its entire
  budget that way and produced nothing.
- Parallel writers must target **disjoint file sets**.

## Current status

See `PROGRESS.md` for the live state and the remaining work list.
