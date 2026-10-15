#pragma once
// ==========================================================================
// DynamicVectorClass_class_oxelAnimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class oxelAnimTypeClass*>
// vtable       : 0x006a3d34  (6 slots)
// constructor  : 0x005ba0d5
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_oxelAnimTypeClass.h"

class DynamicVectorClass_class_oxelAnimTypeClass : public VectorClass_class_oxelAnimTypeClass
{
public:
    // ---- vtable 0x006a3d34 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x005bb3e0
    virtual void vfunc_1();  // [1] 0x005b9f10
    virtual void vfunc_2();  // [2] 0x005ba0f0
    virtual void vfunc_3();  // [3] 0x005b9670
    virtual void vfunc_4();  // [4] 0x005ba1f0
    virtual void vfunc_5();  // [5] 0x005b96a0

    DynamicVectorClass_class_oxelAnimTypeClass();  // 0x005ba0d5
    virtual ~DynamicVectorClass_class_oxelAnimTypeClass();

    // ---- data members (offsets from ctor 0x005ba0d3) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3d34
    unsigned char pad_004[0xc];
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
