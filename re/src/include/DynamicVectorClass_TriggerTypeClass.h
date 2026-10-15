#pragma once
// ==========================================================================
// DynamicVectorClass_TriggerTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TriggerTypeClass*>
// vtable       : 0x0069ef44  (6 slots)
// constructor  : 0x004a7a13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TriggerTypeClass.h"

class DynamicVectorClass_TriggerTypeClass : public VectorClass_TriggerTypeClass
{
public:
    // ---- vtable 0x0069ef44 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ab3f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ab570  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a96e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ab620  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9710  // __stdcall, returns int

    DynamicVectorClass_TriggerTypeClass();  // 0x004a7a13
    virtual ~DynamicVectorClass_TriggerTypeClass();
};
