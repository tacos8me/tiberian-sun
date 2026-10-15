#pragma once
// ==========================================================================
// DynamicVectorClass_SideClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<SideClass*>
// vtable       : 0x0069f36c  (6 slots)
// constructor  : 0x004a8413
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_SideClass.h"

class DynamicVectorClass_SideClass : public VectorClass_SideClass
{
public:
    // ---- vtable 0x0069f36c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ad790  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ad910  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9b90  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ad9c0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9bc0  // __stdcall, returns int

    DynamicVectorClass_SideClass();  // 0x004a8413
    virtual ~DynamicVectorClass_SideClass();
};
