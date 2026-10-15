#pragma once
// ==========================================================================
// BlitPlainXlat_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitPlainXlat<E>
// vtable       : 0x0069b7b4  (3 slots)
// constructor  : 0x00460c16
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitPlainXlat_E : public Blitter
{
public:
    // ---- vtable 0x0069b7b4 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x004698b0
    virtual void vfunc_1();  // [1] 0x00463380
    virtual void vfunc_2();  // [2] 0x004633c0

    BlitPlainXlat_E();  // 0x00460c16
    virtual ~BlitPlainXlat_E();

    // ---- data members (offsets from ctor 0x00460bf0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7b4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
