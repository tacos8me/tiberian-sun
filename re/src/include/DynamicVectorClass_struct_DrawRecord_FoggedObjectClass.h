#pragma once
// ==========================================================================
// DynamicVectorClass_struct_DrawRecord_FoggedObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<struct DrawRecord, FoggedObjectClass>
// vtable       : 0x0069e280  (6 slots)
// constructor  : 0x0049985f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_struct_DrawRecord_FoggedObjectClass.h"

class DynamicVectorClass_struct_DrawRecord_FoggedObjectClass : public VectorClass_struct_DrawRecord_FoggedObjectClass
{
public:
    // ---- vtable 0x0069e280 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0049bc30  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0049b880  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0049ba90  // __thiscall
    virtual void vfunc_3();  // [3] 0x0049b830  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0049bba0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0049b860  // __stdcall, returns int

    DynamicVectorClass_struct_DrawRecord_FoggedObjectClass();  // 0x0049985f
    virtual ~DynamicVectorClass_struct_DrawRecord_FoggedObjectClass();

    // ---- data members (offsets from ctor 0x0049993b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69e280
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
