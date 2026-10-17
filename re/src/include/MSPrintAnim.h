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
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: MSAnim at +0x0 (ends +0x4).
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ecx
    unsigned char m_00c;  // +0x00c init 0x1; 1-byte store: mov byte ptr [esi + 0xc], 1
    unsigned char pad_00d[0x3];
    unsigned int m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], edx
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], edi
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], eax
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], edi
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], edi
    unsigned int m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], edi
    unsigned int m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], edx
    unsigned char m_030;  // +0x030 /* ? */; 1-byte store: mov byte ptr [esi + 0x30], al
    unsigned char pad_031[0x3];
    unsigned int m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], ecx
};
