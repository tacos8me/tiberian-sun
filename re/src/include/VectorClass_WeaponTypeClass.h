#pragma once
// ==========================================================================
// VectorClass_WeaponTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<WeaponTypeClass*>
// vtable       : 0x00698a20  (6 slots)
// constructor  : 0x004050ce
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_WeaponTypeClass
{
public:
    // ---- vtable 0x00698a20 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004053b0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00405150  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00405190  // __thiscall
    virtual void vfunc_3();  // [3] 0x00405240  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00405270  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004052b0  // __stdcall, returns int

    VectorClass_WeaponTypeClass();  // 0x004050ce
    virtual ~VectorClass_WeaponTypeClass();

    // ---- data members (offsets from ctor 0x004053a6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x698a20
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
