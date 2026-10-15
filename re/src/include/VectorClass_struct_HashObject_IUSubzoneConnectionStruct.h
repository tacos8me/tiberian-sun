#pragma once
// ==========================================================================
// VectorClass_struct_HashObject_IUSubzoneConnectionStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct HashObject<IUSubzoneConnectionStruct>>
// vtable       : 0x006a1bd4  (6 slots)
// constructor  : 0x0052249c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_HashObject_IUSubzoneConnectionStruct
{
public:
    // ---- vtable 0x006a1bd4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005227b0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00521da0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00522520  // __thiscall
    virtual void vfunc_3();  // [3] 0x00522620  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00522650  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00522690  // __stdcall, returns int

    VectorClass_struct_HashObject_IUSubzoneConnectionStruct();  // 0x0052249c
    virtual ~VectorClass_struct_HashObject_IUSubzoneConnectionStruct();

    // ---- data members (offsets from ctor 0x0052246b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1bd4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // /* ? */
};
