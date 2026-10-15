#pragma once
// ==========================================================================
// VectorClass_MSSfx.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MSSfx*>
// vtable       : 0x006a26ec  (6 slots)
// constructor  : 0x0055c241
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MSSfx
{
public:
    // ---- vtable 0x006a26ec (6 slots) ----
    virtual void vfunc_0();  // [0] 0x0055d4d0
    virtual void vfunc_1();  // [1] 0x0055d180
    virtual void vfunc_2();  // [2] 0x0055d1c0
    virtual void vfunc_3();  // [3] 0x0055d270
    virtual void vfunc_4();  // [4] 0x0055d2a0
    virtual void vfunc_5();  // [5] 0x0055d2e0

    VectorClass_MSSfx();  // 0x0055c241
    virtual ~VectorClass_MSSfx();

    // ---- data members (offsets from ctor 0x0055c1c0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a26ec
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // /* ? */
};
