#pragma once
// ==========================================================================
// VectorClass_VocClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<VocClass*>
// vtable       : 0x006a7068  (6 slots)
// constructor  : 0x00646d1e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_VocClass
{
public:
    // ---- vtable 0x006a7068 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x006479d0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00647770  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x006477b0  // __thiscall
    virtual void vfunc_3();  // [3] 0x00647860  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00647890  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x006478d0  // __stdcall, returns int

    VectorClass_VocClass();  // 0x00646d1e
    virtual ~VectorClass_VocClass();

    // ---- data members (offsets from ctor 0x006479c6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7068
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
