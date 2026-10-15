#pragma once
// ==========================================================================
// DynamicVectorClass_WeaponTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<WeaponTypeClass*>
// vtable       : 0x00698a04  (6 slots)
// constructor  : 0x004050a3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_WeaponTypeClass.h"

class DynamicVectorClass_WeaponTypeClass : public VectorClass_WeaponTypeClass
{
public:
    // ---- vtable 0x00698a04 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00405400  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00405150  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004052d0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00405100  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00405380  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00405130  // __stdcall, returns int

    DynamicVectorClass_WeaponTypeClass();  // 0x004050a3
    virtual ~DynamicVectorClass_WeaponTypeClass();
};
