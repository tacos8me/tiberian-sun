#pragma once
// ==========================================================================
// WinModemClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WinModemClass
// vtable       : 0x006a7620  (10 slots)
// constructor  : 0x0065791a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class WinModemClass
{
public:
    // ---- vtable 0x006a7620 (10 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4);  // [1] 0x00657c00  // __thiscall
    virtual void vfunc_2(int a0);  // [2] 0x00658050  // __stdcall
    virtual void vfunc_3(int a0);  // [3] 0x00658060  // __cdecl
    virtual void vfunc_4();  // [4] 0x006580b0
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [5] 0x00658480  // __thiscall
    virtual void vfunc_6(int a0);  // [6] 0x006586f0  // __thiscall
    virtual void vfunc_7(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14);  // [7] 0x006588b0  // __thiscall
    virtual void vfunc_8(int a0);  // [8] 0x00658c40  // __stdcall
    virtual void vfunc_9(int a0);  // [9] 0x00658c50  // __stdcall

    WinModemClass();  // 0x0065791a
    virtual ~WinModemClass();

    // ---- data members (offsets from ctor 0x00657907) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a7620
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0x8];
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x28];
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // init 0xffffffff
    unsigned char pad_060[0x4];
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
};
