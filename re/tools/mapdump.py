#!/usr/bin/env python3
"""Dump vtable entries + constructor member-init offsets for assigned classes."""
import json, struct, sys, os

RE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(RE, 'bin', 'GAME.EXE')
IMAGEBASE = 0x400000

# section table from CONTEXT.md
SECTIONS = [
    ('.text', 0x401000, 0x2b0000),
    ('.rdata', 0x698000, 0x23000),
    ('.data', 0x6bb000, 0x17c000),
    ('.rsrc', 0x837000, 0x1000),
]

def va2off(va):
    # parse real PE headers for accuracy
    return va - IMAGEBASE  # placeholder, replaced below

def load_pe():
    data = open(BIN, 'rb').read()
    e_lfanew = struct.unpack_from('<I', data, 0x3c)[0]
    nsec = struct.unpack_from('<H', data, e_lfanew + 6)[0]
    opt_size = struct.unpack_from('<H', data, e_lfanew + 20)[0]
    sec_off = e_lfanew + 24 + opt_size
    secs = []
    for i in range(nsec):
        o = sec_off + i * 40
        name = data[o:o+8].rstrip(b'\0').decode('latin1')
        vsize, vaddr, rawsize, rawptr = struct.unpack_from('<IIII', data, o + 8)
        secs.append((name, vaddr + IMAGEBASE, vsize, rawptr, rawsize))
    return data, secs

DATA, SECS = load_pe()

def va2off(va):
    for name, vaddr, vsize, rawptr, rawsize in SECS:
        if vaddr <= va < vaddr + max(vsize, rawsize):
            return rawptr + (va - vaddr)
    return None

def rd32(va):
    o = va2off(va)
    if o is None or o + 4 > len(DATA):
        return None
    return struct.unpack_from('<I', DATA, o)[0]

def rdbytes(va, n):
    o = va2off(va)
    return DATA[o:o+n]

FUNCS = {f['va']: f for f in json.load(open(os.path.join(RE, 'out', 'functions.json')))['functions']}
CLASSES = json.load(open(os.path.join(RE, 'out', 'classes.json')))
VT = CLASSES['vtables']

def vtable_entries(vtva, maxn=200):
    out = []
    for i in range(maxn):
        v = rd32(vtva + 4 * i)
        if v is None or v < 0x401000 or v >= 0x6bb000:
            break
        out.append(v)
    return out

def find_ctor(vtva):
    """Find functions that store vtva (or vtva+4) into memory."""
    hits = []
    for f in FUNCS.values():
        for ins in f['insns']:
            if ins['mnem'] == 'mov' and 'imm' in ins.get('op', ''):
                pass
        # cheap: check bytes for the immediate
        for ins in f['insns']:
            if ins['mnem'] == 'mov' and ins['op'].startswith('dword ptr [') and ', ' in ins['op']:
                try:
                    imm = int(ins['op'].split(', ')[1], 16)
                except ValueError:
                    continue
                if imm in (vtva, vtva + 4):
                    hits.append((f['va'], ins['va'], ins['op']))
    return hits

def member_inits(fva):
    """Collect [ecx+off] / [esi+off] / [edi+off] stores in a function."""
    f = FUNCS.get(fva)
    if not f:
        return []
    res = []
    for ins in f['insns']:
        if ins['mnem'] in ('mov', 'movzx', 'movsx', 'and', 'or', 'fld', 'fstp', 'fst', 'push', 'lea', 'cmp', 'add', 'sub', 'xor', 'test', 'inc', 'dec'):
            op = ins['op']
            if 'ptr [' in op:
                res.append((ins['va'], ins['mnem'], op))
    return res

if __name__ == '__main__':
    targets = sys.argv[1:]
    for t in targets:
        for v in VT:
            if v['class'] == t or v['class'].endswith('::' + t) or t in v['class']:
                print('=== %s vtable=0x%06x off=%d' % (v['class'], v['vtable_va'], v['offset']))
                ents = vtable_entries(v['vtable_va'])
                for i, e in enumerate(ents):
                    f = FUNCS.get(e)
                    print('  [%2d] 0x%06x ninsn=%s' % (i, e, f['ninsn'] if f else '?'))
                print('  ctors:', ['0x%06x' % h[0] for h in find_ctor(v['vtable_va'])])
