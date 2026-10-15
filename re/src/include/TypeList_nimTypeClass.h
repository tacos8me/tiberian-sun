#pragma once
// ==========================================================================
// TypeList_nimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<nimTypeClass&*>
// vtable       : 0x006a3c48  (6 slots)
// constructor  : 0x005abc5f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_nimTypeClass.h"

class TypeList_nimTypeClass : public DynamicVectorClass_nimTypeClass
{
public:
    // ---- vtable 0x006a3c48 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005b9c40  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005ba290  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005ba470  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005b9740  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005ba570  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005b9770  // __stdcall, returns int

    TypeList_nimTypeClass();  // 0x005abc5f
    virtual ~TypeList_nimTypeClass();

    // ---- data members (offsets from ctor 0x005abc17) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3c80
};
