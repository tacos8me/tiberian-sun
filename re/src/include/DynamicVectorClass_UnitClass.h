#pragma once
// ==========================================================================
// DynamicVectorClass_UnitClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<UnitClass*>
// vtable       : 0x0069f21c  (6 slots)
// constructor  : 0x004a8113
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_UnitClass.h"

class DynamicVectorClass_UnitClass : public VectorClass_UnitClass
{
public:
    // ---- vtable 0x0069f21c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004acbb0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004acd30  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9a00  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004acde0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9a30  // __stdcall, returns int

    DynamicVectorClass_UnitClass();  // 0x004a8113
    virtual ~DynamicVectorClass_UnitClass();
};
