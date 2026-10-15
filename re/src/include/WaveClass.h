#pragma once
// ==========================================================================
// WaveClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class WaveClass
// vtable       : 0x006a72c4  (6 slots)
// constructor  : 0x006522d6
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectClass.h"

class WaveClass : public ObjectClass
{
public:
    // ---- vtable 0x006a72c4 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    WaveClass();  // 0x006522d6
    virtual ~WaveClass();
};
