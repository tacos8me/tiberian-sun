#pragma once
// ==========================================================================
// BlitPlainXlat_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitPlainXlat<G>
// vtable       : 0x0069b6d4  (3 slots)
// constructor  : 0x0046106a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitPlainXlat_G : public Blitter
{
public:
    // ---- vtable 0x0069b6d4 (3 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00463f70  // __thiscall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x00463fb0  // __stdcall

    BlitPlainXlat_G();  // 0x0046106a
    virtual ~BlitPlainXlat_G();
};
