#pragma once
// ==========================================================================
// VectorClass_AlphaShapeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<AlphaShapeClass*>
// vtable       : 0x006997b4  (6 slots)
// constructor  : 0x004119be
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_AlphaShapeClass
{
public:
    // ---- vtable 0x006997b4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004128b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004128f0  // __thiscall
    virtual void vfunc_3();  // [3] 0x004129a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004129d0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00412a10  // __stdcall, returns int

    VectorClass_AlphaShapeClass();  // 0x004119be
    virtual ~VectorClass_AlphaShapeClass();

    // ---- data members (offsets from ctor 0x00412b06) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6997b4
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
