#pragma once
// ==========================================================================
// DynamicVectorClass_ObjectTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ObjectTypeClass*>
// vtable       : 0x006a2a3c  (6 slots)
// constructor  : 0x005711a3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ObjectTypeClass.h"

class DynamicVectorClass_ObjectTypeClass : public VectorClass_ObjectTypeClass
{
public:
    // ---- vtable 0x006a2a3c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00572da0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00572f20  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00572d30  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00572fd0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00572d60  // __stdcall, returns int

    DynamicVectorClass_ObjectTypeClass();  // 0x005711a3
    virtual ~DynamicVectorClass_ObjectTypeClass();
};
