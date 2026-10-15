#pragma once
// ==========================================================================
// MSFadeAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSFadeAnim
// vtable       : 0x006a24a8  (6 slots)
// constructor  : 0x00558110
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSShapeAnim.h"

class MSFadeAnim : public MSShapeAnim
{
public:
    // ---- vtable 0x006a24a8 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x005598a0
    virtual void vfunc_1();  // [1] 0x00557690
    virtual void vfunc_2();  // [2] 0x005576a0
    virtual void vfunc_3();  // [3] 0x005576f0
    virtual void vfunc_4();  // [4] 0x00558120
    virtual void vfunc_5();  // [5] 0x00558500

    MSFadeAnim();  // 0x00558110
    virtual ~MSFadeAnim();
};
