#!/usr/bin/env python3
"""
Emit a compilable C++ header for every class in the manifest.

Design:
  - Inheritance uses ONLY direct bases (out/direct_bases.json), because RTTI
    base arrays list transitive bases which C++ must not repeat.
  - Each header #includes its direct base headers, so types are complete.
  - Classes with no recovered base derive from nothing.
  - Virtual slots are emitted in real vtable order with their target VA.
  - The constructor VA is recorded; member offsets are recovered from the
    constructor's store sequence where possible.

Output is deterministic and regenerable.
"""
import bisect
import glob
import json
import os
import re
import struct
import sys
from collections import defaultdict

import capstone

_KEYWORDS = {"class", "struct", "const", "unsigned", "signed", "int",
             "char", "short", "long", "void", "bool", "float", "double"}

IB = 0x400000


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

    def rd(self, va, n):
        for vs, ve, raw, rs, nm in self.secs:
            if vs <= va < ve:
                o = raw + (va - vs)
                return self.data[o:o + n]
        return None

    def u32(self, va):
        b = self.rd(va, 4)
        return struct.unpack("<I", b)[0] if b and len(b) == 4 else None

    def cstr(self, va, n=256):
        b = self.rd(va, n)
        if not b:
            return None
        z = b.find(b"\x00")
        if z >= 0:
            b = b[:z]
        try:
            return b.decode("ascii")
        except UnicodeDecodeError:
            return b.decode("latin-1")

    def in_text(self, va):
        t = self.text
        return t[0] <= va < t[1]


def sanitize(name):
    n = re.sub(r"^(class|struct)\s+", "", name)
    n = n.replace("::", "_")
    n = re.sub(r"[<>,\s\*&]+", "_", n)
    n = re.sub(r"_+", "_", n).strip("_")
    return n


class StoreWidths:
    """Access-width evidence for recovered member offsets, read from the ctor.

    gen_members.py records WHERE a constructor stores (the displacement and the
    kind of value) but not HOW WIDE the store is, so this emitter previously
    declared every member as a 4-byte `unsigned long`. That is wrong for the
    offsets the constructor writes with a byte store (`mov byte ptr [eax+0xc],
    1`), and it silently swallowed every recovered offset that fell inside the
    4-byte span of such a declaration.

    This re-reads the constructor's machine code -- the same evidence the
    offset table came from -- and records the width of the store at each
    displacement. Only stores through the register that receives the class's
    vtable pointer (the `this` register) count, so a store through a reused
    scratch register into some other object is not attributed to a member.
    """

    # Matches gen_members.py / verify_members_binary.py: the vtable write must
    # be early in the function and go through a non-stack register, and the
    # function body is read as one blob from its recovered entry point.
    MAX_FN = 0x800
    VTABLE_WRITE_WINDOW = 200

    def __init__(self, bin_, fn_starts):
        self.bin = bin_
        self.starts = fn_starts
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.md.detail = True
        self._insns = {}
        self._maps = {}

    def _fn_insns(self, site):
        """Instructions of the recovered function enclosing `site`."""
        i = bisect.bisect_right(self.starts, site) - 1
        if i < 0:
            return []
        fn = self.starts[i]
        if fn not in self._insns:
            blob = self.bin.rd(fn, self.MAX_FN)
            self._insns[fn] = list(self.md.disasm(blob, fn)) if blob else []
        return self._insns[fn]

    def width_map(self, vtva, ctor_site):
        """offset -> (store width in bytes, the store instruction as text).

        Where the constructor stores more than one width to one displacement
        the WIDEST store is kept: a field cannot be narrower than a store into
        it, and declaring it narrower would move the members that follow.
        """
        key = (vtva, ctor_site)
        if key in self._maps:
            return self._maps[key]
        insns = self._fn_insns(ctor_site) if ctor_site else []
        this_reg = None
        for pos, ins in enumerate(insns):
            if pos > self.VTABLE_WRITE_WINDOW:
                break
            if ins.mnemonic != "mov" or len(ins.operands) != 2:
                continue
            dst, src = ins.operands
            if (dst.type == capstone.x86.X86_OP_MEM
                    and src.type == capstone.x86.X86_OP_IMM
                    and src.imm == vtva):
                base = ins.reg_name(dst.mem.base)
                if base not in ("esp", "ebp"):
                    this_reg = base
                    break
        wmap = {}
        if this_reg:
            for ins in insns:
                if ins.mnemonic not in ("mov", "movzx", "movsx", "and", "or"):
                    continue
                if len(ins.operands) != 2:
                    continue
                dst = ins.operands[0]
                if dst.type != capstone.x86.X86_OP_MEM:
                    continue
                if ins.reg_name(dst.mem.base) != this_reg:
                    continue
                w = dst.size
                if w not in (1, 2, 4):
                    continue
                off = dst.mem.disp
                cur = wmap.get(off)
                if cur is None or w > cur[0]:
                    wmap[off] = (w, f"{ins.mnemonic} {ins.op_str}")
        self._maps[key] = wmap
        return wmap


# Declared type for each store width the binary uses. The 4-byte form is
# `unsigned int`, not `unsigned long`: `long` is 8 bytes on an LP64 host but 4
# in the game, so declaring every recovered member `unsigned long` inflates
# every generated struct (2,313 of 2,918 member declarations) and no recovered
# offset can be reproduced. `unsigned int` is 4 bytes on both, so the emitted
# layout matches the target ABI on this host. Nothing here is inferred beyond
# the store instruction itself.
_WIDTH_TYPE = {1: "unsigned char", 2: "unsigned short", 4: "unsigned int"}


def member_width(widths, off):
    """(declared type, size in bytes, evidence annotation) for member `off`.

    The declared type follows the store width the constructor uses at that
    displacement. A record whose width could not be determined keeps
    `unsigned int` and is annotated `/* width ? */` rather than guessed at.

    A declared type's own alignment has to divide its offset, or the compiler
    cannot put the member there at all: IPXInterfaceClass's constructor stores a
    dword at +0x452, and declaring that `unsigned int` puts it at +0x454. So the
    width is narrowed to the largest one whose alignment divides the recovered
    offset, and the annotation says so instead of silently disagreeing with the
    store evidence.
    """
    rec = widths.get(off)
    if rec:
        w, insn = rec
        declared = w
        while w > 1 and off % w:
            w //= 2
        typ = _WIDTH_TYPE.get(w)
        if typ:
            ann = f"{declared}-byte store: {insn}"
            if w != declared:
                ann += ("; declared %d-byte, because a %d-byte type cannot sit "
                        "at +0x%x" % (w, declared, off))
            return typ, w, ann
    w = 4
    while w > 1 and off % w:
        w //= 2
    return _WIDTH_TYPE.get(w, "unsigned int"), w, "/* width ? */"


def _member_decl_size(widths, off):
    """Declared width for the layout model, matching member_width()."""
    rec = widths.get(off)
    w = rec[0] if rec else 4
    while w > 1 and off % w:
        w //= 2
    return w
_MEMBER_BANNER = "// ---- data members (offsets from ctor"

# The first line of the block this emitter writes. A hand-written header may
# imitate the banner line above (DynamicVectorClass does), so the banner cannot
# tell a curated block from a machine one -- but only the emitter writes this
# sentence, and only as the head of its own block.
_MACHINE_MARK = "    // Offsets are exact. Each member is declared at the access width"

# Width in bytes of every member type the curated hand-written blocks use. The
# table is closed on purpose: it is what makes simulating the layout of a
# hand-written block safe. Measured over the tree, the curated declarations are
# `int`, `unsigned int`, `unsigned char`, `bool`, `short`, `unsigned short`,
# `char`, pointers and one `char[25]`; anything else returns None and the
# simulator falls back to the offset the declaration documents.
# (size, alignment) per member type, at the target ABI. Alignment is the
# ELEMENT alignment, not the declaration's total width: `char m_Name[25]` is
# 1-aligned and 25 bytes wide, and treating it as 4-aligned shifted every
# member after it (it moved ObjectTypeClass's block by 3 bytes). g++ -m32
# aligns `double` to 4, measured.
_TYPE_SIZE = {
    "bool": (1, 1), "char": (1, 1), "signed char": (1, 1),
    "unsigned char": (1, 1), "BYTE": (1, 1),
    "short": (2, 2), "unsigned short": (2, 2), "wchar_t": (2, 2), "WORD": (2, 2),
    "int": (4, 4), "unsigned int": (4, 4), "long": (4, 4),
    "unsigned long": (4, 4), "unsigned": (4, 4), "signed": (4, 4),
    "float": (4, 4), "DWORD": (4, 4), "ULONG": (4, 4), "LONG": (4, 4),
    "UINT": (4, 4), "HRESULT": (4, 4), "BOOL": (4, 4),
    "double": (8, 4), "long double": (12, 4),
}

# The offset a curated declaration documents, in any of the comment shapes the
# hand-written headers use: "// +0x14 ctor: mov ...", "// 0xC6 INI \"Theater\"".
_CLAIM_RE = re.compile(r"//[^0-9a-fA-F]*?0x([0-9a-fA-F]+)")

# `type ident`, `type ident[N]`, optional bitfield width, no initialiser.
_MEMBER_DECL_RE = re.compile(
    r"^\s*(.*?)\b([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*(?::\s*\d+)?\s*$")


def _type_size(type_text, array_text):
    """(width, alignment) of a curated member declaration, or (None, None)."""
    n = 1
    if array_text:
        inner = array_text[1:-1].strip()
        if not inner.isdigit():
            return None, None
        n = int(inner)
    t = re.sub(r"\b(const|volatile|struct|class|enum|union)\b", " ", type_text)
    t = re.sub(r"\s+", " ", t).strip()
    if t.endswith("*") or t.endswith("&"):
        base, al = 4, 4
    elif t in _TYPE_SIZE:
        base, al = _TYPE_SIZE[t]
    else:
        return None, None
    return base * n, al


# A declaration this tool commented out because the compiler had already
# passed its documented offset. Recognised so the line can be restored: the
# model must see the declaration the curator wrote, not the emitter's note
# about it, or the plan it computes drifts a little further every run.
_UNCOMMENT_MARK = "//<UNCOMMENT>"

# The marker an earlier revision wrote. Kept so a tree generated by it is
# repaired rather than left with the note repeated on every run.
_OLD_UNCOMMENT_MARK = "-- commented out: it documents +0x"


# Both markers, and the note this tool appends after the declaration, contain
# this phrase; the declaration is everything before the first one.
_NOTE_TAIL = "commented out: it documents"


def _uncommented(line):
    """The declaration a line carries, if this tool commented it out earlier."""
    if _UNCOMMENT_MARK in line:
        body = line.split(_UNCOMMENT_MARK, 1)[1]
    elif _OLD_UNCOMMENT_MARK in line and "//" in line:
        body = "    " + line.split("//", 1)[1]
    else:
        return None
    cut = body.find(_NOTE_TAIL)
    if cut >= 0:
        body = body[:cut]
    # What is left can still carry the punctuation of an earlier note's tail
    # ("--", "//"); the declaration's own comment ends before it.
    body = body.rstrip().rstrip("-/").rstrip()
    return body or None


def curated_members(body):
    """(documented offset, width, alignment, ident) for curated members.

    `body` is a class body as a hand-written header writes it. Width and
    alignment are None when the declaration's type is not in the closed table.

    Everything from the block this emitter writes onward is dropped: that block
    is what the model computes, so reading it back would be circular, and its
    members may be named after INI keys rather than `m_<hex>`, so the identifier
    cannot tell them apart. The cut is at the emitter's own marker sentence, not
    at the banner line: DynamicVectorClass's curated members sit under a banner
    of their own, and cutting there silently dropped them.
    """
    cut = body.find(_MACHINE_MARK)
    if cut >= 0:
        body = body[:cut]
    out = []
    # The body text starts at the class's opening brace, so the class's own
    # members are at depth 0 from the second line on.
    depth = -1
    for lineno, line in enumerate(body.splitlines()):
        line = _uncommented(line) or line
        s = line.strip()
        code_all = line.split("//")[0]
        at_top = depth <= 0
        depth += code_all.count("{") - code_all.count("}")
        # A nested class body is not this class's member list. Without this the
        # outer INIClass body contributed INISection's Name/Value members.
        if not at_top:
            continue
        if not s or s.startswith("//") or ";" not in s:
            continue
        code = line.split("//")[0]
        if "(" in code:
            continue
        if re.match(r"\s*(typedef|using|enum|friend|static_assert)\b", code):
            continue
        if re.match(r"\s*(struct|class|union)\s+\w+\s*;\s*$", code):
            continue
        if re.match(r"\s*(public|private|protected)\s*:", code):
            continue
        if re.match(r"\s*(virtual|static)\b", code):
            continue
        m = _MEMBER_DECL_RE.match(code.rstrip().rstrip(";"))
        if not m:
            continue
        type_text, ident, arr = m.group(1), m.group(2), m.group(3)
        # A declaration needs a type before the identifier; this rejects a
        # bare label and an access specifier.
        if not re.search(r"[A-Za-z_*&]\s*$", type_text):
            continue
        if re.fullmatch(r"(m|vptr|pad)_[0-9a-fA-F]{2,4}", ident):
            continue
        # A pad this tool inserted to hold a documented offset open is machine
        # text too; counting it as a curated declaration added its bytes twice.
        if ident.startswith("pad_doc_"):
            continue
        cm = _CLAIM_RE.search(line)
        w, al = _type_size(type_text, arr)
        out.append((int(cm.group(1), 16) if cm else None, w, al, ident, lineno))
    return out


class DeclaredLayout:
    """Where the compiler will put the members the emitter declares.

    The machine member block has to begin at the offset the compiler uses for
    the class's own first member, which is the compiled end of the base
    sub-object region -- not the first vtable-pointer store the recovered table
    records. For a base-less class the two coincide (the vptr region is the
    whole prefix), which is why those classes already reproduced their offsets;
    for a derived class they do not, and every member came out displaced by the
    difference.

    The cursor is computed from the declaration the compiler will read, which
    is this emitter's own output plus the hand-written text it does not
    replace:

      * the base list the declaration names -- RTTI's direct bases for a
        generated header, the header's own base clause for a hand-written one
        (`out/classes.json` mdisp corroborates the result but is not what the
        compiler lays out),
      * the curated member block a hand-written header already carries, laid
        out the way the compiler lays it out: declarations in order, aligned,
        because a comment saying "+0x1e0" does not move a member there, and
      * the machine block this emitter appends, whose members sit at their
        recovered offsets.

    Where the documented offset of a curated member and the offset the
    compiler gives it disagree, the emitter says so in the class's own block
    instead of averaging it away: the disagreement is a real defect in the
    hand-written header, and the pad the machine block would need is inside
    text this tool does not own.
    """

    def __init__(self, srcdir, hw_dir, manifest, direct, members,
                 widths_by_class, ident_of, rtti_by_ident, owned,
                 emitted_bases, find_body, prelude_owned_types,
                 owner_header, hw_names, body_owners):
        self.srcdir = srcdir
        self.hw_dir = hw_dir
        self.manifest = manifest
        self.direct = direct
        self.members = members
        self.widths = widths_by_class
        self.ident_of = ident_of
        self.rtti_by_ident = rtti_by_ident
        self.owned = owned
        self.emitted_bases = emitted_bases
        self.find_body = find_body
        self.prelude_owned_types = prelude_owned_types
        self.owner_header = owner_header
        self.hw_names = hw_names
        self.body_owners = body_owners
        self._nvirt = {m["class"]: m.get("nvirtuals") or 0 for m in manifest}
        self._manifest = {m["class"] for m in manifest}
        self._by_name = {}
        for m in manifest:
            for key in (m["class"], m["ident"], sanitize(m["class"])):
                self._by_name.setdefault(key, m["class"])
        self._cursor = {}
        self._end = {}
        self._dyn = {}
        self._body_cache = {}
        self._find_cache = {}
        self._resolve_cache = {}
        self._curated = {}

    # -- declaration text ---------------------------------------------------
    def _find(self, key):
        """(path, name) of a definition of `key`, or None."""
        if key in self._find_cache:
            return self._find_cache[key]
        res = None
        # The header that defines the class under the name the emitter uses
        # (a nested class the tool wrote into its own header as
        # INIClass_INISection) is what both the emitter and the layout check
        # see, so it is tried before the enclosing class's header.
        ident = self.ident_of.get(key)
        hdr = self.owner_header.get(ident) if ident else None
        if hdr:
            path = os.path.join(self.hw_dir if hdr in self.hw_names
                                else self.srcdir, hdr)
            if os.path.exists(path):
                src = open(path).read()
                for cand in (ident, key):
                    if cand and class_body_span(src, cand) is not None:
                        res = (path, cand)
                        break
        if res is None:
            res = self.find_body(key)
        if res is None:
            # Not a manifest class: an interface from ts_prelude.h, a mix-in
            # like StageClass, or a template head. Search the hand-written
            # headers first -- they are the authoritative copies -- then the
            # generated tree.
            for d in (self.hw_dir, self.srcdir):
                if not os.path.isdir(d):
                    continue
                for p in sorted(glob.glob(os.path.join(d, "*.h"))):
                    if class_body_span(open(p).read(), key) is not None:
                        res = (p, key)
                        break
                if res is not None:
                    break
        self._find_cache[key] = res
        return res

    def _hw_names(self):
        if not hasattr(self, "_hw_name_set"):
            self._hw_name_set = set()
            if os.path.isdir(self.hw_dir):
                self._hw_name_set = {
                    os.path.basename(p)
                    for p in glob.glob(os.path.join(self.hw_dir, "*.h"))}
        return self._hw_name_set

    def body(self, key):
        """(path, name as written, body text, hand-written?) or None."""
        if key in self._body_cache:
            return self._body_cache[key]
        res = None
        found = self._find(key)
        if found is not None:
            path, name = found
            try:
                src = open(path).read()
            except OSError:
                src = ""
            span = class_body_span(src, name)
            if span is None:
                bare = name.split("::")[-1].split("<")[0].strip()
                for cand in (bare, self.ident_of.get(key), key):
                    if cand:
                        span = class_body_span(src, cand)
                        if span is not None:
                            name = cand
                            break
            if span is not None:
                res = (path, name, src[span[0]:span[1]],
                       os.path.basename(path) in self._hw_names())
        self._body_cache[key] = res
        return res

    def _resolve(self, name):
        """Canonical key for a base name written in a declaration, or None."""
        if name in self._resolve_cache:
            return self._resolve_cache[name]
        key = self._by_name.get(name)
        if key is None and self._find(name) is not None:
            key = name
        self._resolve_cache[name] = key
        return key

    def _declared_bases(self, key):
        """Keys of the bases the emitted declaration actually names."""
        b = self.body(key)
        if b is not None and b[3]:
            src = open(b[0]).read()
            m = re.search(
                r"^[ \t]*(?:template\s*<[^;{}]*>\s*)?(?:class|struct)\s+"
                + re.escape(b[1]) + r"\b([^{;]*)\{", src, re.M)
            if m:
                clause = m.group(1)
                if clause.strip().startswith(":"):
                    out = []
                    for part in clause.strip()[1:].split(","):
                        nm = re.sub(r"\b(public|private|protected|virtual)\b",
                                    " ", part)
                        nm = nm.split("<")[0].strip().rstrip("*&")
                        nm = nm.split("::")[-1]
                        for kw in ("class ", "struct "):
                            if nm.startswith(kw):
                                nm = nm[len(kw):]
                        r = self._resolve(nm)
                        if r and r not in out:
                            out.append(r)
                    return out
            return []
        return list(self.emitted_bases.get(key) or [])

    # -- layout -------------------------------------------------------------
    def dynamic(self, key):
        """True when the declaration gives this class a vtable."""
        if key in self._dyn:
            return self._dyn[key]
        self._dyn[key] = False        # cycle guard
        b = self.body(key)
        if b is not None and b[3]:
            v = bool(re.search(r"\bvirtual\b", b[2]))
        else:
            v = self._nvirt.get(key, 0) > 0
        if not v:
            v = any(self.dynamic(x) for x in self._declared_bases(key))
        self._dyn[key] = v
        return v

    def _base_region(self, key):
        """(cursor, [(base ident, offset, end)]) for the base sub-objects.

        The compiler allocates the first dynamic base at +0x0 (it shares the
        class's vptr), the rest in declaration order at aligned offsets, and
        the class's own vptr at +0x0 when no base is dynamic. This is C++'s
        rule, not RTTI's mdisp: the compiler lays out this header, and a
        disagreement between the two is a defect the check reports.
        """
        bases = self._declared_bases(key)
        dyn = [i for i, x in enumerate(bases) if self.dynamic(x)]
        primary = dyn[0] if dyn else None
        pos = 4 if (primary is None and self.dynamic(key)) else 0
        order = ([primary] + [i for i in range(len(bases)) if i != primary]
                 if primary is not None else list(range(len(bases))))
        parts = []
        for i in order:
            x = bases[i]
            off = 0 if i == primary else (pos + 3) & ~3
            end = self.end(x)
            if end == 0 and self.body(x) is None:
                # A base whose declaration the model cannot read is a prelude
                # stub, and every one of them stands for a real class with a
                # vptr or a reference count. ts_prelude.h declares
                # ATL::CComObjectRootEx as an empty struct, which is an
                # under-declaration: the binary's CComObjectRootBase carries the
                # 4-byte reference count, and the recovered store at +0x4 is
                # that member, not CChatEventSink's. Assume a word.
                end = 4
            parts.append((self.ident_of.get(x) or x, off, off + end))
            pos = off + end
        return pos, parts

    def cursor(self, key):
        """Offset at which the class's own members begin."""
        if key in self._cursor:
            return self._cursor[key]
        self._cursor[key] = 0         # cycle guard
        pos, _parts = self._base_region(key)
        self._cursor[key] = pos
        return pos

    def curated_layout(self, key):
        """[(offset, width, ident, documented offset or None)] curated block.

        Laid out the way the compiler lays it out -- declarations in order,
        each aligned for its type -- because that is what decides where the
        next declaration goes. The documented offset is carried alongside so
        the emitter can report where the two disagree.
        """
        if key in self._curated:
            return self._curated[key]
        b = self.body(key)
        out = []
        if b is not None and b[3]:
            cur = self.cursor(key)
            floor = self.cursor(key)
            for claim, width, align, ident, _ln in curated_members(b[2]):
                w = width if width is not None else 4
                a = align if align else 1
                if claim is not None and claim < floor:
                    # Inside the vptr or base region, which C++ supplies itself.
                    # The emitter comments the line out and it takes no space,
                    # so the model must not count it either.
                    continue
                if claim is not None and claim >= cur:
                    # The emitter inserts a pad so the compiler agrees with the
                    # documented offset; see curated_pad_plan().
                    off = claim
                else:
                    off = (cur + a - 1) & ~(a - 1)
                out.append((off, w, ident, claim))
                cur = off + w
        self._curated[key] = out
        return out

    def curated_end(self, key):
        """End of the curated hand-written block, as the compiler lays it out."""
        lay = self.curated_layout(key)
        return lay[-1][0] + lay[-1][1] if lay else 0

    def curated_spans(self, key):
        """(offset, width, ident) for the curated members, at their layout."""
        return [(o, w, i) for o, w, i, _c in self.curated_layout(key)]

    def curated_names(self, key):
        """Identifiers the curated block already declares."""
        return {i for _o, _w, i, _c in self.curated_layout(key)}

    def curated_pad_plan(self, key):
        """(line, pad, ident, claim) to make a curated block sit where it says.

        The compiler lays declarations out one after another, so a curated block
        with a gap in it lands short of the offsets its own comments document --
        HouseTypeClass's m_Field60 says +0x60 and lands at +0x2c. The emitter
        inserts the missing bytes as a pad before the declaration, which is the
        same mechanism its own block uses, and the pad is named so a re-run can
        find and replace it.

        A declaration that documents an offset BELOW where the compiler has
        already got to cannot be pulled back -- there is no negative pad -- so
        the emitter comments the line out (when the claim is inside the class's
        vptr or base region, C++ supplies that slot itself) and reports it.

        Returns (pads, duplicates, unfixable).
        """
        b = self.body(key)
        if b is None or not b[3]:
            return [], []
        pads, duplicates, unfixable = [], [], []
        floor = self.cursor(key)
        cur = floor
        for claim, width, align, ident, lineno in curated_members(b[2]):
            w = width if width is not None else 4
            a = align if align else 1
            if claim is not None and claim < floor:
                duplicates.append((lineno, ident, claim))
                continue
            if claim is not None and claim >= cur:
                if claim > cur:
                    pads.append((lineno, ident, claim - cur, claim))
                cur = claim + w
            else:
                off = (cur + a - 1) & ~(a - 1)
                if claim is not None:
                    unfixable.append((ident, claim, off))
                cur = off + w
        return pads, duplicates, unfixable

    def curated_drift(self, key):
        """[(ident, documented, actual)] where the compiler moves a member."""
        return [(i, c, o) for o, _w, i, c in self.curated_layout(key)
                if c is not None and c != o]

    def _defined_elsewhere(self, name):
        """True when some header in the tree already defines `name`."""
        for p in sorted(glob.glob(os.path.join(self.srcdir, "*.h"))):
            if re.search(r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+"
                         + re.escape(name) + r"\b", open(p).read(), re.M):
                return True
        return False

    def _members_of(self, key):
        """(offset, width) the declaration gives this class's own members.

        For a class the manifest knows, its own recovered table. For a template
        head (VectorClass, TypeList) the manifest knows only its instantiations,
        whose members the emitter writes into the template body, so the union of
        theirs is what the declaration carries.
        """
        out = []
        if key in self._manifest:
            wmap = self.widths.get(key) or {}
            for off in self.recovered(key):
                out.append((off, _member_decl_size(wmap, off)))
            return out
        b = self.body(key)
        if b is None:
            return out
        for rtti in self.body_owners.get((b[0], b[1]), []):
            if rtti == key:
                continue
            wmap = self.widths.get(rtti) or {}
            for off in self.recovered(rtti):
                out.append((off, _member_decl_size(wmap, off)))
        return sorted(set(out))

    def emits_machine_block(self, key):
        """True when the emitter writes a machine block for this class.

        A class ts_prelude.h owns gets no block (the prelude is regenerated
        after this stage), and neither does one no header owns. That matters to
        the layout: without a block the recovered offsets are not declared at
        all, so the class's declaration ends where its curated block does.
        """
        if key not in self._manifest:
            b = self.body(key)
            return bool(b and self.body_owners.get((b[0], b[1])))
        ident = self.ident_of.get(key)
        if ident is None or ident in self.prelude_owned_types:
            return False
        hdr = self._find(key)
        if hdr is None:
            return not self._defined_elsewhere(key.split("<")[0].split()[-1])
        return os.path.basename(hdr[0]) != "ts_prelude.h"

    def end(self, key):
        """End of the member region the emitted declaration gives this class."""
        if key in self._end:
            return self._end[key]
        self._end[key] = 0            # cycle guard
        cur = self.cursor(key)
        lay = self.curated_layout(key)
        if lay:
            cur = max(cur, lay[-1][0] + lay[-1][1])
        if self.emits_machine_block(key):
            for off, w in self._members_of(key):
                if off < cur:
                    continue
                cur = max(cur, off + w)
        if self.dynamic(key):
            cur = max(cur, 4)
        self._end[key] = cur
        return cur

    def recovered(self, key):
        rec = self.members.get(key)
        if not rec:
            return []
        return sorted({r["off"] for r in (rec.get("members") or [])})

    def base_note(self, key):
        """Human-readable evidence for the cursor, for the emitted comment."""
        _pos, parts = self._base_region(key)
        if not parts:
            return "no base: the compiler's own vptr occupies +0x0"
        return ", ".join("%s at +0x%x (ends +0x%x)" % p for p in parts)





def member_block(rtti_class, members, member_names, widths,
                 cursor=0, base_cursor=0, curated=(), curated_names=(),
                 note=None, drift=()):
    """Emit the recovered data-member declarations for one class.

    The offset table (out/members.json) is keyed by the RTTI class spelling.
    Offsets are exact; names are only used where an INI key or string supplies
    evidence. Each member is declared at the access width the constructor
    actually stores it with (`widths`, from StoreWidths) and the store
    instruction is quoted in the comment as evidence. The block is returned as
    a list of lines so the same code can either place it inside a class body
    being generated here, or append it to the header that already defines the
    class (nested types and template instantiations have no standalone header
    of their own).

    Vtable-pointer stores (`"vptr": true` in the offset table) are NOT emitted
    as members: the compiler supplies the vptr, so a member for it would be a
    second copy. They do not advance the cursor either -- the cursor already
    accounts for the vptr region of this class's declaration, and a vptr store
    at or above it belongs to a base sub-object.

    `cursor` is the offset at which the machine block begins: the compiled end
    of the class's base sub-object region (`base_cursor`) and of the curated
    block a hand-written header already carries. Recovered offsets below it
    belong to a base sub-object or to a curated declaration; they are not
    repeated here and the block says which of the two each one is, because a
    dropped offset that nobody names is how a layout gap gets averaged away.

    Returns [] when no offsets were recovered for the class.
    """
    md = members.get(rtti_class)
    if not (md and md.get("members")):
        return []
    _mn = member_names.get(rtti_class) or {}
    named = _mn.get("members", {})
    # A key can map to several offsets (repeated INI reads). Only the first
    # use of a name is emitted; later ones fall back to m_XXX.
    _used_names = set()
    # Curated declarations a hand-written header already carries. The curated
    # block WINS: it is the richer, evidence-backed source, and the machine
    # block is appended after it, so a second declaration at the same
    # displacement would either collide or displace everything that follows.
    spans = [(c, c + (w or 4), i) for c, w, i in curated if c is not None]
    # A curated declaration's IDENTIFIER is a second way it owns an offset: the
    # machine block names an INI-key member after the key, and the curated block
    # may already declare that name. Re-emitting it is a redeclaration error
    # (AnimTypeClass::TiberiumChainReaction), so the name is checked too.
    curated_names = set(curated_names)
    L = []
    L.append("")
    L.append(f"    // ---- data members (offsets from ctor 0x{md['ctor']:08x}) ----")
    L.append("    // Offsets are exact. Each member is declared at the access width")
    L.append("    // the constructor stores it with; the store instruction is quoted")
    L.append("    // as evidence. `/* width ? */` means no store could be classified.")
    L.append("    // vtable-pointer stores are omitted: the compiler supplies the vptr.")
    if cursor:
        L.append(f"    // The machine block begins at +0x{cursor:x}: the compiled end of the")
        if base_cursor and base_cursor != cursor:
            L.append(f"    // base sub-object region (+0x{base_cursor:x}) and of the curated block")
            L.append("    // above. Base evidence: " + (note or "recovered base offsets") + ".")
        else:
            L.append("    // base sub-object region. Evidence: "
                     + (note or "recovered base offsets") + ".")
    if drift:
        L.append("    // %d curated declaration(s) do not sit at the offset their comment"
                 % len(drift))
        L.append("    // documents -- a comment does not move a member -- so the machine")
        L.append("    // block below is padded from where the compiler actually puts them:")
        L.append("    // " + ", ".join("%s documented +0x%x, compiler +0x%x" % d
                                        for d in drift[:4])
                 + (" ..." if len(drift) > 4 else ""))
    inside_base = []
    in_curated = []
    overlap = []
    covered = []
    named_elsewhere = []
    prev_end = cursor
    body = []
    for mem in md["members"]:
        off = mem["off"]
        if off < base_cursor:
            inside_base.append(off)
            continue
        if off < prev_end:
            if off < cursor:
                in_curated.append(off)
            else:
                overlap.append(off)
            continue
        span = next((s for s in spans if s[0] <= off < s[1]), None)
        if span is not None:
            covered.append((off, span))
            continue
        # The pad is written only when a member is actually emitted, so a
        # skipped offset cannot leave a trail of same-named zero-length pads.
        kind = mem["kind"]
        nm = named.get(str(off), {}).get("name")
        if mem.get("vptr"):
            # A vtable-pointer store is not a data member. C++ inserts the vptr
            # itself for a polymorphic class (and for each polymorphic base
            # sub-object), so emitting the constructor's store as a member
            # counts the slot twice and displaces every member after it
            # (measured: offsetof(AbstractClass, m_000) == 20 while the
            # recovered table says 0). The slot is left to the compiler, and it
            # does NOT advance the cursor: the cursor already covers this
            # class's vptr region, so a store at or above it is a base
            # sub-object's vptr -- AITriggerTypeClass records one at +0x4 that
            # its header's base-less declaration never gives the compiler, and
            # advancing past it put every member 4 bytes low.
            continue
        if nm and nm in curated_names:
            named_elsewhere.append(nm)
            continue
        if nm and nm not in _used_names:
            _used_names.add(nm)
            typ, size, ann = member_width(widths, off)
            line = f"    {typ} {nm};  // +0x{off:03x} (INI key); {ann}"
        elif kind == "zero":
            typ, size, ann = member_width(widths, off)
            line = f"    {typ} m_{off:03x};  // +0x{off:03x} zero-initialised; {ann}"
        elif kind == "imm":
            typ, size, ann = member_width(widths, off)
            line = (f"    {typ} m_{off:03x};  // +0x{off:03x} init "
                    f"0x{mem['value']:x}; {ann}")
        else:
            typ, size, ann = member_width(widths, off)
            line = f"    {typ} m_{off:03x};  // +0x{off:03x} /* ? */; {ann}"
        if off > prev_end:
            body.append(f"    unsigned char pad_{prev_end:03x}[0x{off - prev_end:x}];")
        body.append(line)
        prev_end = off + size
    if inside_base:
        L.append("    // %d recovered offset(s) below +0x%x lie inside the base"
                 % (len(set(inside_base)), base_cursor))
        L.append("    // sub-object region and are NOT declared here: they are"
                 " declared and")
        L.append("    // compiler-checked in the base's own header -- "
                 + ", ".join("+0x%x" % o for o in sorted(set(inside_base))[:8])
                 + (" ..." if len(set(inside_base)) > 8 else ""))
    if in_curated:
        L.append("    // %d recovered offset(s) are inside the curated hand-written block"
                 % len(set(in_curated)))
        L.append("    // above (its span is +0x%x..+0x%x) and are NOT repeated here:"
                 % (base_cursor, cursor))
        L.append("    // the curated block wins -- " +
                 ", ".join("+0x%x" % o for o in sorted(set(in_curated))[:8])
                 + (" ..." if len(set(in_curated)) > 8 else ""))
    if named_elsewhere:
        L.append("    // %d recovered offset(s) are already declared by a curated member"
                 % len(set(named_elsewhere)))
        L.append("    // above under the same identifier, which wins: "
                 + ", ".join(sorted(set(named_elsewhere))[:6])
                 + (" ..." if len(set(named_elsewhere)) > 6 else ""))
    if overlap:
        L.append("    // %d recovered offset(s) fall inside a wider declaration above"
                 % len(set(overlap)))
        L.append("    // (the store width and the recovered table disagree): "
                 + ", ".join("+0x%x" % o for o in sorted(set(overlap))[:8]))
    if covered:
        L.append("    // %d recovered offset(s) coincide with a curated declaration"
                 % len(covered))
        L.append("    // above, which wins: "
                 + ", ".join("+0x%x (%s)" % (o, s[2]) for o, s in sorted(covered)[:6])
                 + (" ..." if len(covered) > 6 else ""))
    L.extend(body)
    if (not body and not inside_base and not in_curated and not covered
            and not overlap and not named_elsewhere):
        L.append("    // (no recovered offset lies at or above the cursor)")
    return L


def class_body_span(src, class_name):
    """Locate the body of `class_name` in a header.

    Returns (body_start, body_end) as character indices into `src`, where
    body_end is the index of the '}' that closes the class. Returns None when
    the class is not defined in the file. Matching is anchored to a definition
    line (`class NAME {`, `class NAME : public ... {`, or the same with a
    leading `template <...>`), so a forward declaration (`class NAME;`) or a
    mention in a comment does not match.
    """
    pat = re.compile(
        r"^[ \t]*(?:template\s*<[^;{}]*>\s*)?(?:class|struct)\s+"
        + re.escape(class_name) + r"\b[^;{}]*\{", re.M)
    m = pat.search(src)
    if not m:
        return None
    i = src.index("{", m.start())
    depth = 0
    j = i
    while j < len(src):
        if src[j] == "{":
            depth += 1
        elif src[j] == "}":
            depth -= 1
            if depth == 0:
                return i, j
        j += 1
    return None


# A pad this tool inserted into a hand-written block. It is machine text, so a
# re-run removes it before deciding where the gaps are -- otherwise every run
# would add another pad and the block would walk down the class.
_CURATED_PAD_RE = re.compile(r"^\s*unsigned char pad_doc_\w+\[[^\]]*\];")


def apply_curated_pads(src, class_name, layout, rtti_class):
    """Make a curated hand-written block sit at the offsets it documents.

    A comment saying "+0x60" does not move a member: the compiler lays
    declarations out one after another, so a block with a gap in it lands short
    (HouseTypeClass's m_Field60 documents +0x60 and lands at +0x2c). The gap is
    filled with a pad, exactly as the machine block fills its own, and the pad
    line says which declaration it is for.

    A declaration that documents an offset the compiler has already passed
    cannot be pulled back -- there is no negative pad -- so it is reported
    instead: Pipe's `void* vptr; // +0x0` is the compiler's own vptr, and the
    compiler puts a second one at +0x4.

    Returns (new_src, pads_inserted, duplicates_commented, unfixable).
    """
    pads, duplicates, unfixable = layout.curated_pad_plan(rtti_class)
    span = class_body_span(src, class_name)
    if span is None:
        return src, 0, 0, unfixable
    start, end = span
    # The pass always runs, even with nothing to insert: it also drops the pads
    # an earlier run inserted and restores the lines it commented out, so the
    # block is the curator's text again before the gaps are measured.
    lines = [_uncommented(ln) or ln for ln in src[start:end].split("\n")
             if not _CURATED_PAD_RE.match(ln)]
    # The line numbers in the plan refer to the block as it was parsed, which is
    # the text with any earlier pads already removed, so re-parse to match.
    loc = {}
    for _c, _w, _a, ident, lineno in curated_members("\n".join(lines)):
        loc.setdefault(ident, lineno)
    pad_at = {loc[i]: (i, size, claim) for _l, i, size, claim in pads if i in loc}
    dup_at = {loc[i]: (i, claim) for _l, i, claim in duplicates if i in loc}
    out = []
    for i, ln in enumerate(lines):
        if i in dup_at:
            ident, claim = dup_at[i]
            # `ln` may already carry the marker and the note from an earlier
            # run. Appending to the raw line makes this non-idempotent: the
            # note accumulates once per pipeline run. Re-emit from the bare
            # declaration instead, so the output is a pure function of the
            # input.
            out.append("    %s%s  // commented out: it documents 0x%x, which "
                       "C++ supplies itself (the compiler's own vptr or base "
                       "sub-object is already there)"
                       % (_UNCOMMENT_MARK, _uncommented(ln) or ln, claim))
            continue
        if i in pad_at:
            ident, size, claim = pad_at[i]
            # The comment must not spell "+0x": the layout check reads a
            # trailing "+0xNN" as a member's claimed offset, and a pad is not
            # a member claim -- naming one made the pad itself an assertion
            # that could not hold.
            out.append("    unsigned char pad_doc_%s[0x%x];  // holds the gap "
                       "before %s, which documents 0x%x (the compiler would "
                       "put it at 0x%x)"
                       % (ident, size, ident, claim, claim - size))
        out.append(ln)
    return (src[:start] + "\n".join(out) + src[end:],
            len(pad_at), len(dup_at), unfixable)


def insert_member_block(src, class_name, block):
    """Replace or insert the machine-generated member block for `class_name`.

    Returns the new source, or None when the class body cannot be found.
    Appending to the owning header (rather than emitting a nested class body)
    keeps every header compiling standalone, which verify.py check 1 requires.

    Any block written by a PREVIOUS run is removed first. Without this a rerun
    accumulates: when the recovery improves and no longer produces an offset,
    the stale declaration stays behind and verify.py check 7 correctly reports
    it as an emitted offset with no corresponding recovered entry.
    """
    span = class_body_span(src, class_name)
    if not span:
        return None
    start, end = span
    body = src[start:end]
    banner = "// ---- data members (offsets from ctor"
    if banner in body:
        # Excise the previous machine block: from its banner to the closing
        # brace, keeping any curated text that preceded it.
        cut = body.index(banner)
        body = body[:cut].rstrip() + "\n"
        src = src[:start] + body + src[end:]
        span = class_body_span(src, class_name)
        if not span:
            return None
        _, end = span
    # Back up over the trailing whitespace/newline before '}' so the inserted
    # block lands on its own lines and the closing brace stays put.
    at = end
    while at > 0 and src[at - 1] in " \t\r\n":
        at -= 1
    return src[:at] + "\n" + "\n".join(block) + "\n" + src[at:]




def shared_slot0_map(manifest):
    """slot-0 addresses that more than one class points at.

    A vtable slot 0 shared between classes is a stub, not a destructor: 66 of
    the 749 classes point at 0x405790. Emitting `virtual ~X()` for each of them
    claims a slot position the destructor does not hold, which is what the
    headers did until this map existed.
    """
    counts = {}
    for e in manifest:
        ents = e.get("entries") or []
        if ents:
            counts[ents[0]] = counts.get(ents[0], 0) + 1
    return {va: n for va, n in counts.items() if n > 1}


def main():
    bindir = sys.argv[1] if len(sys.argv) > 1 else "bin/GAME.EXE"
    outdir = sys.argv[2] if len(sys.argv) > 2 else "out"
    srcdir = sys.argv[3] if len(sys.argv) > 3 else "src/include"

    bin_ = Bin(bindir)
    manifest = json.load(open(os.path.join(outdir, "class_manifest.json")))
    shared_slot0 = shared_slot0_map(manifest)
    direct = json.load(open(os.path.join(outdir, "direct_bases.json")))
    fns = {f["va"]: f for f in json.load(open(os.path.join(outdir, "functions.json")))["functions"]}
    members_path = os.path.join(outdir, "members.json")
    members = json.load(open(members_path)) if os.path.exists(members_path) else {}
    mn_path = os.path.join(outdir, "member_names.json")
    member_names = json.load(open(mn_path)) if os.path.exists(mn_path) else {}
    # Access-width evidence: re-read each constructor's machine code to learn
    # how wide the store at each recovered displacement is, so a member is
    # declared at its real width instead of always as a 4-byte `unsigned long`.
    # See StoreWidths.
    store_widths = StoreWidths(bin_, sorted(fns))
    widths_by_class = {}
    for _m in manifest:
        _entry = members.get(_m["class"])
        if _entry:
            widths_by_class[_m["class"]] = store_widths.width_map(
                _m["vtable"], _entry.get("ctor"))
    vn_path = os.path.join(outdir, "virtual_names.json")
    # Fail loudly rather than silently defaulting to {}. The silent default is
    # exactly what hid the fact that no pipeline stage produced this file: a
    # clean regeneration emitted vfunc_N for all 1,213 recoverable names and
    # nothing complained.
    if not os.path.exists(vn_path):
        sys.stderr.write(
            "ERROR: %s is missing.\n"
            "       It is produced by tools/name_virtuals.py (pipeline stage "
            "8b).\n"
            "       Refusing to emit headers with no virtual names.\n" % vn_path)
        sys.exit(1)
    virtual_names = json.load(open(vn_path))
    sg_path = os.path.join(outdir, "signatures.json")
    signatures = json.load(open(sg_path)) if os.path.exists(sg_path) else {}

    # demangled class name -> ident, for resolving base includes
    ident_of = {m["class"]: m["ident"] for m in manifest}
    # ident -> authoritative RTTI name. The RTTI name is the correct C++
    # spelling of the type, so it is used verbatim for base clauses instead of
    # trying to un-mangle the ident.
    rtti_by_ident = {m["ident"]: m["class"] for m in manifest}
    # Nested types: "class INIClass::INISection" -> INISection qualifies as
    # INIClass::INISection. MSVC's RTTI drops the qualification inside template
    # arguments, so it has to be restored when spelling a base.
    nested = {}
    for m in manifest:
        c = m["class"]
        for kw in ("class ", "struct "):
            if c.startswith(kw):
                c = c[len(kw):]
        if "::" in c and "<" not in c:
            nested[c.split("::")[-1]] = c
    for m in manifest:
        ident_of.setdefault(sanitize(m["class"]), m["ident"])

    # Types supplied by ts_prelude.h are valid bases but have no header of
    # their own; map them so inheritance resolves instead of being dropped.
    prelude_types = {
        "IUnknown", "IPersist", "IPersistStream", "ISequentialStream", "IStream",
        "ILinkStream", "ISwizzle", "IApplication", "IGameMap", "IFlyControl",
        "IClassFactory", "IConnectionPoint", "IConnectionPointContainer",
        "IEnumConnectionPoints", "IEnumConnections", "IRTTITypeInfo",
        "IPiggyback", "ILocomotion", "IHouse", "IPublicHouse",
        "BounceClass", "DiskSwap", "FlasherClass", "StageClass",
        "CDownloadEventSink", "CNetUtilEventSink",
        "WOL_IChatEvent", "WOL_IDownloadEvent", "WOL_INetUtilEvent",
        "ATL_CComObjectRootBase",
        "ATL_CComObjectRootEx_class_CComMultiThreadModel_TL",
    }
    for t in prelude_types:
        ident_of.setdefault(t, t)

    # Sanitized ident -> real C++ spelling for namespaced prelude types.
    cpp_name = {
        "WOL_IChatEvent": "WOL::IChatEvent",
        "WOL_IDownloadEvent": "WOL::IDownloadEvent",
        "WOL_INetUtilEvent": "WOL::INetUtilEvent",
        "ATL_CComObjectRootBase": "ATL::CComObjectRootBase",
        "ATL_CComObjectRootEx_class_CComMultiThreadModel_TL":
            "ATL::CComObjectRootEx<ATL::CComMultiThreadModel, void>",
    }

    # Classes already defined by hand-written headers must not be regenerated,
    # or the two definitions collide. Hand-written headers are richer (named
    # virtuals, member offsets), so they win.
    # Derive ownership from src/handwritten/ directly, so the pipeline works
    # from a clean checkout without depending on a stale out/ artifact.
    owned = set()
    hw_dir = os.path.join(os.path.dirname(srcdir.rstrip("/")), "handwritten")
    if not os.path.isdir(hw_dir):
        hw_dir = "src/handwritten"
    if os.path.isdir(hw_dir):
        for hp in sorted(glob.glob(os.path.join(hw_dir, "*.h"))):
            hs = open(hp).read()
            for hm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    hs, re.M):
                owned.add(hm.group(1))
    # Fall back to the recorded artifact if the directory is absent.
    if not owned:
        owned_path = os.path.join(outdir, "handwritten_types.json")
        if os.path.exists(owned_path):
            owned = set(json.load(open(owned_path)))

    # Types that ts_prelude.h defines are owned by the prelude, exactly as if a
    # hand-written header declared them. Without this, a class that lives only
    # in the prelude (GScreenClass) gets its own generated header and the
    # combined TU fails with a redefinition.
    #
    # The prelude cannot simply be read here: pipeline stage 0 wipes
    # src/include/ and restores only src/handwritten/, while ts_prelude.h is
    # produced later by stage 10 (reconcile_prelude.py). So the set is derived
    # from the two authoritative sources instead -- the hand-written prelude if
    # present, plus reconcile_prelude.py's PRELUDE_ONLY, which is the list that
    # will actually define these types.
    for cand in (os.path.join(srcdir, "ts_prelude.h"),
                 os.path.join(hw_dir, "ts_prelude.h")):
        if os.path.exists(cand):
            ps = open(cand).read()
            for pm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    ps, re.M):
                owned.add(pm.group(1))
            break
    rp = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                      "reconcile_prelude.py")
    if os.path.exists(rp):
        rs = open(rp).read()
        m = re.search(r"PRELUDE_ONLY\s*=\s*\{(.*?)\n\}", rs, re.S)
        if m:
            for pm in re.finditer(r'"([A-Za-z_]\w*)"\s*:', m.group(1)):
                owned.add(pm.group(1))

    # Identifiers the prelude will define; used to keep them out of the
    # deferred-member list, since the prelude is regenerated later.
    #
    # prelude_owned must be initialised HERE, not further down: it is appended
    # to from inside the per-class loop below, and leaving the assignment later
    # raised `UnboundLocalError: prelude_owned` whenever gen_headers.py was run
    # with an already-reconciled src/include/ts_prelude.h present (i.e. outside
    # pipeline stage order, which is exactly how a human re-runs it).
    prelude_owned = []
    prelude_owned_types = set()
    prelude_defines = set()
    for cand in (os.path.join(srcdir, "ts_prelude.h"),
                 os.path.join(hw_dir, "ts_prelude.h")):
        if os.path.exists(cand):
            _ps = open(cand).read()
            for pm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    _ps, re.M):
                prelude_owned_types.add(pm.group(1))
            # Only a definition makes the prelude the owner of a type; a
            # forward declaration does not.
            for pm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)"
                    r"\s*(?::[^{;]*)?\{", _ps, re.M):
                prelude_defines.add(pm.group(1))
            break
    if os.path.exists(rp):
        _rs = open(rp).read()
        _m = re.search(r"PRELUDE_ONLY\s*=\s*\{(.*?)\n\}", _rs, re.S)
        if _m:
            for pm in re.finditer(r'"([A-Za-z_]\w*)"\s*:', _m.group(1)):
                prelude_owned_types.add(pm.group(1))

    # Template name -> the header that actually defines it. Emitting
    # #include "<template>.h" is wrong when the template lives in a
    # differently-named file (DynamicVectorClass is in VectorClass.h, Node is
    # in INIClass.h); the include must name the defining file.
    template_header = {
        "VectorClass": "VectorClass.h",
        "DynamicVectorClass": "VectorClass.h",
        "TypeList": "VectorClass.h",
        "Node": "INIClass.h",
    }
    if os.path.isdir(hw_dir):
        for hp in sorted(glob.glob(os.path.join(hw_dir, "*.h"))):
            hs = open(hp).read()
            for hm in re.finditer(
                    r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+(\w+)",
                    hs, re.M):
                template_header.setdefault(hm.group(1),
                                           os.path.basename(hp))
    # Templates whose formal parameters have no evidence-backed concrete type.
    # MSVC leaks the formal parameter name into RTTI for instantiations that
    # occur inside the template body, so e.g. "VectorClass<H>" has no real H.
    # Such an argument must be forward-declared, not spelled as a template
    # argument of an unknown type.
    for hp in (sorted(glob.glob(os.path.join(hw_dir, "*.h"))) if os.path.isdir(hw_dir) else []):
        hs = open(hp).read()
        for hm in re.finditer(
                r"^(?:template\s*<([^>]*)>)\s*(?:class|struct)\s+(\w+)", hs, re.M):
            for _fp in re.findall(r"[A-Za-z_]\w*", hm.group(1)):
                template_header.setdefault(_fp, None)


    # RTTI class name -> the header file that DEFINES it. Nested classes and
    # template instantiations have no standalone generated header: their
    # recovered member offsets still have to be emitted, so the block is
    # appended to the header that owns the definition instead of being dropped.
    # Ownership is derived from the definitions that actually exist in the tree
    # (hand-written headers are copied into srcdir by the pipeline before this
    # stage runs), so this works from a clean checkout.
    # Header basename -> the class names it DEFINES (not forward-declares).
    # Hand-written headers are copied verbatim into srcdir by the pipeline, so a
    # block appended to a hand-written header's srcdir copy is discarded on the
    # next run. The authoritative file for a hand-written header is the one in
    # src/handwritten/, and that is where such a block has to be written for
    # the change to survive regeneration.
    owner_header = {}
    hw_names = set()
    if os.path.isdir(hw_dir):
        hw_names = {os.path.basename(p) for p in glob.glob(os.path.join(hw_dir, "*.h"))}
    # Only a real definition counts. A forward declaration (`class X;`) must not
    # claim ownership: BrainClass.h says `class NeuronClass;` but never defines
    # it, and TechnoClass.h -- not LocomotionClass.h -- defines TechnoClass.
    _def_re = re.compile(
        r"^[ \t]*(?:template\s*<[^;{}]*>\s*)?(?:class|struct)\s+(\w+)"
        r"(?:\s*:[^;{}]*)?\s*\{", re.M)
    for _hp in sorted(glob.glob(os.path.join(srcdir, "*.h"))):
        _base = os.path.basename(_hp)
        _hs = open(_hp).read()
        for _hm in _def_re.finditer(_hs):
            owner_header.setdefault(_hm.group(1), _base)

    def owning_header(rtti_name):
        """(writable path, class name as the header spells it) or None.

        The owner is the header that DEFINES the class: for a template
        instantiation that is the header defining its template
        (BlitPlain<E> -> Blitter.h); for a nested class the header defining its
        enclosing class (SidebarClass::StripClass -> SidebarClass.h). Returns
        None when no header defines the class anywhere.

        The path is redirected into src/handwritten/ when the owning header is
        hand-written, because the pipeline restores that directory over
        src/include/ after this stage runs.
        """
        n = rtti_name
        for _kw in ("class ", "struct "):
            if n.startswith(_kw):
                n = n[len(_kw):]
        n = n.strip()
        base = None
        if n in owner_header:
            base = owner_header[n]
        elif "<" in n and n.split("<")[0].strip() in owner_header:
            n = n.split("<")[0].strip()
            base = owner_header[n]
        elif "::" in n:
            # Prefer the full nested spelling, then the enclosing class, then
            # the simple nested name (the header may write either form).
            if n in owner_header:
                base = owner_header[n]
            else:
                for _cand in (n.split("::")[0].strip(), n.split("::")[-1].strip()):
                    if _cand in owner_header:
                        base = owner_header[_cand]
                        n = _cand
                        break
        if base is None:
            _ident = ident_of.get(rtti_name)
            if _ident and os.path.exists(os.path.join(srcdir, _ident + ".h")):
                base = _ident + ".h"
                n = _ident
        if base is None:
            return None
        if base in hw_names:
            return os.path.join(hw_dir, base), n
        return os.path.join(srcdir, base), n

    # Appended after the per-class loop: (header basename, rtti class, lines).
    # A class the loop skips never enters the file-writing branch, so its
    # recovered offsets would otherwise be computed and thrown away.
    deferred_members = []

    # The base list the per-class loop below will actually EMIT, per class. The
    # layout model has to model the declaration the compiler sees, and a base
    # the loop cannot resolve is a base the declaration does not name.
    emitted_bases = {}
    for _m in manifest:
        _bl = []
        for _b in direct.get(_m["class"], []):
            _bi = ident_of.get(_b) or ident_of.get(sanitize(_b))
            if _bi and _bi != _m["ident"] and _bi not in _bl:
                _bl.append(_bi)
        # A base the manifest does not know (StageClass, the COM interfaces)
        # still has a declaration the emitter names -- and its layout still
        # contributes to every derived class. Keep its own name as the key.
        emitted_bases[_m["class"]] = [rtti_by_ident.get(i, i) for i in _bl]
    body_owners = {}
    for _m in manifest:
        _h = owning_header(_m["class"])
        if _h:
            body_owners.setdefault(_h, []).append(_m["class"])
    layout = DeclaredLayout(srcdir, hw_dir, manifest, direct, members,
                            widths_by_class, ident_of, rtti_by_ident, owned,
                            emitted_bases, owning_header, prelude_owned_types,
                            owner_header, hw_names, body_owners)
    if os.environ.get("GEN_HEADERS_DEBUG"):
        # GEN_HEADERS_DEBUG="class TechnoClass" prints what the layout model
        # decided for a class and why, which is the only way to read the
        # cursor back out of the emitter without compiling the header.
        for _c in os.environ["GEN_HEADERS_DEBUG"].split(","):
            print("layout model %s: bases=%s cursor=0x%x end=0x%x (%s)" % (
                _c, layout._declared_bases(_c), layout.cursor(_c),
                layout.end(_c), layout.base_note(_c)), file=sys.stderr)
    os.makedirs(srcdir, exist_ok=True)
    written = 0
    skipped = 0
    written_idents = set()
    unresolved_bases = defaultdict(set)
    for m in manifest:
        ident = m["ident"]
        if not ident:
            continue
        if ident in owned:
            skipped += 1
            # Hand-written headers own this class. They already document their
            # members, so only append when the header does NOT yet emit the
            # recovered offsets -- otherwise a class whose hand-written header
            # is silent about layout would lose its evidence a second time.
            #
            # Types owned by ts_prelude.h are the exception: the prelude is
            # regenerated by reconcile_prelude.py AFTER this stage, so a block
            # appended to it would be discarded, and a standalone header would
            # define the type twice. Report and skip them.
            if ident in prelude_owned_types:
                prelude_owned.append(m["class"])
                continue
            deferred_members.append(("handwritten", m, ident))
            continue
        if ident in written_idents:
            # Two manifest entries can share one ident (a template head plus a
            # leaked-formal-parameter instantiation). The file already exists;
            # append its offsets there instead of overwriting.
            deferred_members.append(("dup", m, ident))
            continue
        # A template instantiation whose template is defined by a hand-written
        # header is already defined by that header; skip the generated
        # duplicate. This is decided from the RTTI name, not the mangled ident.
        _rtti = m.get("class", "")
        if "<" in _rtti:
            _tmpl = _rtti.split("<")[0]
            for _kw in ("class ", "struct "):
                if _tmpl.startswith(_kw):
                    _tmpl = _tmpl[len(_kw):]
            if _tmpl.strip() in owned:
                skipped += 1
                deferred_members.append(("tmpl", m, ident))
        # A template instantiation whose template is defined by a hand-written
        # header is already defined by that header; skip the generated
        # duplicate. This is decided from the RTTI name, not the mangled ident.
        _rtti = m.get("class", "")
        if "<" in _rtti:
            _tmpl = _rtti.split("<")[0]
            for _kw in ("class ", "struct "):
                if _tmpl.startswith(_kw):
                    _tmpl = _tmpl[len(_kw):]
            if _tmpl.strip() in owned:
                skipped += 1
                deferred_members.append(("tmpl", m, ident))
                continue

        # direct bases only, resolved to idents, deduplicated
        bases = []
        seen_bases = set()
        for b in direct.get(m["class"], []):
            bi = ident_of.get(b) or ident_of.get(sanitize(b))
            if bi and bi != ident and bi not in seen_bases:
                seen_bases.add(bi)
                bases.append(bi)
            elif not bi:
                unresolved_bases[m["class"]].add(b)

        L = []
        L.append("#pragma once")
        L.append("// " + "=" * 74)
        L.append(f"// {ident}.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)")
        L.append("//")
        L.append(f"// RTTI class   : {m['class']}")
        L.append(f"// vtable       : 0x{m['vtable']:08x}  ({m['nvirtuals']} slots)")
        if m.get("ctors"):
            L.append(f"// constructor  : 0x{m['ctors'][0]:08x}")
        if m.get("ctor_sites"):
            L.append(f"// vtable store : 0x{m['ctor_sites'][0]:08x}  (inside the ctor)")
        L.append("//")
        L.append("// Virtual slots are in real vtable order; each carries its target")
        L.append("// address as evidence. Slots named vfunc_N have no established")
        L.append("// semantics yet.")
        L.append("// " + "=" * 74)
        L.append("")
        L.append('#include "ts_prelude.h"')
        for b in bases:
            # A type ts_prelude.h only FORWARD-declares (StageClass) needs its
            # own header included: the prelude is not a definition, and a base
            # class has to be complete. A type the prelude does define is left
            # alone, or the two definitions collide.
            if b in prelude_types and b not in prelude_defines:
                continue
            if os.path.exists(os.path.join(srcdir, b + ".h")):
                L.append(f'#include "{b}.h"')
                continue
            # A base that is an instantiation of a hand-written template
            # (Node<...>, VectorClass<...>, BlitPlain<...>): include the
            # template's header, since no header exists for the instantiation.
            _bt = rtti_by_ident.get(b, "")
            if "<" in _bt:
                _t = _bt.split("<")[0]
                for _kw in ("class ", "struct "):
                    if _t.startswith(_kw):
                        _t = _t[len(_kw):]
                _t = _t.strip()
                _hdr = template_header.get(_t)
                if _hdr:
                    L.append(f'#include "{_hdr}"')
                # The base's template ARGUMENTS must also be complete types:
                # DynamicVectorClass<ObjectClass*> needs ObjectClass.h. Include
                # the header of every argument that has one.
                for _at in re.findall(r"[A-Za-z_]\w*", _bt[_bt.index("<"):]):
                    if _at in _KEYWORDS:
                        continue
                    if os.path.exists(os.path.join(srcdir, _at + ".h")):
                        L.append(f'#include "{_at}.h"')
                    elif template_header.get(_at):
                        L.append(f'#include "{template_header[_at]}"')
        # A template instantiation needs its template arguments declared.
        # Derive them from the RTTI name, e.g.
        #   class DynamicVectorClass<AbstractClass*>  ->  AbstractClass.h
        rtti = m.get("class", "")
        if "<" in rtti:
            tail = rtti[rtti.index("<"):]
            seen = set()
            for tok in re.findall(r"[A-Za-z_]\w*", tail):
                if tok in _KEYWORDS or tok in seen:
                    continue
                seen.add(tok)
                if os.path.exists(os.path.join(srcdir, tok + ".h")):
                    L.append(f'#include "{tok}.h"')
                elif template_header.get(tok):
                    # The argument is itself defined by a hand-written template
                    # header (e.g. an instantiation nested in another template).
                    L.append(f'#include "{template_header[tok]}"')
                elif tok not in owned and tok not in prelude_types:
                    # The argument has no header of its own (MSVC emits the
                    # formal parameter name for instantiations inside a
                    # template, e.g. DynamicVectorClass<D>). Forward-declare it
                    # so the template-id is well-formed.
                    L.append(f"class {tok};")
        L.append("")

        if bases:
            # Base clauses may mention a template argument that is only a leaked
            # MSVC formal parameter (CounterClass : public VectorClass<H>). Such
            # an argument has no evidence-backed type, so forward-declare it
            # rather than inventing one; this keeps the template-id well-formed
            # while preserving the recovered inheritance edge.
            _declared = set()
            for _b in bases:
                _bt = rtti_by_ident.get(_b, "")
                if "<" not in _bt:
                    continue
                for _tok in re.findall(r"[A-Za-z_]\w*", _bt[_bt.index("<"):]):
                    if _tok in _KEYWORDS or _tok in _declared:
                        continue
                    _declared.add(_tok)
                    if os.path.exists(os.path.join(srcdir, _tok + ".h")):
                        continue
                    if _tok in owned or _tok in prelude_types:
                        continue
                    if template_header.get(_tok):
                        continue
                    L.append(f"class {_tok};  // RTTI formal param, no type evidence")
            spelled = []
            for b in bases:
                if b in cpp_name:
                    spelled.append(cpp_name[b])
                    continue
                # Use the authoritative RTTI spelling of the base. MSVC drops
                # the qualification of nested types inside template arguments,
                # so restore it (Node<INISection*, INIClass> must be written
                # Node<INIClass::INISection, INIClass>).
                r = rtti_by_ident.get(b)
                if not r:
                    spelled.append(b)
                    continue
                for _kw in ("class ", "struct "):
                    if r.startswith(_kw):
                        r = r[len(_kw):]
                r = r.strip()
                for _simple, _qual in nested.items():
                    r = re.sub(r"\b" + re.escape(_simple) + r"\b", _qual, r)
                spelled.append(r)
            L.append(f"class {ident} : public " + ", public ".join(spelled))
        else:
            L.append(f"class {ident}")
        L.append("{")
        L.append("public:")
        L.append(f"    // ---- vtable 0x{m['vtable']:08x} ({m['nvirtuals']} slots) ----")
        vnames = virtual_names.get(m["class"]) or virtual_names.get(m["ident"]) or {}
        for i, e in enumerate(m["entries"]):
            fn = fns.get(e)
            tag = ""
            if fn and fn.get("strings"):
                s = fn["strings"][0]["text"]
                if re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]{2,30}", s):
                    tag = f'  // refs "{s}"'
            vn = vnames.get(str(i))
            sg = signatures.get(hex(e), {})
            cc = sg.get("cc")
            na = sg.get("nargs")
            # build an honest signature: only state what was recovered
            if na is not None and na > 0:
                args = ", ".join(f"int a{k}" for k in range(na))
            elif na == 0:
                args = ""
            else:
                args = ""
            # Return type is recorded as a comment, not in the declaration:
            # the heuristic can disagree with a base class and break overrides.
            ret = "void"
            bits = []
            if cc:
                bits.append(cc)
            if sg.get("ret"):
                bits.append(f"returns {sg['ret']}")
            cctag = ("  // " + ", ".join(bits)) if bits else ""
            if vn and vn.startswith("~"):
                # A destructor named in slot 0 is normally the destructor, and
                # the declaration below covers it, so the slot is skipped.
                #
                # But slot 0 is a SHARED STUB for 66 of the 749 classes -- they
                # all point at 0x405790 -- and a shared address is not 66
                # destructors. Skipping it there silently threw the slot away
                # and left `virtual ~X()` claiming a vtable position it does
                # not hold. Emit the slot under its own name instead.
                if e not in shared_slot0:
                    continue
            elif vn:
                L.append(f"    virtual {ret} {vn}({args});  // [{i}] 0x{e:08x}{cctag}{tag}")
            else:
                L.append(f"    virtual {ret} vfunc_{i}({args});  // [{i}] 0x{e:08x}{cctag}{tag}")
        L.append("")
        if m["ctors"]:
            L.append(f"    {ident}();  // 0x{m['ctors'][0]:08x}")
        # The destructor's address is recorded in the manifest's ctor/entry
        # data, not as a slot, so it is declared without claiming one. Where
        # slot 0 is a shared stub, say so, because `virtual ~X()` on its own
        # reads as "this is slot 0" and here that is not true.
        if m["entries"] and m["entries"][0] in shared_slot0:
            L.append("    // The destructor is declared non-virtual here: slot 0 "
                     "holds a stub")
            L.append("    // shared with %d other classes (0x%08x), not the "
                     "destructor." %
                     (shared_slot0[m["entries"][0]], m["entries"][0]))
            L.append(f"    ~{ident}();")
        else:
            L.append(f"    virtual ~{ident}();")

        # ---- data members recovered from the constructor ----
        L.extend(member_block(m["class"], members, member_names,
                              widths_by_class.get(m["class"]) or {},
                              cursor=layout.cursor(m["class"]),
                              base_cursor=layout.cursor(m["class"]),
                              note=layout.base_note(m["class"])))
        L.append("};")
        L.append("")

        with open(os.path.join(srcdir, ident + ".h"), "w") as f:
            f.write("\n".join(L))
        written += 1
        written_idents.add(ident)
    # ---- append member blocks for classes with no standalone header ----
    #
    # The per-class loop above only reaches the member emission for classes it
    # writes a file for. Nested classes and template instantiations have no
    # such file: the offsets recovered for them were computed and discarded.
    # Append each block to the header that DEFINES the class instead. A class
    # with no owning header anywhere gets a new one, so nothing is dropped
    # silently.
    append_failed = []
    appended_classes = 0
    appended_records = 0
    skipped_already = 0
    # prelude_owned is initialised earlier (it is appended to above); do not
    # re-bind it here, or those appends are discarded.
    orphan_headers = {}
    curated_padded = [0]
    curated_duplicated = [0]
    seen_blocks = set()
    # Work on a per-file in-memory copy so several classes owned by the same
    # header are appended in one deterministic pass instead of racing writes.
    file_cache = {}
    for _why, m, ident in deferred_members:
        hdr = owning_header(m["class"])
        if hdr is None:
            # Before creating a new file, check whether some header ALREADY
            # defines this class (the prelude, or a header that declares
            # several related types). Creating a standalone header for a type
            # that is already defined elsewhere makes the combined TU fail
            # with a redefinition -- which is exactly what happened to
            # GScreenClass, whose definition lives in ts_prelude.h.
            simple_n = m["class"]
            for _kw in ("class ", "struct "):
                if simple_n.startswith(_kw):
                    simple_n = simple_n[len(_kw):]
            simple_n = simple_n.strip()
            if simple_n in prelude_owned_types:
                prelude_owned.append(m["class"])
                continue
            _defined_elsewhere = False
            for _hp in sorted(glob.glob(os.path.join(srcdir, "*.h"))):
                _hs = open(_hp).read()
                if re.search(r"^(?:template\s*<[^>]*>\s*)?(?:class|struct)\s+"
                             + re.escape(simple_n) + r"\b", _hs, re.M):
                    _defined_elsewhere = True
                    break
            if _defined_elsewhere:
                skipped_already += 1
                continue
            # No header defines this class anywhere. Emit a standalone header
            # for it and say so in a comment, rather than dropping the offsets.
            # The header is written immediately so a second manifest entry for
            # the same class appends to it instead of failing the existence
            # check below.
            key = ident + ".h"
            # The orphan header declares the class with no base and no virtual,
            # so the compiler's cursor for its first member is 0 -- not the
            # class's recovered base region. The pads below still put every
            # recovered member at its recovered offset.
            block = member_block(m["class"], members, member_names,
                                 widths_by_class.get(m["class"]) or {},
                                 cursor=0, base_cursor=0,
                                 note="orphan header: the class is declared "
                                      "with no base here")
            if not block:
                continue
            if key not in orphan_headers:
                simple = m["class"]
                for _kw in ("class ", "struct "):
                    if simple.startswith(_kw):
                        simple = simple[len(_kw):]
                simple = simple.strip()
                lines = [
                    "#pragma once",
                    "// " + "=" * 74,
                    f"// {key}  —  recovered member offsets with no owning header",
                    "//",
                    f"// RTTI class : {m['class']}",
                    "//",
                    "// No reconstructed header defines this class: it has no",
                    "// standalone generated file, and no enclosing-class or",
                    "// template header could be found for it. The ctor-store",
                    "// offsets recovered for it are emitted here so the evidence",
                    "// is not dropped again. See gen_headers.py owning_header().",
                    "// " + "=" * 74,
                    "",
                    f"class {simple}",
                    "{",
                    "public:",
                ]
                lines.extend(block)
                lines.append("};")
                lines.append("")
                open(os.path.join(srcdir, key), "w").write("\n".join(lines))
                orphan_headers[key] = simple
                owner_header[simple] = key
                owner_header[ident] = key
                appended_classes += 1
                appended_records += len(block)
            continue
        path, simple = hdr
        if (path, simple) in seen_blocks:
            continue
        seen_blocks.add((path, simple))
        if os.path.basename(path) == "ts_prelude.h":
            # The prelude is regenerated by reconcile_prelude.py and its
            # PRELUDE_ONLY types (GScreenClass, IUnknown) are redefined there
            # unconditionally, so a block appended to the prelude would be
            # discarded and a separate header would define the type twice.
            # Report it instead of dropping it silently.
            prelude_owned.append(m["class"])
            continue
        if not os.path.exists(path):
            append_failed.append(f"{m['class']} -> {path} (missing)")
            continue
        src = file_cache.get(path)
        if src is None:
            src = open(path).read()
        # Idempotent and non-destructive: a body that already documents its
        # members -- the curated hand-written blocks in VectorClass.h and
        # Pipe.h, or a block appended by an earlier run -- keeps its
        # authoritative layout. Appending a second, raw block would give the
        # class two disagreeing layout claims.
        span = class_body_span(src, simple)
        if span is None:
            append_failed.append(f"{m['class']} -> {os.path.basename(path)} "
                                 f"(no class {simple} in file)")
            continue
        body = src[span[0]:span[1]]
        # NOTE: do NOT skip when a banner is already present. A block written
        # by a previous run must be REPLACED, not kept: when the recovery
        # improves and stops producing an offset, the stale declaration would
        # otherwise survive and verify.py check 7 would (correctly) report an
        # emitted offset with no recovered entry. insert_member_block excises
        # the old block first, so re-running is idempotent.
        # The curated block a hand-written header already carries WINS. It is
        # the richer, evidence-backed source, and this machine block is appended
        # after it -- so a second declaration at the same displacement would
        # displace everything that follows. member_block() drops those offsets
        # by DISPLACEMENT and names them in a comment. An earlier revision
        # dropped them by IDENTIFIER, which never matched (the curated member is
        # `m_ID`, the machine one `m_008`), so every hand-written class carried
        # two conflicting declarations and the base sub-objects they inflate
        # displaced every derived class's members.
        # First make the curated block sit where its own comments say it does.
        # The compiler lays declarations out in order, so a block with a gap in
        # it lands short of its documented offsets, and every derived class's
        # base sub-object is that much too small.
        _orig = src
        src, _npads, _ndups, _unfix = apply_curated_pads(
            src, simple, layout, m["class"])
        if _npads or _ndups:
            curated_padded[0] += _npads
            curated_duplicated[0] += _ndups
        _cursor = max(layout.cursor(m["class"]), layout.curated_end(m["class"]))
        block = member_block(m["class"], members, member_names,
                             widths_by_class.get(m["class"]) or {},
                             cursor=_cursor,
                             base_cursor=layout.cursor(m["class"]),
                             curated=layout.curated_spans(m["class"]),
                             curated_names=layout.curated_names(m["class"]),
                             note=layout.base_note(m["class"]),
                             drift=layout.curated_drift(m["class"]))
        if not block:
            continue
        new = insert_member_block(src, simple, block)
        if new is None or new == _orig:
            continue
        file_cache[path] = new
        appended_classes += 1
        appended_records += len(block)
    for path, src in sorted(file_cache.items()):
        open(path, "w").write(src)
    # Orphan headers are written as they are created (see above), so nothing to
    # flush here; only their names are reported.
    print(f"member blocks appended to owning headers: {appended_classes} "
          f"classes")
    if curated_padded[0] or curated_duplicated[0]:
        print(f"curated blocks corrected to their documented offsets: "
              f"{curated_padded[0]} pad(s) inserted, "
              f"{curated_duplicated[0]} declaration(s) commented out as "
              f"duplicates of a slot C++ supplies")
    if prelude_owned:
        print(f"member blocks owned by ts_prelude.h (not appended): "
              f"{len(prelude_owned)} ({', '.join(sorted(prelude_owned))})")
        print(f"member blocks skipped (already documented): {skipped_already} "
              f"classes")
    if orphan_headers:
        print(f"new headers for classes with no owner: {len(orphan_headers)} "
              f"({', '.join(sorted(orphan_headers))})")
    if append_failed:
        print(f"member blocks NOT appended: {len(append_failed)}")
        for e in append_failed[:10]:
            print(f"  {e}")

    print(f"headers written: {written}")
    print(f"skipped (hand-written owns): {skipped}")
    if unresolved_bases:
        print(f"classes with unresolved bases: {len(unresolved_bases)}")
        for k, v in list(unresolved_bases.items())[:10]:
            print(f"  {k}: {sorted(v)}")


if __name__ == "__main__":
    main()
