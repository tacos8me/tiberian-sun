#pragma once
// ==========================================================================
// VectorClass_ObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ObjectClass*>
// vtable       : 0x00698730  (6 slots)
// constructor  : 0x0040244e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ObjectClass
{
public:
    // ---- vtable 0x00698730 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004027a0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00402540  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00402580  // __thiscall
    virtual void vfunc_3();  // [3] 0x00402630  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00402660  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004026a0  // __stdcall, returns int

    VectorClass_ObjectClass();  // 0x0040244e
    virtual ~VectorClass_ObjectClass();

    // ---- data members (offsets from ctor 0x00402491) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x698730
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
