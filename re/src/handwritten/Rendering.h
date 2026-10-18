#ifndef TS_RENDERING_H
#define TS_RENDERING_H

// ---------------------------------------------------------------------------
// Rendering.h -- a real surface/framebuffer layer for the model's DirectDraw.
//
// PROVENANCE. This header is OUR model, not recovered data. Its shapes are
// taken from the retail binary's own DirectDraw usage, measured with
//
//   tools/wine_run.sh --trace ddraw --cd ... --screen 640x480x16
//
// on this host. What that trace establishes (210 Blt, 195 Lock, 7 CreateSurface
// calls before the main menu):
//
//   * every surface is 16bpp RGB565 -- R 0x0000f800, G 0x000007e0,
//     B 0x0000001f -- and lPitch is width*2 (640x480 -> 1280, 64x64 -> 128);
//   * the primary surface is 640x480; the offscreen surfaces are 64x64,
//     472x480, 168x480 and 640x480;
//   * 197 of the 210 Blts are copies with flags 0x01000000 (DDBLT_WAIT) and a
//     source surface; 13 are colour fills with flags 0x01000400
//     (DDBLT_WAIT|DDBLT_COLORFILL), src_surface NULL and a DDBLTFX;
//   * every Lock uses flags 0x1 (DDLOCK_WAIT) and a NULL rect;
//   * the game NEVER calls Flip or SetPalette on this path. It blits into the
//     primary surface directly.
//   * the game's own wrapper (GAME.EXE 0x4874c0) reads a DDSURFACEDESC as
//     +0x08 height, +0x0c width, +0x10 lPitch, +0x24 lpSurface, +0x54
//     dwRGBBitCount and +0x68 ddsCaps.dwCaps, and computes bytes-per-pixel as
//     (bpp+7)>>3.
//
// WHY. The stubs in Win32Stubs.h give every surface the SAME static buffer and
// make Blt/BltFast/Flip no-ops, so nothing the game draws can be observed and
// every surface aliases every other one. This layer gives each surface its own
// pitch-sized pixel memory in the machine model's address space (so the
// transcribed program reads and writes it through st8/ld8), implements the
// DirectDraw operations the game actually calls, and can dump the primary
// surface as a PNG or PPM.
//
// INTEGRATION. This header does not include Win32Stubs.h. The stub layer owns
// the COM objects and the model stack; this layer owns the pixels. The
// one-line bodies that connect them are listed in tools/test_runtime.py and in
// the report that accompanies this file.
//
// Header-only, C++03, like every other handwritten header: the pipeline wipes
// src/generated, so nothing here may depend on a hand-written .cpp.
// ---------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Machine.h"

namespace ts {
namespace rnd {

// -- DirectDraw constants used by the game -----------------------------------
enum {
    DD_OK                       = 0x00000000u,
    DDERR_INVALIDOBJECT         = 0x88760066u,
    DDERR_INVALIDPARAMS         = 0x80070057u,
    DDERR_NOTFOUND              = 0x887601A2u,
    DDERR_SURFACELOST           = 0x887601C2u,

    DDSD_CAPS                   = 0x00000001u,
    DDSD_HEIGHT                 = 0x00000002u,
    DDSD_WIDTH                  = 0x00000004u,
    DDSD_PITCH                  = 0x00000040u,
    DDSD_LPSURFACE              = 0x00000800u,
    DDSD_PIXELFORMAT            = 0x00001000u,

    DDSCAPS_PRIMARYSURFACE      = 0x00000200u,
    DDSCAPS_OFFSCREENPLAIN      = 0x00000040u,
    DDSCAPS_SYSTEMMEMORY        = 0x00000800u,
    DDSCAPS_VIDEOMEMORY         = 0x00004000u,
    DDSCAPS_VISIBLE             = 0x00008000u,
    DDSCAPS_LOCALVIDMEM         = 0x10000000u,
    DDSCAPS_BACKBUFFER          = 0x00000004u,

    DDPF_RGB                    = 0x00000040u,
    DDPF_PALETTEINDEXED8        = 0x00000020u,

    DDBLT_WAIT                  = 0x01000000u,
    DDBLT_COLORFILL             = 0x00000400u,
    DDBLT_KEYSRC                = 0x00008000u,
    DDBLT_KEYDEST               = 0x00002000u,
    DDBLT_ASYNC                 = 0x00000200u,

    DDBLTFAST_SRCCOLORKEY       = 0x00000001u,
    DDBLTFAST_DESTCOLORKEY      = 0x00000002u,
    DDBLTFAST_WAIT              = 0x00000010u,

    DDLOCK_WAIT                 = 0x00000001u,
    DDLOCK_SURFACEMEMORYPTR     = 0x00000000u,

    DDCKEY_SRCBLT               = 0x00000008u,
    DDCKEY_DESTBLT              = 0x00000002u,

    MAX_SURFACES                = 64,
    MAX_PALETTES                = 16,
    PALETTE_ENTRIES             = 256,

    // DDBLTFX.dwFillColor. The DirectX 6 layout puts it at +0x50, in a union
    // after dwAlphaSrcConst (dwSize +0x00 .. dwAlphaSrcConst +0x4c). This is
    // confirmed twice: the bundled ddraw.h's field order, and GAME.EXE's own
    // fill helper, which stores the colour at 0x50 from its pushed argument
    // (0x4883cd, `mov %ecx,0xb0(%esp)` after the fx push). The struct is 100
    // bytes: dwFillColor +0x50, ddckDestColorkey +0x54, ddckSrcColorkey +0x5c.
    DDBLTFX_FILLCOLOR           = 0x50u
};

// -- model-space memory ------------------------------------------------------
// Pixel memory lives in the machine model's address space, not on the host
// heap: the transcribed program addresses it with st8/ld8. 0x10000000 is clear
// of the image (0x400000..0x698000), the model stack (0x300000), the Win32
// stub allocator (0x50000000) and the synthetic stub VAs (0x7f000000+). It used
// to be 0x60000000, which the stub heap grows into: by ~2.72e9 instructions the
// heap had overwritten the primary surface and the loading screen, and the
// "frame" was raw heap memory read as RGB565. Worst case here (64 surfaces of
// 640x480x4) ends at 0x14b00000, still below the heap.
inline u32 &mem_bump() { static u32 b = 0x10000000u; return b; }

inline u32 mem_alloc(u32 size, u32 align) {
    if (size == 0u) size = 1u;
    if (align < 1u) align = 1u;
    u32 &b = mem_bump();
    b = (b + align - 1u) & ~(align - 1u);
    u32 p = b;
    b += size;
    return p;
}

// Copy through the model's page table, chunked so a run never crosses a page
// boundary inside one memcpy. Correctness first: the page() lookup is what
// makes the bytes visible to the game.
inline void mem_write(u32 dst, const void *src, u32 n) {
    const u8 *s = (const u8 *)src;
    while (n) {
        u32 chunk = 4096u - (dst & 0xfffu);
        if (chunk > n) chunk = n;
        std::memcpy(page(dst), s, chunk);
        dst += chunk;
        s += chunk;
        n -= chunk;
    }
}

inline void mem_copy(u32 dst, u32 src, u32 n) {
    while (n) {
        u32 dchunk = 4096u - (dst & 0xfffu);
        u32 schunk = 4096u - (src & 0xfffu);
        u32 chunk = dchunk < schunk ? dchunk : schunk;
        if (chunk > n) chunk = n;
        std::memcpy(page(dst), page(src), chunk);
        dst += chunk;
        src += chunk;
        n -= chunk;
    }
}

// Fill `count` pixels of `bytes` bytes each with `raw`, through the page table.
inline void mem_fill(u32 addr, u32 bytes, u32 raw, u32 count) {
    u8 pat[4];
    pat[0] = (u8)(raw);
    pat[1] = (u8)(raw >> 8);
    pat[2] = (u8)(raw >> 16);
    pat[3] = (u8)(raw >> 24);
    while (count) {
        u32 room = 4096u - (addr & 0xfffu);
        u32 fit = room / bytes;
        if (fit > count) fit = count;
        if (fit == 0u) {
            // One pixel straddles a page boundary.
            for (u32 i = 0; i < bytes; ++i) st8(addr + i, pat[i]);
            addr += bytes;
            --count;
            continue;
        }
        u8 *p = page(addr);
        for (u32 i = 0; i < fit; ++i) std::memcpy(p + i * bytes, pat, bytes);
        addr += fit * bytes;
        count -= fit;
    }
}

// -- pixel format ------------------------------------------------------------
inline u32 mask_shift(u32 mask) {
    u32 s = 0u;
    if (!mask) return 0u;
    while (!(mask & 1u)) { mask >>= 1; ++s; }
    return s;
}

inline u32 mask_bits(u32 mask) {
    u32 n = 0u;
    while (mask) { n += (mask & 1u); mask >>= 1; }
    return n;
}

inline u32 pack_channel(u32 v, u32 mask) {
    u32 bits = mask_bits(mask);
    if (!bits) return 0u;
    u32 maxv = (1u << bits) - 1u;
    return ((v * maxv + 127u) / 255u) << mask_shift(mask);
}

inline u8 unpack_channel(u32 px, u32 mask) {
    u32 bits = mask_bits(mask);
    if (!bits) return 0u;
    u32 maxv = (1u << bits) - 1u;
    u32 v = (px & mask) >> mask_shift(mask);
    return (u8)((v * 255u + maxv / 2u) / maxv);
}

struct Surface {
    u32 obj;              // COM object VA in the model (0 for an internal one)
    u32 pixels;           // model VA of the top-left pixel
    u32 width, height;    // pixels
    u32 pitch;            // bytes per row
    u16 bpp;              // 8, 16 or 32
    u32 rmask, gmask, bmask;
    u32 caps;             // ddsCaps.dwCaps
    bool primary;
    bool locked;
    u32 lock_x, lock_y, lock_w, lock_h;
    bool has_colorkey;
    u32 colorkey_low, colorkey_high;
    bool has_palette;
    u32 palette[PALETTE_ENTRIES];   // 0x00RRGGBB
    Surface *attached;    // back buffer, if any
    u32 flips;

    Surface()
        : obj(0), pixels(0), width(0), height(0), pitch(0), bpp(16),
          rmask(0x0000f800u), gmask(0x000007e0u), bmask(0x0000001fu),
          caps(0), primary(false), locked(false),
          lock_x(0), lock_y(0), lock_w(0), lock_h(0),
          has_colorkey(false), colorkey_low(0), colorkey_high(0),
          has_palette(false), attached(0), flips(0) {
        for (unsigned i = 0; i < PALETTE_ENTRIES; ++i) palette[i] = 0u;
    }
};

inline Surface *surface_table() { static Surface t[MAX_SURFACES]; return t; }
inline unsigned &surface_n() { static unsigned n = 0u; return n; }
inline Surface *&primary_surface() { static Surface *p = 0; return p; }

// Operation counters. A frame that is blank because the game never blitted and
// a frame that is blank because every blit landed off-surface look identical in
// a PNG; these say which. They are cheap and the boot report prints them.
inline unsigned long &stat_blits() { static unsigned long n = 0; return n; }
inline unsigned long &stat_fills() { static unsigned long n = 0; return n; }
inline unsigned long &stat_locks() { static unsigned long n = 0; return n; }
inline unsigned long &stat_flips() { static unsigned long n = 0; return n; }

inline Surface *find(u32 obj) {
    if (!obj) return 0;
    Surface *t = surface_table();
    for (unsigned i = 0; i < surface_n(); ++i)
        if (t[i].obj == obj && t[i].pixels) return &t[i];
    return 0;
}

inline Surface *find_ptr(const Surface *s) { return const_cast<Surface *>(s); }

inline u32 bytes_per_pixel(const Surface *s) { return (u32)(s->bpp >> 3); }

inline Surface *create_simple(u32 obj, u32 w, u32 h, u16 bpp, bool primary) {
    if (surface_n() >= (unsigned)MAX_SURFACES) return 0;
    Surface *s = &surface_table()[surface_n()++];
    *s = Surface();
    s->obj = obj;
    s->width = w ? w : 1u;
    s->height = h ? h : 1u;
    s->bpp = (bpp == 8u || bpp == 16u || bpp == 32u) ? bpp : 16u;
    s->primary = primary;
    if (s->bpp == 32u) {
        s->rmask = 0x00ff0000u; s->gmask = 0x0000ff00u; s->bmask = 0x000000ffu;
    } else if (s->bpp == 8u) {
        s->rmask = s->gmask = s->bmask = 0u;
    }
    s->pitch = s->width * bytes_per_pixel(s);
    s->pixels = mem_alloc(s->pitch * s->height, 4u);
    s->caps = primary
        ? (DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY | DDSCAPS_VISIBLE |
           DDSCAPS_LOCALVIDMEM)
        : (DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY);
    if (primary) primary_surface() = s;
    return s;
}

// Parse a DDSURFACEDESC (DirectX 6, 108 bytes) as the game writes and reads it.
// `def_w`/`def_h` are used when the descriptor leaves the geometry at zero,
// which is what a primary-surface request does.
inline Surface *create(u32 desc_va, u32 obj, u32 def_w, u32 def_h) {
    u32 w = def_w, h = def_h, bpp = 16u;
    u32 rm = 0x0000f800u, gm = 0x000007e0u, bm = 0x0000001fu;
    u32 caps = 0u;
    if (desc_va) {
        h = ld32(desc_va + 0x08u);
        w = ld32(desc_va + 0x0cu);
        bpp = ld32(desc_va + 0x54u);
        u32 r = ld32(desc_va + 0x58u);
        u32 g = ld32(desc_va + 0x5cu);
        u32 b = ld32(desc_va + 0x60u);
        if (r || g || b) { rm = r; gm = g; bm = b; }
        caps = ld32(desc_va + 0x68u);
    }
    if (!w) w = def_w;
    if (!h) h = def_h;
    if (bpp != 8u && bpp != 16u && bpp != 32u) bpp = 16u;
    bool prim = (caps & DDSCAPS_PRIMARYSURFACE) != 0u;
    Surface *s = create_simple(obj, w, h, (u16)bpp, prim);
    if (!s) return 0;
    s->rmask = rm; s->gmask = gm; s->bmask = bm;
    if (caps) s->caps = caps;
    return s;
}

// A surface created without a descriptor (GetAttachedSurface's old path).
inline Surface *create_default(u32 obj, u32 w, u32 h) {
    return create(0u, obj, w, h);
}

inline void link(u32 parent_obj, u32 child_obj) {
    Surface *p = find(parent_obj);
    Surface *c = find(child_obj);
    if (p) p->attached = c;
}

inline u32 attached_obj(u32 parent_obj) {
    Surface *p = find(parent_obj);
    return (p && p->attached) ? p->attached->obj : 0u;
}

inline u32 width_of(u32 obj)  { Surface *s = find(obj); return s ? s->width  : 0u; }
inline u32 height_of(u32 obj) { Surface *s = find(obj); return s ? s->height : 0u; }

// -- pixel access ------------------------------------------------------------
inline u32 get_px(const Surface *s, u32 x, u32 y) {
    u32 a = s->pixels + y * s->pitch + x * bytes_per_pixel(s);
    if (s->bpp == 8u)  return ld8(a);
    if (s->bpp == 16u) return ld16(a);
    return ld32(a);
}

inline void put_px(Surface *s, u32 x, u32 y, u32 raw) {
    u32 a = s->pixels + y * s->pitch + x * bytes_per_pixel(s);
    if (s->bpp == 8u)  { st8(a, (u8)raw); return; }
    if (s->bpp == 16u) { st16(a, (u16)raw); return; }
    st32(a, raw);
}

// 0x00RRGGBB for a raw pixel, through the surface's palette or channel masks.
inline u32 pixel_rgb(const Surface *s, u32 raw) {
    if (s->bpp == 8u) {
        u32 idx = raw & 0xffu;
        return s->has_palette ? s->palette[idx] : (idx * 0x00010101u);
    }
    u32 r = unpack_channel(raw, s->rmask);
    u32 g = unpack_channel(raw, s->gmask);
    u32 b = unpack_channel(raw, s->bmask);
    return (r << 16) | (g << 8) | b;
}

inline u32 pack_rgb(const Surface *s, u8 r, u8 g, u8 b) {
    if (s->bpp == 8u) {
        // Nearest palette entry by squared distance, so a test can name a
        // colour without knowing an index.
        u32 best = 0u, bestd = 0xffffffffu;
        for (u32 i = 0; i < PALETTE_ENTRIES; ++i) {
            u32 pr = (s->palette[i] >> 16) & 0xffu;
            u32 pg = (s->palette[i] >> 8) & 0xffu;
            u32 pb = s->palette[i] & 0xffu;
            u32 dr = pr > r ? pr - r : r - pr;
            u32 dg = pg > g ? pg - g : g - pg;
            u32 db = pb > b ? pb - b : b - pb;
            u32 d = dr * dr + dg * dg + db * db;
            if (d < bestd) { bestd = d; best = i; }
        }
        return best;
    }
    return pack_channel(r, s->rmask) | pack_channel(g, s->gmask) |
           pack_channel(b, s->bmask);
}

// -- rectangle / descriptor helpers -----------------------------------------
struct Rect { i32 l, t, r, b; };

inline Rect rect_from(u32 va) {
    Rect q;
    q.l = (i32)ld32(va + 0u);
    q.t = (i32)ld32(va + 4u);
    q.r = (i32)ld32(va + 8u);
    q.b = (i32)ld32(va + 12u);
    return q;
}

inline Rect whole(const Surface *s) {
    Rect q;
    q.l = 0; q.t = 0; q.r = (i32)s->width; q.b = (i32)s->height;
    return q;
}

// Intersect with the surface and with a source rect's extent. Returns false
// when the result is empty.
inline bool clip_rect(Rect &q, const Surface *s) {
    if (q.l < 0) q.l = 0;
    if (q.t < 0) q.t = 0;
    if (q.r > (i32)s->width) q.r = (i32)s->width;
    if (q.b > (i32)s->height) q.b = (i32)s->height;
    return q.r > q.l && q.b > q.t;
}

// Write the DDSURFACEDESC fields GAME.EXE actually reads (0x4874c0/0x487ad0):
// +0x08 height, +0x0c width, +0x10 lPitch, +0x24 lpSurface, +0x54 bit count,
// +0x58/+0x5c/+0x60 channel masks, +0x68 ddsCaps. The flags word is the
// proven 0x41 (DDSD_CAPS|DDSD_PITCH) the existing stubs wrote.
inline bool write_desc(u32 obj, u32 va) {
    Surface *s = find(obj);
    if (!va) return false;
    for (u32 i = 0; i < 108u; ++i) st8(va + i, 0);
    st32(va + 0x00u, 108u);
    st32(va + 0x04u, DDSD_CAPS | DDSD_PITCH);
    if (!s) return false;
    u32 lp = s->pixels;
    u32 pitch = s->pitch;
    if (s->locked) lp += s->lock_y * s->pitch + s->lock_x * bytes_per_pixel(s);
    st32(va + 0x08u, s->height);
    st32(va + 0x0cu, s->width);
    st32(va + 0x10u, pitch);
    st32(va + 0x24u, lp);
    st32(va + 0x48u, 32u);                 // ddpfPixelFormat.dwSize
    st32(va + 0x4cu, s->bpp == 8u ? DDPF_PALETTEINDEXED8 : DDPF_RGB);
    st32(va + 0x50u, 0u);                  // dwFourCC
    st32(va + 0x54u, s->bpp);
    st32(va + 0x58u, s->rmask);
    st32(va + 0x5cu, s->gmask);
    st32(va + 0x60u, s->bmask);
    st32(va + 0x64u, 0u);                  // alpha mask
    st32(va + 0x68u, s->caps);
    return true;
}

// The display-mode descriptor IDirectDraw::GetDisplayMode returns. The
// existing stub writes dwRGBBitCount at +0x68, which is ddsCaps.dwCaps; the
// game's own wrapper (0x4874c0) reads the bit count at +0x54. The retail trace
// shows GetDisplayMode returning height 480, width 640, pitch 1280 and
// RGB565, so this writes the whole descriptor the wrapper reads.
inline void write_mode_desc(u32 va, u32 w, u32 h, u16 bpp) {
    if (!va) return;
    for (u32 i = 0; i < 108u; ++i) st8(va + i, 0);
    u32 rm = 0x0000f800u, gm = 0x000007e0u, bm = 0x0000001fu;
    if (bpp == 8u) { rm = gm = bm = 0u; }
    else if (bpp == 32u) { rm = 0x00ff0000u; gm = 0x0000ff00u; bm = 0x000000ffu; }
    st32(va + 0x00u, 108u);
    st32(va + 0x04u, DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH |
                      DDSD_PIXELFORMAT);
    st32(va + 0x08u, h);
    st32(va + 0x0cu, w);
    st32(va + 0x10u, w * (u32)(bpp >> 3));
    st32(va + 0x48u, 32u);
    st32(va + 0x4cu, bpp == 8u ? DDPF_PALETTEINDEXED8 : DDPF_RGB);
    st32(va + 0x54u, bpp);
    st32(va + 0x58u, rm);
    st32(va + 0x5cu, gm);
    st32(va + 0x60u, bm);
}

inline u32 write_pixel_format(u32 obj, u32 va) {
    Surface *s = find(obj);
    if (!va) return DDERR_INVALIDPARAMS;
    for (u32 i = 0; i < 32u; ++i) st8(va + i, 0);
    if (!s) return DDERR_INVALIDOBJECT;
    st32(va + 0x00u, 32u);
    st32(va + 0x04u, s->bpp == 8u ? DDPF_PALETTEINDEXED8 : DDPF_RGB);
    st32(va + 0x08u, 0u);
    st32(va + 0x0cu, s->bpp);
    st32(va + 0x10u, s->rmask);
    st32(va + 0x14u, s->gmask);
    st32(va + 0x18u, s->bmask);
    st32(va + 0x1cu, 0u);
    return DD_OK;
}

inline u32 write_caps(u32 obj, u32 va) {
    Surface *s = find(obj);
    if (!va) return DDERR_INVALIDPARAMS;
    if (!s) return DDERR_INVALIDOBJECT;
    st32(va + 0x00u, s->caps);
    st32(va + 0x04u, 0u);
    st32(va + 0x08u, 0u);
    st32(va + 0x0cu, 0u);
    return DD_OK;
}

// -- operations --------------------------------------------------------------
inline u32 fill_rect_obj(u32 obj, const Rect &rect, u32 raw) {
    Surface *s = find(obj);
    if (!s) return DDERR_INVALIDOBJECT;
    ++stat_fills();
    Rect q = rect;
    if (!clip_rect(q, s)) return DD_OK;
    u32 bytes = bytes_per_pixel(s);
    for (i32 y = q.t; y < q.b; ++y) {
        u32 addr = s->pixels + (u32)y * s->pitch + (u32)q.l * bytes;
        mem_fill(addr, bytes, raw, (u32)(q.r - q.l));
    }
    return DD_OK;
}

inline u32 fill(u32 obj, u32 raw) {
    Surface *s = find(obj);
    if (!s) return DDERR_INVALIDOBJECT;
    return fill_rect_obj(obj, whole(s), raw);
}

inline u32 clear(u32 obj, u8 r, u8 g, u8 b) {
    Surface *s = find(obj);
    if (!s) return DDERR_INVALIDOBJECT;
    return fill(obj, pack_rgb(s, r, g, b));
}

// Copy one row, honouring source colour keying and the two pixel formats.
// `dir` is +1 or -1 along x so an overlapping same-surface blit reads each
// source pixel before overwriting it.
inline void blt_row(Surface *dst, Surface *src, i32 dx, i32 dy, i32 sx, i32 sy,
                    i32 w, bool key, u32 klow, u32 khigh) {
    u32 sb = bytes_per_pixel(src), db = bytes_per_pixel(dst);
    if (!key && sb == db && dst->bpp == src->bpp) {
        mem_copy(dst->pixels + (u32)dy * dst->pitch + (u32)dx * db,
                 src->pixels + (u32)sy * src->pitch + (u32)sx * sb,
                 (u32)w * db);
        return;
    }
    if (!key && sb == db) {
        // Same width but different masks (e.g. 16bpp RGB565 -> RGB555).
        for (i32 i = 0; i < w; ++i) {
            u32 raw = get_px(src, (u32)(sx + i), (u32)sy);
            u32 rgb = pixel_rgb(src, raw);
            put_px(dst, (u32)(dx + i), (u32)dy,
                   pack_rgb(dst, (u8)(rgb >> 16), (u8)(rgb >> 8), (u8)rgb));
        }
        return;
    }
    for (i32 i = 0; i < w; ++i) {
        u32 raw = get_px(src, (u32)(sx + i), (u32)sy);
        if (key && raw >= klow && raw <= khigh) continue;
        if (src->bpp == dst->bpp) {
            put_px(dst, (u32)(dx + i), (u32)dy, raw);
        } else {
            u32 rgb = pixel_rgb(src, raw);
            put_px(dst, (u32)(dx + i), (u32)dy,
                   pack_rgb(dst, (u8)(rgb >> 16), (u8)(rgb >> 8), (u8)rgb));
        }
    }
}

// Blt(this, dst_rect, src_surface, src_rect, flags, fx). Handles the two forms
// the retail game uses: DDBLT_COLORFILL (src NULL, colour at fx+0x50) and a
// source copy (197 of 210 calls in the measured trace).
inline u32 blt(u32 dst_obj, u32 dst_rect_va, u32 src_obj, u32 src_rect_va,
               u32 flags, u32 fx_va) {
    Surface *d = find(dst_obj);
    if (!d) return DDERR_INVALIDOBJECT;
    ++stat_blits();

    if (flags & DDBLT_COLORFILL) {
        // Measured on the retail binary with gdb at its own Blt call site
        // (0x4883f5): the 11 startup colour fills carry dwFillColor at +0x50,
        // zero for ten of them and 1 for one, so the game clears surfaces to
        // black and blits art over them.
        u32 color = fx_va ? ld32(fx_va + DDBLTFX_FILLCOLOR) : 0u;
        Rect q = dst_rect_va ? rect_from(dst_rect_va) : whole(d);
        return fill_rect_obj(dst_obj, q, color);
    }

    Surface *s = find(src_obj);
    if (!s) return DDERR_INVALIDOBJECT;
    Rect dq = dst_rect_va ? rect_from(dst_rect_va) : whole(s);
    Rect sq = src_rect_va ? rect_from(src_rect_va) : whole(s);

    i32 w = dq.r - dq.l;
    i32 h = dq.b - dq.t;
    if (sq.r - sq.l < w) w = sq.r - sq.l;
    if (sq.b - sq.t < h) h = sq.b - sq.t;

#ifdef TS_MUTATION_BLT_XOFF
    // Deliberate defect, compiled only by tools/test_runtime.py: the copy
    // lands one pixel to the right. The selftest's pixel checks must catch it.
    dq.l += 1;
#endif

    // Clip the destination to the surface and the source to its own extent.
    if (dq.l < 0) { sq.l -= dq.l; w += dq.l; dq.l = 0; }
    if (dq.t < 0) { sq.t -= dq.t; h += dq.t; dq.t = 0; }
    if (dq.l + w > (i32)d->width)  w = (i32)d->width  - dq.l;
    if (dq.t + h > (i32)d->height) h = (i32)d->height - dq.t;
    if (sq.l < 0) { dq.l -= sq.l; w += sq.l; sq.l = 0; }
    if (sq.t < 0) { dq.t -= sq.t; h += sq.t; sq.t = 0; }
    if (sq.l + w > (i32)s->width)  w = (i32)s->width  - sq.l;
    if (sq.t + h > (i32)s->height) h = (i32)s->height - sq.t;
    if (w <= 0 || h <= 0) return DD_OK;

    bool key = (flags & DDBLT_KEYSRC) && s->has_colorkey;
    u32 klow = s->colorkey_low, khigh = s->colorkey_high;

    // Same surface with a downward destination: copy bottom-to-top so the
    // source rows are read before they are overwritten (the game self-blits
    // 64x64 rows at (0,1) from (0,0)).
    bool overlap = (d == s);
    i32 step = (overlap && dq.t > sq.t) ? -1 : 1;
    i32 y = (step < 0) ? h - 1 : 0;
    for (i32 n = 0; n < h; ++n, y += step) {
        i32 dx = dq.l, sx = sq.l;
        // Horizontal overlap on the same row: copy right-to-left.
        if (overlap && dq.t + y == sq.t + y && dx > sx) {
            for (i32 i = w - 1; i >= 0; --i)
                blt_row(d, s, dx + i, dq.t + y, sx + i, sq.t + y, 1, key, klow, khigh);
        } else {
            blt_row(d, s, dx, dq.t + y, sx, sq.t + y, w, key, klow, khigh);
        }
    }
    return DD_OK;
}

inline u32 blt_fast(u32 dst_obj, u32 x, u32 y, u32 src_obj, u32 src_rect_va,
                    u32 flags) {
    Surface *d = find(dst_obj);
    Surface *s = find(src_obj);
    if (!d || !s) return DDERR_INVALIDOBJECT;
    ++stat_blits();
    Rect sq = src_rect_va ? rect_from(src_rect_va) : whole(s);
    if (sq.r <= sq.l || sq.b <= sq.t) return DD_OK;
    Rect dq;
    dq.l = (i32)x;
    dq.t = (i32)y;
    dq.r = dq.l + (sq.r - sq.l);
    dq.b = dq.t + (sq.b - sq.t);
    // BltFast has no fx and one colour-key flag.
    u32 f = (flags & DDBLTFAST_SRCCOLORKEY) ? DDBLT_KEYSRC : 0u;
    // A DDBLT-style rect is needed for both, so write the source rect back
    // into scratch space is not possible here; call the row copier directly.
    i32 w = sq.r - sq.l, h = sq.b - sq.t;
    if (dq.l < 0) { sq.l -= dq.l; w += dq.l; dq.l = 0; }
    if (dq.t < 0) { sq.t -= dq.t; h += dq.t; dq.t = 0; }
    if (dq.l + w > (i32)d->width)  w = (i32)d->width  - dq.l;
    if (dq.t + h > (i32)d->height) h = (i32)d->height - dq.t;
    if (sq.l < 0) { dq.l -= sq.l; w += sq.l; sq.l = 0; }
    if (sq.t < 0) { dq.t -= sq.t; h += sq.t; sq.t = 0; }
    if (sq.l + w > (i32)s->width)  w = (i32)s->width  - sq.l;
    if (sq.t + h > (i32)s->height) h = (i32)s->height - sq.t;
    if (w <= 0 || h <= 0) return DD_OK;
    bool key = (f & DDBLT_KEYSRC) && s->has_colorkey;
    for (i32 row = 0; row < h; ++row)
        blt_row(d, s, dq.l, dq.t + row, sq.l, sq.t + row, w, key,
                s->colorkey_low, s->colorkey_high);
    return DD_OK;
}

// Flip(this, target, flags). The retail startup path never calls this, but a
// front/back pair is the only way a game that does call it can show a frame:
// swap the pixel buffers so the surface the caller blits into is the one
// dump_primary_* reads.
inline u32 flip(u32 obj, u32 target_obj, u32 flags) {
    (void)flags;
    Surface *s = find(obj);
    if (!s) return DDERR_INVALIDOBJECT;
    ++stat_flips();
    Surface *t = target_obj ? find(target_obj) : s->attached;
    if (!t) return DD_OK;   // nothing to flip to; a no-op is the honest answer
    u32 p = s->pixels; s->pixels = t->pixels; t->pixels = p;
    u32 q = s->pitch;  s->pitch  = t->pitch;  t->pitch  = q;
    ++s->flips;
    if (t->primary && !s->primary) primary_surface() = t;
    return DD_OK;
}

// -- lock / unlock -----------------------------------------------------------
inline u32 lock(u32 obj, u32 rect_va, u32 desc_va, u32 flags) {
    (void)flags;
    Surface *s = find(obj);
    if (!s) return DDERR_INVALIDOBJECT;
    ++stat_locks();
    Rect q = rect_va ? rect_from(rect_va) : whole(s);
    if (!clip_rect(q, s)) return DDERR_INVALIDPARAMS;
    s->locked = true;
    s->lock_x = (u32)q.l;
    s->lock_y = (u32)q.t;
    s->lock_w = (u32)(q.r - q.l);
    s->lock_h = (u32)(q.b - q.t);
    write_desc(obj, desc_va);
    return DD_OK;
}

inline u32 unlock(u32 obj, u32 rect_va) {
    (void)rect_va;
    Surface *s = find(obj);
    if (!s) return DDERR_INVALIDOBJECT;
    s->locked = false;
    s->lock_x = s->lock_y = s->lock_w = s->lock_h = 0u;
    return DD_OK;
}

inline u32 get_surface_desc(u32 obj, u32 va) {
    return write_desc(obj, va) ? DD_OK : DDERR_INVALIDOBJECT;
}

// -- colour key and palette --------------------------------------------------
inline u32 set_color_key(u32 obj, u32 flags, u32 key_va) {
    (void)flags;
    Surface *s = find(obj);
    if (!s) return DDERR_INVALIDOBJECT;
    if (!key_va) { s->has_colorkey = false; return DD_OK; }
    s->colorkey_low = ld32(key_va + 0u);
    s->colorkey_high = ld32(key_va + 4u);
    s->has_colorkey = true;
    return DD_OK;
}

struct Palette {
    u32 obj;
    u32 entries[PALETTE_ENTRIES];   // 0x00RRGGBB
    Palette() : obj(0) {
        for (unsigned i = 0; i < PALETTE_ENTRIES; ++i)
            entries[i] = (i << 16) | (i << 8) | i;   // identity greyscale
    }
};

inline Palette *palette_table() { static Palette t[MAX_PALETTES]; return t; }
inline unsigned &palette_n() { static unsigned n = 0u; return n; }

inline Palette *find_palette(u32 obj) {
    Palette *t = palette_table();
    for (unsigned i = 0; i < palette_n(); ++i)
        if (t[i].obj == obj && obj) return &t[i];
    return 0;
}

inline void palette_set_entry(Palette *p, u32 index, u8 r, u8 g, u8 b) {
    if (p && index < PALETTE_ENTRIES) p->entries[index] = ((u32)r << 16) | ((u32)g << 8) | b;
}

// PALETTEENTRY is peRed, peGreen, peBlue, peFlags (4 bytes each).
inline Palette *palette_create(u32 obj, u32 entries_va) {
    if (palette_n() >= (unsigned)MAX_PALETTES) return 0;
    Palette *p = &palette_table()[palette_n()++];
    *p = Palette();
    p->obj = obj;
    if (entries_va) {
        for (u32 i = 0; i < PALETTE_ENTRIES; ++i) {
            u32 a = entries_va + i * 4u;
            p->entries[i] = ((u32)ld8(a + 0u) << 16) |
                            ((u32)ld8(a + 1u) << 8) |
                            (u32)ld8(a + 2u);
        }
    }
    return p;
}

inline u32 palette_set_entries(u32 obj, u32 start, u32 count, u32 entries_va) {
    Palette *p = find_palette(obj);
    if (!p) return DDERR_INVALIDOBJECT;
    for (u32 i = 0; i < count && start + i < PALETTE_ENTRIES; ++i) {
        u32 a = entries_va + i * 4u;
        palette_set_entry(p, start + i, ld8(a + 0u), ld8(a + 1u), ld8(a + 2u));
    }
    return DD_OK;
}

inline u32 palette_get_entries(u32 obj, u32 start, u32 count, u32 entries_va) {
    Palette *p = find_palette(obj);
    if (!p) return DDERR_INVALIDOBJECT;
    for (u32 i = 0; i < count && start + i < PALETTE_ENTRIES; ++i) {
        u32 c = p->entries[start + i];
        u32 a = entries_va + i * 4u;
        st8(a + 0u, (u8)(c >> 16));
        st8(a + 1u, (u8)(c >> 8));
        st8(a + 2u, (u8)c);
        st8(a + 3u, 0u);
    }
    return DD_OK;
}

inline u32 set_palette(u32 surface_obj, u32 palette_obj) {
    Surface *s = find(surface_obj);
    if (!s) return DDERR_INVALIDOBJECT;
    Palette *p = find_palette(palette_obj);
    if (!p) return DDERR_INVALIDOBJECT;
    for (u32 i = 0; i < PALETTE_ENTRIES; ++i) s->palette[i] = p->entries[i];
    s->has_palette = true;
    return DD_OK;
}

inline u32 get_palette(u32 surface_obj, u32 out_va) {
    Surface *s = find(surface_obj);
    if (!s) return DDERR_INVALIDOBJECT;
    if (out_va) st32(out_va, 0u);   // the layer has no palette object handle
    return DD_OK;
}

// -- frame dumps -------------------------------------------------------------
// Every dump converts through pixel_rgb, so 16bpp RGB565, 32bpp and 8bpp with
// a palette all produce the same RGB triple a screenshot would.

inline bool dump_ppm(const Surface *s, const char *path) {
    if (!s || !path) return false;
    std::FILE *f = std::fopen(path, "wb");
    if (!f) return false;
    std::fprintf(f, "P6\n%u %u\n255\n", s->width, s->height);
    for (u32 y = 0; y < s->height; ++y) {
        for (u32 x = 0; x < s->width; ++x) {
            u32 c = pixel_rgb(s, get_px(s, x, y));
            u8 rgb[3];
            rgb[0] = (u8)(c >> 16); rgb[1] = (u8)(c >> 8); rgb[2] = (u8)c;
            std::fwrite(rgb, 1, 3, f);
        }
    }
    std::fclose(f);
    return true;
}

inline u32 crc32_update(u32 crc, const u8 *p, u32 n) {
    crc = ~crc;
    while (n--) {
        crc ^= *p++;
        for (int k = 0; k < 8; ++k)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

inline u32 adler32_update(u32 adler, const u8 *p, u32 n) {
    u32 a = adler & 0xffffu, b = (adler >> 16) & 0xffffu;
    while (n--) { a = (a + *p++) % 65521u; b = (b + a) % 65521u; }
    return (b << 16) | a;
}

inline void be32(u8 *p, u32 v) {
    p[0] = (u8)(v >> 24); p[1] = (u8)(v >> 16);
    p[2] = (u8)(v >> 8);  p[3] = (u8)v;
}

inline void write_chunk(std::FILE *f, const char type[4], const u8 *data, u32 len) {
    u8 hdr[8];
    be32(hdr, len);
    std::memcpy(hdr + 4, type, 4);
    std::fwrite(hdr, 1, 8, f);
    if (len) std::fwrite(data, 1, len, f);
    u32 crc = crc32_update(0u, (const u8 *)type, 4u);
    if (len) crc = crc32_update(crc, data, len);
    u8 tail[4];
    be32(tail, crc);
    std::fwrite(tail, 1, 4, f);
}

// A PNG with a zlib stream made of stored (uncompressed) deflate blocks. No
// external dependency, and the result is a standards-compliant PNG that any
// decoder reads.
inline bool dump_png(const Surface *s, const char *path) {
    if (!s || !path) return false;
    u32 raw_len = s->height * (1u + s->width * 3u);
    u8 *raw = (u8 *)std::malloc(raw_len ? raw_len : 1u);
    if (!raw) return false;
    u32 o = 0;
    for (u32 y = 0; y < s->height; ++y) {
        raw[o++] = 0u;   // filter type 0 (None)
        for (u32 x = 0; x < s->width; ++x) {
            u32 c = pixel_rgb(s, get_px(s, x, y));
            raw[o++] = (u8)(c >> 16);
            raw[o++] = (u8)(c >> 8);
            raw[o++] = (u8)c;
        }
    }

    u32 blocks = (raw_len + 65534u) / 65535u;
    if (blocks == 0u) blocks = 1u;
    u32 zlen = 2u + blocks * 5u + raw_len + 4u;
    u8 *z = (u8 *)std::malloc(zlen);
    if (!z) { std::free(raw); return false; }
    u32 zo = 0;
    z[zo++] = 0x78u; z[zo++] = 0x01u;   // zlib header, 32K window, no dict
    u32 left = raw_len, pos = 0;
    do {
        u32 n = left > 65535u ? 65535u : left;
        z[zo++] = (left <= 65535u) ? 1u : 0u;   // BFINAL, stored block
        z[zo++] = (u8)(n & 0xffu);
        z[zo++] = (u8)(n >> 8);
        z[zo++] = (u8)(~n & 0xffu);
        z[zo++] = (u8)((~n >> 8) & 0xffu);
        if (n) std::memcpy(z + zo, raw + pos, n);
        zo += n; pos += n; left -= n;
    } while (left);
    u32 ad = adler32_update(1u, raw, raw_len);
    be32(z + zo, ad); zo += 4;
    std::free(raw);

    std::FILE *f = std::fopen(path, "wb");
    if (!f) { std::free(z); return false; }
    static const u8 sig[8] = { 0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a };
    std::fwrite(sig, 1, 8, f);
    u8 ihdr[13];
    be32(ihdr + 0, s->width);
    be32(ihdr + 4, s->height);
    ihdr[8] = 8u;    // bit depth
    ihdr[9] = 2u;    // colour type RGB
    ihdr[10] = 0u;   // compression
    ihdr[11] = 0u;   // filter
    ihdr[12] = 0u;   // interlace
    write_chunk(f, "IHDR", ihdr, 13u);
    write_chunk(f, "IDAT", z, zo);
    write_chunk(f, "IEND", 0, 0u);
    std::fclose(f);
    std::free(z);
    return true;
}

inline const Surface *primary() { return primary_surface(); }

// How much of a surface is not black. A dumped frame that is entirely zero is
// a real answer ("nothing was drawn"), and it has to be distinguishable from a
// dump that failed; the boot report prints this next to the dump path.
inline unsigned long nonblack_pixels(const Surface *s) {
    if (!s) return 0u;
    unsigned long n = 0u;
    for (u32 y = 0; y < s->height; ++y)
        for (u32 x = 0; x < s->width; ++x)
            if (get_px(s, x, y) != 0u) ++n;
    return n;
}

inline bool dump_primary_png(const char *path) {
    return dump_png(primary_surface(), path);
}

inline bool dump_primary_ppm(const char *path) {
    return dump_ppm(primary_surface(), path);
}

}  // namespace rnd
}  // namespace ts

#endif  // TS_RENDERING_H
