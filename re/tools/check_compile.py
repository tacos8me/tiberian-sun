#!/usr/bin/env python3
"""Compile a set of generated translation units standalone and fail if any does.

Used by the pipeline to prove the generated tree compiles. Split out of the
shell script so the check is a real script with a real exit code rather than an
inline heredoc, and so the exclusion of a concatenated TU is explicit.

Usage:
  python3 tools/check_compile.py <label> <glob> [--exclude <substring>]
"""

import glob
import subprocess
import sys


def main():
    if len(sys.argv) < 3:
        print("usage: check_compile.py <label> <glob> [--exclude <substring>]")
        return 2
    label = sys.argv[1]
    pattern = sys.argv[2]
    excludes = []
    if "--exclude" in sys.argv:
        i = sys.argv.index("--exclude")
        excludes = sys.argv[i + 1:]

    tus = sorted(glob.glob(pattern))
    tus = [p for p in tus
           if not any(x in p for x in excludes)]
    bad = []
    for p in tus:
        r = subprocess.run(["g++", "-std=c++03", "-fsyntax-only",
                            "-Isrc/include", "-Ibuild/shim", p],
                           capture_output=True)
        if r.returncode != 0:
            bad.append((p, r.stderr.decode("utf-8", "replace")))
    print("%s compile: %d/%d" % (label, len(tus) - len(bad), len(tus)))
    if bad:
        for p, err in bad[:3]:
            print("--- %s" % p)
            print("\n".join(err.splitlines()[:12]))
        print("FAILED: %s" % ", ".join(p for p, _ in bad[:10]))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
