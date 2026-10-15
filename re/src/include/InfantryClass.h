#pragma once
// ==========================================================================
// InfantryClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class InfantryClass
// vtable       : 0x0069ff34  (6 slots)
// constructor  : 0x004cb59f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FootClass.h"

class InfantryClass : public FootClass
{
public:
    // ---- vtable 0x0069ff34 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    InfantryClass();  // 0x004cb59f
    virtual ~InfantryClass();

    // ---- data members (offsets from ctor 0x004cb520) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69ff34
    unsigned long m_004;  // /* ? */
};
