#pragma once
// ==========================================================================
// DynamicVectorClass_FileEntryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<FileEntryClass*>
// vtable       : 0x006a182c  (6 slots)
// constructor  : 0x004f8017
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_FileEntryClass.h"

class DynamicVectorClass_FileEntryClass : public VectorClass_FileEntryClass
{
public:
    // ---- vtable 0x006a182c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004f96e0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004f9430  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f95b0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004f93e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f9660  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004f9410  // __stdcall, returns int

    DynamicVectorClass_FileEntryClass();  // 0x004f8017
    virtual ~DynamicVectorClass_FileEntryClass();

    // ---- data members (offsets from ctor 0x004f7ff7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a182c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
