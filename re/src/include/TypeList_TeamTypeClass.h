#pragma once
// ==========================================================================
// TypeList_TeamTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<TeamTypeClass*>
// vtable       : 0x0069faec  (6 slots)
// constructor  : 0x004b633d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_TeamTypeClass.h"

class TypeList_TeamTypeClass : public DynamicVectorClass_TeamTypeClass
{
public:
    // ---- vtable 0x0069faec (6 slots) ----
    virtual void vfunc_0();  // [0] 0x004c53e0
    virtual void vfunc_1();  // [1] 0x004c6030
    virtual void vfunc_2();  // [2] 0x004c61b0
    virtual void vfunc_3();  // [3] 0x004c5330
    virtual void vfunc_4();  // [4] 0x004c6260
    virtual void vfunc_5();  // [5] 0x004c5360

    TypeList_TeamTypeClass();  // 0x004b633d
    virtual ~TypeList_TeamTypeClass();

    // ---- data members (offsets from ctor 0x004b6297) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x8];
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // init 0x69faec
    unsigned char pad_024[0x18];
    unsigned long m_03c;  // /* ? */
};
