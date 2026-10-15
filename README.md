# Tiberian Sun — Disassembly to Source

A reverse-engineering project that takes the retail PC release of
**Command & Conquer: Tiberian Sun** (Westwood Studios, 1999) and reconstructs
it as compilable, documented C++ source.

## Status

| Layer | State |
|---|---|
| Binary recovery (CD → ISO → PE) | complete |
| RTTI / class hierarchy recovery | complete, verified |
| Disassembly + call graph | complete |
| INI schema recovery | complete |
| Source reconstruction | **in progress** — see [PROGRESS.md](re/PROGRESS.md) |

The recovered data layer is complete and independently verified. The source
layer is being filled in systematically; `re/PROGRESS.md` tracks exactly what
is done and what remains.

## What was recovered

- **755** classes with exact inheritance, from intact MSVC 6 RTTI
- **843** vtables bound to their owning class
- **9,057** functions / 275,283 instructions
- **1,209** referenced strings, including **988 `rules.ini` field names** —
  the original struct member names, recovered verbatim

## Layout

```
re/
  bin/GAME.EXE        target binary
  tools/              recovery + generation tooling
  out/                recovered data (JSON)
  src/include/        reconstructed C++ headers
  build/              compile harness
  REPORT.md           results and verification
  PROGRESS.md         live status and remaining work
```

## Reproducing

```bash
cd re
python3 tools/rtti.py bin/GAME.EXE out        # RTTI recovery
python3 tools/disasm.py bin/GAME.EXE out      # disassembly
python3 tools/gen_manifest.py bin/GAME.EXE out
python3 tools/gen_headers.py bin/GAME.EXE out src/include
```

## Verifying

```bash
cd re
g++ -std=c++03 -fsyntax-only -Isrc/include -Ibuild/shim build/tu_all.cpp
```

## Method

The binary is stripped of symbols but retains complete MSVC 6 RTTI. The
`type_info` and Complete Object Locator layouts were determined empirically
from the binary rather than assumed, and a recursive MSVC name demangler was
written for the observed mangling grammar. Every declaration in the
reconstructed source cites its evidence: an RTTI descriptor address, a vtable
address, a constructor address, an INI key string, or a function address.

Uncertainty is marked explicitly (`/* ? */`, `vfunc_N`) rather than guessed at.

## Contributing / working on this

See [AGENTS.md](AGENTS.md) for ground rules, toolchain, and technical notes.

## Legal

Tiberian Sun and its assets are © 1999 Electronic Arts / Westwood Studios.
This repository contains only reconstructed source and derived data; the
original game binaries are included solely as the reference target.
