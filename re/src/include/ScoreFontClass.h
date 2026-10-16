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
    unsigned long m_000;  // +0x000 init 0x6a3f18; 4-byte store: mov dword ptr [esi], 0x6a3f18
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned char pad_010[0x4];
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
};
