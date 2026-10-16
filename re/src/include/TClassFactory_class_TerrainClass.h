#pragma once
// ==========================================================================
// TClassFactory_class_TerrainClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TerrainClass>
// vtable       : 0x006a48e0  (5 slots)
// constructor  : 0x005e8c98
// vtable store : 0x005e8cbb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
class TerrainClass;

class TClassFactory_class_TerrainClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a48e0 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005e8cc0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005e8d30  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005e8d50  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005e8d80  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005e8e00  // __stdcall, returns int

    TClassFactory_class_TerrainClass();  // 0x005e8c98
    virtual ~TClassFactory_class_TerrainClass();
};
