#pragma once
// ==========================================================================
// SuperClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SuperClass
// vtable       : 0x006a4f6c  (6 slots)
// constructor  : 0x005f131c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class SuperClass : public AbstractClass
{
public:
    // ---- vtable 0x006a4f6c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x005f2810
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    SuperClass();  // 0x005f131c
    virtual ~SuperClass();
};
