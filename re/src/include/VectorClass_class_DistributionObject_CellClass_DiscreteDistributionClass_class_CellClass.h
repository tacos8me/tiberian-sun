#pragma once
// ==========================================================================
// VectorClass_class_DistributionObject_CellClass_DiscreteDistributionClass_class_CellClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<class DistributionObject<CellClass*>, DiscreteDistributionClass<class CellClass>>
// vtable       : 0x0069e70c  (6 slots)
// constructor  : 0x004a3869
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_class_DistributionObject_CellClass_DiscreteDistributionClass_class_CellClass
{
public:
    // ---- vtable 0x0069e70c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004a5430  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004a4e90  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004a4ef0  // __thiscall
    virtual void vfunc_3();  // [3] 0x004a5000  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004a5040  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004a5090  // __stdcall, returns int

    VectorClass_class_DistributionObject_CellClass_DiscreteDistributionClass_class_CellClass();  // 0x004a3869
    virtual ~VectorClass_class_DistributionObject_CellClass_DiscreteDistributionClass_class_CellClass();

    // ---- data members (offsets from ctor 0x004a37d0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x1c];
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x4];
    unsigned long m_028;  // init 0x69e70c
    unsigned char pad_02c[0x14];
    unsigned long m_040;  // zero-initialised
    unsigned char pad_044[0x8];
    unsigned long m_04c;  // /* ? */
};
