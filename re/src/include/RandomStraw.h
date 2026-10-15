#pragma once
// ==========================================================================
// RandomStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RandomStraw
// vtable       : 0x006a3b78  (3 slots)
// constructor  : 0x005ab413
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class RandomStraw : public Straw
{
public:
    // ---- vtable 0x006a3b78 (3 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005f1020  // __thiscall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005ab930  // __thiscall

    RandomStraw();  // 0x005ab413
    virtual ~RandomStraw();

    // ---- data members (offsets from ctor 0x005ab3c5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69bbdc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
};
