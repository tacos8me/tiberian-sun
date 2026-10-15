#pragma once
// ==========================================================================
// VectorClass_EgoClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<EgoClass*>
// vtable       : 0x0069de80  (6 slots)
// constructor  : 0x0048d6be
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_EgoClass
{
public:
    // ---- vtable 0x0069de80 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0048e4d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0048e510  // __thiscall
    virtual void vfunc_3();  // [3] 0x0048e5c0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0048e5f0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0048e630  // __stdcall, returns int

    VectorClass_EgoClass();  // 0x0048d6be
    virtual ~VectorClass_EgoClass();

    // ---- data members (offsets from ctor 0x0048e726) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69de80
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
