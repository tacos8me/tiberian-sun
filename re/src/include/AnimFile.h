#pragma once
// ==========================================================================
// AnimFile.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AnimFile
// vtable       : 0x00699a6c  (8 slots)
// constructor  : 0x004169c3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Animate.h"

class AnimFile : public Animate
{
public:
    // ---- vtable 0x00699a6c (8 slots) ----
    virtual void vfunc_0();  // [0] 0x004175a0
    virtual void vfunc_1();  // [1] 0x00416a50
    virtual void vfunc_2();  // [2] 0x00416a90
    virtual void vfunc_3();  // [3] 0x00416d90
    virtual void vfunc_4();  // [4] 0x00417590
    virtual void vfunc_5();  // [5] 0x00416d50
    virtual void vfunc_6();  // [6] 0x00416d70
    virtual void vfunc_7();  // [7] 0x00416d80

    AnimFile();  // 0x004169c3
    virtual ~AnimFile();
};
