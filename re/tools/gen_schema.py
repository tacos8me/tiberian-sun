#!/usr/bin/env python3
"""Emit src/include/SCHEMA.md from tools/ini_map.json."""
import json, os

RE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
m = json.load(open(os.path.join(RE, 'tools', 'ini_map.json')))

# Section order: the classes this task owns first, then the rest.
OWNED = ['WeaponTypeClass', 'WarheadTypeClass', 'AnimTypeClass', 'BulletTypeClass']
RULES = ['RulesClass', 'RulesClass::General', 'RulesClass::AI', 'RulesClass::Power',
         'RulesClass::Land', 'RulesClass::Audio', 'RulesClass::Visual',
         'RulesClass::Building', 'RulesClass::Infantry', 'RulesClass::Unit',
         'RulesClass::Aircraft']
REST = [c for c in m if c not in OWNED and c not in RULES]


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


def table(cls):
    r = m[cls]
    lines = ['| INI key | owning class | inferred C++ type | evidence fn VA |',
             '|---|---|---|---|']
    for k, acc in sorted(r['keys'].items()):
        lines.append('| `%s` | %s | %s | `%s` |' % (k, cls, cpp_type(k, acc), r['reader']))
    return '\n'.join(lines)


out = []
out.append('# rules.ini key -> class schema (recovered from GAME.EXE)')
out.append('')
out.append('Every row is a literal INI key string referenced by the named reader')
out.append('function.  The reader VA is the function that pushes the key string and')
out.append('then calls an INIClass accessor; the accessor determines the inferred C++')
out.append('type (GetBool -> bool, GetInt -> int, GetDouble -> double, GetString ->')
out.append('char[N]).  Keys whose accessor could not be resolved are typed `/* ? */`.')
out.append('')
out.append('Accessor VAs (all `__thiscall` on INIClass, `this` in ECX):')
out.append('')
out.append('| VA | accessor | evidence |')
out.append('|---|---|---|')
out.append('| `0x4D60B0` | GetString | 3 args, returns char* |')
out.append('| `0x4D6960` | GetString | 3 args, returns char* |')
out.append('| `0x4D6ED0` | GetString | 4 args (buf,len), returns int |')
out.append('| `0x4D70B0` | GetString | 3 args, returns char* |')
out.append('| `0x4D54C0` | GetString | 3 args, returns char* |')
out.append('| `0x4D50F0` | GetString | 3 args, returns char* |')
out.append('| `0x4D6030` | GetString | 3 args, returns char* |')
out.append('| `0x4D72B0` | GetString | 3 args, returns char* |')
out.append('| `0x4D6290` | GetString | 3 args, returns char* |')
out.append('| `0x4D4250` | GetString | 3 args, returns StringClass |')
out.append('| `0x4D6B50` | GetInt | 3 args, hashes key via 0x46D710 |')
out.append('| `0x4D5D50` | GetInt | 3 args, index variant |')
out.append('| `0x4D6B00` | GetDouble | 3 args, sprintf "%f" via 0x682AAE |')
out.append('| `0x4D7490` | GetBool | 3 args, returns bool |')
out.append('| `0x4D5C60` | GetBool | 3 args, returns bool |')
out.append('| `0x4D7070` | GetBool | pushes "yes" (0x6C9C28) then GetInt |')
out.append('')
out.append('## Weapon / warhead / anim / bullet type classes')
out.append('')
for c in OWNED:
    out.append('### %s (reader `%s`)' % (c, m[c]['reader']))
    out.append('')
    out.append(table(c))
    out.append('')
out.append('## RulesClass sections')
out.append('')
for c in RULES:
    out.append('### %s (reader `%s`)' % (c, m[c]['reader']))
    out.append('')
    out.append(table(c))
    out.append('')
out.append('## Other type classes')
out.append('')
for c in REST:
    out.append('### %s (reader `%s`)' % (c, m[c]['reader']))
    out.append('')
    out.append(table(c))
    out.append('')

# unassigned keys
assigned = set()
for c in m:
    assigned |= set(m[c]['keys'])
allkeys = json.load(open(os.path.join(RE, 'out', 'ini_keys.json')))
unassigned = sorted(k for k in allkeys if k not in assigned)
out.append('## Unassigned keys')
out.append('')
out.append('%d of the %d referenced strings were not matched to a reader in this'
           % (len(unassigned), len(allkeys)))
out.append('pass.  They are either format strings, file extensions, section names,')
out.append('or keys read by readers not enumerated here.')
out.append('')
out.append('| key | referenced at VA(s) |')
out.append('|---|---|')
for k in unassigned:
    vas = ', '.join('`0x%X`' % v for v in allkeys[k])
    out.append('| `%s` | %s |' % (k, vas))
out.append('')

open(os.path.join(RE, 'src', 'include', 'SCHEMA.md'), 'w').write('\n'.join(out))
print('wrote SCHEMA.md; assigned=%d unassigned=%d' % (len(assigned), len(unassigned)))
