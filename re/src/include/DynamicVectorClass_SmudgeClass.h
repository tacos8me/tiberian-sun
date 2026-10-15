#pragma once
// ==========================================================================
// DynamicVectorClass_SmudgeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<SmudgeClass*>
// vtable       : 0x0069f13c  (6 slots)
// constructor  : 0x004a7e93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_SmudgeClass.h"

class DynamicVectorClass_SmudgeClass : public VectorClass_SmudgeClass
{
public:
    // ---- vtable 0x0069f13c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ac230  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ac3b0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a98c0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ac460  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a98f0  // __stdcall, returns int

    DynamicVectorClass_SmudgeClass();  // 0x004a7e93
    virtual ~DynamicVectorClass_SmudgeClass();
};
