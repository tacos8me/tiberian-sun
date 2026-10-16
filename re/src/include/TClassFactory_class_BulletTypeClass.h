#pragma once
// ==========================================================================
// TClassFactory_class_BulletTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class BulletTypeClass>
// vtable       : 0x006a4bb0  (5 slots)
// constructor  : 0x005eb998
// vtable store : 0x005eb9bb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "BulletTypeClass.h"

class TClassFactory_class_BulletTypeClass : public IClassFactory
{
public:
    // ---- vtable 0x006a4bb0 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005eb9c0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005eba30  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005eba50  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005eba80  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ebb00  // __stdcall, returns int

    TClassFactory_class_BulletTypeClass();  // 0x005eb998
    virtual ~TClassFactory_class_BulletTypeClass();
};
