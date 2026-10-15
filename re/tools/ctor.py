#!/usr/bin/env python3
"""Extract member-init offsets from constructors + string refs for a class's methods."""
import json, struct, sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from mapdump import FUNCS, rd32, va2off, DATA

def ctor_offsets(fva):
    f = FUNCS.get(fva)
    if not f:
        return []
    offs = {}
    for ins in f['insns']:
        op = ins['op']
        m = re.match(r'dword ptr \[(ecx|esi|edi|eax)(?: \+ (0x[0-9a-f]+))?\], ', op)
        if m and ins['mnem'] in ('mov', 'movzx', 'movsx'):
            base = m.group(1)
            off = int(m.group(2), 16) if m.group(2) else 0
            offs.setdefault(off, []).append((ins['va'], ins['mnem'], op))
    return sorted(offs.items())

def strings_of(fva):
    f = FUNCS.get(fva)
    return f.get('strings', []) if f else []

if __name__ == '__main__':
    for a in sys.argv[1:]:
        fva = int(a, 16)
        print('--- ctor 0x%06x ---' % fva)
        for off, hits in ctor_offsets(fva):
            print('  +0x%03x : %s' % (off, hits[0][2]))
        s = strings_of(fva)
        if s:
            print('  strings:', s[:20])
