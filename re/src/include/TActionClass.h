#pragma once
// ==========================================================================
// TActionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TActionClass
// vtable       : 0x006a5364  (24 slots)
// constructor  : 0x005fddf0
// vtable store : 0x005fde22  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class TActionClass : public AbstractClass
{
public:
    // ---- vtable 0x006a5364 (24 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00602760  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x006027a0  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00602810  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x005fe060  // __stdcall
    virtual void vfunc_11();  // [11] 0x00602990  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00602980  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00602830  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void vfunc_16();  // [16] 0x006029a0  // __cdecl
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl

    TActionClass();  // 0x005fddf0
    virtual ~TActionClass();

    // ---- data members (offsets from ctor 0x005fddf0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a5364; 4-byte store: mov dword ptr [esi], 0x6a5364
    unsigned long m_004;  // +0x004 init 0x6a5348; 4-byte store: mov dword ptr [esi + 4], 0x6a5348
    unsigned char pad_008[0xc];
    unsigned long m_014;  // +0x014 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x14], 0xffffffff
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], ebx
    unsigned long m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], ebx
    unsigned long m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], ebx
    unsigned long m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], ebx
    unsigned long m_028;  // +0x028 /* ? */; 4-byte store: mov dword ptr [esi + 0x28], ebx
    unsigned long m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], ebx
    unsigned long m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], ebx
    unsigned long m_034;  // +0x034 /* ? */; 4-byte store: mov dword ptr [esi + 0x34], ebx
    unsigned long m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [esi + 0x38], ebx
    unsigned long m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], ebx
    unsigned long m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], ebx
};
