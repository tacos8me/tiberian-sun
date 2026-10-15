#pragma once
// ==========================================================================
// DynamicVectorClass_I.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<I>
// vtable       : 0x00699d1c  (6 slots)
// constructor  : 0x0041a94b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_I.h"

class DynamicVectorClass_I : public VectorClass_I
{
public:
    // ---- vtable 0x00699d1c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0041dd10  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0041d7b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0041d930  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0041d710  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0041da30  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0041d740  // __stdcall, returns int

    DynamicVectorClass_I();  // 0x0041a94b
    virtual ~DynamicVectorClass_I();

    // ---- data members (offsets from ctor 0x0041a8e2) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699d1c
    unsigned long m_004;  // init 0x10000
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x4];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
