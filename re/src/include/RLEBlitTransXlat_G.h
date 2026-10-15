#pragma once
// ==========================================================================
// RLEBlitTransXlat_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransXlat<G>
// vtable       : 0x0069b3d8  (2 slots)
// constructor  : 0x00461b07
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransXlat_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b3d8 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a330
    virtual void vfunc_1();  // [1] 0x004668f0

    RLEBlitTransXlat_G();  // 0x00461b07
    virtual ~RLEBlitTransXlat_G();

    // ---- data members (offsets from ctor 0x00461707) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3d8
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
