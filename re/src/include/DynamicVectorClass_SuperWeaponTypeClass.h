#pragma once
// ==========================================================================
// DynamicVectorClass_SuperWeaponTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<SuperWeaponTypeClass*>
// vtable       : 0x0069f75c  (6 slots)
// constructor  : 0x004a8d13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_SuperWeaponTypeClass.h"

class DynamicVectorClass_SuperWeaponTypeClass : public VectorClass_SuperWeaponTypeClass
{
public:
    // ---- vtable 0x0069f75c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004b0250  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004b03d0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004aa130  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004b0480  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004aa160  // __stdcall, returns int

    DynamicVectorClass_SuperWeaponTypeClass();  // 0x004a8d13
    virtual ~DynamicVectorClass_SuperWeaponTypeClass();
};
