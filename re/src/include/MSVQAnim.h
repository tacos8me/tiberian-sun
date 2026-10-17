#pragma once
// ==========================================================================
// MSVQAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSVQAnim
// vtable       : 0x006a24e0  (6 slots)
// constructor  : 0x00558a10
// vtable store : 0x00558a5b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSAnim.h"

class MSVQAnim : public MSAnim
{
public:
    // ---- vtable 0x006a24e0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __thiscall, returns char
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall, returns ?
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall, returns void
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4);  // [4] 0x00558c70  // __thiscall, returns ?
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00558dc0  // __thiscall, returns int

    MSVQAnim();  // 0x00558a10
    virtual ~MSVQAnim();

    // ---- data members (offsets from ctor 0x00558c20) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: MSAnim at +0x0 (ends +0x4).
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
};
