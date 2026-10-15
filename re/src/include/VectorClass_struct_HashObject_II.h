#pragma once
// ==========================================================================
// VectorClass_struct_HashObject_II.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct HashObject<II>>
// vtable       : 0x006a1bb8  (6 slots)
// constructor  : 0x0052228c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_HashObject_II
{
public:
    // ---- vtable 0x006a1bb8 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00522760  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00521c50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00522310  // __thiscall
    virtual void vfunc_3();  // [3] 0x005223e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00522410  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00522450  // __stdcall, returns int

    VectorClass_struct_HashObject_II();  // 0x0052228c
    virtual ~VectorClass_struct_HashObject_II();

    // ---- data members (offsets from ctor 0x00522259) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1bb8
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // /* ? */
};
