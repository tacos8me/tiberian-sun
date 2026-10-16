#pragma once
// ==========================================================================
// RAMFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RAMFileClass
// vtable       : 0x006a3958  (17 slots)
// constructor  : 0x005a5990
// vtable store : 0x005a59ba  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FileClass.h"

class RAMFileClass : public FileClass
{
public:
    // ---- vtable 0x006a3958 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x005a5c90  // __cdecl, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005a5ca0  // __stdcall
    virtual void vfunc_3();  // [3] 0x005a5a20  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a5a40  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005a5a60  // __stdcall
    virtual void vfunc_6();  // [6] 0x005a5a70  // __cdecl
    virtual void vfunc_7(int a0);  // [7] 0x005a5a90  // __thiscall
    virtual void vfunc_8(int a0, int a1);  // [8] 0x005a5a80  // __stdcall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x005a5ae0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x005a5b60  // __thiscall
    virtual void vfunc_11();  // [11] 0x005a5be0  // __cdecl
    virtual void vfunc_12(int a0, int a1);  // [12] 0x005a5bf0  // __thiscall
    virtual void vfunc_13();  // [13] 0x005a5c80  // __cdecl
    virtual void vfunc_14();  // [14] 0x005a5cb0  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x005a5cc0  // __stdcall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a5cd0  // __stdcall

    RAMFileClass();  // 0x005a5990
    virtual ~RAMFileClass();

    // ---- data members (offsets from ctor 0x005a5990) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a3958; 4-byte store: mov dword ptr [esi], 0x6a3958
    unsigned long m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], ecx
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned long m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], edx
    unsigned long m_014;  // +0x014 init 0x1; 4-byte store: mov dword ptr [esi + 0x14], 1
    unsigned char m_018;  // +0x018 /* ? */; 1-byte store: mov byte ptr [esi + 0x18], dl
    unsigned char m_019;  // +0x019 /* ? */; 1-byte store: mov byte ptr [esi + 0x19], dl
};
