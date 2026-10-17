# ROADMAP — the remaining trajectory to documented, modifiable, compilable source

Written after a full audit of the tree. Every figure below was
measured, not estimated; the commands that produce them are named. Re-measure
before trusting this file (`python3 tools/harness.py metrics`).

The point of this document is to say **what is actually left**, what the
*smallest* honest size of that work is, and in what order it can be done — not to
list everything that could conceivably be improved.

---

## 1. What "done" has to mean

The goal is a Tiberian Sun that is **documented**, **modifiable** and
**compilable**. Those are three different claims and they are nowhere near
equally satisfied. Stated so they can be checked:

| Claim | Done means | Measurable exit criterion |
|---|---|---|
| **Compilable** | The tree builds as 32-bit code with the game's ABI, and the compiler itself confirms the recovered memory layout | `tools/verify.py` compiles everything `-m32` and fails unless every recovered member offset is confirmed by `offsetof` |
| **Documented** | Every class, virtual and member is either named with cited evidence or explicitly marked unknown *with a reason* | 0 declarations without an evidence record; the `vfunc_N` remainder carries a per-slot reason |
| **Modifiable** | A human changes a function's *behaviour* by editing C++ statements, rebuilds, and sees the change — without touching machine instructions | ≥50% of instructions in the 500 highest-centrality functions absorbed into recovered C++ statements, each passing a differential test |

Nothing below counts as done until one of those commands changes its output.

---

## 2. Where the tree actually is

### 2.1 Recovery is effectively closed — this is not the problem any more

`.text` is 2,712,101 bytes. 104 KB is unattributed. I read the bytes in the
largest gaps rather than assuming:

```
gap 0x663820 224B: c62a6600 4c376600 db2c6600 8e2a6600 ...   <- jump table
gap 0x45fef0  64B: c7fe4500 e3fe4500 d0fe4500 dafe4500 ...   <- jump table
```

Those are **switch tables**, plus alignment padding. The largest gap in the whole
binary is 224 bytes and there are **zero gaps larger than 512 bytes**. So the
"4% unattributed" is not unknown code — it is data living inside `.text`.
Function discovery is done.

| Item | State |
|---|---|
| Functions recovered | 41,822 (96.06% of `.text` as recovered code) |
| Residual `.text` | 104 KB, classified: 11 jump tables (256 B) and 356 B of switch index tables, emitted as data in `out/text_data.json`, plus 105,808 B of alignment padding. 554 B in 90 runs are neither and are reported verbatim. |
| Instructions with no faithful rendering | 0 (the last 166 — MMX/SSE, x87 env, far pointers, `lar`, `fbld`, `fcmovnbe`, `pushaw` — are modelled) |
| Member offsets refuted by the binary | 4 of 4,326 (`ScoreTimeClass`, `ScorePrintClass`) |

**Do not spend the next round on disassembly.** It is finished.

### 2.2 The ABI gap is real, and it is a gate fix, not a project

The host build is LP64, so `sizeof(void*)==8` and `#define __stdcall` is a no-op.
But the bigger and more immediate problem is what the generator emits:

```
member declarations in the generated headers, by declared type
   2313 unsigned long        <- 8 bytes here, 4 in the game
    569 unsigned char
     36 unsigned short
```

`unsigned long` is the generator's universal slot type for a recovered member, so
**2,313 of 2,919 member declarations have the wrong width on this host** and every
generated struct is inflated (`ObjectClass`: 248 bytes here against 164 at the
target's ABI). The recovered offsets are documented in comments and cannot be
reproduced by the code.

That part needs no toolchain change: `unsigned int` is 4 bytes on both LP64 and
i386, so retyping the emission fixes the width problem outright. What it cannot
fix is pointer width, `__stdcall`/`__thiscall` (the 22,328 recovered calling
conventions are not exercised at all while the macro is defined away), or `GUID`
(24 bytes here against 16). Those need the real 32-bit target.

I installed `g++-multilib` and measured what `-m32` gives:

```
sizeof(void*)=4  sizeof(DWORD)=4  sizeof(ULONG)=4  sizeof(long)=4
standalone headers -m32 : 262/262 OK
generated code -m32     : compiles
whole tree -m32         : links (482 TUs, 1m37s) and the entry point RUNS
```

So the tree is *already* ABI-correct at 32-bit; **the gate just never asked**. That
is the single highest-value change available, and it is small.

### 2.3 …but the layout still does not match, and that is the real defect

Making the compiler the authority immediately exposes something worse than the
host-width problem:

```
offsetof(AbstractClass, m_000) == 20     // compiler, -m32
recovered table says               0     // out/members.json
```

The cause is structural, not cosmetic. **1,053 of the 4,326 "members" in the
recovered table are vtable-pointer stores** — 622 of them at offset 0 — and the
generator emits each as an ordinary data member:

```cpp
unsigned long m_000;  // +0x000 init 0x698a60   <- this is the vptr
```

C++ inserts the vptr itself, so the reconstructed class counts it twice and every
member after it is displaced. This means the tree compiles, links and runs, and
still **does not have the game's memory layout**. For a decompilation that is the
difference between "it builds" and "it is the game".

### 2.4 Semantics — the bulk of what is left, and smaller than it looks

```
vtable slots in the manifest            9,780
named                                     1,500  (15.3%)
  of which destructors                      600
  of which COM boilerplate (QI/AddRef/Rel)  403
  of which genuinely semantic               497  (5.1%)
unnamed                                   8,280
```

The raw 8,280 is misleading. **65.1% of all (class, slot) pairs are inherited
from a base class.** The number of slots that need an original name is:

```
genuinely distinct (class, slot) pairs  3,409
```

And the leverage is extreme — naming a base names its whole subtree:

| Class | slots | subclasses | slot-instances covered |
|---|---|---|---|
| `ObjectClass` | 105 | 20 | 2,205 |
| `AbstractClass` | 24 | 68 | 1,656 |
| `TechnoClass` | 214 | 5 | 1,284 |
| `MissionClass` | 133 | 7 | 1,064 |
| `RadioClass` | 135 | 6 | 945 |
| `FootClass` | 236 | 3 | 944 |
| `AbstractTypeClass` | 27 | 28 | 783 |
| `GadgetClass` | 31 | 20 | 651 |
| `ObjectTypeClass` | 40 | 14 | 600 |

The top 18 bases cover 13,011 slot-instances. **The naming problem is ~1,800
slots of real work, not 9,810.** That is tractable, and it is where the next
rounds belong.

Members: 4,326 recovered entries, 3,273 after removing vptr stores, **135 named
(3.1%)**.

### 2.5 Modifiability — the hardest gap, and the one with no partial credit

**There is not one hand-written `.cpp` file in the tree.** Every one of the
43,668 functions is generated machine transcription, one instruction per line:

```cpp
ts::push32(0x19u);          //   0x006829a5  push 0x19
ts::mov_r32(EAX.d, 0u);     //   0x006829a7  mov eax, 0
```

This is faithful and gated, and it is **not source**. A human can read it against
the recovered listing line for line, but cannot meaningfully *edit* it. The
hand-editable surface today is 47 headers; the other 262 headers and all the code
are regenerated.

So: the tree is compilable and increasingly documented, and it is only
*marginally* modifiable. This is the long pole and it needs a different
technique from everything done so far — pattern recognition over 847,121
instructions rather than more recovery.

---

## 3. The trajectory

### P0 — Make "compilable" mean the game's ABI

*Small, immediate, unblocks every later claim about layout.*

- **0.0** Stop emitting members as `unsigned long`. That is the generator's
  universal slot type, and it is 8 bytes on this host against 4 in the game:
  **2,313 of the 2,919 member declarations in the generated headers are
  `unsigned long`**, so every generated struct is inflated and no recovered
  offset can mean anything. `unsigned int` is 4 bytes on both, so this costs
  nothing and needs no toolchain change. Do this first: it is the reason the
  offsets disagree, and it is independent of the `-m32` question below.

- **0.1** Gate on `-m32`, fail closed when multilib is absent (a gate that
  silently skips is exactly how an unlinkable tree survived for months).
  Document the toolchain requirement. *Verified working already.*
- **0.2** Stop emitting vptr stores as data members; reconcile base-class
  layout so absolute offsets are reproduced rather than displaced.
- **0.3** Generate `offsetof` assertions for every recovered member offset and
  gate on them. **Expect this to fail widely at first. That failure list is the
  work**, and it is the first time the layout will be enforced by the compiler
  instead of asserted in prose.

Exit: `tools/verify.py` fails unless the compiler confirms every recovered
member offset at `-m32`.

### P1 — Close recovery and hygiene

*Bounded and mostly mechanical.*

- **1.1** Emit the 104 KB of jump tables as data and classify the padding.
  **Done:** `tools/disasm.py` classifies the residual into jump tables, switch
  index tables and padding, reports what is neither, and writes
  `out/text_data.json`.
- **1.2** Implement the 166 unmodelled instructions. **Done:** 0 unmodelled;
  the model additions are in `src/handwritten/Machine.h`.
- **1.3** Fix the 4 refuted offsets.
- **1.4** Locate the 228 destructors, or mark each explicitly rather than
  labelling it a stub.
- **1.5** Get `out/functions.json` (64.3 MB, over GitHub's 50 MB warning) out of
  the repository — release asset or compressed.
- **1.6** Decrypt the encrypted TS MIX header (`tools/mix.py` currently fails
  explicitly on it).

Exit: `.text` coverage ≥99.5% with the remainder classified; 0 unmodelled; 0
labelled stubs; no tracked file over 50 MB.

### P2 — Name the tree

*Bounded in size: 3,409 distinct virtuals, 3,273 real members. This is the
semantics layer, and it is where council rounds fit — one lane per evidence
source, with disjoint files.*

- **2.1** Build the inheritance-propagation engine: a name established on a base
  propagates to every override, with the evidence chain recorded per slot. Only
  2.8% of unnamed slots propagate today *because there are only 497 names to
  propagate* — this engine is what makes each new name worth 20.
- **2.2** Work the leverage table in §2.4 in order, starting with `ObjectClass`,
  `AbstractClass`, `TechnoClass`, `MissionClass`, `RadioClass`, `FootClass`.
- **2.3** Evidence sources to mine, in rough order of yield: call-site argument
  and return shape; cross-references to the 1,876 recovered INI keys; the string
  table; the existing 497 anchors; RTTI. `tools/e2e_rules.py` already validates
  names against shipped data — that validation is what keeps this honest.
- **2.4** Fix the arity check (`nargs` counts `this` for COM `__stdcall` but not
  for `__thiscall`) so check 8 can become a gate instead of a signal.
- **2.5** Member naming from INI store sites, extending `name_members.py` beyond
  the current 20 classes.

Exit: ≥60% of the 3,409 distinct virtuals named with a per-name evidence record;
arity check gated; ≥50% of the 3,273 real members named.

### P3 — Source-shaping (the long pole)

*Open-ended in effort; it needs a new technique, and it is what "modifiable"
actually requires.*

- **3.1** Recover real signatures — parameters, types, return — from
  prologue/epilogue shape and call sites, not just arity.
- **3.2** Build an idiom lifter: recognise the standard MSVC 6 sequences
  (this-relative access, virtual dispatch through the vtable, switch tables,
  string and memcpy idioms) and emit C++ statements instead of instructions.
- **3.3** Build the check that makes lifting trustworthy: a lifted function must
  reach the **same register and memory state** as its transcription over random
  inputs. Without this, a lifter is a plausible-looking guess generator — and
  this project's whole method is that a claim is checkable.
- **3.4** Order by call-graph centrality, not alphabetically. Lift what a reader
  needs to understand the game first.

Exit: ≥50% of instructions in the 500 highest-centrality functions absorbed into
recovered statements, every lifted function passing the differential test.

### P4 — Documentation and reproduction

- **4.1** Per-class documentation: for any class, why every declaration is
  believed. The evidence already exists in `out/`; it is not yet readable.
- **4.2** The end-to-end test: load the real game data, run the entry point, and
  compare something observable against the original.

Exit: a documented command that shows the model doing something the original
does, and that fails if it stops doing it.

---

## 4. Critical path, and what is deliberately not on it

**P0 → P1 → P2 → P3.** P0 and P1 are small and bounded. P2 is bounded in item
count and is where parallel rounds pay. P3 is the long pole.

Not on the critical path, and should not be worked on: more disassembly
(§2.1), more class discovery (749/749 with every constructor a real entry
point), more header coverage.

## 5. Honest risks

- **P3 may not recover the original source's shape, only *a* correct shape.**
  Lifting cannot prove it found the author's intent. The honest claim is
  behavioural equivalence with the machine, and the differential test is the
  strongest available evidence for it. This limit will not go away.
- **P0.2 may require redesigning member emission**, not patching it: if vptr
  stores, base subobjects and real members are conflated in the recovered table,
  the fix is a new emitter, and the 4,326 offsets may need re-deriving with
  vptrs excluded.
- **`-m32` becomes a build requirement.** Anyone without `g++-multilib` cannot
  pass the gate. That is the correct trade — a silent skip is what produced the
  unlinkable tree — but it must be documented, not discovered.
- **Naming effort is not linear.** The first names on `ObjectClass` are cheap
  (string cross-references); the last ones in a 214-slot class may each need
  reading a function. The leverage table in §2.4 is a *floor*, not a promise.
- **Everything here assumes the recovered offsets are right.** 4 are already
  refuted. If P0.3 shows the recovered table is systematically wrong rather than
  merely mis-emitted, P2's member work has to wait.
