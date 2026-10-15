#pragma once
// ==========================================================================
// VectorClass_W4PassabilityType.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<W4PassabilityType>
// vtable       : 0x006a1b64  (6 slots)
// constructor  : 0x00507f72
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_W4PassabilityType
{
public:
    // ---- vtable 0x006a1b64 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x005226c0
    virtual void vfunc_1();  // [1] 0x005219f0
    virtual void vfunc_2();  // [2] 0x00521a30
    virtual void vfunc_3();  // [3] 0x00521ae0
    virtual void vfunc_4();  // [4] 0x00521b10
    virtual void vfunc_5();  // [5] 0x00521b50

    VectorClass_W4PassabilityType();  // 0x00507f72
    virtual ~VectorClass_W4PassabilityType();

    // ---- data members (offsets from ctor 0x00507aec) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0x2
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x4];
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned char pad_03c[0x8];
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x4];
    unsigned long m_050;  // init 0x6a1b64
};
