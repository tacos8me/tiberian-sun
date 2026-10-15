#!/usr/bin/env python3
"""Full evidence dump for the map/display/UI layer classes."""
import json, struct, sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from mapdump import FUNCS, rd32, va2off, DATA, VT, CLASSES

def typename_va(col_va):
    """COL: sig, offset, cd_va, td_va, chd_va -> read td_va then name ptr."""
    td = rd32(col_va + 12)
    if td is None:
        return None, None
    nameptr = rd32(td + 8)
    if nameptr is None:
        return td, None
    o = va2off(nameptr)
    s = DATA[o:o+120].split(b'\0')[0].decode('latin1', 'replace') if o else None
    return td, (nameptr, s)

def dump(cls):
    for v in VT:
        if v['class'].replace('class ', '') == cls:
            print('### %s vtable=0x%06x col=0x%06x colptr=0x%06x off=%d' % (
                cls, v['vtable_va'], v['col_va'], v['col_ptr_va'], v['offset']))
            td, nm = typename_va(v['col_va'])
            print('    td=0x%x name=%s' % (td or 0, nm))
            n = 0
            for i in range(400):
                e = rd32(v['vtable_va'] + 4 * i)
                if e is None or e < 0x401000 or e >= 0x6bb000:
                    break
                f = FUNCS.get(e)
                print('    [%3d] 0x%06x ninsn=%s' % (i, e, f['ninsn'] if f else '?'))
                n += 1
            print('    total=%d' % n)

if __name__ == '__main__':
    for c in sys.argv[1:]:
        dump(c)
