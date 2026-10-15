#pragma once
// ==========================================================================
// DynamicVectorClass_TriggerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TriggerClass*>
// vtable       : 0x0069ef7c  (6 slots)
// constructor  : 0x004a7a93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TriggerClass.h"

class DynamicVectorClass_TriggerClass : public VectorClass_TriggerClass
{
public:
    // ---- vtable 0x0069ef7c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ab190  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ab310  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9690  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ab3c0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a96c0  // __stdcall, returns int

    DynamicVectorClass_TriggerClass();  // 0x004a7a93
    virtual ~DynamicVectorClass_TriggerClass();
};
