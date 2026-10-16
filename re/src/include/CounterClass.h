#pragma once
// ==========================================================================
// CounterClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CounterClass
// vtable       : 0x0069b7ec  (6 slots)
// constructor  : 0x0046b283
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass.h"

class CounterClass : public VectorClass<H>
{
public:
    // ---- vtable 0x0069b7ec (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046b570  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0044af30  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044af70  // __thiscall
    virtual void vfunc_3();  // [3] 0x0046b3e0  // __cdecl
    virtual void vfunc_4(int a0);  // [4] 0x0044b050  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0044b090  // __stdcall, returns int

    CounterClass();  // 0x0046b283
    virtual ~CounterClass();

    // ---- data members (offsets from ctor 0x0046b26f) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7ec
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
};
