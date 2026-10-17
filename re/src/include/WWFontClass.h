#pragma once
// ==========================================================================
// WWFontClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WWFontClass
// vtable       : 0x006a7b34  (9 slots)
// constructor  : 0x00671b90
// vtable store : 0x00671bb7  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FontClass.h"

class WWFontClass : public FontClass
{
public:
    // ---- vtable 0x006a7b34 (9 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00671c40  // __thiscall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x00671c80  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x00671ce0  // __thiscall, returns ?
    virtual void vfunc_4();  // [4] 0x00671db0  // __thiscall, returns int
    virtual void vfunc_5();  // [5] 0x00671de0  // __thiscall, returns int
    virtual void vfunc_6(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [6] 0x00671eb0  // __thiscall, returns ?
    virtual void vfunc_7(int a0);  // [7] 0x00671e10  // __thiscall, returns ?
    virtual void vfunc_8(int a0);  // [8] 0x00671e60  // __thiscall, returns ?

    WWFontClass();  // 0x00671b90
    virtual ~WWFontClass();

    // ---- data members (offsets from ctor 0x00671b90) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: FontClass at +0x0 (ends +0x4).
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char m_004;  // +0x004 /* ? */; 1-byte store: mov byte ptr [esi + 4], al
    unsigned char pad_005[0x3];
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], ecx
    unsigned int m_00c;  // +0x00c zero-initialised; 4-byte store: mov dword ptr [esi + 0xc], 0
    unsigned int m_010;  // +0x010 zero-initialised; 4-byte store: mov dword ptr [esi + 0x10], 0
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], edx
};
