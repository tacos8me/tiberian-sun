#pragma once
// ==========================================================================
// MSOverlayAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSOverlayAnim
// vtable       : 0x006a24c4  (6 slots)
// constructor  : 0x00558660
// vtable store : 0x0055868d  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSFadeAnim.h"

class MSOverlayAnim : public MSFadeAnim
{
public:
    // ---- vtable 0x006a24c4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2);  // [4] 0x005586a0  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5);  // [5] 0x005588d0  // __thiscall

    MSOverlayAnim();  // 0x00558660
    virtual ~MSOverlayAnim();

    // ---- data members (offsets from ctor 0x00558660) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a24c4; 4-byte store: mov dword ptr [esi], 0x6a24c4
    unsigned char pad_004[0x3c];
    unsigned long m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], edx
};
