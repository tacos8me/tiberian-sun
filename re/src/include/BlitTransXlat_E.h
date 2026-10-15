#pragma once
// ==========================================================================
// BlitTransXlat_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransXlat<E>
// vtable       : 0x0069b7a4  (3 slots)
// constructor  : 0x00460c3a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransXlat_E : public Blitter
{
public:
    // ---- vtable 0x0069b7a4 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x004698d0
    virtual void vfunc_1();  // [1] 0x004633f0
    virtual void vfunc_2();  // [2] 0x00463430

    BlitTransXlat_E();  // 0x00460c3a
    virtual ~BlitTransXlat_E();

    // ---- data members (offsets from ctor 0x00460c21) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7a4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
