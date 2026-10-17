#pragma once
// ==========================================================================
// MSWordAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSWordAnim
// vtable       : 0x006a2454  (6 slots)
// constructor  : 0x00555500
// vtable store : 0x00555578  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSPrintAnim.h"

class MSWordAnim : public MSPrintAnim
{
public:
    // ---- vtable 0x006a2454 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005595a0  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [5] 0x005597a0  // __thiscall

    MSWordAnim();  // 0x00555500
    virtual ~MSWordAnim();

    // ---- data members (offsets from ctor 0x00555500) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x44];
    unsigned int m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [edi + 0x48], ecx
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [edi + 0x4c], eax
    unsigned int m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [edi + 0x50], eax
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [edi + 0x54], eax
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [edi + 0x58], eax
};
