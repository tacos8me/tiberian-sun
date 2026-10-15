#pragma once
// ==========================================================================
// DynamicVectorClass_HWND.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<HWND__*>
// vtable       : 0x006a272c  (6 slots)
// constructor  : 0x0055f6a3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_HWND.h"

class DynamicVectorClass_HWND : public VectorClass_HWND
{
public:
    // ---- vtable 0x006a272c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00560120  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0055fb40  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055fcc0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0055faa0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055fd70  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0055fad0  // __stdcall, returns int

    DynamicVectorClass_HWND();  // 0x0055f6a3
    virtual ~DynamicVectorClass_HWND();
};
