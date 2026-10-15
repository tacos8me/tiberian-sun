#pragma once
// ==========================================================================
// VectorClass_NodeNameType.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<NodeNameType*>
// vtable       : 0x006a4244  (6 slots)
// constructor  : 0x005d3bf7
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_NodeNameType
{
public:
    // ---- vtable 0x006a4244 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005d7370  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005d6cd0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005d6d10  // __thiscall
    virtual void vfunc_3();  // [3] 0x005d6dc0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005d6df0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005d6e30  // __stdcall, returns int

    VectorClass_NodeNameType();  // 0x005d3bf7
    virtual ~VectorClass_NodeNameType();

    // ---- data members (offsets from ctor 0x005d7369) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4244
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
