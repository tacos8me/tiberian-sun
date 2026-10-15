#pragma once
// ==========================================================================
// VectorClass_W4PassabilityType.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<W4PassabilityType>
// vtable       : 0x006a1b64  (6 slots)
// constructor  : 0x00507f72
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_W4PassabilityType
{
public:
    // ---- vtable 0x006a1b64 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005226c0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005219f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00521a30  // __thiscall
    virtual void vfunc_3();  // [3] 0x00521ae0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00521b10  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00521b50  // __stdcall, returns int

    VectorClass_W4PassabilityType();  // 0x00507f72
    virtual ~VectorClass_W4PassabilityType();

    // ---- data members (offsets from ctor 0x005226b4) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1b64
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
