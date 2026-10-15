#pragma once
// ==========================================================================
// DynamicVectorClass_SmudgeTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<SmudgeTypeClass*>
// vtable       : 0x0069f4bc  (6 slots)
// constructor  : 0x004a8713
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_SmudgeTypeClass.h"

class DynamicVectorClass_SmudgeTypeClass : public VectorClass_SmudgeTypeClass
{
public:
    // ---- vtable 0x0069f4bc (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1930  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004ae5d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ae750  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9d70  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ae800  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9da0  // __stdcall, returns int

    DynamicVectorClass_SmudgeTypeClass();  // 0x004a8713
    virtual ~DynamicVectorClass_SmudgeTypeClass();
};
