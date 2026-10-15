#pragma once
// ==========================================================================
// EnumConnectionsClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class EnumConnectionsClass
// vtable       : 0x0069b830  (7 slots)
// constructor  : 0x0046b909
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class EnumConnectionsClass : public IEnumConnections, public IUnknown
{
public:
    // ---- vtable 0x0069b830 (7 slots) ----
    virtual void vfunc_0();  // [0] 0x0046b850
    virtual void vfunc_1();  // [1] 0x0046b8c0
    virtual void vfunc_2();  // [2] 0x0046b8e0
    virtual void vfunc_3();  // [3] 0x0046b980
    virtual void vfunc_4();  // [4] 0x0046ba30
    virtual void vfunc_5();  // [5] 0x0046ba50
    virtual void vfunc_6();  // [6] 0x0046ba60

    EnumConnectionsClass();  // 0x0046b909
    virtual ~EnumConnectionsClass();
};
