#pragma once
// ==========================================================================
// VectorClass_TaskForceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<TaskForceClass*>
// vtable       : 0x0069f3f8  (6 slots)
// constructor  : 0x004a853e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_TaskForceClass
{
public:
    // ---- vtable 0x0069f3f8 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1660  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004adc50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004adc90  // __thiscall
    virtual void vfunc_3();  // [3] 0x004add40  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004add70  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004addb0  // __stdcall, returns int

    VectorClass_TaskForceClass();  // 0x004a853e
    virtual ~VectorClass_TaskForceClass();

    // ---- data members (offsets from ctor 0x004b1659) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f3f8
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
