#pragma once
// ==========================================================================
// BlitTransRemapXlat_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransRemapXlat<E>
// vtable       : 0x0069b774  (3 slots)
// constructor  : 0x00460cbb
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransRemapXlat_E : public Blitter
{
public:
    // ---- vtable 0x0069b774 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00469930
    virtual void vfunc_1();  // [1] 0x00463550
    virtual void vfunc_2();  // [2] 0x004635a0

    BlitTransRemapXlat_E();  // 0x00460cbb
    virtual ~BlitTransRemapXlat_E();

    // ---- data members (offsets from ctor 0x00460c96) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b774
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
