#pragma once
// ==========================================================================
// com_error.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class _com_error
// vtable       : 0x006a8c10  (1 slots)
// constructor  : 0x00694050
// vtable store : 0x00694064  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class com_error
{
public:
    // ---- vtable 0x006a8c10 (1 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00694082  // __thiscall

    com_error();  // 0x00694050
    virtual ~com_error();

    // ---- data members (offsets from ctor 0x00694050) ----
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
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned int m_00c;  // +0x00c zero-initialised; 4-byte store: and dword ptr [esi + 0xc], 0
};
