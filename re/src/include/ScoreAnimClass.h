#pragma once
// ==========================================================================
// ScoreAnimClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreAnimClass
// vtable       : 0x006a3efc  (4 slots)
// constructor  : 0x005cde7c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ScoreAnimClass
{
public:
    // ---- vtable 0x006a3efc (4 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005cdf00  // __thiscall
    virtual void vfunc_1();  // [1] 0x005cde90  // __thiscall
    virtual void vfunc_2();  // [2] 0x005cdee0  // __thiscall
    virtual void vfunc_3();  // [3] 0x006829a5  // __cdecl

    ScoreAnimClass();  // 0x005cde7c
    virtual ~ScoreAnimClass();

    // ---- data members (offsets from ctor 0x005cde48) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3efc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned char pad_010[0x4];
    unsigned long m_014;  // zero-initialised
    unsigned long m_018;  // /* ? */
};
