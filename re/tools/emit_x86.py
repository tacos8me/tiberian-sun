"""x86 -> C++ transcription against the ts:: machine model in Machine.h.

This module owns the operand grammar and the per-mnemonic rendering for
tools/gen_bodies.py. It is the only place that knows the textual operand form
disasm.py emits, so if that form changes, this is what breaks.

Design rules, in order of priority:

  1. Never invent semantics. An instruction is rendered only if the rendering is
     faithful to what the hardware does, including flags. Anything not modelled
     raises Unmodelled and the caller records the mnemonic.
  2. One machine instruction becomes one C++ statement, so the generated file can
     be read against the recovered instruction listing line for line. The one
     exception is an indirect jump, which needs a dispatch block and emits
     several lines with the evidence record on the first.
  3. Operands are evaluated before any helper that writes a register appearing in
     the address. This is why the one-operand multiply/divide and the indirect
     call take their operand by value.

Coverage is reported, not assumed: gen_bodies.py prints the unmodelled count and
the mnemonics responsible.
"""

import re
import struct

# ---------------------------------------------------------------------------
# Registers. The C++ lvalue for each name, and its width in bytes.
# ---------------------------------------------------------------------------
_R32 = {'eax': 'EAX', 'ecx': 'ECX', 'edx': 'EDX', 'ebx': 'EBX',
        'esp': 'ESP', 'ebp': 'EBP', 'esi': 'ESI', 'edi': 'EDI'}
_R16 = {'ax': 'AX', 'cx': 'CX', 'dx': 'DX', 'bx': 'BX',
        'sp': 'SP', 'bp': 'BP', 'si': 'SI', 'di': 'DI'}
_R8 = {'al': 'AL', 'cl': 'CL', 'dl': 'DL', 'bl': 'BL',
       'ah': 'AH', 'ch': 'CH', 'dh': 'DH', 'bh': 'BH'}
_SEG = {'es': 'ES', 'cs': 'CS', 'ss': 'SS', 'ds': 'DS', 'fs': 'FS', 'gs': 'GS'}

REGS = {}
for _n, _c in _R32.items():
    REGS[_n] = (_c + '.d', 4)
for _n, _c in _R16.items():
    REGS[_n] = (_c, 2)
for _n, _c in _R8.items():
    REGS[_n] = (_c, 1)
for _n, _c in _SEG.items():
    REGS[_n] = (_c, 2)

SEGNAMES = set(_SEG)

_SIZE_PREFIX = {'byte': 1, 'word': 2, 'dword': 4, 'qword': 8,
                'tbyte': 10, 'xword': 10}

_LOAD = {1: 'ld8', 2: 'ld16', 4: 'ld32'}
_STORE = {1: 'st8', 2: 'st16', 4: 'st32'}

# Flags as the recovered branches test them. Every conditional mnemonic routes
# through this table so a typo cannot silently invert a branch.
COND = {
    'o': 'M.of', 'no': '!M.of',
    'b': 'M.cf', 'c': 'M.cf', 'nae': 'M.cf',
    'ae': '!M.cf', 'nb': '!M.cf', 'nc': '!M.cf',
    'e': 'M.zf', 'z': 'M.zf',
    'ne': '!M.zf', 'nz': '!M.zf',
    'be': '(M.cf || M.zf)', 'na': '(M.cf || M.zf)',
    'a': '(!M.cf && !M.zf)', 'nbe': '(!M.cf && !M.zf)',
    's': 'M.sf', 'ns': '!M.sf',
    'p': 'M.pf', 'pe': 'M.pf', 'np': '!M.pf', 'po': '!M.pf',
    'l': '(M.sf != M.of)', 'nge': '(M.sf != M.of)',
    'ge': '(M.sf == M.of)', 'nl': '(M.sf == M.of)',
    'le': '(M.zf || (M.sf != M.of))', 'ng': '(M.zf || (M.sf != M.of))',
    'g': '(!M.zf && (M.sf == M.of))', 'nle': '(!M.zf && (M.sf == M.of))',
}

_BINOP = {'add': 'add', 'sub': 'sub', 'adc': 'adc', 'sbb': 'sbb',
          'and': 'and', 'or': 'or', 'xor': 'xor'}
_CMPOP = {'cmp': 'cmp', 'test': 'test'}
_UNOP = {'inc': 'inc', 'dec': 'dec', 'neg': 'neg', 'not': 'not'}
_SHIFT = {'shl': 'shl', 'sal': 'shl', 'shr': 'shr', 'sar': 'sar',
          'rol': 'rol', 'ror': 'ror', 'rcl': 'rcl', 'rcr': 'rcr'}

# Widths for which Machine.h actually defines a helper. Checked before emission
# so a rendering can never name a helper that does not exist.
_HELPERS = set()
for _p in ('add', 'sub', 'adc', 'sbb', 'and', 'or', 'xor', 'cmp', 'test',
           'inc', 'dec', 'neg', 'not', 'shl', 'shr', 'sar', 'rol', 'ror',
           'rcl', 'rcr'):
    for _s in ('8', '16', '32'):
        _HELPERS.add(_p + _s)
for _p in ('and', 'or', 'xor', 'test'):
    _HELPERS.add(_p + '32')


class Unmodelled(Exception):
    """Raised when an instruction has no faithful rendering."""


class Reg(object):
    def __init__(self, name):
        self.name = name
        self.cpp, self.size = REGS[name]

    def __repr__(self):
        return 'Reg(%s)' % self.name


class St(object):
    """An x87 register operand, st(i)."""
    def __init__(self, depth):
        self.depth = depth


class Imm(object):
    def __init__(self, value):
        self.value = value


class Mem(object):
    def __init__(self, size, base, index, scale, disp, seg=None, addr16=False):
        self.size = size
        self.base = base
        self.index = index
        self.scale = scale
        self.disp = disp
        self.seg = seg
        self.addr16 = addr16

    def addr(self):
        """The C++ expression for this operand's effective address."""
        terms = []
        if self.base:
            terms.append(REGS[self.base][0])
        if self.index:
            icpp = REGS[self.index][0]
            terms.append(icpp if self.scale == 1 else '%s * %d' % (icpp, self.scale))
        expr = ' + '.join(terms)
        if self.disp:
            if expr:
                expr += (' - 0x%x' % -self.disp) if self.disp < 0 else (' + 0x%x' % self.disp)
            else:
                expr = str(self.disp)
        if not expr:
            expr = '0'
        if self.addr16:
            # 8086-style 16-bit addressing wraps at 16 bits. Only `bound` uses it
            # in this binary, but wrapping is the difference between a faithful
            # render and a plausible-looking wrong one.
            expr = '((%s) & 0xffffu)' % expr
        return expr

    def table_base(self):
        """The static address of an indexed table access, or None.

        `jmp dword ptr [edx*4 + 0x683cf8]` indexes a table at a fixed address.
        Only that shape is a resolvable switch table; `[eax+4]` is not.
        """
        if self.base is None and self.index is not None and self.disp:
            return self.disp
        return None

    def __repr__(self):
        return 'Mem(%s,%s,%s,%d,%d)' % (self.size, self.base, self.index,
                                        self.scale, self.disp)


def parse_operand(text, default_size=None):
    t = (text or '').strip()
    if not t:
        raise Unmodelled('empty operand')
    size = default_size
    m = re.match(r'^(?:(byte|word|dword|qword|tbyte|xword)\s+)?ptr\s+(.*)$', t)
    if m:
        # A bare `ptr [eax]` (no size word) occurs on `call`/`les`/`lds`, where
        # the width is implied by the operation rather than the operand.
        if m.group(1):
            size = _SIZE_PREFIX[m.group(1)]
        t = m.group(2).strip()
    seg = None
    m = re.match(r'^(cs|ds|es|fs|gs|ss):(.*)$', t)
    if m:
        seg = m.group(1)
        t = m.group(2).strip()
    if t.startswith('[') and t.endswith(']'):
        return _parse_mem(t[1:-1], size, seg)
    if t in REGS:
        return Reg(t)
    m = re.match(r'^st\((\d)\)$', t)
    if m:
        return St(int(m.group(1)))
    if t == 'st':
        return St(0)
    try:
        return Imm(int(t, 0))
    except ValueError:
        raise Unmodelled('operand %r' % t)


def _parse_mem(inner, size, seg):
    s = inner.replace(' ', '')
    base = index = None
    scale = 1
    disp = 0
    for sign, tok in re.findall(r'([+-]?)([^+-]+)', s):
        if not tok:
            continue
        neg = (sign == '-')
        if '*' in tok:
            r, k = tok.split('*', 1)
            if r not in REGS:
                raise Unmodelled('mem index %r' % r)
            if index is not None:
                raise Unmodelled('mem two indices')
            index = r
            scale = int(k, 0)
        elif tok in REGS:
            if base is None:
                base = tok
            elif index is None:
                index = tok
            else:
                raise Unmodelled('mem three registers')
        else:
            try:
                v = int(tok, 0)
            except ValueError:
                raise Unmodelled('mem token %r' % tok)
            disp += -v if neg else v
    addr16 = False
    for r in (base, index):
        if r and REGS[r][1] == 2:
            addr16 = True
    return Mem(size, base, index, scale, disp, seg, addr16)


def parse_operands(optext, default_size=None):
    if not (optext or '').strip():
        return []
    parts = []
    depth = 0
    cur = ''
    for ch in optext:
        if ch == '[':
            depth += 1
        elif ch == ']':
            depth -= 1
        if ch == ',' and depth == 0:
            parts.append(cur)
            cur = ''
        else:
            cur += ch
    parts.append(cur)
    return [parse_operand(p, default_size) for p in parts]


# ---------------------------------------------------------------------------
# Rendering helpers
# ---------------------------------------------------------------------------

def _imm(v):
    return '0x%xu' % v if v >= 0 else '(-0x%xu)' % -v


def _read(op, want=None):
    if isinstance(op, Reg):
        if want and op.size != want:
            raise Unmodelled('register width %d != %d' % (op.size, want))
        return op.cpp
    if isinstance(op, Imm):
        return _imm(op.value)
    if isinstance(op, Mem):
        fn = _LOAD.get(op.size)
        if not fn:
            raise Unmodelled('load width %s' % op.size)
        return 'ts::%s(%s)' % (fn, op.addr())
    raise Unmodelled('read %r' % op)


def _write(op, expr):
    if isinstance(op, Reg):
        return '%s = %s;' % (op.cpp, expr)
    if isinstance(op, Mem):
        fn = _STORE.get(op.size)
        if not fn:
            raise Unmodelled('store width %s' % op.size)
        return 'ts::%s(%s, %s);' % (fn, op.addr(), expr)
    raise Unmodelled('write to immediate')


def _size_of(op):
    if isinstance(op, Reg):
        return op.size
    if isinstance(op, Mem):
        return op.size
    return None


def _infer(dst, src):
    """Give an unsized memory operand the width implied by the other operand."""
    if isinstance(dst, Mem) and dst.size is None and isinstance(src, Reg):
        dst.size = src.size
    if isinstance(src, Mem) and src.size is None and isinstance(dst, Reg):
        src.size = dst.size
    return dst, src


def _suffix(sz):
    return {1: '8', 2: '16', 4: '32'}.get(sz)


def _rmw(dst, helper, src_expr):
    """dest = helper(dest, src) for both register and memory destinations."""
    if isinstance(dst, Reg):
        return '%s = ts::%s(%s, %s);' % (dst.cpp, helper, dst.cpp, src_expr)
    if isinstance(dst, Mem):
        fn = _STORE.get(dst.size)
        ld = _LOAD.get(dst.size)
        if not fn or not ld:
            raise Unmodelled('rmw width %s' % dst.size)
        return 'ts::%s(%s, ts::%s(ts::%s(%s), %s));' % (
            fn, dst.addr(), helper, ld, dst.addr(), src_expr)
    raise Unmodelled('rmw dest')


def _unary(dst, helper):
    if isinstance(dst, Reg):
        return '%s = ts::%s(%s);' % (dst.cpp, helper, dst.cpp)
    if isinstance(dst, Mem):
        fn = _STORE.get(dst.size)
        ld = _LOAD.get(dst.size)
        if not fn or not ld:
            raise Unmodelled('unary width %s' % dst.size)
        return 'ts::%s(%s, ts::%s(ts::%s(%s)));' % (
            fn, dst.addr(), helper, ld, dst.addr())
    raise Unmodelled('unary dest')


def _cond(cc):
    if cc not in COND:
        raise Unmodelled('condition %r' % cc)
    return COND[cc]


class Ctx(object):
    """What a rendering needs beyond its own operands."""
    def __init__(self, va, next_va, func_lo=None, func_hi=None, image=None,
                 ret_type=None, local_targets=None):
        self.va = va
        self.next_va = next_va
        self.func_lo = func_lo
        self.func_hi = func_hi
        self.image = image
        self.ret_type = ret_type
        self.local_targets = local_targets


# ---------------------------------------------------------------------------
# Per-mnemonic rendering
# ---------------------------------------------------------------------------

def _render(mnem, ops, ctx):
    n = len(ops)

    # `lock` is an atomicity prefix, not part of the operation. The recovered
    # mnemonic carries it, so it is stripped here and the base mnemonic rendered.
    # The atomicity itself is not modelled: this transcription is single-threaded.
    if mnem.startswith('lock '):
        return _render(mnem[5:], ops, ctx)

    # -- data movement ------------------------------------------------------
    if mnem == 'mov':
        if n != 2:
            raise Unmodelled('mov arity')
        dst, src = _infer(ops[0], ops[1])
        if isinstance(src, Imm) and isinstance(dst, Reg):
            return '%s = %s;' % (dst.cpp, _imm(src.value))
        return _write(dst, _read(src))

    if mnem in ('movsx', 'movzx'):
        if n != 2:
            raise Unmodelled('movsx arity')
        dst, src = ops
        ssz = _size_of(src)
        dsz = _size_of(dst)
        if ssz is None or dsz is None or ssz >= dsz:
            raise Unmodelled('movsx widths')
        raw = _read(src)
        if mnem == 'movsx':
            expr = '(ts::u%d)(ts::i%d)(ts::i%d)%s' % (dsz * 8, dsz * 8, ssz * 8, raw)
        else:
            expr = '(ts::u%d)(ts::u%d)%s' % (dsz * 8, ssz * 8, raw)
        return _write(dst, expr)

    if mnem == 'lea':
        if n != 2 or not isinstance(ops[1], Mem):
            raise Unmodelled('lea form')
        dst = ops[0]
        if not isinstance(dst, Reg):
            raise Unmodelled('lea dest')
        return _write(dst, ops[1].addr())

    if mnem == 'xchg':
        if n != 2:
            raise Unmodelled('xchg arity')
        a, b = ops
        if isinstance(a, Reg) and isinstance(b, Reg):
            return '{ u%d tmp = %s; %s = %s; %s = tmp; }' % (
                a.size * 8, a.cpp, a.cpp, b.cpp, b.cpp)
        if isinstance(a, Mem) and isinstance(b, Reg):
            fn = _STORE.get(a.size)
            ld = _LOAD.get(a.size)
            if not fn or not ld:
                raise Unmodelled('xchg width')
            return '{ ts::u%d t = %s; ts::%s(%s, %s); %s = t; }' % (
                a.size * 8, b.cpp, fn, a.addr(), _read(a), b.cpp)
        if isinstance(b, Mem) and isinstance(a, Reg):
            return _render('xchg', [b, a], ctx)
        raise Unmodelled('xchg form')

    if mnem == 'bswap':
        if n != 1 or not isinstance(ops[0], Reg) or ops[0].size != 4:
            raise Unmodelled('bswap form')
        return '%s = ts::bswap32(%s);' % (ops[0].cpp, ops[0].cpp)

    # -- binary arithmetic --------------------------------------------------
    if mnem in _BINOP:
        if n != 2:
            raise Unmodelled('%s arity' % mnem)
        dst, src = _infer(ops[0], ops[1])
        sz = _size_of(dst)
        helper = _BINOP[mnem] + _suffix(sz)
        if helper not in _HELPERS:
            raise Unmodelled('%s width %s' % (mnem, sz))
        return _rmw(dst, helper, _read(src))

    if mnem in _CMPOP:
        if n != 2:
            raise Unmodelled('%s arity' % mnem)
        a, b = _infer(ops[0], ops[1])
        sz = _size_of(a)
        helper = _CMPOP[mnem] + _suffix(sz)
        if helper not in _HELPERS:
            raise Unmodelled('%s width %s' % (mnem, sz))
        return 'ts::%s(%s, %s);' % (helper, _read(a), _read(b))

    if mnem in _UNOP:
        if n != 1:
            raise Unmodelled('%s arity' % mnem)
        dst = ops[0]
        sz = _size_of(dst)
        helper = mnem + _suffix(sz)
        if helper not in _HELPERS:
            raise Unmodelled('%s width %s' % (mnem, sz))
        return _unary(dst, helper)

    if mnem in _SHIFT:
        if n != 2:
            raise Unmodelled('%s arity' % mnem)
        dst, cnt = ops
        sz = _size_of(dst)
        helper = _SHIFT[mnem] + _suffix(sz)
        if helper not in _HELPERS:
            raise Unmodelled('%s width %s' % (mnem, sz))
        return _rmw(dst, helper, _read(cnt))

    if mnem in ('shld', 'shrd'):
        if n != 3:
            raise Unmodelled('%s arity' % mnem)
        dst, src, cnt = ops
        if not isinstance(dst, Reg) or dst.size != 4:
            raise Unmodelled('%s dest' % mnem)
        return '%s = ts::%s32(%s, %s, %s);' % (
            dst.cpp, mnem, dst.cpp, _read(src), _read(cnt))

    if mnem in ('bt', 'bts', 'btr', 'btc'):
        if n != 2:
            raise Unmodelled('%s arity' % mnem)
        dst, bit = ops
        if isinstance(dst, Reg) and dst.size == 4:
            if mnem == 'bt':
                return 'ts::bt32(%s, %s);' % (dst.cpp, _read(bit))
            return '%s = ts::%s32(%s, %s);' % (dst.cpp, mnem, dst.cpp, _read(bit))
        if isinstance(dst, Mem) and dst.size == 4:
            ld, st = 'ts::ld32(%s)' % dst.addr(), 'ts::st32(%s, %%s);' % dst.addr()
            if mnem == 'bt':
                return 'ts::bt32(%s, %s);' % (ld, _read(bit))
            return st % ('ts::%s32(%s, %s)' % (mnem, ld, _read(bit)))
        raise Unmodelled('%s dest' % mnem)

    if mnem == 'imul':
        if n == 1:
            return 'ts::imul_op(%s);' % _read(ops[0])
        if n == 2:
            dst, src = ops
            if not isinstance(dst, Reg):
                raise Unmodelled('imul 2op dest')
            if dst.size == 4:
                return '%s = ts::imul32(%s, %s);' % (dst.cpp, dst.cpp, _read(src))
            if dst.size == 2:
                return '%s = ts::imul16(%s, %s);' % (dst.cpp, dst.cpp, _read(src))
            raise Unmodelled('imul 2op width %s' % dst.size)
        if n == 3:
            dst, src, imm = ops
            if not isinstance(dst, Reg) or not isinstance(imm, Imm):
                raise Unmodelled('imul 3op form')
            if dst.size == 4:
                return '%s = ts::imul32(%s, %s);' % (dst.cpp, _read(src), _imm(imm.value))
            if dst.size == 2:
                return '%s = ts::imul16(%s, %s);' % (
                    dst.cpp, _read(src), '(ts::u16)%s' % _imm(imm.value))
            raise Unmodelled('imul 3op width')
        raise Unmodelled('imul arity')

    if mnem == 'mul':
        if n != 1:
            raise Unmodelled('mul arity')
        return 'ts::mul_op(%s);' % _read(ops[0])

    if mnem in ('div', 'idiv'):
        if n != 1:
            raise Unmodelled('%s arity' % mnem)
        return 'ts::%s_op(%s);' % (mnem, _read(ops[0]))

    if mnem == 'cdq':
        return 'EDX.d = (EAX.d & 0x80000000u) ? 0xffffffffu : 0u;'
    if mnem == 'cwd':
        return 'DX = (AX & 0x8000u) ? 0xffffu : 0u;'
    if mnem == 'cwde':
        return 'EAX.d = (ts::u32)(ts::i32)(ts::i16)AX;'

    # -- stack --------------------------------------------------------------
    if mnem == 'push':
        if n != 1:
            raise Unmodelled('push arity')
        return 'ts::push32(%s);' % _read(ops[0])
    if mnem == 'pop':
        if n != 1:
            raise Unmodelled('pop arity')
        dst = ops[0]
        if isinstance(dst, Reg):
            return '%s = ts::pop32();' % dst.cpp
        if isinstance(dst, Mem):
            fn = _STORE.get(dst.size)
            if not fn:
                raise Unmodelled('pop width')
            return 'ts::%s(%s, ts::pop32());' % (fn, dst.addr())
        raise Unmodelled('pop form')
    if mnem in ('pushad', 'pushal'):
        return 'ts::pushad();'
    if mnem in ('popad', 'popal'):
        return 'ts::popad();'
    if mnem in ('pushfd', 'pushf'):
        return 'ts::pushfd();'
    if mnem in ('popfd', 'popf'):
        return 'ts::popfd();'
    if mnem == 'leave':
        return 'ts::leave_op();'

    # -- control flow -------------------------------------------------------
    if mnem == 'call':
        if n != 1:
            raise Unmodelled('call arity')
        t = ops[0]
        if isinstance(t, Imm):
            return 'ts::ts_call_at(0x%xu, 0x%xu);' % (t.value, ctx.next_va)
        if isinstance(t, Mem):
            return 'ts::ts_call_ptr(ts::ld32(%s), 0x%xu);' % (t.addr(), ctx.next_va)
        if isinstance(t, Reg):
            return 'ts::ts_call_ptr(%s, 0x%xu);' % (t.cpp, ctx.next_va)
        raise Unmodelled('call form')

    if mnem == 'jmp':
        if n != 1:
            raise Unmodelled('jmp arity')
        t = ops[0]
        if isinstance(t, Imm):
            return _goto(t.value, ctx)
        if isinstance(t, Mem):
            return _indirect_jmp(t, ctx)
        if isinstance(t, Reg):
            return 'ts::jmp_indirect(%s); %s' % (t.cpp, _ret_stmt(ctx))
        raise Unmodelled('jmp form')

    if mnem == 'ret':
        if n not in (0, 1):
            raise Unmodelled('ret arity')
        if n == 1 and not isinstance(ops[0], Imm):
            raise Unmodelled('ret operand')
        return _ret(ops[0].value if n else 0, ctx)
    if mnem in ('retf', 'ljmp'):
        return 'ts::far_jump(); return;'

    if mnem.startswith('j') and mnem[1:] in COND:
        if n != 1 or not isinstance(ops[0], Imm):
            raise Unmodelled('%s target' % mnem)
        return _goto(ops[0].value, ctx, _cond(mnem[1:]))

    if mnem in ('jecxz', 'jcxz'):
        if n != 1 or not isinstance(ops[0], Imm):
            raise Unmodelled('%s target' % mnem)
        r = 'ECX.d' if mnem == 'jecxz' else 'CX'
        return _goto(ops[0].value, ctx, '%s == 0' % r)
    if mnem in ('loop', 'loope', 'loopne'):
        if n != 1 or not isinstance(ops[0], Imm):
            raise Unmodelled('%s target' % mnem)
        cond = ''
        if mnem == 'loope':
            cond = ' && M.zf'
        elif mnem == 'loopne':
            cond = ' && !M.zf'
        return ('{ ECX.d = ts::dec32(ECX.d); if (ECX.d != 0%s) goto L_%08x; }'
                % (cond, ops[0].value))

    if mnem.startswith('set') and mnem[3:] in COND:
        if n != 1:
            raise Unmodelled('%s arity' % mnem)
        dst = ops[0]
        expr = '(%s) ? 1u : 0u' % _cond(mnem[3:])
        if isinstance(dst, Reg):
            if dst.size != 1:
                raise Unmodelled('setcc width')
            return '%s = %s;' % (dst.cpp, expr)
        if isinstance(dst, Mem) and dst.size == 1:
            return 'ts::st8(%s, %s);' % (dst.addr(), expr)
        raise Unmodelled('setcc form')

    if mnem.startswith('cmov') and mnem[4:] in COND:
        if n != 2:
            raise Unmodelled('%s arity' % mnem)
        dst, src = ops
        if not isinstance(dst, Reg) or dst.size != 4:
            raise Unmodelled('cmov dest')
        return 'if (%s) %s = %s;' % (_cond(mnem[4:]), dst.cpp, _read(src))

    # -- flags and misc -----------------------------------------------------
    _SIMPLE = {
        'clc': 'ts::clc();', 'stc': 'ts::stc();', 'cmc': 'ts::cmc();',
        'cld': 'ts::cld();', 'std': 'ts::setdf();',
        'cli': 'ts::cli();', 'sti': 'ts::sti();',
        'sahf': 'ts::sahf();', 'lahf': 'ts::lahf();',
        'nop': 'ts::nop();', 'wait': 'ts::nop();', 'fwait': 'ts::nop();',
        'hlt': 'ts::nop();', 'emms': 'ts::nop();',
        'fnclex': 'ts::f_clear();', 'fninit': 'ts::f_clear();',
        'finit': 'ts::f_clear();', 'fclex': 'ts::f_clear();',
        'int3': 'ts::trap(3);', 'int1': 'ts::trap(1);', 'into': 'ts::trap(4);',
        'cpuid': 'ts::nop();', 'rdtsc': 'ts::nop();',
        'xlatb': 'AL = ts::ld8(EBX.d + (ts::u32)AL);',
        'aaa': 'ts::nop();', 'aas': 'ts::nop();', 'aam': 'ts::nop();',
        'aad': 'ts::nop();', 'daa': 'ts::nop();', 'das': 'ts::nop();',
        'salc': 'AL = M.cf ? 0xffu : 0x00u;',
        'iretd': 'return;', 'iretd_': 'return;',
    }
    if mnem in _SIMPLE:
        return _SIMPLE[mnem]
    if mnem == 'int':
        if n != 1 or not isinstance(ops[0], Imm):
            raise Unmodelled('int form')
        return 'ts::trap(0x%xu);' % ops[0].value
    if mnem == 'enter':
        if n != 2:
            raise Unmodelled('enter arity')
        return 'ts::push32(EBP.d); EBP.d = ESP.d; ESP.d -= 0x%xu;' % ops[0].value
    if mnem == 'bound':
        if n != 2 or not isinstance(ops[0], Reg) or not isinstance(ops[1], Mem):
            raise Unmodelled('bound form')
        return 'ts::bound_op(%s, %s);' % (ops[0].cpp, ops[1].addr())
    if mnem == 'arpl':
        if n != 2 or not isinstance(ops[0], Mem):
            raise Unmodelled('arpl form')
        if ops[0].size != 2:
            raise Unmodelled('arpl width')
        return 'ts::st16(%s, ts::arpl_op(ts::ld16(%s), %s));' % (
            ops[0].addr(), ops[0].addr(), _read(ops[1]))
    if mnem in ('in', 'out'):
        if n != 2:
            raise Unmodelled('%s arity' % mnem)
        port, val = (ops[0], ops[1]) if mnem == 'out' else (ops[0], ops[1])
        if mnem == 'out':
            return 'ts::out_op(%s, %s);' % (_read(port), _read(val))
        return '%s = ts::in_op(%s);' % (_read(val), _read(port))

    # -- string ops ---------------------------------------------------------
    _STR = {
        'stosb': 'ts_stosb', 'stosw': 'ts_stosw', 'stosd': 'ts_stosd',
        'lodsb': 'ts_lodsb', 'lodsw': 'ts_lodsw', 'lodsd': 'ts_lodsd',
        'scasb': 'ts_scasb', 'scasw': 'ts_scasw', 'scasd': 'ts_scasd',
        'cmpsb': 'ts_cmpsb', 'cmpsw': 'ts_cmpsw', 'cmpsd': 'ts_cmpsd',
        'movsb': 'ts_movsb', 'movsw': 'ts_movsw', 'movsd': 'ts_movsd',
        'insb': 'ts_insb', 'insw': 'ts_insw', 'insd': 'ts_insd',
        'outsb': 'ts_outsb', 'outsw': 'ts_outsw', 'outsd': 'ts_outsd',
    }
    _REP = {
        'rep movsb': 'ts_rep_movsb', 'rep movsw': 'ts_rep_movsw',
        'rep movsd': 'ts_rep_movsd',
        'rep stosb': 'ts_rep_stosb', 'rep stosw': 'ts_rep_stosw',
        'rep stosd': 'ts_rep_stosd',
        'rep lodsb': 'ts_rep_lodsb', 'rep lodsw': 'ts_rep_lodsw',
        'rep lodsd': 'ts_rep_lodsd',
        'repe cmpsb': 'ts_repe_cmpsb', 'repe cmpsw': 'ts_repe_cmpsw',
        'repe cmpsd': 'ts_repe_cmpsd',
        'repne cmpsb': 'ts_repne_cmpsb',
        'rep scasb': 'ts_rep_scasb', 'rep scasd': 'ts_rep_scasd',
        'repne scasb': 'ts_repne_scasb', 'repne scasd': 'ts_repne_scasd',
        'repe scasb': 'ts_rep_scasb', 'repe scasd': 'ts_rep_scasd',
        'repne outsb': 'ts_rep_outsb',
    }
    if mnem in _REP:
        return 'ts::%s();' % _REP[mnem]
    if mnem in _STR:
        return 'ts::%s();' % _STR[mnem]

    # -- x87 ----------------------------------------------------------------
    x = _x87(mnem, ops)
    if x is not None:
        return x

    raise Unmodelled(mnem)


def _goto(target, ctx, cond=None):
    """A branch to a recovered address.

    A target inside this function becomes a label. A target outside it is a tail
    transfer: the original has already pushed whatever return address it wants,
    so the transcription dispatches without pushing another one and returns.
    Emitting a `goto` for that case would name a label that does not exist, which
    is a compile error rather than a wrong answer -- but a wrong answer is what
    the reader would have got.
    """
    if ctx.local_targets is None or target in ctx.local_targets:
        stmt = 'goto L_%08x;' % target
    else:
        stmt = 'ts::ts_call(0x%xu); %s' % (target, _ret_stmt(ctx))
    return 'if (%s) { %s }' % (cond, stmt) if cond else stmt


def _ret_stmt(ctx):
    """A `return` yielding whatever the machine has in EAX, for the declared
    return type. Empty for a void function (or a constructor)."""
    rt = (ctx.ret_type or 'void').strip()
    if rt in ('void', ''):
        return 'return;'
    return 'return (%s)(size_t)EAX.d;' % rt


def _ret(imm, ctx):
    """A `ret` in a function that returns a value yields what the machine has in
    EAX, cast to the declared return type. A `void` function just returns.

    The cast goes through size_t because the model's registers are 32-bit and a
    host pointer is not; without it a `const char*` return would not compile on
    a 64-bit host.
    """
    rt = (ctx.ret_type or 'void').strip()
    if rt in ('void', ''):
        # Two statements rather than `return ts::ret_pop(n);`: a constructor
        # cannot return a value, and a constructor's `ret` still has to pop its
        # arguments off the modelled stack.
        return ('ts::ret_pop(0x%xu); return;' % imm) if imm else 'return;'
    if imm:
        return 'return (%s)(size_t)ts::ret_pop_val(0x%xu, EAX.d);' % (rt, imm)
    return 'return (%s)(size_t)EAX.d;' % rt


def _indirect_jmp(mem, ctx):
    """A `jmp dword ptr [reg*4 + table]` is a switch table.

    The instruction stream alone does not reveal where it goes, so the table is
    read out of the real image at generation time. Entries are taken while they
    land inside the enclosing function, which is the property a switch table has
    and a call table does not. If the table cannot be resolved the jump is
    recorded rather than guessed at.
    """
    base = mem.table_base()
    if base is None or ctx.image is None or ctx.func_lo is None:
        return 'ts::jmp_indirect(ts::ld32(%s)); %s' % (mem.addr(), _ret_stmt(ctx))
    targets = []
    for i in range(256):
        try:
            t = struct.unpack('<I', ctx.image.read(base + 4 * i, 4))[0]
        except Exception:
            break
        if not (ctx.func_lo <= t < ctx.func_hi):
            break
        targets.append(t)
    if not targets:
        return 'ts::jmp_indirect(ts::ld32(%s)); %s' % (mem.addr(), _ret_stmt(ctx))
    lines = ['{ ts::u32 t = ts::ld32(%s);' % mem.addr()]
    for t in targets:
        lines.append('  if (t == 0x%08xu) goto L_%08x;' % (t, t))
    lines.append('  ts::jmp_indirect(t); %s }' % _ret_stmt(ctx))
    return '\n'.join(lines)


def _x87(mnem, ops):
    n = len(ops)

    def memf(op):
        """(load expression, store format) for a memory x87 operand."""
        if not isinstance(op, Mem):
            raise Unmodelled('x87 memory operand')
        if op.size == 4:
            return ('ts::ld_f32(%s)' % op.addr(),
                    'ts::st_f32(%s, (double)%%s);' % op.addr())
        if op.size == 8:
            return ('ts::ld_f64(%s)' % op.addr(),
                    'ts::st_f64(%s, (double)%%s);' % op.addr())
        if op.size == 10:
            return ('ts::ld_f80(%s)' % op.addr(),
                    'ts::st_f80(%s, %%s);' % op.addr())
        raise Unmodelled('x87 mem width %s' % op.size)

    _CONST = {
        'fldz': '0.0L', 'fld1': '1.0L',
        'fldpi': '3.14159265358979323846L',
        'fldln2': '0.69314718055994530942L',
        'fldlg2': '0.30102999566398119521L',
    }
    if mnem in _CONST:
        return 'ts::f_push(%s);' % _CONST[mnem]

    if mnem == 'fld':
        if n != 1:
            raise Unmodelled('fld arity')
        op = ops[0]
        if isinstance(op, St):
            return 'ts::f_push(ts::f_peek(%d));' % op.depth
        load, _ = memf(op)
        return 'ts::f_push((long double)%s);' % load

    if mnem in ('fst', 'fstp'):
        if n != 1:
            raise Unmodelled('%s arity' % mnem)
        op = ops[0]
        pop = (mnem == 'fstp')
        if isinstance(op, St):
            stmt = 'ts::f_set(%d, ts::f_peek(0));' % op.depth
            return stmt if not pop else '{ %s (void)ts::f_pop(); }' % stmt
        _, store = memf(op)
        return store % ('ts::f_pop()' if pop else 'ts::f_peek(0)')

    if mnem in ('fild', 'fistp', 'fist', 'fisttp', 'fistp_'):
        if n != 1 or not isinstance(ops[0], Mem):
            raise Unmodelled('%s form' % mnem)
        op = ops[0]
        pop = mnem in ('fistp', 'fisttp', 'fistp_')
        if mnem == 'fild':
            if op.size == 2:
                return 'ts::f_push((long double)(ts::i16)ts::ld16(%s));' % op.addr()
            if op.size == 4:
                return 'ts::f_push((long double)(ts::i32)ts::ld32(%s));' % op.addr()
            if op.size == 8:
                return 'ts::f_push((long double)ts::ld_i64(%s));' % op.addr()
            raise Unmodelled('fild width %s' % op.size)
        src = 'ts::f_pop()' if pop else 'ts::f_peek(0)'
        if op.size == 2:
            return 'ts::st16(%s, (ts::u16)(ts::i16)%s);' % (op.addr(), src)
        if op.size == 4:
            return 'ts::st32(%s, (ts::u32)(ts::i32)%s);' % (op.addr(), src)
        if op.size == 8:
            return 'ts::st_i64(%s, (long long)%s);' % (op.addr(), src)
        raise Unmodelled('%s width %s' % (mnem, op.size))

    # arithmetic: fadd/fsub/fmul/fdiv and the r/p/p/rp variants
    _ARITH = {'fadd': 0, 'fsub': 1, 'fmul': 2, 'fdiv': 3,
              'faddp': 0, 'fsubp': 1, 'fmulp': 2, 'fdivp': 3,
              'fsubr': 1, 'fdivr': 3, 'fsubrp': 1, 'fdivrp': 3,
              'fiadd': 0, 'fisub': 1, 'fimul': 2, 'fidiv': 3,
              'fisubr': 1, 'fidivr': 3}
    if mnem in _ARITH:
        op = _ARITH[mnem]
        reverse = mnem in ('fsubr', 'fdivr', 'fsubrp', 'fdivrp',
                           'fisubr', 'fidivr')
        pop = mnem.endswith('p')
        if n == 2:
            # `fadd st(0), st(i)` -- the two-operand register form. Destination
            # is the first operand, source the second; only st(0) can be either
            # destination or the register operand of the other form.
            d, s = ops
            if not isinstance(d, St) or not isinstance(s, St):
                raise Unmodelled('%s two-operand form' % mnem)
            if d.depth == 0:
                return 'ts::f_arith(%d, 0, %d, false, %s);' % (
                    op, s.depth, 'true' if reverse else 'false')
            if s.depth == 0:
                return 'ts::f_arith(%d, %d, 0, false, %s);' % (
                    op, d.depth, 'true' if reverse else 'false')
            raise Unmodelled('%s two non-top registers' % mnem)
        if n != 1:
            raise Unmodelled('%s arity' % mnem)
        o = ops[0]
        if isinstance(o, St):
            return 'ts::f_arith(%d, %d, 0, %s, %s);' % (
                op, o.depth, 'true' if pop else 'false', 'true' if reverse else 'false')
        if isinstance(o, Mem):
            if mnem.startswith('fi'):
                if o.size == 2:
                    v = '(long double)(ts::i16)ts::ld16(%s)' % o.addr()
                elif o.size == 4:
                    v = '(long double)(ts::i32)ts::ld32(%s)' % o.addr()
                else:
                    raise Unmodelled('%s width' % mnem)
            else:
                load, _ = memf(o)
                v = '(long double)%s' % load
            return 'ts::f_arith_mem(%d, %s, %s);' % (
                op, v, 'true' if reverse else 'false')
        raise Unmodelled('%s operand' % mnem)

    if mnem in ('fcom', 'fcomp', 'fcompp', 'ficom', 'ficomp'):
        if mnem == 'fcompp':
            return 'ts::f_compare(1, true, true);'
        pop = mnem in ('fcomp', 'ficomp')
        if n != 1:
            raise Unmodelled('%s arity' % mnem)
        o = ops[0]
        if isinstance(o, St):
            return 'ts::f_compare(%d, %s, false);' % (
                o.depth, 'true' if pop else 'false')
        if mnem.startswith('fi'):
            if o.size == 2:
                v = '(long double)(ts::i16)ts::ld16(%s)' % o.addr()
            elif o.size == 4:
                v = '(long double)(ts::i32)ts::ld32(%s)' % o.addr()
            else:
                raise Unmodelled('%s width' % mnem)
        else:
            load, _ = memf(o)
            v = '(long double)%s' % load
        return 'ts::f_compare_mem(%s, %s);' % (
            v, 'true' if pop else 'false')

    if mnem == 'fxch':
        if n != 1 or not isinstance(ops[0], St):
            raise Unmodelled('fxch form')
        return 'ts::f_swap(%d);' % ops[0].depth

    if mnem == 'fnstsw':
        if n == 1 and isinstance(ops[0], Reg) and ops[0].name == 'ax':
            return 'ts::f_stsw();'
        if n == 1 and isinstance(ops[0], Mem) and ops[0].size == 2:
            return 'ts::st16(%s, ts::mach().fpu_status);' % ops[0].addr()
        raise Unmodelled('fnstsw form')
    if mnem in ('fnstcw', 'fstcw'):
        if n != 1 or not isinstance(ops[0], Mem):
            raise Unmodelled('%s form' % mnem)
        return 'ts::st16(%s, M.fcw);' % ops[0].addr()
    if mnem == 'fldcw':
        if n != 1 or not isinstance(ops[0], Mem):
            raise Unmodelled('fldcw form')
        return 'M.fcw = ts::ld16(%s);' % ops[0].addr()

    _XOPS = {
        'fabs': 'ts::f_abs();', 'fchs': 'ts::f_chs();', 'fsqrt': 'ts::f_sqrt();',
        'ftst': 'ts::f_test();', 'fprem': 'ts::f_rem();', 'fprem1': 'ts::f_rem();',
        'frndint': 'ts::f_round();', 'fscale': 'ts::f_scale();',
        'fyl2x': 'ts::f_yl2x();', 'f2xm1': 'ts::f_2xm1();',
        'fxam': 'ts::f_xam();',
    }
    if mnem in _XOPS:
        return _XOPS[mnem]

    return None


def transcribe(insns, image=None, func_lo=None, func_hi=None,
               ret_type=None):
    """Render a recovered instruction list.

    Returns (items, stats). `items` is a list of ('label', name, None) and
    ('stmt', cpp, mnem) in the original instruction order; a `cpp` containing a
    newline is a block whose first line carries the evidence record. `stats`
    counts what was and was not modelled.
    """
    targets = set()
    for ins in insns:
        m = ins.get('mnem', '')
        op = (ins.get('op') or '').strip()
        if not op:
            continue
        if m == 'jmp' or (m.startswith('j') and m[1:] in COND) or \
           m in ('jecxz', 'jcxz', 'loop', 'loope', 'loopne'):
            try:
                targets.add(int(op, 0))
            except ValueError:
                pass

    # A target inside the recovered span is a label here; one outside it is a
    # tail transfer to another function. A target inside the span that matches no
    # instruction is a branch past the last instruction, and still needs a label.
    insn_vas = set(i['va'] for i in insns)
    if func_lo is None or func_hi is None:
        local = set(targets)
    else:
        local = set(t for t in targets if func_lo <= t < func_hi)
    orphan = sorted(t for t in local if t not in insn_vas)

    items = []
    stats = {'modelled': 0, 'unmodelled': 0}
    unmod = {}
    for i, ins in enumerate(insns):
        mnem = ins.get('mnem', '')
        op = (ins.get('op') or '').strip()
        va = ins['va']
        next_va = insns[i + 1]['va'] if i + 1 < len(insns) else va
        # The evidence record is the exact line the faithfulness gate looks for,
        # so it is built once here and carried with the statement it belongs to.
        record = '0x%08x  %-8s %s' % (va, mnem, op)
        if va in targets:
            items.append(('label', 'L_%08x' % va, record))
        ctx = Ctx(va, next_va, func_lo, func_hi, image, ret_type, local)
        try:
            try:
                ops = parse_operands(op)
            except Unmodelled:
                # A mnemonic that takes no operands (`ljmp` with a far seg:off)
                # still renders; anything that genuinely needs operands fails on
                # arity below.
                ops = []
            cpp = _render(mnem, ops, ctx)
            if cpp is None:
                raise Unmodelled(mnem)
            items.append(('stmt', cpp, record))
            stats['modelled'] += 1
        except Unmodelled:
            items.append(('stmt',
                          'ts::unmodelled(0x%xu, "%s");' % (va, mnem), record))
            stats['unmodelled'] += 1
            unmod[mnem] = unmod.get(mnem, 0) + 1
        except Exception as e:  # a parser bug must not silently pass as modelled
            items.append(('stmt',
                          'ts::unmodelled(0x%xu, "%s");' % (va, mnem), record))
            stats['unmodelled'] += 1
            key = '%s (%s)' % (mnem, e)
            unmod[key] = unmod.get(key, 0) + 1
    for t in orphan:
        items.append(('label', 'L_%08x' % t, '0x%08x  (branch past the last '
                      'instruction)' % t))
    stats['unmodelled_by_mnemonic'] = unmod
    return items, stats


# ---------------------------------------------------------------------------
# Target image access, for resolving switch tables
# ---------------------------------------------------------------------------

class PEImage(object):
    """VA-addressed read access into the target PE.

    An indirect jump's destination lives in a table in the image, not in the
    instruction stream, so resolving one needs the real bytes. This maps a
    virtual address to a file offset through the section table.
    """

    def __init__(self, path):
        self.data = open(path, 'rb').read()
        if self.data[:2] != b'MZ':
            raise ValueError('%s is not a PE' % path)
        e_lfanew = struct.unpack_from('<I', self.data, 0x3c)[0]
        if self.data[e_lfanew:e_lfanew + 4] != b'PE\0\0':
            raise ValueError('%s has no PE signature' % path)
        coff = e_lfanew + 4
        nsec = struct.unpack_from('<H', self.data, coff + 2)[0]
        opt_size = struct.unpack_from('<H', self.data, coff + 16)[0]
        opt = coff + 20
        self.image_base = struct.unpack_from('<I', self.data, opt + 28)[0]
        self.sections = []
        sec = opt + opt_size
        for i in range(nsec):
            off = sec + 40 * i
            name = self.data[off:off + 8].rstrip(b'\0').decode('latin-1')
            vsize, va, rawsize, rawptr = struct.unpack_from('<IIII', self.data, off + 8)
            self.sections.append((name, va, vsize, rawptr, rawsize))

    def read(self, va, n):
        rva = va - self.image_base
        for _name, sva, vsize, rawptr, rawsize in self.sections:
            if sva <= rva < sva + max(vsize, rawsize):
                off = rawptr + (rva - sva)
                chunk = self.data[off:off + n]
                if len(chunk) != n:
                    raise ValueError('short read at 0x%x' % va)
                return chunk
        raise ValueError('address 0x%x is in no section' % va)


if __name__ == '__main__':
    import json
    import sys
    img = None
    if len(sys.argv) > 2:
        img = PEImage(sys.argv[2])
    fns = json.load(open(sys.argv[1] if len(sys.argv) > 1 else
                          'out/functions.json'))['functions']
    tot = mod = un = 0
    bymnem = {}
    for f in fns:
        insns = f.get('insns') or []
        items, st = transcribe(insns, img, f['va'], f.get('end') or f['va'] + f['size'])
        tot += st['modelled'] + st['unmodelled']
        mod += st['modelled']
        un += st['unmodelled']
        for k, v in st['unmodelled_by_mnemonic'].items():
            bymnem[k] = bymnem.get(k, 0) + v
    print('instructions: %d, modelled: %d (%.2f%%), unmodelled: %d (%.2f%%)' %
          (tot, mod, 100.0 * mod / tot, un, 100.0 * un / tot))
    print('unmodelled by mnemonic (top 40):')
    for k, v in sorted(bymnem.items(), key=lambda kv: -kv[1])[:40]:
        print('  %-28s %d' % (k, v))
