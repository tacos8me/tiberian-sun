#pragma once
// ==========================================================================
// DynamicVectorClass_HouseTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<HouseTypeClass*>
// vtable       : 0x0069f3a4  (6 slots)
// constructor  : 0x004a8493
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_HouseTypeClass.h"

class DynamicVectorClass_HouseTypeClass : public VectorClass_HouseTypeClass
{
public:
    // ---- vtable 0x0069f3a4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1610  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004ad9f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004adb70  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9be0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004adc20  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9c10  // __stdcall, returns int

    DynamicVectorClass_HouseTypeClass();  // 0x004a8493
    virtual ~DynamicVectorClass_HouseTypeClass();
};
