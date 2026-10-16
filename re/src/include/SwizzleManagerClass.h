#pragma once
// ==========================================================================
// SwizzleManagerClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SwizzleManagerClass
// vtable       : 0x006a5090  (10 slots)
// constructor  : 0x005f3508
// vtable store : 0x005f356e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class SwizzleManagerClass : public ISwizzle, public IUnknown
{
public:
    // ---- vtable 0x006a5090 (10 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005f38d0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005f3950  // __stdcall, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005f3960  // __stdcall, returns int
    virtual void vfunc_3(int a0);  // [3] 0x005f36d0  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005f36e0  // __stdcall, returns int
    virtual void vfunc_5(int a0, int a1, int a2);  // [5] 0x005f3930  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1, int a2);  // [6] 0x005f3760  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1, int a2);  // [7] 0x005f3970  // __stdcall, returns int
    virtual void vfunc_8(int a0, int a1, int a2, int a3);  // [8] 0x005f3980  // __stdcall, returns int
    virtual void vfunc_9(int a0, int a1);  // [9] 0x005f38b0  // __stdcall, returns int

    SwizzleManagerClass();  // 0x005f3508
    virtual ~SwizzleManagerClass();

    // ---- data members (offsets from ctor 0x005f3615) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a5090
    unsigned long m_004;  // init 0x6a50bc
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // init 0x1
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // init 0x6a50bc
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0x1
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
};
