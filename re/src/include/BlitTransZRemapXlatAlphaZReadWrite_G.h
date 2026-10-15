#pragma once
// ==========================================================================
// BlitTransZRemapXlatAlphaZReadWrite_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransZRemapXlatAlphaZReadWrite<G>
// vtable       : 0x0069b414  (3 slots)
// constructor  : 0x004619f7
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransZRemapXlatAlphaZReadWrite_G : public Blitter
{
public:
    // ---- vtable 0x0069b414 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0046a230  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00466420  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x00466500  // __stdcall

    BlitTransZRemapXlatAlphaZReadWrite_G();  // 0x004619f7
    virtual ~BlitTransZRemapXlatAlphaZReadWrite_G();

    // ---- data members (offsets from ctor 0x004619c7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b414
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
};
