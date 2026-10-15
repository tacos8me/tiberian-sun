#pragma once
// ==========================================================================
// TypeList_articleSystemTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<articleSystemTypeClass**>
// vtable       : 0x006a5ca8  (6 slots)
// constructor  : 0x0061ee1d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_articleSystemTypeClass.h"

class TypeList_articleSystemTypeClass : public DynamicVectorClass_articleSystemTypeClass
{
public:
    // ---- vtable 0x006a5ca8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0061e2b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0061e430  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0061e260  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0061e4e0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0061e290  // __stdcall, returns int

    TypeList_articleSystemTypeClass();  // 0x0061ee1d
    virtual ~TypeList_articleSystemTypeClass();

    // ---- data members (offsets from ctor 0x0061f0b7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a5ca8
};
