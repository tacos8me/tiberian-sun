#pragma once
// ==========================================================================
// UDPInterfaceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class UDPInterfaceClass
// vtable       : 0x006a7a94  (21 slots)
// constructor  : 0x0066f6d0
// vtable store : 0x0066f73c  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "WinsockInterfaceClass.h"

class UDPInterfaceClass : public WinsockInterfaceClass
{
public:
    // ---- vtable 0x006a7a94 (21 slots) ----
    virtual void vfunc_1();  // [1] 0x0066eb20  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x0066ed50  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x0066ede0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0066fbe0  // __stdcall
    virtual void vfunc_5();  // [5] 0x0066ebb0  // __thiscall
    virtual void vfunc_6();  // [6] 0x0066ec00  // __thiscall
    virtual void vfunc_7();  // [7] 0x0066eb40  // __thiscall
    virtual void vfunc_8();  // [8] 0x0066eb90  // __thiscall
    virtual void vfunc_9(int a0);  // [9] 0x0066ef50  // __stdcall
    virtual void vfunc_10();  // [10] 0x0066efa0  // __thiscall
    virtual void vfunc_11(int a0);  // [11] 0x0066f8f0  // __thiscall
    virtual void vfunc_12();  // [12] 0x0066f890  // __thiscall
    virtual void vfunc_13();  // [13] 0x0066fef0  // __cdecl, returns int
    virtual void vfunc_14();  // [14] 0x0066ff00  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x0066f990  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2, int a3);  // [16] 0x0066fcd0  // __thiscall, returns int
    virtual void vfunc_17(int a0, int a1);  // [17] 0x0066f030  // __stdcall
    virtual void vfunc_18();  // [18] 0x0066ff10  // __cdecl
    virtual void vfunc_19(int a0);  // [19] 0x0066ff20  // __stdcall
    virtual void vfunc_20(int a0);  // [20] 0x0066e9a0  // __stdcall

    UDPInterfaceClass();  // 0x0066f6d0
    virtual ~UDPInterfaceClass();

    // ---- data members (offsets from ctor 0x0066f6d0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a7a94; 4-byte store: mov dword ptr [esi], 0x6a7a94
    unsigned char pad_004[0x444];
    unsigned long m_448;  // +0x448 /* ? */; 4-byte store: mov dword ptr [esi + 0x448], edx
    unsigned long m_44c;  // +0x44c /* ? */; 4-byte store: mov dword ptr [esi + 0x44c], eax
    unsigned long m_450;  // +0x450 /* ? */; 4-byte store: mov dword ptr [esi + 0x450], eax
    unsigned char m_454;  // +0x454 init 0x1; 1-byte store: mov byte ptr [esi + 0x454], 1
    unsigned char m_455;  // +0x455 /* ? */; 1-byte store: mov byte ptr [esi + 0x455], al
    unsigned char pad_456[0x2];
    unsigned long m_458;  // +0x458 /* ? */; 4-byte store: mov dword ptr [esi + 0x458], eax
    unsigned long m_45c;  // +0x45c /* ? */; 4-byte store: mov dword ptr [esi + 0x45c], ecx
    unsigned long m_460;  // +0x460 /* ? */; 4-byte store: mov dword ptr [esi + 0x460], edx
    unsigned long m_464;  // +0x464 /* ? */; 4-byte store: mov dword ptr [esi + 0x464], eax
    unsigned long m_468;  // +0x468 /* ? */; 4-byte store: mov dword ptr [esi + 0x468], eax
    unsigned char m_46c;  // +0x46c init 0x1; 1-byte store: mov byte ptr [esi + 0x46c], 1
    unsigned char m_46d;  // +0x46d /* ? */; 1-byte store: mov byte ptr [esi + 0x46d], al
    unsigned char pad_46e[0x2];
    unsigned long m_470;  // +0x470 /* ? */; 4-byte store: mov dword ptr [esi + 0x470], eax
    unsigned long m_474;  // +0x474 /* ? */; 4-byte store: mov dword ptr [esi + 0x474], ecx
};
