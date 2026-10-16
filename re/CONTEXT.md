# Tiberian Sun Reverse Engineering — Shared Context

## Target
`re/bin/GAME.EXE` — Command & Conquer: Tiberian Sun (Westwood Studios, 1999).
- SHA256 `2941b01998da0cd2bb0b7db30125d4130e3876a74d6cc5588b630c6d5fac5a12`
- PE32 i386, MSVC 6.0, ImageBase 0x400000, EntryPoint 0x6854e3
- Sections: .text 0x401000 (2.7MB), .rdata 0x698000, .data 0x6bb000, .rsrc 0x837000
- Original source path embedded in binary: `D:\Projects\TSun\Code\`
- Stripped of symbols, but full MSVC RTTI is present.

## Recovered artifacts (in `re/out/`)

Counts are approximate and drift as the tools improve; run
`python3 tools/harness.py metrics --no-append` for the current figures.

- `classes.json` — RTTI classes, inheritance edges, object locators, vtables
- `hierarchy.txt` — human-readable class hierarchy with base-class displacements
- `vtables.txt` — vtable VA -> class binding
- `functions.json` — recovered functions with full instruction listings,
  callees and string references
- `class_manifest.json` — per-class vtable, constructor and base set
- `members.json` — member offsets recovered from constructor store sequences
- `signatures.json` — calling conventions and arity
- `ini_keys.json` — strings referenced by code, including `rules.ini` field names
## Tools (in `re/tools/`)

The full pipeline is `./tools/pipeline.sh`. The main tools:

- `rtti.py` — RTTI recovery
- `demangle.py` — MSVC RTTI name demangler
- `disasm.py` — function boundary + call graph recovery
- `gen_manifest.py` — per-class vtable, constructor and base set
- `gen_members.py` / `name_members.py` — member offsets, and names from INI keys
- `gen_signatures.py` / `name_virtuals.py` — calling conventions and virtual names
- `gen_headers.py` — header emission
- `gen_code.py` — the canonical transcription of every recovered function into
  `src/generated/code/`, against the machine model in `src/handwritten/Machine.h`
- `gen_bodies.py` — the declaration-shaped forwarders the headers require
- `emit_x86.py` — the operand grammar and per-mnemonic rendering both use
- `verify.py` — the 8-check fail-closed gate
- `verify_members_binary.py` — re-derives member offsets from the binary's own code
- `link_test.py` — links the whole tree into a program and runs it
- `test_tools.py` — unit tests for the tools themselves
- `harness.py` — measures real state (`metrics`) and proves reproducibility
  (`determinism`)

**Measure, do not trust prose.** `python3 tools/harness.py metrics` reports the
real state; `python3 tools/verify.py src/include build/shim` is the gate.
## Key facts for reconstruction
1. **RTTI gives exact class names and inheritance.** Use `hierarchy.txt`.
2. **INI keys are the original struct field names.** e.g. `TiberiumChainReaction`,
   `HalfDamageSmokeLocation1`, `Capturable`, `Unsellable`, `CustomRotor`.
   These come from `rules.ini` parsing code and map to `TechnoTypeClass` /
   `BuildingTypeClass` / `UnitTypeClass` members.
3. **Vtable layout gives virtual function order.** Each vtable entry is a
   function VA in `functions.json`.
4. **MSVC 6 calling convention**: `__thiscall` for member functions (this in ECX),
   `__stdcall` for API, `__cdecl` for varargs.
5. **Known original module names** from embedded strings:
   `Connect.CPP`, `DSAudio.Cpp`, `Queue.CPP`, `wwmous.cpp`.

## Class hierarchy anchors (verified from RTTI)
```
AbstractClass
  -> AbstractTypeClass
       -> ObjectTypeClass
            -> TechnoTypeClass
                 -> BuildingTypeClass, InfantryTypeClass, UnitTypeClass,
                    AircraftTypeClass, VesselTypeClass
       -> TerrainTypeClass, SmudgeTypeClass, OverlayTypeClass, ...
  -> ObjectClass
       -> TechnoClass
            -> BuildingClass, InfantryClass, UnitClass, AircraftClass, VesselClass
       -> TerrainClass, SmudgeClass, OverlayClass, AnimClass, BulletClass
```
Locomotion classes: DriveLocomotionClass, FlyLocomotionClass, WalkLocomotionClass,
JumpjetLocomotionClass, HoverLocomotionClass, MechLocomotionClass,
TeleportLocomotionClass, TunnelLocomotionClass, DropPodLocomotionClass.

## The machine model

`src/handwritten/Machine.h` models the x86 machine the recovered code ran on:
registers with sub-register views, flag semantics for every operation emitted,
lazily-paged memory, the stack, the x87 register stack, string ops, segment
registers and port I/O. Transcribed bodies read their arguments from this model,
**not** from their C++ parameter lists — the parameter list is the reconstructed
interface, the model is where the values actually are. Hand-written callers reach
recovered code through `ts::ts_invoke`; generated forwarders call the
transcription by name and read the result from `ts::ret_eax()`.

## Output conventions
- Reconstructed headers go in `re/src/include/`
- Generated code goes in `re/src/generated/` — never hand-edit it
- Reconstructed sources go in `re/src/`
- Every reconstructed declaration must cite its evidence (RTTI name, vtable VA,
  INI key, or string VA) in a comment.
- Do NOT invent members that have no evidence. Mark uncertain ones `/* ? */`.
