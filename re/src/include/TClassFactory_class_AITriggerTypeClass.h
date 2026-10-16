#pragma once
// ==========================================================================
// TClassFactory_class_AITriggerTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class AITriggerTypeClass>
// vtable       : 0x006a4e68  (5 slots)
// constructor  : 0x005ee518
// vtable store : 0x005ee53b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TriggerTypeClass.h"

class TClassFactory_class_AITriggerTypeClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4e68 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ee540  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ee5b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ee5d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ee600  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ee680  // __stdcall, returns int

    TClassFactory_class_AITriggerTypeClass();  // 0x005ee518
    virtual ~TClassFactory_class_AITriggerTypeClass();
};
