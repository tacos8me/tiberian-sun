#pragma once
// ==========================================================================
// DynamicVectorClass_ircraftTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ircraftTypeClass&*>
// vtable       : 0x006a3da4  (6 slots)
// constructor  : 0x005baf85
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ircraftTypeClass.h"

class DynamicVectorClass_ircraftTypeClass : public VectorClass_ircraftTypeClass
{
public:
    // ---- vtable 0x006a3da4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005badc0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005bafa0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005b9b50  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005bb0a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005b9b80  // __stdcall, returns int

    DynamicVectorClass_ircraftTypeClass();  // 0x005baf85
    virtual ~DynamicVectorClass_ircraftTypeClass();

    // ---- data members (offsets from ctor 0x005baf3b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3cf0
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
