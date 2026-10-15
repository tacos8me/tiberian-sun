#pragma once
// ==========================================================================
// TagTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TagTypeClass
// vtable       : 0x006a5498  (6 slots)
// constructor  : 0x00603f0f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractTypeClass.h"

class TagTypeClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x006a5498 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    TagTypeClass();  // 0x00603f0f
    virtual ~TagTypeClass();

    // ---- data members (offsets from ctor 0x00603ee0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a54b4
    unsigned long m_004;  // init 0x6a5498
    unsigned char pad_008[0x58];
    unsigned long m_060;  // init 0xffffffff
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
};
