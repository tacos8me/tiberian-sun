#pragma once
// ==========================================================================
// DynamicVectorClass_TerrainClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TerrainClass*>
// vtable       : 0x0069f174  (6 slots)
// constructor  : 0x004a7f13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TerrainClass.h"

class DynamicVectorClass_TerrainClass : public VectorClass_TerrainClass
{
public:
    // ---- vtable 0x0069f174 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ac490  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ac610  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9910  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ac6c0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9940  // __stdcall, returns int

    DynamicVectorClass_TerrainClass();  // 0x004a7f13
    virtual ~DynamicVectorClass_TerrainClass();
};
