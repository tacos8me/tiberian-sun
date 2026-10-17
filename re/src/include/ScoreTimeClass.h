#pragma once
// ==========================================================================
// ScoreTimeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreTimeClass
// vtable       : 0x006a3ee4  (4 slots)
// constructor  : 0x005ca088
// vtable store : 0x005ca0db  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScoreAnimClass.h"

class ScoreTimeClass : public ScoreAnimClass
{
public:
    // ---- vtable 0x006a3ee4 (4 slots) ----
    virtual void vfunc_1();  // [1] 0x005cde90  // __thiscall
    virtual void vfunc_2();  // [2] 0x005cdee0  // __thiscall
    virtual void vfunc_3(int a0);  // [3] 0x005cda00  // __thiscall

    ScoreTimeClass();  // 0x005ca088
    virtual ~ScoreTimeClass();

    // ---- data members (offsets from ctor 0x005ca088) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x1c: the compiled end of the
    // base sub-object region. Evidence: ScoreAnimClass at +0x0 (ends +0x1c).
    // 6 recovered offset(s) below +0x1c lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x14, +0x18
    unsigned int m_01c;  // +0x01c zero-initialised; 4-byte store: mov dword ptr [edi + 0x1c], 0
    unsigned int m_020;  // +0x020 init 0x3c; 4-byte store: mov dword ptr [edi + 0x20], 0x3c
    unsigned int m_024;  // +0x024 init 0x3; 4-byte store: mov dword ptr [edi + 0x24], 3
    unsigned int m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [edi + 0x28], esi
};
