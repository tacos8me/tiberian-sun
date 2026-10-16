#pragma once
// ==========================================================================
// CCFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CCFileClass
// vtable       : 0x00699bc8  (17 slots)
// constructor  : 0x00418e88
// vtable store : 0x00418ec3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "CDFileClass.h"

class CCFileClass : public CDFileClass
{
public:
    // ---- vtable 0x00699bc8 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x00419090  // __cdecl
    virtual void vfunc_2(int a0);  // [2] 0x0044e320  // __thiscall
    virtual void vfunc_3();  // [3] 0x005a6850  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a6890  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004475d0  // __thiscall
    virtual void vfunc_6();  // [6] 0x00447610  // __cdecl
    virtual void vfunc_7(int a0);  // [7] 0x00447650  // __thiscall, returns int
    virtual void vfunc_8(int a0, int a1);  // [8] 0x004190d0  // __thiscall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x00447490  // __thiscall
    virtual void vfunc_10(int a0);  // [10] 0x00447520
    virtual void vfunc_11();  // [11] 0x00447580  // __thiscall
    virtual void vfunc_12(int a0, int a1);  // [12] 0x00447460  // __thiscall
    virtual void vfunc_13();  // [13] 0x00447620  // __thiscall
    virtual void vfunc_14(int a0, int a1, int a2, int a3, int a4, int a5);  // [14] 0x00447780  // __thiscall
    virtual void vfunc_15(int a0);  // [15] 0x00447830  // __thiscall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x00447430  // __stdcall

    CCFileClass();  // 0x00418e88
    virtual ~CCFileClass();

    // ---- data members (offsets from ctor 0x00418dcb) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x4];
    unsigned long m_004;  // init 0x699bc8
    unsigned char pad_008[0x5c];
    unsigned long m_064;  // zero-initialised
};
