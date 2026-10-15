#pragma once
// ==========================================================================
// DynamicVectorClass_AbstractClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<AbstractClass*>
// vtable       : 0x0069aa58  (6 slots)
// constructor  : 0x004449e8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_AbstractClass.h"

class DynamicVectorClass_AbstractClass : public VectorClass_AbstractClass
{
public:
    // ---- vtable 0x0069aa58 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x004455c0
    virtual void vfunc_1();  // [1] 0x00445310
    virtual void vfunc_2();  // [2] 0x00445490
    virtual void vfunc_3();  // [3] 0x004452a0
    virtual void vfunc_4();  // [4] 0x00445540
    virtual void vfunc_5();  // [5] 0x004452d0

    DynamicVectorClass_AbstractClass();  // 0x004449e8
    virtual ~DynamicVectorClass_AbstractClass();
};
