#!/usr/bin/env python3
"""Link the whole generated tree into a program and run it.

A per-file syntax check cannot see a forwarder that names a symbol nothing
defines. That is not a hypothetical: the reconstructed headers declare 214
destructors that had no definition at all, and the `DECL` pattern in
gen_bodies.py could not match a destructor declaration, so the tree did not link
and nothing noticed. Only a link finds that class of defect.

So this links every transcription and every forwarder into one program, then
runs it. The program (build/ts_link_test.cpp) registers the recovered functions,
proves dispatch resolves a sample of VAs, and executes the original PE entry
point against the machine model.

Usage:
  python3 tools/link_test.py
"""

import glob
import os
import subprocess
import sys


def main():
    srcs = ["build/ts_link_test.cpp"]
    srcs += sorted(glob.glob("src/generated/code/*.cpp"))
    # bodies_all.cpp is a concatenation of the per-class files; including both
    # is a duplicate-definition error, not a real finding.
    srcs += [p for p in sorted(glob.glob("src/generated/*.cpp"))
             if not p.endswith("bodies_all.cpp")]

    out = os.path.join("build", "ts_link_test")
    r = subprocess.run(["g++", "-std=c++03", "-O0", "-Isrc/include",
                        "-Isrc/generated", "-Ibuild/shim", "-o", out] + srcs,
                       capture_output=True)
    if r.returncode != 0:
        err = r.stderr.decode("utf-8", "replace")
        print("link: FAILED")
        sys.stderr.write("\n".join(err.splitlines()[:40]) + "\n")
        return 1
    print("link: OK (%d translation units)" % len(srcs))

    r = subprocess.run([out], capture_output=True)
    sys.stdout.write(r.stdout.decode("utf-8", "replace"))
    if r.returncode != 0:
        print("link test program exited %d" % r.returncode)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
