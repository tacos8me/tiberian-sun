#pragma once
// ==========================================================================
// AnimFile.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AnimFile
// vtable       : 0x00699a6c  (8 slots)
// constructor  : 0x00416970
// vtable store : 0x004169c3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Animate.h"

class AnimFile : public Animate
{
public:
    // ---- vtable 0x00699a6c (8 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00416a50  // __thiscall
    virtual void vfunc_2();  // [2] 0x00416a90  // __thiscall
    virtual void vfunc_3(int a0);  // [3] 0x00416d90  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x00417590  // __cdecl
    virtual void vfunc_5();  // [5] 0x00416d50  // __cdecl
    virtual void vfunc_6();  // [6] 0x00416d70  // __cdecl, returns int
    virtual void vfunc_7();  // [7] 0x00416d80  // __cdecl, returns int

    AnimFile();  // 0x00416970
    virtual ~AnimFile();

    // ---- data members (offsets from ctor 0x00416970) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned char pad_004[0x80];
    unsigned int m_084;  // +0x084 /* ? */; 4-byte store: mov dword ptr [eax + 0x84], edx
    unsigned char m_088;  // +0x088 /* ? */; 1-byte store: mov byte ptr [eax + 0x88], dl
    unsigned char m_089;  // +0x089 /* ? */; 1-byte store: mov byte ptr [eax + 0x89], dl
    unsigned char pad_08a[0xd82];
    unsigned int m_e0c;  // +0xe0c /* ? */; 4-byte store: mov dword ptr [eax + 0xe0c], edx
    unsigned int m_e10;  // +0xe10 /* ? */; 4-byte store: mov dword ptr [eax + 0xe10], edx
    unsigned int m_e14;  // +0xe14 /* ? */; 4-byte store: mov dword ptr [eax + 0xe14], ecx
    unsigned int m_e18;  // +0xe18 /* ? */; 4-byte store: mov dword ptr [eax + 0xe18], ecx
};
