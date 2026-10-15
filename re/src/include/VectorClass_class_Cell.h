#pragma once
// ==========================================================================
// VectorClass_class_Cell.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<class Cell>
// vtable       : 0x00699e00  (6 slots)
// constructor  : 0x0041f61e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_class_Cell
{
public:
    // ---- vtable 0x00699e00 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0041f5b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0041f680  // __thiscall
    virtual void vfunc_3();  // [3] 0x0041f750  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0041f780  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0041f7d0  // __stdcall, returns int

    VectorClass_class_Cell();  // 0x0041f61e
    virtual ~VectorClass_class_Cell();

    // ---- data members (offsets from ctor 0x0041f5ff) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699e00
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
