#pragma once
// ==========================================================================
// RLEBlitter.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitter
// vtable       : 0x0069b7d4  (2 slots)
// constructor  : 0x0046995b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class RLEBlitter
{
public:
    // ---- vtable 0x0069b7d4 (2 slots) ----
    virtual void vfunc_1();  // [1] 0x006829a5  // __cdecl

    RLEBlitter();  // 0x0046995b
    virtual ~RLEBlitter();

    // ---- data members (offsets from ctor 0x0046a32b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7d4
};
