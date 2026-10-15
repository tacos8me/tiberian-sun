#pragma once
// ==========================================================================
// RLEBlitTransXlatZReadWrite_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransXlatZReadWrite<E>
// vtable       : 0x0069b708  (2 slots)
// constructor  : 0x00460f52
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransXlatZReadWrite_E : public RLEBlitter
{
public:
    // ---- vtable 0x0069b708 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x00469a70
    virtual void vfunc_1();  // [1] 0x00463b80

    RLEBlitTransXlatZReadWrite_E();  // 0x00460f52
    virtual ~RLEBlitTransXlatZReadWrite_E();

    // ---- data members (offsets from ctor 0x00460f36) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b708
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
