#pragma once
// ==========================================================================
// TypeList_SmudgeTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TypeList<SmudgeTypeClass*>
// vtable       : 0x006a3c2c  (6 slots)
// constructor  : 0x005abdb8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "DynamicVectorClass_SmudgeTypeClass.h"

class TypeList_SmudgeTypeClass : public DynamicVectorClass_SmudgeTypeClass
{
public:
    // ---- vtable 0x006a3c2c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005ba6c0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005ba8a0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005b9920  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005ba9a0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005b9950  // __stdcall, returns int

    TypeList_SmudgeTypeClass();  // 0x005abdb8
    virtual ~TypeList_SmudgeTypeClass();

    // ---- data members (offsets from ctor 0x005abc17) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3c80
};
