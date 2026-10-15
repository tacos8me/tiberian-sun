#pragma once
// ==========================================================================
// RLEBlitTransLucent25AlphaZReadWarp_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent25AlphaZReadWarp<G>
// vtable       : 0x0069b24c  (2 slots)
// constructor  : 0x00462265
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent25AlphaZReadWarp_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b24c (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046a8d0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [1] 0x00468d90

    RLEBlitTransLucent25AlphaZReadWarp_G();  // 0x00462265
    virtual ~RLEBlitTransLucent25AlphaZReadWarp_G();

    // ---- data members (offsets from ctor 0x0046a8cb) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b24c
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised
};
