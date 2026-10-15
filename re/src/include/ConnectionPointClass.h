#pragma once
// ==========================================================================
// ConnectionPointClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ConnectionPointClass
// vtable       : 0x0069b86c  (8 slots)
// constructor  : 0x0046be3c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ConnectionPointClass : public IConnectionPoint, public IUnknown
{
public:
    // ---- vtable 0x0069b86c (8 slots) ----
    virtual void vfunc_0();  // [0] 0x0046bd80
    virtual void vfunc_1();  // [1] 0x0046bdf0
    virtual void vfunc_2();  // [2] 0x0046be10
    virtual void vfunc_3();  // [3] 0x0046bea0
    virtual void vfunc_4();  // [4] 0x0046bee0
    virtual void vfunc_5();  // [5] 0x0046bf00
    virtual void vfunc_6();  // [6] 0x0046bfd0
    virtual void vfunc_7();  // [7] 0x0046c030

    ConnectionPointClass();  // 0x0046be3c
    virtual ~ConnectionPointClass();

    // ---- data members (offsets from ctor 0x0046be10) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b86c
};
