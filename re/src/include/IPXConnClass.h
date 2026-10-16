#pragma once
// ==========================================================================
// IPXConnClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IPXConnClass
// vtable       : 0x006a0abc  (10 slots)
// constructor  : 0x004e6e2a
// vtable store : 0x004e6e8f  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConnectionClass.h"

class IPXConnClass : public ConnectionClass
{
public:
    // ---- vtable 0x006a0abc (10 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004e6f10
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0045eb70  // __thiscall
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0045ec10  // __thiscall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0045edf0  // __thiscall, returns int
    virtual void vfunc_5();  // [5] 0x0045ee80  // __thiscall, returns int
    virtual void vfunc_6();  // [6] 0x0045f020  // __cdecl, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x0045eeb0  // __thiscall, returns int
    virtual void vfunc_8();  // [8] 0x0045f030  // __thiscall, returns int
    virtual void vfunc_9(int a0, int a1, int a2, int a3, int a4, int a5);  // [9] 0x004e7000

    IPXConnClass();  // 0x004e6e2a
    virtual ~IPXConnClass();

    // ---- data members (offsets from ctor 0x004e6e47) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a0abc
    unsigned char pad_004[0x44];
    unsigned long m_048;  // zero-initialised
    unsigned long m_04c;  // /* ? */
};
