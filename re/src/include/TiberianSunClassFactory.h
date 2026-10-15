#pragma once
// ==========================================================================
// TiberianSunClassFactory.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TiberianSunClassFactory
// vtable       : 0x0069f804  (5 slots)
// constructor  : 0x004a8ff6
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class TiberianSunClassFactory : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x0069f804 (5 slots) ----
    virtual void vfunc_0();  // [0] 0x00419410
    virtual void vfunc_1();  // [1] 0x004a9460
    virtual void vfunc_2();  // [2] 0x004a9470
    virtual void vfunc_3();  // [3] 0x00419470
    virtual void vfunc_4();  // [4] 0x004a9480

    TiberianSunClassFactory();  // 0x004a8ff6
    virtual ~TiberianSunClassFactory();
};
