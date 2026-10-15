#pragma once
// ==========================================================================
// ScoreFontClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreFontClass
// vtable       : 0x006a3f18  (5 slots)
// constructor  : 0x005cd263
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ScoreFontClass
{
public:
    // ---- vtable 0x006a3f18 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005cdf60  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005cd4d0  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005cd4a0  // __thiscall
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x005cd6e0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4);  // [4] 0x005cd590  // __thiscall

    ScoreFontClass();  // 0x005cd263
    virtual ~ScoreFontClass();

    // ---- data members (offsets from ctor 0x005cd24c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3f18
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned char pad_010[0x4];
    unsigned long m_014;  // /* ? */
};
