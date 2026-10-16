#pragma once
// ==========================================================================
// MultiScore.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MultiScore
// vtable       : 0x006a2444  (3 slots)
// constructor  : 0x00554f80
// vtable store : 0x00554fab  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSEngine.h"

class MultiScore : public MSEngine
{
public:
    // ---- vtable 0x006a2444 (3 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005554f0  // __stdcall
    virtual void vfunc_2();  // [2] 0x00557490

    MultiScore();  // 0x00554f80
    virtual ~MultiScore();

    // ---- data members (offsets from ctor 0x00555010) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a2444; 4-byte store: mov dword ptr [esi], 0x6a2444
    unsigned char pad_004[0x44];
    unsigned long m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [esi + 0x48], eax
    unsigned long m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], eax
    unsigned long m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned long m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], eax
    unsigned long m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], eax
};
