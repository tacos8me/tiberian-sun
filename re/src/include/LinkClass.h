#pragma once
// ==========================================================================
// LinkClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LinkClass
// vtable       : 0x0069e7cc  (10 slots)
// constructor  : 0x004a59b0
// vtable store : 0x004a59c0  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class LinkClass
{
public:
    // ---- vtable 0x0069e7cc (10 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004a5f90  // __thiscall
    virtual void vfunc_1();  // [1] 0x004f6960  // __cdecl
    virtual void vfunc_2();  // [2] 0x004f6970  // __cdecl
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __stdcall
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004f6a50  // __thiscall, returns int

    LinkClass();  // 0x004a59b0
    virtual ~LinkClass();

    // ---- data members (offsets from ctor 0x004a59b0) ----
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
    // (the store width and the recovered table disagree): +0x16
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [eax + 4], ecx
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [eax + 8], ecx
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [eax + 0xc], edx
    unsigned int m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [eax + 0x10], edx
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [eax + 0x14], edx
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [eax + 0x18], edx
    unsigned char m_01c;  // +0x01c /* ? */; 1-byte store: mov byte ptr [eax + 0x1c], dl
    unsigned char m_01d;  // +0x01d /* ? */; 1-byte store: mov byte ptr [eax + 0x1d], dl
    unsigned char m_01e;  // +0x01e /* ? */; 1-byte store: mov byte ptr [eax + 0x1e], dl
    unsigned char pad_01f[0x1];
    unsigned int m_020;  // +0x020 /* ? */; 4-byte store: mov dword ptr [eax + 0x20], ecx
};
