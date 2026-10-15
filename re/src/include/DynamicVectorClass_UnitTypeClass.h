#pragma once
// ==========================================================================
// DynamicVectorClass_UnitTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<UnitTypeClass*>
// vtable       : 0x0069f59c  (6 slots)
// constructor  : 0x004a8913
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_UnitTypeClass.h"

class DynamicVectorClass_UnitTypeClass : public VectorClass_UnitTypeClass
{
public:
    // ---- vtable 0x0069f59c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1bb0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aef50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004af0d0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9eb0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004af180  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9ee0  // __stdcall, returns int

    DynamicVectorClass_UnitTypeClass();  // 0x004a8913
    virtual ~DynamicVectorClass_UnitTypeClass();

    // ---- data members (offsets from ctor 0x005c543a) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // init 0x1
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0x69a668
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned char pad_034[0x1];
    unsigned long m_035;  // zero-initialised
    unsigned char pad_039[0x7];
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // init 0x69f5b8
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // init 0x69f59c
    unsigned char pad_050[0x1];
    unsigned long m_051;  // zero-initialised
    unsigned char pad_055[0x7];
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned char pad_068[0x1];
    unsigned long m_069;  // zero-initialised
    unsigned char pad_06d[0x3];
    unsigned long m_070;  // /* ? */
    unsigned char pad_074[0x4];
    unsigned long m_078;  // init 0x699e00
    unsigned long m_07c;  // /* ? */
    unsigned long m_080;  // /* ? */
    unsigned char pad_084[0x1];
    unsigned long m_085;  // zero-initialised
    unsigned char pad_089[0x7];
    unsigned long m_090;  // init 0x1
    unsigned char pad_094[0xc];
    unsigned long m_0a0;  // /* ? */
    unsigned long m_0a4;  // /* ? */
    unsigned char pad_0a8[0x8];
    unsigned long m_0b0;  // /* ? */
};
