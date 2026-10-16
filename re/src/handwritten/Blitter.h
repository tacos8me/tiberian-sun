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

    // ---- data members (offsets from ctor 0x00469870) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7c4

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

    // ---- data members (offsets from ctor 0x00469950) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7d4

};

// --------------------------------------------------------------------------
// The blitter templates.
//
// Each is declared once here; the 110 RTTI instantiations are the <E> and <G>
// specialisations of these. Only the families that actually appear in the
// binary are listed, with their specialisations in a comment.
// --------------------------------------------------------------------------

// --- opaque ---
template <class T> class BlitPlain : public Blitter {

    // ---- data members (offsets from ctor 0x006759ec) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_06c;  // init 0x6a7c24
    // 1 member(s) already declared above; their offsets are recorded in out/members.json

};              // E,G
template <class T> class BlitPlainXlat : public Blitter {

    // ---- data members (offsets from ctor 0x00460bf0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7b4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};          // E,G
template <class T> class BlitPlainXlatAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00469dd0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b554
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

};     // G
template <class T> class BlitPlainXlatZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};     // G
template <class T> class BlitPlainXlatZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};// G

// --- transparent ---
template <class T> class BlitTrans : public Blitter {

    // ---- data members (offsets from ctor 0x00675910) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 11 member(s) already declared above; their offsets are recorded in out/members.json

};              // E,G
template <class T> class BlitTransDarken : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};        // G
template <class T> class BlitTransDarkenZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};   // G
template <class T> class BlitTransDarkenZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransRemapDest : public Blitter {

    // ---- data members (offsets from ctor 0x00460c72) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b784
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};     // E
template <class T> class BlitTransRemapXlat : public Blitter {

    // ---- data members (offsets from ctor 0x00460c96) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b774
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};     // E
template <class T> class BlitTransXlat : public Blitter {

    // ---- data members (offsets from ctor 0x00460c21) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7a4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};          // E,G
template <class T> class BlitTransXlatAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00469e10) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b544
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

};     // G
template <class T> class BlitTransXlatAlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00469ff0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b4a4
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

};// G
template <class T> class BlitTransXlatAlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x0046a1f0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b424
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransXlatMultWriteAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};  // G
template <class T> class BlitTransXlatWriteAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};// G
template <class T> class BlitTransXlatZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};     // G
template <class T> class BlitTransXlatZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};// G
template <class T> class BlitTransZRemapXlat : public Blitter {

    // ---- data members (offsets from ctor 0x00460c45) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b794
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};    // E,G
template <class T> class BlitTransZRemapXlatAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00469e50) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b534
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised

}; // G
template <class T> class BlitTransZRemapXlatAlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x0046a030) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b494
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised

}; // G
template <class T> class BlitTransZRemapXlatAlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x0046a230) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b414
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised

}; // G
template <class T> class BlitTransZRemapXlatZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // G
template <class T> class BlitTransZRemapXlatZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G

// --- translucent ---
template <class T> class BlitTransLucent25 : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};      // G
template <class T> class BlitTransLucent25Alpha : public Blitter {

    // ---- data members (offsets from ctor 0x00469f10) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b504
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent25AlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x0046a0f0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b464
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent25AlphaZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x0046a1b0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b434
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent25AlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x0046a2f0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3e4
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent25ZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // G
template <class T> class BlitTransLucent25ZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent25ZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent50 : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};      // G
template <class T> class BlitTransLucent50Alpha : public Blitter {

    // ---- data members (offsets from ctor 0x00469ed0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b514
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent50AlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x0046a0b0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b474
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent50AlphaZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x0046a170) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b444
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent50AlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x0046a2b0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3f4
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent50ZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // G
template <class T> class BlitTransLucent50ZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // G
template <class T> class BlitTransLucent50ZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent75 : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};      // G
template <class T> class BlitTransLucent75Alpha : public Blitter {

    // ---- data members (offsets from ctor 0x00469e90) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b524
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent75AlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x0046a070) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b484
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent75AlphaZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x0046a130) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b454
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent75AlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x0046a270) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b404
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class BlitTransLucent75ZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // G
template <class T> class BlitTransLucent75ZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6d4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // G
template <class T> class BlitTransLucent75ZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G

// --- alpha-only variants ---
template <class T> class BlitTranslucent50NonzeroAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTranslucent50ZeroAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};    // G
template <class T> class BlitTranslucentWriteAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};     // G

// --- run-length encoded ---
template <class T> class RLEBlitTransDarken : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};  // G
template <class T> class RLEBlitTransDarkenZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransDarkenZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25 : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25Alpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a6d0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2ac
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent25AlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a810) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b270
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent25AlphaZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a8d0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b24c
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent25AlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046aa10) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b210
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent25ZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25ZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25ZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50 : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50Alpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a690) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2b8
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent50AlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a7d0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b27c
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent50AlphaZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a890) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b258
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent50AlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a9d0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b21c
    unsigned long m_008;  // zero-initialised
    // 1 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50ZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50ZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50ZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75 : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75Alpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a650) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2c4
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent75AlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a790) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b288
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent75AlphaZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a850) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b264
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent75AlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a990) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b228
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransLucent75ZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75ZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75ZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461054) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransRemapDest : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d71) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b750
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E
template <class T> class RLEBlitTransRemapDestZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460e7f) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b720
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E
template <class T> class RLEBlitTransRemapDestZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460f8d) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6f0
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E
template <class T> class RLEBlitTransRemapXlat : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d98) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b744
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E
template <class T> class RLEBlitTransRemapXlatZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460ea6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b714
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E
template <class T> class RLEBlitTransRemapXlatZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460fb4) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6e4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E
template <class T> class RLEBlitTransXlat : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d1d) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b768
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

};    // E,G
template <class T> class RLEBlitTransXlatAlpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a5d0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2dc
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransXlatAlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a710) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2a0
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransXlatAlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a910) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b240
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised

}; // G
template <class T> class RLEBlitTransXlatZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460e28) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b738
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E,G
template <class T> class RLEBlitTransXlatZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460f36) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b708
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E,G
template <class T> class RLEBlitTransZRemapXlat : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d41) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b75c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E,G
template <class T> class RLEBlitTransZRemapXlatAlpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a610) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2d0
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised

}; // G
template <class T> class RLEBlitTransZRemapXlatAlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a750) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b294
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised

}; // G
template <class T> class RLEBlitTransZRemapXlatAlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046a950) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b234
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised

}; // G
template <class T> class RLEBlitTransZRemapXlatZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460e4f) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b72c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E,G
template <class T> class RLEBlitTransZRemapXlatZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460f5d) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b6fc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */

}; // E,G
