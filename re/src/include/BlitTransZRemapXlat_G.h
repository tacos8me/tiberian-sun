#pragma once
// ==========================================================================
// BlitTransZRemapXlat_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransZRemapXlat<G>
// vtable       : 0x0069b6b4  (3 slots)
// constructor  : 0x004610be
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransZRemapXlat_G : public Blitter
{
public:
    // ---- vtable 0x0069b6b4 (3 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00463fe0  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x00464030  // __stdcall

    BlitTransZRemapXlat_G();  // 0x004610be
    virtual ~BlitTransZRemapXlat_G();
};
