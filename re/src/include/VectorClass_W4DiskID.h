#pragma once
// ==========================================================================
// VectorClass_W4DiskID.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<W4DiskID>
// vtable       : 0x006a4298  (6 slots)
// constructor  : 0x005d545b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_W4DiskID
{
public:
    // ---- vtable 0x006a4298 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x005d7190
    virtual void vfunc_1();  // [1] 0x005d6500
    virtual void vfunc_2();  // [2] 0x005d6540
    virtual void vfunc_3();  // [3] 0x005d65f0
    virtual void vfunc_4();  // [4] 0x005d6620
    virtual void vfunc_5();  // [5] 0x005d6660

    VectorClass_W4DiskID();  // 0x005d545b
    virtual ~VectorClass_W4DiskID();

    // ---- data members (offsets from ctor 0x005d5430) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x15c];
    unsigned long m_15c;  // init 0x6a4298
    unsigned long m_160;  // /* ? */
    unsigned long m_164;  // /* ? */
    unsigned char pad_168[0x1];
    unsigned long m_169;  // /* ? */
};
