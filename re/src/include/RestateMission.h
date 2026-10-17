#pragma once
// ==========================================================================
// RestateMission.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RestateMission
// vtable       : 0x006a3a7c  (3 slots)
// constructor  : 0x005a80c0
// vtable store : 0x005a80e6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSEngine.h"

class RestateMission : public MSEngine
{
public:
    // ---- vtable 0x006a3a7c (3 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005a8d30  // __thiscall
    virtual void vfunc_2();  // [2] 0x0055cc40  // __cdecl

    RestateMission();  // 0x005a80c0
    virtual ~RestateMission();

    // ---- data members (offsets from ctor 0x005a8210) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x8: the compiled end of the
    // base sub-object region. Evidence: MSEngine at +0x0 (ends +0x8).
    // 1 recovered offset(s) below +0x8 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char pad_008[0x40];
    unsigned int m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [esi + 0x48], eax
    unsigned char pad_04c[0x400];
    unsigned int m_44c;  // +0x44c /* ? */; 4-byte store: mov dword ptr [esi + 0x44c], eax
    unsigned char pad_450[0x18];
    unsigned int m_468;  // +0x468 /* ? */; 4-byte store: mov dword ptr [esi + 0x468], eax
    unsigned int m_46c;  // +0x46c /* ? */; 4-byte store: mov dword ptr [esi + 0x46c], eax
    unsigned int m_470;  // +0x470 /* ? */; 4-byte store: mov dword ptr [esi + 0x470], eax
};
