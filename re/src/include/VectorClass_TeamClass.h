#pragma once
// ==========================================================================
// VectorClass_TeamClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<TeamClass*>
// vtable       : 0x0069f2e0  (6 slots)
// constructor  : 0x004a82be
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_TeamClass
{
public:
    // ---- vtable 0x0069f2e0 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b0a30  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aadb0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aadf0  // __thiscall
    virtual void vfunc_3();  // [3] 0x004aaea0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aaed0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004aaf10  // __stdcall, returns int

    VectorClass_TeamClass();  // 0x004a82be
    virtual ~VectorClass_TeamClass();

    // ---- data members (offsets from ctor 0x004b0a29) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f2e0
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
