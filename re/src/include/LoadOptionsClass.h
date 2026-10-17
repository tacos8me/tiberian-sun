#pragma once
// ==========================================================================
// LoadOptionsClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LoadOptionsClass
// vtable       : 0x006a1814  (5 slots)
// constructor  : 0x004f8000
// vtable store : 0x004f8032  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class LoadOptionsClass
{
public:
    // ---- vtable 0x006a1814 (5 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004f9100  // __stdcall, returns ?
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f9150  // __stdcall, returns ?
    virtual void vfunc_3(int a0);  // [3] 0x004f91a0  // __stdcall, returns ?
    virtual void vfunc_4(int a0, int a1);  // [4] 0x004f91c0  // __stdcall, returns char

    LoadOptionsClass();  // 0x004f8000
    virtual ~LoadOptionsClass();

    // ---- data members (offsets from ctor 0x004f8000) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned int m_008;  // +0x008 init 0x6cc528; 4-byte store: mov dword ptr [esi + 8], 0x6cc528
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned int m_010;  // +0x010 init 0x800; 4-byte store: mov dword ptr [esi + 0x10], 0x800
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], eax
};
