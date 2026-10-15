#pragma once
// ==========================================================================
// DynamicVectorClass_IConnectionPoint.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<IConnectionPoint*>
// vtable       : 0x0069b8cc  (6 slots)
// constructor  : 0x0046c4a6
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_IConnectionPoint.h"

class DynamicVectorClass_IConnectionPoint : public VectorClass_IConnectionPoint
{
public:
    // ---- vtable 0x0069b8cc (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0046cae0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0046cc60  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0046c7d0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0046cd10  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0046c800  // __stdcall, returns int

    DynamicVectorClass_IConnectionPoint();  // 0x0046c4a6
    virtual ~DynamicVectorClass_IConnectionPoint();

    // ---- data members (offsets from ctor 0x0046c440) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b8cc
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // init 0x1
    unsigned char pad_011[0x3];
    unsigned long m_014;  // /* ? */
};
