#pragma once
// ==========================================================================
// TypeList_H.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<H>
// vtable       : 0x0069ad58  (6 slots)
// constructor  : 0x00449458
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_H.h"

class TypeList_H : public DynamicVectorClass_H
{
public:
    // ---- vtable 0x0069ad58 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x0044ad70
    virtual void vfunc_1();  // [1] 0x0044af30
    virtual void vfunc_2();  // [2] 0x0044b0b0
    virtual void vfunc_3();  // [3] 0x0044ac70
    virtual void vfunc_4();  // [4] 0x0044b160
    virtual void vfunc_5();  // [5] 0x0044aca0

    TypeList_H();  // 0x00449458
    virtual ~TypeList_H();

    // ---- data members (offsets from ctor 0x00449400) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x8];
    unsigned long m_008;  // init 0x69ad3c
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // init 0x69ad58
    unsigned char pad_014[0x1];
    unsigned long m_015;  // /* ? */
    unsigned char pad_019[0x7];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // init 0xa
    unsigned char pad_028[0x20c];
    unsigned long m_234;  // init 0x69ad3c
};
