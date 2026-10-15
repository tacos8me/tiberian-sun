#pragma once
// ==========================================================================
// DynamicVectorClass_SpotLightClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<SpotLightClass*>
// vtable       : 0x006a2e0c  (6 slots)
// constructor  : 0x00577b73
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_SpotLightClass.h"

class DynamicVectorClass_SpotLightClass : public VectorClass_SpotLightClass
{
public:
    // ---- vtable 0x006a2e0c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00578940  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00578690  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00578810  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00578640  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005788c0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00578670  // __stdcall, returns int

    DynamicVectorClass_SpotLightClass();  // 0x00577b73
    virtual ~DynamicVectorClass_SpotLightClass();
};
