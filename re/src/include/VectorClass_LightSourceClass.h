#pragma once
// ==========================================================================
// VectorClass_LightSourceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<LightSourceClass*>
// vtable       : 0x006a1558  (6 slots)
// constructor  : 0x004f4bde
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_LightSourceClass
{
public:
    // ---- vtable 0x006a1558 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004f5be0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004f5720  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f5760  // __thiscall
    virtual void vfunc_3();  // [3] 0x004f5810  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f5840  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004f5880  // __stdcall, returns int

    VectorClass_LightSourceClass();  // 0x004f4bde
    virtual ~VectorClass_LightSourceClass();

    // ---- data members (offsets from ctor 0x004f5bd6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1558
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
