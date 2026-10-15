#pragma once
// ==========================================================================
// MSFont.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSFont
// vtable       : 0x006a2708  (5 slots)
// constructor  : 0x0055d5a5
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class MSFont
{
public:
    // ---- vtable 0x006a2708 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0055de20  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0055da70  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x0055da20  // __thiscall
    virtual void vfunc_3(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [3] 0x0055dcb0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4);  // [4] 0x0055db40  // __thiscall

    MSFont();  // 0x0055d5a5
    virtual ~MSFont();

    // ---- data members (offsets from ctor 0x0055d587) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2708
    unsigned char pad_004[0x8];
    unsigned long m_00c;  // /* ? */
};
