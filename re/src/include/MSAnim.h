#pragma once
// ==========================================================================
// MSAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSAnim
// vtable       : 0x006a2470  (6 slots)
// constructor  : 0x0055771b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class MSAnim
{
public:
    // ---- vtable 0x006a2470 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00557730
    virtual void vfunc_1();  // [1] 0x00557690
    virtual void vfunc_2();  // [2] 0x005576a0
    virtual void vfunc_3();  // [3] 0x005576f0
    virtual void vfunc_4();  // [4] 0x006829a5
    virtual void vfunc_5();  // [5] 0x006829a5

    MSAnim();  // 0x0055771b
    virtual ~MSAnim();

    // ---- data members (offsets from ctor 0x00557710) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2470
};
