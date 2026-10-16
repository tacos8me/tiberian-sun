#pragma once
// ==========================================================================
// WinModemClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WinModemClass
// vtable       : 0x006a7620  (10 slots)
// constructor  : 0x00657910
// vtable store : 0x0065791a  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class WinModemClass
{
public:
    // ---- vtable 0x006a7620 (10 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4);  // [1] 0x00657c00  // __thiscall
    virtual void vfunc_2(int a0);  // [2] 0x00658050  // __stdcall
    virtual void vfunc_3(int a0);  // [3] 0x00658060  // __cdecl
    virtual void vfunc_4();  // [4] 0x006580b0
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [5] 0x00658480  // __thiscall
    virtual void vfunc_6(int a0);  // [6] 0x006586f0  // __thiscall
    virtual void vfunc_7(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [7] 0x006588b0  // __thiscall
    virtual void vfunc_8(int a0);  // [8] 0x00658c40  // __stdcall
    virtual void vfunc_9(int a0);  // [9] 0x00658c50  // __stdcall

    WinModemClass();  // 0x00657910
    virtual ~WinModemClass();

    // ---- data members (offsets from ctor 0x00657910) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a7620; 4-byte store: mov dword ptr [esi], 0x6a7620
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned long m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned char pad_01c[0x8];
    unsigned long m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], eax
    unsigned char pad_028[0x28];
    unsigned char m_050;  // +0x050 /* ? */; 1-byte store: mov byte ptr [esi + 0x50], al
    unsigned char m_051;  // +0x051 /* ? */; 1-byte store: mov byte ptr [esi + 0x51], al
    unsigned char pad_052[0x2];
    unsigned long m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], eax
    unsigned long m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], eax
    unsigned long m_05c;  // +0x05c init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x5c], 0xffffffff
    unsigned char pad_060[0x4];
    unsigned long m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], eax
    unsigned long m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], eax
};
