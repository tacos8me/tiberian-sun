#pragma once
// ==========================================================================
// RLEBlitTransLucent50AlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent50AlphaZReadWrite<G>
// vtable       : 0x0069b21c  (2 slots)
// constructor  : 0x00462384
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent50AlphaZReadWrite_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b21c (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046a9d0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [1] 0x004694b0

    RLEBlitTransLucent50AlphaZReadWrite_G();  // 0x00462384
    virtual ~RLEBlitTransLucent50AlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x00462380) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b21c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
