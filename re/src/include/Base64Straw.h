#pragma once
// ==========================================================================
// Base64Straw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Base64Straw
// vtable       : 0x006a0430  (3 slots)
// constructor  : 0x004d599c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class Base64Straw : public Straw
{
public:
    // ---- vtable 0x006a0430 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x004d8020
    virtual void vfunc_1();  // [1] 0x005f1020
    virtual void vfunc_2();  // [2] 0x0041e140

    Base64Straw();  // 0x004d599c
    virtual ~Base64Straw();
};
