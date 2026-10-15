#pragma once
// ==========================================================================
// TypeList_uildingTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<uildingTypeClass*>
// vtable       : 0x006a3c80  (6 slots)
// constructor  : 0x005abc34
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_uildingTypeClass.h"

class TypeList_uildingTypeClass : public DynamicVectorClass_uildingTypeClass
{
public:
    // ---- vtable 0x006a3c80 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005b9ba0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004c5510  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004c5690  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004c51f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004c5740  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004c5220  // __stdcall, returns int

    TypeList_uildingTypeClass();  // 0x005abc34
    virtual ~TypeList_uildingTypeClass();

    // ---- data members (offsets from ctor 0x005abc17) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3c80
};
