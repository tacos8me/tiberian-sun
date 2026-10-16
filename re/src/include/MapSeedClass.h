#pragma once
// ==========================================================================
// MapSeedClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MapSeedClass
// vtable       : 0x006a1e24  (5 slots)
// constructor  : 0x0052c500
// vtable store : 0x0052c54a  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "LoadOptionsClass.h"

class MapSeedClass : public LoadOptionsClass
{
public:
    // ---- vtable 0x006a1e24 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00540230  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0052d760  // __thiscall  // refs "Description"
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0052d4f0  // __thiscall  // refs "Description"
    virtual void vfunc_3(int a0);  // [3] 0x0052da20  // __stdcall
    virtual void vfunc_4(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);  // [4] 0x0052da30

    MapSeedClass();  // 0x0052c500
    virtual ~MapSeedClass();

    // ---- data members (offsets from ctor 0x0052c500) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a1e24; 4-byte store: mov dword ptr [ebx], 0x6a1e24
    unsigned char pad_004[0x4];
    unsigned long m_008;  // +0x008 init 0x6cd9cc; 4-byte store: mov dword ptr [ebx + 8], 0x6cd9cc
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [ebx + 0xc], ebp
    unsigned char pad_010[0x24];
    unsigned long m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [ebx + 0x34], eax
    unsigned long m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [ebx + 0x38], eax
    unsigned long RandomMap;  // +0x03c (INI key); 4-byte store: mov dword ptr [ebx + 0x3c], 1
    unsigned long m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [ebx + 0x40], eax
    unsigned long m_044;  // +0x044 init 0x2; 4-byte store: mov dword ptr [ebx + 0x44], 2
    unsigned long m_048;  // +0x048 /* ? */; 4-byte store: mov dword ptr [ebx + 0x48], eax
    unsigned long m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [ebx + 0x4c], eax
    unsigned long m_050;  // +0x050 /* ? */; 4-byte store: mov dword ptr [ebx + 0x50], eax
    unsigned long m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [ebx + 0x54], eax
    unsigned long m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [ebx + 0x58], eax
    unsigned long m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [ebx + 0x5c], eax
    unsigned long m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [ebx + 0x60], eax
    unsigned long m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [ebx + 0x64], eax
    unsigned long m_068;  // +0x068 init 0xffffffff; 4-byte store: mov dword ptr [ebx + 0x68], 0xffffffff
};
