#pragma once
// ==========================================================================
// SHAPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SHAPipe
// vtable       : 0x0069acf4  (5 slots)
// constructor  : 0x00447c19
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class SHAPipe : public Pipe
{
public:
    // ---- vtable 0x0069acf4 (5 slots) ----
    virtual void vfunc_0();  // [0] 0x0044ab80
    virtual void vfunc_1();  // [1] 0x00591fb0
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3();  // [3] 0x00591f40
    virtual void vfunc_4();  // [4] 0x005d7ba0

    SHAPipe();  // 0x00447c19
    virtual ~SHAPipe();
};
