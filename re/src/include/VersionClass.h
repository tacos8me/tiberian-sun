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
    unsigned long m_000;  // +0x000 init 0x69f81c; 4-byte store: mov dword ptr [esi], 0x69f81c
};
