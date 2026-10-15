#pragma once
// ==========================================================================
// DynamicVectorClass_WinsockBufferType_WinsockInterfaceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<WinsockBufferType*, WinsockInterfaceClass>
// vtable       : 0x006a7a54  (6 slots)
// constructor  : 0x0066ea16
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_WinsockBufferType_WinsockInterfaceClass.h"

class DynamicVectorClass_WinsockBufferType_WinsockInterfaceClass : public VectorClass_WinsockBufferType_WinsockInterfaceClass
{
public:
    // ---- vtable 0x006a7a54 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0066f1a0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0066f320  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0066f150  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0066f3d0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0066f180  // __stdcall, returns int

    DynamicVectorClass_WinsockBufferType_WinsockInterfaceClass();  // 0x0066ea16
    virtual ~DynamicVectorClass_WinsockBufferType_WinsockInterfaceClass();
};
