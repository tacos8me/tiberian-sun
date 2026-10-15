#pragma once
// ==========================================================================
// VectorClass_UnitTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<UnitTypeClass*>
// vtable       : 0x0069f5b8  (6 slots)
// constructor  : 0x004a893e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_UnitTypeClass
{
public:
    // ---- vtable 0x0069f5b8 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1b60  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aef50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aef90  // __thiscall
    virtual void vfunc_3();  // [3] 0x004af040  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004af070  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004af0b0  // __stdcall, returns int

    VectorClass_UnitTypeClass();  // 0x004a893e
    virtual ~VectorClass_UnitTypeClass();

    // ---- data members (offsets from ctor 0x004b1b59) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f5b8
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
