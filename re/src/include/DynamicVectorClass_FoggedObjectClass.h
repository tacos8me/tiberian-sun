#pragma once
// ==========================================================================
// DynamicVectorClass_FoggedObjectClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<FoggedObjectClass*>
// vtable       : 0x0069a6fc  (6 slots)
// constructor  : 0x0043a07f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_FoggedObjectClass.h"

class DynamicVectorClass_FoggedObjectClass : public VectorClass_FoggedObjectClass
{
public:
    // ---- vtable 0x0069a6fc (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0043aae0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0043a880  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0043a8c0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0043a2f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0043a970  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0043a320  // __stdcall, returns int

    DynamicVectorClass_FoggedObjectClass();  // 0x0043a07f
    virtual ~DynamicVectorClass_FoggedObjectClass();

    // ---- data members (offsets from ctor 0x00439f35) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69a6fc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
