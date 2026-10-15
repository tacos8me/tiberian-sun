#pragma once
// ==========================================================================
// PKStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class PKStraw
// vtable       : 0x006a3270  (4 slots)
// constructor  : 0x00593101
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class PKStraw : public Straw
{
public:
    // ---- vtable 0x006a3270 (4 slots) ----
    virtual void vfunc_0();  // [0] 0x005933d0
    virtual void vfunc_1();  // [1] 0x00593120
    virtual void vfunc_2();  // [2] 0x00593190
    virtual void vfunc_3();  // [3] 0x005933c0

    PKStraw();  // 0x00593101
    virtual ~PKStraw();

    // ---- data members (offsets from ctor 0x005930b0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3270
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x20];
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned char pad_03c[0x100];
    unsigned long m_13c;  // /* ? */
    unsigned long m_140;  // /* ? */
};
