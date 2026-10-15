#pragma once
// ==========================================================================
// CD.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CD
// vtable       : 0x0069ac10  (3 slots)
// constructor  : 0x00447441
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class CD : public DiskSwap
{
public:
    // ---- vtable 0x0069ac10 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0044c220  // __stdcall
    virtual void vfunc_1(int a0, int a1);  // [1] 0x0044c240  // __thiscall
    virtual void vfunc_2(int a0);  // [2] 0x0044c3b0  // __thiscall

    CD();  // 0x00447441
    virtual ~CD();

    // ---- data members (offsets from ctor 0x00447425) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x4];
    unsigned long m_004;  // init 0x69ac10
};
