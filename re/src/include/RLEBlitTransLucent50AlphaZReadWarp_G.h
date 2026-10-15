#pragma once
// ==========================================================================
// RLEBlitTransLucent50AlphaZReadWarp_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent50AlphaZReadWarp<G>
// vtable       : 0x0069b258  (2 slots)
// constructor  : 0x0046221a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent50AlphaZReadWarp_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b258 (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046a890  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5);  // [1] 0x00468bd0

    RLEBlitTransLucent50AlphaZReadWarp_G();  // 0x0046221a
    virtual ~RLEBlitTransLucent50AlphaZReadWarp_G();

    // ---- data members (offsets from ctor 0x0046a88b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b258
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised
};
