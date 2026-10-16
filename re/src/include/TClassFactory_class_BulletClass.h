#pragma once
// ==========================================================================
// TClassFactory_class_BulletClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class BulletClass>
// vtable       : 0x006a4da8  (5 slots)
// constructor  : 0x005ed918
// vtable store : 0x005ed93b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "BulletClass.h"

class TClassFactory_class_BulletClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4da8 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ed940  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ed9b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ed9d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005eda00  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005eda80  // __stdcall, returns int

    TClassFactory_class_BulletClass();  // 0x005ed918
    virtual ~TClassFactory_class_BulletClass();
};
