#pragma once
// ==========================================================================
// BlitTransLucent25Alpha_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent25Alpha<G>
// vtable       : 0x0069b504  (3 slots)
// constructor  : 0x004615df
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent25Alpha_G : public Blitter
{
public:
    // ---- vtable 0x0069b504 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00469f10
    virtual void vfunc_1();  // [1] 0x00465360
    virtual void vfunc_2();  // [2] 0x00465430

    BlitTransLucent25Alpha_G();  // 0x004615df
    virtual ~BlitTransLucent25Alpha_G();

    // ---- data members (offsets from ctor 0x004611df) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b554
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
