#pragma once
// ==========================================================================
// ToolTipManager.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ToolTipManager
// vtable       : 0x006a62c4  (6 slots)
// constructor  : 0x0062aa5c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ToolTipManager
{
public:
    // ---- vtable 0x006a62c4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0062b520  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0062b3c0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x0062b3d0  // __stdcall
    virtual void vfunc_3(int a0);  // [3] 0x0062b4a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0062b4d0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0062b4f0  // __stdcall, returns int

    ToolTipManager();  // 0x0062aa5c
    virtual ~ToolTipManager();

    // ---- data members (offsets from ctor 0x0062a9e7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a62c4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x8];
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x110];
    unsigned long m_128;  // init 0x3e8
    unsigned long m_12c;  // init 0x2710
    unsigned char pad_130[0x18];
    unsigned long m_148;  // /* ? */
    unsigned long m_14c;  // /* ? */
    unsigned long m_150;  // /* ? */
    unsigned long m_154;  // /* ? */
    unsigned long m_158;  // /* ? */
};
