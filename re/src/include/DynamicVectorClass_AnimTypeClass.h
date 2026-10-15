#pragma once
// ==========================================================================
// DynamicVectorClass_AnimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<AnimTypeClass*>
// vtable       : 0x0069f5d4  (6 slots)
// constructor  : 0x004a8993
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_AnimTypeClass.h"

class DynamicVectorClass_AnimTypeClass : public VectorClass_AnimTypeClass
{
public:
    // ---- vtable 0x0069f5d4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1c50  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004af1b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004af330  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9f00  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004af3e0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9f30  // __stdcall, returns int

    DynamicVectorClass_AnimTypeClass();  // 0x004a8993
    virtual ~DynamicVectorClass_AnimTypeClass();
};
