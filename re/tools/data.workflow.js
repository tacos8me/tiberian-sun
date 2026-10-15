// Retry of the failed "data" workstream, split into two smaller children so
// each finishes well inside its budget.

const common = `
You are reconstructing original C++ source from a disassembled 1999 game binary.

WORKING DIR: /mnt/nvme-1/projects/tiberian/re
READ FIRST: CONTEXT.md

Artifacts:
- out/classes.json    : 755 classes, inheritance edges, 842 COLs, 843 vtables
- out/hierarchy.txt   : readable class hierarchy
- out/vtables.txt     : vtable VA -> class name
- out/functions.json  : 9057 functions {va,end,size,ninsn,callees,insns[],strings[]}
- out/ini_keys.json   : 1209 referenced strings incl. 988 rules.ini field names
- bin/GAME.EXE        : the binary (PE32, ImageBase 0x400000)

IMPORTANT — WORK EFFICIENTLY:
- Do NOT write giant inline python heredocs. Write a small script to a file
  under tools/ and run it, or use short one-liners.
- Do NOT try to be exhaustive. Produce correct, evidence-cited headers for the
  classes assigned, then STOP and report.
- Budget your time: aim to finish in well under 10 minutes.

RULES:
- Cite evidence in comments: RTTI name, vtable VA, INI key, function VA.
- Mark uncertain items /* ? */. Never invent members.
- Write ONLY your assigned files.
`;

const tasks = [
  {
    key: "ini",
    agent: "worker",
    task: common + `
YOUR TASK: Reconstruct the INI/config layer and the weapon/warhead/anim/bullet
type classes.

Write exactly these files:
  src/include/INIClass.h
  src/include/CCINIClass.h
  src/include/RulesClass.h
  src/include/WeaponTypeClass.h
  src/include/WarheadTypeClass.h
  src/include/AnimTypeClass.h
  src/include/BulletTypeClass.h
  src/include/SCHEMA.md

METHOD (keep it simple and evidence-based):
1. Find the vtable VA for each class in out/vtables.txt, then read the vtable
   entries from bin/GAME.EXE (dwords at that VA) and resolve each against
   out/functions.json. Emit virtuals in that order with their real addresses.
2. For data members, use out/ini_keys.json: find functions whose "strings"
   contain a given key; that function is the class's Read_INI. The keys it
   reads ARE the member names. Group keys by which class's reader references
   them.
3. SCHEMA.md: a markdown table of INI key -> owning class -> inferred C++ type
   -> evidence function VA. Cover as many of the 988 keys as you can group
   confidently; put the rest in an "unassigned" section. Do not guess wildly.

Report: files written, #keys mapped, #classes documented, unresolved items.
`,
  },
  {
    key: "map",
    agent: "worker",
    task: common + `
YOUR TASK: Reconstruct the map / display / UI layer.

Write exactly these files:
  src/include/CellClass.h
  src/include/MapClass.h
  src/include/DisplayClass.h
  src/include/RadarClass.h
  src/include/SidebarClass.h
  src/include/AnimClass.h
  src/include/BulletClass.h
  src/include/MAP_INDEX.md

METHOD (keep it simple and evidence-based):
1. Find each class's vtable VA in out/vtables.txt, read the vtable entries from
   bin/GAME.EXE, resolve each against out/functions.json, and emit the virtual
   functions in vtable order with real addresses.
2. For data members, inspect the class constructor (find it by locating the
   function that stores the class's vtable VA) and record the member offsets it
   initialises. Name members from any INI keys or strings the class's methods
   reference.
3. MAP_INDEX.md: table of class -> vtable VA -> #virtuals -> #members ->
   confidence, plus notes on anything unresolved.

Report: files written, #classes documented, unresolved items.
`,
  },
];

const results = await runs.all(tasks);
return results.map((r) => ({ key: r.key, status: r.status, output: r.output }));
