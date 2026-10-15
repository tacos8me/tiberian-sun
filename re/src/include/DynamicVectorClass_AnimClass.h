#pragma once
// ==========================================================================
// DynamicVectorClass_AnimClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<AnimClass*>
// vtable       : 0x0069f28c  (6 slots)
// constructor  : 0x004a8213
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_AnimClass.h"

class DynamicVectorClass_AnimClass : public VectorClass_AnimClass
{
public:
    // ---- vtable 0x0069f28c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1390  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004ad070  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ad1f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9aa0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ad2a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9ad0  // __stdcall, returns int

    DynamicVectorClass_AnimClass();  // 0x004a8213
    virtual ~DynamicVectorClass_AnimClass();
};
