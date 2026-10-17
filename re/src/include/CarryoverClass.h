#pragma once
// ==========================================================================
// CarryoverClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CarryoverClass
// vtable       : 0x0069abe4  (10 slots)
// constructor  : 0x00447120
// vtable store : 0x00447149  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "LinkClass.h"

class CarryoverClass : public LinkClass
{
public:
    // ---- vtable 0x0069abe4 (10 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004472f0  // __thiscall, returns ?
    virtual void vfunc_1();  // [1] 0x004f6960  // __thiscall, returns ?
    virtual void vfunc_2();  // [2] 0x004f6970  // __thiscall, returns ?
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __thiscall, returns ?
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall, returns ?
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall, returns ?
    virtual void vfunc_6();  // [6] 0x004f6980  // __thiscall, returns ?
    virtual void vfunc_7();  // [7] 0x004f69b0  // __thiscall, returns ?
    virtual void vfunc_8();  // [8] 0x004f6930  // __thiscall, returns int
    virtual void vfunc_9();  // [9] 0x004f6a50  // __thiscall, returns int

    CarryoverClass();  // 0x00447120
    virtual ~CarryoverClass();

    // ---- data members (offsets from ctor 0x00447120) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x24: the compiled end of the
    // base sub-object region. Evidence: LinkClass at +0x0 (ends +0x24).
    // 9 recovered offset(s) below +0x24 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10, +0x14, +0x16, +0x18 ...
};
