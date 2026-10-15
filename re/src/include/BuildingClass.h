#pragma once
// ==========================================================================
// BuildingClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BuildingClass
// vtable       : 0x0069a2b4  (6 slots)
// constructor  : 0x00425570
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TechnoClass.h"

class BuildingClass : public TechnoClass
{
public:
    // ---- vtable 0x0069a2b4 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    BuildingClass();  // 0x00425570
    virtual ~BuildingClass();

    // ---- data members (offsets from ctor 0x00425530) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69a2d0
    unsigned long m_004;  // init 0x69a2b4
    unsigned char pad_008[0x2b0];
    unsigned long m_2b8;  // /* ? */
    unsigned long m_2bc;  // /* ? */
    unsigned char pad_2c0[0x4];
    unsigned long m_2c4;  // /* ? */
    unsigned long m_2c8;  // /* ? */
    unsigned long m_2cc;  // init 0x1
};
