#pragma once
// ==========================================================================
// RestateMission.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RestateMission
// vtable       : 0x006a3a7c  (3 slots)
// constructor  : 0x005a80e6
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSEngine.h"

class RestateMission : public MSEngine
{
public:
    // ---- vtable 0x006a3a7c (3 slots) ----
    virtual void vfunc_0();  // [0] 0x005a8f30
    virtual void vfunc_1();  // [1] 0x005a8d30
    virtual void vfunc_2();  // [2] 0x0055cc40

    RestateMission();  // 0x005a80e6
    virtual ~RestateMission();
};
