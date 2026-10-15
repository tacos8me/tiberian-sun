#pragma once
// ==========================================================================
// RLEBlitTransXlatZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransXlatZRead<G>
// vtable       : 0x0069b390  (2 slots)
// constructor  : 0x00461c1d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransXlatZRead_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b390 (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5);  // [1] 0x00466c80

    RLEBlitTransXlatZRead_G();  // 0x00461c1d
    virtual ~RLEBlitTransXlatZRead_G();

    // ---- data members (offsets from ctor 0x00461b37) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3c0
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
