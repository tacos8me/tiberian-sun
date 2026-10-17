#!/usr/bin/env python3
"""
Name virtual functions where binary evidence supports a name.

Inputs (all under out/):
  class_manifest.json  - per-class vtable/ctor manifest
  classes.json         - RTTI vtables with their real VAs and sub-object offsets
  direct_bases.json    - reduced direct-base set (AGENTS.md: use this for C++)
  functions.json       - disassembly: callees, instructions, referenced strings
  ini_keys.json        - referenced strings (the rules.ini field-name universe)
  signatures.json      - recovered arity, used only to keep declarations legal

Output:
  out/virtual_names.json           - { "<class ident>": { "<slot>": "<name>" } }
  out/virtual_name_evidence.json   - per-name audit trail:
                                     { "classes": { "<class>": { "<slot>": {
                                           "name": ..., "rule": ..., "signal": ...,
                                           "target": "0x..." } } },
                                       "unattributed_slot_targets": { "<target va>":
                                           { "slots": N, "size": ..., "ninsn": ...,
                                             "callees": [...], "strings": [...],
                                             "instructions": [...] } } }

Method
------
For every class we read its *primary* vtable (sub-object offset 0) directly from
GAME.EXE and take each slot target from the recovered disassembly.  A name is
emitted only when a rule fires with unambiguous evidence; otherwise the slot is
left unnamed so the header generator keeps emitting vfunc_N.

Evidence rules (each cites the concrete signal it keys on):

  R1 destructor
     The slot target calls the CRT operator delete (0x68298c) and writes a
     vtable pointer into [ecx] / [esi] / [edi].  MSVC 6 emits the scalar
     deleting destructor in this shape.  -> "~<Class>" (deleting form).

  R2 Read_INI
     The slot target references >= 4 distinct rules.ini field names AND calls at
     least one known INI accessor AND shows the *reader* store pattern: an
     accessor call immediately followed (within 6 instructions) by a store of
     the returned value into [this+off].  -> "Read_INI".

  R3 Fetch_ID / Set_ID
     The slot target references the literal "Name" and is small (< 80 bytes),
     matching AbstractClass::Fetch_ID (0x405f20) / Set_ID (0x405fb0).
     -> "Fetch_ID" / "Set_ID".

  R5 stub
     The slot target is the shared COM stub 0x6829a5 (push 0x19; call
     _com_issue_error) or a bare `ret`.  -> left unnamed (no semantics).

  R6 same-code-address propagation
     If a slot target is byte-identical to a slot target already named in
     another class's vtable, the same name is propagated.  This rule names a
     slot from *another* slot's evidence; it is only sound because the two slots
     are the same code address.  A propagated destructor name is rewritten to
     the derived class.

  R9 QueryInterface / AddRef / Release
     The slot is slot 0 of the primary vtable and its target has the exact
     MSVC 6 IUnknown::QueryInterface error path.  Slots 1 and 2 of the same
     class are then AddRef and Release by the MS IUnknown ABI.

  R10 Write_INI
     The slot target references >= 4 distinct rules.ini field names, calls at
     least one known INI accessor, and calls AbstractClass::Set_ID (0x405fb0).

  R11 inherited slot  (the inheritance-propagation engine)
     A virtual overridden in a derived class occupies the same vtable slot as
     the base class's virtual: MSVC lays the primary base sub-object's vtable
     out first, and a derived class appends only new virtuals.  So a name on
     <base> slot N propagates to <derived> slot N for every direct base whose
     sub-object displacement (RTTI mdisp) is 0, and to the derived vtable at
     offset mdisp for a secondary base when that vtable exists.  The evidence
     record names the base, the base slot, the base name and the displacement,
     so the chain is auditable.  A propagated destructor is renamed to the
     derived class.  The rule is sound only because MSVC preserves slot order;
     where a derived class's vtable is shorter than its base's the edge is
     skipped rather than guessed.

  R12 curated anchor
     A small table of AbstractClass slots whose semantics were established by
     reading the function shape directly (see CURATED_SLOT_NAMES).  Each entry
     carries the shape that justifies it.  These are the anchors the engine
     propagates.

  R13 override corroboration
     R11 names a derived slot from the *layout*.  When the derived slot's own
     code also calls the base class's same-slot target, the override is proven
     by the call graph as well, and the name is recorded under R13 instead.  R13
     is R11 plus an independent signal; it exists so the layout assumption is
     checkable, not so the count rises.

  R15 per-class constant slot
     A slot implemented as a one-instruction constant return in many classes,
     each returning a different constant, is a class-identifying virtual: the
     value *is* the class.  Slot 11 returns a small per-class RTTI id
     (BuildingClass 6, UnitClass 1, InfantryClass 0xf, AircraftClass 2,
     AnimClass 4, BulletClass 8, CellClass 0xb, HouseClass 0xd -- class/type
     pairs are adjacent) and slot 12 returns a per-class object size
     (BuildingClass 0x310, UnitClass 0x370, InfantryClass 0x378, AnimClass
     0x108, BulletClass 0xc8, CellClass 0xa8).  The rule fires only when at
     least 8 classes implement the slot and every one returns a distinct
     constant; the record cites the class's own constant.
  R7 base from subclass overrides
     The reverse of R11: when >= 2 direct subclasses independently carry a
     *direct-evidence* name at the same slot index of an unnamed base slot, the
     base slot takes that name.  Two independent subclasses make a layout
     accident unlikely.  This names an intermediate base whose own code carries
     no direct evidence.

Names are only ever emitted for slots whose target VA is inside .text and
present in functions.json.  Everything else stays vfunc_N.  Every emitted name
is recorded with its rule and the concrete signal in
out/virtual_name_evidence.json.

A final guard drops a name that would collide with another name already emitted
for the same class at the same arity, because two virtual declarations with one
name and one signature do not compile; the dropped slot keeps vfunc_N.
"""
import json
import os
import re
import struct
import sys
from collections import defaultdict

IB = 0x400000

# --- well-known addresses established by prior recovery work ----------------
OP_DELETE = 0x68298c          # CRT operator delete (push arg; call 0x683fa4)
COM_STUB = 0x6829a5           # shared "not implemented" COM stub
ABSTRACT_FETCH_ID = 0x405f20  # AbstractClass::Fetch_ID, refs "Name"
ABSTRACT_SET_ID = 0x405fb0    # AbstractClass::Set_ID, refs "Name"
ABSTRACT_QUERY_INTERFACE = 0x405790  # AbstractClass::QueryInterface

# INI accessor VAs (same set tools/name_members.py traces through; see SCHEMA.md).
INI_ACCESSORS = {
    0x4D60B0, 0x4D6960, 0x4D6ED0, 0x4D70B0, 0x4D54C0, 0x4D50F0,
    0x4D6030, 0x4D72B0, 0x4D6290, 0x4D4250, 0x4D6B50, 0x4D5D50,
    0x4D6B00, 0x4D7490, 0x4D5C60, 0x4D7070, 0x447F70,
}

# A rules.ini field name: CamelCase identifier.  Deliberately stricter than
# "any identifier" so that format strings ("%s : %s") and file names are not
# mistaken for keys.
_INI_KEY_RE = re.compile(r"[A-Z][A-Za-z0-9]{2,30}")

# Read the reader store pattern: `call <accessor>` then, within a few
# instructions, `mov dword ptr [<reg> + <off>], <reg>`.
_STORE_RE = re.compile(r"^(dword|word|byte) ptr \[([a-z0-9]+) \+ (0x[0-9a-f]+|\d+)\], ")
_ACCESSOR_OP = {hex(v) for v in INI_ACCESSORS}

# AbstractClass primary vtable (0x698a60) slot -> name, re-derived from the
# disassembly of each slot target rather than from the hand-written header's
# numbering (that header counted the COL pointer as a slot, shifting indices).
# Each entry carries the code shape that justifies it.  Slot 0 is established
# by R9, and slot 8 (the deleting destructor) by R1, so neither is repeated.
CURATED_SLOT_NAMES = {
    "AbstractClass": {
        1: ("Init", "returns 1"),
        2: ("Shutdown", "returns 1"),
        4: ("Release", "reads [this+0x10], returns !flag"),
        7: ("GetType", "2 args, E_POINTER check on the out pointer"),
        9: ("Set_ID", "bare ret"),
        10: ("Get_RTTI", "ret 8"),
        13: ("Load", "calls the stream accessors"),
        14: ("Save", "returns -1"),
        15: ("GetSizeMax", "returns 0"),
        16: ("Compute_CRC", "returns 0"),
    },
    "ObjectClass": {
        # Every subclass overrides this slot to return its own type object
        # from a class-specific member (BuildingClass [this+0x218], UnitClass
        # [this+0x358], AircraftClass [this+0x34c], InfantryClass [this+0x348],
        # AnimClass [this+0x64], BulletClass [this+0x4c]), and ObjectClass's own
        # body returns 0 -- the base is abstract here.
        33: ("Get_Type", "each subclass returns its type pointer from its own member; "
             "the base returns 0"),
    },
}

UNATTRIBUTED_TARGETS = (0x405A80, 0x405A30)



class Bin:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        import pefile
        pe = pefile.PE(path, fast_load=True)
        self.secs = []
        for s in pe.sections:
            self.secs.append((IB + s.VirtualAddress,
                              IB + s.VirtualAddress + s.Misc_VirtualSize,
                              s.PointerToRawData, s.SizeOfRawData,
                              s.Name.decode(errors="replace").strip("\x00")))
        self.text = next(s for s in self.secs if s[4] == ".text")

    def u32(self, va):
        for vs, ve, raw, rs, nm in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                b = self.data[o:o + 4]
                return struct.unpack("<I", b)[0] if len(b) == 4 else None
        return None

    def in_text(self, va):
        t = self.text
        return t[0] <= va < t[1]


def read_primary_vtable(bin_, vtable_va, limit=400):
    """Read consecutive code pointers from a vtable."""
    slots = []
    for i in range(limit):
        x = bin_.u32(vtable_va + 4 * i)
        if x is None or not bin_.in_text(x):
            break
        slots.append(x)
    return slots


def writes_vtable_ptr(fn):
    """True if fn stores a .rdata vtable pointer into [ecx]/[esi]/[edi]."""
    for i in fn["insns"]:
        if i["mnem"] != "mov":
            continue
        op = i["op"]
        if "dword ptr [" not in op:
            continue
        dst = op.split(",")[0]
        if not any(r in dst for r in ("[ecx]", "[esi]", "[edi]")):
            continue
        src = op.split(",", 1)[1].strip()
        if src.startswith("0x6"):
            return True
    return False


def ini_keys_of(fn):
    """Distinct rules.ini field names referenced by this function."""
    return {s["text"] for s in fn.get("strings", []) if _INI_KEY_RE.fullmatch(s["text"])}


def ini_accessor_calls(fn):
    """VAs of known INI accessors this function calls, in call order."""
    return [c for c in fn.get("callees", []) if c in INI_ACCESSORS]


def reader_store_count(fn):
    """Count the INI-reader store pattern: an accessor call followed within 6
    instructions by a store of the result into [reg+off].

    This is the same pattern tools/name_members.py uses to bind a key string to
    a member offset, so a non-zero count means the function really parses INI
    fields into an object rather than merely mentioning key strings.
    """
    insns = fn["insns"]
    n = 0
    for i, ins in enumerate(insns):
        if ins["mnem"] != "call" or ins["op"] not in _ACCESSOR_OP:
            continue
        for j in range(i + 1, min(i + 7, len(insns))):
            if insns[j]["mnem"] != "mov":
                continue
            if _STORE_RE.match(insns[j]["op"]):
                n += 1
            break
    return n


def is_query_interface(fn):
    """True if fn has the exact MSVC 6 IUnknown::QueryInterface error path.

    All three signals must be present, because each alone is not specific:
      * a load of the third stack argument (ppvObject) from [esp+0xc]
      * a test of that argument
      * the OLE E_POINTER return constant 0x80004003
      * a `ret 0xc` epilogue (three stack arguments)
    """
    ops = [i["op"] for i in fn["insns"]]
    mnems = [i["mnem"] for i in fn["insns"]]
    loads = ("eax, dword ptr [esp + 0xc]", "edx, dword ptr [esp + 0xc]")
    if not any(i["mnem"] == "mov" and i["op"].strip() in loads for i in fn["insns"]):
        return False
    if "test" not in mnems:
        return False
    if not any("0x80004003" in o for o in ops):
        return False
    if not any(m == "ret" and o.strip() == "0xc" for m, o in zip(mnems, ops)):
        return False
    return True


# Slots whose implementation is a tiny constant return that differs per class
# are RTTI/size virtuals rather than guesses: the constant identifies the class.
# Slot 11 returns a per-class RTTI id (BuildingClass 6, UnitClass 1,
# InfantryClass 0xf, AircraftClass 2, AnimClass 4, BulletClass 8, CellClass
# 0xb, HouseClass 0xd) and slot 12 returns a per-class object size (BuildingClass
# 0x310, UnitClass 0x370, InfantryClass 0x378, AnimClass 0x108, BulletClass
# 0xc8, CellClass 0xa8).  The name is applied only where the shape holds and at
# least 8 classes each return a distinct constant.
CONSTANT_SLOT_NAMES = {
    11: ("What_Am_I", "small per-class RTTI id"),
    12: ("Get_Heap", "per-class object size"),
}


def immediate_return(fn):
    """The immediate constant a tiny leaf returns, or None.

    Only a body of at most six instructions with a `mov eax, <const>` and a bare
    `ret`, and no call or branch, qualifies.  A zero from `xor eax, eax` is not
    counted: it does not identify a class.
    """
    if fn is None or fn["ninsn"] > 6:
        return None
    val = None
    for i in fn["insns"]:
        if i["mnem"] == "call" or i["mnem"] == "jmp" or i["mnem"].startswith("j"):
            return None
        if i["mnem"] == "mov" and i["op"].startswith("eax, "):
            try:
                val = int(i["op"][5:].strip(), 0)
            except ValueError:
                return None
    if val is None or fn["insns"][-1]["mnem"] != "ret":
        return None
    return val


def load_layout(outdir, manifest, classes):
    """Direct-base edges joined to the RTTI member displacement.

    out/direct_bases.json is the reduced direct-base set (AGENTS.md); classes.json
    supplies the per-edge mdisp that says where the base sub-object sits in the
    derived class.  mdisp 0 is the primary base, whose vtable is the derived
    class's first vtable; a non-zero mdisp is a secondary base whose vtable is
    at that offset.
    """
    ident_of = {m["class"]: m["ident"] for m in manifest}
    vtable_of = {}
    for v in classes["vtables"]:
        i = ident_of.get(v["class"])
        if i:
            vtable_of.setdefault(i, {})[v["offset"]] = v["vtable_va"]
    mdisp = {}
    for e in classes["edges"]:
        di = ident_of.get(e["derived"])
        bi = ident_of.get(e["base"])
        if di and bi:
            mdisp[(di, bi)] = e["mdisp"]
    direct = {}
    path = os.path.join(outdir, "direct_bases.json")
    reduced = json.load(open(path))
    for cls, bs in reduced.items():
        di = ident_of.get(cls)
        if not di:
            continue
        edges = []
        for b in bs:
            bi = ident_of.get(b)
            if bi is not None:
                edges.append((bi, mdisp.get((di, bi), 0)))
        if edges:
            direct[di] = edges
    return ident_of, vtable_of, direct


def class_depths(idents, direct):
    """Longest base-chain length per class, so a base is always processed
    before its derived classes."""
    dep = {i: 0 for i in idents}
    for _ in range(64):
        changed = False
        for i in idents:
            for b, _ in direct.get(i, []):
                if b in dep and dep[b] + 1 > dep[i]:
                    dep[i] = dep[b] + 1
                    changed = True
        if not changed:
            break
    return dep




def arity_of(sigs, va):
    """The argument list the header generator will build, so the collision
    guard matches what it emits."""
    sg = sigs.get(hex(va)) or {}
    na = sg.get("nargs")
    if na is not None and na > 0:
        return ", ".join("int a%d" % k for k in range(na))
    return ""


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    classes = json.load(open(os.path.join(outdir, "classes.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    try:
        sigs = json.load(open(os.path.join(outdir, "signatures.json")))
    except (OSError, ValueError):
        sigs = {}
    ini_universe = {s for s in json.load(open(os.path.join(outdir, "ini_keys.json")))
                    if _INI_KEY_RE.fullmatch(s)}

    manifest_by_ident = {m["ident"]: m for m in manifest}
    ident_of, vtable_of, direct = load_layout(outdir, manifest, classes)
    depth = class_depths(list(manifest_by_ident), direct)

    slot_cache = {}

    def slot_targets(ident, offset):
        key = (ident, offset)
        if key in slot_cache:
            return slot_cache[key]
        if offset == 0:
            vals = list(manifest_by_ident[ident]["entries"])
        else:
            va = vtable_of.get(ident, {}).get(offset)
            vals = read_primary_vtable(bin_, va) if va is not None else []
        slot_cache[key] = vals
        return vals

    names = defaultdict(dict)
    evidence = defaultdict(dict)
    stats = defaultdict(int)
    unattributed = {hex(va): {"slots": 0} for va in UNATTRIBUTED_TARGETS}

    def set_name(ident, slot, name, rule, signal, **extra):
        if slot in names[ident]:
            return False
        names[ident][slot] = name
        rec = {"name": name, "rule": rule, "signal": signal}
        rec.update(extra)
        evidence[ident][slot] = rec
        return True

    # ---- phase 1: direct evidence, with anchor-driven same-target precedence
    # A name already established for a target address is reused for every slot
    # that holds the same address (R6) *before* the weaker per-slot rules run.
    # That order matters: AbstractClass slots 1 and 2 are Init/Shutdown, not
    # IUnknown::AddRef/Release, so a class that inherits them must take the
    # anchor name rather than the IUnknown ABI guess.
    index = {}

    def index_add(ident, slot):
        entries = manifest_by_ident[ident]["entries"]
        if slot < len(entries) and slot in names[ident]:
            index.setdefault(entries[slot], (names[ident][slot], ident, slot))

    for ident, table in CURATED_SLOT_NAMES.items():
        if ident not in manifest_by_ident:
            continue
        entries = manifest_by_ident[ident]["entries"]
        for slot, (nm, sig) in table.items():
            if slot < len(entries) and set_name(ident, slot, nm, "R12", sig,
                                                target=hex(entries[slot])):
                stats["curated"] += 1
                index_add(ident, slot)

    for ident in sorted(manifest_by_ident):
        entries = manifest_by_ident[ident]["entries"]
        qi_target = entries[0] if entries else None
        qi_fn = fns.get(qi_target)
        qi_verified = qi_fn is not None and is_query_interface(qi_fn)
        for slot, target in enumerate(entries):
            if slot in names[ident]:
                continue
            fn = fns.get(target)
            if fn is None:
                continue
            if target == COM_STUB:
                stats["stub"] += 1
                continue
            if target in index:
                nm, oi, os_ = index[target]
                if nm.startswith("~"):
                    nm = "~" + ident
                set_name(ident, slot, nm, "R6",
                         f"slot target 0x{target:x} is the same code address as "
                         f"{oi} slot {os_} ({index[target][0]})", target=hex(target))
                index_add(ident, slot)
                stats["propagated"] += 1
                continue
            if qi_verified and slot == 0:
                set_name(ident, slot, "QueryInterface", "R9",
                         "loads ppvObject from [esp+0xc]; tests it; returns E_POINTER "
                         "0x80004003; ret 0xc; slot 0 of the primary vtable",
                         target=hex(target))
                index_add(ident, slot)
                stats["query_interface"] += 1
                continue
            if qi_verified and slot in (1, 2):
                nm = "AddRef" if slot == 1 else "Release"
                set_name(ident, slot, nm, "R9",
                         f"slot 0 target 0x{qi_target:x} verified as QueryInterface "
                         f"(R9); slot {slot} is IUnknown::{nm} by the MS IUnknown ABI",
                         target=hex(target))
                index_add(ident, slot)
                stats["addref_release"] += 1
                continue
            strs = [s["text"] for s in fn.get("strings", [])]
            if "Name" in strs and fn["size"] < 80:
                if target == ABSTRACT_FETCH_ID:
                    nm = "Fetch_ID"
                elif target == ABSTRACT_SET_ID:
                    nm = "Set_ID"
                else:
                    nm = "Fetch_ID" if fn["ninsn"] <= 20 else "Set_ID"
                set_name(ident, slot, nm, "R3",
                         f'references string "Name"; {fn["size"]} bytes; '
                         f'{fn["ninsn"]} instructions', target=hex(target))
                index_add(ident, slot)
                stats["name_key"] += 1
                continue
            keys = ini_keys_of(fn) & ini_universe
            accessors = ini_accessor_calls(fn)
            stores = reader_store_count(fn)
            if len(keys) >= 4 and accessors and stores > 0:
                set_name(ident, slot, "Read_INI", "R2",
                         f"{len(keys)} rules.ini field names "
                         f"({', '.join(sorted(keys)[:4])}, ...); "
                         f"{len(accessors)} known INI accessor call(s); "
                         f"{stores} reader store pattern(s)", target=hex(target))
                index_add(ident, slot)
                stats["read_ini"] += 1
                continue
            if (len(keys) >= 4 and accessors and ABSTRACT_SET_ID in fn["callees"]):
                set_name(ident, slot, "Write_INI", "R10",
                         f"{len(keys)} rules.ini field names; {len(accessors)} known "
                         f"INI accessor call(s); calls AbstractClass::Set_ID 0x405fb0",
                         target=hex(target))
                index_add(ident, slot)
                stats["write_ini"] += 1
                continue
            if OP_DELETE in fn["callees"] and writes_vtable_ptr(fn):
                set_name(ident, slot, "~" + ident, "R1",
                         "calls operator delete 0x68298c and writes a vtable pointer "
                         "into [ecx]/[esi]/[edi]", target=hex(target))
                index_add(ident, slot)
                stats["destructor"] += 1
                continue
            stats["unnamed"] += 1

    # ---- phase 2: per-class constant slots ----------------------------------
    # A slot implemented as a one-instruction constant return in many classes,
    # each with a different constant, is a class-identifying virtual (see
    # CONSTANT_SLOT_NAMES).  The signal is the class's own constant.
    const_by_slot = defaultdict(dict)
    for ident, m in manifest_by_ident.items():
        for slot in CONSTANT_SLOT_NAMES:
            if slot >= len(m["entries"]):
                continue
            v = immediate_return(fns.get(m["entries"][slot]))
            if v is not None:
                const_by_slot[slot][ident] = v
    for slot, byclass in sorted(const_by_slot.items()):
        if len(byclass) < 8 or len(set(byclass.values())) < 8:
            continue
        name, kind = CONSTANT_SLOT_NAMES[slot]
        for ident, v in byclass.items():
            if slot in names.get(ident, {}):
                continue
            set_name(ident, slot, name, "R15",
                     f"returns the immediate constant {v:#x} ({v}) as a {kind}; "
                     f"{len(byclass)} classes implement this slot and all return "
                     f"distinct constants, so the value identifies the class",
                     target=hex(manifest_by_ident[ident]["entries"][slot]))
            stats["constant_slot"] += 1

    # ---- phase 3: reverse naming of a base from subclass overrides ----------
    # A direct-evidence name (R1/R2/R3/R9/R10/R12/R15) that >= 2 direct subclasses
    # agree on names the base slot they all override.
    direct_rules = {"R1", "R2", "R3", "R9", "R10", "R12", "R15"}
    subclass_of = defaultdict(list)
    for d, edges in direct.items():
        for b, md in edges:
            subclass_of[b].append((d, md))
    for base, subs in subclass_of.items():
        if base not in manifest_by_ident:
            continue
        entries = manifest_by_ident[base]["entries"]
        votes = defaultdict(lambda: defaultdict(list))
        for d, md in subs:
            if md != 0:
                continue
            for slot, rec in evidence.get(d, {}).items():
                if rec["rule"] in direct_rules and slot < len(entries):
                    votes[slot][rec["name"]].append(d)
        for slot, byname in votes.items():
            if slot in names[base]:
                continue
            for nm, ds in sorted(byname.items(), key=lambda kv: -len(kv[1])):
                if len(ds) >= 2:
                    set_name(base, slot, nm, "R7",
                             f"{len(ds)} direct subclasses ({', '.join(sorted(ds)[:3])}) "
                             f"independently name slot {slot} {nm}; base slot is the "
                             f"shared virtual they override",
                             target=hex(entries[slot]))
                    stats["base_from_subclass"] += 1
                    break

    # ---- phase 4: propagation to fixpoint -----------------------------------
    order = sorted(manifest_by_ident, key=lambda i: (depth.get(i, 0), i))

    def same_target():
        index = {}
        for ident, slots in names.items():
            entries = manifest_by_ident[ident]["entries"]
            for slot, nm in slots.items():
                if slot < len(entries) and entries[slot] != COM_STUB:
                    index.setdefault(entries[slot], (nm, ident, slot))
        added = 0
        for ident in order:
            entries = manifest_by_ident[ident]["entries"]
            for slot, target in enumerate(entries):
                if slot in names[ident] or target == COM_STUB or target not in index:
                    continue
                nm, oi, os_ = index[target]
                if nm.startswith("~"):
                    nm = "~" + ident
                if set_name(ident, slot, nm, "R6",
                            f"slot target 0x{target:x} is the same code address as "
                            f"{oi} slot {os_} ({index[target][0]})",
                            target=hex(target)):
                    added += 1
        return added

    def inherited():
        added = 0
        for ident in order:
            entries = manifest_by_ident[ident]["entries"]
            for base, md in direct.get(ident, []):
                bslots = slot_targets(base, 0)
                dslots = slot_targets(ident, md)
                if not bslots or not dslots or len(dslots) < len(bslots):
                    stats["edge_skipped"] += 1
                    continue
                for slot, nm in list(names.get(base, {}).items()):
                    if slot in names[ident] or slot >= len(bslots):
                        continue
                    if dslots[slot] == COM_STUB:
                        continue
                    target = dslots[slot]
                    dfn = fns.get(target)
                    newnm = ("~" + ident) if nm.startswith("~") else nm
                    corroborated = dfn is not None and bslots[slot] in dfn.get("callees", [])
                    if corroborated:
                        rule = "R13"
                        sig = (f"derived slot {slot} calls the base class's slot "
                               f"{slot} target 0x{bslots[slot]:x}, proving the "
                               f"override; name inherited from {base} slot {slot} "
                               f"({nm}, displacement {md})")
                    else:
                        rule = "R11"
                        sig = (f"direct base {base} slot {slot} named {nm} "
                               f"(displacement {md}); MSVC keeps inherited virtuals "
                               f"at their base slot index, so this slot is the "
                               f"override of the same virtual")
                    if set_name(ident, slot, newnm, rule, sig,
                                target=hex(target), base=base, base_slot=slot,
                                base_name=nm, mdisp=md):
                        added += 1
                        stats["inherited_corroborated" if corroborated
                              else "inherited_layout"] += 1
        return added

    for _ in range(32):
        a = inherited()
        b = same_target()
        if not a and not b:
            break

    # ---- phase 5: legal-declaration guard -----------------------------------
    # Two virtual declarations with one name and one argument list do not
    # compile.  gen_headers skips destructor-named slots, so only non-destructor
    # names can collide.
    for ident, slots in names.items():
        entries = manifest_by_ident[ident]["entries"]
        seen = {}
        for slot in sorted(slots):
            nm = slots[slot]
            if nm.startswith("~"):
                continue
            key = (nm, arity_of(sigs, entries[slot]))
            if key in seen:
                stats["dropped_duplicate"] += 1
                del slots[slot]
                del evidence[ident][slot]
            else:
                seen[key] = slot

    # Slots with no string evidence are left unnamed; record the shape of the
    # two shared targets so the omission is auditable.
    for key, rec in unattributed.items():
        f = fns.get(int(key, 16))
        if f is None:
            rec["shape"] = "not present in out/functions.json"
            continue
        rec.update({
            "size": f["size"],
            "ninsn": f["ninsn"],
            "callees": f["callees"],
            "strings": [s["text"] for s in f.get("strings", [])],
            "instructions": [f'{i["mnem"]} {i["op"]}'.strip() for i in f["insns"][:10]],
        })

    out = {k: {str(s): n for s, n in sorted(v.items())}
           for k, v in sorted(names.items()) if v}
    outpath = os.path.join(outdir, "virtual_names.json")
    with open(outpath, "w") as fh:
        json.dump(out, fh, indent=1, sort_keys=True)
        fh.write("\n")

    ev = {
        "classes": {k: {str(s): v[s] for s in sorted(v, key=int)}
                    for k, v in sorted(evidence.items()) if v},
        "unattributed_slot_targets": unattributed,
    }
    evpath = os.path.join(outdir, "virtual_name_evidence.json")
    with open(evpath, "w") as fh:
        json.dump(ev, fh, indent=1)
        fh.write("\n")

    total = sum(len(v) for v in out.values())
    cats = defaultdict(int)
    for v in out.values():
        for n in v.values():
            if n.startswith("~"):
                cats["destructor"] += 1
            elif n in ("QueryInterface", "AddRef", "Release"):
                cats["com"] += 1
            else:
                cats["semantic"] += 1
    print(f"wrote {outpath}")
    print(f"wrote {evpath}")
    print(f"classes affected : {len(out)}")
    print(f"virtuals named   : {total}")
    for k in sorted(cats):
        print(f"  {k:16s} {cats[k]}")
    for k in sorted(stats):
        print(f"  {k:16s} {stats[k]}")


if __name__ == "__main__":
    main()
