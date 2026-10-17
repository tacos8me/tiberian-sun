#pragma once
// ==========================================================================
// MSEngine.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSEngine
// vtable       : 0x006a266c  (3 slots)
// constructor  : 0x0055c130
// vtable store : 0x0055c193  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class MSEngine
{
public:
    // ---- vtable 0x006a266c (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0055cc50  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00542040  // __stdcall
    virtual void vfunc_2();  // [2] 0x0055cc40  // __cdecl

    MSEngine();  // 0x0055c130
    virtual ~MSEngine();

    // ---- data members (offsets from ctor 0x0055c130) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [ebx + 4], eax
};
