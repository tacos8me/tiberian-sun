#pragma once
// ==========================================================================
// VectorClass_TechnoClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<TechnoClass*>
// vtable       : 0x0069a668  (6 slots)
// constructor  : 0x0042fc06
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_TechnoClass
{
public:
    // ---- vtable 0x0069a668 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0043a620  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0043a660  // __thiscall
    virtual void vfunc_3();  // [3] 0x0043a710  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0043a740  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0043a780  // __stdcall, returns int

    VectorClass_TechnoClass();  // 0x0042fc06
    virtual ~VectorClass_TechnoClass();

    // ---- data members (offsets from ctor 0x0042fb9a) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x28];
    unsigned long m_028;  // init 0x69a668
};
