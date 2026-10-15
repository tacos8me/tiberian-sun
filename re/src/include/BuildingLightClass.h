#pragma once
// ==========================================================================
// BuildingLightClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BuildingLightClass
// vtable       : 0x00699f9c  (6 slots)
// constructor  : 0x00420f5a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectClass.h"

class BuildingLightClass : public ObjectClass
{
public:
    // ---- vtable 0x00699f9c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    BuildingLightClass();  // 0x00420f5a
    virtual ~BuildingLightClass();

    // ---- data members (offsets from ctor 0x00420ee0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699fb8
    unsigned long m_004;  // init 0x699f9c
    unsigned char pad_008[0x48];
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
    unsigned long m_06c;  // /* ? */
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // /* ? */
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // /* ? */
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // /* ? */
};
