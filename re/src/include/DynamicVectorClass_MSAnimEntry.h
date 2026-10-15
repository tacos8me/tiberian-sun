#pragma once
// ==========================================================================
// DynamicVectorClass_MSAnimEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MSAnimEntry*>
// vtable       : 0x006a2538  (6 slots)
// constructor  : 0x005599f7
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MSAnimEntry.h"

class DynamicVectorClass_MSAnimEntry : public VectorClass_MSAnimEntry
{
public:
    // ---- vtable 0x006a2538 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0055c020  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0055b890  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055ba10  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0055ade0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055bac0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0055ae10  // __stdcall, returns int

    DynamicVectorClass_MSAnimEntry();  // 0x005599f7
    virtual ~DynamicVectorClass_MSAnimEntry();

    // ---- data members (offsets from ctor 0x005599b7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2538
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
};
