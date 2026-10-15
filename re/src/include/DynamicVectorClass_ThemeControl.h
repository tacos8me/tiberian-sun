#pragma once
// ==========================================================================
// DynamicVectorClass_ThemeControl.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ThemeControl*>
// vtable       : 0x006a61cc  (6 slots)
// constructor  : 0x006276dc
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ThemeControl.h"

class DynamicVectorClass_ThemeControl : public VectorClass_ThemeControl
{
public:
    // ---- vtable 0x006a61cc (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00627ec0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004ab010  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00627de0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00627d90  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00627e90  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00627dc0  // __stdcall, returns int

    DynamicVectorClass_ThemeControl();  // 0x006276dc
    virtual ~DynamicVectorClass_ThemeControl();

    // ---- data members (offsets from ctor 0x006276a8) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // /* ? */
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0xff
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0x6a61cc
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // init 0x1
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0xa
};
