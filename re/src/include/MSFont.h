#pragma once
// ==========================================================================
// MSFont.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSFont
// vtable       : 0x006a2708  (5 slots)
// constructor  : 0x0055d590
// vtable store : 0x0055d5a5  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class MSFont
{
public:
    // ---- vtable 0x006a2708 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0055de20  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0055da70  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x0055da20  // __thiscall
    virtual void vfunc_3(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [3] 0x0055dcb0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4);  // [4] 0x0055db40  // __thiscall

    MSFont();  // 0x0055d590
    virtual ~MSFont();

    // ---- data members (offsets from ctor 0x0055d590) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char pad_004[0x8];
    unsigned char m_00c;  // +0x00c /* ? */; 1-byte store: mov byte ptr [esi + 0xc], al
    unsigned char m_00d;  // +0x00d /* ? */; 1-byte store: mov byte ptr [esi + 0xd], al
    unsigned char m_00e;  // +0x00e /* ? */; 1-byte store: mov byte ptr [esi + 0xe], al
};
