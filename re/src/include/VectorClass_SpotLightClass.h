#pragma once
// ==========================================================================
// VectorClass_SpotLightClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<SpotLightClass*>
// vtable       : 0x006a2e28  (6 slots)
// constructor  : 0x00577b9e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_SpotLightClass
{
public:
    // ---- vtable 0x006a2e28 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00578690  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005786d0  // __thiscall
    virtual void vfunc_3();  // [3] 0x00578780  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005787b0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005787f0  // __stdcall, returns int

    VectorClass_SpotLightClass();  // 0x00577b9e
    virtual ~VectorClass_SpotLightClass();

    // ---- data members (offsets from ctor 0x005788e6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2e28
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
