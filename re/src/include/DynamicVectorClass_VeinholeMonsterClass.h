#pragma once
// ==========================================================================
// DynamicVectorClass_VeinholeMonsterClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<VeinholeMonsterClass*>
// vtable       : 0x006a6e34  (6 slots)
// constructor  : 0x00643133
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_VeinholeMonsterClass.h"

class DynamicVectorClass_VeinholeMonsterClass : public VectorClass_VeinholeMonsterClass
{
public:
    // ---- vtable 0x006a6e34 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00645cd0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00645e50  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00645c80  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00645f00  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00645cb0  // __stdcall, returns int

    DynamicVectorClass_VeinholeMonsterClass();  // 0x00643133
    virtual ~DynamicVectorClass_VeinholeMonsterClass();
};
