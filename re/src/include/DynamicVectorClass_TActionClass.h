#pragma once
// ==========================================================================
// DynamicVectorClass_TActionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TActionClass*>
// vtable       : 0x006a5310  (6 slots)
// constructor  : 0x005fdd93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TActionClass.h"

class DynamicVectorClass_TActionClass : public VectorClass_TActionClass
{
public:
    // ---- vtable 0x006a5310 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00602e20  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00602b70  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00602cf0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00602ad0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00602da0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00602b00  // __stdcall, returns int

    DynamicVectorClass_TActionClass();  // 0x005fdd93
    virtual ~DynamicVectorClass_TActionClass();
};
