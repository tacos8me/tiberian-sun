#pragma once
// ==========================================================================
// RLEBlitTransZRemapXlatAlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransZRemapXlatAlphaZReadWrite<G>
// vtable       : 0x0069b234  (2 slots)
// constructor  : 0x004622ee
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransZRemapXlatAlphaZReadWrite_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b234 (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);  // [1] 0x00469100  // __thiscall

    RLEBlitTransZRemapXlatAlphaZReadWrite_G();  // 0x004622ee
    virtual ~RLEBlitTransZRemapXlatAlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x0046a94b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b234
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised
};
