#pragma once
// ==========================================================================
// ScoreAnimClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreAnimClass
// vtable       : 0x006a3efc  (4 slots)
// constructor  : 0x005cde50
// vtable store : 0x005cde7c  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ScoreAnimClass
{
public:
    // ---- vtable 0x006a3efc (4 slots) ----
    virtual void vfunc_1();  // [1] 0x005cde90  // __thiscall
    virtual void vfunc_2();  // [2] 0x005cdee0  // __thiscall
    virtual void vfunc_3();  // [3] 0x006829a5  // __cdecl

    ScoreAnimClass();  // 0x005cde50
    virtual ~ScoreAnimClass();

    // ---- data members (offsets from ctor 0x005cde50) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ecx
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned char pad_010[0x4];
    unsigned int m_014;  // +0x014 zero-initialised; 4-byte store: mov dword ptr [esi + 0x14], 0
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], edx
};
