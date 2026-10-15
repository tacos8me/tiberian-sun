#pragma once
// ==========================================================================
// VectorClass_AITriggerTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<AITriggerTypeClass*>
// vtable       : 0x0069ef28  (6 slots)
// constructor  : 0x004a79be
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_AITriggerTypeClass
{
public:
    // ---- vtable 0x0069ef28 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b0850  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aa690  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aa6d0  // __thiscall
    virtual void vfunc_3();  // [3] 0x004aa780  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aa7b0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004aa7f0  // __stdcall, returns int

    VectorClass_AITriggerTypeClass();  // 0x004a79be
    virtual ~VectorClass_AITriggerTypeClass();

    // ---- data members (offsets from ctor 0x004b0849) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69ef28
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
