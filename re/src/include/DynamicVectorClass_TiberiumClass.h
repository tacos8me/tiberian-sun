#pragma once
// ==========================================================================
// DynamicVectorClass_TiberiumClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TiberiumClass*>
// vtable       : 0x006a61f4  (6 slots)
// constructor  : 0x00628183
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TiberiumClass.h"

class DynamicVectorClass_TiberiumClass : public VectorClass_TiberiumClass
{
public:
    // ---- vtable 0x006a61f4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0062a420  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0062a170  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0062a2f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0062a120  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0062a3a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0062a150  // __stdcall, returns int

    DynamicVectorClass_TiberiumClass();  // 0x00628183
    virtual ~DynamicVectorClass_TiberiumClass();
};
