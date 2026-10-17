#!/usr/bin/env python3
"""Capture the retail game's blowfish.dll registration path and cipher use.

The model fails at GAME.EXE 0x5e6216 because it has no `DllRegisterServer`
export to resolve. This reads what the REAL binary does there, so the model can
copy it instead of guessing:

  * the result of the first CoCreateInstance(CLSID {1440AD10-...}),
  * LoadLibraryA("blowfish.dll") and GetProcAddress(..., "DllRegisterServer"),
  * what DllRegisterServer returns and which registry keys it writes,
  * the retried CoCreateInstance, the object pointer and its vtable,
  * every call the game makes into BLOWFISH.DLL (SetKey/Encrypt/Decrypt) with
    arguments, so we know whether a fake S_OK is enough or real Blowfish is
    needed.

It attaches gdb to the running GAME.EXE at its preferred base and logs events
from Python breakpoints, so a breakpoint inside BLOWFISH.DLL (mapped at
0x11000000) can be installed lazily, after the DLL is loaded.

    ./tools/oracle_blowfish.py --secs 50            # warm: class already registered
    ./tools/oracle_blowfish.py --cold --secs 50     # delete the CLSID key first
    ./tools/oracle_blowfish.py --trace              # also a plain +module,+ole run

Full gdb output: /tmp/blowfish_oracle_gdb.log
Live counters:   /tmp/blowfish_oracle_counts.json

The cold run is the one that matches the model, which starts with no registry:
the first CoCreateInstance returns REGDB_E_CLASSNOTREG, the fallback runs, and
the retry succeeds. The key it writes is left in the prefix exactly as the game
wrote it; `--cold` deletes only the one CLSID subtree first.
"""
import argparse
import json
import os
import re
import shutil
import signal
import subprocess
import sys
import threading
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPO = os.path.dirname(ROOT)
GAMEDIR = os.environ.get("GAMEDIR", os.path.join(REPO, "gamedir"))
PREFIX = os.environ.get("WINEPREFIX", os.path.join(REPO, ".wine32"))
TSCD = os.environ.get("TSCD", os.path.join(REPO, "work", "cd1"))
IMAGE_LO, IMAGE_HI = 0x00400000, 0x00698000
BLOWFISH_BASE = 0x11000000
CLSID_KEY = r"HKCR\CLSID\{1440AD10-6AA8-11D1-B6F9-00A024DDAFD1}"
GDB_LOG = "/tmp/blowfish_oracle_gdb.log"
COUNTS = "/tmp/blowfish_oracle_counts.json"


def die(msg, code=1):
    print("oracle_blowfish: %s" % msg, file=sys.stderr)
    return code


def wine_env(display=None, debug="-all"):
    e = dict(os.environ)
    e.update(WINEPREFIX=PREFIX, WINEARCH="win32", WINEDEBUG=debug)
    if display:
        e["DISPLAY"] = display
    return e


def preflight():
    if not os.path.isfile(os.path.join(GAMEDIR, "GAME.EXE")):
        return die("no retail binary at %s (copy work/cd1/INSTALL there)" % GAMEDIR)
    if not os.path.isdir(PREFIX):
        return die("no 32-bit Wine prefix at %s" % PREFIX)
    if not os.path.isfile(os.path.join(TSCD, "TS1.DSK")):
        return die("no CD tree at %s (needs TS1.DSK)" % TSCD)
    for tool in ("wine", "Xvfb", "gdb", "xdotool", "xwininfo", "wineserver"):
        if not shutil.which(tool):
            return die("missing %s" % tool)
    try:
        scope = open("/proc/sys/kernel/yama/ptrace_scope").read().strip()
    except OSError:
        scope = "0"
    if scope not in ("0", ""):
        return die("ptrace_scope is %s; attach will fail. Run:\n"
                   "  sudo sysctl -w kernel.yama.ptrace_scope=0" % scope)
    return 0


def setup_cd():
    """Same two requirements as wine_run.sh --cd: d: is a cdrom with label TS1."""
    link = os.path.join(PREFIX, "dosdevices", "d:")
    if os.path.exists(link) and not os.path.islink(link):
        return die("%s exists and is not a symlink" % link)
    os.symlink(TSCD, link) if not os.path.islink(link) else None
    with open(os.path.join(TSCD, ".windows-label"), "w") as f:
        f.write("TS1\n")
    r = subprocess.run(["wine", "reg", "add", r"HKLM\Software\Wine\Drives",
                        "/v", "d:", "/d", "cdrom", "/f"],
                       env=wine_env(), capture_output=True, text=True)
    if r.returncode != 0:
        return die("could not set d: to cdrom: %s" % (r.stderr or r.stdout))
    subprocess.run(["wineserver", "-k"], env=wine_env(), capture_output=True)
    time.sleep(1)
    return 0


def delete_clsid_key():
    r = subprocess.run(["wine", "reg", "delete", CLSID_KEY, "/f"],
                       env=wine_env(), capture_output=True, text=True)
    return r.returncode == 0


def key_exists():
    r = subprocess.run(["wine", "reg", "query", CLSID_KEY],
                       env=wine_env(), capture_output=True, text=True)
    return r.returncode == 0


def start_display():
    disp = 79
    while os.path.exists("/tmp/.X%d-lock" % disp):
        disp += 1
    subprocess.Popen(["Xvfb", ":%d" % disp, "-screen", "0", "640x480x16"],
                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(2)
    return ":%d" % disp


def game_pid():
    out = subprocess.run(["ps", "-eo", "pid=,comm="], capture_output=True,
                         text=True).stdout
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 2 and parts[1] == "GAME.EXE":
            return int(parts[0])
    return None


def mapped(pid):
    try:
        for line in open("/proc/%d/maps" % pid):
            if "GAME.EXE" in line and line.lower().startswith("%08x" % IMAGE_LO):
                return True
    except OSError:
        pass
    return False


def wait_for_game(display, secs, early=False):
    subprocess.run(["pkill", "-x", "wine"], capture_output=True)
    subprocess.run(["wineserver", "-k"], env=wine_env(), capture_output=True)
    time.sleep(1)
    subprocess.Popen(["wine", "GAME.EXE"], cwd=GAMEDIR,
                     env=wine_env(display), stdout=subprocess.DEVNULL,
                     stderr=subprocess.DEVNULL)
    deadline = time.time() + secs
    pid = None
    while time.time() < deadline:
        if early:
            # The registration loop is in the first milliseconds of GAME.EXE
            # init; an attach takes ~200 ms and always lands after it. Stop the
            # process the instant its comm appears, then let gdb attach to a
            # stopped process and set the breakpoints before it runs.
            for d in os.listdir("/proc"):
                if not d.isdigit():
                    continue
                try:
                    comm = open("/proc/%s/comm" % d).read().strip()
                except OSError:
                    continue
                if comm == "GAME.EXE":
                    p = int(d)
                    os.kill(p, signal.SIGSTOP)
                    if mapped(p):
                        return p
                    os.kill(p, signal.SIGCONT)
        else:
            pid = game_pid()
            if pid and mapped(pid):
                return pid
        time.sleep(0.01 if early else 0.5)
    return None


def dismiss_loop(stop, display, secs=120):
    """The retail game shows a modal DirectSound warning; without a click the
    startup path past it never runs. Same trick as tools/oracle.py."""
    end = time.time() + secs
    while time.time() < end and not stop.is_set():
        try:
            tree = subprocess.run(["xwininfo", "-root", "-tree"],
                                  env=wine_env(display), capture_output=True,
                                  text=True, timeout=10).stdout
            for line in tree.splitlines():
                if '"Tiberian Sun"' not in line or "640x480" in line:
                    continue
                m = re.search(r"(0x[0-9a-f]+)", line)
                if m:
                    subprocess.run(["xdotool", "key", "--window", m.group(1),
                                    "Return"], env=wine_env(display),
                                   capture_output=True, timeout=10)
        except (subprocess.SubprocessError, OSError):
            pass
        time.sleep(2)


# The gdb Python program. Breakpoints are Python objects: stop() logs and
# returns False, so the inferior keeps running and no `commands` blocks are
# needed. The BLOWFISH.DLL breakpoints are installed from the first hit inside
# the game, when the DLL is guaranteed mapped.
GDB_PY = r'''
import gdb, json, os, struct

COUNTS = {}
OUT = open("/tmp/blowfish_oracle_gdb.log", "a", buffering=1)
DLL_BPS = []

def flush():
    tmp = "/tmp/blowfish_oracle_counts.json.tmp"
    with open(tmp, "w") as f:
        json.dump(COUNTS, f, sort_keys=True)
    os.replace(tmp, "/tmp/blowfish_oracle_counts.json")

def log(msg):
    OUT.write(msg + "\n")

def rd(addr, n):
    return bytes(gdb.selected_inferior().read_memory(addr, n))

def u32(addr):
    return struct.unpack("<I", rd(addr, 4))[0]

def cstr(addr):
    out = b""
    for i in range(0, 256, 4):
        chunk = rd(addr + i, 4)
        out += chunk
        if b"\0" in chunk:
            break
    return out.split(b"\0")[0].decode("latin1", "replace")

def hx(bs):
    return " ".join("%02x" % b for b in bs)

def dump_vtable(addr, n=10):
    words = struct.unpack("<%dI" % n, rd(addr, 4 * n))
    return " ".join("%08x" % w for w in words)

class BP(gdb.Breakpoint):
    def __init__(self, addr, name, fn=None, dynamic=False):
        super(BP, self).__init__("*0x%x" % addr, internal=True)
        self.name = name
        self.fn = fn
        self.dynamic = dynamic
    def stop(self):
        COUNTS[self.name] = COUNTS.get(self.name, 0) + 1
        n = COUNTS[self.name]
        try:
            msg = self.fn() if self.fn else ""
            if msg is not None and (n <= 4 or n % 500 == 0
                                    or self.name == "BF_SetKey"):
                log("%-18s #%-5d %s" % (self.name, n, msg))
        except Exception as e:
            if n <= 4:
                log("%-18s #%-5d <error %s>" % (self.name, n, e))
        flush()
        return False

def install_dll_breakpoints():
    if DLL_BPS:
        return
    specs = [
        (0x11001f30, "BF_QueryInterface", bf_qi),
        (0x11001fb0, "BF_AddRef", lambda: ""),
        (0x11001fc4, "BF_Release", lambda: ""),
        (0x11002011, "BF_SetKey", bf_setkey),
        (0x1100204d, "BF_GetKeySize", bf_getsize),
        (0x1100206c, "BF_GetBlockSize", bf_getsize),
        (0x1100208b, "BF_Encrypt", bf_crypt),
        (0x110020cb, "BF_Decrypt", bf_crypt),
    ]
    for addr, name, fn in specs:
        try:
            DLL_BPS.append(BP(addr, name, fn, dynamic=True))
        except Exception as e:
            log("install %s failed: %s" % (name, e))
    log("installed %d BLOWFISH.DLL breakpoints" % len(DLL_BPS))

def bf_qi():
    # QueryInterface(this, riid, ppv): at entry this=[esp+4], riid=[esp+8],
    # ppv=[esp+0xc] (the method has not run push ebp yet at the breakpoint).
    sp = int(gdb.parse_and_eval("$esp"))
    return "this=%08x riid=%s" % (u32(sp + 4), hx(rd(u32(sp + 8), 16)))

LAST_DECRYPT = [0]

def bf_setkey():
    sp = int(gdb.parse_and_eval("$esp"))
    this, size, key = u32(sp + 4), u32(sp + 8), u32(sp + 0xc)
    blocks = COUNTS.get("BF_Decrypt", 0) - LAST_DECRYPT[0]
    LAST_DECRYPT[0] = COUNTS.get("BF_Decrypt", 0)
    return "this=%08x size=%d blocks_since_last_key=%d key=%s" % (
        this, size, blocks, hx(rd(key, min(size, 56))) if key and size else "-")

def bf_getsize():
    sp = int(gdb.parse_and_eval("$esp"))
    return "this=%08x out=%08x" % (u32(sp + 4), u32(sp + 8))

def bf_crypt():
    sp = int(gdb.parse_and_eval("$esp"))
    this, ln, src, dst = (u32(sp + 4), u32(sp + 8), u32(sp + 0xc), u32(sp + 0x10))
    return "this=%08x len=%d in=%08x(%s) out=%08x" % (
        this, ln, src, hx(rd(src, min(ln, 8))) if src and ln else "-", dst)

# --- GAME.EXE breakpoints -------------------------------------------------
def first_cc():
    return "hr=%08x out=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff,
                                 u32(int(gdb.parse_and_eval("$ebp")) - 0x604))

def loadlib():
    hm = int(gdb.parse_and_eval("$eax")) & 0xffffffff
    name = cstr(u32(0x6a484c))
    return "hm=%08x name=%s" % (hm, name)

def getproc():
    return "proc=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff)

def dllreg():
    return "hr=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff)

def retry_cc():
    return "hr=%08x out=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff,
                                 u32(int(gdb.parse_and_eval("$ebp")) - 0x604))

def release_obj():
    obj = int(gdb.parse_and_eval("$eax")) & 0xffffffff
    vt = u32(obj)
    return "obj=%08x vptr=%08x vtable[0..9]=%s" % (obj, vt, dump_vtable(vt))

def w_cc():
    sp = int(gdb.parse_and_eval("$esp"))
    return "hr=%08x tmp=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff,
                                 u32(sp + 0x10))

def w_ole():
    return "hr=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff)

def w_qi():
    edi = int(gdb.parse_and_eval("$edi")) & 0xffffffff
    obj = u32(edi)
    vt = u32(obj) if obj else 0
    install_dll_breakpoints()
    return "hr=%08x out=%08x obj=%08x vptr=%08x vtable[0..9]=%s" % (
        int(gdb.parse_and_eval("$eax")) & 0xffffffff, edi, obj, vt,
        dump_vtable(vt) if vt else "-")

def bp_cc():
    sp = int(gdb.parse_and_eval("$esp"))
    return "hr=%08x tmp=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff,
                                 u32(sp + 8))

def bp_ole():
    return "hr=%08x" % (int(gdb.parse_and_eval("$eax")) & 0xffffffff)

def bp_qi():
    edi = int(gdb.parse_and_eval("$edi")) & 0xffffffff
    obj = u32(edi)
    vt = u32(obj) if obj else 0
    install_dll_breakpoints()
    return "hr=%08x out=%08x obj=%08x vptr=%08x vtable[0..9]=%s" % (
        int(gdb.parse_and_eval("$eax")) & 0xffffffff, edi, obj, vt,
        dump_vtable(vt) if vt else "-")

BP(0x5e6211, "FIRST_CoCreate", first_cc)
BP(0x5e622d, "LoadLibraryA", loadlib)
BP(0x5e623f, "GetProcAddress", getproc)
BP(0x5e6245, "DllRegisterServer", dllreg)
BP(0x5e6257, "RETRY_CoCreate", retry_cc)
BP(0x5e629b, "Release_created", release_obj)
BP(0x5e87d3, "wrapper_CoCreate", w_cc)
BP(0x5e87e2, "wrapper_OleRun", w_ole)
BP(0x5e87f7, "wrapper_QueryInt", w_qi)
BP(0x423384, "BlowPipe_CoCreate", bp_cc)
BP(0x423395, "BlowPipe_OleRun", bp_ole)
BP(0x4233aa, "BlowPipe_QueryInt", bp_qi)
log("=== oracle_blowfish attach: breakpoints installed ===")
flush()
# --early SIGSTOPs the process before attaching, so the first continue reports
# that SIGSTOP as a stop. Consume it and keep going; the breakpoints themselves
# return False and never stop the inferior.
for _ in range(10000):
    try:
        gdb.execute("continue")
    except (gdb.error, KeyboardInterrupt):
        break
    if not gdb.selected_inferior().pid:
        break
log("=== gdb stopped; counters: %s" % json.dumps(COUNTS, sort_keys=True))
flush()
try:
    gdb.execute("detach")
except gdb.error:
    pass
'''


def run_gdb(pid, secs):
    script = "/tmp/blowfish_oracle_gdb.py"
    with open(script, "w") as f:
        f.write(GDB_PY)
    open(GDB_LOG, "w").close()
    try:
        os.remove(COUNTS)
    except OSError:
        pass
    cmd = ["gdb", "-batch", "-q", "-p", str(pid),
           "-ex", "set pagination off",
           "-ex", "set confirm off",
           "-ex", "handle SIGSTOP nostop noprint nopass",
           "-ex", "source %s" % script]
    p = subprocess.Popen(cmd, stdout=open(GDB_LOG + ".stdout", "w"),
                         stderr=subprocess.STDOUT, text=True)
    time.sleep(secs)
    p.terminate()
    try:
        p.wait(timeout=20)
    except subprocess.TimeoutExpired:
        p.kill()
    return p.returncode


def summarize():
    print("oracle_blowfish: gdb log %s (%d lines)" % (
        GDB_LOG, sum(1 for _ in open(GDB_LOG, errors="replace"))))
    for line in open(GDB_LOG, errors="replace"):
        if line.startswith(("FIRST_", "LoadLibraryA", "GetProcAddress",
                            "DllRegisterServer", "RETRY_", "Release_created",
                            "wrapper_", "BlowPipe_", "BF_", "installed",
                            "===")):
            print("  " + line.rstrip())
    try:
        counts = json.load(open(COUNTS))
    except (OSError, ValueError):
        counts = {}
    print("oracle_blowfish: counters %s" % json.dumps(counts, sort_keys=True))


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument("--secs", type=int, default=50,
                    help="how long the game runs before gdb is stopped")
    ap.add_argument("--cold", action="store_true",
                    help="delete the CLSID key first, so the registration "
                         "fallback is exercised (the model's situation)")
    ap.add_argument("--trace", action="store_true",
                    help="also run a plain +module,+ole pass and save the log")
    ap.add_argument("--no-dismiss", action="store_true")
    ap.add_argument("--early", action="store_true",
                    help="SIGSTOP GAME.EXE the instant it appears so gdb can "
                         "break in the registration loop, which runs before a "
                         "normal attach lands")
    opt = ap.parse_args(argv)

    rc = preflight()
    if rc:
        return rc
    rc = setup_cd()
    if rc:
        return rc

    existed = key_exists()
    print("oracle_blowfish: CLSID key %s before run" %
          ("present" if existed else "absent"))
    if opt.cold:
        if existed and not delete_clsid_key():
            return die("could not delete %s" % CLSID_KEY)
        print("oracle_blowfish: deleted %s (cold start)" % CLSID_KEY)

    display = start_display()
    pid = wait_for_game(display, 45, early=opt.early)
    if not pid:
        return die("the game never mapped at its preferred base")
    print("oracle_blowfish: pid %d on %s; image at 0x%08x" % (pid, display, IMAGE_LO))

    stop = threading.Event()
    if not opt.no_dismiss:
        threading.Thread(target=dismiss_loop, args=(stop, display), daemon=True).start()
    try:
        run_gdb(pid, opt.secs)
    finally:
        stop.set()

    print("oracle_blowfish: CLSID key %s after run" %
          ("present" if key_exists() else "absent"))
    if opt.cold:
        r = subprocess.run(["wine", "reg", "query", CLSID_KEY],
                           env=wine_env(), capture_output=True, text=True)
        for line in (r.stdout or "").splitlines()[:12]:
            print("  reg: " + line.strip())
        r = subprocess.run(["wine", "reg", "query",
                            CLSID_KEY + r"\InprocServer32"],
                           env=wine_env(), capture_output=True, text=True)
        for line in (r.stdout or "").splitlines()[:6]:
            print("  reg: " + line.strip())

    if opt.trace:
        log = "/tmp/blowfish_oracle_trace.log"
        env = wine_env(display, "+module,+ole")
        with open(log, "w") as f:
            subprocess.Popen(["wine", "GAME.EXE"], cwd=GAMEDIR, env=env,
                             stdout=f, stderr=subprocess.STDOUT)
            time.sleep(min(opt.secs, 40))
            subprocess.run(["pkill", "-x", "wine"], capture_output=True)
        print("oracle_blowfish: trace %s (%d lines)" % (
            log, sum(1 for _ in open(log, errors="replace"))))

    summarize()
    subprocess.run(["pkill", "-x", "wine"], capture_output=True)
    subprocess.run(["pkill", "-x", "Xvfb"], capture_output=True)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
