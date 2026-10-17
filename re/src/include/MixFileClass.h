#pragma once
// ==========================================================================
// MixFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MixFileClass
// vtable       : 0x006a2318  (1 slots)
// constructor  : 0x00547290
// vtable store : 0x005472cb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class MixFileClass
{
public:
    // ---- vtable 0x006a2318 (1 slots) ----

    MixFileClass();  // 0x00547290
    virtual ~MixFileClass();

    // ---- data members (offsets from ctor 0x00547290) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], ebx
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ebx
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], ebx
    unsigned char m_010;  // +0x010 /* ? */; 1-byte store: mov byte ptr [esi + 0x10], bl
    unsigned char m_011;  // +0x011 /* ? */; 1-byte store: mov byte ptr [esi + 0x11], bl
    unsigned char m_012;  // +0x012 /* ? */; 1-byte store: mov byte ptr [esi + 0x12], bl
    unsigned char pad_013[0x1];
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], ebx
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], ebx
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], ebx
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], ebx
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], ebx
};
