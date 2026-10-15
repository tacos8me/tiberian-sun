#pragma once
// ==========================================================================
// DynamicVectorClass_class_RGBClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class RGBClass>
// vtable       : 0x006a3514  (6 slots)
// constructor  : 0x0059886e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_RGBClass.h"

class DynamicVectorClass_class_RGBClass : public VectorClass_class_RGBClass
{
public:
    // ---- vtable 0x006a3514 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00598890
    virtual void vfunc_1();  // [1] 0x0044b530
    virtual void vfunc_2();  // [2] 0x0044b740
    virtual void vfunc_3();  // [3] 0x0044ad10
    virtual void vfunc_4();  // [4] 0x0044b840
    virtual void vfunc_5();  // [5] 0x0044ad40

    DynamicVectorClass_class_RGBClass();  // 0x0059886e
    virtual ~DynamicVectorClass_class_RGBClass();

    // ---- data members (offsets from ctor 0x0059886c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3514
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
