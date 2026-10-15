#pragma once
// ==========================================================================
// RLEBlitTransLucent50AlphaZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent50AlphaZRead<G>
// vtable       : 0x0069b27c  (2 slots)
// constructor  : 0x00462139
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent50AlphaZRead_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b27c (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5);  // [1] 0x00468690

    RLEBlitTransLucent50AlphaZRead_G();  // 0x00462139
    virtual ~RLEBlitTransLucent50AlphaZRead_G();

    // ---- data members (offsets from ctor 0x0046a7cb) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b27c
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised
};
