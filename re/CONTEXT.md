# Tiberian Sun Reverse Engineering — Shared Context

## Target
`re/bin/GAME.EXE` — Command & Conquer: Tiberian Sun (Westwood Studios, 1999).
- SHA256 `2941b01998da0cd2bb0b7db30125d4130e3876a74d6cc5588b630c6d5fac5a12`
- PE32 i386, MSVC 6.0, ImageBase 0x400000, EntryPoint 0x6854e3
- Sections: .text 0x401000 (2.7MB), .rdata 0x698000, .data 0x6bb000, .rsrc 0x837000
- Original source path embedded in binary: `D:\Projects\TSun\Code\`
- Stripped of symbols, but full MSVC RTTI is present.

## Recovered artifacts (in `re/out/`)
- `classes.json` — 755 classes, 1291 inheritance edges, 842 object locators, 843 vtables
- `hierarchy.txt` — human-readable class hierarchy with base-class displacements
- `vtables.txt` — vtable VA -> class binding
- `functions.json` — 9057 functions with full instruction listings, callees, string refs
- `ini_keys.json` — 1209 strings referenced by code, incl. 988 rules.ini field names

## Tools (in `re/tools/`)
- `rtti.py` — RTTI recovery (run: `python3 tools/rtti.py bin/GAME.EXE out`)
- `demangle.py` — MSVC RTTI name demangler
- `disasm.py` — function boundary + call graph recovery

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

## Output conventions
- Reconstructed headers go in `re/src/include/`
- Reconstructed sources go in `re/src/`
- Every reconstructed declaration must cite its evidence (RTTI name, vtable VA,
  INI key, or string VA) in a comment.
- Do NOT invent members that have no evidence. Mark uncertain ones `/* ? */`.
