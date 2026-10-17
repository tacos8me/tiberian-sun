#!/usr/bin/env python3
"""Boot bin/GAME.EXE inside the machine model: one command, repeatable.

    python3 tools/boot.py                     # boot with the default budget
    python3 tools/boot.py --budget 5000000    # stop after 5M instructions
    python3 tools/boot.py --timeout 120       # a longer wall-clock watchdog
    python3 tools/boot.py --imports 24        # a longer tail of import calls
    python3 tools/boot.py --watch             # print every import call as it happens
    python3 tools/boot.py --json              # the parsed report as JSON
    python3 tools/boot.py --rebuild           # ignore the object cache
    python3 tools/boot.py --no-build          # reuse the last binary, do not compile
    python3 tools/boot.py --help

WHAT IT DOES

  Compiles build/boot_driver.cpp plus the generated tree (173 transcription
  units and the per-class forwarders) at the target ABI (-m32), links them, maps
  bin/GAME.EXE at its real VAs, resolves all 341 imports to stubs, and runs the
  original PE entry point 0x006854e3 against the model under an instruction
  budget and a wall-clock watchdog.

  Object files are cached in build/boot_obj/, keyed by source mtime and size and
  by the compile flags, so the first run pays a ~60-90 s full build and every
  run after a one-file edit recompiles only the files that changed -- typically
  one translation unit plus the link. That cache is the point: a stub or model
  edit is a 90 s rebuild without it and a few seconds with it.

THE REPORT

  instructions, deepest call depth, the stop reason, the faulting EIP with the
  instruction bytes there, import calls by name and count, and whether a message
  loop was reached. Then, for the stop point, the recovered function that
  contains it (from out/functions.json when present, else from the boundaries
  recorded in src/generated/code/*.cpp), the transcribed instruction at the
  faulting EIP with its original disassembly, and the first few instructions of
  that function.

BISECTING A RUN

  The tail is the diagnosis, not the head. A run that stops at an address says
  nothing about how it got there, so the report also carries, oldest first:

    * the last N import calls, with the instruction number and call depth each
      happened at, consecutive repeats collapsed (a burst of 40 HeapAlloc calls
      reads as one line with x40);
    * the model's last 8 call targets (Machine.h keeps exactly 8 in
      recent_calls; that ring is not this tool's to widen);
    * on a fault, the same two lists as they stood at the fault.

  --budget is the bisect lever. Raise it and re-run: the stop point moves
  forward and the report names the function it moved into. Lower it and the run
  stops earlier with the state intact. `--watch` prints the import calls live.

  Two conditions are reported as failures rather than as measurements, because
  both are defects in this tree and not missing Windows: an image that will not
  load, and an unmapped call to an address inside .text (a recovered function
  that nothing registered).

  build/ts_link_test.cpp is untouched by this tool: it is the pipeline's link
  gate, and tools/link_test.py still builds and runs it.
"""

import glob
import json
import os
import re
import shutil
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)

OBJDIR = os.path.join("build", "boot_obj")
MANIFEST = os.path.join(OBJDIR, "manifest.json")
BINARY = os.path.join("build", "boot_driver")
INDEX_CACHE = os.path.join(OBJDIR, "function_index.json")
INCLUDE = ["-Isrc/include", "-Isrc/generated", "-Ibuild/shim"]
CXXFLAGS = ["-std=c++03", "-O0", "-m32"] + INCLUDE


def sources():
    """Every translation unit the boot needs, in a stable order."""
    srcs = ["build/boot_driver.cpp"]
    srcs += sorted(glob.glob("src/generated/code/*.cpp"))
    # bodies_all.cpp is a concatenation of the per-class files; linking both is
    # a duplicate definition, not a finding.
    srcs += [p for p in sorted(glob.glob("src/generated/*.cpp"))
             if not p.endswith("bodies_all.cpp")]
    return srcs


def object_path(src):
    return os.path.join(OBJDIR, src.replace("/", "_")[:-4] + ".o")


def signature(path):
    st = os.stat(path)
    return [st.st_mtime_ns, st.st_size]


def dependencies(obj):
    """The headers an object was compiled against, read from its -MMD file.

    The cache must not miss a header edit: Machine.h and Win32Stubs.h are where
    a boot's behaviour actually changes, and a cache keyed on the .cpp alone
    would hand back a binary built against the previous header. The compiler
    already knows the answer, so it writes it (-MMD) and this reads it back.
    """
    path = obj + ".d"
    try:
        with open(path) as fh:
            text = fh.read().replace("\\\n", " ")
    except OSError:
        return None
    _, _, deps = text.partition(":")
    return deps.split()


def stale(src, obj):
    """True when the object is missing or older than anything it was built from."""
    if not os.path.exists(obj):
        return True
    try:
        built = os.stat(obj).st_mtime_ns
    except OSError:
        return True
    deps = dependencies(obj)
    if deps is None:
        return True
    for dep in deps:
        try:
            if os.stat(dep).st_mtime_ns > built:
                return True
        except OSError:
            return True          # a header that vanished changes the build
    return False


def load_manifest():
    try:
        with open(MANIFEST) as fh:
            m = json.load(fh)
        if m.get("flags") == CXXFLAGS:
            return m
    except (OSError, ValueError):
        pass
    return {"flags": CXXFLAGS, "objects": {}}


def save_manifest(m):
    os.makedirs(OBJDIR, exist_ok=True)
    tmp = MANIFEST + ".tmp"
    with open(tmp, "w") as fh:
        json.dump(m, fh)
    os.replace(tmp, MANIFEST)


def compile_one(src):
    obj = object_path(src)
    r = subprocess.run(["g++"] + CXXFLAGS + ["-MMD", "-MF", obj + ".d",
                                             "-c", src, "-o", obj],
                       capture_output=True)
    return src, obj, r.returncode, r.stderr.decode("utf-8", "replace")


def build(rebuild):
    """Compile what changed, link the rest. Returns (ok, seconds, stats)."""
    started = time.time()
    srcs = sources()
    man = load_manifest()
    objects = man["objects"]

    todo = []
    for src in srcs:
        sig = signature(src)
        ent = objects.get(src)
        obj = ent["obj"] if ent else object_path(src)
        if (rebuild or not ent or ent.get("sig") != sig
                or obj != object_path(src) or stale(src, obj)):
            todo.append(src)

    if todo:
        os.makedirs(OBJDIR, exist_ok=True)
        workers = min(16, (os.cpu_count() or 4))
        failures = []
        with ThreadPoolExecutor(max_workers=workers) as pool:
            for src, obj, rc, err in pool.map(compile_one, todo):
                if rc != 0:
                    failures.append((src, err))
                else:
                    objects[src] = {"sig": signature(src), "obj": obj}
        if failures:
            src, err = failures[0]
            print("boot: compile FAILED (%d of %d translation units)"
                  % (len(failures), len(todo)))
            print("  %s" % src)
            for line in err.splitlines()[:24]:
                print("  " + line)
            return False, time.time() - started, (len(todo), 0)
        save_manifest(man)

    link = ["g++"] + CXXFLAGS + ["-o", BINARY] + \
           [objects[s]["obj"] for s in srcs]
    r = subprocess.run(link, capture_output=True)
    if r.returncode != 0:
        print("boot: link FAILED")
        for line in r.stderr.decode("utf-8", "replace").splitlines()[:24]:
            print("  " + line)
        return False, time.time() - started, (len(todo), 0)
    return True, time.time() - started, (len(todo), len(srcs))

# ---------------------------------------------------------------------------
# Where an address lives.
#
# The boundaries come from the artifacts, not from a disassembly this tool
# redoes. src/generated/code/*.cpp records, per transcribed function, its start,
# its size and the C++ that executes it, so one pass over the committed
# generated tree answers both "which function contains this address" and "what
# is the instruction there". That is the primary source.
#
# out/functions.json holds the same boundaries for all 41,822 recovered
# functions (disasm.py's table), and it is consulted only when the generated
# index has no answer -- an address in a recovered function with no
# transcription, which is a boundary the generated tree cannot see. It is read
# lazily because it is a 64 MB artifact that a fresh clone does not have until
# the pipeline runs.
SUB_RE = re.compile(
    rb"^// (sub_[0-9a-f]{8}) -- transcription of (\d+) instructions at 0x([0-9a-f]{8})$")
SIZE_RE = re.compile(rb"^// size (\d+) bytes$")


def code_signature():
    files = sorted(glob.glob("src/generated/code/*.cpp"))
    return [[f, os.stat(f).st_mtime_ns, os.stat(f).st_size] for f in files]


def build_index():
    """[{start, size, ninsn, name, file, line}] sorted by start, cached."""
    sig = code_signature()
    try:
        with open(INDEX_CACHE) as fh:
            cached = json.load(fh)
        if cached.get("sig") == sig:
            return cached["entries"]
    except (OSError, ValueError):
        pass

    entries = []
    for path in glob.glob("src/generated/code/*.cpp"):
        with open(path, "rb") as fh:
            lines = fh.read().split(b"\n")
        for i, line in enumerate(lines):
            m = SUB_RE.match(line)
            if not m:
                continue
            size = 0
            if i + 1 < len(lines):
                ms = SIZE_RE.match(lines[i + 1])
                if ms:
                    size = int(ms.group(1))
            entries.append([int(m.group(3), 16), size, int(m.group(2)),
                            m.group(1).decode("ascii"), path, i])
    entries.sort(key=lambda e: e[0])

    os.makedirs(OBJDIR, exist_ok=True)
    try:
        with open(INDEX_CACHE, "w") as fh:
            json.dump({"sig": sig, "entries": entries}, fh)
    except OSError:
        pass
    return entries


def function_at(entries, va):
    """The recovered function containing `va`, or None.

    A binary search on starts, then a containment test against the recorded
    size. A va between two functions (padding, a tail entry) names nothing
    rather than the wrong function.
    """
    lo, hi = 0, len(entries) - 1
    best = None
    while lo <= hi:
        mid = (lo + hi) // 2
        if entries[mid][0] <= va:
            best = entries[mid]
            lo = mid + 1
        else:
            hi = mid - 1
    if best and (best[1] == 0 or va < best[0] + best[1]):
        return best
    return None


_FUNCTIONS_JSON = []


def function_from_disassembly(va):
    """The recovered function containing `va` per out/functions.json, or None.

    Loaded at most once, and only when the generated index has no answer: this
    is the table of every recovered function, including the ones with no
    transcription, and it costs 64 MB and a second to parse.
    """
    if not _FUNCTIONS_JSON:
        try:
            with open(os.path.join("out", "functions.json")) as fh:
                data = json.load(fh)
        except (OSError, ValueError):
            _FUNCTIONS_JSON.append(None)
            return None
        table = {}
        for f in data.get("functions", []):
            table[f["va"]] = (f.get("size", 0), f.get("ninsn", 0))
        _FUNCTIONS_JSON.append(table)
    table = _FUNCTIONS_JSON[0]
    if not table:
        return None
    best = None
    for start in table:
        if start <= va and (best is None or start > best):
            best = start
    if best is None:
        return None
    size, ninsn = table[best]
    if size and va >= best + size:
        return None
    return [best, size, ninsn, "sub_%08x" % best, "", -1]


def function_lines(entry):
    """The transcribed instruction lines of one function, as [(va, text)]."""
    with open(entry[4], "rb") as fh:
        lines = fh.read().split(b"\n")
    out = []
    for line in lines[entry[5] + 1:]:
        if line.startswith(b"// sub_"):
            break
        if b"ts::insn(" not in line:
            continue
        text = line.decode("utf-8", "replace").strip()
        out.append(text)
    return out


def describe_instruction(line):
    """Split a transcription line into (va, c++ statement, original asm).

    The generator keeps the original instruction as a trailing comment, so a
    fault report can show what the machine was told to do and what the original
    binary did at the same address.
    """
    m = re.search(r"ts::insn\(0x([0-9a-f]{8})u\);\s*(.*)$", line)
    if not m:
        return None, line, ""
    va = int(m.group(1), 16)
    rest = m.group(2)
    asm = ""
    if "//" in rest:
        code, asm = rest.split("//", 1)
        rest = code.rstrip()
        asm = asm.strip()
    return va, rest, asm


# ---------------------------------------------------------------------------
# The driver's report.
#
# The driver prints one `key: value` line per fact between two markers, with
# indented continuation lines for the lists. Nothing else on stdout is parsed,
# so a future stub that writes to stdout cannot corrupt the report.
BEGIN = "boot-report: begin"
END = "boot-report: end"


def parse_report(stdout):
    keys, lists, other = {}, {}, []
    inside = False
    current = None
    for line in stdout.splitlines():
        if line.strip() == BEGIN:
            inside = True
            continue
        if line.strip() == END:
            inside = False
            continue
        if not inside:
            if line.strip():
                other.append(line)
            continue
        if line.startswith("  ") and current:
            lists[current].append(line.strip())
            continue
        if ":" not in line:
            continue
        key, value = line.split(":", 1)
        key, value = key.strip(), value.strip()
        current = None
        if value:
            keys[key] = value
        else:
            lists.setdefault(key, [])
            current = key
    return keys, lists, other


def hexwords(text):
    return [int(t, 16) for t in re.findall(r"0x[0-9a-fA-F]+", text or "")]


def thunk_labels(keys):
    """{va: import name} for the driver's own traced import thunks.

    A `call dword ptr [IAT]` lands on a thunk this driver registered, not on a
    recovered function, so the model's call ring records a VA that no recovered
    function contains. The driver names those itself because only it knows the
    binding; naming them here as "no recovered function" would read as a gap in
    the recovery.
    """
    labels = {}
    for key in ("last-call-thunks", "fault-call-thunks"):
        for va_text, label in re.findall(r"(0x[0-9a-f]+)=(\S+)", keys.get(key, "")):
            labels[int(va_text, 16)] = label
    return labels


def fmt_name(entries, va, thunks=None):
    e = function_at(entries, va) or function_from_disassembly(va)
    if e:
        return "0x%08x  %s +0x%x" % (va, e[3], va - e[0])
    if thunks and va in thunks:
        return "0x%08x  import thunk -> %s" % (va, thunks[va])
    return "0x%08x  (no recovered function contains this address)" % va


STOP_WORD = {
    "fault": "fault",
    "budget": "instruction budget exhausted",
    "watchdog": "stopped by the wall-clock watchdog",
    "returned": "returned to its caller",
    "halted": "stopped without a fault",
}


def stop_point(keys, entries, out):
    """Name the recovered function the run stopped in, and show its code."""
    va_text = keys.get("fault-eip") or keys.get("eip")
    va = hexwords(va_text)
    if not va:
        return
    va = va[0]
    entry = function_at(entries, va)
    if not entry:
        entry = function_from_disassembly(va)
    if not entry:
        out.append("  in:    0x%08x -- no recovered function contains this address"
                   % va)
        return
    if entry[5] < 0:
        out.append("  in:    0x%08x  %s +0x%x  (%d instructions, %d bytes; "
                   "recovered but not transcribed, boundaries from "
                   "out/functions.json)"
                   % (va, entry[3], va - entry[0], entry[2], entry[1]))
        return
    start, size, ninsn, name, path, line = entry
    out.append("  in:    0x%08x  %s +0x%x  (%d instructions, %d bytes, %s:%d)"
               % (va, name, va - start, ninsn, size, path, line + 1))

    lines = function_lines(entry)
    here = None
    for text in lines:
        insn_va, code, asm = describe_instruction(text)
        if insn_va == va:
            here = (code, asm)
            break
    if here:
        out.append("  here:  %s%s" % (here[0], ("   ; " + here[1]) if here[1] else ""))

    out.append("  first instructions of %s:" % name)
    for text in lines[:6]:
        insn_va, code, asm = describe_instruction(text)
        if insn_va is None:
            continue
        out.append("    0x%08x  %-52s ; %s" % (insn_va, code, asm))
    if len(lines) > 6:
        out.append("    ... %d more instructions" % (len(lines) - 6))


def print_report(keys, lists, entries, build_note, elapsed):
    stop = keys.get("stop", "?")
    steps = keys.get("steps", "?")
    out = []
    out.append("boot:   %s  (%s)" % (keys.get("image", "?"), build_note))
    out.append("image:  %s bytes in %s sections; %s of %s imports resolved, %s traced"
               % (keys.get("image-bytes", "?"), keys.get("sections", "?"),
                  keys.get("imports-resolved", "?"), keys.get("imports-total", "?"),
                  keys.get("imports-bound", "?")))
    out.append("run:    budget %s instructions, depth limit %s, watchdog %ss; "
               "%.1fs wall" % (keys.get("budget", "?"), keys.get("depth-limit", "?"),
                               keys.get("watchdog", "?"), elapsed))
    out.append("stop:   %s at 0x%08x after %s instructions, deepest call depth %s"
               % (STOP_WORD.get(stop, stop),
                  (hexwords(keys.get("eip", "0x0")) or [0])[0],
                  steps, keys.get("depth", "?")))
    if stop in ("fault", "budget"):
        out.append("  why:   %s" % keys.get("fault", "(unrecorded)"))
        out.append("  eip:   0x%08x  bytes %s"
                   % ((hexwords(keys.get("fault-eip", "0x0")) or [0])[0],
                      keys.get("fault-bytes", "")))
        out.append("  regs:  %s" % keys.get("fault-regs", ""))
        out.append("  at:    %s instructions, depth %s"
                   % (keys.get("fault-steps", "?"), keys.get("fault-depth", "?")))
    elif stop == "watchdog":
        out.append("  eip:   0x%08x  bytes %s"
                   % ((hexwords(keys.get("watchdog-eip", "0x0")) or [0])[0],
                      keys.get("eip-bytes", "")))
        out.append("  regs:  %s" % keys.get("regs", ""))
        out.append("  at:    %s instructions" % keys.get("watchdog-steps", "?"))
    stop_point(keys, entries, out)

    out.append("imports: %s calls over %s traced imports, %s distinct"
               % (keys.get("import-calls", "?"), keys.get("imports-bound", "?"),
                  keys.get("distinct-imports", "?")))
    for row in lists.get("imports-by-count", []):
        out.append("  " + row)
    if lists.get("last-imports"):
        out.append("last import calls, oldest first (repeats collapsed):")
        for row in lists["last-imports"]:
            out.append("  " + row)
    thunks = thunk_labels(keys)
    for va in hexwords(keys.get("last-calls", "")):
        out.append("last call: " + fmt_name(entries, va, thunks))
    for va in hexwords(keys.get("fault-calls", "")):
        out.append("call at fault: " + fmt_name(entries, va, thunks))
    out.append("message loop: %s   (%s)"
               % (keys.get("message-loop", "?"), keys.get("message-calls", "")))

    unmapped = int(keys.get("unmapped-calls", "0") or 0)
    in_text = int(keys.get("unmapped-in-text", "0") or 0)
    if unmapped:
        out.append("unmapped calls: %d (%d inside .text)" % (unmapped, in_text))
        for va in hexwords(keys.get("unmapped-sample", "")):
            out.append("  " + fmt_name(entries, va))
    if in_text:
        out.append("DEFECT: %d unmapped calls inside .text -- a recovered function "
                   "nothing registered" % in_text)
    out.append("depth guard: %s truncations, unmodelled instructions: %s"
               % (keys.get("depth-exceeded", "?"), keys.get("unmodelled", "?")))
    print("\n".join(out))


def usage():
    print(__doc__.strip().split("\n\n")[0])


def parse_args(argv):
    opt = {"image": "bin/GAME.EXE", "budget": 20000000, "timeout": 60,
           "depth": 20000, "imports": 12, "calls": 8, "watch": False,
           "json": False, "rebuild": False, "no_build": False}
    flags = {"--watch": "watch", "--json": "json", "--rebuild": "rebuild",
             "--no-build": "no_build"}
    values = {"--image": "image", "--budget": "budget", "--timeout": "timeout",
              "--depth": "depth", "--imports": "imports", "--calls": "calls"}
    i = 0
    while i < len(argv):
        arg = argv[i]
        if arg in ("-h", "--help"):
            return None
        if arg in flags:
            opt[flags[arg]] = True
        elif arg.split("=")[0] in values:
            key = values[arg.split("=")[0]]
            if "=" in arg:
                value = arg.split("=", 1)[1]
            else:
                i += 1
                if i >= len(argv):
                    print("boot: %s needs a value" % arg)
                    return None
                value = argv[i]
            opt[key] = value if key == "image" else int(value)
        else:
            print("boot: unknown argument %s (try --help)" % arg)
            return None
        i += 1
    return opt


def main(argv):
    opt = parse_args(argv)
    if opt is None:
        usage()
        return 1

    note = "cached binary"
    if not opt["no_build"]:
        ok, secs, (compiled, total) = build(opt["rebuild"])
        if not ok:
            return 1
        note = "build %.1fs, %d of %d units compiled" % (secs, compiled, total)
    if not os.path.exists(BINARY):
        print("boot: %s does not exist; run without --no-build" % BINARY)
        return 1

    cmd = [BINARY,
           "--image=%s" % opt["image"],
           "--budget=%d" % opt["budget"],
           "--timeout=%d" % opt["timeout"],
           "--depth=%d" % opt["depth"],
           "--imports=%d" % opt["imports"],
           "--calls=%d" % opt["calls"]]
    if opt["watch"]:
        cmd.append("--verbose")

    started = time.time()
    try:
        run = subprocess.run(cmd, capture_output=True, timeout=opt["timeout"] + 60)
    except subprocess.TimeoutExpired:
        print("boot: the driver did not exit within %ds -- the wall-clock watchdog "
              "did not fire either, which is itself a defect" % (opt["timeout"] + 60))
        return 1
    elapsed = time.time() - started

    stdout = run.stdout.decode("utf-8", "replace")
    stderr = run.stderr.decode("utf-8", "replace").strip()
    keys, lists, other = parse_report(stdout)
    if not keys:
        print("boot: the driver produced no report (exit %d)" % run.returncode)
        if stderr:
            print(stderr)
        print(stdout[-2000:])
        return 1

    entries = build_index()
    for line in other:
        print(line)
    if opt["json"]:
        keys = dict(keys)
        keys["build"] = note
        keys["elapsed-seconds"] = round(elapsed, 2)
        keys["lists"] = lists
        stop_va = (hexwords(keys.get("fault-eip", "")) or hexwords(keys.get("eip", "")) or [0])[0]
        entry = function_at(entries, stop_va)
        keys["stop-function"] = (
            {"va": entry[0], "size": entry[1], "instructions": entry[2],
             "name": entry[3], "file": entry[4], "line": entry[5] + 1}
            if entry else None)
        print(json.dumps(keys, indent=1, sort_keys=True))
    else:
        print_report(keys, lists, entries, note, elapsed)
        if stderr:
            print(stderr)

    if run.returncode == 2:
        return 2
    if run.returncode == 3:
        return 3
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
