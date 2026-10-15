#pragma once
// ==========================================================================
// BlitTransXlatAlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransXlatAlphaZReadWrite<G>
// vtable       : 0x0069b424  (3 slots)
// constructor  : 0x004619ae
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransXlatAlphaZReadWrite_G : public Blitter
{
public:
    // ---- vtable 0x0069b424 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a1f0
    virtual void vfunc_1();  // [1] 0x00466300
    virtual void vfunc_2();  // [2] 0x004663f0

    BlitTransXlatAlphaZReadWrite_G();  // 0x004619ae
    virtual ~BlitTransXlatAlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x004615ae) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b504
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
