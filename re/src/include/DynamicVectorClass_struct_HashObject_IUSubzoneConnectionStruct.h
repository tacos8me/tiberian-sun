#pragma once
// ==========================================================================
// DynamicVectorClass_struct_HashObject_IUSubzoneConnectionStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct HashObject<IUSubzoneConnectionStruct>>
// vtable       : 0x006a1b2c  (6 slots)
// constructor  : 0x00502293
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_HashObject_IUSubzoneConnectionStruct.h"

class DynamicVectorClass_struct_HashObject_IUSubzoneConnectionStruct : public VectorClass_struct_HashObject_IUSubzoneConnectionStruct
{
public:
    // ---- vtable 0x006a1b2c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00521da0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00521de0  // __thiscall
    virtual void vfunc_3();  // [3] 0x00520810  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00521ee0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00520840  // __stdcall, returns int

    DynamicVectorClass_struct_HashObject_IUSubzoneConnectionStruct();  // 0x00502293
    virtual ~DynamicVectorClass_struct_HashObject_IUSubzoneConnectionStruct();

    // ---- data members (offsets from ctor 0x00550325) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1bd4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
