#pragma once
// ==========================================================================
// DynamicVectorClass_BuildingClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<BuildingClass*>
// vtable       : 0x0069a27c  (6 slots)
// constructor  : 0x004254d3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_BuildingClass.h"

class DynamicVectorClass_BuildingClass : public VectorClass_BuildingClass
{
public:
    // ---- vtable 0x0069a27c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0043a3c0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0043a540  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0043a250  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0043a5f0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0043a280  // __stdcall, returns int

    DynamicVectorClass_BuildingClass();  // 0x004254d3
    virtual ~DynamicVectorClass_BuildingClass();

    // ---- data members (offsets from ctor 0x004b3938) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f840
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
