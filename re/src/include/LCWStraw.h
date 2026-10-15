#pragma once
// ==========================================================================
// LCWStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LCWStraw
// vtable       : 0x006a1524  (3 slots)
// constructor  : 0x004f4528
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class LCWStraw : public Straw
{
public:
    // ---- vtable 0x006a1524 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004f4700  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f1020  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x004f45a0  // __thiscall

    LCWStraw();  // 0x004f4528
    virtual ~LCWStraw();

    // ---- data members (offsets from ctor 0x004f44e9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1524
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
};
