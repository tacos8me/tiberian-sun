// build/render_selftest.cpp -- standalone exercise of Rendering.h and Input.h.
//
// No image, no Win32Stubs.h, no generated tree: this compiles against the
// machine model alone, so a failure here is a failure of the new layer rather
// than of the recovered program. tools/test_runtime.py compiles it at -m32,
// runs it, decodes the PNG it writes with Python's zlib, and recompiles it with
// a deliberate defect to prove the checks can fail.
//
//   build/render_selftest [out.png] [out.ppm]
//
// Exit 0 with "SELFTEST PASSED <n> checks"; exit 1 naming the failed check.

#include <cstdio>
#include <cstring>

#include "Machine.h"
#include "Rendering.h"
#include "Input.h"

using ts::u32;
using ts::u16;
using ts::i32;
using ts::u8;

namespace {

int g_checks = 0;
int g_fail = 0;

void check(bool ok, const char *what) {
    ++g_checks;
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_fail;
    }
}

u32 &scratch_bump() { static u32 b = 0x61000000u; return b; }

u32 scratch(u32 n) {
    u32 &b = scratch_bump();
    b = (b + 15u) & ~15u;
    u32 p = b;
    b += n + 16u;
    return p;
}

u32 mk_rect(i32 l, i32 t, i32 r, i32 b) {
    u32 p = scratch(16u);
    ts::st32(p + 0u, (u32)l);
    ts::st32(p + 4u, (u32)t);
    ts::st32(p + 8u, (u32)r);
    ts::st32(p + 12u, (u32)b);
    return p;
}

u32 mk_fx(u32 fill_color) {
    u32 p = scratch(100u);
    for (u32 i = 0; i < 100u; ++i) ts::st8(p + i, 0u);
    ts::st32(p + 0x00u, 100u);
    ts::st32(p + ts::rnd::DDBLTFX_FILLCOLOR, fill_color);
    return p;
}

u32 rgb_of(u32 obj, u32 x, u32 y) {
    ts::rnd::Surface *s = ts::rnd::find(obj);
    if (!s) return 0xffffffffu;
    return ts::rnd::pixel_rgb(s, ts::rnd::get_px(s, x, y));
}

bool is_rgb(u32 obj, u32 x, u32 y, u8 r, u8 g, u8 b) {
    // RGB565 quantises: 96 becomes 99, 224 becomes 222. Compare with the
    // tolerance the format itself imposes, not exact equality.
    u32 c = rgb_of(obj, x, y);
    i32 dr = (i32)((c >> 16) & 0xffu) - (i32)r;
    i32 dg = (i32)((c >> 8) & 0xffu) - (i32)g;
    i32 db = (i32)(c & 0xffu) - (i32)b;
    if (dr < 0) dr = -dr;
    if (dg < 0) dg = -dg;
    if (db < 0) db = -db;
    return dr <= 8 && dg <= 8 && db <= 8;
}

void draw_frame(u32 prim_obj) {
    ts::rnd::Surface *s = ts::rnd::find(prim_obj);
    if (!s) return;
    ts::rnd::clear(prim_obj, 8, 16, 40);   // dark blue field

    struct Box { i32 l, t, r, b; u8 cr, cg, cb; };
    static const Box boxes[] = {
        {   0,   0, 640,  40,  96,  96, 112 },   // top bar
        {   0, 440, 640, 480,  96,  96, 112 },   // bottom bar
        {  40,  80, 200, 200, 224,  32,  32 },   // red
        { 240,  80, 400, 200,  32, 224,  32 },   // green
        { 440,  80, 600, 200,  32,  32, 224 },   // blue
        {  40, 260, 200, 380, 240, 240, 240 },   // white
        { 240, 260, 400, 380, 240, 240,  32 },   // yellow
        { 440, 260, 600, 380,  32, 240, 240 },   // cyan
    };
    for (unsigned i = 0; i < sizeof(boxes) / sizeof(boxes[0]); ++i) {
        const Box &b = boxes[i];
        u32 fx = mk_fx(ts::rnd::pack_rgb(s, b.cr, b.cg, b.cb));
        u32 rc = mk_rect(b.l, b.t, b.r, b.b);
        ts::rnd::blt(prim_obj, rc, 0u, 0u,
                     ts::rnd::DDBLT_WAIT | ts::rnd::DDBLT_COLORFILL, fx);
    }

    // A diagonal written through Lock, so the dump exercises the same
    // lpSurface/pitch path the game uses.
    u32 desc = scratch(108u);
    if (ts::rnd::lock(prim_obj, 0u, desc, ts::rnd::DDLOCK_WAIT) == 0u) {
        u32 lp = ts::ld32(desc + 0x24u);
        u32 pitch = ts::ld32(desc + 0x10u);
        u32 white = ts::rnd::pack_rgb(s, 255, 255, 255);
        for (u32 i = 0; i < 200u; ++i)
            ts::st16(lp + (60u + i) * pitch + (40u + i) * 2u, (u16)white);
        ts::rnd::unlock(prim_obj, 0u);
    }
}

bool test_surfaces(const char *png_path, const char *ppm_path) {
    const u32 PRIM = 0x11110001u;
    const u32 OFF  = 0x11110002u;
    const u32 BACK = 0x11110003u;
    const u32 P8   = 0x11110004u;
    const u32 CK   = 0x11110006u;

    ts::rnd::Surface *prim = ts::rnd::create_simple(PRIM, 640, 480, 16, true);
    check(prim != 0, "primary surface created");
    if (!prim) return false;
    check(prim->pitch == 1280u, "primary pitch is 1280");
    check(prim->bpp == 16u, "primary is 16bpp");
    check(prim->rmask == 0x0000f800u && prim->gmask == 0x000007e0u &&
          prim->bmask == 0x0000001fu, "primary is RGB565");
    check(ts::rnd::primary() == prim, "primary() names the primary surface");

    check(ts::rnd::clear(PRIM, 255, 0, 0) == 0u, "clear primary red");
    check(is_rgb(PRIM, 0, 0, 255, 0, 0), "pixel (0,0) red");
    check(is_rgb(PRIM, 639, 479, 255, 0, 0), "pixel (639,479) red");

    ts::rnd::Surface *off = ts::rnd::create_simple(OFF, 64, 64, 16, false);
    check(off != 0 && off->pitch == 128u, "64x64 offscreen pitch is 128");
    check(ts::rnd::clear(OFF, 0, 255, 0) == 0u, "clear offscreen green");
    check(is_rgb(OFF, 63, 63, 0, 255, 0), "offscreen (63,63) green");

    // Colour fill through Blt(DDBLT_COLORFILL): the form the retail game uses
    // 13 times before its main menu. mk_fx writes the colour at DDBLTFX+0x50
    // (dwFillColor) and leaves +0x40 zero, so reading the wrong offset would
    // fill black and fail the blue checks below.
    u32 fx_blue = mk_fx(ts::rnd::pack_rgb(off, 0, 0, 255));
    u32 rc = mk_rect(16, 16, 48, 48);
    check(ts::rnd::blt(OFF, rc, 0u, 0u,
                       ts::rnd::DDBLT_WAIT | ts::rnd::DDBLT_COLORFILL,
                       fx_blue) == 0u, "Blt colour fill returns DD_OK");
    check(is_rgb(OFF, 15, 15, 0, 255, 0), "outside fill still green");
    check(is_rgb(OFF, 16, 16, 0, 0, 255), "fill top-left blue");
    check(is_rgb(OFF, 47, 47, 0, 0, 255), "fill bottom-right blue");
    check(is_rgb(OFF, 48, 48, 0, 255, 0), "past fill still green");

    // The fill colour is dwFillColor at +0x50, not the +0x40 field. Put a red
    // herring at +0x40 and a blue colour at +0x50: the fill must be blue.
    u32 fx_probe = mk_fx(ts::rnd::pack_rgb(off, 0, 0, 255));
    ts::st32(fx_probe + 0x40u, ts::rnd::pack_rgb(off, 255, 0, 0));
    u32 rc2 = mk_rect(48, 0, 64, 16);
    check(ts::rnd::blt(OFF, rc2, 0u, 0u,
                       ts::rnd::DDBLT_WAIT | ts::rnd::DDBLT_COLORFILL,
                       fx_probe) == 0u, "probe fill returns DD_OK");
    check(is_rgb(OFF, 50, 5, 0, 0, 255),
          "colour fill reads dwFillColor at +0x50, not +0x40");

    // Copy through Blt(DDBLT_WAIT) with a source surface: the form used 197
    // times. TS_MUTATION_BLT_XOFF moves this one pixel right.
    u32 dr = mk_rect(100, 50, 164, 114);
    check(ts::rnd::blt(PRIM, dr, OFF, 0u, ts::rnd::DDBLT_WAIT, 0u) == 0u,
          "Blt copy returns DD_OK");
    check(is_rgb(PRIM, 100, 50, 0, 255, 0), "copy top-left green");
    check(is_rgb(PRIM, 116, 66, 0, 0, 255), "copy sees the blue fill");
    check(is_rgb(PRIM, 147, 97, 0, 0, 255), "copy bottom-right of fill blue");
    check(is_rgb(PRIM, 148, 98, 0, 255, 0), "copy past fill green");
    check(is_rgb(PRIM, 99, 50, 255, 0, 0), "left of copy untouched");
    check(is_rgb(PRIM, 164, 114, 255, 0, 0), "past copy untouched");

    // Lock hands out the surface's own memory with its own pitch.
    u32 desc = scratch(108u);
    check(ts::rnd::lock(PRIM, 0u, desc, ts::rnd::DDLOCK_WAIT) == 0u,
          "Lock returns DD_OK");
    u32 lp = ts::ld32(desc + 0x24u);
    u32 pitch = ts::ld32(desc + 0x10u);
    check(pitch == 1280u, "Lock reports pitch 1280");
    check(lp != 0u, "Lock reports a non-null lpSurface");
    ts::st16(lp + 3u * pitch + 4u * 2u, (u16)ts::rnd::pack_rgb(prim, 255, 255, 255));
    check(ts::rnd::unlock(PRIM, 0u) == 0u, "Unlock returns DD_OK");
    check(is_rgb(PRIM, 4, 3, 255, 255, 255), "write through lpSurface landed");

    // GetSurfaceDesc / GetPixelFormat: the fields GAME.EXE 0x4874c0 reads.
    u32 d2 = scratch(108u);
    check(ts::rnd::get_surface_desc(OFF, d2) == 0u, "GetSurfaceDesc returns DD_OK");
    check(ts::ld32(d2 + 0x08u) == 64u, "desc height 64");
    check(ts::ld32(d2 + 0x0cu) == 64u, "desc width 64");
    check(ts::ld32(d2 + 0x10u) == 128u, "desc pitch 128");
    check(ts::ld32(d2 + 0x54u) == 16u, "desc bit count 16");
    check(ts::ld32(d2 + 0x58u) == 0x0000f800u &&
          ts::ld32(d2 + 0x5cu) == 0x000007e0u &&
          ts::ld32(d2 + 0x60u) == 0x0000001fu, "desc RGB565 masks");
    check(ts::ld32(d2 + 0x24u) == off->pixels, "desc lpSurface is the surface memory");
    u32 pf = scratch(32u);
    check(ts::rnd::write_pixel_format(OFF, pf) == 0u, "GetPixelFormat returns DD_OK");
    check(ts::ld32(pf + 0x00u) == 32u && ts::ld32(pf + 0x0cu) == 16u,
          "DDPIXELFORMAT size/bit count");
    check(ts::ld32(pf + 0x10u) == 0x0000f800u, "DDPIXELFORMAT red mask");

    // The display-mode descriptor: bit count at +0x54, not at the caps offset.
    u32 md = scratch(108u);
    ts::rnd::write_mode_desc(md, 640u, 480u, 16u);
    check(ts::ld32(md + 0x08u) == 480u && ts::ld32(md + 0x0cu) == 640u,
          "display mode height/width");
    check(ts::ld32(md + 0x10u) == 1280u, "display mode pitch 1280");
    check(ts::ld32(md + 0x54u) == 16u, "display mode bit count at +0x54");
    check(ts::ld32(md + 0x58u) == 0x0000f800u, "display mode red mask");

    // A front/back pair: Flip swaps the pixel buffers, so the frame the game
    // drew into the back buffer becomes the one primary() reads.
    ts::rnd::Surface *back = ts::rnd::create_simple(BACK, 640, 480, 16, false);
    check(back != 0, "back buffer created");
    ts::rnd::link(PRIM, BACK);
    check(ts::rnd::attached_obj(PRIM) == BACK, "primary reports its back buffer");
    check(ts::rnd::clear(BACK, 255, 255, 0) == 0u, "clear back buffer yellow");
    check(ts::rnd::flip(PRIM, 0u, 0u) == 0u, "Flip returns DD_OK");
    check(is_rgb(PRIM, 0, 0, 255, 255, 0), "flipped front is the back buffer");
    check(ts::rnd::find(PRIM)->flips == 1u, "flip counted");
    check(is_rgb(BACK, 4, 3, 255, 255, 255), "old front is now the back buffer");

    // 8bpp palette support (the game runs 16bpp, but the layer must not lie
    // about a format it accepts).
    u32 entries = scratch(256u * 4u);
    for (u32 i = 0; i < 256u; ++i) {
        ts::st8(entries + i * 4u + 0u, (u8)i);
        ts::st8(entries + i * 4u + 1u, (u8)(i * 2u));
        ts::st8(entries + i * 4u + 2u, (u8)(i * 3u));
        ts::st8(entries + i * 4u + 3u, 0u);
    }
    const u32 PAL = 0x11110005u;
    check(ts::rnd::palette_create(PAL, entries) != 0, "palette created");
    ts::rnd::Surface *p8 = ts::rnd::create_simple(P8, 16, 16, 8, false);
    check(p8 != 0 && p8->pitch == 16u, "8bpp surface pitch is 16");
    check(ts::rnd::set_palette(P8, PAL) == 0u, "SetPalette returns DD_OK");
    check(ts::rnd::fill(P8, 3u) == 0u, "fill 8bpp index 3");
    check(ts::rnd::pixel_rgb(p8, ts::rnd::get_px(p8, 0, 0)) ==
          ((3u << 16) | (6u << 8) | 9u), "8bpp palette maps index 3");

    // Source colour keying: a keyed pixel is not copied.
    ts::rnd::Surface *ck = ts::rnd::create_simple(CK, 4, 4, 16, false);
    check(ck != 0, "colour-key surface created");
    ts::rnd::clear(CK, 0, 255, 0);
    ts::rnd::put_px(ck, 1, 1, 0xf81fu);   // magenta in RGB565
    u32 key = scratch(8u);
    ts::st32(key + 0u, 0xf81fu);
    ts::st32(key + 4u, 0xf81fu);
    check(ts::rnd::set_color_key(CK, ts::rnd::DDCKEY_SRCBLT, key) == 0u,
          "SetColorKey returns DD_OK");
    u32 ckr = mk_rect(200, 200, 204, 204);
    check(ts::rnd::blt(PRIM, ckr, CK, 0u,
                       ts::rnd::DDBLT_WAIT | ts::rnd::DDBLT_KEYSRC, 0u) == 0u,
          "keyed Blt returns DD_OK");
    check(is_rgb(PRIM, 200, 200, 0, 255, 0), "keyed blit copies a normal pixel");
    check(is_rgb(PRIM, 201, 201, 255, 255, 0), "keyed pixel left the destination");

    // A synthetic frame, for the PNG/PPM dump and the independent decoder.
    draw_frame(PRIM);
    check(is_rgb(PRIM, 10, 10, 96, 96, 112), "frame top bar");
    check(is_rgb(PRIM, 100, 100, 224, 32, 32), "frame red box");
    check(is_rgb(PRIM, 300, 300, 240, 240, 32), "frame yellow box");
    check(is_rgb(PRIM, 500, 300, 32, 240, 240), "frame cyan box");
    check(is_rgb(PRIM, 60, 80, 255, 255, 255), "frame diagonal pixel");

    check(ts::rnd::dump_png(prim, png_path), "PNG dumped");
    check(ts::rnd::dump_ppm(prim, ppm_path), "PPM dumped");
    std::printf("frame png: %s\n", png_path);
    std::printf("frame ppm: %s\n", ppm_path);
    return true;
}

bool test_input() {
    static const char *script =
        "# tools/test_runtime.py drives this timeline\n"
        "@50   mousemove 320 240\n"
        "@100  lbuttondown\n"
        "@150  lbuttonup\n"
        "@200  keydown 0x41\n"
        "@250  keyup 0x41\n"
        "@300  text 0x42\n"
        "@350  keydown 0x14\n"
        "@360  keyup 0x14\n"
        "@400  wheel -120\n";

    check(ts::input::load_script(script), ts::input::last_error());
    check(ts::input::pending_messages() == 0u, "no messages before the first event");

    ts::input::advance(49u);
    check(ts::input::pending_messages() == 0u, "nothing due before 50 ms");
    check(ts::input::mouse_x() == 0 && ts::input::mouse_y() == 0,
          "cursor starts at the origin");

    ts::input::advance(50u);
    check(ts::input::mouse_x() == 320 && ts::input::mouse_y() == 240,
          "mousemove updates the cursor");
    u32 m = 0, w = 0, l = 0;
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_MOUSEMOVE,
          "mousemove synthesises WM_MOUSEMOVE");
    check((l & 0xffffu) == 320u && ((l >> 16) & 0xffffu) == 240u,
          "WM_MOUSEMOVE lParam is the cursor position");

    ts::input::advance(100u);
    check(ts::input::is_down(ts::input::VK_LBUTTON), "left button is down");
    u32 ka = ts::input::async_key_state(ts::input::VK_LBUTTON);
    check((ka & 0x8000u) != 0u, "GetAsyncKeyState high bit for a down button");
    check((ka & 0x0001u) != 0u, "GetAsyncKeyState latch set once");
    check((ts::input::async_key_state(ts::input::VK_LBUTTON) & 0x0001u) == 0u,
          "GetAsyncKeyState latch cleared by the read");
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_LBUTTONDOWN,
          "lbuttondown synthesises WM_LBUTTONDOWN");
    check(w == ts::input::MK_LBUTTON, "WM_LBUTTONDOWN wParam is MK_LBUTTON");

    ts::input::advance(150u);
    check(!ts::input::is_down(ts::input::VK_LBUTTON), "left button is up");
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_LBUTTONUP,
          "lbuttonup synthesises WM_LBUTTONUP");

    ts::input::advance(200u);
    check(ts::input::is_down(0x41u), "'A' is down");
    u32 kbd = scratch(256u);
    ts::input::keyboard_state(kbd);
    check(ts::ld8(kbd + 0x41u) == 0x80u, "GetKeyboardState reports 'A' down");
    check(ts::ld8(kbd + 0x42u) == 0x00u, "'B' is not down");
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_KEYDOWN,
          "keydown synthesises WM_KEYDOWN");
    check(w == 0x41u, "WM_KEYDOWN wParam is the virtual key");
    check(((l >> 16) & 0xffu) == 0x1eu, "WM_KEYDOWN lParam carries the scan code");

    ts::input::advance(250u);
    check(!ts::input::is_down(0x41u), "'A' is up");
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_KEYUP,
          "keyup synthesises WM_KEYUP");

    ts::input::advance(300u);
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_CHAR,
          "text synthesises WM_CHAR");
    check(w == 0x42u, "WM_CHAR wParam is the character");

    ts::input::advance(350u);
    check((ts::input::key_state(ts::input::VK_CAPITAL) & 0x0001u) != 0u,
          "Caps Lock toggles on keydown");
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_KEYDOWN &&
          w == ts::input::VK_CAPITAL, "Caps Lock keydown message");
    ts::input::advance(360u);
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_KEYUP &&
          w == ts::input::VK_CAPITAL, "Caps Lock keyup message");

    ts::input::advance(400u);
    check(ts::input::pop_message(&m, &w, &l) && m == ts::input::WM_MOUSEWHEEL,
          "wheel synthesises WM_MOUSEWHEEL");
    check(((w >> 16) & 0xffffu) == 0xff88u, "wheel delta -120 is in the high word");
    check(ts::input::pending_messages() == 0u, "timeline drained");

    // A malformed script must be refused, not silently shortened.
    check(!ts::input::load_script("@10 mousemove 1\n"),
          "malformed mousemove is rejected");
    check(ts::input::last_error()[0] != 0, "the rejection names a reason");
    return true;
}

}  // namespace

int main(int argc, char **argv) {
    const char *png = argc > 1 ? argv[1] : "out/frame_selftest.png";
    const char *ppm = argc > 2 ? argv[2] : "out/frame_selftest.ppm";

    test_surfaces(png, ppm);
    test_input();

    if (g_fail) {
        std::printf("SELFTEST FAILED: %d of %d checks\n", g_fail, g_checks);
        return 1;
    }
    std::printf("SELFTEST PASSED %d checks\n", g_checks);
    return 0;
}
