#pragma once
// ==========================================================================
// BlitPlain_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitPlain<G>
// vtable       : 0x006a7c14  (3 slots)
// constructor  : 0x00675a33
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitPlain_G : public Blitter
{
public:
    // ---- vtable 0x006a7c14 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x006763e0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x006764c0  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x006764f0  // __stdcall

    BlitPlain_G();  // 0x00675a33
    virtual ~BlitPlain_G();

    // ---- data members (offsets from ctor 0x00675f68) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // init 0x6a7c24
};
