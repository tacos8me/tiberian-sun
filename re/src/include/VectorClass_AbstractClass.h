#pragma once
// ==========================================================================
// VectorClass_AbstractClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<AbstractClass*>
// vtable       : 0x0069aa3c  (6 slots)
// constructor  : 0x00444e10
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_AbstractClass
{
public:
    // ---- vtable 0x0069aa3c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00445570  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00445310  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00445350  // __thiscall
    virtual void vfunc_3();  // [3] 0x00445400  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00445430  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00445470  // __stdcall, returns int

    VectorClass_AbstractClass();  // 0x00444e10
    virtual ~VectorClass_AbstractClass();

    // ---- data members (offsets from ctor 0x00444de0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x2c];
    unsigned long m_02c;  // init 0x69aa3c
    unsigned char pad_030[0x14];
    unsigned long m_044;  // /* ? */
};
