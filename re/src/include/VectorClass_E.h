#pragma once
// ==========================================================================
// VectorClass_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<E>
// vtable       : 0x006a6df4  (6 slots)
// constructor  : 0x0064292f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_E
{
public:
    // ---- vtable 0x006a6df4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00642d40  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00642d80  // __thiscall
    virtual void vfunc_3();  // [3] 0x00642e30  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00642e60  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00642ea0  // __stdcall, returns int

    VectorClass_E();  // 0x0064292f
    virtual ~VectorClass_E();

    // ---- data members (offsets from ctor 0x00642944) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a6df4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
};
