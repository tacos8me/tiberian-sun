#pragma once
// ==========================================================================
// DynamicVectorClass_BulletClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<BulletClass*>
// vtable       : 0x0069f094  (6 slots)
// constructor  : 0x004a7d13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_BulletClass.h"

class DynamicVectorClass_BulletClass : public VectorClass_BulletClass
{
public:
    // ---- vtable 0x0069f094 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b0e90  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004abd70  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004abef0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9820  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004abfa0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9850  // __stdcall, returns int

    DynamicVectorClass_BulletClass();  // 0x004a7d13
    virtual ~DynamicVectorClass_BulletClass();
};
