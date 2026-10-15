#pragma once
// ==========================================================================
// TClassFactory_class_TiberiumClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TiberiumClass>
// vtable       : 0x006a49a0  (5 slots)
// constructor  : 0x005e98bb
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class TClassFactory_class_TiberiumClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a49a0 (5 slots) ----
    virtual void vfunc_0();  // [0] 0x005e98c0
    virtual void vfunc_1();  // [1] 0x005e9930
    virtual void vfunc_2();  // [2] 0x005e9950
    virtual void vfunc_3();  // [3] 0x005e9980
    virtual void vfunc_4();  // [4] 0x005e9a00

    TClassFactory_class_TiberiumClass();  // 0x005e98bb
    virtual ~TClassFactory_class_TiberiumClass();
};
