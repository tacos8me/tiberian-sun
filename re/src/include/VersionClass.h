#pragma once
// ==========================================================================
// VersionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VersionClass
// vtable       : 0x0069f81c  (1 slots)
// constructor  : 0x004a9150
// vtable store : 0x004a9156  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VersionClass
{
public:
    // ---- vtable 0x0069f81c (1 slots) ----

    VersionClass();  // 0x004a9150
    virtual ~VersionClass();

    // ---- data members (offsets from ctor 0x004a9490) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
};
