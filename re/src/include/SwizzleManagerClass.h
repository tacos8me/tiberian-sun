#pragma once
// ==========================================================================
// SwizzleManagerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SwizzleManagerClass
// vtable       : 0x006a5090  (10 slots)
// constructor  : 0x005f3510
// vtable store : 0x005f356e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class SwizzleManagerClass : public ISwizzle
{
public:
    // ---- vtable 0x006a5090 (10 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005f38d0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005f3950  // __stdcall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005f3960  // __stdcall, returns int
    virtual void vfunc_3(int a0);  // [3] 0x005f36d0  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005f36e0  // __stdcall, returns int
    virtual void vfunc_5(int a0, int a1, int a2);  // [5] 0x005f3930  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005f3760  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1, int a2);  // [7] 0x005f3970  // __stdcall, returns int
    virtual void vfunc_8(int a0, int a1, int a2, int a3);  // [8] 0x005f3980  // __stdcall, returns int
    virtual void vfunc_9(int a0, int a1);  // [9] 0x005f38b0  // __stdcall, returns int

    SwizzleManagerClass();  // 0x005f3510
    virtual ~SwizzleManagerClass();

    // ---- data members (offsets from ctor 0x005f3620) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    unsigned long m_000;  // +0x000 init 0x6a5090; 4-byte store: mov dword ptr [eax], 0x6a5090
    unsigned long m_004;  // +0x004 init 0x6a50bc; 4-byte store: mov dword ptr [eax + 4], 0x6a50bc
    unsigned long m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx
    unsigned long m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [eax + 0xc], ecx
    unsigned char m_010;  // +0x010 init 0x1; 1-byte store: mov byte ptr [eax + 0x10], 1
    unsigned char m_011;  // +0x011 /* ? */; 1-byte store: mov byte ptr [eax + 0x11], cl
    unsigned char pad_012[0x2];
    unsigned long m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [eax + 0x14], ecx
    unsigned long m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [eax + 0x18], edx
    unsigned long m_01c;  // +0x01c init 0x6a50bc; 4-byte store: mov dword ptr [eax + 0x1c], 0x6a50bc
    unsigned long m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [eax + 0x20], ecx
    unsigned long m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [eax + 0x24], ecx
    unsigned char m_028;  // +0x028 init 0x1; 1-byte store: mov byte ptr [eax + 0x28], 1
    unsigned char m_029;  // +0x029 /* ? */; 1-byte store: mov byte ptr [eax + 0x29], cl
    unsigned char pad_02a[0x2];
    unsigned long m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [eax + 0x2c], ecx
    unsigned long m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [eax + 0x30], edx
};
