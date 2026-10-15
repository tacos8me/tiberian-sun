#pragma once
// ==========================================================================
// DynamicVectorClass_OverlayTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<OverlayTypeClass*>
// vtable       : 0x0069f484  (6 slots)
// constructor  : 0x004a8693
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_OverlayTypeClass.h"

class DynamicVectorClass_OverlayTypeClass : public VectorClass_OverlayTypeClass
{
public:
    // ---- vtable 0x0069f484 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ae370  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ae4f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9d20  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ae5a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9d50  // __stdcall, returns int

    DynamicVectorClass_OverlayTypeClass();  // 0x004a8693
    virtual ~DynamicVectorClass_OverlayTypeClass();
};
