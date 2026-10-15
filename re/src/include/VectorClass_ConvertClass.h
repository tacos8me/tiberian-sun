#pragma once
// ==========================================================================
// VectorClass_ConvertClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ConvertClass*>
// vtable       : 0x0069b1ec  (6 slots)
// constructor  : 0x004606f4
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ConvertClass
{
public:
    // ---- vtable 0x0069b1ec (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0046aa50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0046aa90  // __thiscall
    virtual void vfunc_3();  // [3] 0x0046ab40  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0046ab70  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0046abb0  // __stdcall, returns int

    VectorClass_ConvertClass();  // 0x004606f4
    virtual ~VectorClass_ConvertClass();

    // ---- data members (offsets from ctor 0x0046aca6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b1ec
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
