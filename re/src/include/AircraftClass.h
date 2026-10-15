#pragma once
// ==========================================================================
// AircraftClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class AircraftClass
// vtable       : 0x00698d10  (8 slots)
// constructor  : 0x00407ddb
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FootClass.h"

class AircraftClass : public FootClass, public IFlyControl
{
public:
    // ---- vtable 0x00698d10 (8 slots) ----
    virtual void vfunc_0();  // [0] 0x0040edc0
    virtual void vfunc_1();  // [1] 0x0040edd0
    virtual void vfunc_2();  // [2] 0x0040ede0
    virtual void vfunc_3();  // [3] 0x0040e260
    virtual void vfunc_4();  // [4] 0x0040e300
    virtual void vfunc_5();  // [5] 0x0040e350
    virtual void vfunc_6();  // [6] 0x0040e370
    virtual void vfunc_7();  // [7] 0x0040e3c0

    AircraftClass();  // 0x00407ddb
    virtual ~AircraftClass();
};
