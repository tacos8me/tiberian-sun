#pragma once
// ==========================================================================
// BlowStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlowStraw
// vtable       : 0x006a2308  (3 slots)
// constructor  : 0x00547415
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class BlowStraw : public Straw
{
public:
    // ---- vtable 0x006a2308 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00547b90
    virtual void vfunc_1();  // [1] 0x005f1020
    virtual void vfunc_2();  // [2] 0x00423620

    BlowStraw();  // 0x00547415
    virtual ~BlowStraw();
};
