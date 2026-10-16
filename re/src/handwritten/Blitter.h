#pragma once
// ==========================================================================
// Blitter.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// The engine's pixel-blitting templates. 110 classes in the RTTI manifest are
// instantiations of these templates, across 97 distinct template names.
//
// TEMPLATE PARAMETER
// ------------------
// The single template parameter is the pixel format, and the code proves the
// mapping. There is no type-name string in the binary, so the concrete C++
// type is inferred from operand width only and is marked /* ? */:
//
//     <E>  = 8-bit pixel   /* ? */   19 specialisations
//     <G>  = 16-bit pixel  /* ? */   89 specialisations
//
// Evidence for the split, from the blit entry points:
//
//   BlitPlain<E> 0x676470:  mov ecx,[esp+0xc] / shr ecx,2 / rep movsd
//                           and ecx,3 / rep movsb
//                           -> copies `ecx` BYTES, stride 1
//   BlitPlain<G> 0x6764c0:  lea ecx,[eax+eax] / shr ecx,2 / rep movsd
//                           -> doubles the count first, stride 2
//
//   BlitTrans<E> 0x676390:  mov byte ptr [edi],al
//   BlitTrans<G> 0x676510:  mov word ptr [edx],ax
//
//   BlitTransXlat<G> 0x4632b0:  reads word ptr [ebx+eax*2], writes word ptr
//                               [edi], add edi,2
//   RLEBlitTransXlat<E> 0x4635d0: mov byte ptr [esi-1],cl via a scale-1 table
//   RLEBlitTransXlat<G> 0x4668f0: mov word ptr [edx+ecx*2], add eax,2
//
// The split is independently corroborated by the factory clusters: <E>
// blitters are constructed at 0x460bf0-0x461014 (8-byte allocations) and <G>
// blitters at 0x469dd0-0x46aa10.
//
// BASE CLASSES
// ------------
//   class Blitter     vtable 0x69b7c4, 3 slots
//   class RLEBlitter  vtable 0x69b7d4, 2 slots
//
// NAME GRAMMAR
// ------------
// The template names compose from a fixed vocabulary, which is what makes the
// family reconstructible:
//
//   Plain      no transparency
//   Trans      transparent (source colour 0 is skipped)
//   Xlat       translate source through a palette
//   Remap      remap destination
//   ZRemap     z-buffer-aware remap
//   Lucent25/50/75   translucency levels
//   Alpha      alpha blending
//   ZRead      reads the z-buffer
//   ZReadWrite reads and writes the z-buffer
//   Warp       warping
//   Darken     darkening
//   RLE        run-length encoded source
// ==========================================================================

#include "ts_prelude.h"

// --------------------------------------------------------------------------
// Blitter — abstract base for all non-RLE blitters.
// --------------------------------------------------------------------------
class Blitter
{
public:
    virtual ~Blitter();

    // ---- vtable 0x69b7c4 (3 slots) ----
    // [0] ~Blitter  0x6763c0 (scalar deleting dtor; writes vftable 0x69b7c4)
    // [1] Blit      /* ? */ pure virtual in effect
    // [2] vfunc_2   /* ? */
    virtual void Blit();
    virtual void vfunc_2();
};

// --------------------------------------------------------------------------
// RLEBlitter — abstract base for the run-length-encoded blitters.
// --------------------------------------------------------------------------
class RLEBlitter
{
public:
    virtual ~RLEBlitter();

    // ---- vtable 0x69b7d4 (2 slots) ----
    // [0] ~RLEBlitter  /* ? */
    // [1] Blit         /* ? */
    virtual void Blit();
};

// --------------------------------------------------------------------------
// The blitter templates.
//
// Each is declared once here; the 110 RTTI instantiations are the <E> and <G>
// specialisations of these. Only the families that actually appear in the
// binary are listed, with their specialisations in a comment.
// --------------------------------------------------------------------------

// --- opaque ---
template <class T> class BlitPlain : public Blitter {};              // E,G
template <class T> class BlitPlainXlat : public Blitter {};          // E,G
template <class T> class BlitPlainXlatAlpha : public Blitter {};     // G
template <class T> class BlitPlainXlatZRead : public Blitter {};     // G
template <class T> class BlitPlainXlatZReadWrite : public Blitter {};// G

// --- transparent ---
template <class T> class BlitTrans : public Blitter {};              // E,G
template <class T> class BlitTransDarken : public Blitter {};        // G
template <class T> class BlitTransDarkenZRead : public Blitter {};   // G
template <class T> class BlitTransDarkenZReadWrite : public Blitter {}; // G
template <class T> class BlitTransRemapDest : public Blitter {};     // E
template <class T> class BlitTransRemapXlat : public Blitter {};     // E
template <class T> class BlitTransXlat : public Blitter {};          // E,G
template <class T> class BlitTransXlatAlpha : public Blitter {};     // G
template <class T> class BlitTransXlatAlphaZRead : public Blitter {};// G
template <class T> class BlitTransXlatAlphaZReadWrite : public Blitter {}; // G
template <class T> class BlitTransXlatMultWriteAlpha : public Blitter {};  // G
template <class T> class BlitTransXlatWriteAlpha : public Blitter {};// G
template <class T> class BlitTransXlatZRead : public Blitter {};     // G
template <class T> class BlitTransXlatZReadWrite : public Blitter {};// G
template <class T> class BlitTransZRemapXlat : public Blitter {};    // E,G
template <class T> class BlitTransZRemapXlatAlpha : public Blitter {}; // G
template <class T> class BlitTransZRemapXlatAlphaZRead : public Blitter {}; // G
template <class T> class BlitTransZRemapXlatAlphaZReadWrite : public Blitter {}; // G
template <class T> class BlitTransZRemapXlatZRead : public Blitter {}; // G
template <class T> class BlitTransZRemapXlatZReadWrite : public Blitter {}; // G

// --- translucent ---
template <class T> class BlitTransLucent25 : public Blitter {};      // G
template <class T> class BlitTransLucent25Alpha : public Blitter {}; // G
template <class T> class BlitTransLucent25AlphaZRead : public Blitter {}; // G
template <class T> class BlitTransLucent25AlphaZReadWarp : public Blitter {}; // G
template <class T> class BlitTransLucent25AlphaZReadWrite : public Blitter {}; // G
template <class T> class BlitTransLucent25ZRead : public Blitter {}; // G
template <class T> class BlitTransLucent25ZReadWarp : public Blitter {}; // G
template <class T> class BlitTransLucent25ZReadWrite : public Blitter {}; // G
template <class T> class BlitTransLucent50 : public Blitter {};      // G
template <class T> class BlitTransLucent50Alpha : public Blitter {}; // G
template <class T> class BlitTransLucent50AlphaZRead : public Blitter {}; // G
template <class T> class BlitTransLucent50AlphaZReadWarp : public Blitter {}; // G
template <class T> class BlitTransLucent50AlphaZReadWrite : public Blitter {}; // G
template <class T> class BlitTransLucent50ZRead : public Blitter {}; // G
template <class T> class BlitTransLucent50ZReadWarp : public Blitter {}; // G
template <class T> class BlitTransLucent50ZReadWrite : public Blitter {}; // G
template <class T> class BlitTransLucent75 : public Blitter {};      // G
template <class T> class BlitTransLucent75Alpha : public Blitter {}; // G
template <class T> class BlitTransLucent75AlphaZRead : public Blitter {}; // G
template <class T> class BlitTransLucent75AlphaZReadWarp : public Blitter {}; // G
template <class T> class BlitTransLucent75AlphaZReadWrite : public Blitter {}; // G
template <class T> class BlitTransLucent75ZRead : public Blitter {}; // G
template <class T> class BlitTransLucent75ZReadWarp : public Blitter {}; // G
template <class T> class BlitTransLucent75ZReadWrite : public Blitter {}; // G

// --- alpha-only variants ---
template <class T> class BlitTranslucent50NonzeroAlpha : public Blitter {}; // G
template <class T> class BlitTranslucent50ZeroAlpha : public Blitter {};    // G
template <class T> class BlitTranslucentWriteAlpha : public Blitter {};     // G

// --- run-length encoded ---
template <class T> class RLEBlitTransDarken : public RLEBlitter {};  // G
template <class T> class RLEBlitTransDarkenZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransDarkenZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25 : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25Alpha : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25AlphaZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25AlphaZReadWarp : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25AlphaZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25ZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25ZReadWarp : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent25ZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50 : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50Alpha : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50AlphaZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50AlphaZReadWarp : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50AlphaZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50ZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50ZReadWarp : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent50ZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75 : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75Alpha : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75AlphaZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75AlphaZReadWarp : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75AlphaZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75ZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75ZReadWarp : public RLEBlitter {}; // G
template <class T> class RLEBlitTransLucent75ZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransRemapDest : public RLEBlitter {}; // E
template <class T> class RLEBlitTransRemapDestZRead : public RLEBlitter {}; // E
template <class T> class RLEBlitTransRemapDestZReadWrite : public RLEBlitter {}; // E
template <class T> class RLEBlitTransRemapXlat : public RLEBlitter {}; // E
template <class T> class RLEBlitTransRemapXlatZRead : public RLEBlitter {}; // E
template <class T> class RLEBlitTransRemapXlatZReadWrite : public RLEBlitter {}; // E
template <class T> class RLEBlitTransXlat : public RLEBlitter {};    // E,G
template <class T> class RLEBlitTransXlatAlpha : public RLEBlitter {}; // G
template <class T> class RLEBlitTransXlatAlphaZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransXlatAlphaZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransXlatZRead : public RLEBlitter {}; // E,G
template <class T> class RLEBlitTransXlatZReadWrite : public RLEBlitter {}; // E,G
template <class T> class RLEBlitTransZRemapXlat : public RLEBlitter {}; // E,G
template <class T> class RLEBlitTransZRemapXlatAlpha : public RLEBlitter {}; // G
template <class T> class RLEBlitTransZRemapXlatAlphaZRead : public RLEBlitter {}; // G
template <class T> class RLEBlitTransZRemapXlatAlphaZReadWrite : public RLEBlitter {}; // G
template <class T> class RLEBlitTransZRemapXlatZRead : public RLEBlitter {}; // E,G
template <class T> class RLEBlitTransZRemapXlatZReadWrite : public RLEBlitter {}; // E,G
