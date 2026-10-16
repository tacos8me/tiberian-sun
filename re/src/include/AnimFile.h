#pragma once
// ==========================================================================
// AnimFile.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AnimFile
// vtable       : 0x00699a6c  (8 slots)
// constructor  : 0x004169c3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Animate.h"

class AnimFile : public Animate
{
public:
    // ---- vtable 0x00699a6c (8 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00416a50  // __thiscall
    virtual void vfunc_2();  // [2] 0x00416a90  // __thiscall
    virtual void vfunc_3(int a0);  // [3] 0x00416d90  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x00417590  // __cdecl
    virtual void vfunc_5();  // [5] 0x00416d50  // __cdecl
    virtual void vfunc_6();  // [6] 0x00416d70  // __cdecl, returns int
    virtual void vfunc_7();  // [7] 0x00416d80  // __cdecl, returns int

    AnimFile();  // 0x004169c3
    virtual ~AnimFile();

    // ---- data members (offsets from ctor 0x004169b0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699a6c
    unsigned char pad_004[0xe10];
    unsigned long m_e14;  // /* ? */
    unsigned long m_e18;  // /* ? */
};
