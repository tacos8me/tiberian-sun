#pragma once
// ==========================================================================
// RLEBlitTransLucent75ZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent75ZReadWrite<G>
// vtable       : 0x0069b300  (2 slots)
// constructor  : 0x00461e54
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent75ZReadWrite_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b300 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a570
    virtual void vfunc_1();  // [1] 0x00467880

    RLEBlitTransLucent75ZReadWrite_G();  // 0x00461e54
    virtual ~RLEBlitTransLucent75ZReadWrite_G();

    // ---- data members (offsets from ctor 0x00461a54) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3d8
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
