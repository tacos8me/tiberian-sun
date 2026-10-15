#pragma once
// ==========================================================================
// WaypointPathClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WaypointPathClass
// vtable       : 0x006a74d4  (6 slots)
// constructor  : 0x00655818
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class WaypointPathClass : public AbstractClass
{
public:
    // ---- vtable 0x006a74d4 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    WaypointPathClass();  // 0x00655818
    virtual ~WaypointPathClass();

    // ---- data members (offsets from ctor 0x006557e0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a74f0
    unsigned long m_004;  // init 0x6a74d4
    unsigned char pad_008[0xc];
    unsigned long m_014;  // init 0xffffffff
    unsigned long m_018;  // init 0x6a7554
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // init 0x1
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // init 0xa
};
