#pragma once
// ==========================================================================
// DynamicVectorClass_FootClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<FootClass*>
// vtable       : 0x0069e2ec  (6 slots)
// constructor  : 0x0049c093
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_FootClass.h"

class DynamicVectorClass_FootClass : public VectorClass_FootClass
{
public:
    // ---- vtable 0x0069e2ec (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004a4c30  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004a4db0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a4b80  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004a4e60  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a4bb0  // __stdcall, returns int

    DynamicVectorClass_FootClass();  // 0x0049c093
    virtual ~DynamicVectorClass_FootClass();
};
