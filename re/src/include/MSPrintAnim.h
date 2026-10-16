#pragma once
// ==========================================================================
// MSPrintAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSPrintAnim
// vtable       : 0x006a24fc  (6 slots)
// constructor  : 0x00558f20
// vtable store : 0x00558f7f  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSAnim.h"

class MSPrintAnim : public MSAnim
{
public:
    // ---- vtable 0x006a24fc (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x00559310  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [5] 0x005594a0  // __thiscall

    MSPrintAnim();  // 0x00558f20
    virtual ~MSPrintAnim();

    // ---- data members (offsets from ctor 0x00558f20) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a2470; 4-byte store: mov dword ptr [esi], 0x6a2470
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ecx
    unsigned char m_00c;  // +0x00c init 0x1; 1-byte store: mov byte ptr [esi + 0xc], 1
    unsigned char pad_00d[0x3];
    unsigned long m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], edx
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], edi
    unsigned long m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], eax
    unsigned long m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], edi
    unsigned long m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], edi
    unsigned long m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], edi
    unsigned long m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], edx
    unsigned char m_030;  // +0x030 /* ? */; 1-byte store: mov byte ptr [esi + 0x30], al
    unsigned char pad_031[0x3];
    unsigned long m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], ecx
};
