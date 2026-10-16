#pragma once
// ==========================================================================
// INIClass_INISection.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class INIClass::INISection
// vtable       : 0x006a0410  (1 slots)
// constructor  : 0x004d4ad0
// vtable store : 0x004d4b1e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "INIClass.h"
#include "INIClass.h"

class INISection;  // RTTI formal param, no type evidence
class INIClass_INISection : public Node<INIClass::INISection*, INIClass>
{
public:
    // ---- vtable 0x006a0410 (1 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004d7f80  // __thiscall

    INIClass_INISection();  // 0x004d4ad0
    virtual ~INIClass_INISection();

    // ---- data members (offsets from ctor 0x004d4ad0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a0418; 4-byte store: mov dword ptr [ebx], 0x6a0418
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [ebx + 4], edi
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [ebx + 8], edi
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [ebx + 0xc], eax
    unsigned char pad_010[0x2c];
    unsigned long m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [ebx + 0x3c], ecx
};
