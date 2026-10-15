#!/usr/bin/env python3
"""Emit the INI/config + weapon/warhead/anim/bullet type headers.

Evidence: out/vtables.txt, out/classes.json, out/functions.json, out/ini_keys.json, bin/GAME.EXE
"""
import json, struct, os, re

RE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(RE, 'out')
SRC = os.path.join(RE, 'src', 'include')
BIN = open(os.path.join(RE, 'bin', 'GAME.EXE'), 'rb').read()
IB = 0x400000


def dword(va):
    return struct.unpack('<I', BIN[va - IB:va - IB + 4])[0]


def cstr(va):
    off = va - IB
    e = BIN.index(b'\x00', off)
    return BIN[off:e].decode('latin1')


fns = {f['va']: f for f in json.load(open(os.path.join(OUT, 'functions.json')))['functions']}
classes = json.load(open(os.path.join(OUT, 'classes.json')))
ini = json.load(open(os.path.join(OUT, 'ini_keys.json')))

# ---------------------------------------------------------------- vtables
def vtable_entries(vt_va, maxn=80):
    out = []
    for i in range(maxn):
        va = dword(vt_va + 4 * i)
        if not (0x401000 <= va < 0x698000):
            break
        out.append(va)
    return out


def vt_for(cls, off=0):
    for v in classes['vtables']:
        if v['class'] == cls and v['offset'] == off:
            return v['vtable_va']
    return None


# ------------------------------------------------------- INI accessor map
# All __thiscall on INIClass (this in ECX).  Identified by body shape:
#   GetString: 3 args (section,key,def) -> returns char*; 0x4d60b0/0x4d6960/
#              0x4d70b0/0x4d54c0/0x4d50f0/0x4d6030/0x4d72b0/0x4d6290
#   GetInt:    3 args -> returns int; 0x4d6b50 (calls 0x46d710 str hash),
#              0x4d5d50
#   GetDouble: 3 args -> returns double; 0x4d6b00 (calls 0x682aae = sprintf "%f")
#   GetBool:   3 args -> returns bool; 0x4d7490, 0x4d5c60, 0x4d7070
#              0x4d7070 pushes "yes" (0x6c9c28) then calls 0x4d6b50 -> bool
INI_ACCESSORS = {
    0x4d60b0: 'GetString', 0x4d6960: 'GetString', 0x4d6ed0: 'GetString',
    0x4d70b0: 'GetString', 0x4d6b50: 'GetInt', 0x4d6b00: 'GetDouble',
    0x4d7490: 'GetBool', 0x4d6290: 'GetString', 0x4d54c0: 'GetString',
    0x4d4250: 'GetString', 0x4d44c0: 'GetString', 0x4d5c60: 'GetBool',
    0x4d5d50: 'GetInt', 0x4d50f0: 'GetString', 0x4d6030: 'GetString',
    0x4d7070: 'GetBool', 0x4d72b0: 'GetString',
    # Thin wrappers around the INIClass accessors used by the type readers:
    #   0x448160 pushes -1 then calls GetString  -> GetInt (atoi of the text)
    #   0x447F70 pushes -1.0 then calls GetString -> GetDouble (atof)
    0x448160: 'GetInt', 0x447f70: 'GetDouble',
}

# --------------------------------------------------- class -> reader funcs
# Evidence: the reader is the function whose string table is exactly the
# class's rules.ini field set and which calls the INIClass accessor family.
READERS = {
    'WeaponTypeClass': 0x656d40,
    'WarheadTypeClass': 0x656f60,
    'AnimTypeClass': 0x417fd0,
    'BulletTypeClass': 0x445ba0,
    'BuildingTypeClass': 0x43f250,
    'InfantryTypeClass': 0x4d3980,
    'UnitTypeClass': 0x63e8e0,
    'AircraftTypeClass': 0x61f590,
    'VoxelAnimTypeClass': 0x597e0e,
    'ParticleSystemTypeClass': 0x597090,
    'SuperWeaponTypeClass': 0x620a13,
    'TerrainTypeClass': 0x577090,
    'OverlayTypeClass': 0x4eb9ad,
    'SmudgeTypeClass': 0x4182cf,
    'TiberiumClass': 0x5c6e8e,
    'HouseTypeClass': 0x4c74d0,
    'TeamTypeClass': 0x60d180,
    'ScriptTypeClass': 0x60d510,
    'SideClass': 0x5e2e90,
    'MultiplayerSettingsClass': 0x5e3000,
    'SpecialClass': 0x5b1aa3,
    'ColorSchemeClass': 0x5c6ae7,
    'MovieClass': 0x5c68c0,
    'ScenarioClass': 0x52e8b0,
    'MapSeedClass': 0x52d4f0,
    'RandomMapClass': 0x52d760,
    'RulesClass': 0x5b2343,
    'RulesClass::General': 0x5b3451,
    'RulesClass::AI': 0x5b504f,
    'RulesClass::Power': 0x5b11cd,
    'RulesClass::Land': 0x5b16c6,
    'RulesClass::Audio': 0x5c7410,
    'RulesClass::Visual': 0x4eaa00,
    'RulesClass::Building': 0x43fb5d,
    'RulesClass::Infantry': 0x62027b,
    'RulesClass::Unit': 0x620a13,
    'RulesClass::Aircraft': 0x61f701,
}


def key_types_for(reader_va):
    f = fns.get(reader_va)
    if not f:
        return {}
    res = {}
    insns = f['insns']
    sva = {s['va']: s['text'] for s in f['strings']}
    # Locate every push of a key string; the accessor for a key is the first
    # INI accessor call between that push and the next key push.
    keypushes = []
    for i, ins in enumerate(insns):
        if ins['mnem'] != 'push':
            continue
        m = re.match(r'0x([0-9a-f]+)$', ins['op'])
        if m and int(m.group(1), 16) in sva:
            keypushes.append((i, sva[int(m.group(1), 16)]))
    for n, (i, key) in enumerate(keypushes):
        end = keypushes[n + 1][0] if n + 1 < len(keypushes) else len(insns)
        for j in range(i + 1, end):
            if insns[j]['mnem'] == 'call':
                cm = re.match(r'0x([0-9a-f]+)$', insns[j]['op'])
                if cm and int(cm.group(1), 16) in INI_ACCESSORS:
                    res.setdefault(key, INI_ACCESSORS[int(cm.group(1), 16)])
                    break
    return res


def cpp_type(key, acc):
    if acc == 'GetBool':
        return 'bool'
    if acc == 'GetInt':
        return 'int'
    if acc == 'GetDouble':
        return 'double'
    if acc == 'GetString':
        if key.endswith('Color') or key.endswith('Colour'):
            return 'int /* RGB */'
        if key.endswith('Sound') or key.endswith('Voice') or key.endswith('Report'):
            return 'char[64] /* sound id */'
        if key.endswith('Anim') or key.endswith('Image') or key.endswith('Name'):
            return 'char[64] /* id */'
        return 'char[64]'
    return '/* ? */'


def main():
    os.makedirs(SRC, exist_ok=True)
    mapped = {c: (r, key_types_for(r)) for c, r in READERS.items()}
    with open(os.path.join(RE, 'tools', 'ini_map.json'), 'w') as fh:
        json.dump({k: {'reader': hex(v[0]), 'keys': v[1]} for k, v in mapped.items()},
                  fh, indent=1)
    print('classes:', len(mapped), 'keys:', sum(len(v[1]) for v in mapped.values()))
    for c in ('WeaponTypeClass', 'WarheadTypeClass', 'AnimTypeClass', 'BulletTypeClass'):
        print(c, hex(mapped[c][0]), mapped[c][1])


if __name__ == '__main__':
    main()
