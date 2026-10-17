#!/usr/bin/env python3
"""Read the REAL game's registers at a chosen address, to compare with the model.

The model can be told to record ESP/EAX/ECX/EDX at an address (TS_TRACE=addr).
This does the same thing to the retail binary under Wine, and prints it in the
same shape, so a divergence is visible rather than argued about.

    python3 tools/oracle.py 0x5e72f5
    python3 tools/oracle.py 0x5e72f5 --compare      # also run the model, diff

Why this exists: the project's only oracle used to be the project. Every "the
model behaves like the game" claim was the reconstruction agreeing with itself.
The retail binary runs on this host (see tools/wine_run.sh), so the question
"what is ESP here, really?" has an answer that does not come from our code.

WHAT BITES, all found the hard way:
  * Attaching needs kernel.yama.ptrace_scope=0. It is 1 by default here and the
    error gdb gives ("Inappropriate ioctl for device") does not mention it.
  * Do NOT break at the instruction AFTER ExitProcess. Wine terminates the
    process and never returns, so that breakpoint is never hit and gdb just sits
    there. Break at the call instruction itself.
  * `gdb --args wine` fails: /usr/bin/wine is a shell wrapper, not an ELF. Attach
    to the running GAME.EXE pid instead, which also guarantees the PE is mapped
    at its preferred base before any address is trusted.
  * Verify the mapping first. /proc/<pid>/maps must show
    00400000-00698000 r-xp ... GAME.EXE
    If it does not, every address below is meaningless.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
import threading
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPO = os.path.dirname(ROOT)
GAMEDIR = os.path.join(REPO, "gamedir")
PREFIX = os.path.join(REPO, ".wine32")
DISPLAY = ":78"
IMAGE_LO, IMAGE_HI = 0x00400000, 0x00698000


def die(msg, code=1):
    print("oracle: %s" % msg, file=sys.stderr)
    return code


def preflight():
    if not os.path.isfile(os.path.join(GAMEDIR, "GAME.EXE")):
        return die("no retail binary at %s (copy work/cd1/INSTALL there)" % GAMEDIR)
    if not os.path.isdir(PREFIX):
        return die("no 32-bit Wine prefix at %s" % PREFIX)
    for tool in ("wine", "Xvfb", "gdb"):
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


def env():
    e = dict(os.environ)
    e.update(WINEPREFIX=PREFIX, WINEARCH="win32", WINEDEBUG="-all", DISPLAY=DISPLAY)
    return e


def start_display():
    subprocess.run(["pkill", "-x", "Xvfb"], capture_output=True)
    time.sleep(1)
    subprocess.Popen(["Xvfb", DISPLAY, "-screen", "0", "1024x768x16"],
                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(2)


def game_pid():
    out = subprocess.run(["ps", "-eo", "pid=,comm="], capture_output=True,
                         text=True).stdout
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 2 and parts[1] == "GAME.EXE":
            return int(parts[0])
    return None


def mapped(pid):
    """True when the PE is at its preferred base -- the precondition for any VA."""
    try:
        for line in open("/proc/%d/maps" % pid):
            if "GAME.EXE" in line and line.lower().startswith("%08x" % IMAGE_LO):
                return True
    except OSError:
        pass
    return False


def wait_for_game(secs):
    """Start the game and wait until its image is mapped, then settle."""
    subprocess.run(["pkill", "-x", "wine"], capture_output=True)
    time.sleep(1)
    subprocess.Popen(["wine", "GAME.EXE"], cwd=GAMEDIR, env=env(),
                     stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    deadline = time.time() + secs
    pid = None
    while time.time() < deadline:
        pid = game_pid()
        if pid and mapped(pid):
            break
        time.sleep(0.5)
    return pid


def dismiss_loop(stop, secs=90):
    """Dismiss the game's startup dialogs while it runs.

    This is not optional. The retail game stops on a modal
    "Warning - Unable to create Direct Sound Object" dialog and waits for a
    click, so a breakpoint anywhere past startup is never reached until it is
    dismissed. gdb only stops the inferior at a breakpoint, so the game runs
    normally in between and an out-of-process dismisser works.
    """
    end = time.time() + secs
    while time.time() < end and not stop.is_set():
        try:
            tree = subprocess.run(["xwininfo", "-root", "-tree"], env=env(),
                                  capture_output=True, text=True, timeout=10).stdout
            for line in tree.splitlines():
                if '"Tiberian Sun"' not in line or "1024x768" in line:
                    continue
                m = re.search(r"(0x[0-9a-f]+)", line)
                if m:
                    subprocess.run(["xdotool", "key", "--window", m.group(1), "Return"],
                                   env=env(), capture_output=True, timeout=10)
        except (subprocess.SubprocessError, OSError):
            pass
        time.sleep(2)


def gdb_at(pid, addr, timeout):
    """Attach, break at addr, and report registers plus the top of the stack.

    Uses plain `info registers` and `x/8wx` rather than a custom printf. A printf
    that fails to evaluate prints nothing at all under -batch, so the failure looks
    identical to "the breakpoint was never hit" -- which cost a debugging cycle.
    Standard commands either work or say why.
    """
    cmd = ["gdb", "-batch", "-q", "-p", str(pid),
           "-ex", "set pagination off",
           "-ex", "set confirm off",
           "-ex", "break *0x%x" % addr,
           "-ex", "continue",
           "-ex", "info registers eip esp ebp ebx esi edi eax ecx edx",
           "-ex", "x/8wx $esp",
           "-ex", "detach"]
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        return r.stdout + r.stderr
    except subprocess.TimeoutExpired as e:
        def _s(x):
            if x is None:
                return ""
            return x.decode("utf-8", "replace") if isinstance(x, bytes) else x
        return _s(e.stdout) + _s(e.stderr)


def parse_gdb(out):
    """Pull eip/esp/... and the stack words out of `info registers` + `x/8wx`."""
    regs = {}
    for name in ("eip", "esp", "ebp", "ebx", "esi", "edi", "eax", "ecx", "edx"):
        m = re.search(r"\b%s\s+0x([0-9a-f]+)" % name, out)
        if m:
            regs[name] = int(m.group(1), 16)
    stack = []
    for line in out.splitlines():
        if re.match(r"^0x[0-9a-f]+:", line):
            for word in re.findall(r"0x([0-9a-f]{8})", line.split(":", 1)[1]):
                stack.append(int(word, 16))
    return regs, stack


def model_at(addr, budget):
    """The model's registers at the same address, via TS_TRACE."""
    drv = os.path.join(ROOT, "build", "boot_driver")
    if not os.path.isfile(drv):
        return None
    e = dict(os.environ, TS_TRACE="0x%x" % addr)
    try:
        r = subprocess.run([drv, "--budget=%d" % budget], cwd=ROOT, env=e,
                           capture_output=True, text=True, timeout=600)
    except subprocess.TimeoutExpired:
        return None
    for line in r.stdout.splitlines():
        if line.strip().startswith("esp=0x"):
            return line.strip()
    return None


def main(argv):
    ap = argparse.ArgumentParser()
    ap.add_argument("addr", help="address to break at, e.g. 0x5e72f5")
    ap.add_argument("--secs", type=int, default=45,
                    help="how long to wait for the game to map (default 45)")
    ap.add_argument("--gdb-timeout", type=int, default=120)
    ap.add_argument("--compare", action="store_true",
                    help="also read the model at the same address and diff")
    ap.add_argument("--budget", type=int, default=20000000)
    opt = ap.parse_args(argv)

    rc = preflight()
    if rc:
        return rc

    addr = int(opt.addr, 0)
    if not (IMAGE_LO <= addr < IMAGE_HI):
        print("oracle: warning: 0x%x is outside the image (%08x..%08x)"
              % (addr, IMAGE_LO, IMAGE_HI))

    start_display()
    pid = wait_for_game(opt.secs)
    if not pid:
        return die("the game never mapped at its preferred base")
    print("oracle: pid %d, GAME.EXE mapped at %08x" % (pid, IMAGE_LO))

    stop = threading.Event()
    t = threading.Thread(target=dismiss_loop, args=(stop,), daemon=True)
    t.start()
    try:
        out = gdb_at(pid, addr, opt.gdb_timeout)
    finally:
        stop.set()
    real = [l for l in out.splitlines() if l.startswith("REAL ") or l.startswith("STACK")]
    if not real:
        print(out[-1500:], file=sys.stderr)
        return die("no breakpoint hit at 0x%08x -- the game did not reach it "
                   "(or gdb could not attach)" % addr)
    for line in real:
        print("  " + line)

    subprocess.run(["pkill", "-x", "wine"], capture_output=True)
    subprocess.run(["pkill", "-x", "Xvfb"], capture_output=True)

    if opt.compare:
        m = model_at(addr, opt.budget)
        if m is None:
            return die("could not read the model at 0x%08x" % addr)
        print("  MODEL " + m)
        model_esp = re.search(r"esp=0x([0-9a-f]+)", m)
        if "esp" in regs and model_esp:
            a, b = regs["esp"], int(model_esp.group(1), 16)
            print("  ESP delta: %+d bytes (real 0x%08x, model 0x%08x)"
                  % (b - a, a, b))
            if a != b:
                print("  DIVERGENCE -- the model's stack is not the game's here")
                return 1
            print("  ESP agrees")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
