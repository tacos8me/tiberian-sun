#pragma once
// ==========================================================================
// DynamicVectorClass_NodeNameType.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<NodeNameType*>
// vtable       : 0x006a41b8  (6 slots)
// constructor  : 0x005d3751
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_NodeNameType.h"

class DynamicVectorClass_NodeNameType : public VectorClass_NodeNameType
{
public:
    // ---- vtable 0x006a41b8 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005d73c0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005d6cd0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005d6e50  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005d6410  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005d6f00  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005d6440  // __stdcall, returns int

    DynamicVectorClass_NodeNameType();  // 0x005d3751
    virtual ~DynamicVectorClass_NodeNameType();

    // ---- data members (offsets from ctor 0x005d36d9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a41b8
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
