#pragma once
// ==========================================================================
// RLEBlitTransZRemapXlatAlphaZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransZRemapXlatAlphaZRead<G>
// vtable       : 0x0069b294  (2 slots)
// constructor  : 0x004620a3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransZRemapXlatAlphaZRead_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b294 (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [1] 0x00468320  // __thiscall

    RLEBlitTransZRemapXlatAlphaZRead_G();  // 0x004620a3
    virtual ~RLEBlitTransZRemapXlatAlphaZRead_G();

    // ---- data members (offsets from ctor 0x0046a74b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b294
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // zero-initialised
};
