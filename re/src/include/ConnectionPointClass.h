#pragma once
// ==========================================================================
// ConnectionPointClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ConnectionPointClass
// vtable       : 0x0069b86c  (8 slots)
// constructor  : 0x0046be10
// vtable store : 0x0046be3c  (inside the ctor)
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
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x0046bd80  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x0046bdf0  // __stdcall
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0046bea0  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0046bee0  // __stdcall
    virtual void vfunc_5(int a0, int a1, int a2);  // [5] 0x0046bf00  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1);  // [6] 0x0046bfd0  // __stdcall, returns int
    virtual void vfunc_7(int a0, int a1);  // [7] 0x0046c030  // __stdcall, returns int

    ConnectionPointClass();  // 0x0046be10
    virtual ~ConnectionPointClass();

    // ---- data members (offsets from ctor 0x0046be0e) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b86c
};
