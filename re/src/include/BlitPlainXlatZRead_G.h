#pragma once
// ==========================================================================
// BlitPlainXlatZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitPlainXlatZRead<G>
// vtable       : 0x0069b664  (3 slots)
// constructor  : 0x00461192
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitPlainXlatZRead_G : public Blitter
{
public:
    // ---- vtable 0x0069b664 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00469bb0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00464280  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x004642e0  // __stdcall

    BlitPlainXlatZRead_G();  // 0x00461192
    virtual ~BlitPlainXlatZRead_G();
};
