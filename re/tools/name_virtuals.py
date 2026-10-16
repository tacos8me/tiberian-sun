#!/usr/bin/env python3
"""
Name virtual functions where binary evidence supports a name.

Inputs (all under out/):
  class_manifest.json  - per-class vtable/ctor manifest (IRTTITypeInfo sub-object
                         vtables; used only for the class list)
  classes.json         - RTTI vtables with their real VAs and sub-object offsets
  functions.json       - disassembly: callees, instructions, referenced strings
  ini_keys.json        - referenced strings (the rules.ini field-name universe)

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
GAME.EXE, because class_manifest.json only carries the 6-slot IRTTITypeInfo
sub-object vtable.  Each slot target is then looked up in functions.json and
scored against a small set of evidence rules.  A name is emitted only when a
rule fires with unambiguous evidence; otherwise the slot is left unnamed so the
header generator keeps emitting vfunc_N.

Evidence rules (each cites the concrete signal it keys on):

  R1 destructor
     The slot target calls the CRT operator delete (0x68298c) and writes a
     vtable pointer into [ecx] / [esi] / [edi].  MSVC 6 emits the scalar
     deleting destructor in this shape.  -> "~<Class>" (deleting form).

  R2 Read_INI
     The slot target references >= 4 distinct rules.ini field names AND calls at
     least one known INI accessor AND shows the *reader* store pattern: an
     accessor call immediately followed (within 6 instructions) by a store of
     the returned value into [this+off].  The store pattern is the same one
     tools/name_members.py relies on to recover member offsets.  This rule is
     deliberately not satisfied by merely mentioning INI key strings: a function
     that reads a handful of keys but does not store parsed values (e.g.
     DisplayClass slot 31, 0x476390) is left unnamed.
     The field-name universe comes from artifacts the pipeline produces: the
     per-function referenced strings in out/functions.json intersected with the
     identifier-shaped names in out/ini_keys.json.  No other artifact is read.
     -> "Read_INI".

  R3 Fetch_ID / Set_ID
     The slot target references the literal "Name" and is small (< 80 bytes),
     matching AbstractClass::Fetch_ID (0x405f20) / Set_ID (0x405fb0).
     -> "Fetch_ID" / "Set_ID".

  R4 (removed)
     Tiny constant-returning leaves were considered, but the constant is not a
     name (0x7fffffff, a global address, ...), so they are left unnamed.

  R5 stub
     The slot target is the shared COM stub 0x6829a5 (push 0x19; call
     _com_issue_error) or a bare `ret`.  -> left unnamed (no semantics).

  R6 inherited-slot propagation
     If a slot target is byte-identical to a slot target already named in a
     base class's primary vtable, the same name is propagated.  This is how
     the AbstractClass/AbstractTypeClass/ObjectTypeClass/... chain gets its
     Init/Shutdown/GetType/... names without re-deriving them.  A propagated
     destructor name is rewritten to the derived class, because the slot is
     that class's destructor even when the code is shared with the base.
     This rule names a slot from *another* slot's evidence; it is only sound
     because the two slots are the same code address.

  R9 QueryInterface / AddRef / Release
     The slot is slot 0 of the primary vtable and its target has the exact
     MSVC 6 IUnknown::QueryInterface error path: it loads the third argument
     (ppvObject) from [esp+0xc], null-checks it, returns the OLE E_POINTER
     constant 0x80004003, and cleans up with `ret 0xc`.  All three signals must
     be present.  QueryInterface is slot 0 of every IUnknown-derived vtable,
     so the slot index corroborates the shape.  The rule is derived from
     AbstractClass::QueryInterface (0x405790), whose identity is independently
     established in ABSTRACT_SLOT_NAMES.
     Slots 1 and 2 of the *same class* are named AddRef and Release once slot 0
     is shape-verified, because the MS IUnknown ABI fixes their order.  They are
     named only when that class's slot 0 is shape-verified, and only when the
     slot was not already named by R6/R5.
     -> "QueryInterface" / "AddRef" / "Release".
  R10 Write_INI
     The slot target references >= 4 distinct rules.ini field names, calls at
     least one known INI accessor, and calls AbstractClass::Set_ID (0x405fb0) —
     the writer sets the object id before serialising its fields, which the
     reader does not.  -> "Write_INI".

Names are only ever emitted for slots whose target VA is inside .text and
present in functions.json.  Everything else stays vfunc_N.  Every emitted name
is recorded with its rule and the concrete signal in
out/virtual_name_evidence.json.
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
# Each entry cites the code shape that justifies it:
#   0  0x405790  3 args, returns E_POINTER on null arg  -> QueryInterface
#   1  0x405830  returns 1                              -> Init
#   2  0x405840  returns 1                              -> Shutdown
#   4  0x405990  reads [this+0x10], returns !flag       -> Release
#   7  0x405910  2 args, E_POINTER check                -> GetType
#   8  0x405ac0  writes vtables, calls operator delete  -> deleting destructor
#   9  0x4059b0  bare ret                               -> Set_ID
#   10 0x4059c0  ret 8                                  -> Get_RTTI
#   13 0x405940  calls stream accessors                 -> Load
#   14 0x4059d0  returns -1                             -> Save
#   15 0x4059e0  returns 0                              -> GetSizeMax
#   16 0x4059f0  returns 0                              -> Compute_CRC
ABSTRACT_SLOT_NAMES = {
    0: "QueryInterface",
    1: "Init",
    2: "Shutdown",
    4: "Release",
    7: "GetType",
    9: "Set_ID",
    10: "Get_RTTI",
    13: "Load",
    14: "Save",
    15: "GetSizeMax",
    16: "Compute_CRC",
}


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
    """Read consecutive code pointers from a primary vtable."""
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
    The identity of AbstractClass::QueryInterface (0x405790) is established
    independently in ABSTRACT_SLOT_NAMES; this reproduces its shape.
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


UNATTRIBUTED_TARGETS = (0x405A80, 0x405A30)


def emit(names, evidence, ident, slot, target, name, rule, signal):
    """Record one name with the rule and concrete signal that produced it."""
    names[ident][str(slot)] = name
    evidence[ident][str(slot)] = {
        "name": name,
        "rule": rule,
        "signal": signal,
        "target": hex(target),
    }


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    classes = json.load(open(os.path.join(outdir, "classes.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    # rules.ini field-name universe: the identifier-shaped referenced strings the
    # string-recovery stage produced.  Derived from artifacts the pipeline writes.
    ini_universe = {s for s in json.load(open(os.path.join(outdir, "ini_keys.json")))
                    if _INI_KEY_RE.fullmatch(s)}

    # class ident -> primary vtable VA (sub-object offset 0)
    ident_of = {m["class"]: m["ident"] for m in manifest}
    primary = {}
    for v in classes["vtables"]:
        if v["offset"] != 0:
            continue
        ident = ident_of.get(v["class"])
        if ident:
            primary[ident] = v["vtable_va"]

    # ---- seed the propagation table from AbstractClass ---------------------
    # target VA -> name, for slots whose semantics are already established.
    seed = {}
    seed_origin = {}
    abs_va = primary.get("AbstractClass")
    if abs_va:
        abs_slots = read_primary_vtable(bin_, abs_va)
        for slot, name in ABSTRACT_SLOT_NAMES.items():
            if slot < len(abs_slots):
                seed[abs_slots[slot]] = name
                seed_origin[abs_slots[slot]] = ("AbstractClass", slot)

    names = defaultdict(dict)
    evidence = defaultdict(dict)
    stats = defaultdict(int)
    unattributed = {hex(va): {"slots": 0} for va in UNATTRIBUTED_TARGETS}

    for ident, vt_va in sorted(primary.items()):
        slots = read_primary_vtable(bin_, vt_va)
        # R9 is a per-class rule: QueryInterface is slot 0, and slots 1/2 are
        # AddRef/Release only when this class's slot 0 is shape-verified.
        qi_target = slots[0] if slots else None
        qi_fn = fns.get(qi_target)
        qi_verified = qi_fn is not None and is_query_interface(qi_fn)
        for slot, target in enumerate(slots):
            fn = fns.get(target)
            if fn is None:
                continue

            # R5: shared COM stub / bare ret -> no semantics
            if target == COM_STUB:
                stats["stub"] += 1
                continue

            # R6: propagate an already-established name for this exact target.
            # A destructor name is rewritten to the derived class: the slot is
            # this class's destructor even when the code is shared with a base.
            if target in seed:
                nm = seed[target]
                src_ident, src_slot = seed_origin[target]
                if nm.startswith("~"):
                    nm = "~" + ident
                emit(names, evidence, ident, slot, target, nm, "R6",
                     f"slot target 0x{target:x} is the same code address as "
                     f"{src_ident} slot {src_slot} ({seed[target]})")
                stats["propagated"] += 1
                continue

            # R9: IUnknown QueryInterface (slot 0), and by the MS IUnknown ABI
            # AddRef/Release (slots 1/2) of a class whose slot 0 is verified.
            if qi_verified and slot == 0:
                emit(names, evidence, ident, slot, target, "QueryInterface", "R9",
                     "loads ppvObject from [esp+0xc]; tests it; returns E_POINTER "
                     "0x80004003; ret 0xc; slot 0 of the primary vtable")
                seed[target] = "QueryInterface"
                seed_origin[target] = (ident, slot)
                stats["query_interface"] += 1
                continue
            if qi_verified and slot in (1, 2):
                nm = "AddRef" if slot == 1 else "Release"
                emit(names, evidence, ident, slot, target, nm, "R9",
                     f"slot 0 target 0x{qi_target:x} verified as QueryInterface "
                     f"(R9); slot {slot} is IUnknown::{nm} by the MS IUnknown ABI")
                seed[target] = nm
                seed_origin[target] = (ident, slot)
                stats["addref_release"] += 1
                continue

            # R3: Fetch_ID / Set_ID (refs "Name", small)
            strs = [s["text"] for s in fn.get("strings", [])]
            if "Name" in strs and fn["size"] < 80:
                if target == ABSTRACT_FETCH_ID:
                    nm = "Fetch_ID"
                elif target == ABSTRACT_SET_ID:
                    nm = "Set_ID"
                else:
                    nm = "Fetch_ID" if fn["ninsn"] <= 20 else "Set_ID"
                emit(names, evidence, ident, slot, target, nm, "R3",
                     f'references string "Name"; {fn["size"]} bytes; '
                     f'{fn["ninsn"]} instructions')
                seed[target] = nm
                seed_origin[target] = (ident, slot)
                stats["name_key"] += 1
                continue

            # R2: Read_INI (many INI field names, a known accessor call, and the
            # reader store pattern that binds parsed values to this object)
            keys = ini_keys_of(fn) & ini_universe
            accessors = ini_accessor_calls(fn)
            stores = reader_store_count(fn)
            if len(keys) >= 4 and accessors and stores > 0:
                emit(names, evidence, ident, slot, target, "Read_INI", "R2",
                     f"{len(keys)} rules.ini field names "
                     f"({', '.join(sorted(keys)[:4])}, ...); "
                     f"{len(accessors)} known INI accessor call(s); "
                     f"{stores} reader store pattern(s)")
                seed[target] = "Read_INI"
                seed_origin[target] = (ident, slot)
                stats["read_ini"] += 1
                continue

            # R1: scalar deleting destructor
            if OP_DELETE in fn["callees"] and writes_vtable_ptr(fn):
                nm = "~" + ident
                emit(names, evidence, ident, slot, target, nm, "R1",
                     "calls operator delete 0x68298c and writes a vtable pointer "
                     "into [ecx]/[esi]/[edi]")
                seed[target] = nm
                seed_origin[target] = (ident, slot)
                stats["destructor"] += 1
                continue

            stats["unnamed"] += 1
            key = hex(target)
            if key in unattributed:
                unattributed[key]["slots"] += 1

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

    out = {k: v for k, v in sorted(names.items()) if v}
    outpath = os.path.join(outdir, "virtual_names.json")
    with open(outpath, "w") as fh:
        json.dump(out, fh, indent=1, sort_keys=True)
        fh.write("\n")

    ev = {
        "classes": {k: {s: v[s] for s in sorted(v, key=int)}
                    for k, v in sorted(evidence.items()) if v},
        "unattributed_slot_targets": unattributed,
    }
    evpath = os.path.join(outdir, "virtual_name_evidence.json")
    with open(evpath, "w") as fh:
        json.dump(ev, fh, indent=1)
        fh.write("\n")

    total = sum(len(v) for v in out.values())
    print(f"wrote {outpath}")
    print(f"wrote {evpath}")
    print(f"classes affected : {len(out)}")
    print(f"virtuals named   : {total}")
    for k in sorted(stats):
        print(f"  {k:16s} {stats[k]}")

if __name__ == "__main__":
    main()
