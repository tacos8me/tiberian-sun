#pragma once
// ==========================================================================
// ScoreFullFontClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreFullFontClass
// vtable       : 0x006a3f30  (5 slots)
// constructor  : 0x005cd830
// vtable store : 0x005cd864  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScoreFontClass.h"

class ScoreFullFontClass : public ScoreFontClass
{
public:
    // ---- vtable 0x006a3f30 (5 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005cd4d0  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005cd4a0  // __thiscall
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x005cd6e0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4);  // [4] 0x005cd590  // __thiscall

    ScoreFullFontClass();  // 0x005cd830
    virtual ~ScoreFullFontClass();

    // ---- data members (offsets from ctor 0x005cd830) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x18: the compiled end of the
    // base sub-object region. Evidence: ScoreFontClass at +0x0 (ends +0x18).
    // 6 recovered offset(s) below +0x18 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10, +0x14
};
