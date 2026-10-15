#pragma once
// ==========================================================================
// RLEBlitTransZRemapXlatAlpha_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransZRemapXlatAlpha<G>
// vtable       : 0x0069b2d0  (2 slots)
// constructor  : 0x00461f39
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransZRemapXlatAlpha_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b2d0 (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5);  // [1] 0x00467ce0  // __thiscall

    RLEBlitTransZRemapXlatAlpha_G();  // 0x00461f39
    virtual ~RLEBlitTransZRemapXlatAlpha_G();

    // ---- data members (offsets from ctor 0x0046a60b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2d0
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised
};
