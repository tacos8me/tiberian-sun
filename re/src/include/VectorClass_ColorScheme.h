#pragma once
// ==========================================================================
// VectorClass_ColorScheme.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ColorScheme*>
// vtable       : 0x006a3e88  (6 slots)
// constructor  : 0x005c8bde
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ColorScheme
{
public:
    // ---- vtable 0x006a3e88 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005c9630  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005c93d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005c9410  // __thiscall
    virtual void vfunc_3();  // [3] 0x005c94c0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005c94f0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005c9530  // __stdcall, returns int

    VectorClass_ColorScheme();  // 0x005c8bde
    virtual ~VectorClass_ColorScheme();

    // ---- data members (offsets from ctor 0x005c9626) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3e88
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
