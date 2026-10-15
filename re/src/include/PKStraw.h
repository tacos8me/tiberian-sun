#pragma once
// ==========================================================================
// PKStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class PKStraw
// vtable       : 0x006a3270  (4 slots)
// constructor  : 0x00593101
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class PKStraw : public Straw
{
public:
    // ---- vtable 0x006a3270 (4 slots) ----
    virtual void vfunc_0();  // [0] 0x005933d0
    virtual void vfunc_1();  // [1] 0x00593120
    virtual void vfunc_2();  // [2] 0x00593190
    virtual void vfunc_3();  // [3] 0x005933c0

    PKStraw();  // 0x00593101
    virtual ~PKStraw();
};
