#pragma once
// ==========================================================================
// MSAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSAnim
// vtable       : 0x006a2470  (6 slots)
// constructor  : 0x00557710
// vtable store : 0x0055771b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class MSAnim
{
public:
    // ---- vtable 0x006a2470 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4();  // [4] 0x006829a5  // __cdecl
    virtual void vfunc_5();  // [5] 0x006829a5  // __cdecl

    MSAnim();  // 0x00557710
    virtual ~MSAnim();

    // ---- data members (offsets from ctor 0x00557710) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a2470; 4-byte store: mov dword ptr [esi], 0x6a2470
};
