#pragma once
// ==========================================================================
// bad_cast.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class bad_cast
// vtable       : 0x006a7dd0  (2 slots)
// constructor  : 0x006830da
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
    virtual void vfunc_0(int a0);  // [0] 0x006831cb  // __thiscall
    virtual void vfunc_1();  // [1] 0x006878ac  // __cdecl, returns int

    bad_cast();  // 0x006830da
    virtual ~bad_cast();

    // ---- data members (offsets from ctor 0x006831ec) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7dd0
};
