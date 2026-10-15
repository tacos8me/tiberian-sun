#pragma once
// ==========================================================================
// BlitTransLucent75_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent75<G>
// vtable       : 0x0069b694  (3 slots)
// constructor  : 0x00461113
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent75_G : public Blitter
{
public:
    // ---- vtable 0x0069b694 (3 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x004640d0  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x00464130  // __stdcall

    BlitTransLucent75_G();  // 0x00461113
    virtual ~BlitTransLucent75_G();
};
