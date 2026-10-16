#pragma once
// ==========================================================================
// OverlayTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class OverlayTypeClass
// vtable       : 0x006a2d58  (41 slots)
// constructor  : 0x00576bf0
// vtable store : 0x00576c81  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class OverlayTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a2d58 (41 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00405790  // __stdcall, returns int
    virtual void Init(int a0);  // [1] 0x00405830  // __stdcall, returns int
    virtual void Shutdown(int a0);  // [2] 0x00405840  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00577500  // __stdcall, returns int
    virtual void Release(int a0);  // [4] 0x00405990  // __stdcall
    virtual void vfunc_5(int a0, int a1, int a2);  // [5] 0x005773f0
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005774e0  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x00572ba0  // __stdcall, returns int
    virtual void Set_ID();  // [9] 0x004059b0  // __cdecl
    virtual void Get_RTTI(int a0, int a1);  // [10] 0x004059c0  // __stdcall
    virtual void vfunc_11();  // [11] 0x00577820  // __cdecl, returns int
    virtual void vfunc_12();  // [12] 0x00577830  // __cdecl, returns int
    virtual void vfunc_13(int a0);  // [13] 0x00577350  // __thiscall
    virtual void Save();  // [14] 0x004059d0  // __cdecl
    virtual void GetSizeMax();  // [15] 0x004059e0  // __cdecl, returns int
    virtual void vfunc_16();  // [16] 0x00577840  // __cdecl
    virtual void vfunc_17();  // [17] 0x00405970  // __cdecl
    virtual void vfunc_18(int a0);  // [18] 0x00405a00  // __stdcall
    virtual void vfunc_19(int a0);  // [19] 0x00405a30  // __stdcall
    virtual void vfunc_20();  // [20] 0x00405a60  // __cdecl
    virtual void vfunc_21();  // [21] 0x00405a70  // __cdecl
    virtual void vfunc_22(int a0);  // [22] 0x00405a80  // __stdcall
    virtual void vfunc_23();  // [23] 0x00405ab0  // __cdecl
    virtual void vfunc_24(int a0);  // [24] 0x00406010  // __stdcall
    virtual void vfunc_25(int a0, int a1);  // [25] 0x00577090  // __thiscall  // refs "Land"
    virtual void Set_ID(int a0);  // [26] 0x00405fb0  // __thiscall  // refs "Name"
    virtual void vfunc_27(int a0, int a1);  // [27] 0x00577330  // __stdcall
    virtual void vfunc_28();  // [28] 0x00418f40  // __cdecl
    virtual void vfunc_29();  // [29] 0x005715e0  // __cdecl, returns int
    virtual void vfunc_30(int a0);  // [30] 0x005715f0  // __thiscall
    virtual void vfunc_31(int a0);  // [31] 0x00571610  // __thiscall
    virtual void vfunc_32(int a0, int a1);  // [32] 0x00576eb0  // __thiscall
    virtual void vfunc_33(int a0);  // [33] 0x00571640  // __stdcall, returns int
    virtual void vfunc_34();  // [34] 0x00571650  // __cdecl, returns int
    virtual void vfunc_35(int a0);  // [35] 0x00576ee0  // __thiscall
    virtual void vfunc_36(int a0);  // [36] 0x00576e40  // __stdcall, returns int
    virtual void vfunc_37(int a0, int a1, int a2, int a3, int a4, int a5);  // [37] 0x00571710  // __thiscall
    virtual void vfunc_38();  // [38] 0x00571660  // __cdecl, returns int
    virtual void vfunc_39(int a0, int a1);  // [39] 0x00577720  // __thiscall
    virtual void vfunc_40(int a0, int a1, int a2);  // [40] 0x00576f10  // __stdcall

    OverlayTypeClass();  // 0x00576bf0
    virtual ~OverlayTypeClass();

    // ---- data members (offsets from ctor 0x00576bf0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a2d58; 4-byte store: mov dword ptr [esi], 0x6a2d58
    unsigned char pad_004[0xc4];
    unsigned char m_0c8;  // +0x0c8 init 0x1; 1-byte store: mov byte ptr [esi + 0xc8], 1
    unsigned char m_0c9;  // +0x0c9 /* ? */; 1-byte store: mov byte ptr [esi + 0xc9], bl
    unsigned char pad_0ca[0x1];
    unsigned char m_0cb;  // +0x0cb init 0x1; 1-byte store: mov byte ptr [esi + 0xcb], 1
    unsigned char pad_0cc[0x2];
    unsigned char m_0ce;  // +0x0ce /* ? */; 1-byte store: mov byte ptr [esi + 0xce], bl
    unsigned char pad_0cf[0x55];
    unsigned long m_124;  // +0x124 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x124], 0xffffffff
    unsigned long m_128;  // +0x128 /* ? */; 4-byte store: mov dword ptr [esi + 0x128], ebx
    unsigned long m_12c;  // +0x12c /* ? */; 4-byte store: mov dword ptr [esi + 0x12c], ebx
    unsigned long m_130;  // +0x130 /* ? */; 4-byte store: mov dword ptr [esi + 0x130], eax
    unsigned long m_134;  // +0x134 /* ? */; 4-byte store: mov dword ptr [esi + 0x134], eax
    unsigned char Wall;  // +0x138 (INI key); 1-byte store: mov byte ptr [esi + 0x138], bl
    unsigned char High;  // +0x139 (INI key); 1-byte store: mov byte ptr [esi + 0x139], bl
    unsigned char m_13a;  // +0x13a /* ? */; 1-byte store: mov byte ptr [esi + 0x13a], bl
    unsigned char Crate;  // +0x13b (INI key); 1-byte store: mov byte ptr [esi + 0x13b], bl
    unsigned char CrateTrigger;  // +0x13c (INI key); 1-byte store: mov byte ptr [esi + 0x13c], bl
    unsigned char m_13d;  // +0x13d /* ? */; 1-byte store: mov byte ptr [esi + 0x13d], al
    unsigned char m_13e;  // +0x13e /* ? */; 1-byte store: mov byte ptr [esi + 0x13e], bl
    unsigned char m_13f;  // +0x13f /* ? */; 1-byte store: mov byte ptr [esi + 0x13f], bl
    unsigned char m_140;  // +0x140 /* ? */; 1-byte store: mov byte ptr [esi + 0x140], bl
    unsigned char m_141;  // +0x141 /* ? */; 1-byte store: mov byte ptr [esi + 0x141], bl
    unsigned char m_142;  // +0x142 /* ? */; 1-byte store: mov byte ptr [esi + 0x142], bl
    unsigned char Overrides;  // +0x143 (INI key); 1-byte store: mov byte ptr [esi + 0x143], bl
    unsigned char m_144;  // +0x144 /* ? */; 1-byte store: mov byte ptr [esi + 0x144], al
    unsigned char m_145;  // +0x145 /* ? */; 1-byte store: mov byte ptr [esi + 0x145], bl
};
