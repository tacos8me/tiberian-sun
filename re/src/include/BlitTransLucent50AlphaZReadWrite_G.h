#pragma once
// ==========================================================================
// BlitTransLucent50AlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent50AlphaZReadWrite<G>
// vtable       : 0x0069b3f4  (3 slots)
// constructor  : 0x00461a8d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent50AlphaZReadWrite_G : public Blitter
{
public:
    // ---- vtable 0x0069b3f4 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a2b0
    virtual void vfunc_1();  // [1] 0x00466670
    virtual void vfunc_2();  // [2] 0x00466780

    BlitTransLucent50AlphaZReadWrite_G();  // 0x00461a8d
    virtual ~BlitTransLucent50AlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x0046168d) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b554
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
