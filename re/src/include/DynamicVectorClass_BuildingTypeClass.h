#pragma once
// ==========================================================================
// DynamicVectorClass_BuildingTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<BuildingTypeClass*>
// vtable       : 0x0069f52c  (6 slots)
// constructor  : 0x004a8813
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_BuildingTypeClass.h"

class DynamicVectorClass_BuildingTypeClass : public VectorClass_BuildingTypeClass
{
public:
    // ---- vtable 0x0069f52c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004aea90  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aec10  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9e10  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aecc0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9e40  // __stdcall, returns int

    DynamicVectorClass_BuildingTypeClass();  // 0x004a8813
    virtual ~DynamicVectorClass_BuildingTypeClass();

    // ---- data members (offsets from ctor 0x004c19ed) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // init 0x69f52c
    unsigned char pad_014[0xc];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // init 0xa
};
