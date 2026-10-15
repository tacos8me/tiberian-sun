#pragma once
// ==========================================================================
// FileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FileClass
// vtable       : 0x006a39a0  (17 slots)
// constructor  : 0x005a5a0d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class FileClass
{
public:
    // ---- vtable 0x006a39a0 (17 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005a5d50  // __thiscall
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
    virtual void vfunc_14();  // [14] 0x005a5d30  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x005a5d40  // __stdcall
    virtual void vfunc_16();  // [16] 0x006829a5  // __cdecl

    FileClass();  // 0x005a5a0d
    virtual ~FileClass();

    // ---- data members (offsets from ctor 0x005a59da) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3958
    unsigned long m_004;  // zero-initialised
    unsigned char pad_008[0x10];
    unsigned long m_018;  // zero-initialised
};
