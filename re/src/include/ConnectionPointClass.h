#pragma once
// ==========================================================================
// ConnectionPointClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ConnectionPointClass
// vtable       : 0x0069b86c  (8 slots)
// constructor  : 0x0046be10
// vtable store : 0x0046be3c  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ConnectionPointClass : public IConnectionPoint
{
public:
    // ---- vtable 0x0069b86c (8 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x0046bd80  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x0046bdf0  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0046bea0  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0046bee0  // __stdcall, returns ?
    virtual void vfunc_5(int a0, int a1, int a2);  // [5] 0x0046bf00  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1);  // [6] 0x0046bfd0  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x0046c030  // __stdcall, returns int

    ConnectionPointClass();  // 0x0046be10
    virtual ~ConnectionPointClass();

    // ---- data members (offsets from ctor 0x0046be10) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: IConnectionPoint at +0x0 (ends +0x4).
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
};
