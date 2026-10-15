#pragma once
// ==========================================================================
// DynamicVectorClass_VQHandle.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<VQHandle*>
// vtable       : 0x006a70b4  (6 slots)
// constructor  : 0x0064d863
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_VQHandle.h"

class DynamicVectorClass_VQHandle : public VectorClass_VQHandle
{
public:
    // ---- vtable 0x006a70b4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0064eab0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0064ec30  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0064ea60  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0064ece0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0064ea90  // __stdcall, returns int

    DynamicVectorClass_VQHandle();  // 0x0064d863
    virtual ~DynamicVectorClass_VQHandle();
};
