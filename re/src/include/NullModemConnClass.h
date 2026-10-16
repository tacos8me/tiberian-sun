#pragma once
// ==========================================================================
// NullModemConnClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class NullModemConnClass
// vtable       : 0x006a27d4  (10 slots)
// constructor  : 0x0056a1c0
// vtable store : 0x0056a2ce  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConnectionClass.h"

class NullModemConnClass : public ConnectionClass
{
public:
    // ---- vtable 0x006a27d4 (10 slots) ----
    virtual void vfunc_1();  // [1] 0x0056a420
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0045eb70  // __thiscall
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0045ec10  // __thiscall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0045edf0  // __thiscall, returns int
    virtual void vfunc_5();  // [5] 0x0045ee80  // __thiscall, returns int
    virtual void vfunc_6();  // [6] 0x0045f020  // __cdecl, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x0045eeb0  // __thiscall, returns int
    virtual void vfunc_8();  // [8] 0x0045f030  // __thiscall, returns int
    virtual void vfunc_9(int a0, int a1, int a2, int a3);  // [9] 0x0056a330  // __thiscall, returns int

    NullModemConnClass();  // 0x0056a1c0
    virtual ~NullModemConnClass();

    // ---- data members (offsets from ctor 0x0056a29c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a27d4
    unsigned char pad_004[0x34];
    unsigned long m_038;  // init 0xffffffff
    unsigned long m_03c;  // /* ? */
};
