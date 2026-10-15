#pragma once
// ==========================================================================
// DynamicVectorClass_EgoClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<EgoClass*>
// vtable       : 0x0069de64  (6 slots)
// constructor  : 0x0048d693
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_EgoClass.h"

class DynamicVectorClass_EgoClass : public VectorClass_EgoClass
{
public:
    // ---- vtable 0x0069de64 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0048e4d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0048e650  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0048e480  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0048e700  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0048e4b0  // __stdcall, returns int

    DynamicVectorClass_EgoClass();  // 0x0048d693
    virtual ~DynamicVectorClass_EgoClass();
};
