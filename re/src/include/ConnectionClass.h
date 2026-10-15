#pragma once
// ==========================================================================
// ConnectionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ConnectionClass
// vtable       : 0x0069b09c  (10 slots)
// constructor  : 0x0045eac5
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ConnectionClass
{
public:
    // ---- vtable 0x0069b09c (10 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0045eb50
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0045eb70  // __thiscall
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0045ec10  // __thiscall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0045edf0  // __thiscall, returns int
    virtual void vfunc_5();  // [5] 0x0045ee80  // __thiscall, returns int
    virtual void vfunc_6();  // [6] 0x0045f020  // __cdecl, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x0045eeb0  // __thiscall, returns int
    virtual void vfunc_8();  // [8] 0x0045f030  // __thiscall, returns int
    virtual void vfunc_9();  // [9] 0x006829a5  // __cdecl

    ConnectionClass();  // 0x0045eac5
    virtual ~ConnectionClass();

    // ---- data members (offsets from ctor 0x0045ea9e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b09c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
};
