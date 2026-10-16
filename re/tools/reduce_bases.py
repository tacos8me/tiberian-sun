#!/usr/bin/env python3
"""
Reduce RTTI base-class arrays to C++ direct bases.

RTTI lists every ancestor transitively. C++ must name only DIRECT bases, so a
base B is dropped from class C when some other base of C already inherits B.

Two things this has to get right, both of which the previous one-level version
got wrong:

1. Ancestry must be followed TRANSITIVELY. Dropping B only when a direct base
   of C lists B is insufficient: ATL::CComObjectRootBase sits two levels below
   CChatEventSink (via CComObjectRootEx), so a one-level check leaves it in the
   list and C++ then inherits it twice. That is not merely cosmetic -- it means
   the emitted layout does not match the binary.

2. Ancestry must be resolved for bases that have no top-level entry. Many
   ATL/COM and pure-interface bases (CComObjectRootBase, IUnknown,
   ISequentialStream) appear ONLY as base references, because they have no
   vtable of their own and so are never emitted as a class. A lookup keyed on
   top-level classes returns None for every one of them, which silently
   disabled the whole reduction for those hierarchies.

Output: out/direct_bases.json  { class -> [direct base names] }
"""
import json
import os
import sys
from collections import defaultdict


def build_graph(classes):
    """name -> set of names it inherits, over the whole RTTI graph.

    Two edge sources, because neither alone is complete:

    * Base-class ARRAYS. RTTI stores a class's full ancestor list as a
      BaseClassArray whose entries carry `numContainedBases`, the count of
      ancestors each entry itself has. A non-zero count means the next entries
      in the array belong to it -- that is how the array encodes the tree. So
      for a class whose base list is [B0(ncb=4), B1(ncb=1), B2(ncb=0), ...],
      B2 is an ancestor of B1 and B1..B4 are ancestors of the class.

    * Any class that has its own top-level entry contributes its listed bases
      directly.

    Crucially this does NOT require a top-level entry for every base: 33
    spellings here (CComObjectRootBase, IUnknown, IStream, ...) appear only as
    base references, because they have no vtable and so are never emitted as
    classes. A lookup keyed on top-level classes returns None for all of them,
    which silently disabled reduction for exactly the ATL/COM hierarchies.
    """
    g = defaultdict(set)
    for e in classes:
        me = e.get("demangled")
        if not me:
            continue
        bases = e.get("bases", [])
        # bases[0] is the class itself in this binary's encoding; the rest are
        # its ancestors in hierarchy order.
        rest = [b for b in bases if b.get("demangled") != me]
        # The array is a pre-order walk: an entry with numContainedBases = k
        # owns the next k entries as ITS ancestors.
        i = 0
        while i < len(rest):
            ncb = rest[i].get("numContainedBases") or 0
            owner = rest[i].get("demangled")
            for j in range(i + 1, min(i + 1 + ncb, len(rest))):
                child = rest[j].get("demangled")
                if owner and child and child != owner:
                    g[owner].add(child)
                if child:
                    g[me].add(child)
            if i == 0 and owner:
                g[me].add(owner)
            i += 1
    return g


def ancestors(name, g, memo, seen):
    """All transitive ancestors of `name`."""
    if name in memo:
        return memo[name]
    if name in seen:
        return set()  # cycle guard; MSVC RTTI should not produce these
    seen.add(name)
    out = set()
    for b in g.get(name, ()):
        out.add(b)
        out |= ancestors(b, g, memo, seen)
    seen.discard(name)
    memo[name] = out
    return out


def main():
    outdir = sys.argv[1] if len(sys.argv) > 1 else "out"
    path = os.path.join(outdir, "classes.json")
    d = json.load(open(path))

    g = build_graph(d["classes"])
    memo = {}

    direct = {}
    reduced_total = 0
    for e in d["classes"]:
        me = e.get("demangled")
        if not me:
            continue
        bases = [b["demangled"] for b in e.get("bases", [])
                 if b.get("demangled") and b["demangled"] != me]
        # A base is transitive -- and therefore not a direct base -- when any
        # OTHER base of this class is one of its ancestors.
        drop = set()
        for b in bases:
            for other in bases:
                if other == b:
                    continue
                if b in ancestors(other, g, memo, set()):
                    drop.add(b)
                    break
        kept = [b for b in bases if b not in drop]
        reduced_total += len(bases) - len(kept)
        direct[me] = kept

    with open(os.path.join(outdir, "direct_bases.json"), "w") as f:
        json.dump(direct, f, indent=1)

    multi = sum(1 for v in direct.values() if len(v) > 1)
    print(f"classes: {len(direct)}")
    print(f"with >1 direct base: {multi}")
    print(f"transitive bases removed: {reduced_total}")
    print(f"wrote {outdir}/direct_bases.json")


if __name__ == "__main__":
    main()
