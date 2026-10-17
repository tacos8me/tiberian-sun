#pragma once
// ==========================================================================
// ScoreFontClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreFontClass
// vtable       : 0x006a3f18  (5 slots)
// constructor  : 0x005cd250
// vtable store : 0x005cd263  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ScoreFontClass
{
public:
    // ---- vtable 0x006a3f18 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005cdf60  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005cd4d0  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005cd4a0  // __thiscall
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x005cd6e0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4);  // [4] 0x005cd590  // __thiscall

    ScoreFontClass();  // 0x005cd250
    virtual ~ScoreFontClass();

    // ---- data members (offsets from ctor 0x005cd250) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned char pad_010[0x4];
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
};
