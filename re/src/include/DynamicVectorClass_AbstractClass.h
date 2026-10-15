#pragma once
// ==========================================================================
// DynamicVectorClass_AbstractClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<AbstractClass*>
// vtable       : 0x0069aa58  (6 slots)
// constructor  : 0x004449e8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_AbstractClass.h"

class DynamicVectorClass_AbstractClass : public VectorClass_AbstractClass
{
public:
    // ---- vtable 0x0069aa58 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x004455c0
    virtual void vfunc_1();  // [1] 0x00445310
    virtual void vfunc_2();  // [2] 0x00445490
    virtual void vfunc_3();  // [3] 0x004452a0
    virtual void vfunc_4();  // [4] 0x00445540
    virtual void vfunc_5();  // [5] 0x004452d0

    DynamicVectorClass_AbstractClass();  // 0x004449e8
    virtual ~DynamicVectorClass_AbstractClass();

    // ---- data members (offsets from ctor 0x00444828) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0x4];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // init 0x69aa58
    unsigned char pad_030[0xc];
    unsigned long m_03c;  // zero-initialised
    unsigned long m_040;  // init 0xa
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0xc];
    unsigned long m_058;  // /* ? */
    unsigned char pad_05c[0xc];
    unsigned long m_068;  // /* ? */
    unsigned char pad_06c[0x28];
    unsigned long m_094;  // /* ? */
    unsigned long m_098;  // /* ? */
    unsigned char pad_09c[0x8];
    unsigned long m_0a4;  // /* ? */
    unsigned long m_0a8;  // /* ? */
    unsigned long m_0ac;  // /* ? */
    unsigned long m_0b0;  // /* ? */
};
