#pragma once
// ==========================================================================
// BaseClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BaseClass
// vtable       : 0x00699db8  (3 slots)
// constructor  : 0x0041e8c0
// vtable store : 0x0041e943  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class BaseClass
{
public:
    // ---- vtable 0x00699db8 (3 slots) ----
    virtual void vfunc_0(int a0, int a1);  // [0] 0x0041ef90  // __stdcall, returns int
    virtual void vfunc_1(int a0, int a1);  // [1] 0x0041f1b0  // __stdcall, returns void*
    virtual void vfunc_2(int a0);  // [2] 0x0041f2b0  // __thiscall, returns ?

    BaseClass();  // 0x0041e8c0
    virtual ~BaseClass();

    // ---- data members (offsets from ctor 0x0041e8c0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: no base: the compiler's own vptr occupies +0x0.
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char pad_004[0x18];
    unsigned int m_01c;  // +0x01c /* ? */; 4-byte store: mov dword ptr [esi + 0x1c], edi
    unsigned char pad_020[0x30];
    unsigned short m_050;  // +0x050 /* ? */; 2-byte store: mov word ptr [esi + 0x50], di
    unsigned short m_052;  // +0x052 /* ? */; 2-byte store: mov word ptr [esi + 0x52], di
    unsigned int m_054;  // +0x054 /* ? */; 4-byte store: mov dword ptr [esi + 0x54], edi
    unsigned int m_058;  // +0x058 /* ? */; 4-byte store: mov dword ptr [esi + 0x58], edi
    unsigned int m_05c;  // +0x05c /* ? */; 4-byte store: mov dword ptr [esi + 0x5c], edi
    unsigned int m_060;  // +0x060 /* ? */; 4-byte store: mov dword ptr [esi + 0x60], edi
    unsigned int m_064;  // +0x064 /* ? */; 4-byte store: mov dword ptr [esi + 0x64], edi
    unsigned int m_068;  // +0x068 /* ? */; 4-byte store: mov dword ptr [esi + 0x68], edi
    unsigned int m_06c;  // +0x06c /* ? */; 4-byte store: mov dword ptr [esi + 0x6c], edi
    unsigned int m_070;  // +0x070 /* ? */; 4-byte store: mov dword ptr [esi + 0x70], edi
    unsigned int m_074;  // +0x074 /* ? */; 4-byte store: mov dword ptr [esi + 0x74], edi
};
