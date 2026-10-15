// P2 workflow: reconstruct the template/container/IO layers.
//
// Three bounded children, each writing ONLY its own JSON artifact. No child
// writes headers, so there is no write contention. The parent (me) merges the
// artifacts into hand-written template headers afterwards.
//
// Evidence discipline: every claim must cite a vtable VA, a function VA, or an
// RTTI name. Anything unproven is marked /* ? */.

const COMMON = `
You are working in /mnt/nvme-1/projects/tiberian/re on a reconstruction of
Command & Conquer: Tiberian Sun (1999, MSVC 6.0, PE32 i386, ImageBase 0x400000).

Ground truth artifacts (READ ONLY — do not modify):
  /tmp/out_backup/class_manifest.json  - 749 classes, vtable VAs, virtual slots
  /tmp/out_backup/members.json         - recovered member offsets per class
  /tmp/out_backup/functions.json       - 9057 disassembled functions
  /tmp/out_backup/ini_keys.json        - referenced strings
  bin/GAME.EXE                         - the binary itself

Rules:
  - Every claim must cite evidence: a vtable VA, a function VA, an RTTI name,
    or a member offset. No invented members, no invented methods.
  - Mark anything uncertain with /* ? */.
  - Write ONLY the output file named in your task. Do not touch src/include/.
  - Use python3. Read the binary with pefile if you need raw bytes.
  - Be concise in your final report: what you proved, what you could not.
`;

const members = {
  agent: "worker",
  task: COMMON + `
YOUR TASK: recover the layout of the container templates.

There are 297 instantiations of VectorClass<>/DynamicVectorClass<> and 12 of
TypeList<>. These are all instantiations of a small number of real templates.
Recover the ACTUAL template layout by finding the shared code.

Method:
  1. From class_manifest.json, collect every class whose name matches
     VectorClass<...> or DynamicVectorClass<...>. Note their vtable VAs.
  2. The template's methods are shared across instantiations. Find the
     constructor of a concrete instantiation (e.g. DynamicVectorClass<AbstractClass*>)
     and disassemble it. The member offsets it initialises ARE the template
     layout (VectorClass is the base, DynamicVectorClass derives from it).
  3. Cross-check the same offsets across at least 3 different instantiations.
     Only report an offset that is consistent across all of them.
  4. Identify the virtual methods by their vtable slot index and what they do
     (e.g. a slot that calls operator delete on the buffer is the destructor).

Write /tmp/p2_containers.json:
{
  "VectorClass": {
     "evidence": {"vtables": [...], "ctors": [...]},
     "members": [{"off": 4, "name": "m_pData", "kind": "ptr", "evidence": "..."}],
     "virtuals": [{"slot": 0, "name": "~VectorClass", "evidence": "..."}]
  },
  "DynamicVectorClass": {...},
  "TypeList": {...}
}
Report: the recovered layouts and how many instantiations corroborate each.`
};

const io = {
  agent: "worker",
  task: COMMON + `
YOUR TASK: reconstruct the Pipe/Straw IO hierarchy.

There are 20 classes matching Pipe|Straw (Base64Pipe, Base64Straw, BlowPipe,
etc.). These form a compression/IO pipeline.

Method:
  1. From class_manifest.json, list every Pipe/Straw class with its vtable VA
     and virtual slot count.
  2. Build the inheritance tree from the RTTI base-class records.
  3. For each class, disassemble its constructor and its virtual methods.
     Identify what each virtual does from the calls it makes and the strings
     it references (e.g. a method calling the zlib inflate entry point is
     decompression).
  4. Name the virtuals where the evidence is clear; leave vfunc_N otherwise.

Write /tmp/p2_io.json:
{
  "classes": [
    {"name": "BlowPipe", "vtable": "0x...", "bases": ["Pipe"],
     "virtuals": [{"slot": 0, "name": "...", "evidence": "..."}],
     "members": [{"off": 4, "name": "...", "evidence": "..."}]}
  ],
  "hierarchy": {"Pipe": ["BlowPipe", ...]}
}
Report: the hierarchy and how many virtuals you could name with evidence.`
};

const blit = {
  agent: "worker",
  task: COMMON + `
YOUR TASK: reconstruct the Blit* blitter family.

Classes matching ^Blit or RLEBlit (BlitPlain, BlitTransXlat, RLEBlitTransXlat,
etc.) are pixel-blitting specialisations. They are NOT in the RTTI manifest
under those names, so find them from the vtables and the code.

Method:
  1. Search functions.json for functions whose name or referenced strings
     mention "Blit". Also look at the vtables in class_manifest.json for
     classes with Blit-like slot counts.
  2. These are template specialisations: BlitPlain<T>, BlitTrans<T>, etc.
     Determine the template parameter from the pixel size the code operates on
     (a loop advancing 1 byte/px is 8-bit, 2 bytes is 16-bit, 4 is 32-bit).
  3. Group them into template families and record the specialisation arguments.

Write /tmp/p2_blit.json:
{
  "families": [
    {"template": "BlitPlain", "specialisations": [
        {"args": ["unsigned char"], "evidence": "loop stride 1 at 0x..."}]}
  ],
  "notes": "..."
}
Report: the families you identified and the evidence for each.`
};

const r = await runs.all([
  { key: "containers", agent: members.agent, task: members.task },
  { key: "io",         agent: io.agent,       task: io.task },
  { key: "blit",       agent: blit.agent,     task: blit.task }
]);

for (const x of r) {
  emit(`${x.key}: ${x.status}`);
}
return r.map(x => ({ key: x.key, status: x.status, output: x.output }));
