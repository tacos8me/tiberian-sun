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
    virtual void vfunc_1(int a0, int a1, int a2);  // [1] 0x00672a60  // __thiscall, returns ?
    virtual void vfunc_2();  // [2] 0x006740d0  // __thiscall, returns ?
    virtual void vfunc_3();  // [3] 0x00673900  // __thiscall, returns ?
    virtual void vfunc_4();  // [4] 0x006737a0  // __thiscall, returns ?
    virtual void vfunc_5();  // [5] 0x00673ab0  // __thiscall, returns ?
    virtual void vfunc_6();  // [6] 0x00673a10  // __thiscall, returns ?
    virtual void vfunc_7();  // [7] 0x006740e0  // __thiscall, returns char
    virtual void vfunc_8(int a0, int a1, int a2, int a3);  // [8] 0x00673b60  // __thiscall, returns ?
    virtual void vfunc_9(int a0, int a1);  // [9] 0x00673b70  // __thiscall, returns ?
    virtual void vfunc_10();  // [10] 0x00672a30  // __thiscall, returns ?
    virtual void vfunc_11();  // [11] 0x006740f0  // __thiscall, returns ?
    virtual void vfunc_12();  // [12] 0x00674100  // __thiscall, returns ?
    virtual void vfunc_13(int a0);  // [13] 0x00674110  // __thiscall, returns ?
    virtual void vfunc_14(int a0, int a1, int a2, int a3, int a4, int a5);  // [14] 0x00673150  // __thiscall, returns ?
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4);  // [15] 0x00673350  // __thiscall, returns ?
    virtual void vfunc_16(int a0, int a1);  // [16] 0x00673b80  // __thiscall, returns ?

    WWMouseClass();  // 0x006727c0
    virtual ~WWMouseClass();

    // ---- data members (offsets from ctor 0x006727c0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x94: the compiled end of the
    // base sub-object region. Evidence: Mouse at +0x0 (ends +0x94).
    // 21 recovered offset(s) below +0x94 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10, +0x14, +0x18, +0x1c ...
};
