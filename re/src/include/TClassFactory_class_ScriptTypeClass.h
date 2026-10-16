#pragma once
// ==========================================================================
// TClassFactory_class_ScriptTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class ScriptTypeClass>
// vtable       : 0x006a4ac0  (5 slots)
// constructor  : 0x005eaa98
// vtable store : 0x005eaabb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScriptTypeClass.h"

class TClassFactory_class_ScriptTypeClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4ac0 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005eaac0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005eab30  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005eab50  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005eab80  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005eac00  // __stdcall, returns int

    TClassFactory_class_ScriptTypeClass();  // 0x005eaa98
    virtual ~TClassFactory_class_ScriptTypeClass();
};
