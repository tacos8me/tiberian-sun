#pragma once
// ==========================================================================
// WWMouseClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WWMouseClass
// vtable       : 0x006a7b84  (17 slots)
// constructor  : 0x006727c0
// vtable store : 0x006728d7  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Mouse.h"

class WWMouseClass : public Mouse
{
public:
    // ---- vtable 0x006a7b84 (17 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00674130  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00672a60  // __thiscall
    virtual void vfunc_2();  // [2] 0x006740d0  // __cdecl, returns int
    virtual void vfunc_3();  // [3] 0x00673900  // __thiscall
    virtual void vfunc_4();  // [4] 0x006737a0  // __thiscall
    virtual void vfunc_5();  // [5] 0x00673ab0  // __thiscall
    virtual void vfunc_6();  // [6] 0x00673a10  // __thiscall
    virtual void vfunc_7();  // [7] 0x006740e0  // __cdecl
    virtual void vfunc_8(int a0, int a1, int a2, int a3);  // [8] 0x00673b60  // __stdcall
    virtual void vfunc_9(int a0);  // [9] 0x00673b70
    virtual void vfunc_10();  // [10] 0x00672a30  // __thiscall
    virtual void vfunc_11();  // [11] 0x006740f0  // __cdecl
    virtual void vfunc_12();  // [12] 0x00674100  // __cdecl
    virtual void vfunc_13(int a0);  // [13] 0x00674110  // __stdcall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x00673150  // __thiscall
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5);  // [15] 0x00673350  // __thiscall
    virtual void vfunc_16(int a0, int a1);  // [16] 0x00673b80  // __stdcall

    WWMouseClass();  // 0x006727c0
    virtual ~WWMouseClass();
};
