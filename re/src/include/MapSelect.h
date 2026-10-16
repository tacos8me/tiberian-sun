#pragma once
// ==========================================================================
// MapSelect.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MapSelect
// vtable       : 0x006a1fac  (3 slots)
// constructor  : 0x00540f30
// vtable store : 0x00540f72  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSEngine.h"

class MapSelect : public MSEngine
{
public:
    // ---- vtable 0x006a1fac (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00542050  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00542040  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x00541980  // __thiscall

    MapSelect();  // 0x00540f30
    virtual ~MapSelect();
};
