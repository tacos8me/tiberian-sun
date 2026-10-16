#pragma once
// ==========================================================================
// exception.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class exception
// vtable       : 0x006a7f54  (2 slots)
// constructor  : 0x006877c3
// vtable store : 0x006877cf  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class exception
{
public:
    // ---- vtable 0x006a7f54 (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x006877d4  // __thiscall
    virtual void vfunc_1();  // [1] 0x006878ac  // __cdecl, returns int

    exception();  // 0x006877c3
    virtual ~exception();

    // ---- data members (offsets from ctor 0x006877c3) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7f54
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
};
