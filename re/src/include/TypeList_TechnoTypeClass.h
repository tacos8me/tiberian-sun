#pragma once
// ==========================================================================
// TypeList_TechnoTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<TechnoTypeClass*>
// vtable       : 0x0069ad90  (6 slots)
// constructor  : 0x00449bd9
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_TechnoTypeClass.h"

class TypeList_TechnoTypeClass : public DynamicVectorClass_TechnoTypeClass
{
public:
    // ---- vtable 0x0069ad90 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0044adc0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0044b210  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044b390  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0044acc0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0044b440  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0044acf0  // __stdcall, returns int

    TypeList_TechnoTypeClass();  // 0x00449bd9
    virtual ~TypeList_TechnoTypeClass();

    // ---- data members (offsets from ctor 0x00449b7e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0x69ad90
    unsigned char pad_018[0x1];
    unsigned long m_019;  // /* ? */
    unsigned char pad_01d[0x7];
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0xa
    unsigned char pad_02c[0x20c];
    unsigned long m_238;  // /* ? */
};
