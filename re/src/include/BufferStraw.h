#pragma once
// ==========================================================================
// BufferStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BufferStraw
// vtable       : 0x0069bbcc  (3 slots)
// constructor  : 0x00476910
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class BufferStraw : public Straw
{
public:
    // ---- vtable 0x0069bbcc (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00478980
    virtual void vfunc_1();  // [1] 0x005f1020
    virtual void vfunc_2();  // [2] 0x00674260

    BufferStraw();  // 0x00476910
    virtual ~BufferStraw();
};
