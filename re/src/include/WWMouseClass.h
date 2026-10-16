#pragma once
// ==========================================================================
// WWMouseClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WWMouseClass
// vtable       : 0x006a7b84  (17 slots)
// constructor  : 0x006727c0
// vtable store : 0x006728d7  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Mouse.h"

class WWMouseClass : public Mouse
{
public:
    // ---- vtable 0x006a7b84 (17 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00674130  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00672a60  // __thiscall
    virtual void vfunc_2();  // [2] 0x006740d0  // __cdecl, returns int
    virtual void vfunc_3();  // [3] 0x00673900  // __thiscall
    virtual void vfunc_4();  // [4] 0x006737a0  // __thiscall
    virtual void vfunc_5();  // [5] 0x00673ab0  // __thiscall
    virtual void vfunc_6();  // [6] 0x00673a10  // __thiscall
    virtual void vfunc_7();  // [7] 0x006740e0  // __cdecl
    virtual void vfunc_8(int a0, int a1, int a2, int a3);  // [8] 0x00673b60  // __stdcall
    virtual void vfunc_9(int a0);  // [9] 0x00673b70
    virtual void vfunc_10();  // [10] 0x00672a30  // __thiscall
    virtual void vfunc_11();  // [11] 0x006740f0  // __cdecl
    virtual void vfunc_12();  // [12] 0x00674100  // __cdecl
    virtual void vfunc_13(int a0);  // [13] 0x00674110  // __stdcall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x00673150  // __thiscall
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5);  // [15] 0x00673350  // __thiscall
    virtual void vfunc_16(int a0, int a1);  // [16] 0x00673b80  // __stdcall

    WWMouseClass();  // 0x006727c0
    virtual ~WWMouseClass();

    // ---- data members (offsets from ctor 0x006727c0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a7bcc; 4-byte store: mov dword ptr [esi], 0x6a7bcc
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], ebx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ebx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], ebx
    unsigned long m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned char m_014;  // +0x014 /* ? */; 1-byte store: mov byte ptr [esi + 0x14], bl
    unsigned char pad_015[0x3];
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], ebx
    unsigned long m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], ebx
    unsigned long m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], ecx
    unsigned long m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], edx
    unsigned char pad_028[0x10];
    unsigned long m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [esi + 0x38], ebx
    unsigned long m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], ebx
    unsigned long m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], ebx
    unsigned long m_044;  // +0x044 /* ? */; 4-byte store: mov dword ptr [esi + 0x44], edx
    unsigned char pad_048[0xc];
    unsigned long m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], ebx
    unsigned long m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], edx
    unsigned char pad_05c[0xc];
    unsigned long m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], ebx
    unsigned long m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], edx
    unsigned char pad_070[0xc];
    unsigned long m_07c;  // +0x07c /* ? */; 4-byte store: mov dword ptr [esi + 0x7c], edx
    unsigned char pad_080[0xc];
    unsigned long m_08c;  // +0x08c /* ? */; 4-byte store: mov dword ptr [esi + 0x8c], eax
    unsigned long m_090;  // +0x090 /* ? */; 4-byte store: mov dword ptr [esi + 0x90], ebx
};
