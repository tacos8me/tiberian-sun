#pragma once
// ==========================================================================
// CStreamClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CStreamClass
// vtable       : 0x0069b904  (5 slots)
// constructor  : 0x0046e087
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class CStreamClass : public IStream, public ISequentialStream, public IUnknown, public ILinkStream
{
public:
    // ---- vtable 0x0069b904 (5 slots) ----
    virtual void vfunc_0();  // [0] 0x0046ea40
    virtual void vfunc_1();  // [1] 0x0046ea50
    virtual void vfunc_2();  // [2] 0x0046ea60
    virtual void vfunc_3();  // [3] 0x0046e240
    virtual void vfunc_4();  // [4] 0x0046e2d0

    CStreamClass();  // 0x0046e087
    virtual ~CStreamClass();
};
