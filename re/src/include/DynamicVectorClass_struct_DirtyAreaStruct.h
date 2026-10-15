#pragma once
// ==========================================================================
// DynamicVectorClass_struct_DirtyAreaStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct DirtyAreaStruct>
// vtable       : 0x006a520c  (6 slots)
// constructor  : 0x005f4a03
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_DirtyAreaStruct.h"

class DynamicVectorClass_struct_DirtyAreaStruct : public VectorClass_struct_DirtyAreaStruct
{
public:
    // ---- vtable 0x006a520c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005fd390  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x005fd600  // __thiscall
    virtual void vfunc_3();  // [3] 0x005fd2e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005fd720  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005fd310  // __stdcall, returns int

    DynamicVectorClass_struct_DirtyAreaStruct();  // 0x005f4a03
    virtual ~DynamicVectorClass_struct_DirtyAreaStruct();
};
