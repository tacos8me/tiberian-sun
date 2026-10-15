#!/usr/bin/env python3
"""
MSVC 6.0 RTTI recovery for GAME.EXE (Command & Conquer: Tiberian Sun, 1999).

Recovers the complete C++ class hierarchy from the binary's RTTI data.

MSVC 6 type_info layout (confirmed against this binary):
    +0x00  vftable ptr  -> type_info vftable (in .rdata)
    +0x04  spare (0)
    +0x08  inline mangled name, NUL terminated

RTTIClassHierarchyDescriptor:
    +0x00  signature (0)
    +0x04  attributes
    +0x08  numBaseClasses
    +0x0c  pBaseClassArray

RTTIBaseClassDescriptor:
    +0x00  pTypeDescriptor
    +0x04  numContainedBases
    +0x08  PMD.mdisp
    +0x0c  PMD.pdisp
    +0x10  PMD.vdisp
    +0x14  attributes

RTTICompleteObjectLocator:
    +0x00  signature (0)
    +0x04  offset
    +0x08  pTypeDescriptor
    +0x0c  pClassDescriptor
    +0x10  pSelf
"""
import json
import os
import struct
import sys

import pefile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from demangle import demangle as demangle_msvc

IMAGE_BASE = 0x400000


class Image:
    def __init__(self, path):
        self.pe = pefile.PE(path, fast_load=True)
        self.data = self.pe.__data__
        self.sections = []
        for s in self.pe.sections:
            self.sections.append((
                IMAGE_BASE + s.VirtualAddress,
                IMAGE_BASE + s.VirtualAddress + s.Misc_VirtualSize,
                s.PointerToRawData,
                s.SizeOfRawData,
                s.Name.decode(errors="replace").strip("\x00"),
            ))

    def read(self, va, n):
        for vstart, vend, raw, rawsize, name in self.sections:
            if vstart <= va < vend:
                off = raw + (va - vstart)
                return self.data[off:off + n]
        return None

    def u32(self, va):
        b = self.read(va, 4)
        return struct.unpack("<I", b)[0] if b and len(b) == 4 else None

    def cstr(self, va, maxlen=1024):
        b = self.read(va, maxlen)
        if not b:
            return None
        z = b.find(b"\x00")
        if z >= 0:
            b = b[:z]
        try:
            return b.decode("ascii")
        except UnicodeDecodeError:
            return b.decode("latin-1")

    def valid_va(self, va):
        return any(vs <= va < ve for vs, ve, _, _, _ in self.sections)

    def va_of(self, file_off):
        for vs, ve, raw, rawsize, name in self.sections:
            if raw <= file_off < raw + rawsize:
                return vs + (file_off - raw)
        return None


def parse(path):
    img = Image(path)
    data = img.data

    # --- Locate the type_info vftable: it is the dword immediately preceding
    #     the inline name of the ".?AVtype_info@@" descriptor.
    ti_name_off = data.find(b".?AVtype_info@@")
    if ti_name_off < 0:
        raise SystemExit("type_info descriptor not found")
    ti_vftable = struct.unpack("<I", data[ti_name_off - 8:ti_name_off - 4])[0]
    print(f"type_info vftable = 0x{ti_vftable:08x}")

    # --- Pass 1: enumerate type descriptors by their vftable pointer.
    desc_by_name = {}   # desc_va -> mangled name
    name_to_desc = {}   # mangled name -> desc_va
    pat = struct.pack("<I", ti_vftable)
    start = 0
    while True:
        j = data.find(pat, start)
        if j < 0:
            break
        start = j + 1
        desc_va = img.va_of(j)
        if desc_va is None:
            continue
        nm = img.cstr(desc_va + 8)
        if not nm or not nm.startswith(".?A"):
            continue
        desc_by_name[desc_va] = nm
        name_to_desc.setdefault(nm, desc_va)

    print(f"type descriptors : {len(desc_by_name)}")

    # --- Pass 2: parse class hierarchy descriptors.
    classes = {}
    for desc_va, nm in desc_by_name.items():
        # The CHD is referenced from a Complete Object Locator; instead of
        # scanning, find CHDs by validating their base-class arrays.
        pass

    # Scan the whole image for CHD candidates.
    for off in range(0, len(data) - 16, 4):
        sig = struct.unpack("<I", data[off:off + 4])[0]
        if sig != 0:
            continue
        attrs = struct.unpack("<I", data[off + 4:off + 8])[0]
        num = struct.unpack("<I", data[off + 8:off + 12])[0]
        arr = struct.unpack("<I", data[off + 12:off + 16])[0]
        if not (1 <= num <= 64):
            continue
        if not img.valid_va(arr):
            continue
        chd_va = img.va_of(off)
        if chd_va is None:
            continue
        bases = []
        ok = True
        for k in range(num):
            bcd = img.u32(arr + k * 4)
            if bcd is None or not img.valid_va(bcd):
                ok = False
                break
            td = img.u32(bcd)
            if td is None or td not in desc_by_name:
                ok = False
                break
            bases.append({
                "type": desc_by_name[td],
                "demangled": demangle_msvc(desc_by_name[td]),
                "numContainedBases": img.u32(bcd + 4),
                "mdisp": img.u32(bcd + 8),
                "pdisp": img.u32(bcd + 12),
                "vdisp": img.u32(bcd + 16),
                "attributes": img.u32(bcd + 20),
                "bcd_va": bcd,
            })
        if not ok or not bases:
            continue
        classes[chd_va] = {
            "chd_va": chd_va,
            "attributes": attrs,
            "numBaseClasses": num,
            "bases": bases,
            "name": bases[0]["type"],
            "demangled": bases[0]["demangled"],
        }

    print(f"class hierarchies: {len(classes)}")

    # --- Pass 3: Complete Object Locators bind vtables to classes.
    #     Layout confirmed against this binary:
    #       +0x00 signature (0)
    #       +0x04 offset
    #       +0x08 reserved (0)
    #       +0x0c pTypeDescriptor
    #       +0x10 pClassDescriptor
    #     A COL is referenced from the dword immediately preceding its vtable.
    col_to_class = {}
    for off in range(0, len(data) - 20, 4):
        sig = struct.unpack("<I", data[off:off + 4])[0]
        if sig != 0:
            continue
        coff = struct.unpack("<I", data[off + 4:off + 8])[0]
        td = struct.unpack("<I", data[off + 12:off + 16])[0]
        cd = struct.unpack("<I", data[off + 16:off + 20])[0]
        col_va = img.va_of(off)
        if col_va is None:
            continue
        if td not in desc_by_name or cd not in classes:
            continue
        col_to_class[col_va] = {
            "col_va": col_va,
            "offset": coff,
            "type": desc_by_name[td],
            "demangled": demangle_msvc(desc_by_name[td]),
            "chd_va": cd,
        }

    print(f"object locators  : {len(col_to_class)}")

    # --- Pass 4: bind vtables. The COL pointer is the dword immediately
    #     preceding the vtable's first entry.
    vtables = []
    for vs2, ve2, raw2, rawsize2, nm in img.sections:
        if nm not in (".rdata", ".data"):
            continue
        b2 = data[raw2:raw2 + rawsize2]
        for i in range(0, len(b2) - 4, 4):
            p = struct.unpack("<I", b2[i:i + 4])[0]
            if p in col_to_class:
                vtables.append({
                    "vtable_va": vs2 + i + 4,
                    "col_ptr_va": vs2 + i,
                    "section": nm,
                    "col_va": p,
                    "class": col_to_class[p]["demangled"],
                    "mangled": col_to_class[p]["type"],
                    "offset": col_to_class[p]["offset"],
                })

    print(f"vtables found    : {len(vtables)}")
    return img, classes, col_to_class, vtables, desc_by_name


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    img, classes, cols, vtables, descs = parse(path)

    edges = []
    for chd_va, c in classes.items():
        for b in c["bases"][1:]:
            edges.append({
                "derived": c["demangled"],
                "base": b["demangled"],
                "mdisp": b["mdisp"], "pdisp": b["pdisp"], "vdisp": b["vdisp"],
                "attributes": b["attributes"],
            })

    os.makedirs(outdir, exist_ok=True)
    with open(f"{outdir}/classes.json", "w") as f:
        json.dump({
            "classes": list(classes.values()),
            "edges": edges,
            "vtables": vtables,
            "locators": list(cols.values()),
        }, f, indent=1)

    with open(f"{outdir}/hierarchy.txt", "w") as f:
        for c in sorted(classes.values(), key=lambda x: x["demangled"]):
            f.write(f"{c['demangled']}\n")
            for b in c["bases"][1:]:
                f.write(f"    : {b['demangled']}  "
                        f"(mdisp={b['mdisp']} pdisp={b['pdisp']} vdisp={b['vdisp']})\n")

    with open(f"{outdir}/vtables.txt", "w") as f:
        for v in sorted(vtables, key=lambda x: x["vtable_va"]):
            f.write(f"0x{v['vtable_va']:08x} {v['section']} "
                    f"off={v['offset']:5d} {v['class']}\n")

    print(f"\nwrote {outdir}/classes.json, hierarchy.txt, vtables.txt")


if __name__ == "__main__":
    main()
