#pragma once
// ==========================================================================
// DynamicVectorClass_ObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ObjectClass*>
// vtable       : 0x0069ae9c  (6 slots)
// constructor  : 0x00454be1
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ObjectClass.h"

class DynamicVectorClass_ObjectClass : public VectorClass_ObjectClass
{
public:
    // ---- vtable 0x0069ae9c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00402540  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004026c0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004024f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00402770  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00402520  // __stdcall, returns int

    DynamicVectorClass_ObjectClass();  // 0x00454be1
    virtual ~DynamicVectorClass_ObjectClass();

    // ---- data members (offsets from ctor 0x00454b59) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x8];
    unsigned long m_008;  // init 0x698730
    unsigned char pad_00c[0x3];
    unsigned long m_00f;  // zero-initialised
    unsigned char pad_013[0x1];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
};
