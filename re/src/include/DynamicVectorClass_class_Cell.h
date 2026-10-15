#pragma once
// ==========================================================================
// DynamicVectorClass_class_Cell.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class Cell>
// vtable       : 0x00699dc8  (6 slots)
// constructor  : 0x0041e8f4
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_Cell.h"

class DynamicVectorClass_class_Cell : public VectorClass_class_Cell
{
public:
    // ---- vtable 0x00699dc8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0041f5b0  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x0041f7f0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0041f3e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0041f8c0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0041f410  // __stdcall, returns int

    DynamicVectorClass_class_Cell();  // 0x0041e8f4
    virtual ~DynamicVectorClass_class_Cell();

    // ---- data members (offsets from ctor 0x0041e8b5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699de4
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
