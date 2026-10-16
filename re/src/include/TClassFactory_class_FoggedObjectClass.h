#pragma once
// ==========================================================================
// TClassFactory_class_FoggedObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class FoggedObjectClass>
// vtable       : 0x006a4e98  (5 slots)
// constructor  : 0x005ee818
// vtable store : 0x005ee83b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FoggedObjectClass.h"

class TClassFactory_class_FoggedObjectClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4e98 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ee840  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ee8b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ee8d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ee900  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ee980  // __stdcall, returns int

    TClassFactory_class_FoggedObjectClass();  // 0x005ee818
    virtual ~TClassFactory_class_FoggedObjectClass();
};
