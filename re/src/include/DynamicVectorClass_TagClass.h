#pragma once
// ==========================================================================
// DynamicVectorClass_TagClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TagClass*>
// vtable       : 0x0069f840  (6 slots)
// constructor  : 0x004a9293
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TagClass.h"

class DynamicVectorClass_TagClass : public VectorClass_TagClass
{
public:
    // ---- vtable 0x0069f840 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b0760  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aa1d0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aa350  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a94b0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aa400  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a94e0  // __stdcall, returns int

    DynamicVectorClass_TagClass();  // 0x004a9293
    virtual ~DynamicVectorClass_TagClass();

    // ---- data members (offsets from ctor 0x004b3938) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f840
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
