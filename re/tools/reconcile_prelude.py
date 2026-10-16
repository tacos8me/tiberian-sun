#!/usr/bin/env python3
"""
Reconcile ts_prelude.h against the reconstructed headers.

The prelude must define only types that no reconstructed header defines.
Anything a real header owns is reduced to a forward declaration here, so the
two never collide. This runs after header generation and is idempotent.

Also restores the small set of prelude-only definitions that generated headers
depend on (IUnknown for the ATL/COM layer, GScreenClass for the UI layer).
"""
import glob
import os
import re
import sys

SRCDIR = sys.argv[1] if len(sys.argv) > 1 else "src/include"
PRELUDE = os.path.join(SRCDIR, "ts_prelude.h")

# Types the prelude must keep a full definition for, because generated headers
# use them as bases and no reconstructed header defines them.
PRELUDE_ONLY = {
    "IUnknown": """struct IUnknown {
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) = 0;
    virtual ULONG   __stdcall AddRef() = 0;
    virtual ULONG   __stdcall Release() = 0;
};""",
    "GScreenClass": """struct GScreenClass {
    virtual ~GScreenClass() {}
    virtual void _dummy_GScreen() {}
};""",
}


def brace_end(s, start):
    """Return the index just past the '};' closing the brace at/after start."""
    i = s.index("{", start)
    depth = 0
    j = i
    while j < len(s):
        if s[j] == "{":
            depth += 1
        elif s[j] == "}":
            depth -= 1
            if depth == 0:
                break
        j += 1
    end = s.index(";", j) + 1
    while end < len(s) and s[end] == "\n":
        end += 1
    return end


def main():
    if not os.path.exists(PRELUDE):
        print("no prelude found")
        return

    # Types defined by any reconstructed header.
    owned = set()
    for p in sorted(glob.glob(os.path.join(SRCDIR, "*.h"))):
        if os.path.basename(p) == "ts_prelude.h":
            continue
        s = open(p).read()
        for m in re.finditer(r"^(?:class|struct)\s+(\w+)", s, re.M):
            owned.add(m.group(1))

    s = open(PRELUDE).read()

    # 1. Reduce any prelude definition that a real header owns.
    reduced = []
    for t in sorted(owned):
        if t in PRELUDE_ONLY:
            continue
        m = re.search(r"^struct " + re.escape(t) + r"\b", s, re.M)
        if not m:
            continue
        head = s[m.start():m.start() + 400].split(";")[0]
        if "{" not in head:
            continue  # already a forward declaration
        end = brace_end(s, m.start())
        s = s[:m.start()] + f"struct {t};   // defined by its own reconstructed header\n" + s[end:]
        reduced.append(t)

    # 2. Ensure prelude-only types are fully defined.
    restored = []
    for t, body in PRELUDE_ONLY.items():
        m = re.search(r"^struct " + re.escape(t) + r"\b", s, re.M)
        if m:
            head = s[m.start():m.start() + 400].split(";")[0]
            if "{" in head:
                continue  # already defined
            end = s.index(";", m.start()) + 1
            while end < len(s) and s[end] == "\n":
                end += 1
            s = s[:m.start()] + body + "\n" + s[end:]
        else:
            s = body + "\n" + s
        restored.append(t)

    # 3. Node<T,Key> is defined by INIClass.h; drop any prelude definition.
    m = re.search(r"^template <class T, class Key>\nstruct Node \{", s, re.M)
    if m:
        end = brace_end(s, m.start())
        s = s[:m.start()] + "// Node<T,Key> is defined by INIClass.h.\n" + s[end:]
        reduced.append("Node")

    # 4. Hand-written headers that duplicate a prelude-only type must defer to
    #    the prelude instead of redefining it.
    for hdr, tname in [("AbstractClass.h", "IUnknown")]:
        hp = os.path.join(SRCDIR, hdr)
        if not os.path.exists(hp):
            continue
        hs = open(hp).read()
        m = re.search(r"^struct " + re.escape(tname) + r"\n\{", hs, re.M)
        if not m:
            continue
        end = brace_end(hs, m.start())
        hs = hs[:m.start()] + f"// {tname} is defined by ts_prelude.h (shared COM base).\n" + hs[end:]
        open(hp, "w").write(hs)
        print(f"  {hdr}: deferred {tname} to prelude")

    # 5. Headers that derive from a prelude-only type must include the prelude.
    for hdr in ("SidebarClass.h",):
        hp = os.path.join(SRCDIR, hdr)
        if not os.path.exists(hp):
            continue
        hs = open(hp).read()
        if '#include "PowerClass.h"' not in hs:
            hs = hs.replace('#include "RadarClass.h"',
                            '#include "RadarClass.h"\n#include "PowerClass.h"', 1)
            open(hp, "w").write(hs)
            print(f"  {hdr}: added PowerClass.h include")

    open(PRELUDE, "w").write(s)
    print(f"prelude reconciled: reduced={len(reduced)} restored={len(restored)}")
    if reduced:
        print("  reduced:", ", ".join(reduced))
    if restored:
        print("  restored:", ", ".join(restored))


if __name__ == "__main__":
    main()
