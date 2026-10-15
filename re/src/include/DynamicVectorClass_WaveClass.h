#pragma once
// ==========================================================================
// DynamicVectorClass_WaveClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<WaveClass*>
// vtable       : 0x0069f05c  (6 slots)
// constructor  : 0x004a7c93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_WaveClass.h"

class DynamicVectorClass_WaveClass : public VectorClass_WaveClass
{
public:
    // ---- vtable 0x0069f05c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004abb10  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004abc90  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a97d0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004abd40  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9800  // __stdcall, returns int

    DynamicVectorClass_WaveClass();  // 0x004a7c93
    virtual ~DynamicVectorClass_WaveClass();
};
