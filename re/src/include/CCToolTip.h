#pragma once
// ==========================================================================
// CCToolTip.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CCToolTip
// vtable       : 0x006a7654  (6 slots)
// constructor  : 0x0065bbfd
// vtable store : 0x0065bc24  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ToolTipManager.h"

class CCToolTip : public ToolTipManager
{
public:
    // ---- vtable 0x006a7654 (6 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5);  // [1] 0x0044be30  // __thiscall
    virtual void vfunc_2(int a0);  // [2] 0x0044bfb0  // __thiscall
    virtual void vfunc_3(int a0);  // [3] 0x0044c010  // __stdcall
    virtual void vfunc_4(int a0, int a1, int a2, int a3);  // [4] 0x0044c030  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x0044c1a0  // __stdcall, returns int

    CCToolTip();  // 0x0065bbfd
    virtual ~CCToolTip();

    // ---- data members (offsets from ctor 0x0065bbfd) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x15c];
    unsigned int m_160;  // +0x160 init 0x7; 4-byte store: mov dword ptr [esi + 0x160], 7
};
