#pragma once
// ==========================================================================
// DynamicVectorClass_ConvertClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ConvertClass*>
// vtable       : 0x0069b1d0  (6 slots)
// constructor  : 0x004606c3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ConvertClass.h"

class DynamicVectorClass_ConvertClass : public VectorClass_ConvertClass
{
public:
    // ---- vtable 0x0069b1d0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0046aa50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0046abd0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00463330  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0046ac80  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00463360  // __stdcall, returns int

    DynamicVectorClass_ConvertClass();  // 0x004606c3
    virtual ~DynamicVectorClass_ConvertClass();
};
