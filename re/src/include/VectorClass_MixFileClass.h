#pragma once
// ==========================================================================
// VectorClass_MixFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MixFileClass*>
// vtable       : 0x00698840  (6 slots)
// constructor  : 0x00402bde
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MixFileClass
{
public:
    // ---- vtable 0x00698840 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00402ec0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00402c60  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00402ca0  // __thiscall
    virtual void vfunc_3();  // [3] 0x00402d50  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00402d80  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00402dc0  // __stdcall, returns int

    VectorClass_MixFileClass();  // 0x00402bde
    virtual ~VectorClass_MixFileClass();

    // ---- data members (offsets from ctor 0x00402eb6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x698840
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
