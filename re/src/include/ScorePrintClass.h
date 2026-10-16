#pragma once
// ==========================================================================
// ScorePrintClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScorePrintClass
// vtable       : 0x006a3ecc  (4 slots)
// constructor  : 0x005ca16a
// vtable store : 0x005ca2b0  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScoreAnimClass.h"

class ScorePrintClass : public ScoreAnimClass
{
public:
    // ---- vtable 0x006a3ecc (4 slots) ----
    virtual void vfunc_1();  // [1] 0x005cde90  // __thiscall
    virtual void vfunc_2();  // [2] 0x005cdee0  // __thiscall
    virtual void vfunc_3(int a0);  // [3] 0x005cdbc0  // __thiscall

    ScorePrintClass();  // 0x005ca16a
    virtual ~ScorePrintClass();

    // ---- data members (offsets from ctor 0x005ca16a) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a3ecc; 4-byte store: mov dword ptr [edi], 0x6a3ecc
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [edi + 4], esi
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [edi + 8], eax
    unsigned long m_00c;  // +0x00c /* ? */; /* width ? */
    unsigned char pad_010[0x4];
    unsigned long m_014;  // +0x014 /* ? */; /* width ? */
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [edi + 0x18], edx
    unsigned long m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [edi + 0x1c], eax
    unsigned long m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [edi + 0x20], eax
    unsigned char m_024;  // +0x024 /* ? */; 1-byte store: mov byte ptr [edi + 0x24], al
    unsigned char pad_025[0x3];
    unsigned long m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [edi + 0x28], eax
};
