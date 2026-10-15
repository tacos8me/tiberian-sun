#pragma once
// ==========================================================================
// BlitTranslucent50ZeroAlpha_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTranslucent50ZeroAlpha<G>
// vtable       : 0x0069b4b4  (3 slots)
// constructor  : 0x004616ab
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTranslucent50ZeroAlpha_G : public Blitter
{
public:
    // ---- vtable 0x0069b4b4 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00469fd0
    virtual void vfunc_1();  // [1] 0x00465890
    virtual void vfunc_2();  // [2] 0x00465920

    BlitTranslucent50ZeroAlpha_G();  // 0x004616ab
    virtual ~BlitTranslucent50ZeroAlpha_G();

    // ---- data members (offsets from ctor 0x004612ab) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b5f4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
