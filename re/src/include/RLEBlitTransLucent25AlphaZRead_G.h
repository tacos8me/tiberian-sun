#pragma once
// ==========================================================================
// RLEBlitTransLucent25AlphaZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent25AlphaZRead<G>
// vtable       : 0x0069b270  (2 slots)
// constructor  : 0x00462184
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent25AlphaZRead_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b270 (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [1] 0x00468840

    RLEBlitTransLucent25AlphaZRead_G();  // 0x00462184
    virtual ~RLEBlitTransLucent25AlphaZRead_G();

    // ---- data members (offsets from ctor 0x0046a80b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b270
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised
};
