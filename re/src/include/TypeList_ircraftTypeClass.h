#pragma once
// ==========================================================================
// TypeList_ircraftTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<ircraftTypeClass&*>
// vtable       : 0x006a3bf4  (6 slots)
// constructor  : 0x005abf9b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_ircraftTypeClass.h"

class TypeList_ircraftTypeClass : public DynamicVectorClass_ircraftTypeClass
{
public:
    // ---- vtable 0x006a3bf4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005badc0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005bafa0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005b9b50  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005bb0a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005b9b80  // __stdcall, returns int

    TypeList_ircraftTypeClass();  // 0x005abf9b
    virtual ~TypeList_ircraftTypeClass();

    // ---- data members (offsets from ctor 0x005abee9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3c80
};
