#pragma once
// ==========================================================================
// DynamicVectorClass_CommandClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<CommandClass*>
// vtable       : 0x00698650  (6 slots)
// constructor  : 0x00401663
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_CommandClass.h"

class DynamicVectorClass_CommandClass : public VectorClass_CommandClass
{
public:
    // ---- vtable 0x00698650 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00401710  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00401890  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004016c0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00401940  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004016f0  // __stdcall, returns int

    DynamicVectorClass_CommandClass();  // 0x00401663
    virtual ~DynamicVectorClass_CommandClass();
};
