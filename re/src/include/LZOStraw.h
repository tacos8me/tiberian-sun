#pragma once
// ==========================================================================
// LZOStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LZOStraw
// vtable       : 0x006a189c  (3 slots)
// constructor  : 0x004fb97c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class LZOStraw : public Straw
{
public:
    // ---- vtable 0x006a189c (3 slots) ----
    virtual void vfunc_0();  // [0] 0x004fbba0
    virtual void vfunc_1();  // [1] 0x005f1020
    virtual void vfunc_2();  // [2] 0x004fb9f0

    LZOStraw();  // 0x004fb97c
    virtual ~LZOStraw();

    // ---- data members (offsets from ctor 0x004fb950) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a189c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
};
