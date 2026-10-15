#pragma once
// ==========================================================================
// TypeList_class_oxelAnimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<class oxelAnimTypeClass*>
// vtable       : 0x006a3c64  (6 slots)
// constructor  : 0x005abc48
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_class_oxelAnimTypeClass.h"

class TypeList_class_oxelAnimTypeClass : public DynamicVectorClass_class_oxelAnimTypeClass
{
public:
    // ---- vtable 0x006a3c64 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005b9f10  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005ba0f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005b9670  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005ba1f0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005b96a0  // __stdcall, returns int

    TypeList_class_oxelAnimTypeClass();  // 0x005abc48
    virtual ~TypeList_class_oxelAnimTypeClass();

    // ---- data members (offsets from ctor 0x005abc17) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3c80
};
