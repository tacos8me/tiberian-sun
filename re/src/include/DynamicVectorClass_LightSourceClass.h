#pragma once
// ==========================================================================
// DynamicVectorClass_LightSourceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<LightSourceClass*>
// vtable       : 0x006a153c  (6 slots)
// constructor  : 0x004f4bb3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_LightSourceClass.h"

class DynamicVectorClass_LightSourceClass : public VectorClass_LightSourceClass
{
public:
    // ---- vtable 0x006a153c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004f5c30  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004f5720  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f58a0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004f5680  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f5950  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004f56b0  // __stdcall, returns int

    DynamicVectorClass_LightSourceClass();  // 0x004f4bb3
    virtual ~DynamicVectorClass_LightSourceClass();
};
