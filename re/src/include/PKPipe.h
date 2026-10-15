#pragma once
// ==========================================================================
// PKPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class PKPipe
// vtable       : 0x006a323c  (6 slots)
// constructor  : 0x00592d5b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class PKPipe : public Pipe
{
public:
    // ---- vtable 0x006a323c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00593050
    virtual void vfunc_1();  // [1] 0x00591fb0
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3();  // [3] 0x00592d70
    virtual void vfunc_4();  // [4] 0x00592e20
    virtual void vfunc_5();  // [5] 0x00593040

    PKPipe();  // 0x00592d5b
    virtual ~PKPipe();
};
