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
`python3 tools/harness.py metrics --no-append` for the current figures. The
figures in this file were re-measured on 2026-10-17.

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
- `verify.py` — the **9-check** fail-closed gate, compiled at the target ABI
  (`-m32`); check 9 makes the compiler confirm recovered member offsets with
  `offsetof`
- `verify_members_binary.py` — re-derives member offsets from the binary's own
  code (4,285 of 4,289 confirmed, 99.9%)
- `link_test.py` — links all 492 translation units into a program and runs it;
  measured **FAILED, exit 1**, message loop reached and dispatching
  (`GetMessage=2`, `PeekMessage=5`, `DispatchMessage=2`), **10 unmapped calls
  inside `.text`**
- `test_tools.py` — 43 unit tests for the tools themselves
- `harness.py` — measures real state (`metrics`) and proves reproducibility
  (`determinism`)
- `gen_win32_stubs.py` — generates `Win32Stubs.h` from the PE import directory
  (341 imports across 13 DLLs; the stub table has 342 rows because it also
  carries `COMCTL32!DllGetVersion`, reached through `GetProcAddress`). It
  refuses to write when a `TS_RET` body's argument count disagrees with `NARGS`;
  `--evidence` reports per-ordinal agreement.
- `boot.py` — builds and runs the image against the model. Measured:
  **20,000,001 instructions** (budget) at `0x006844db` (`_exit`/atexit walk),
  `CreateWindowExA=1`, `GetMessage=2`, `PeekMessage=5`, `DispatchMessage=2` —
  message loop **reached and dispatching**; only `MessageBoxA` is the DirectSound
  warning at `0x484456` (`type=0x30`); **70 unmapped calls (10 inside `.text`)**;
  coverage 51,604 distinct instructions / 4,535 of 46,008 functions; primary
  surface `TS_PEEK=0x006beed8`=1; WinMain returns ~7.66M with ESP `0x11`.
  `--no-build --strict` exits **3**.
- `wine_run.sh` / `wine_env.sh` — run the **retail** binary under Wine.
  `wine_env.sh` supplies the CD (`d:`, label `TS1`, `TS1.DSK`) and a
  640x480x16 screen; measured, the retail game reaches its **main menu**
  (screenshot md5 `58cb9c0bb64a1bf11c6e9b28e96eca11`, still running after 60 s).
- `oracle.py` — attach gdb to the retail binary, parse its registers and stack,
  and `--compare` against the model at the same address
- `oracle_blowfish.py` / `wine_blowfish_trace.sh` — the tooling behind
  `out/blowfish_oracle.md`, the measured retail spec for `blowfish.dll`. It
  establishes that the registered object is a **real Blowfish cipher the game
  uses**: 9 `SetKey(56,key)` and 1,586 `Decrypt(8,in,out)` calls during startup,
  matching nine shipped MIX indexes. The model **implements standard
  Blowfish-ECB** (`345aae7`), proven by four known-answer vectors and a
  byte-identical `tools/mix.py` cross-check on the shipped TIBSUN.MIX index
  (`2ce3171bdbecb40e` → `110090d97904849a`, count 17, body 75,094,416).

**Current runtime defects** (queued in `todo.md`): the MIX index read stops after
2 of 27 blocks (`0x547435`/`0x547426`); shutdown corrupts ESP (WinMain returns
with `0x11`); null teardown calls produce 70 unmapped calls, 10 inside `.text`.

**Measure, do not trust prose.** `python3 tools/harness.py metrics --no-append`
reports the real state; `python3 tools/verify.py src/include build/shim` is the
gate.
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
