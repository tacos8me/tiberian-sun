#pragma once
// ==========================================================================
// BlitPlain_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitPlain<E>
// vtable       : 0x006a7c24  (3 slots)
// constructor  : 0x00455950
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitPlain_E : public Blitter
{
public:
    // ---- vtable 0x006a7c24 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x006763c0
    virtual void vfunc_1();  // [1] 0x00676470
    virtual void vfunc_2();  // [2] 0x006764a0

    BlitPlain_E();  // 0x00455950
    virtual ~BlitPlain_E();
};
