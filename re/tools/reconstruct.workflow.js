// Parallel reconstruction workflow for Tiberian Sun.
// Three independent workstreams writing to disjoint output paths.

const common = `
You are reconstructing original C++ source from a disassembled 1999 game binary.

WORKING DIR: /mnt/nvme-1/projects/tiberian/re
READ FIRST: CONTEXT.md (full briefing), out/hierarchy.txt, out/vtables.txt

Available artifacts:
- out/classes.json    : 755 classes, inheritance edges, 842 COLs, 843 vtables
- out/hierarchy.txt   : readable class hierarchy with base displacements
- out/vtables.txt     : vtable VA -> class name
- out/functions.json  : 9057 functions, each with {va,end,size,ninsn,callees,insns[],strings[]}
- out/ini_keys.json   : 1209 referenced strings incl. 988 rules.ini field names
- tools/demangle.py   : MSVC RTTI demangler (importable)
- bin/GAME.EXE        : the binary (PE32, ImageBase 0x400000)

Useful commands:
  python3 -c "import json; d=json.load(open('out/functions.json')); ..."
  python3 -c "import json; d=json.load(open('out/classes.json')); ..."
  objdump -D -b pei-i386 -M intel --start-address=0xADDR --stop-address=0xADDR2 bin/GAME.EXE

RULES:
- Every declaration must cite evidence in a comment: RTTI name, vtable VA,
  INI key string, or string VA. No invented members.
- Mark uncertain items with /* ? */.
- Write ONLY to your assigned output paths. Do not touch other agents' files.
- Prefer real recovered names over guesses. The RTTI names ARE the original names.
`;

const tasks = [
  {
    key: "headers",
    agent: "worker",
    task: common + `
YOUR TASK: Reconstruct the core class hierarchy headers.

Write these files (and only these):
  src/include/AbstractClass.h
  src/include/ObjectTypeClass.h
  src/include/TechnoTypeClass.h
  src/include/ObjectClass.h
  src/include/TechnoClass.h
  src/include/BuildingTypeClass.h
  src/include/UnitTypeClass.h
  src/include/InfantryTypeClass.h
  src/include/AircraftTypeClass.h
  src/include/HouseClass.h
  src/include/HouseTypeClass.h
  src/include/INDEX.md

METHOD:
1. Read CONTEXT.md and out/hierarchy.txt.
2. For each class, extract its exact base classes and their mdisp/pdisp/vdisp
   from out/classes.json (field "bases"). Emit the real inheritance.
3. For each class, find its vtable in out/vtables.txt, then read the vtable
   entries from the binary to enumerate virtual functions IN ORDER. Each entry
   is a function VA; look it up in out/functions.json to see what it does and
   give it a plausible name based on its strings/callees.
4. For data members: use the INI keys in out/ini_keys.json. The functions that
   reference keys like "TiberiumChainReaction" or "Capturable" are the
   TechnoTypeClass/BuildingTypeClass INI readers; the order and types of the
   fields can be inferred from the offsets used in those functions.
5. Emit C++ headers with #pragma once, correct inheritance, virtual function
   declarations in vtable order, and data members with evidence comments.

INDEX.md must summarize: class name, base class, vtable VA, #virtuals,
#data members recovered, and confidence (high/medium/low).

Report back: file list, total classes documented, and any class you could not
resolve with the reason.
`,
  },
  {
    key: "locomotion",
    agent: "worker",
    task: common + `
YOUR TASK: Reconstruct the locomotion, command, and AI subsystems.

Write these files (and only these):
  src/include/LocomotionClass.h
  src/include/DriveLocomotionClass.h
  src/include/FlyLocomotionClass.h
  src/include/WalkLocomotionClass.h
  src/include/JumpjetLocomotionClass.h
  src/include/HoverLocomotionClass.h
  src/include/MechLocomotionClass.h
  src/include/TeleportLocomotionClass.h
  src/include/TunnelLocomotionClass.h
  src/include/DropPodLocomotionClass.h
  src/include/CommandClass.h
  src/include/BrainClass.h
  src/include/TeamClass.h
  src/include/TeamTypeClass.h
  src/include/TriggerClass.h
  src/include/TriggerTypeClass.h
  src/include/LOCOMOTION_INDEX.md

METHOD:
1. Read CONTEXT.md. Find all *LocomotionClass and *CommandClass entries in
   out/hierarchy.txt and out/classes.json.
2. LocomotionClass is the abstract base (ILocomotion interface). Recover its
   vtable from out/vtables.txt and enumerate the virtual interface in order.
   Each concrete locomotion class overrides a subset; use the vtable entries
   to determine which.
3. For CommandClass: there are ~50 command subclasses (AddTeamCommandClass,
   DeployCommandClass, etc.). Recover the common CommandClass interface and
   each subclass's overrides. Command classes typically have Execute(),
   and a name string.
4. For BrainClass/TeamClass/TriggerClass: recover from vtables + the INI keys
   they reference (e.g. AITriggerSuccessWeightDelta, AITriggerFailureWeightDelta,
   AITriggerTrackRecordCoefficient belong to AITriggerTypeClass).
5. Emit headers with evidence comments.

LOCOMOTION_INDEX.md must list every locomotion class with its vtable VA,
virtual count, and the concrete movement parameters recovered.

Report back: file list, #classes documented, unresolved items with reasons.
`,
  },
  {
    key: "data",
    agent: "worker",
    task: common + `
YOUR TASK: Reconstruct the game-data / INI schema and the map/display layer.

Write these files (and only these):
  src/include/RulesClass.h
  src/include/INIClass.h
  src/include/CCINIClass.h
  src/include/CellClass.h
  src/include/MapClass.h
  src/include/DisplayClass.h
  src/include/RadarClass.h
  src/include/SidebarClass.h
  src/include/AnimClass.h
  src/include/AnimTypeClass.h
  src/include/BulletClass.h
  src/include/BulletTypeClass.h
  src/include/WeaponTypeClass.h
  src/include/WarheadTypeClass.h
  src/include/SCHEMA.md

METHOD:
1. Read CONTEXT.md and out/ini_keys.json.
2. The INI keys are the ground truth for the rules.ini schema. Group them by
   which class reads them. To find the reader, search out/functions.json for
   functions whose "strings" contain the key, then inspect that function's
   callees and the offsets it uses.
3. RulesClass is the master config object. Recover its members as the set of
   global rules.ini keys (AALimit, AIBaseSpacing, AIIonCannon*, etc.).
4. For CellClass/MapClass/DisplayClass/RadarClass/SidebarClass: recover from
   vtables in out/vtables.txt plus the strings they reference.
5. For Anim/Bullet/Weapon/Warhead type classes: these are INI-driven; recover
   their fields from the keys they read (e.g. WeaponTypeClass reads Damage,
   Range, ROF, Projectile, Warhead; WarheadTypeClass reads Verses, Spread).
6. Emit headers with evidence comments.

SCHEMA.md must be a table: INI key -> owning class -> inferred C++ type ->
evidence (function VA that reads it). Include ALL 988 keys if you can, grouped
by class. This is the most valuable single artifact — be thorough.

Report back: file list, #INI keys mapped, #classes documented, unresolved items.
`,
  },
];

const results = await runs.all(tasks);
return results.map((r) => ({
  key: r.key,
  status: r.status,
  output: r.output,
}));
