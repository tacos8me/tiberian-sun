#pragma once
// ==========================================================================
// FontClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FontClass
// vtable       : 0x006a7b5c  (9 slots)
// constructor  : 0x00672570
// vtable store : 0x0067257b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class FontClass
{
public:
    // ---- vtable 0x006a7b5c (9 slots) ----
    virtual void vfunc_1();  // [1] 0x006829a5  // __cdecl
    virtual void vfunc_2();  // [2] 0x006829a5  // __cdecl
    virtual void vfunc_3();  // [3] 0x006829a5  // __cdecl
    virtual void vfunc_4();  // [4] 0x006829a5  // __cdecl
    virtual void vfunc_5();  // [5] 0x006829a5  // __cdecl
    virtual void vfunc_6();  // [6] 0x006829a5  // __cdecl
    virtual void vfunc_7();  // [7] 0x006829a5  // __cdecl
    virtual void vfunc_8();  // [8] 0x006829a5  // __cdecl

    FontClass();  // 0x00672570
    virtual ~FontClass();

    // ---- data members (offsets from ctor 0x00672570) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a7b5c; 4-byte store: mov dword ptr [esi], 0x6a7b5c
};
