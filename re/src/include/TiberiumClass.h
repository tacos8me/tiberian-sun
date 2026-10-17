#pragma once
// ==========================================================================
// TiberiumClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TiberiumClass
// vtable       : 0x006a6248  (27 slots)
// constructor  : 0x006281e0
// vtable store : 0x006282b3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class TiberiumClass : public AbstractTypeClass
{
public:
    // ---- vtable 0x006a6248 (27 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00628900  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x00628b80  // __stdcall
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00628b50  // __stdcall, returns int
    virtual void vfunc_8(int a0);  // [8] 0x0062a100  // __thiscall
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00628bf0  // __stdcall
    virtual void vfunc_11();  // [11] 0x0062a0e0  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x0062a0d0  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00628890  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void vfunc_16();  // [16] 0x0062a0f0  // __cdecl
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl
    virtual void vfunc_24(int a0);  // [24] 0x00406010  // __stdcall
    virtual void vfunc_25(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [25] 0x00628540  // __thiscall  // refs "Spread"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"

    TiberiumClass();  // 0x006281e0
    // The destructor is declared non-virtual here: slot 0 holds a stub
    // shared with 66 other classes (0x00405790), not the destructor.
    ~TiberiumClass();

    // ---- data members (offsets from ctor 0x006281e0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_008[0x58];
    unsigned int m_060;  // +0x060 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x60], 0xffffffff
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], ebx
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], ecx
    unsigned int m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], eax
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], ebx
    unsigned char pad_074[0x4];
    unsigned int m_078;  // +0x078 /* ? */; 4-byte store: mov dword ptr [esi + 0x78], ecx
    unsigned int m_07c;  // +0x07c /* ? */; 4-byte store: mov dword ptr [esi + 0x7c], eax
    unsigned int Value;  // +0x080 (INI key); 4-byte store: mov dword ptr [esi + 0x80], ebx
    unsigned int m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [esi + 0x84], ebx
    unsigned int m_088;  // +0x088 /* ? */; 4-byte store: mov dword ptr [esi + 0x88], ebx
    unsigned char pad_08c[0x1c];
    unsigned int m_0a8;  // +0x0a8 /* ? */; 4-byte store: mov dword ptr [esi + 0xa8], ebx
    unsigned int m_0ac;  // +0x0ac /* ? */; 4-byte store: mov dword ptr [esi + 0xac], ebx
    unsigned int m_0b0;  // +0x0b0 /* ? */; 4-byte store: mov dword ptr [esi + 0xb0], ebx
    unsigned int m_0b4;  // +0x0b4 /* ? */; 4-byte store: mov dword ptr [esi + 0xb4], ebx
    unsigned int m_0b8;  // +0x0b8 /* ? */; 4-byte store: mov dword ptr [esi + 0xb8], ebx
    unsigned int m_0bc;  // +0x0bc /* ? */; 4-byte store: mov dword ptr [esi + 0xbc], ebx
    unsigned int m_0c0;  // +0x0c0 /* ? */; 4-byte store: mov dword ptr [esi + 0xc0], ebx
    unsigned int m_0c4;  // +0x0c4 /* ? */; 4-byte store: mov dword ptr [esi + 0xc4], ebx
    unsigned int m_0c8;  // +0x0c8 /* ? */; 4-byte store: mov dword ptr [esi + 0xc8], ecx
    unsigned char pad_0cc[0x4];
    unsigned int m_0d0;  // +0x0d0 /* ? */; 4-byte store: mov dword ptr [esi + 0xd0], ebx
    unsigned int m_0d4;  // +0x0d4 /* ? */; 4-byte store: mov dword ptr [esi + 0xd4], ebx
    unsigned int m_0d8;  // +0x0d8 /* ? */; 4-byte store: mov dword ptr [esi + 0xd8], ebx
    unsigned int m_0dc;  // +0x0dc /* ? */; 4-byte store: mov dword ptr [esi + 0xdc], ebx
    unsigned int m_0e0;  // +0x0e0 /* ? */; 4-byte store: mov dword ptr [esi + 0xe0], ebx
    unsigned int m_0e4;  // +0x0e4 /* ? */; 4-byte store: mov dword ptr [esi + 0xe4], edx
    unsigned char pad_0e8[0x4];
    unsigned int m_0ec;  // +0x0ec /* ? */; 4-byte store: mov dword ptr [esi + 0xec], ebx
};
