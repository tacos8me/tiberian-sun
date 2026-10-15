#pragma once
// ==========================================================================
// VectorClass_struct_nitTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct nitTypeClass*>
// vtable       : 0x006a3cd4  (6 slots)
// constructor  : 0x005ad85e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_nitTypeClass
{
public:
    // ---- vtable 0x006a3cd4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005bb570  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005baa40  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005baa80  // __thiscall
    virtual void vfunc_3();  // [3] 0x005bab30  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005bab60  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005baba0  // __stdcall, returns int

    VectorClass_struct_nitTypeClass();  // 0x005ad85e
    virtual ~VectorClass_struct_nitTypeClass();

    // ---- data members (offsets from ctor 0x005ad78c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3d0c
};
