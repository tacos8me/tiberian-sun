#pragma once
// ==========================================================================
// MultiScore.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MultiScore
// vtable       : 0x006a2444  (3 slots)
// constructor  : 0x00554fab
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSEngine.h"

class MultiScore : public MSEngine
{
public:
    // ---- vtable 0x006a2444 (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00557620
    virtual void vfunc_1();  // [1] 0x005554f0
    virtual void vfunc_2();  // [2] 0x00557490

    MultiScore();  // 0x00554fab
    virtual ~MultiScore();
};
