#pragma once
// ==========================================================================
// LightConvertClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LightConvertClass
// vtable       : 0x006a163c  (2 slots)
// constructor  : 0x004f6250
// vtable store : 0x004f62d5  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConvertClass.h"

class LightConvertClass : public ConvertClass
{
public:
    // ---- vtable 0x006a163c (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3);  // [1] 0x004f63e0  // __thiscall

    LightConvertClass();  // 0x004f6250
    virtual ~LightConvertClass();

    // ---- data members (offsets from ctor 0x004f6250) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a163c; 4-byte store: mov dword ptr [esi], 0x6a163c
    unsigned char pad_004[0x184];
    unsigned long m_188;  // +0x188 /* ? */; 4-byte store: mov dword ptr [esi + 0x188], ebx
    unsigned long m_18c;  // +0x18c /* ? */; 4-byte store: mov dword ptr [esi + 0x18c], edi
    unsigned long m_190;  // +0x190 /* ? */; 4-byte store: mov dword ptr [esi + 0x190], eax
    unsigned long m_194;  // +0x194 zero-initialised; 4-byte store: mov dword ptr [esi + 0x194], 0
    unsigned long m_198;  // +0x198 /* ? */; 4-byte store: mov dword ptr [esi + 0x198], edi
    unsigned long m_19c;  // +0x19c /* ? */; 4-byte store: mov dword ptr [esi + 0x19c], ebp
    unsigned long m_1a0;  // +0x1a0 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a0], ebx
    unsigned long m_1a4;  // +0x1a4 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a4], edi
    unsigned long m_1a8;  // +0x1a8 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a8], ebp
    unsigned long m_1ac;  // +0x1ac /* ? */; 4-byte store: mov dword ptr [esi + 0x1ac], ebx
    unsigned char m_1b0;  // +0x1b0 zero-initialised; 1-byte store: mov byte ptr [esi + 0x1b0], 0
};
