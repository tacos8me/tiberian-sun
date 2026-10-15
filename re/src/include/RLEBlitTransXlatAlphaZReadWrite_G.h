#pragma once
// ==========================================================================
// RLEBlitTransXlatAlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransXlatAlphaZReadWrite<G>
// vtable       : 0x0069b240  (2 slots)
// constructor  : 0x004622a5
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransXlatAlphaZReadWrite_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b240 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a910
    virtual void vfunc_1();  // [1] 0x00468f60

    RLEBlitTransXlatAlphaZReadWrite_G();  // 0x004622a5
    virtual ~RLEBlitTransXlatAlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x00461ea5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2dc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
