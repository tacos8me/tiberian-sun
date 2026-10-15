#pragma once
// ==========================================================================
// RLEBlitTransZRemapXlatZReadWrite_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RLEBlitTransZRemapXlatZReadWrite<E>
// vtable       : 0x0069b6fc  (2 slots)
// constructor  : 0x00460f85
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "RLEBlitter.h"

class RLEBlitTransZRemapXlatZReadWrite_E : public RLEBlitter
{
public:
    // ---- vtable 0x0069b6fc (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00469a90  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4);  // [1] 0x00463c70

    RLEBlitTransZRemapXlatZReadWrite_E();  // 0x00460f85
    virtual ~RLEBlitTransZRemapXlatZReadWrite_E();

    // ---- data members (offsets from ctor 0x00460be1) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7b4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
