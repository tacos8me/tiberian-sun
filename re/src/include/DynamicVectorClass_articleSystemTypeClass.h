#pragma once
// ==========================================================================
// DynamicVectorClass_articleSystemTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<articleSystemTypeClass**>
// vtable       : 0x006a5b58  (6 slots)
// constructor  : 0x006134aa
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_articleSystemTypeClass.h"

class DynamicVectorClass_articleSystemTypeClass : public VectorClass_articleSystemTypeClass
{
public:
    // ---- vtable 0x006a5b58 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0061e560  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0061e2b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0061e430  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0061e260  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0061e4e0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0061e290  // __stdcall, returns int

    DynamicVectorClass_articleSystemTypeClass();  // 0x006134aa
    virtual ~DynamicVectorClass_articleSystemTypeClass();

    // ---- data members (offsets from ctor 0x00613413) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x8];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x10];
    unsigned long m_038;  // init 0x6a5b58
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // init 0x1
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
};
