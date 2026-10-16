#pragma once
// ==========================================================================
// ApplicationClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ApplicationClass
// vtable       : 0x00699c2c  (11 slots)
// constructor  : 0x004191aa
// vtable store : 0x00419406  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ApplicationClass : public IApplication, public IUnknown
{
public:
    // ---- vtable 0x00699c2c (11 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x00419560  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x004195e0  // __stdcall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x004195f0  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00419620  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x00419650  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004196a0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1);  // [6] 0x004196b0  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00419700  // __stdcall, returns int
    virtual void vfunc_8(int a0, int a1);  // [8] 0x00419750  // __stdcall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x00419600  // __stdcall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00419680  // __stdcall

    ApplicationClass();  // 0x004191aa
    virtual ~ApplicationClass();
};
