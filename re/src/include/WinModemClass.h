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
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4);  // [1] 0x00657c00  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x00658050  // __thiscall, returns ?
    virtual void vfunc_3();  // [3] 0x00658060  // __cdecl, returns ?
    virtual void vfunc_4(int a0);  // [4] 0x006580b0  // __thiscall, returns ?
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);  // [5] 0x00658480  // __thiscall, returns int
    virtual void vfunc_6(int a0);  // [6] 0x006586f0  // __thiscall, returns void*
    virtual void vfunc_7(int a0, int a1, int a2, int a3);  // [7] 0x006588b0  // __thiscall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x00658c40  // __thiscall, returns ?
    virtual void vfunc_9(int a0);  // [9] 0x00658c50  // __thiscall, returns ?

    WinModemClass();  // 0x00657910
    virtual ~WinModemClass();

    // ---- data members (offsets from ctor 0x00657910) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned int m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned char pad_01c[0x8];
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], eax
    unsigned char pad_028[0x28];
    unsigned char m_050;  // +0x050 /* ? */; 1-byte store: mov byte ptr [esi + 0x50], al
    unsigned char m_051;  // +0x051 /* ? */; 1-byte store: mov byte ptr [esi + 0x51], al
    unsigned char pad_052[0x2];
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], eax
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], eax
    unsigned int m_05c;  // +0x05c init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x5c], 0xffffffff
    unsigned char pad_060[0x4];
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], eax
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], eax
};
