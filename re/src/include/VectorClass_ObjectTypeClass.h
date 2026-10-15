#pragma once
// ==========================================================================
// VectorClass_ObjectTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ObjectTypeClass*>
// vtable       : 0x006a2a58  (6 slots)
// constructor  : 0x005711d4
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ObjectTypeClass
{
public:
    // ---- vtable 0x006a2a58 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00573000  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00572da0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00572de0  // __thiscall
    virtual void vfunc_3();  // [3] 0x00572e90  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00572ec0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00572f00  // __stdcall, returns int

    VectorClass_ObjectTypeClass();  // 0x005711d4
    virtual ~VectorClass_ObjectTypeClass();

    // ---- data members (offsets from ctor 0x00572ff6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2a58
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
