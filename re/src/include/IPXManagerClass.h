#pragma once
// ==========================================================================
// IPXManagerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IPXManagerClass
// vtable       : 0x006a0b34  (17 slots)
// constructor  : 0x004e7aa6
// vtable store : 0x004e7d09  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConnManClass.h"

class IPXManagerClass : public ConnManClass
{
public:
    // ---- vtable 0x006a0b34 (17 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004e8ac0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);  // [1] 0x004e8510  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x004e83a0  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1, int a2);  // [3] 0x004e8480  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x004e81b0  // __cdecl
    virtual void vfunc_5(int a0);  // [5] 0x004e81c0  // __stdcall
    virtual void vfunc_6(int a0);  // [6] 0x004e8260  // __stdcall
    virtual void vfunc_7();  // [7] 0x004e87c0  // __cdecl
    virtual void vfunc_8();  // [8] 0x004e87e0  // __cdecl
    virtual void vfunc_9(int a0);  // [9] 0x004e8800  // __thiscall
    virtual void vfunc_10(int a0);  // [10] 0x004e8870  // __thiscall
    virtual void vfunc_11();  // [11] 0x004e8960  // __thiscall
    virtual void vfunc_12();  // [12] 0x004e8900  // __thiscall
    virtual void vfunc_13(int a0, int a1, int a2, int a3);  // [13] 0x004e7f90  // __thiscall
    virtual void vfunc_14(int a0, int a1, int a2);  // [14] 0x004e8040  // __stdcall
    virtual void vfunc_15(int a0, int a1, int a2, int a3, int a4, int a5);  // [15] 0x004e8a50  // __stdcall
    virtual void vfunc_16(int a0, int a1);  // [16] 0x004e8ab0  // __stdcall

    IPXManagerClass();  // 0x004e7aa6
    virtual ~IPXManagerClass();

    // ---- data members (offsets from ctor 0x004e7df0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a0b34
    unsigned char pad_004[0x1];
    unsigned long m_005;  // /* ? */
    unsigned char pad_009[0x33];
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
};
