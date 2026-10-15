#pragma once
// ==========================================================================
// DynamicVectorClass_struct_ScoutStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct ScoutStruct>
// vtable       : 0x0069fa7c  (6 slots)
// constructor  : 0x004b3a65
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_ScoutStruct.h"

class DynamicVectorClass_struct_ScoutStruct : public VectorClass_struct_ScoutStruct
{
public:
    // ---- vtable 0x0069fa7c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x004c6810
    virtual void vfunc_1();  // [1] 0x004c5aa0
    virtual void vfunc_2();  // [2] 0x004c5c90
    virtual void vfunc_3();  // [3] 0x004c5290
    virtual void vfunc_4();  // [4] 0x004c5d90
    virtual void vfunc_5();  // [5] 0x004c52c0

    DynamicVectorClass_struct_ScoutStruct();  // 0x004b3a65
    virtual ~DynamicVectorClass_struct_ScoutStruct();

    // ---- data members (offsets from ctor 0x004b3940) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f840
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
