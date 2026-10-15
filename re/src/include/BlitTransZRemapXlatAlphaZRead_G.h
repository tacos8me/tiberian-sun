#pragma once
// ==========================================================================
// BlitTransZRemapXlatAlphaZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransZRemapXlatAlphaZRead<G>
// vtable       : 0x0069b494  (3 slots)
// constructor  : 0x0046176c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransZRemapXlatAlphaZRead_G : public Blitter
{
public:
    // ---- vtable 0x0069b494 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a030
    virtual void vfunc_1();  // [1] 0x00465a60
    virtual void vfunc_2();  // [2] 0x00465b60

    BlitTransZRemapXlatAlphaZRead_G();  // 0x0046176c
    virtual ~BlitTransZRemapXlatAlphaZRead_G();

    // ---- data members (offsets from ctor 0x0046136c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b554
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
