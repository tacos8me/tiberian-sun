#pragma once
// ==========================================================================
// BlitTransLucent50_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent50<G>
// vtable       : 0x0069b684  (3 slots)
// constructor  : 0x00461142
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent50_G : public Blitter
{
public:
    // ---- vtable 0x0069b684 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00469b70  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00464160  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x004641c0  // __stdcall

    BlitTransLucent50_G();  // 0x00461142
    virtual ~BlitTransLucent50_G();
};
