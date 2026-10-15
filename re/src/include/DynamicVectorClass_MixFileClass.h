#pragma once
// ==========================================================================
// DynamicVectorClass_MixFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MixFileClass*>
// vtable       : 0x00698824  (6 slots)
// constructor  : 0x00402bb3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MixFileClass.h"

class DynamicVectorClass_MixFileClass : public VectorClass_MixFileClass
{
public:
    // ---- vtable 0x00698824 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00402c60  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00402de0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00402c10  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00402e90  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00402c40  // __stdcall, returns int

    DynamicVectorClass_MixFileClass();  // 0x00402bb3
    virtual ~DynamicVectorClass_MixFileClass();
};
