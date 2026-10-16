#pragma once
// ==========================================================================
// IPXGlobalConnClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IPXGlobalConnClass
// vtable       : 0x006a0af4  (13 slots)
// constructor  : 0x004e71e0
// vtable store : 0x004e749e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "IPXConnClass.h"

class IPXGlobalConnClass : public IPXConnClass
{
public:
    // ---- vtable 0x006a0af4 (13 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004e6f10
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x004e79e0  // __stdcall
    virtual void vfunc_3(int a0, int a1);  // [3] 0x004e7a00  // __stdcall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x004e7a20  // __stdcall
    virtual void vfunc_5();  // [5] 0x0045ee80  // __thiscall, returns int
    virtual void vfunc_6(int a0, int a1, int a2, int a3, int a4);  // [6] 0x004e7970  // __thiscall
    virtual void vfunc_7(int a0, int a1);  // [7] 0x0045eeb0  // __thiscall, returns int
    virtual void vfunc_8();  // [8] 0x004e7910  // __thiscall, returns int
    virtual void vfunc_9(int a0, int a1, int a2, int a3);  // [9] 0x004e78a0  // __thiscall
    virtual void vfunc_10(int a0, int a1, int a2);  // [10] 0x004e7590  // __thiscall
    virtual void vfunc_11(int a0, int a1, int a2);  // [11] 0x004e7640  // __thiscall, returns int
    virtual void vfunc_12(int a0, int a1, int a2, int a3);  // [12] 0x004e7810  // __stdcall, returns int

    IPXGlobalConnClass();  // 0x004e71e0
    virtual ~IPXGlobalConnClass();

    // ---- data members (offsets from ctor 0x004e7452) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a0af4
    unsigned char pad_004[0x74];
    unsigned long m_078;  // /* ? */
    unsigned char pad_07c[0x8];
    unsigned long m_084;  // /* ? */
    unsigned long m_088;  // /* ? */
    unsigned long m_08c;  // /* ? */
    unsigned long m_090;  // /* ? */
    unsigned long m_094;  // /* ? */
};
