#pragma once
// ==========================================================================
// DynamicVectorClass_struct_AngerStruct.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct AngerStruct>
// vtable       : 0x0069fa98  (6 slots)
// constructor  : 0x004b3a4a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_AngerStruct.h"

class DynamicVectorClass_struct_AngerStruct : public VectorClass_struct_AngerStruct
{
public:
    // ---- vtable 0x0069fa98 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004c6770  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004c5770  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004c5960  // __thiscall
    virtual void vfunc_3();  // [3] 0x004c5240  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004c5a60  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004c5270  // __stdcall, returns int

    DynamicVectorClass_struct_AngerStruct();  // 0x004b3a4a
    virtual ~DynamicVectorClass_struct_AngerStruct();

    // ---- data members (offsets from ctor 0x004b3938) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f840
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
