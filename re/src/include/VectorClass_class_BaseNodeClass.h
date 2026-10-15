#pragma once
// ==========================================================================
// VectorClass_class_BaseNodeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<class BaseNodeClass>
// vtable       : 0x00699e1c  (6 slots)
// constructor  : 0x0041f908
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_class_BaseNodeClass
{
public:
    // ---- vtable 0x00699e1c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x0041fac0
    virtual void vfunc_1();  // [1] 0x0041f430
    virtual void vfunc_2();  // [2] 0x0041f950
    virtual void vfunc_3();  // [3] 0x0041fa20
    virtual void vfunc_4();  // [4] 0x0041fa50
    virtual void vfunc_5();  // [5] 0x0041faa0

    VectorClass_class_BaseNodeClass();  // 0x0041f908
    virtual ~VectorClass_class_BaseNodeClass();

    // ---- data members (offsets from ctor 0x0041f900) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699e1c
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
