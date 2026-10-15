#pragma once
// ==========================================================================
// VectorClass_TagTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<TagTypeClass*>
// vtable       : 0x006a547c  (6 slots)
// constructor  : 0x00603eae
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_TagTypeClass
{
public:
    // ---- vtable 0x006a547c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00604a40  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x006047e0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00604820  // __thiscall
    virtual void vfunc_3();  // [3] 0x006048d0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00604900  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00604940  // __stdcall, returns int

    VectorClass_TagTypeClass();  // 0x00603eae
    virtual ~VectorClass_TagTypeClass();

    // ---- data members (offsets from ctor 0x00604a36) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a547c
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
