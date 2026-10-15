#pragma once
// ==========================================================================
// DynamicVectorClass_FactoryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<FactoryClass*>
// vtable       : 0x0069f2fc  (6 slots)
// constructor  : 0x004a8313
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_FactoryClass.h"

class DynamicVectorClass_FactoryClass : public VectorClass_FactoryClass
{
public:
    // ---- vtable 0x0069f2fc (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ad2d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ad450  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9af0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ad500  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9b20  // __stdcall, returns int

    DynamicVectorClass_FactoryClass();  // 0x004a8313
    virtual ~DynamicVectorClass_FactoryClass();
};
