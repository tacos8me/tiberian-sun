#pragma once
// ==========================================================================
// DynamicVectorClass_VocClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<VocClass*>
// vtable       : 0x006a704c  (6 slots)
// constructor  : 0x00646cf3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_VocClass.h"

class DynamicVectorClass_VocClass : public VectorClass_VocClass
{
public:
    // ---- vtable 0x006a704c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00647a20  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00647770  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x006478f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00647720  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x006479a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00647750  // __stdcall, returns int

    DynamicVectorClass_VocClass();  // 0x00646cf3
    virtual ~DynamicVectorClass_VocClass();
};
