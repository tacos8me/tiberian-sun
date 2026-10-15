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
    virtual void vfunc_1(int a0);  // [1] 0x0044af30  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044b0b0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0044ac70  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0044b160  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0044aca0  // __stdcall, returns int

    TypeList_H();  // 0x00449458
    virtual ~TypeList_H();

    // ---- data members (offsets from ctor 0x004493f7) ----
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
