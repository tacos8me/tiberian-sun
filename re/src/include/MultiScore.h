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
    virtual void vfunc_1(int a0);  // [1] 0x005554f0  // __stdcall, returns void
    virtual void vfunc_2();  // [2] 0x00557490  // returns char

    MultiScore();  // 0x00554f80
    virtual ~MultiScore();

    // ---- data members (offsets from ctor 0x00555010) ----
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
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [esi + 0x4c], eax
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [esi + 0x50], eax
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], eax
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], eax
};
