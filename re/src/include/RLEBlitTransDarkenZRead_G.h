#pragma once
// ==========================================================================
// RLEBlitTransDarkenZRead_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransDarkenZRead<G>
// vtable       : 0x0069b378  (2 slots)
// constructor  : 0x00461c76
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransDarkenZRead_G : public RLEBlitter
{
public:
    // ---- vtable 0x0069b378 (2 slots) ----
    virtual void vfunc_0();  // [0] 0x0046a430
    virtual void vfunc_1();  // [1] 0x00466e70

    RLEBlitTransDarkenZRead_G();  // 0x00461c76
    virtual ~RLEBlitTransDarkenZRead_G();

    // ---- data members (offsets from ctor 0x00461876) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b3d8
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
