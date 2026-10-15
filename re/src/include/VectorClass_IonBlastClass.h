#pragma once
// ==========================================================================
// VectorClass_IonBlastClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<IonBlastClass*>
// vtable       : 0x006a0a80  (6 slots)
// constructor  : 0x004e5724
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_IonBlastClass
{
public:
    // ---- vtable 0x006a0a80 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004e6960  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004e6700  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004e6740  // __thiscall
    virtual void vfunc_3();  // [3] 0x004e67f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004e6820  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004e6860  // __stdcall, returns int

    VectorClass_IonBlastClass();  // 0x004e5724
    virtual ~VectorClass_IonBlastClass();

    // ---- data members (offsets from ctor 0x004e6956) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a0a80
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
