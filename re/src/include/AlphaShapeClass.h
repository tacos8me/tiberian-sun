#pragma once
// ==========================================================================
// AlphaShapeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AlphaShapeClass
// vtable       : 0x006997d0  (6 slots)
// constructor  : 0x00411a15
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class AlphaShapeClass : public AbstractClass
{
public:
    // ---- vtable 0x006997d0 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    AlphaShapeClass();  // 0x00411a15
    virtual ~AlphaShapeClass();

    // ---- data members (offsets from ctor 0x004119f0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6997ec
    unsigned long m_004;  // init 0x6997d0
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0xc];
    unsigned long m_028;  // zero-initialised
    unsigned long m_02c;  // zero-initialised
};
