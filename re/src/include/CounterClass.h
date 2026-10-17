#pragma once
// ==========================================================================
// CounterClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CounterClass
// vtable       : 0x0069b7ec  (6 slots)
// constructor  : 0x0046b270
// vtable store : 0x0046b283  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass.h"

class H;  // RTTI formal param, no type evidence
class CounterClass : public VectorClass<H>
{
public:
    // ---- vtable 0x0069b7ec (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0044af30  // __thiscall, returns char
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044af70  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0046b3e0  // __thiscall, returns int
    virtual void vfunc_4(int a0);  // [4] 0x0044b050  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0044b090  // __thiscall, returns int

    CounterClass();  // 0x0046b270
    virtual ~CounterClass();

    // ---- data members (offsets from ctor 0x0046b270) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x18: the compiled end of the
    // base sub-object region. Evidence: VectorClass_H at +0x0 (ends +0x18).
    // 5 recovered offset(s) below +0x18 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0xd
};
