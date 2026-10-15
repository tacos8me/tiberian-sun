#pragma once
// ==========================================================================
// RandomStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RandomStraw
// vtable       : 0x006a3b78  (3 slots)
// constructor  : 0x005ab413
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class RandomStraw : public Straw
{
public:
    // ---- vtable 0x006a3b78 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x005ab990
    virtual void vfunc_1();  // [1] 0x005f1020
    virtual void vfunc_2();  // [2] 0x005ab930

    RandomStraw();  // 0x005ab413
    virtual ~RandomStraw();
};
