#pragma once
// ==========================================================================
// DynamicVectorClass_class_oxelAnimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class oxelAnimTypeClass*>
// vtable       : 0x006a3d34  (6 slots)
// constructor  : 0x005ba0d5
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_oxelAnimTypeClass.h"

class DynamicVectorClass_class_oxelAnimTypeClass : public VectorClass_class_oxelAnimTypeClass
{
public:
    // ---- vtable 0x006a3d34 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005b9f10  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005ba0f0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005b9670  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005ba1f0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005b96a0  // __stdcall, returns int

    DynamicVectorClass_class_oxelAnimTypeClass();  // 0x005ba0d5
    virtual ~DynamicVectorClass_class_oxelAnimTypeClass();

    // ---- data members (offsets from ctor 0x005ba08b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3c9c
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
