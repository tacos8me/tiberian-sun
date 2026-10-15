#pragma once
// ==========================================================================
// VectorClass_struct_DirtyAreaStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct DirtyAreaStruct>
// vtable       : 0x006a5228  (6 slots)
// constructor  : 0x005f4a34
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_DirtyAreaStruct
{
public:
    // ---- vtable 0x006a5228 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005fd9e0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005fd390  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x005fd410  // __thiscall
    virtual void vfunc_3();  // [3] 0x005fd530  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005fd560  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005fd5d0  // __stdcall, returns int

    VectorClass_struct_DirtyAreaStruct();  // 0x005f4a34
    virtual ~VectorClass_struct_DirtyAreaStruct();

    // ---- data members (offsets from ctor 0x005fd9d6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a5228
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
