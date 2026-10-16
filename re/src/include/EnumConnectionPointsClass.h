#pragma once
// ==========================================================================
// EnumConnectionPointsClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class EnumConnectionPointsClass
// vtable       : 0x0069b8ac  (7 slots)
// constructor  : 0x0046c280
// vtable store : 0x0046c2a9  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class EnumConnectionPointsClass : public IEnumConnectionPoints
{
public:
    // ---- vtable 0x0069b8ac (7 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x0046c1f0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x0046c260  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x0046c320  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0046c3c0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0046c3e0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1);  // [6] 0x0046c3f0  // __stdcall, returns int

    EnumConnectionPointsClass();  // 0x0046c280
    virtual ~EnumConnectionPointsClass();

    // ---- data members (offsets from ctor 0x0046c280) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b8ac
};
