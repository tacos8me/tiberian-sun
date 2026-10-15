#pragma once
// ==========================================================================
// BlitTrans_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTrans<G>
// vtable       : 0x006a7c34  (3 slots)
// constructor  : 0x006759e0
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTrans_G : public Blitter
{
public:
    // ---- vtable 0x006a7c34 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00676420
    virtual void vfunc_1();  // [1] 0x00676510
    virtual void vfunc_2();  // [2] 0x00676540

    BlitTrans_G();  // 0x006759e0
    virtual ~BlitTrans_G();
};
