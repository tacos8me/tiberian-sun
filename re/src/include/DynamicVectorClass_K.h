#pragma once
// ==========================================================================
// DynamicVectorClass_K.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<K>
// vtable       : 0x006a4860  (6 slots)
// constructor  : 0x005e5463
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_K.h"

class DynamicVectorClass_K : public VectorClass_K
{
public:
    // ---- vtable 0x006a4860 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005eee40  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005eeb40  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005eecc0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005e8750  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005eedc0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005e8780  // __stdcall, returns int

    DynamicVectorClass_K();  // 0x005e5463
    virtual ~DynamicVectorClass_K();
};
