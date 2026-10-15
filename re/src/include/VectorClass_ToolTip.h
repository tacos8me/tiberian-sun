#pragma once
// ==========================================================================
// VectorClass_ToolTip.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ToolTip*>
// vtable       : 0x006a62fc  (6 slots)
// constructor  : 0x0062ab6d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ToolTip
{
public:
    // ---- vtable 0x006a62fc (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0062b7f0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0062b590  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0062b5d0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0062b680  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0062b6b0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0062b6f0  // __stdcall, returns int

    VectorClass_ToolTip();  // 0x0062ab6d
    virtual ~VectorClass_ToolTip();

    // ---- data members (offsets from ctor 0x0062aa97) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a62fc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // /* ? */
};
