#pragma once
// ==========================================================================
// VectorClass_I.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<I>
// vtable       : 0x00699d38  (6 slots)
// constructor  : 0x0041ac29
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_I
{
public:
    // ---- vtable 0x00699d38 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0041dcc0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0041d7b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0041d7f0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0041d8a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0041d8d0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0041d910  // __stdcall, returns int

    VectorClass_I();  // 0x0041ac29
    virtual ~VectorClass_I();

    // ---- data members (offsets from ctor 0x0041ab06) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // /* ? */
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
