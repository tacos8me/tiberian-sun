#pragma once
// ==========================================================================
// LZOPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LZOPipe
// vtable       : 0x006a1884  (5 slots)
// constructor  : 0x004fb507
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class LZOPipe : public Pipe
{
public:
    // ---- vtable 0x006a1884 (5 slots) ----
    virtual void vfunc_0();  // [0] 0x004fb900
    virtual void vfunc_1();  // [1] 0x004fb810
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3();  // [3] 0x00591f40
    virtual void vfunc_4();  // [4] 0x004fb580

    LZOPipe();  // 0x004fb507
    virtual ~LZOPipe();
};
