#pragma once
// ==========================================================================
// DynamicVectorClass_struct_HashObject_II.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct HashObject<II>>
// vtable       : 0x006a1b48  (6 slots)
// constructor  : 0x005021c8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_HashObject_II.h"

class DynamicVectorClass_struct_HashObject_II : public VectorClass_struct_HashObject_II
{
public:
    // ---- vtable 0x006a1b48 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00521c50  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x00521c90  // __thiscall
    virtual void vfunc_3();  // [3] 0x005207c0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00521d70  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005207f0  // __stdcall, returns int

    DynamicVectorClass_struct_HashObject_II();  // 0x005021c8
    virtual ~DynamicVectorClass_struct_HashObject_II();

    // ---- data members (offsets from ctor 0x005502ae) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1bb8
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
