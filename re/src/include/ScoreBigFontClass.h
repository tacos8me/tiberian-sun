#pragma once
// ==========================================================================
// ScoreBigFontClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreBigFontClass
// vtable       : 0x006a3f48  (5 slots)
// constructor  : 0x005cd8f0
// vtable store : 0x005cd924  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScoreFontClass.h"

class ScoreBigFontClass : public ScoreFontClass
{
public:
    // ---- vtable 0x006a3f48 (5 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005cd4d0  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005cd4a0  // __thiscall, returns ?
    virtual void vfunc_3(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);  // [3] 0x005cd6e0  // __thiscall, returns void*
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12);  // [4] 0x005cd590  // __thiscall, returns ?

    ScoreBigFontClass();  // 0x005cd8f0
    virtual ~ScoreBigFontClass();

    // ---- data members (offsets from ctor 0x005cd8f0) ----
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
