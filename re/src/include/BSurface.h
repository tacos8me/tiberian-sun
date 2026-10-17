#pragma once
// ==========================================================================
// BSurface.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BSurface
// vtable       : 0x00698b58  (35 slots)
// constructor  : 0x004060c0
// vtable store : 0x0040612f  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "XSurface.h"

class BSurface : public XSurface
{
public:
    // ---- vtable 0x00698b58 (35 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2);  // [1] 0x00675870  // __thiscall, returns ?
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4);  // [2] 0x00675910  // __thiscall, returns void*
    virtual void vfunc_3(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [3] 0x00675a70  // __thiscall, returns void*
    virtual void vfunc_4(int a0, int a1, int a2);  // [4] 0x00674dd0  // __thiscall, returns char
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00674da0  // __thiscall, returns void*
    virtual void vfunc_6(int a0);  // [6] 0x00675830  // __thiscall, returns void*
    virtual void vfunc_7(int a0, int a1, int a2);  // [7] 0x006750c0  // __stdcall, returns int
    virtual void vfunc_8(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12);  // [8] 0x006750d0  // __thiscall, returns int
    virtual void vfunc_9(int a0, int a1);  // [9] 0x00674c30  // __thiscall, returns char
    virtual void vfunc_10(int a0);  // [10] 0x00674be0  // __thiscall, returns ?
    virtual void vfunc_11(int a0, int a1, int a2, int a3);  // [11] 0x006743a0  // __thiscall, returns char
    virtual void vfunc_12(int a0, int a1, int a2);  // [12] 0x00674370  // __thiscall, returns ?
    virtual void vfunc_13(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [13] 0x006748e0  // __stdcall, returns int
    virtual void vfunc_14(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [14] 0x006748f0  // __stdcall, returns int
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);  // [15] 0x00674900  // __stdcall, returns int
    virtual void vfunc_16(int a0, int a1, int a2, int a3);  // [16] 0x00674910  // __thiscall, returns int
    virtual void vfunc_17(int a0, int a1, int a2, int a3, int a4);  // [17] 0x00674650  // __thiscall, returns ?
    virtual void vfunc_18(int a0, int a1, int a2, int a3, int a4, int a5);  // [18] 0x006748c0  // __stdcall, returns int
    virtual void vfunc_19(int a0, int a1, int a2, int a3);  // [19] 0x006748d0  // __stdcall, returns int
    virtual void vfunc_20(int a0, int a1, int a2);  // [20] 0x00674b40  // __thiscall, returns char
    virtual void vfunc_21(int a0, int a1);  // [21] 0x00674b10  // __thiscall, returns void*
    virtual void vfunc_22(int a0, int a1);  // [22] 0x004069d0  // __thiscall, returns int
    virtual void vfunc_23();  // [23] 0x00406950  // __thiscall, returns char
    virtual void vfunc_24(int a0, int a1);  // [24] 0x004068d0  // __stdcall, returns char
    virtual void vfunc_25(int a0, int a1);  // [25] 0x004068e0  // __stdcall, returns char
    virtual void vfunc_26();  // [26] 0x00406960  // __thiscall, returns ?
    virtual void vfunc_27();  // [27] 0x00406a10  // __thiscall, returns ?
    virtual void vfunc_28();  // [28] 0x00406a20  // __thiscall, returns ?
    virtual void vfunc_29(int a0);  // [29] 0x004068f0  // __thiscall, returns ?
    virtual void vfunc_30();  // [30] 0x00406920  // __thiscall, returns ?
    virtual void vfunc_31();  // [31] 0x00406930  // __thiscall, returns ?
    virtual void vfunc_32();  // [32] 0x00406970  // __cdecl, returns int
    virtual void vfunc_33(int a0, int a1, int a2);  // [33] 0x00674d10  // __thiscall, returns char
    virtual void vfunc_34(int a0, int a1);  // [34] 0x00674c90  // __thiscall, returns ?

    BSurface();  // 0x004060c0
    virtual ~BSurface();

    // ---- data members (offsets from ctor 0x004060c0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x14: the compiled end of the
    // base sub-object region. Evidence: XSurface at +0x0 (ends +0x14).
    // 5 recovered offset(s) below +0x14 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10
};
