#pragma once
// ==========================================================================
// RLEBlitTransRemapDest_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransRemapDest<E>
// vtable       : 0x0069b750  (2 slots)
// constructor  : 0x00460d8d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransRemapDest_E : public RLEBlitter
{
public:
    // ---- vtable 0x0069b750 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x004699b0
    virtual void vfunc_1();  // [1] 0x004636e0

    RLEBlitTransRemapDest_E();  // 0x00460d8d
    virtual ~RLEBlitTransRemapDest_E();

    // ---- data members (offsets from ctor 0x00460d71) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b750
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
