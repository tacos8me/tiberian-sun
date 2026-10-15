#pragma once
// ==========================================================================
// DynamicVectorClass_struct_UndoInfoStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct UndoInfoStruct>
// vtable       : 0x006a4564  (6 slots)
// constructor  : 0x005de733
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_UndoInfoStruct.h"

class DynamicVectorClass_struct_UndoInfoStruct : public VectorClass_struct_UndoInfoStruct
{
public:
    // ---- vtable 0x006a4564 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005e0730  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005e0940  // __thiscall
    virtual void vfunc_3();  // [3] 0x005e06d0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005e0a50  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005e0700  // __stdcall, returns int

    DynamicVectorClass_struct_UndoInfoStruct();  // 0x005de733
    virtual ~DynamicVectorClass_struct_UndoInfoStruct();
};
