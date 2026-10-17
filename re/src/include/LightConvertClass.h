#pragma once
// ==========================================================================
// LightConvertClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LightConvertClass
// vtable       : 0x006a163c  (2 slots)
// constructor  : 0x004f6250
// vtable store : 0x004f62d5  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConvertClass.h"

class LightConvertClass : public ConvertClass
{
public:
    // ---- vtable 0x006a163c (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3);  // [1] 0x004f63e0  // __thiscall, returns char

    LightConvertClass();  // 0x004f6250
    virtual ~LightConvertClass();

    // ---- data members (offsets from ctor 0x004f6250) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x188: the compiled end of the
    // base sub-object region. Evidence: ConvertClass at +0x0 (ends +0x188).
    // 1 recovered offset(s) below +0x188 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_188;  // +0x188 /* ? */; 4-byte store: mov dword ptr [esi + 0x188], ebx
    unsigned int m_18c;  // +0x18c /* ? */; 4-byte store: mov dword ptr [esi + 0x18c], edi
    unsigned int m_190;  // +0x190 /* ? */; 4-byte store: mov dword ptr [esi + 0x190], eax
    unsigned int m_194;  // +0x194 zero-initialised; 4-byte store: mov dword ptr [esi + 0x194], 0
    unsigned int m_198;  // +0x198 /* ? */; 4-byte store: mov dword ptr [esi + 0x198], edi
    unsigned int m_19c;  // +0x19c /* ? */; 4-byte store: mov dword ptr [esi + 0x19c], ebp
    unsigned int m_1a0;  // +0x1a0 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a0], ebx
    unsigned int m_1a4;  // +0x1a4 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a4], edi
    unsigned int m_1a8;  // +0x1a8 /* ? */; 4-byte store: mov dword ptr [esi + 0x1a8], ebp
    unsigned int m_1ac;  // +0x1ac /* ? */; 4-byte store: mov dword ptr [esi + 0x1ac], ebx
    unsigned char m_1b0;  // +0x1b0 zero-initialised; 1-byte store: mov byte ptr [esi + 0x1b0], 0
};
