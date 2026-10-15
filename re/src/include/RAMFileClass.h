#pragma once
// ==========================================================================
// RAMFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RAMFileClass
// vtable       : 0x006a3958  (17 slots)
// constructor  : 0x005a59ba
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FileClass.h"

class RAMFileClass : public FileClass
{
public:
    // ---- vtable 0x006a3958 (17 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005a5ce0  // __thiscall
    virtual void vfunc_1();  // [1] 0x005a5c90  // __cdecl, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005a5ca0  // __stdcall
    virtual void vfunc_3();  // [3] 0x005a5a20  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a5a40  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005a5a60  // __stdcall
    virtual void vfunc_6();  // [6] 0x005a5a70  // __cdecl
    virtual void vfunc_7(int a0);  // [7] 0x005a5a90  // __thiscall
    virtual void vfunc_8(int a0, int a1);  // [8] 0x005a5a80  // __stdcall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x005a5ae0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x005a5b60  // __thiscall
    virtual void vfunc_11();  // [11] 0x005a5be0  // __cdecl
    virtual void vfunc_12(int a0, int a1);  // [12] 0x005a5bf0  // __thiscall
    virtual void vfunc_13();  // [13] 0x005a5c80  // __cdecl
    virtual void vfunc_14();  // [14] 0x005a5cb0  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x005a5cc0  // __stdcall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a5cd0  // __stdcall

    RAMFileClass();  // 0x005a59ba
    virtual ~RAMFileClass();

    // ---- data members (offsets from ctor 0x005a598b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3958
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0x1
    unsigned long m_018;  // /* ? */
};
