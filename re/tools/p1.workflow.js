// P1 deepening workflow: name members, name virtuals, recover signatures.
// Three bounded children writing to disjoint outputs.

const common = `
You are deepening a reconstructed C++ source tree for Command & Conquer:
Tiberian Sun (1999). The class skeleton is COMPLETE and VERIFIED: 749 classes,
all headers compile. Your job is to add semantic detail, not structure.

WORKING DIR: /mnt/nvme-1/projects/tiberian/re
READ FIRST: AGENTS.md (ground rules), PROGRESS.md (state), CONTEXT.md

Artifacts:
- out/class_manifest.json : 749 classes {class,ident,vtable,nvirtuals,bases,ctors,entries[]}
- out/members.json        : 628 classes with recovered member offsets {ctor,members[{off,kind,value}]}
- out/functions.json      : 9057 functions {va,end,size,ninsn,callees[],insns[],strings[]}
- out/ini_keys.json       : 1209 referenced strings incl. 988 rules.ini field names
- out/ini_functions.json  : 155 functions that reference INI keys
- out/classes.json        : RTTI classes, inheritance, vtables
- bin/GAME.EXE            : the binary (PE32, ImageBase 0x400000, .text at 0x401000)

HARD RULES:
- Evidence or it doesn't go in. Cite the VA / key / string that justifies a name.
- Never invent. If you cannot justify a name, leave the placeholder.
- Mark inference with /* ? */.
- Do NOT restructure headers. Only add names, comments, and signatures.
- Do NOT write giant inline heredocs. Write a script to tools/ and run it.
- Keep the build green: g++ -std=c++03 -fsyntax-only -Isrc/include -Ibuild/shim build/tu_all.cpp
- Write ONLY your assigned files.

Report back concisely: what you changed, counts, and what you could not resolve.
`;

const tasks = [
  {
    key: "members",
    agent: "worker",
    task: common + `
YOUR TASK: Name data members where evidence exists.

Write ONLY:
  tools/name_members.py     (the tool you build)
  out/member_names.json     (your output: class -> {offset -> name})

Do NOT edit src/include/*.h — the orchestrator regenerates headers from your
JSON after all three of you finish. Editing headers concurrently would clash.

METHOD:
1. For each class in out/members.json, find its INI reader: the vtable slot
   function (from out/class_manifest.json entries) that references the most
   INI-like key strings. Use out/ini_functions.json to find candidates.
2. In that reader, find the store pattern: a key string is pushed, an INI
   accessor is called, and the result is stored to [this+offset]. The offset
   identifies which member the key configures. That key IS the member name.
3. Emit out/member_names.json mapping class -> offset -> name.
4. Do NOT modify tools/gen_headers.py either — the orchestrator wires your
   JSON into it. Just produce out/member_names.json in the documented shape.

Aim for real names on the type classes (BuildingTypeClass, UnitTypeClass,
InfantryTypeClass, AircraftTypeClass, WeaponTypeClass, WarheadTypeClass,
AnimTypeClass, BulletTypeClass, TerrainTypeClass, OverlayTypeClass,
SmudgeTypeClass, SuperWeaponTypeClass, VoxelAnimTypeClass, ParticleTypeClass,
ParticleSystemTypeClass, TiberiumClass, IsometricTileTypeClass) — these are
INI-driven and have the richest evidence.

Report: #members named, #classes affected, what blocked the rest.
`,
  },
  {
    key: "virtuals",
    agent: "worker",
    task: common + `
YOUR TASK: Name virtual functions where evidence exists.

Write ONLY:
  tools/name_virtuals.py    (the tool you build)
  out/virtual_names.json    (your output: class -> {slot -> name})

Do NOT edit src/include/*.h — the orchestrator regenerates headers from your
JSON after all three of you finish.

METHOD:
1. For each class in out/class_manifest.json, walk its vtable entries.
2. For each entry, look up the function in out/functions.json and gather
   evidence for what it does:
     - strings it references (INI keys, format strings, file extensions)
     - which functions it calls (e.g. calls to INIClass accessors => Read_INI)
     - whether it is a destructor (calls operator delete / frees members)
     - whether it returns a constant (Get_* style)
3. Assign a name ONLY when the evidence is clear. Good signals:
     - references many INI keys            -> Read_INI
     - calls the INI accessor family       -> Read_INI / Write_INI
     - calls operator delete on this       -> destructor
     - references a "Name"/"Cameo" key     -> Fetch_ID / Get_Name
     - tiny function returning a constant  -> Get_<something>
   Otherwise leave vfunc_N.
4. Emit out/virtual_names.json in the documented shape. Do not touch
   gen_headers.py.

Be conservative. A wrong name is worse than vfunc_7. Aim for a few hundred
confident names, not thousands of guesses.

Report: #virtuals named, #classes affected, your naming heuristics.
`,
  },
  {
    key: "signatures",
    agent: "worker",
    task: common + `
YOUR TASK: Recover function signatures (calling convention, arg count, return).

Write ONLY:
  tools/gen_signatures.py   (the tool you build)
  out/signatures.json       (your output: va -> {cc, nargs, ret, evidence})

Do NOT edit src/include/*.h — the orchestrator regenerates headers from your
JSON after all three of you finish.

METHOD:
1. For each function in out/functions.json, determine the calling convention
   and argument count from its prologue/epilogue:
     - 'ret N' (C2 imm16) => __stdcall with N/4 args
     - 'ret'  (C3)        => __cdecl or __thiscall; count args from how many
                             [esp+X] reads occur before the first call
     - 'mov eax,ecx' / use of ecx as this => __thiscall
     - functions that only read [esp+4] => 1 arg, etc.
2. Determine return type heuristically: if the function sets eax from a
   computed value it returns something; if it ends with 'xor eax,eax' it
   likely returns bool/int 0.
3. Emit out/signatures.json in the documented shape. Do not touch
   gen_headers.py. Keep it honest: only record a signature when the evidence
   supports it; omit the entry otherwise.

Report: #functions with recovered signatures, distribution of conventions,
and what you could not determine.
`,
  },
];

const results = await runs.all(tasks);
return results.map((r) => ({ key: r.key, status: r.status, output: r.output }));
