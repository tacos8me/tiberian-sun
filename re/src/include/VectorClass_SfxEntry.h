#pragma once
// ==========================================================================
// VectorClass_SfxEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<SfxEntry*>
// vtable       : 0x006a3e18  (6 slots)
// constructor  : 0x005c3831
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_SfxEntry
{
public:
    // ---- vtable 0x006a3e18 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005c89c0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005c8610  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005c8650  // __thiscall
    virtual void vfunc_3();  // [3] 0x005c8700  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005c8730  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005c8770  // __stdcall, returns int

    VectorClass_SfxEntry();  // 0x005c3831
    virtual ~VectorClass_SfxEntry();

    // ---- data members (offsets from ctor 0x005c878b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3e18
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // zero-initialised
    unsigned long m_014;  // init 0xa
};
