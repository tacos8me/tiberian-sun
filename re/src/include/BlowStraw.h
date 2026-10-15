#pragma once
// ==========================================================================
// BlowStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlowStraw
// vtable       : 0x006a2308  (3 slots)
// constructor  : 0x00547415
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class BlowStraw : public Straw
{
public:
    // ---- vtable 0x006a2308 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00547b90  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f1020  // __thiscall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00423620  // __thiscall

    BlowStraw();  // 0x00547415
    virtual ~BlowStraw();

    // ---- data members (offsets from ctor 0x0054728e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x1c];
    unsigned long m_01c;  // init 0x69ad0c
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // init 0x69ac10
    unsigned char pad_030[0x8];
    unsigned long m_038;  // init 0x699bc8
    unsigned char pad_03c[0x5c];
    unsigned long m_098;  // /* ? */
    unsigned char pad_09c[0x14];
    unsigned long m_0b0;  // init 0x6a2308
    unsigned char pad_0b4[0x8];
    unsigned long m_0bc;  // /* ? */
};
