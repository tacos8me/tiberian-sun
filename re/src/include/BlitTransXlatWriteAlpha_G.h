#pragma once
// ==========================================================================
// BlitTransXlatWriteAlpha_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransXlatWriteAlpha<G>
// vtable       : 0x0069b4f4  (3 slots)
// constructor  : 0x00461602
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransXlatWriteAlpha_G : public Blitter
{
public:
    // ---- vtable 0x0069b4f4 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00469f50  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00465460  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x004654d0  // __stdcall

    BlitTransXlatWriteAlpha_G();  // 0x00461602
    virtual ~BlitTransXlatWriteAlpha_G();

    // ---- data members (offsets from ctor 0x0046150c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b4f4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
