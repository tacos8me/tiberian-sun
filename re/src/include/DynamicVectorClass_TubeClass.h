#pragma once
// ==========================================================================
// DynamicVectorClass_TubeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TubeClass*>
// vtable       : 0x0069f024  (6 slots)
// constructor  : 0x004a7c13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TubeClass.h"

class DynamicVectorClass_TubeClass : public VectorClass_TubeClass
{
public:
    // ---- vtable 0x0069f024 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b0cb0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004ab650  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ab7d0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9730  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ab880  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9760  // __stdcall, returns int

    DynamicVectorClass_TubeClass();  // 0x004a7c13
    virtual ~DynamicVectorClass_TubeClass();
};
