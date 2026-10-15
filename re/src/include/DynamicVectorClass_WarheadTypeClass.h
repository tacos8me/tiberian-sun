#pragma once
// ==========================================================================
// DynamicVectorClass_WarheadTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<WarheadTypeClass*>
// vtable       : 0x006989c4  (6 slots)
// constructor  : 0x004049d3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_WarheadTypeClass.h"

class DynamicVectorClass_WarheadTypeClass : public VectorClass_WarheadTypeClass
{
public:
    // ---- vtable 0x006989c4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00404e00  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00404b50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00404cd0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00404b00  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00404d80  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00404b30  // __stdcall, returns int

    DynamicVectorClass_WarheadTypeClass();  // 0x004049d3
    virtual ~DynamicVectorClass_WarheadTypeClass();
};
