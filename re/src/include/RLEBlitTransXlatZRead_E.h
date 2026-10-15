#pragma once
// ==========================================================================
// RLEBlitTransXlatZRead_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransXlatZRead<E>
// vtable       : 0x0069b738  (2 slots)
// constructor  : 0x00460e44
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransXlatZRead_E : public RLEBlitter
{
public:
    // ---- vtable 0x0069b738 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x004699f0
    virtual void vfunc_1();  // [1] 0x004637f0

    RLEBlitTransXlatZRead_E();  // 0x00460e44
    virtual ~RLEBlitTransXlatZRead_E();

    // ---- data members (offsets from ctor 0x00460e28) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b738
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
