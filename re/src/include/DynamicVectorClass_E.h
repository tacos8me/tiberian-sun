#pragma once
// ==========================================================================
// DynamicVectorClass_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<E&*>
// vtable       : 0x006a7aec  (6 slots)
// constructor  : 0x0066f6db
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_E.h"

class DynamicVectorClass_E : public VectorClass_E
{
public:
    // ---- vtable 0x006a7aec (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x006700d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00670250  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00670080  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00670300  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x006700b0  // __stdcall, returns int

    DynamicVectorClass_E();  // 0x0066f6db
    virtual ~DynamicVectorClass_E();
};
