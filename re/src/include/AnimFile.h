#pragma once
// ==========================================================================
// AnimFile.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AnimFile
// vtable       : 0x00699a6c  (8 slots)
// constructor  : 0x00416970
// vtable store : 0x004169c3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Animate.h"

class AnimFile : public Animate
{
public:
    // ---- vtable 0x00699a6c (8 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00416a50  // __thiscall, returns ?
    virtual void vfunc_2();  // [2] 0x00416a90  // __thiscall, returns ?
    virtual void vfunc_3(int a0);  // [3] 0x00416d90  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x00417590  // __thiscall, returns void*
    virtual void vfunc_5();  // [5] 0x00416d50  // __thiscall, returns int
    virtual void vfunc_6();  // [6] 0x00416d70  // __thiscall, returns short
    virtual void vfunc_7();  // [7] 0x00416d80  // __thiscall, returns short

    AnimFile();  // 0x00416970
    virtual ~AnimFile();

    // ---- data members (offsets from ctor 0x00416970) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0xe1c: the compiled end of the
    // base sub-object region. Evidence: Animate at +0x0 (ends +0xe1c).
    // 8 recovered offset(s) below +0xe1c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x84, +0x88, +0x89, +0xe0c, +0xe10, +0xe14, +0xe18
};
