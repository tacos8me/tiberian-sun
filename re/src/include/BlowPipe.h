#pragma once
// ==========================================================================
// BlowPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlowPipe
// vtable       : 0x006a3258  (5 slots)
// constructor  : 0x00592d43
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class BlowPipe : public Pipe
{
public:
    // ---- vtable 0x006a3258 (5 slots) ----
    virtual void vfunc_0();  // [0] 0x00592ff0
    virtual void vfunc_1();  // [1] 0x00423470
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3();  // [3] 0x00591f40
    virtual void vfunc_4();  // [4] 0x004234b0

    BlowPipe();  // 0x00592d43
    virtual ~BlowPipe();
};
