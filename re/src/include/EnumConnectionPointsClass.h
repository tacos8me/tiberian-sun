#pragma once
// ==========================================================================
// EnumConnectionPointsClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class EnumConnectionPointsClass
// vtable       : 0x0069b8ac  (7 slots)
// constructor  : 0x0046c2a9
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class EnumConnectionPointsClass : public IEnumConnectionPoints, public IUnknown
{
public:
    // ---- vtable 0x0069b8ac (7 slots) ----
    virtual void vfunc_0();  // [0] 0x0046c1f0
    virtual void vfunc_1();  // [1] 0x0046c260
    virtual void vfunc_2();  // [2] 0x0046c280
    virtual void vfunc_3();  // [3] 0x0046c320
    virtual void vfunc_4();  // [4] 0x0046c3c0
    virtual void vfunc_5();  // [5] 0x0046c3e0
    virtual void vfunc_6();  // [6] 0x0046c3f0

    EnumConnectionPointsClass();  // 0x0046c2a9
    virtual ~EnumConnectionPointsClass();
};
