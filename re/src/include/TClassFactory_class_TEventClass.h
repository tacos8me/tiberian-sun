#pragma once
// ==========================================================================
// TClassFactory_class_TEventClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TEventClass>
// vtable       : 0x006a4a18  (5 slots)
// constructor  : 0x005ea018
// vtable store : 0x005ea03b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
class TEventClass;

class TClassFactory_class_TEventClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4a18 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ea040  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ea0b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ea0d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ea100  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ea180  // __stdcall, returns int

    TClassFactory_class_TEventClass();  // 0x005ea018
    virtual ~TClassFactory_class_TEventClass();
};
