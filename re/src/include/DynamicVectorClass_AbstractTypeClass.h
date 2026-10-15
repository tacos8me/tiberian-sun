#pragma once
// ==========================================================================
// DynamicVectorClass_AbstractTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<AbstractTypeClass*>
// vtable       : 0x0069f7cc  (6 slots)
// constructor  : 0x004a8f93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_AbstractTypeClass.h"

class DynamicVectorClass_AbstractTypeClass : public VectorClass_AbstractTypeClass
{
public:
    // ---- vtable 0x0069f7cc (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b2150  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004b04b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004b0630  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004aa180  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004b06e0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004aa1b0  // __stdcall, returns int

    DynamicVectorClass_AbstractTypeClass();  // 0x004a8f93
    virtual ~DynamicVectorClass_AbstractTypeClass();
};
