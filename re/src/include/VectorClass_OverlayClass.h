#pragma once
// ==========================================================================
// VectorClass_OverlayClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<OverlayClass*>
// vtable       : 0x0069f0e8  (6 slots)
// constructor  : 0x004a7dbe
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_OverlayClass
{
public:
    // ---- vtable 0x0069f0e8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004abfd0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ac010  // __thiscall
    virtual void vfunc_3();  // [3] 0x004ac0c0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ac0f0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004ac130  // __stdcall, returns int

    VectorClass_OverlayClass();  // 0x004a7dbe
    virtual ~VectorClass_OverlayClass();

    // ---- data members (offsets from ctor 0x004b0ed9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f0e8
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
