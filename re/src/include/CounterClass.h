#pragma once
// ==========================================================================
// CounterClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CounterClass
// vtable       : 0x0069b7ec  (6 slots)
// constructor  : 0x0046b283
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_H.h"

class CounterClass : public VectorClass_H
{
public:
    // ---- vtable 0x0069b7ec (6 slots) ----
    virtual void vfunc_0();  // [0] 0x0046b570
    virtual void vfunc_1();  // [1] 0x0044af30
    virtual void vfunc_2();  // [2] 0x0044af70
    virtual void vfunc_3();  // [3] 0x0046b3e0
    virtual void vfunc_4();  // [4] 0x0044b050
    virtual void vfunc_5();  // [5] 0x0044b090

    CounterClass();  // 0x0046b283
    virtual ~CounterClass();
};
