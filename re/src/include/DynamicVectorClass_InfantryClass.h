#pragma once
// ==========================================================================
// DynamicVectorClass_InfantryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<InfantryClass*>
// vtable       : 0x0069f1ac  (6 slots)
// constructor  : 0x004a7f93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_InfantryClass.h"

class DynamicVectorClass_InfantryClass : public VectorClass_InfantryClass
{
public:
    // ---- vtable 0x0069f1ac (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1110  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004ac6f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ac870  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9960  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ac920  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9990  // __stdcall, returns int

    DynamicVectorClass_InfantryClass();  // 0x004a7f93
    virtual ~DynamicVectorClass_InfantryClass();
};
