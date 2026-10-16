#pragma once
// ==========================================================================
// CommBufferClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CommBufferClass
// vtable       : 0x0069b094  (1 slots)
// constructor  : 0x0045d980
// vtable store : 0x0045d99e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class CommBufferClass
{
public:
    // ---- vtable 0x0069b094 (1 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0045ea80  // __thiscall

    CommBufferClass();  // 0x0045d980
    virtual ~CommBufferClass();

    // ---- data members (offsets from ctor 0x0045d980) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x69b094; 4-byte store: mov dword ptr [esi], 0x69b094
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], ebx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], edi
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], ebp
    unsigned long m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned char pad_014[0x10];
    unsigned long m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], eax
    unsigned char pad_028[0x8];
    unsigned long m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], eax
    unsigned long m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], eax
    unsigned char pad_038[0x8];
    unsigned long m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], eax
};
