#pragma once
// ==========================================================================
// CarryoverClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CarryoverClass
// vtable       : 0x0069abe4  (10 slots)
// constructor  : 0x00447149
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

    CarryoverClass();  // 0x00447149
    virtual ~CarryoverClass();

    // ---- data members (offsets from ctor 0x00447117) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69abe4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // init 0xffffffff
};
