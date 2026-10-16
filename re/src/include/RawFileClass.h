#pragma once
// ==========================================================================
// RawFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RawFileClass
// vtable       : 0x006a39e8  (17 slots)
// constructor  : 0x005a6120
// vtable store : 0x005a6125  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FileClass.h"

class RawFileClass : public FileClass
{
public:
    // ---- vtable 0x006a39e8 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x00419090  // __cdecl
    virtual void vfunc_2(int a0);  // [2] 0x005a61d0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005a6850  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a6890  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005a6300  // __thiscall
    virtual void vfunc_6();  // [6] 0x005a6b20  // __cdecl, returns int
    virtual void vfunc_7(int a0);  // [7] 0x005a6260  // __thiscall, returns int
    virtual void vfunc_8(int a0, int a1);  // [8] 0x005a6240  // __thiscall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x005a63f0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x005a6600  // __thiscall
    virtual void vfunc_11();  // [11] 0x005a67d0  // __thiscall
    virtual void vfunc_12(int a0, int a1);  // [12] 0x005a64d0  // __thiscall
    virtual void vfunc_13();  // [13] 0x005a63b0  // __thiscall
    virtual void vfunc_14(int a0, int a1);  // [14] 0x005a68f0  // __cdecl
    virtual void vfunc_15(int a0);  // [15] 0x005a6940  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a6190  // __stdcall

    RawFileClass();  // 0x005a6120
    virtual ~RawFileClass();

    // ---- data members (offsets from ctor 0x005a6116) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a39e8
    unsigned char pad_004[0xc];
    unsigned long m_010;  // init 0xffffffff
    unsigned long m_014;  // zero-initialised
    unsigned char pad_018[0x4];
    unsigned long m_01c;  // zero-initialised
};
