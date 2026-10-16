#pragma once
// ==========================================================================
// NullModemClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class NullModemClass
// vtable       : 0x006a2814  (16 slots)
// constructor  : 0x0056c0df
// vtable store : 0x0056c363  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConnManClass.h"

class NullModemClass : public ConnManClass
{
public:
    // ---- vtable 0x006a2814 (16 slots) ----
    virtual void vfunc_1();  // [1] 0x0056c9d0  // __thiscall, returns int
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x0056e040  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x0056e080  // __stdcall, returns int
    virtual void vfunc_4();  // [4] 0x0056c6c0  // __cdecl
    virtual void vfunc_5(int a0);  // [5] 0x0056e000  // __stdcall, returns int
    virtual void vfunc_6(int a0);  // [6] 0x0056e010  // __stdcall, returns int
    virtual void vfunc_7();  // [7] 0x0056e0b0  // __cdecl
    virtual void vfunc_8();  // [8] 0x0056e0d0  // __cdecl
    virtual void vfunc_9(int a0);  // [9] 0x0056e0f0  // __stdcall
    virtual void vfunc_10(int a0);  // [10] 0x0056e110  // __stdcall
    virtual void vfunc_11();  // [11] 0x0056cc70  // __cdecl
    virtual void vfunc_12();  // [12] 0x0056cc50  // __cdecl, returns int
    virtual void vfunc_13(int a0, int a1, int a2, int a3);  // [13] 0x0056c930  // __stdcall
    virtual void vfunc_14(int a0, int a1, int a2);  // [14] 0x0056e020  // __stdcall
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5);  // [15] 0x0056ccd0  // __stdcall

    NullModemClass();  // 0x0056c0df
    virtual ~NullModemClass();

    // ---- data members (offsets from ctor 0x0056c3c0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2814
    unsigned char pad_004[0x3c];
    unsigned long m_040;  // /* ? */
};
