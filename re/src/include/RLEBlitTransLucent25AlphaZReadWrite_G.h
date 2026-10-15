#pragma once
// ==========================================================================
// RLEBlitTransLucent25AlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent25AlphaZReadWrite<G>
// vtable       : 0x0069b210  (2 slots)
// constructor  : 0x004623cf
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent25AlphaZReadWrite_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b210 (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046aa10  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);  // [1] 0x004696a0

    RLEBlitTransLucent25AlphaZReadWrite_G();  // 0x004623cf
    virtual ~RLEBlitTransLucent25AlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x004623c0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b210
    unsigned char pad_004[0x4];
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
