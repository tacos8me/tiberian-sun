#pragma once
// ==========================================================================
// CarryoverClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CarryoverClass
// vtable       : 0x0069abe4  (10 slots)
// constructor  : 0x00447120
// vtable store : 0x00447149  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "LinkClass.h"

class CarryoverClass : public LinkClass
{
public:
    // ---- vtable 0x0069abe4 (10 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004472f0  // __thiscall
    virtual void vfunc_1();  // [1] 0x004f6960  // __cdecl
    virtual void vfunc_2();  // [2] 0x004f6970  // __cdecl
    virtual void vfunc_3(int a0);  // [3] 0x004f69e0  // __stdcall
    virtual void vfunc_4(int a0);  // [4] 0x004f6a20  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x004f6a00  // __thiscall
    virtual void vfunc_6();  // [6] 0x004f6980  // __cdecl
    virtual void vfunc_7();  // [7] 0x004f69b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004f6930  // __cdecl, returns int
    virtual void vfunc_9();  // [9] 0x004f6a50  // __thiscall, returns int

    CarryoverClass();  // 0x00447120
    virtual ~CarryoverClass();

    // ---- data members (offsets from ctor 0x00447120) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    unsigned int m_004;  // +0x004 /* ? */; 4-byte store: mov dword ptr [esi + 4], eax
    unsigned int m_008;  // +0x008 /* ? */; 4-byte store: mov dword ptr [esi + 8], eax
    unsigned int m_00c;  // +0x00c /* ? */; 4-byte store: mov dword ptr [esi + 0xc], eax
    unsigned int m_010;  // +0x010 /* ? */; 4-byte store: mov dword ptr [esi + 0x10], eax
    unsigned int m_014;  // +0x014 /* ? */; 4-byte store: mov dword ptr [esi + 0x14], ecx
    unsigned int m_018;  // +0x018 /* ? */; 4-byte store: mov dword ptr [esi + 0x18], eax
    unsigned int m_01c;  // +0x01c init 0xffffffff; 4-byte store: mov dword ptr [esi + 0x1c], 0xffffffff
};
