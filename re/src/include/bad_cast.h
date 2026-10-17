#pragma once
// ==========================================================================
// bad_cast.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class bad_cast
// vtable       : 0x006a7dd0  (2 slots)
// constructor  : 0x006831b3
// vtable store : 0x006831c1  (inside the ctor)
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
    virtual void vfunc_0(int a0);  // [0] 0x006831cb  // __thiscall, returns ?
    virtual void vfunc_1();  // [1] 0x006878ac  // __thiscall, returns void*

    bad_cast();  // 0x006831b3
    virtual ~bad_cast();

    // ---- data members (offsets from ctor 0x006831b3) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0xc: the compiled end of the
    // base sub-object region. Evidence: exception at +0x0 (ends +0xc).
    // 1 recovered offset(s) below +0xc lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
};
