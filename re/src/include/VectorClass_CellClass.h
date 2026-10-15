#pragma once
// ==========================================================================
// VectorClass_CellClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<CellClass*>
// vtable       : 0x006a1a68  (6 slots)
// constructor  : 0x005019f9
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_CellClass
{
public:
    // ---- vtable 0x006a1a68 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00521870  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005218b0  // __thiscall
    virtual void vfunc_3();  // [3] 0x00521960  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00521990  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005219d0  // __stdcall, returns int

    VectorClass_CellClass();  // 0x005019f9
    virtual ~VectorClass_CellClass();

    // ---- data members (offsets from ctor 0x0050192a) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a19f0
    unsigned char pad_004[0x160];
    unsigned long m_164;  // init 0x6a1a68
    unsigned long m_168;  // /* ? */
    unsigned long m_16c;  // /* ? */
    unsigned long m_170;  // init 0x1
};
