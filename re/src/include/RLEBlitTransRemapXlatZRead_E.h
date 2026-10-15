#pragma once
// ==========================================================================
// RLEBlitTransRemapXlatZRead_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransRemapXlatZRead<E>
// vtable       : 0x0069b714  (2 slots)
// constructor  : 0x00460ece
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransRemapXlatZRead_E : public RLEBlitter
{
public:
    // ---- vtable 0x0069b714 (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5);  // [1] 0x00463a90

    RLEBlitTransRemapXlatZRead_E();  // 0x00460ece
    virtual ~RLEBlitTransRemapXlatZRead_E();

    // ---- data members (offsets from ctor 0x00460be1) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7b4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
