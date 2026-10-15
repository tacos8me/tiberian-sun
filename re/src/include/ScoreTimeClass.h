#pragma once
// ==========================================================================
// ScoreTimeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ScoreTimeClass
// vtable       : 0x006a3ee4  (4 slots)
// constructor  : 0x005ca0db
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ScoreAnimClass.h"

class ScoreTimeClass : public ScoreAnimClass
{
public:
    // ---- vtable 0x006a3ee4 (4 slots) ----
    virtual void vfunc_0();  // [0] 0x005cdf20
    virtual void vfunc_1();  // [1] 0x005cde90
    virtual void vfunc_2();  // [2] 0x005cdee0
    virtual void vfunc_3();  // [3] 0x005cda00

    ScoreTimeClass();  // 0x005ca0db
    virtual ~ScoreTimeClass();
};
