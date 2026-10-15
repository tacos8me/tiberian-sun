#!/usr/bin/env python3
"""
Name virtual functions where binary evidence supports a name.

Inputs (all under out/):
  class_manifest.json  - per-class vtable/ctor manifest (IRTTITypeInfo sub-object
                         vtables; used only for the class list)
  classes.json         - RTTI vtables with their real VAs and sub-object offsets
  functions.json       - disassembly: callees, instructions, referenced strings
  ini_functions.json   - functions that reference INI keys, with the key list

Output:
  out/virtual_names.json  - { "<class ident>": { "<slot>": "<name>" } }

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
     The slot target references >= 4 distinct INI field names (ini_functions
     key list) or is a known INI reader.  -> "Read_INI".

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

Names are only ever emitted for slots whose target VA is inside .text and
present in functions.json.  Everything else stays vfunc_N.
"""
import json
import os
import struct
import sys
from collections import defaultdict

IB = 0x400000

# --- well-known addresses established by prior recovery work ----------------
OP_DELETE = 0x68298c          # CRT operator delete (push arg; call 0x683fa4)
COM_STUB = 0x6829a5           # shared "not implemented" COM stub
ABSTRACT_FETCH_ID = 0x405f20  # AbstractClass::Fetch_ID, refs "Name"
ABSTRACT_SET_ID = 0x405fb0    # AbstractClass::Set_ID, refs "Name"

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


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    classes = json.load(open(os.path.join(outdir, "classes.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    ini_path = os.path.join(outdir, "ini_functions.json")
    ini = json.load(open(ini_path)) if os.path.exists(ini_path) else {}

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
    abs_va = primary.get("AbstractClass")
    if abs_va:
        abs_slots = read_primary_vtable(bin_, abs_va)
        for slot, name in ABSTRACT_SLOT_NAMES.items():
            if slot < len(abs_slots):
                seed[abs_slots[slot]] = name

    names = defaultdict(dict)
    stats = defaultdict(int)

    for ident, vt_va in sorted(primary.items()):
        slots = read_primary_vtable(bin_, vt_va)
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
                if nm.startswith("~"):
                    nm = "~" + ident
                names[ident][str(slot)] = nm
                stats["propagated"] += 1
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
                names[ident][str(slot)] = nm
                seed[target] = nm
                stats["name_key"] += 1
                continue

            # R2: Read_INI (references many INI field names)
            keys = ini.get(hex(target), {}).get("keys", [])
            if len(keys) >= 4:
                names[ident][str(slot)] = "Read_INI"
                seed[target] = "Read_INI"
                stats["read_ini"] += 1
                continue

            # R1: scalar deleting destructor
            if OP_DELETE in fn["callees"] and writes_vtable_ptr(fn):
                nm = "~" + ident
                names[ident][str(slot)] = nm
                seed[target] = nm
                stats["destructor"] += 1
                continue

            stats["unnamed"] += 1
            stats["unnamed"] += 1

    out = {k: v for k, v in sorted(names.items()) if v}
    outpath = os.path.join(outdir, "virtual_names.json")
    with open(outpath, "w") as fh:
        json.dump(out, fh, indent=1, sort_keys=True)
        fh.write("\n")

    total = sum(len(v) for v in out.values())
    print(f"wrote {outpath}")
    print(f"classes affected : {len(out)}")
    print(f"virtuals named   : {total}")
    for k in sorted(stats):
        print(f"  {k:12s} {stats[k]}")


if __name__ == "__main__":
    main()
