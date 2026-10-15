#pragma once
// ==========================================================================
// VectorClass_struct_tagCONNECTDATA.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct tagCONNECTDATA>
// vtable       : 0x0069b814  (6 slots)
// constructor  : 0x0046b93b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_tagCONNECTDATA
{
public:
    // ---- vtable 0x0069b814 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x0046cd40
    virtual void vfunc_1();  // [1] 0x0046c820
    virtual void vfunc_2();  // [2] 0x0046c880
    virtual void vfunc_3();  // [3] 0x0046c940
    virtual void vfunc_4();  // [4] 0x0046c970
    virtual void vfunc_5();  // [5] 0x0046c9c0

    VectorClass_struct_tagCONNECTDATA();  // 0x0046b93b
    virtual ~VectorClass_struct_tagCONNECTDATA();

    // ---- data members (offsets from ctor 0x0046b8e0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b814
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
