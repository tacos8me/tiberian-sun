#pragma once
// ==========================================================================
// RLEBlitTransZRemapXlatZRead_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransZRemapXlatZRead<E>
// vtable       : 0x0069b72c  (2 slots)
// constructor  : 0x00460e77
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransZRemapXlatZRead_E : public RLEBlitter
{
public:
    // ---- vtable 0x0069b72c (2 slots) ----
    virtual void vfunc_0();  // [0] 0x00469a10
    virtual void vfunc_1();  // [1] 0x004638c0

    RLEBlitTransZRemapXlatZRead_E();  // 0x00460e77
    virtual ~RLEBlitTransZRemapXlatZRead_E();

    // ---- data members (offsets from ctor 0x00460e4f) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b72c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
