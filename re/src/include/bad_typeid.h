#pragma once
// ==========================================================================
// bad_typeid.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class bad_typeid
// vtable       : 0x006a7d4c  (2 slots)
// constructor  : 0x00682f67
// vtable store : 0x00682f92  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "exception.h"

class bad_typeid : public exception
{
public:
    // ---- vtable 0x006a7d4c (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00682f9c  // __thiscall
    virtual void vfunc_1();  // [1] 0x006878ac  // __cdecl, returns int

    bad_typeid();  // 0x00682f67
    virtual ~bad_typeid();
};
