#pragma once
// ==========================================================================
// VectorClass_AbstractClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<AbstractClass*>
// vtable       : 0x0069aa3c  (6 slots)
// constructor  : 0x00444e10
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_AbstractClass
{
public:
    // ---- vtable 0x0069aa3c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00445570
    virtual void vfunc_1();  // [1] 0x00445310
    virtual void vfunc_2();  // [2] 0x00445350
    virtual void vfunc_3();  // [3] 0x00445400
    virtual void vfunc_4();  // [4] 0x00445430
    virtual void vfunc_5();  // [5] 0x00445470

    VectorClass_AbstractClass();  // 0x00444e10
    virtual ~VectorClass_AbstractClass();

    // ---- data members (offsets from ctor 0x00444c0b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // zero-initialised
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x18];
    unsigned long m_02c;  // init 0x69aa3c
    unsigned char pad_030[0x14];
    unsigned long m_044;  // /* ? */
    unsigned char pad_048[0x10];
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
