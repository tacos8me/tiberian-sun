#pragma once
// ==========================================================================
// DynamicVectorClass_TechnoClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TechnoClass*>
// vtable       : 0x0069a684  (6 slots)
// constructor  : 0x0042fa03
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TechnoClass.h"

class DynamicVectorClass_TechnoClass : public VectorClass_TechnoClass
{
public:
    // ---- vtable 0x0069a684 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0043a620  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0043a7a0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0043a2a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0043a850  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0043a2d0  // __stdcall, returns int

    DynamicVectorClass_TechnoClass();  // 0x0042fa03
    virtual ~DynamicVectorClass_TechnoClass();

    // ---- data members (offsets from ctor 0x0042f8e7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0xc];
    unsigned long m_028;  // init 0x69a684
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // init 0x1
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // init 0xa
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0xc];
    unsigned long m_058;  // init 0x4000
};
