#pragma once
// ==========================================================================
// VectorClass_FactoryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<FactoryClass*>
// vtable       : 0x0069f318  (6 slots)
// constructor  : 0x004a833e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_FactoryClass
{
public:
    // ---- vtable 0x0069f318 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b13e0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004ad2d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ad310  // __thiscall
    virtual void vfunc_3();  // [3] 0x004ad3c0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ad3f0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004ad430  // __stdcall, returns int

    VectorClass_FactoryClass();  // 0x004a833e
    virtual ~VectorClass_FactoryClass();

    // ---- data members (offsets from ctor 0x004b13d9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f318
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
