#pragma once
// ==========================================================================
// DynamicVectorClass_BulletTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<BulletTypeClass*>
// vtable       : 0x0069f644  (6 slots)
// constructor  : 0x004a8a93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_BulletTypeClass.h"

class DynamicVectorClass_BulletTypeClass : public VectorClass_BulletTypeClass
{
public:
    // ---- vtable 0x0069f644 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1d90  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004af670  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004af7f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9fa0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004af8a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9fd0  // __stdcall, returns int

    DynamicVectorClass_BulletTypeClass();  // 0x004a8a93
    virtual ~DynamicVectorClass_BulletTypeClass();
};
