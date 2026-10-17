#pragma once
// ==========================================================================
// GadgetClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class GadgetClass
// vtable       : 0x0069e74c  (31 slots)
// constructor  : 0x004a5960
// vtable store : 0x004a599f  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "LinkClass.h"

class GadgetClass : public LinkClass
{
public:
    // ---- vtable 0x0069e74c (31 slots) ----
    virtual void vfunc_1();  // [1] 0x004a5b00
    virtual void vfunc_2();  // [2] 0x004a5b10
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __stdcall
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004a5ae0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x004a5be0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x004a5bb0  // __thiscall
    virtual void vfunc_12();  // [12] 0x004a5b20  // __thiscall
    virtual void vfunc_13(int a0);  // [13] 0x004a5de0  // __thiscall, returns int
    virtual void vfunc_14();  // [14] 0x0045afe0  // __cdecl
    virtual void vfunc_15();  // [15] 0x004a5ad0  // __thiscall
    virtual void vfunc_16();  // [16] 0x004a5ac0
    virtual void vfunc_17();  // [17] 0x004788d0  // __cdecl, returns int
    virtual void vfunc_18();  // [18] 0x004a5e20  // __cdecl
    virtual void vfunc_19(int a0, int a1, int a2);  // [19] 0x00460650  // __stdcall
    virtual void vfunc_20();  // [20] 0x004a5e60  // __thiscall
    virtual void vfunc_21();  // [21] 0x004a5e90  // __cdecl
    virtual void vfunc_22();  // [22] 0x004a5eb0  // __cdecl, returns int
    virtual void vfunc_23();  // [23] 0x004a5ec0  // __cdecl, returns int
    virtual void vfunc_24();  // [24] 0x0045aff0  // __cdecl
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004a5ee0  // __stdcall
    virtual void vfunc_26(int a0, int a1);  // [26] 0x004a5f00  // __stdcall
    virtual void vfunc_27(int a0);  // [27] 0x004a5b90  // __stdcall, returns int
    virtual void vfunc_28(int a0);  // [28] 0x004a5e30  // __stdcall
    virtual void vfunc_29(int a0, int a1);  // [29] 0x004a5b70  // __stdcall, returns int
    virtual void vfunc_30(int a0, int a1, int a2, int a3);  // [30] 0x004a5a60  // __stdcall, returns int

    GadgetClass();  // 0x004a5960
    virtual ~GadgetClass();

    // ---- data members (offsets from ctor 0x004a5960) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x24: the compiled end of the
    // base sub-object region. Evidence: LinkClass at +0x0 (ends +0x24).
    // 11 recovered offset(s) below +0x24 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10, +0x14, +0x18, +0x1c ...
};
