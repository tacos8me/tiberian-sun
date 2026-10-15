#pragma once
// ==========================================================================
// RLEBlitTransLucent75Alpha_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent75Alpha<G>
// vtable       : 0x0069b2c4  (2 slots)
// constructor  : 0x00461f84
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent75Alpha_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b2c4 (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00467e10  // __thiscall

    RLEBlitTransLucent75Alpha_G();  // 0x00461f84
    virtual ~RLEBlitTransLucent75Alpha_G();

    // ---- data members (offsets from ctor 0x0046a64b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2c4
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised
};
