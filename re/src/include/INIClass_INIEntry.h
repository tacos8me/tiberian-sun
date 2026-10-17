#pragma once
// ==========================================================================
// INIClass_INIEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class INIClass::INIEntry
// vtable       : 0x006a0408  (1 slots)
// constructor  : 0x004d4cef
// vtable store : 0x004d4d82  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class INIClass_INIEntry
{
public:
    // ---- vtable 0x006a0408 (1 slots) ----

    INIClass_INIEntry();  // 0x004d4cef
    virtual ~INIClass_INIEntry();

    // ---- data members (offsets from ctor 0x004d4cef) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], ecx
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ecx
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned int m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], edx
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], ebx
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], edi
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], edx
};
