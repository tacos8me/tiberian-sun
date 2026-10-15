#pragma once
// ==========================================================================
// Blitter.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Blitter
// vtable       : 0x0069b7c4  (3 slots)
// constructor  : 0x0046987b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class Blitter
{
public:
    // ---- vtable 0x0069b7c4 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00469890
    virtual void vfunc_1();  // [1] 0x006829a5
    virtual void vfunc_2();  // [2] 0x006829a5

    Blitter();  // 0x0046987b
    virtual ~Blitter();

    // ---- data members (offsets from ctor 0x00469870) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7c4
};
