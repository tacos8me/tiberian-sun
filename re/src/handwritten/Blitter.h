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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 1 member(s) already declared above; their offsets are recorded in out/members.json

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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 1 member(s) already declared above; their offsets are recorded in out/members.json

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
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};          // E,G
template <class T> class BlitPlainXlatAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461456) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

};     // G
template <class T> class BlitPlainXlatZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461179) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};     // G
template <class T> class BlitPlainXlatZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x0046132e) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b5c4; 4-byte store: mov dword ptr [eax], 0x69b5c4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], ecx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

};// G

// --- transparent ---
template <class T> class BlitTrans : public Blitter {

    // ---- data members (offsets from ctor 0x00675910) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    // 11 member(s) already declared above; their offsets are recorded in out/members.json

};              // E,G
template <class T> class BlitTransDarken : public Blitter {

    // ---- data members (offsets from ctor 0x004610c6) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};        // G
template <class T> class BlitTransDarkenZRead : public Blitter {

    // ---- data members (offsets from ctor 0x004611ee) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};   // G
template <class T> class BlitTransDarkenZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x004613a3) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b594; 4-byte store: mov dword ptr [eax], 0x69b594
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class BlitTransRemapDest : public Blitter {

    // ---- data members (offsets from ctor 0x00460c72) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};     // E
template <class T> class BlitTransRemapXlat : public Blitter {

    // ---- data members (offsets from ctor 0x00460c96) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};     // E
template <class T> class BlitTransXlat : public Blitter {

    // ---- data members (offsets from ctor 0x00460c21) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};          // E,G
template <class T> class BlitTransXlatAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461494) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

};     // G
template <class T> class BlitTransXlatAlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x004616f3) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

};// G
template <class T> class BlitTransXlatAlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x0046197e) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransXlatMultWriteAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x0046160a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b4e4; 4-byte store: mov dword ptr [eax], 0x69b4e4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], ecx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

};  // G
template <class T> class BlitTransXlatWriteAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x004615ef) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b4f4; 4-byte store: mov dword ptr [eax], 0x69b4f4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], ecx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

};// G
template <class T> class BlitTransXlatZRead : public Blitter {

    // ---- data members (offsets from ctor 0x0046119d) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};     // G
template <class T> class BlitTransXlatZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461352) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b5b4; 4-byte store: mov dword ptr [eax], 0x69b5b4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], ecx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

};// G
template <class T> class BlitTransZRemapXlat : public Blitter {

    // ---- data members (offsets from ctor 0x00460c45) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};    // E,G
template <class T> class BlitTransZRemapXlatAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x004614d1) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [edi + 8], ecx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransZRemapXlatAlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461733) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [edi + 8], ecx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransZRemapXlatAlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x004619be) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [edi + 8], ecx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransZRemapXlatZRead : public Blitter {

    // ---- data members (offsets from ctor 0x004611c1) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransZRemapXlatZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461376) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b5a4; 4-byte store: mov dword ptr [eax], 0x69b5a4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G

// --- translucent ---
template <class T> class BlitTransLucent25 : public Blitter {

    // ---- data members (offsets from ctor 0x0046114a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};      // G
template <class T> class BlitTransLucent25Alpha : public Blitter {

    // ---- data members (offsets from ctor 0x004615a7) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent25AlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461812) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent25AlphaZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x004618f3) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent25AlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461a9d) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent25ZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461272) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent25ZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x004612ff) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b5d4; 4-byte store: mov dword ptr [eax], 0x69b5d4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class BlitTransLucent25ZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461427) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b564; 4-byte store: mov dword ptr [eax], 0x69b564
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class BlitTransLucent50 : public Blitter {

    // ---- data members (offsets from ctor 0x0046111b) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};      // G
template <class T> class BlitTransLucent50Alpha : public Blitter {

    // ---- data members (offsets from ctor 0x0046155f) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent50AlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x004617c7) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent50AlphaZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x004618a8) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent50AlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461a52) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent50ZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461243) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent50ZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x004612d0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent50ZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x004613f8) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b574; 4-byte store: mov dword ptr [eax], 0x69b574
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class BlitTransLucent75 : public Blitter {

    // ---- data members (offsets from ctor 0x004610ec) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};      // G
template <class T> class BlitTransLucent75Alpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461517) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent75AlphaZRead : public Blitter {

    // ---- data members (offsets from ctor 0x0046177c) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent75AlphaZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x0046185d) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent75AlphaZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x00461a07) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent75ZRead : public Blitter {

    // ---- data members (offsets from ctor 0x00461214) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent75ZReadWarp : public Blitter {

    // ---- data members (offsets from ctor 0x004612a1) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class BlitTransLucent75ZReadWrite : public Blitter {

    // ---- data members (offsets from ctor 0x004613c9) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b584; 4-byte store: mov dword ptr [eax], 0x69b584
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G

// --- alpha-only variants ---
template <class T> class BlitTranslucent50NonzeroAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x0046164f) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b4c4; 4-byte store: mov dword ptr [eax], 0x69b4c4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class BlitTranslucent50ZeroAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461681) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b4b4; 4-byte store: mov dword ptr [eax], 0x69b4b4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

};    // G
template <class T> class BlitTranslucentWriteAlpha : public Blitter {

    // ---- data members (offsets from ctor 0x00461628) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b4d4; 4-byte store: mov dword ptr [eax], 0x69b4d4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], ecx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

};     // G

// --- run-length encoded ---
template <class T> class RLEBlitTransDarken : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461b3f) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b3c0; 4-byte store: mov dword ptr [eax], 0x69b3c0
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

};  // G
template <class T> class RLEBlitTransDarkenZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461c55) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b378; 4-byte store: mov dword ptr [eax], 0x69b378
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransDarkenZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461e01) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b30c; 4-byte store: mov dword ptr [eax], 0x69b30c
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned short m_008;  // +0x008 /* ? */; 2-byte store: mov word ptr [eax + 8], cx

}; // G
template <class T> class RLEBlitTransLucent25 : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461bcc) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b39c; 4-byte store: mov dword ptr [eax], 0x69b39c
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent25Alpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461fdf) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25AlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00462149) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25AlphaZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046222a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25AlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00462394) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned short m_00c;  // +0x00c /* ? */; 2-byte store: mov word ptr [edi + 0xc], ax
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent25ZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461ce2) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b354; 4-byte store: mov dword ptr [eax], 0x69b354
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent25ZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461d78) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b330; 4-byte store: mov dword ptr [eax], 0x69b330
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent25ZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461e8e) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b2e8; 4-byte store: mov dword ptr [eax], 0x69b2e8
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned short m_008;  // +0x008 /* ? */; 2-byte store: mov word ptr [eax + 8], cx

}; // G
template <class T> class RLEBlitTransLucent50 : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461b9a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b3a8; 4-byte store: mov dword ptr [eax], 0x69b3a8
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent50Alpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461f94) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50AlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x004620fe) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50AlphaZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x004621df) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50AlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00462349) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned short m_00c;  // +0x00c /* ? */; 2-byte store: mov word ptr [edi + 0xc], ax
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent50ZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461cb0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b360; 4-byte store: mov dword ptr [eax], 0x69b360
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent50ZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461d46) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b33c; 4-byte store: mov dword ptr [eax], 0x69b33c
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent50ZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461e5c) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b2f4; 4-byte store: mov dword ptr [eax], 0x69b2f4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned short m_008;  // +0x008 /* ? */; 2-byte store: mov word ptr [eax + 8], cx

}; // G
template <class T> class RLEBlitTransLucent75 : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461b68) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b3b4; 4-byte store: mov dword ptr [eax], 0x69b3b4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent75Alpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461f49) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75AlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x004620b3) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75AlphaZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00462194) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75AlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x004622fe) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned short m_00c;  // +0x00c /* ? */; 2-byte store: mov word ptr [edi + 0xc], ax
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransLucent75ZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461c7e) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b36c; 4-byte store: mov dword ptr [eax], 0x69b36c
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent75ZReadWarp : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461d14) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b348; 4-byte store: mov dword ptr [eax], 0x69b348
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx

}; // G
template <class T> class RLEBlitTransLucent75ZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461e2a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b300; 4-byte store: mov dword ptr [eax], 0x69b300
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], edx
    unsigned short m_008;  // +0x008 /* ? */; 2-byte store: mov word ptr [eax + 8], cx

}; // G
template <class T> class RLEBlitTransRemapDest : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d71) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E
template <class T> class RLEBlitTransRemapDestZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460e7f) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E
template <class T> class RLEBlitTransRemapDestZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460f8d) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E
template <class T> class RLEBlitTransRemapXlat : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d98) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E
template <class T> class RLEBlitTransRemapXlatZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460ea6) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E
template <class T> class RLEBlitTransRemapXlatZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460fb4) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E
template <class T> class RLEBlitTransXlat : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d1d) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

};    // E,G
template <class T> class RLEBlitTransXlatAlpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461ec0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransXlatAlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046202a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransXlatAlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00462275) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [edi + 0xc], ebx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransXlatZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460e28) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E,G
template <class T> class RLEBlitTransXlatZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460f36) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E,G
template <class T> class RLEBlitTransZRemapXlat : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460d41) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E,G
template <class T> class RLEBlitTransZRemapXlatAlpha : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00461f00) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [edi + 8], ecx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransZRemapXlatAlphaZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x0046206a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [edi + 8], ecx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransZRemapXlatAlphaZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x004622b5) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], edx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [edi + 8], ecx
    // 2 member(s) already declared above; their offsets are recorded in out/members.json

}; // G
template <class T> class RLEBlitTransZRemapXlatZRead : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460e4f) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E,G
template <class T> class RLEBlitTransZRemapXlatZReadWrite : public RLEBlitter {

    // ---- data members (offsets from ctor 0x00460f5d) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // 3 member(s) already declared above; their offsets are recorded in out/members.json

}; // E,G
