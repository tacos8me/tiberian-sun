#pragma once
// ==========================================================================
// MSVQAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSVQAnim
// vtable       : 0x006a24e0  (6 slots)
// constructor  : 0x00558a10
// vtable store : 0x00558a5b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSAnim.h"

class MSVQAnim : public MSAnim
{
public:
    // ---- vtable 0x006a24e0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x00558c70  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [5] 0x00558dc0

    MSVQAnim();  // 0x00558a10
    virtual ~MSVQAnim();

    // ---- data members (offsets from ctor 0x00558c20) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a24e0; 4-byte store: mov dword ptr [esi], 0x6a24e0
};
