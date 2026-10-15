#pragma once
// ==========================================================================
// VectorClass_AircraftTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<AircraftTypeClass*>
// vtable       : 0x0069f580  (6 slots)
// constructor  : 0x004a88be
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_AircraftTypeClass
{
public:
    // ---- vtable 0x0069f580 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1ac0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aecf0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aed30  // __thiscall
    virtual void vfunc_3();  // [3] 0x004aede0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aee10  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004aee50  // __stdcall, returns int

    VectorClass_AircraftTypeClass();  // 0x004a88be
    virtual ~VectorClass_AircraftTypeClass();

    // ---- data members (offsets from ctor 0x004b1ab9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f580
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
