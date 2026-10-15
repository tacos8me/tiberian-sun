#pragma once
// ==========================================================================
// DynamicVectorClass_W4PassabilityType.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<W4PassabilityType>
// vtable       : 0x006a1b80  (6 slots)
// constructor  : 0x00507976
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_W4PassabilityType.h"

class DynamicVectorClass_W4PassabilityType : public VectorClass_W4PassabilityType
{
public:
    // ---- vtable 0x006a1b80 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00522710  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005219f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00521b70  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00520770  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00521c20  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005207a0  // __stdcall, returns int

    DynamicVectorClass_W4PassabilityType();  // 0x00507976
    virtual ~DynamicVectorClass_W4PassabilityType();

    // ---- data members (offsets from ctor 0x00507931) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0x2
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // zero-initialised
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // init 0x6a1b80
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // init 0x1
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // init 0x12c
};
