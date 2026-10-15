#pragma once
// ==========================================================================
// RLEBlitTransLucent75AlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent75AlphaZReadWrite<G>
// vtable       : 0x0069b228  (2 slots)
// constructor  : 0x00462339
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent75AlphaZReadWrite_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b228 (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046a990  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [1] 0x004692e0

    RLEBlitTransLucent75AlphaZReadWrite_G();  // 0x00462339
    virtual ~RLEBlitTransLucent75AlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x0046a98b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b228
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised
};
