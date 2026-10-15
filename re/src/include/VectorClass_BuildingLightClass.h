#pragma once
// ==========================================================================
// VectorClass_BuildingLightClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<BuildingLightClass*>
// vtable       : 0x0069efd0  (6 slots)
// constructor  : 0x004a7b3e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_BuildingLightClass
{
public:
    // ---- vtable 0x0069efd0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004aa8f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aa930  // __thiscall
    virtual void vfunc_3();  // [3] 0x004aa9e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aaa10  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004aaa50  // __stdcall, returns int

    VectorClass_BuildingLightClass();  // 0x004a7b3e
    virtual ~VectorClass_BuildingLightClass();

    // ---- data members (offsets from ctor 0x004b08e9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69efd0
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
