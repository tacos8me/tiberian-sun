#!/usr/bin/env python3
"""
Make every reconstructed header self-contained.

A header is "self-contained" if it compiles when included alone. Generated
headers already include their direct bases; hand-written headers sometimes rely
on include order. This script adds the missing includes so each header stands
alone, which is what makes the tree safe to edit.

Idempotent: running it twice changes nothing.
"""
import glob
import os
import re
import subprocess
import sys

SRCDIR = sys.argv[1] if len(sys.argv) > 1 else "src/include"
SHIM = sys.argv[2] if len(sys.argv) > 2 else "build/shim"

# Types that live in the prelude; a header referencing one needs the prelude.
PRELUDE_TYPES = [
    "IUnknown", "IPersist", "IPersistStream", "IStream", "ISequentialStream",
    "IRTTITypeInfo", "IPiggyback", "ILocomotion", "IHouse", "IPublicHouse",
    "IConnectionPointContainer", "IConnectionPoint", "IEnumConnectionPoints",
    "IEnumConnections", "IClassFactory", "IApplication", "IGameMap",
    "IFlyControl", "ISwizzle", "ILinkStream", "CoordStruct", "CellStruct",
    "GScreenClass", "PowerClass", "FlasherClass", "StageClass", "BounceClass",
    "DiskSwap", "CDownloadEventSink", "CNetUtilEventSink", "WOL::", "ATL::",
]


def compiles(path):
    r = subprocess.run(
        ["g++", "-std=c++03", "-fsyntax-only", f"-I{SRCDIR}", f"-I{SHIM}",
         "-include", path, "-x", "c++", "/dev/null"],
        capture_output=True)
    return r.returncode == 0, r.stderr.decode(errors="replace")


def add_include(src, inc):
    """Insert an include after the first existing include or pragma once."""
    if f'#include "{inc}"' in src:
        return src
    m = re.search(r"^#include .*$", src, re.M)
    if m:
        return src[:m.end()] + f'\n#include "{inc}"' + src[m.end():]
    m = re.search(r"^#pragma once$", src, re.M)
    if m:
        return src[:m.end()] + f'\n#include "{inc}"' + src[m.end():]
    return f'#include "{inc}"\n' + src


def main():
    headers = sorted(p for p in glob.glob(os.path.join(SRCDIR, "*.h"))
                     if os.path.basename(p) != "ts_prelude.h")
    fixed = 0
    for p in headers:
        ok, _ = compiles(p)
        if ok:
            continue
        src = open(p).read()
        changed = False

        # 1. prelude types -> include the prelude
        if '#include "ts_prelude.h"' not in src:
            if any(re.search(r"\b" + re.escape(t) + r"\b", src)
                   for t in PRELUDE_TYPES):
                src = add_include(src, "ts_prelude.h")
                changed = True

        # 2. missing base-class headers -> include them. Some types are
        #    defined inside another header rather than having their own file.
        TYPE_HOME = {
            "StageClass": "TechnoClass.h",
            "FlasherClass": "TechnoClass.h",
            "AbstractTypeClass": "ObjectTypeClass.h",
            "GenericNode": "INIClass.h",
        }
        for m in re.finditer(r"public\s+([A-Za-z_]\w*)", src):
            base = m.group(1)
            inc = TYPE_HOME.get(base, base + ".h")
            bp = os.path.join(SRCDIR, inc)
            if os.path.exists(bp) and f'#include "{inc}"' not in src:
                src = add_include(src, inc)
                changed = True

        if changed:
            open(p, "w").write(src)
            ok2, _ = compiles(p)
            if ok2:
                fixed += 1
            else:
                print(f"  still failing: {os.path.basename(p)}")

    print(f"headers made self-contained: {fixed}")

    # final report
    bad = []
    for p in headers:
        ok, _ = compiles(p)
        if not ok:
            bad.append(os.path.basename(p))
    print(f"standalone: {len(headers) - len(bad)}/{len(headers)} pass")
    if bad:
        print("failing:", ", ".join(bad))


if __name__ == "__main__":
    main()
