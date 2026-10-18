#!/usr/bin/env python3
"""Standalone test for the rendering and input layer.

Builds build/render_selftest.cpp against src/handwritten/Rendering.h and
src/handwritten/Input.h at the target ABI (-m32), runs it, decodes the PNG it
writes with Python's own zlib (an independent decoder, so a bug in the PNG
writer cannot hide), and then rebuilds the same test with a deliberate defect
to prove the checks can fail.

    python3 tools/test_runtime.py

Exit 0 on success. Nothing here touches the generated tree, the image, or
Win32Stubs.h; this is the layer's own test.

INTEGRATION POINTS (the exact lines to add to Win32Stubs.h, not applied here):

  after  #include "Machine.h"
      #include "Rendering.h"
      #include "Input.h"

  forward declarations (before dd_CreateSurface)
      inline u32 make_surface(u32 desc_va = 0u, u32 def_w = 800u,
                              u32 def_h = 600u);
      void k_sfs_Blt(); void k_sfs_BltFast(); void k_sfs_Flip();
      void k_sfs_Unlock(); void k_sfs_GetPixelFormat();
      void k_sfs_SetPalette(); void k_sfs_SetColorKey(); void k_sfs_GetCaps();

  dd_CreateSurface
      if (out) st32(out, make_surface(arg(1), 640u, 480u));
      // 640x480 is the mode the retail game sets (SetDisplayMode 640x400 then
      // 640x480, measured). A primary request carries no width/height, so the
      // defaults decide the front buffer's size.

  dd_GetDisplayMode (the existing body writes dwRGBBitCount at +0x68, which is
  ddsCaps.dwCaps; the game's wrapper reads it at +0x54)
      TS_BEGIN(dd_GetDisplayMode)
          ts::rnd::write_mode_desc(arg(1), SCREEN_W, SCREEN_H, SCREEN_BPP);
          stub_done(2, 0u);
      TS_END

  make_surface's definition
      inline u32 make_surface(u32 desc_va, u32 def_w, u32 def_h) { ...
          u32 obj = alloc_block(16u, 4u); st32(obj, vt);
          ts::rnd::create(desc_va, obj, def_w, def_h);   // NEW
          return obj; }

  make_com_vtable overrides
      over[5]  = &k_sfs_Blt;             // nargs 6 -- see the defect note below
      over[7]  = &k_sfs_BltFast;         // nargs 6
      over[11] = &k_sfs_Flip;            // nargs 3
      over[14] = &k_sfs_GetCaps;         // nargs 2
      over[21] = &k_sfs_GetPixelFormat;  // nargs 2
      over[25] = &k_sfs_Lock;            // nargs 4
      over[29] = &k_sfs_SetColorKey;     // nargs 3
      over[31] = &k_sfs_SetPalette;      // nargs 2
      over[32] = &k_sfs_Unlock;          // nargs 2

  bodies
      TS_BEGIN(sfs_GetSurfaceDesc) stub_done(2, ts::rnd::get_surface_desc(arg(0), arg(1))); TS_END
      TS_BEGIN(sfs_Lock)           stub_done(4, ts::rnd::lock(arg(0), arg(1), arg(2), arg(3))); TS_END
      TS_BEGIN(sfs_Unlock)         stub_done(2, ts::rnd::unlock(arg(0), arg(1))); TS_END
      TS_BEGIN(sfs_Blt)            stub_done(6, ts::rnd::blt(arg(0), arg(1), arg(2), arg(3), arg(4), arg(5))); TS_END
      TS_BEGIN(sfs_BltFast)        stub_done(6, ts::rnd::blt_fast(arg(0), arg(1), arg(2), arg(3), arg(4), arg(5))); TS_END
      TS_BEGIN(sfs_Flip)           stub_done(3, ts::rnd::flip(arg(0), arg(1), arg(2))); TS_END
      TS_BEGIN(sfs_GetCaps)        stub_done(2, ts::rnd::write_caps(arg(0), arg(1))); TS_END
      TS_BEGIN(sfs_GetPixelFormat) stub_done(2, ts::rnd::write_pixel_format(arg(0), arg(1))); TS_END
      TS_BEGIN(sfs_SetColorKey)    stub_done(3, ts::rnd::set_color_key(arg(0), arg(1), arg(2))); TS_END
      TS_BEGIN(sfs_SetPalette)     stub_done(2, ts::rnd::set_palette(arg(0), arg(1))); TS_END

  dd_CreatePalette
      if (arg(3)) { u32 pal = make_dd_palette(); ts::rnd::palette_create(pal, arg(2)); st32(arg(3), pal); }

  input (USER32)
      TS_BEGIN(GetAsyncKeyState) stub_done(1, ts::input::async_key_state(arg(0))); TS_END
      TS_BEGIN(GetKeyState)      stub_done(1, ts::input::key_state(arg(0))); TS_END
      TS_BEGIN(GetCursorPos)     ts::input::cursor_pos(arg(0)); stub_done(1, 1u); TS_END
      TS_BEGIN(SetCursorPos)     ts::input::set_cursor((i32)arg(0), (i32)arg(1)); stub_done(2, 1u); TS_END
      TS_BEGIN(PeekMessageA)
          ts::input::advance(tick());
          u32 im, iw, il;
          while (ts::input::pop_message(&im, &iw, &il))
              msg_push(wnd_handle(), im, iw, il);
          ... existing body ...

DEFECT FOUND WHILE VALIDATING THE PATCH. make_surface()'s nargs table says
Blt (slot 5) takes 5 words. GAME.EXE's own colour-fill call site pushes SIX
before `call *0x14(%edx)` at 0x4883f5:

    4883cc: push %eax          ; lpDDBltFx
    4883d4: push $0x1000400    ; dwFlags (WAIT|COLORFILL)
    4883d9: push $0x0          ; lpSrcRect
    4883f1: push $0x0          ; lpDDSrcSurface
    4883f3: push %ecx          ; lpDestRect
    4883f4: push %ebx          ; this

so the generic slot stub pops one word too few and leaves 4 bytes on the stack
per Blt (210 Blt calls before the main menu). The body above pops 6 and fixes
it. The other overridden slots agree with the game's call sites (BltFast 6,
Flip 3, GetCaps 2, GetPixelFormat 2, Lock 4, SetColorKey 3, SetPalette 2,
Unlock 2, GetSurfaceDesc 2).

FILL COLOUR, measured rather than assumed. gdb attached to the retail
GAME.EXE, breakpoint at its own `call *0x14(%edx)` (0x4883f5), captured all 11
startup colour fills: dwSize 100, dst rectangles (0,0)-(64,64), (0,0)-(640,480),
(0,0)-(472,480), (0,0)-(168,480), and every DDBLTFX field from +0x40 to +0x60
zero except a transient 1 at +0x50. dwFillColor (the DirectX 6 layout's +0x40)
is therefore 0 for every fill on this path: the game clears to black and blits
art over it. Rendering.h reads +0x40.
"""

import os
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

sys.dont_write_bytecode = True

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(ROOT, "src", "handwritten")
SHIM = os.path.join(ROOT, "build", "shim")
TEST = os.path.join(ROOT, "build", "render_selftest.cpp")

CXXFLAGS = ["-std=c++03", "-m32", "-O1", "-Wall", "-Wextra",
            "-I" + SRC, "-I" + SHIM]

failures = []


def fail(what):
    failures.append(what)
    print("FAIL: " + what)


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def check_headers(tmp):
    """Both headers must compile standalone at the target ABI."""
    for h in ("Rendering.h", "Input.h"):
        r = run(["g++"] + CXXFLAGS[:2] + ["-fsyntax-only", "-I" + SRC,
                                          "-I" + SHIM, "-include",
                                          os.path.join(SRC, h),
                                          "-x", "c++", os.devnull])
        if r.returncode != 0:
            fail("standalone compile of %s: %s" % (h, r.stderr.strip()[:400]))
    print("[1] standalone headers compile at -m32")


def build(tmp, name, defines=()):
    exe = os.path.join(tmp, name)
    cmd = ["g++"] + CXXFLAGS + list(defines) + [TEST, "-o", exe]
    r = run(cmd)
    if r.returncode != 0:
        fail("compile %s: %s" % (name, r.stderr.strip()[:1200]))
        return None
    return exe


def decode_png(path):
    """Decode an 8-bit RGB PNG with no external dependency.

    Supports exactly what Rendering.h writes: colour type 2, bit depth 8,
    filter type 0. Returns (width, height, rows) where rows is a list of
    bytearrays of RGB triples. Raises on anything else, so a malformed writer
    fails the test rather than passing it.
    """
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("not a PNG")
    pos = 8
    w = h = None
    idat = b""
    seen_end = False
    while pos < len(data):
        (length,) = struct.unpack(">I", data[pos:pos + 4])
        ctype = data[pos + 4:pos + 8]
        body = data[pos + 8:pos + 8 + length]
        crc = data[pos + 8 + length:pos + 12 + length]
        if len(crc) != 4:
            raise ValueError("truncated chunk")
        if zlib.crc32(ctype + body) & 0xffffffff != struct.unpack(">I", crc)[0]:
            raise ValueError("bad CRC in %r" % ctype)
        if ctype == b"IHDR":
            w, h, depth, colour, comp, filt, inter = struct.unpack(">IIBBBBB", body)
            if (depth, colour, comp, filt, inter) != (8, 2, 0, 0, 0):
                raise ValueError("unexpected PNG format")
        elif ctype == b"IDAT":
            idat += body
        elif ctype == b"IEND":
            seen_end = True
        pos += 12 + length
    if not seen_end or w is None:
        raise ValueError("missing IEND/IHDR")
    raw = zlib.decompress(idat)
    stride = w * 3 + 1
    if len(raw) != h * stride:
        raise ValueError("raw size %d != %d" % (len(raw), h * stride))
    rows = []
    for y in range(h):
        row = raw[y * stride:(y + 1) * stride]
        if row[0] != 0:
            raise ValueError("unexpected filter %d" % row[0])
        rows.append(bytearray(row[1:]))
    return w, h, rows


def pixel(rows, x, y):
    o = x * 3
    return rows[y][o], rows[y][o + 1], rows[y][o + 2]


def near(got, want, tol=8):
    return all(abs(a - b) <= tol for a, b in zip(got, want))


def check_png(path):
    try:
        w, h, rows = decode_png(path)
    except Exception as e:  # noqa: BLE001 - report the decoder's own words
        fail("PNG decode: %s" % e)
        return
    if (w, h) != (640, 480):
        fail("PNG is %dx%d, expected 640x480" % (w, h))
        return
    want = [
        (10, 10, (96, 96, 112), "top bar"),
        (100, 100, (224, 32, 32), "red box"),
        (300, 100, (32, 224, 32), "green box"),
        (500, 100, (32, 32, 224), "blue box"),
        (100, 300, (240, 240, 240), "white box"),
        (300, 300, (240, 240, 32), "yellow box"),
        (500, 300, (32, 240, 240), "cyan box"),
        (10, 300, (8, 16, 40), "background"),
        (60, 80, (255, 255, 255), "diagonal through Lock"),
        (400, 400, (8, 16, 40), "background lower right"),
    ]
    for x, y, rgb, what in want:
        got = pixel(rows, x, y)
        if not near(got, rgb):
            fail("PNG pixel (%d,%d) is %r, expected %r (%s)" % (x, y, got, rgb, what))
    print("[2] PNG independently decoded: 640x480, %d pixels checked" % len(want))


def check_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    header, rest = data.split(b"\n255\n", 1)
    fields = header.split()
    if fields[0] != b"P6" or int(fields[1]) != 640 or int(fields[2]) != 480:
        fail("PPM header is %r" % header)
        return
    if len(rest) != 640 * 480 * 3:
        fail("PPM body is %d bytes" % len(rest))
        return
    o = (100 * 640 + 100) * 3
    got = (rest[o], rest[o + 1], rest[o + 2])
    if not near(got, (224, 32, 32)):
        fail("PPM pixel (100,100) is %r" % (got,))
    print("[3] PPM decoded: 640x480, pixel (100,100) red")


def run_selftest(exe, tmp, tag):
    png = os.path.join(tmp, tag + ".png")
    ppm = os.path.join(tmp, tag + ".ppm")
    r = run([exe, png, ppm])
    return r, png, ppm


def main():
    tmp = tempfile.mkdtemp(prefix="ts_runtime_")
    try:
        check_headers(tmp)

        exe = build(tmp, "selftest")
        if not exe:
            return report()
        r, png, ppm = run_selftest(exe, tmp, "frame")
        if r.returncode != 0:
            fail("selftest exited %d:\n%s%s" % (r.returncode, r.stdout, r.stderr))
        elif "SELFTEST PASSED" not in r.stdout:
            fail("selftest did not report success:\n%s" % r.stdout)
        else:
            n = r.stdout.strip().splitlines()[-1]
            print("[4] selftest: " + n)
        if os.path.exists(png):
            check_png(png)
        else:
            fail("selftest wrote no PNG")
        if os.path.exists(ppm):
            check_ppm(ppm)
        else:
            fail("selftest wrote no PPM")

        # Mutation 1: the blit copy lands one pixel to the right. The pixel
        # checks must notice, or they are not checking the copy.
        m1 = build(tmp, "selftest_blt_mutant", ["-DTS_MUTATION_BLT_XOFF"])
        if m1:
            r1, _, _ = run_selftest(m1, tmp, "mutant_blt")
            if r1.returncode == 0 or "SELFTEST FAILED" not in r1.stdout:
                fail("mutation BLT_XOFF was not caught (rc=%d)\n%s"
                     % (r1.returncode, r1.stdout))
            else:
                print("[5] mutation BLT_XOFF caught: " +
                      r1.stdout.strip().splitlines()[-1])

        # Mutation 2: GetAsyncKeyState always reports no key. The input checks
        # must notice, or they are not checking the key table.
        m2 = build(tmp, "selftest_key_mutant", ["-DTS_MUTATION_KEY_ZERO"])
        if m2:
            r2, _, _ = run_selftest(m2, tmp, "mutant_key")
            if r2.returncode == 0 or "SELFTEST FAILED" not in r2.stdout:
                fail("mutation KEY_ZERO was not caught (rc=%d)\n%s"
                     % (r2.returncode, r2.stdout))
            else:
                print("[6] mutation KEY_ZERO caught: " +
                      r2.stdout.strip().splitlines()[-1])

        if not failures:
            print("RUNTIME TEST PASSED")
            print("frame dump: %s" % os.path.join(tmp, "frame.png"))
        return report()
    finally:
        if failures:
            print("artifacts kept in %s" % tmp)
        else:
            shutil.rmtree(tmp, ignore_errors=True)


def report():
    if failures:
        print("RUNTIME TEST FAILED: %d problem(s)" % len(failures))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
