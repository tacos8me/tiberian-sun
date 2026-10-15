#pragma once
// ==========================================================================
// DynamicVectorClass_TaskForceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TaskForceClass*>
// vtable       : 0x0069f3dc  (6 slots)
// constructor  : 0x004a8513
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TaskForceClass.h"

class DynamicVectorClass_TaskForceClass : public VectorClass_TaskForceClass
{
public:
    // ---- vtable 0x0069f3dc (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004adc50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004addd0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9c30  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ade80  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9c60  // __stdcall, returns int

    DynamicVectorClass_TaskForceClass();  // 0x004a8513
    virtual ~DynamicVectorClass_TaskForceClass();
};
