#pragma once
// ==========================================================================
// TClassFactory_class_BuildingClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class BuildingClass>
// vtable       : 0x006a4c28  (5 slots)
// constructor  : 0x005ec13b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class TClassFactory_class_BuildingClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4c28 (5 slots) ----
    virtual void vfunc_0();  // [0] 0x005ec140
    virtual void vfunc_1();  // [1] 0x005ec1b0
    virtual void vfunc_2();  // [2] 0x005ec1d0
    virtual void vfunc_3();  // [3] 0x005ec200
    virtual void vfunc_4();  // [4] 0x005ec280

    TClassFactory_class_BuildingClass();  // 0x005ec13b
    virtual ~TClassFactory_class_BuildingClass();
};
