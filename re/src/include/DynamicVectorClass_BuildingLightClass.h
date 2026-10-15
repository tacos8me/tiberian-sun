#pragma once
// ==========================================================================
// DynamicVectorClass_BuildingLightClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<BuildingLightClass*>
// vtable       : 0x0069efb4  (6 slots)
// constructor  : 0x004a7b13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_BuildingLightClass.h"

class DynamicVectorClass_BuildingLightClass : public VectorClass_BuildingLightClass
{
public:
    // ---- vtable 0x0069efb4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b0940  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aa8f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aaa70  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a95a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aab20  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a95d0  // __stdcall, returns int

    DynamicVectorClass_BuildingLightClass();  // 0x004a7b13
    virtual ~DynamicVectorClass_BuildingLightClass();
};
