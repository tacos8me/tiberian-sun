#!/usr/bin/env python3
"""
End-to-end harness for the Tiberion Sun reconstruction.

Two jobs, both of which the project previously did by hand in ad-hoc shell
one-liners that nobody re-ran:

  metrics   measure the real state of the tree and append it to a history file
  determinism  prove the pipeline is reproducible (or name what is not)

Metrics are deliberately hard to inflate. Coverage is measured as MERGED
interval bytes over .text, not the sum of function sizes (which double-counts
the ~194 legitimate shared-tail jump targets). Every number printed here can be
recomputed from out/ and bin/GAME.EXE alone.

Usage:
  python3 tools/harness.py metrics            # print + append to out/metrics.jsonl
  python3 tools/harness.py metrics --no-append
  python3 tools/harness.py determinism --run   # run the pipeline twice and diff
"""
import bisect
import hashlib
import json
import os
import subprocess
import sys

TEXT_VA = 0x401000


def _load(p):
    if not os.path.exists(p):
        return None
    with open(p) as f:
        return json.load(f)


def _merged_text_coverage(fns):
    """Merged-interval coverage of .text.

    The sum of function sizes over-counts because a function may contain
    another function's entry point (a shared tail reached by jmp). Merging
    intervals is the honest measure and matches what verify.py reports.
    """
    iv = sorted((f["va"], f["va"] + f["size"]) for f in fns)
    merged = []
    for a, b in iv:
        if merged and a <= merged[-1][1]:
            merged[-1][1] = max(merged[-1][1], b)
        else:
            merged.append([a, b])
    return merged


def collect():
    m = {}

    # --- disassembly -------------------------------------------------------
    fn = _load("out/functions.json")
    if fn:
        fns = fn["functions"]
        merged = _merged_text_coverage(fns)
        covered = sum(b - a for a, b in merged)
        text_size = 2712101
        m["functions"] = len(fns)
        m["text_covered_bytes"] = covered
        m["text_coverage_pct"] = round(100.0 * covered / text_size, 2)
        m["text_unattributed_kb"] = round((text_size - covered) / 1024.0)
        # The residual .text is classified by tools/disasm.py into jump tables,
        # padding and what is left over. Those bytes are attributed as *data*
        # and are reported beside the code figure, never added into it: 99.98%
        # attributed is not 99.98% recovered code, and the two numbers are
        # printed separately so it cannot be read as if it were.
        td = _load("out/text_data.json")
        if td:
            data = (td["jump_tables_bytes"] + td["switch_index_tables_bytes"]
                    + td["padding_bytes"])
            m["text_jump_table_bytes"] = td["jump_tables_bytes"]
            m["text_padding_bytes"] = td["padding_bytes"]
            m["text_unclassified_bytes"] = td["unclassified_bytes"]
            m["text_attributed_pct"] = round(
                100.0 * (covered + data) / text_size, 2)
        # shared tails: a function entry strictly inside another body
        starts = sorted(f["va"] for f in fns)
        inside = 0
        for f in fns:
            a, b = f["va"], f["va"] + f["size"]
            i = bisect.bisect_right(starts, a)
            while i < len(starts) and starts[i] < b:
                inside += 1
                i += 1
        m["shared_tail_entry_points"] = inside

    # --- classes / vtables -------------------------------------------------
    man = _load("out/class_manifest.json")
    if man:
        m["classes"] = len(man)
        slots = [s for e in man for s in e.get("entries", [])]
        m["vtable_slots"] = len(slots)
        if fn:
            fstarts = {f["va"] for f in fn["functions"]}
            m["vtable_slots_unseeded"] = sum(1 for s in slots
                                             if s not in fstarts)
        m["ctors_are_function_starts"] = None
        if fn:
            fs = {f["va"] for f in fn["functions"]}
            c = [x for e in man for x in e.get("ctors", [])]
            if c:
                m["ctors_are_function_starts"] = round(
                    100.0 * sum(1 for x in c if x in fs) / len(c), 1)

    # --- headers -----------------------------------------------------------
    vfunc = named = hdrs = 0
    for root, _, files in os.walk("src/include"):
        for f in files:
            if not f.endswith(".h"):
                continue
            hdrs += 1
            src = open(os.path.join(root, f), errors="replace").read()
            for line in src.splitlines():
                if "virtual" not in line:
                    continue
                if "vfunc_" in line:
                    vfunc += 1
                elif "~" not in line:
                    named += 1
    m["headers"] = hdrs
    m["virtuals_named"] = named
    m["virtuals_unnamed"] = vfunc
    if named + vfunc:
        m["virtuals_named_pct"] = round(100.0 * named / (named + vfunc), 1)

    # --- evidence artifacts ------------------------------------------------
    for key, path in (("member_offsets_recovered", "out/members.json"),
                      ("member_names", "out/member_names.json"),
                      ("signatures", "out/signatures.json"),
                      ("virtual_names", "out/virtual_names.json"),
                      ("ini_keys", "out/ini_keys.json")):
        d = _load(path)
        if d is None:
            m[key] = None
        elif key == "member_offsets_recovered":
            m[key] = sum(len(v.get("members") or []) for v in d.values())
        elif key == "member_names":
            m[key] = sum(len(v.get("members") or {}) for v in d.values())
        elif key == "virtual_names":
            m[key] = len(d)
        elif key == "ini_keys":
            m[key] = len(d) if isinstance(d, list) else len(d.get("keys", d))
        else:
            m[key] = len(d)

    # --- binary identity ---------------------------------------------------
    if os.path.exists("bin/GAME.EXE"):
        h = hashlib.sha256(open("bin/GAME.EXE", "rb").read()).hexdigest()
        m["game_exe_sha256"] = h
    return m


def cmd_metrics(append):
    m = collect()
    width = max(len(k) for k in m)
    for k in sorted(m):
        print(f"  {k:<{width}}  {m[k]}")
    if append:
        os.makedirs("out", exist_ok=True)
        with open("out/metrics.jsonl", "a") as f:
            f.write(json.dumps(m, sort_keys=True) + "\n")
        print(f"\nappended to out/metrics.jsonl")
    return 0


def _hash_tree(root):
    """Hash each artifact keyed by its path RELATIVE to root.

    Keying on the absolute path made two runs in different temp directories
    look different even when every byte was identical -- the check reported
    false nondeterminism.
    """
    out = {}
    for dirpath, dirnames, files in os.walk(root):
        dirnames.sort()
        for f in sorted(files):
            p = os.path.join(dirpath, f)
            rel = os.path.relpath(p, root)
            out[rel] = hashlib.sha256(open(p, "rb").read()).hexdigest()
    return out


def cmd_determinism(run):
    """Two clean runs must produce byte-identical output."""
    import shutil
    import tempfile

    if not run:
        print("determinism: pass --run to execute the pipeline twice")
        return 0

    results = []
    for i in (1, 2):
        d = tempfile.mkdtemp(prefix=f"tsdet{i}-")
        for sub in ("bin", "tools", "build"):
            shutil.copytree(sub, os.path.join(d, sub))
        os.makedirs(os.path.join(d, "out"))
        shutil.copytree("src/handwritten",
                        os.path.join(d, "src", "handwritten"))
        r = subprocess.run(["./tools/pipeline.sh"], cwd=d,
                           capture_output=True)
        if r.returncode != 0:
            print(f"run {i} FAILED (exit {r.returncode})")
            print(r.stderr.decode(errors="replace")[-2000:])
            return 1
        # The generated C++ is a pipeline product too. Hashing only out/ would
        # have called the pipeline deterministic while the 172 transcription
        # TUs and 309 forwarder TUs went unchecked.
        h = dict(_hash_tree(os.path.join(d, "out")))
        for k, v in _hash_tree(os.path.join(d, "src", "generated")).items():
            h["src/generated/" + os.path.basename(k)] = v
        results.append(h)

    a, b = results
    # out/metrics.jsonl is an append-only history, not a pipeline product.
    ignore = {"metrics.jsonl"}
    diffs = [k for k in set(a) | set(b)
             if k not in ignore and a.get(k) != b.get(k)]
    if diffs:
        print(f"NOT DETERMINISTIC: {len(diffs)} artifact(s) differ")
        for k in sorted(diffs)[:10]:
            print("  -", os.path.basename(k))
        return 1
    print(f"determinism: OK ({len(a)} artifacts byte-identical across two "
          f"clean runs)")
    return 0


def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else "metrics"
    if cmd == "metrics":
        sys.exit(cmd_metrics("--no-append" not in sys.argv))
    if cmd == "determinism":
        sys.exit(cmd_determinism("--run" in sys.argv))
    print(__doc__)
    sys.exit(2)


if __name__ == "__main__":
    main()
