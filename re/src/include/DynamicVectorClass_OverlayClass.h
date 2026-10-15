#pragma once
// ==========================================================================
// DynamicVectorClass_OverlayClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<OverlayClass*>
// vtable       : 0x0069f0cc  (6 slots)
// constructor  : 0x004a7d93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_OverlayClass.h"

class DynamicVectorClass_OverlayClass : public VectorClass_OverlayClass
{
public:
    // ---- vtable 0x0069f0cc (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004abfd0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ac150  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9870  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ac200  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a98a0  // __stdcall, returns int

    DynamicVectorClass_OverlayClass();  // 0x004a7d93
    virtual ~DynamicVectorClass_OverlayClass();
};
