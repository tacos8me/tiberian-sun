#pragma once
// ==========================================================================
// ToolTipManager.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ToolTipManager
// vtable       : 0x006a62c4  (6 slots)
// constructor  : 0x0062a9f0
// vtable store : 0x0062aa5c  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ToolTipManager
{
public:
    // ---- vtable 0x006a62c4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0062b520  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0062b3c0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x0062b3d0  // __stdcall
    virtual void vfunc_3(int a0);  // [3] 0x0062b4a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0062b4d0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0062b4f0  // __stdcall, returns int

    ToolTipManager();  // 0x0062a9f0
    virtual ~ToolTipManager();

    // ---- data members (offsets from ctor 0x0062a9f0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a62c4; 4-byte store: mov dword ptr [esi], 0x6a62c4
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ecx
    unsigned char pad_00c[0x8];
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], ebx
    unsigned char pad_018[0x110];
    unsigned long m_128;  // +0x128 init 0x3e8; 4-byte store: mov dword ptr [esi + 0x128], 0x3e8
    unsigned long m_12c;  // +0x12c init 0x2710; 4-byte store: mov dword ptr [esi + 0x12c], 0x2710
    unsigned char pad_130[0x18];
    unsigned long m_148;  // +0x148 /* ? */; 4-byte store: mov dword ptr [esi + 0x148], ebx
    unsigned long m_14c;  // +0x14c /* ? */; 4-byte store: mov dword ptr [esi + 0x14c], ebx
    unsigned long m_150;  // +0x150 /* ? */; 4-byte store: mov dword ptr [esi + 0x150], ebx
    unsigned char m_154;  // +0x154 /* ? */; 1-byte store: mov byte ptr [esi + 0x154], bl
    unsigned char pad_155[0x3];
    unsigned long m_158;  // +0x158 /* ? */; 4-byte store: mov dword ptr [esi + 0x158], ebx
};
