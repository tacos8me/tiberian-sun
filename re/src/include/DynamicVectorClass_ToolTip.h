#pragma once
// ==========================================================================
// DynamicVectorClass_ToolTip.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ToolTip*>
// vtable       : 0x006a62e0  (6 slots)
// constructor  : 0x0062aa2c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ToolTip.h"

class DynamicVectorClass_ToolTip : public VectorClass_ToolTip
{
public:
    // ---- vtable 0x006a62e0 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0062b840  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0062b590  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0062b710  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0062b540  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0062b7c0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0062b570  // __stdcall, returns int

    DynamicVectorClass_ToolTip();  // 0x0062aa2c
    virtual ~DynamicVectorClass_ToolTip();

    // ---- data members (offsets from ctor 0x0062a9e7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a62e0
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
