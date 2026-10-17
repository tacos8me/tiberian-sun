#pragma once
// ==========================================================================
// WinsockInterfaceClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WinsockInterfaceClass
// vtable       : 0x006a79fc  (21 slots)
// constructor  : 0x0066ea10
// vtable store : 0x0066ea58  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class WinsockInterfaceClass
{
public:
    // ---- vtable 0x006a79fc (21 slots) ----
    virtual void vfunc_1();  // [1] 0x0066eb20  // __thiscall, returns ?
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x0066ed50  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x0066ede0  // __thiscall, returns ?
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0066eea0  // __thiscall, returns ?
    virtual void vfunc_5();  // [5] 0x0066ebb0  // __thiscall, returns ?
    virtual void vfunc_6();  // [6] 0x0066ec00  // __thiscall, returns ?
    virtual void vfunc_7();  // [7] 0x0066eb40  // __thiscall, returns int
    virtual void vfunc_8();  // [8] 0x0066eb90  // __thiscall, returns ?
    virtual void vfunc_9(int a0);  // [9] 0x0066ef50  // __stdcall, returns void*
    virtual void vfunc_10();  // [10] 0x0066efa0  // __thiscall, returns char
    virtual void vfunc_11(int a0);  // [11] 0x0066e960  // __stdcall, returns void
    virtual void vfunc_12();  // [12] 0x0066e970  // __cdecl, returns void
    virtual void vfunc_13();  // [13] 0x0066f080  // __cdecl, returns int
    virtual void vfunc_14();  // [14] 0x0066f090  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x0066f0a0  // __stdcall, returns int
    virtual void vfunc_16(int a0, int a1, int a2, int a3);  // [16] 0x0066f0b0  // __stdcall, returns int
    virtual void vfunc_17(int a0, int a1);  // [17] 0x0066f030  // __stdcall, returns int
    virtual void vfunc_18();  // [18] 0x0066e980  // __cdecl, returns int
    virtual void vfunc_19(int a0);  // [19] 0x0066e990  // __stdcall, returns int
    virtual void vfunc_20(int a0);  // [20] 0x0066e9a0  // __thiscall, returns ?

    WinsockInterfaceClass();  // 0x0066ea10
    virtual ~WinsockInterfaceClass();

    // ---- data members (offsets from ctor 0x0066ea10) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    // 1 recovered offset(s) fall inside a wider declaration above
    // (the store width and the recovered table disagree): +0x11
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], edx
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned int m_010;  // +0x010 init 0x1; 4-byte store: mov dword ptr [esi + 0x10], ecx
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], ecx
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], edx
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + 0x20], eax
    unsigned int m_024;  // +0x024 /* ? */; 4-byte store: mov dword ptr [esi + 0x24], eax
    unsigned char m_028;  // +0x028 init 0x1; 1-byte store: mov byte ptr [esi + 0x28], 1
    unsigned char m_029;  // +0x029 /* ? */; 1-byte store: mov byte ptr [esi + 0x29], al
    unsigned char pad_02a[0x2];
    unsigned int m_02c;  // +0x02c /* ? */; 4-byte store: mov dword ptr [esi + 0x2c], eax
    unsigned int m_030;  // +0x030 /* ? */; 4-byte store: mov dword ptr [esi + 0x30], ecx
    unsigned char m_034;  // +0x034 /* ? */; 1-byte store: mov byte ptr [esi + 0x34], al
    unsigned char pad_035[0x3];
    unsigned int m_038;  // +0x038 /* ? */; 4-byte store: mov dword ptr [esi + 0x38], eax
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], eax
};
