#pragma once
// ==========================================================================
// VectorClass_NeuronClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<NeuronClass*>
// vtable       : 0x0069a244  (6 slots)
// constructor  : 0x00424974
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_NeuronClass
{
public:
    // ---- vtable 0x0069a244 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00424fd0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00424e50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00424e90  // __thiscall
    virtual void vfunc_3();  // [3] 0x00424f40  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00424f70  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00424fb0  // __stdcall, returns int

    VectorClass_NeuronClass();  // 0x00424974
    virtual ~VectorClass_NeuronClass();

    // ---- data members (offsets from ctor 0x00424925) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69a244
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // /* ? */
};
