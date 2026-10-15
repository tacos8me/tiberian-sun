#pragma once
// ==========================================================================
// RLEBlitTransLucent50_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent50<G>
// vtable       : 0x0069b3a8  (2 slots)
// constructor  : 0x00461bc4
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent50_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b3a8 (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046a3b0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2);  // [1] 0x00466b40

    RLEBlitTransLucent50_G();  // 0x00461bc4
    virtual ~RLEBlitTransLucent50_G();

    // ---- data members (offsets from ctor 0x00461b37) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3c0
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
