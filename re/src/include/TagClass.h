#pragma once
// ==========================================================================
// TagClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TagClass
// vtable       : 0x006a53f0  (24 slots)
// constructor  : 0x006031e0
// vtable store : 0x00603209  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class TagClass : public AbstractClass
{
public:
    // ---- vtable 0x006a53f0 (24 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00603a00  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1);  // [5] 0x00603a90  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00603af0  // __stdcall, returns int
    virtual void GetType(int a0, int a1);  // [7] 0x00405910  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x00603bf0  // __thiscall
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00603970  // __thiscall
    virtual void vfunc_11();  // [11] 0x00603be0  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00603bd0  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00603a40  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void Compute_CRC();  // [16] 0x004059f0  // __cdecl, returns int
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl

    TagClass();  // 0x006031e0
    virtual ~TagClass();

    // ---- data members (offsets from ctor 0x006031e0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a53f0; 4-byte store: mov dword ptr [esi], 0x6a53f0
    unsigned long m_004;  // +0x004 init 0x6a53d4; 4-byte store: mov dword ptr [esi + 4], 0x6a53d4
    unsigned char pad_008[0xc];
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], edi
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], ebx
    unsigned long m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], ebx
    unsigned long m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], eax
    unsigned char m_024;  // +0x024 /* ? */; 1-byte store: mov byte ptr [esi + 0x24], bl
    unsigned char m_025;  // +0x025 /* ? */; 1-byte store: mov byte ptr [esi + 0x25], bl
};
