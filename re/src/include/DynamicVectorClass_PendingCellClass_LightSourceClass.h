#pragma once
// ==========================================================================
// DynamicVectorClass_PendingCellClass_LightSourceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<PendingCellClass*, LightSourceClass>
// vtable       : 0x006a1574  (6 slots)
// constructor  : 0x004b6b7a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_PendingCellClass_LightSourceClass.h"

class DynamicVectorClass_PendingCellClass_LightSourceClass : public VectorClass_PendingCellClass_LightSourceClass
{
public:
    // ---- vtable 0x006a1574 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004f5980  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f5b00  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004f56d0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f5bb0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004f5700  // __stdcall, returns int

    DynamicVectorClass_PendingCellClass_LightSourceClass();  // 0x004b6b7a
    virtual ~DynamicVectorClass_PendingCellClass_LightSourceClass();
};
