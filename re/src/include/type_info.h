#pragma once
// ==========================================================================
// type_info.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class type_info
// vtable       : 0x006a7d04  (1 slots)
// constructor  : 0x006828dd
// vtable store : 0x006828e4  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class type_info
{
public:
    // ---- vtable 0x006a7d04 (1 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00682906  // __thiscall

    type_info();  // 0x006828dd
    virtual ~type_info();

    // ---- data members (offsets from ctor 0x006828dd) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7d04
};
