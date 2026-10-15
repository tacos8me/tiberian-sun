#pragma once
// ==========================================================================
// RLEBlitTransLucent25ZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent25ZReadWrite<G>
// vtable       : 0x0069b2e8  (2 slots)
// constructor  : 0x00461eb8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent25ZReadWrite_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b2e8 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a5b0
    virtual void vfunc_1();  // [1] 0x00467ab0

    RLEBlitTransLucent25ZReadWrite_G();  // 0x00461eb8
    virtual ~RLEBlitTransLucent25ZReadWrite_G();

    // ---- data members (offsets from ctor 0x00461ab8) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3d8
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
