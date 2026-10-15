#pragma once
// ==========================================================================
// VectorClass_ParticleSystemTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ParticleSystemTypeClass*>
// vtable       : 0x0069f740  (6 slots)
// constructor  : 0x004a8cbe
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ParticleSystemTypeClass
{
public:
    // ---- vtable 0x0069f740 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1fc0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004afff0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004b0030  // __thiscall
    virtual void vfunc_3();  // [3] 0x004b00e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004b0110  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004b0150  // __stdcall, returns int

    VectorClass_ParticleSystemTypeClass();  // 0x004a8cbe
    virtual ~VectorClass_ParticleSystemTypeClass();

    // ---- data members (offsets from ctor 0x004b1fb9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f740
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
