#pragma once
// ==========================================================================
// VectorClass_MapSelection.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MapSelection*>
// vtable       : 0x006a2650  (6 slots)
// constructor  : 0x0055a894
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MapSelection
{
public:
    // ---- vtable 0x006a2650 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x0055be90
    virtual void vfunc_1();  // [1] 0x0055b3d0
    virtual void vfunc_2();  // [2] 0x0055b410
    virtual void vfunc_3();  // [3] 0x0055b4c0
    virtual void vfunc_4();  // [4] 0x0055b4f0
    virtual void vfunc_5();  // [5] 0x0055b530

    VectorClass_MapSelection();  // 0x0055a894
    virtual ~VectorClass_MapSelection();

    // ---- data members (offsets from ctor 0x0055a7a0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x24];
    unsigned long m_024;  // init 0x6a2618
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned char pad_030[0x1];
    unsigned long m_031;  // /* ? */
    unsigned char pad_035[0x7];
    unsigned long m_03c;  // init 0x6a2634
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned char pad_048[0x1];
    unsigned long m_049;  // /* ? */
    unsigned char pad_04d[0x7];
    unsigned long m_054;  // init 0x6a2650
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned char pad_060[0x1];
    unsigned long m_061;  // /* ? */
};
