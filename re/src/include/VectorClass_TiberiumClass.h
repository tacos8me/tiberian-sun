#pragma once
// ==========================================================================
// VectorClass_TiberiumClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<TiberiumClass*>
// vtable       : 0x006a6210  (6 slots)
// constructor  : 0x006281ae
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_TiberiumClass
{
public:
    // ---- vtable 0x006a6210 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0062a170  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0062a1b0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0062a260  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0062a290  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0062a2d0  // __stdcall, returns int

    VectorClass_TiberiumClass();  // 0x006281ae
    virtual ~VectorClass_TiberiumClass();

    // ---- data members (offsets from ctor 0x0062a3c6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a6210
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
