#pragma once
// ==========================================================================
// DynamicVectorClass_ShadowControlClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ShadowControlClass*>
// vtable       : 0x006a5244  (6 slots)
// constructor  : 0x004cfbed
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ShadowControlClass.h"

class DynamicVectorClass_ShadowControlClass : public VectorClass_ShadowControlClass
{
public:
    // ---- vtable 0x006a5244 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005fdad0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005fd780  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005fd900  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005fd340  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005fd9b0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005fd370  // __stdcall, returns int

    DynamicVectorClass_ShadowControlClass();  // 0x004cfbed
    virtual ~DynamicVectorClass_ShadowControlClass();
};
