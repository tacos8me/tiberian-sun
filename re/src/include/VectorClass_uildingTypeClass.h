#pragma once
// ==========================================================================
// VectorClass_uildingTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<uildingTypeClass*>
// vtable       : 0x0069fb24  (6 slots)
// constructor  : 0x004bab50
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_uildingTypeClass
{
public:
    // ---- vtable 0x0069fb24 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004c5510  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004c5550  // __thiscall
    virtual void vfunc_3();  // [3] 0x004c5600  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004c5630  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004c5670  // __stdcall, returns int

    VectorClass_uildingTypeClass();  // 0x004bab50
    virtual ~VectorClass_uildingTypeClass();

    // ---- data members (offsets from ctor 0x004c193d) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x44];
    unsigned long m_044;  // init 0x69fb24
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned char pad_050[0x1];
    unsigned long m_051;  // zero-initialised
    unsigned char pad_055[0x7b];
    unsigned long m_0d0;  // /* ? */
};
