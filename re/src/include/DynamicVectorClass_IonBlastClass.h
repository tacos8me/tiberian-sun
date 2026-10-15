#pragma once
// ==========================================================================
// DynamicVectorClass_IonBlastClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<IonBlastClass*>
// vtable       : 0x006a0a64  (6 slots)
// constructor  : 0x004e56f3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_IonBlastClass.h"

class DynamicVectorClass_IonBlastClass : public VectorClass_IonBlastClass
{
public:
    // ---- vtable 0x006a0a64 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004e69b0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004e6700  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004e6880  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004e66b0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004e6930  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004e66e0  // __stdcall, returns int

    DynamicVectorClass_IonBlastClass();  // 0x004e56f3
    virtual ~DynamicVectorClass_IonBlastClass();
};
