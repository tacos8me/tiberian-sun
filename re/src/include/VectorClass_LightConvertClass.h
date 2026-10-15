#pragma once
// ==========================================================================
// VectorClass_LightConvertClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<LightConvertClass*>
// vtable       : 0x006986a4  (6 slots)
// constructor  : 0x00401a5e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_LightConvertClass
{
public:
    // ---- vtable 0x006986a4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00401ae0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00401b20  // __thiscall
    virtual void vfunc_3();  // [3] 0x00401bd0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00401c00  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00401c40  // __stdcall, returns int

    VectorClass_LightConvertClass();  // 0x00401a5e
    virtual ~VectorClass_LightConvertClass();

    // ---- data members (offsets from ctor 0x00401d36) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6986a4
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
