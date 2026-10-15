#pragma once
// ==========================================================================
// BlitTransLucent75AlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent75AlphaZReadWrite<G>
// vtable       : 0x0069b404  (3 slots)
// constructor  : 0x00461a42
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent75AlphaZReadWrite_G : public Blitter
{
public:
    // ---- vtable 0x0069b404 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a270
    virtual void vfunc_1();  // [1] 0x00466530
    virtual void vfunc_2();  // [2] 0x00466640

    BlitTransLucent75AlphaZReadWrite_G();  // 0x00461a42
    virtual ~BlitTransLucent75AlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x00461642) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b554
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
