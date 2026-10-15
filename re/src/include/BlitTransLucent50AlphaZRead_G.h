#pragma once
// ==========================================================================
// BlitTransLucent50AlphaZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent50AlphaZRead<G>
// vtable       : 0x0069b474  (3 slots)
// constructor  : 0x00461802
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent50AlphaZRead_G : public Blitter
{
public:
    // ---- vtable 0x0069b474 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046a0b0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00465cd0  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x00465dd0  // __stdcall

    BlitTransLucent50AlphaZRead_G();  // 0x00461802
    virtual ~BlitTransLucent50AlphaZRead_G();

    // ---- data members (offsets from ctor 0x004616b7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b554
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
