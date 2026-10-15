#pragma once
// ==========================================================================
// SHAStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SHAStraw
// vtable       : 0x006a2320  (3 slots)
// constructor  : 0x0054788b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class SHAStraw : public Straw
{
public:
    // ---- vtable 0x006a2320 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00547ce0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f1020  // __thiscall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005d7be0  // __thiscall

    SHAStraw();  // 0x0054788b
    virtual ~SHAStraw();
};
