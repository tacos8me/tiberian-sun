#pragma once
// ==========================================================================
// ScoreTimeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreTimeClass
// vtable       : 0x006a3ee4  (4 slots)
// constructor  : 0x005ca088
// vtable store : 0x005ca0db  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScoreAnimClass.h"

class ScoreTimeClass : public ScoreAnimClass
{
public:
    // ---- vtable 0x006a3ee4 (4 slots) ----
    virtual void vfunc_1();  // [1] 0x005cde90  // __thiscall
    virtual void vfunc_2();  // [2] 0x005cdee0  // __thiscall
    virtual void vfunc_3(int a0);  // [3] 0x005cda00  // __thiscall

    ScoreTimeClass();  // 0x005ca088
    virtual ~ScoreTimeClass();

    // ---- data members (offsets from ctor 0x005ca01c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3ee4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned char pad_010[0x4];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // zero-initialised
    unsigned long m_020;  // init 0x3c
    unsigned long m_024;  // init 0x3
    unsigned long m_028;  // /* ? */
};
