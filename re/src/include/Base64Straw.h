#pragma once
// ==========================================================================
// Base64Straw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Base64Straw
// vtable       : 0x006a0430  (3 slots)
// constructor  : 0x004d599c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class Base64Straw : public Straw
{
public:
    // ---- vtable 0x006a0430 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004d8020  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f1020  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3);  // [2] 0x0041e140  // __thiscall

    Base64Straw();  // 0x004d599c
    virtual ~Base64Straw();

    // ---- data members (offsets from ctor 0x004d5984) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x30];
    unsigned long m_030;  // init 0x6a0430
    unsigned char pad_034[0x4];
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned char pad_044[0x4];
    unsigned long m_048;  // /* ? */
};
