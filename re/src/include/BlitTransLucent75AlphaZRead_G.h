#pragma once
// ==========================================================================
// BlitTransLucent75AlphaZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent75AlphaZRead<G>
// vtable       : 0x0069b484  (3 slots)
// constructor  : 0x004617b7
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent75AlphaZRead_G : public Blitter
{
public:
    // ---- vtable 0x0069b484 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a070
    virtual void vfunc_1();  // [1] 0x00465b90
    virtual void vfunc_2();  // [2] 0x00465ca0

    BlitTransLucent75AlphaZRead_G();  // 0x004617b7
    virtual ~BlitTransLucent75AlphaZRead_G();

    // ---- data members (offsets from ctor 0x004613b7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b554
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
