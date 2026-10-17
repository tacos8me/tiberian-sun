#pragma once
// ==========================================================================
// FileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FileClass
// vtable       : 0x006a39a0  (17 slots)
// constructor  : 0x005a59e0
// vtable store : 0x005a5a0d  (inside the ctor)
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

    FileClass();  // 0x005a59e0
    virtual ~FileClass();

    // ---- data members (offsets from ctor 0x005a59e0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [esi + 4], 0
    unsigned int m_008;  // +0x008 /* ? */; /* width ? */
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], 0
    unsigned int m_010;  // +0x010 init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x10], 0
    unsigned int m_014;  // +0x014 zero-initialised; 4-byte store: mov dword ptr [esi + 0x14], eax
    unsigned char m_018;  // +0x018 zero-initialised; 1-byte store: mov byte ptr [esi + 0x18], 0
    unsigned char m_019;  // +0x019 zero-initialised; 1-byte store: mov byte ptr [esi + 0x19], 0
};
