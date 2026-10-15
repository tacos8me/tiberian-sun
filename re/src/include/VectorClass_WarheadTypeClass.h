#pragma once
// ==========================================================================
// VectorClass_WarheadTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<WarheadTypeClass*>
// vtable       : 0x006989e0  (6 slots)
// constructor  : 0x004049fe
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_WarheadTypeClass
{
public:
    // ---- vtable 0x006989e0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00404b50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00404b90  // __thiscall
    virtual void vfunc_3();  // [3] 0x00404c40  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00404c70  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00404cb0  // __stdcall, returns int

    VectorClass_WarheadTypeClass();  // 0x004049fe
    virtual ~VectorClass_WarheadTypeClass();

    // ---- data members (offsets from ctor 0x00404da6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6989e0
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
