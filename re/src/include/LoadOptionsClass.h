#pragma once
// ==========================================================================
// LoadOptionsClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LoadOptionsClass
// vtable       : 0x006a1814  (5 slots)
// constructor  : 0x004f8000
// vtable store : 0x004f8032  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class LoadOptionsClass
{
public:
    // ---- vtable 0x006a1814 (5 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004f9100  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f9150  // __stdcall
    virtual void vfunc_3(int a0);  // [3] 0x004f91a0  // __stdcall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);  // [4] 0x004f91c0

    LoadOptionsClass();  // 0x004f8000
    virtual ~LoadOptionsClass();

    // ---- data members (offsets from ctor 0x004f8000) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a1814; 4-byte store: mov dword ptr [esi], 0x6a1814
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned long m_008;  // +0x008 init 0x6cc528; 4-byte store: mov dword ptr [esi + 8], 0x6cc528
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned long m_010;  // +0x010 init 0x800; 4-byte store: mov dword ptr [esi + 0x10], 0x800
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
};
