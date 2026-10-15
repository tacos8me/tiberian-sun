#pragma once
// ==========================================================================
// RLEBlitTransLucent25Alpha_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransLucent25Alpha<G>
// vtable       : 0x0069b2ac  (2 slots)
// constructor  : 0x0046201a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransLucent25Alpha_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b2ac (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00468070  // __thiscall

    RLEBlitTransLucent25Alpha_G();  // 0x0046201a
    virtual ~RLEBlitTransLucent25Alpha_G();

    // ---- data members (offsets from ctor 0x0046a6cb) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b2ac
    unsigned char pad_004[0x4];
    unsigned long m_008;  // zero-initialised
};
