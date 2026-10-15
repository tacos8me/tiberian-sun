#pragma once
// ==========================================================================
// bad_cast.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class bad_cast
// vtable       : 0x006a7dd0  (2 slots)
// constructor  : 0x006831c1
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "exception.h"

class bad_cast : public exception
{
public:
    // ---- vtable 0x006a7dd0 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x006831cb
    virtual void vfunc_1();  // [1] 0x006878ac

    bad_cast();  // 0x006831c1
    virtual ~bad_cast();
};
