#pragma once
// ==========================================================================
// VectorClass_FoggedObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<FoggedObjectClass*>
// vtable       : 0x0069a718  (6 slots)
// constructor  : 0x0043aae8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_FoggedObjectClass
{
public:
    // ---- vtable 0x0069a718 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0043ac70  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0043a880  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0043ab30  // __thiscall
    virtual void vfunc_3();  // [3] 0x0043abe0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0043ac10  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0043ac50  // __stdcall, returns int

    VectorClass_FoggedObjectClass();  // 0x0043aae8
    virtual ~VectorClass_FoggedObjectClass();

    // ---- data members (offsets from ctor 0x0043aad9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69a718
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
