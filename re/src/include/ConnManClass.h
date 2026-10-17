#pragma once
// ==========================================================================
// ConnManClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ConnManClass
// vtable       : 0x006a0b7c  (16 slots)
// constructor  : 0x004e7df0
// vtable store : 0x004e7e47  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ConnManClass
{
public:
    // ---- vtable 0x006a0b7c (16 slots) ----
    virtual void vfunc_1();  // [1] 0x006829a5  // __cdecl
    virtual void vfunc_2();  // [2] 0x006829a5  // __cdecl
    virtual void vfunc_3();  // [3] 0x006829a5  // __cdecl
    virtual void vfunc_4();  // [4] 0x006829a5  // __cdecl
    virtual void vfunc_5();  // [5] 0x006829a5  // __cdecl
    virtual void vfunc_6();  // [6] 0x006829a5  // __cdecl
    virtual void vfunc_7();  // [7] 0x006829a5  // __cdecl
    virtual void vfunc_8();  // [8] 0x006829a5  // __cdecl
    virtual void vfunc_9();  // [9] 0x006829a5  // __cdecl
    virtual void vfunc_10();  // [10] 0x006829a5  // __cdecl
    virtual void vfunc_11();  // [11] 0x006829a5  // __cdecl
    virtual void vfunc_12();  // [12] 0x006829a5  // __cdecl
    virtual void vfunc_13();  // [13] 0x006829a5  // __cdecl
    virtual void vfunc_14();  // [14] 0x006829a5  // __cdecl
    virtual void vfunc_15();  // [15] 0x006829a5  // __cdecl

    ConnManClass();  // 0x004e7df0
    virtual ~ConnManClass();

    // ---- data members (offsets from ctor 0x004e7df0) ----
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
    // (the store width and the recovered table disagree): +0x1a
    unsigned char m_004;  // +0x004 /* ? */; 1-byte store: mov byte ptr [esi + 4], 1
    unsigned char m_005;  // +0x005 /* ? */; 1-byte store: mov byte ptr [esi + 5], bl
    unsigned char pad_006[0x2];
    unsigned int m_008;  // +0x008 /* ? */; /* width ? */
    unsigned int m_00c;  // +0x00c /* ? */; /* width ? */
    unsigned int m_010;  // +0x010 init 0x1f4; /* width ? */
    unsigned int m_014;  // +0x014 /* ? */; /* width ? */
    unsigned int m_018;  // +0x018 /* ? */; /* width ? */
    unsigned int m_01c;  // +0x01c /* ? */; /* width ? */
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [esi + edx*4 + 0x20], eax
    unsigned int m_024;  // +0x024 init 0xbb8; /* width ? */
    unsigned int m_028;  // +0x028 init 0x4b0; /* width ? */
    unsigned int m_02c;  // +0x02c /* ? */; /* width ? */
    unsigned int m_030;  // +0x030 init 0x2b; /* width ? */
    unsigned int m_034;  // +0x034 /* ? */; /* width ? */
    unsigned int m_038;  // +0x038 /* ? */; /* width ? */
    unsigned int m_03c;  // +0x03c /* ? */; 4-byte store: mov dword ptr [esi + 0x3c], ebx
    unsigned int m_040;  // +0x040 /* ? */; 4-byte store: mov dword ptr [esi + 0x40], ebx
};
