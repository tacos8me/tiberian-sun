#pragma once
// ==========================================================================
// VectorClass_struct_DrawRecord_FoggedObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<struct DrawRecord, FoggedObjectClass>
// vtable       : 0x0069e29c  (6 slots)
// constructor  : 0x0049a7b0
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_struct_DrawRecord_FoggedObjectClass
{
public:
    // ---- vtable 0x0069e29c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0049b880  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0049b8e0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0049b9f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0049ba20  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0049ba70  // __stdcall, returns int

    VectorClass_struct_DrawRecord_FoggedObjectClass();  // 0x0049a7b0
    virtual ~VectorClass_struct_DrawRecord_FoggedObjectClass();

    // ---- data members (offsets from ctor 0x0049a5b4) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69e218
    unsigned long m_004;  // init 0x69e1fc
    unsigned char pad_008[0x44];
    unsigned long m_04c;  // init 0x69e29c
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned char pad_058[0x1];
    unsigned long m_059;  // /* ? */
};
