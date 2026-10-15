#pragma once
// ==========================================================================
// Animate.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Animate
// vtable       : 0x00699a90  (8 slots)
// constructor  : 0x0041697c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class Animate
{
public:
    // ---- vtable 0x00699a90 (8 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00417570  // __thiscall
    virtual void vfunc_1();  // [1] 0x006829a5  // __cdecl
    virtual void vfunc_2();  // [2] 0x006829a5  // __cdecl
    virtual void vfunc_3();  // [3] 0x006829a5  // __cdecl
    virtual void vfunc_4();  // [4] 0x006829a5  // __cdecl
    virtual void vfunc_5();  // [5] 0x006829a5  // __cdecl
    virtual void vfunc_6();  // [6] 0x006829a5  // __cdecl
    virtual void vfunc_7();  // [7] 0x006829a5  // __cdecl

    Animate();  // 0x0041697c
    virtual ~Animate();

    // ---- data members (offsets from ctor 0x00416967) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699a90
    unsigned char pad_004[0x80];
    unsigned long m_084;  // /* ? */
    unsigned long m_088;  // /* ? */
    unsigned char pad_08c[0xd80];
    unsigned long m_e0c;  // /* ? */
    unsigned long m_e10;  // /* ? */
    unsigned long m_e14;  // /* ? */
    unsigned long m_e18;  // /* ? */
};
