# todo.md — Tiberian Sun reconstruction work list

Prioritized, evidence-backed work items. Each item states what is wrong, how it
was verified, and what "done" means. Ordered by (value × certainty) ÷ risk.

Status legend: `[ ]` open · `[~]` in progress · `[x]` done.

**Latest push (e2e planning council + execution).** The verification gate is now
fail-closed and seven checks deep; disassembly coverage went 49.2% -> 63.7% of
`.text` (12,784 -> 23,129 functions) via a gap-recovery pass over already-recorded
branch targets; and `name_virtuals.py` is wired into the pipeline so the 1,213
recoverable virtual names survive a clean checkout (named virtuals 903 -> 1,610).
Everything below is re-measured against the current tree.

---

## P0 — correctness of the gate and the tools

### [x] Fix `tools/disasm.py` prologue scan (dead `pats` list)
`find_prologues` defined five prologue patterns and then scanned only the
hardcoded `55 8b ec` sequence. Fixed to scan every pattern, with a
preceding-byte boundary check so `83 ec`/`81 ec` sites inside existing bodies
are not mistaken for function starts. Also added frame-less `__thiscall`
prologues (`56 8b f1` etc.) and frame-less `__stdcall` (`8b 44 24 04`).
Effect: 9,057 → 12,784 functions; `.text` coverage 30.5% → 49.2%.

### [x] Fix `tools/disasm.py` vtable seeding truncation
The seeding loop `break`-ed on the first non-`.text` vtable entry, silently
truncating the scan for 821 of 843 vtables. Changed to skip non-`.text` entries
and continue. This is why `TechnoClass` had 146 of 214 slots with no function
record. Remaining gaps are `.data`-resident code (see P1).

### [x] Add a header-content check to `tools/verify.py`
Checks 1–4 proved the tree compiles and that the *manifest* agrees with the
binary; none of them read what a header claims, so a fabricated vtable address
still printed `VERIFICATION PASSED`. Check 5 now compares each header's cited
vtable VA and per-slot target addresses against the manifest and fails on
mismatch. Verified by mutation: a fabricated vtable VA and a fabricated slot VA
are both caught with precise diagnostics.

### [x] Correct the "constructor" citations in generated headers
**This is the most serious evidence defect found.** `gen_manifest.py` records
the address of the *vtable-store immediate* (`c7 00 <vtable>`), not the
enclosing function start, and the header banner labels it `// constructor`.
Verified: `AbstractClass` cites `0x405703`, which is the `imm32` operand of
`mov [eax], 0x698a60`; **0 of 179 sampled ctor VAs are function starts**, median
offset into the enclosing function is ~690 bytes. `PROGRESS.md` claims
"Constructor addresses confirmed to be real function starts" — that is false.
Fix: map each store site to its enclosing recovered function and emit either the
real function start or an honest `// vtable store site 0x… (in fn 0x…)`.

### [ ] Filter vptr records out of `out/members.json`
428 of 4,634 member records are offset-0 entries whose value equals the class's
own vtable pointer — vptr initialisation, not data members. They are emitted as
`unsigned long m_000; // init 0x…`, presenting the vptr as a field.

### [ ] Fix glob-order nondeterminism in `gen_headers.py` / `reconcile_prelude.py`
`template_header` is built with `setdefault` while iterating unsorted
`glob.glob`; 16 type names are declared in more than one hand-written header and
reversing glob order changes the owner for 7 of them. Currently latent, but it
makes output filesystem-dependent, violating AGENTS.md rule 6. Fix: sort glob
results, and only let a header claim a name it actually *defines* (not merely
forward-declares — `LocomotionClass.h:27` is `class TechnoClass;` yet wins
ownership of that name).

### [x] Resolve the orphaned `out/virtual_names.json`
`gen_headers.py` reads it; `name_virtuals.py` writes it; **no pipeline stage
invokes `name_virtuals.py`**, and the file is untracked and absent from a fresh
clone. Consequence: every generated header has zero recovered virtual names on a
clean regeneration. Either wire `name_virtuals.py` into `pipeline.sh` as an
explicit stage and commit the artifact, or delete the dead read. Leaving a read
of an artifact no tool produces is exactly what rule 6 forbids.

---

## P1 — depth: emit evidence already recovered

### [ ] Emit member offsets for classes with no standalone header
2,544 member-offset records across 394 classes are computed and discarded purely
because the emission code lives inside the per-class file-writing branch. Owner
decision required: emit nested class bodies (faithful, but risks the standalone
gate) or append a member block to the owning header (safer, but the header
contents no longer match the class it names).

### [ ] Stop emitting `void` for unrecovered return types
7,094 of 8,689 signatures have no recovered return type and the generator emits
`void` for all of them — wrong by default rather than honestly unknown. Emit an
explicit unknown marker per AGENTS.md rule 2.

### [ ] Use the unused `imm` member kind to type fields
1,159 member records carry a concrete immediate VA that can often be typed as a
vtable pointer or a string/object reference. This is the first step that
requires inference; defer until the plumbing above is verified.

### [ ] Mine the remaining INI keys for member names
1,209 referenced strings, 988 of them `rules.ini` field names, but only 112
distinct names map to members (135 members across 20 classes). ~877 unused.

### [ ] Recover code outside `.text`
`in_text()` rejects everything outside `.text`, so code in `.data` (e.g.
`0x0060f590`, which begins `56 8b f1` — a real `__thiscall` prologue) is
structurally unreachable. 86 `TechnoClass` slots are still unseeded; some are
`.data`-resident, others are jump-table-adjacent. Extending section handling is
the next coverage win.

---

## P2 — remaining reconstruction

### [ ] `Node<>` as its own header
Currently declared inside `INIClass.h`; should have its own header with the
layout recovered from the binary.

### [ ] MS* multimedia classes (MSAnim, MSFont, …)
### [ ] WOL/ATL COM layer
### [ ] Function bodies (`.cpp`)
Zero bodies exist today. `build/tu_all.cpp` is 262 lines of `#include`; it
proves declarations are self-consistent, not that behaviour is reproduced.
Recommended to land *after* boundary recovery, since bodies scale with the
function set and are cheap to regenerate — but every boundary bug left in
`disasm.py` is baked into every downstream artifact.

---

## P3 — verification and validation

### [x] Extend check 5 to inheritance and member offsets
Check 5 currently validates vtable VA and slot targets only. Add: emitted base
clauses vs `out/direct_bases.json`, and emitted member offsets vs
`out/members.json`.

### [x] Make skipped checks fail instead of silently passing
With `out/` absent, checks 3 and 4 skip via `os.path.exists` guards and the tool
still prints `VERIFICATION PASSED` after running only 2 of 5 checks.

### [x] Replace the `man[:200]` prefix sample with all 749 entries
Check 4 validates an ordered alphabetical prefix of the manifest (26.7%).
`TechnoClass`, `ObjectClass`, `UnitClass`, `InfantryClass`, `HouseClass` and
`RulesClass` are all outside it.

### [ ] Compile the TU with `-m32`
The shim types are host-sized: on this LP64 host `sizeof(DWORD)` is 8 and
`sizeof(GUID)` is 24, versus 4 and 16 in the PE32 target. Also
`#define __stdcall` is empty, so the calling-convention data recovered for 8,689
functions is not exercised by the build.

### [ ] Unit tests for the load-bearing tools
No test directory exists. `verify.py` tests output artifacts, never the tools.
Cheapest high-value test: assert `rtti.py` + `gen_manifest.py` reproduce known
counts (749 classes, 843 vtables) so a silent regression in a recovery tool is
caught.

### [ ] Byte-level validation against the original binary
Recompile and diff where a matching compiler is available.

### [ ] Regenerate `REPORT.md`
It describes a 42-header project. Stale figures include "42 reconstructed
headers compile cleanly" (260 exist), "7094 lines" (18,038),
"42/42 pass" (259/259). `PROGRESS.md` also still carries the superseded
"5,604 slots" (manifest sums to 9,810) and "692 headers" (260).

---

## Standing caveats

- `VERIFICATION PASSED` now means "the tree compiles **and** 219 headers' cited
  vtable evidence matches the manifest". It does **not** yet validate
  inheritance, member offsets, or member layout, and it does not prove the
  disassembly itself is correct.
- Member offsets are exact where recovered; names only where an INI key or
  string provides evidence. Do not infer a name.
- `RulesClass` has no RTTI record (not polymorphic), so its members are not
  isolated.
