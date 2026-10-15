#pragma once
// ==========================================================================
// VectorClass_articleSystemTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<articleSystemTypeClass**>
// vtable       : 0x006a5b3c  (6 slots)
// constructor  : 0x0061364c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_articleSystemTypeClass
{
public:
    // ---- vtable 0x006a5b3c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0061e510  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0061e2b0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0061e2f0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0061e3a0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0061e3d0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0061e410  // __stdcall, returns int

    VectorClass_articleSystemTypeClass();  // 0x0061364c
    virtual ~VectorClass_articleSystemTypeClass();

    // ---- data members (offsets from ctor 0x00613556) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x8];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x10];
    unsigned long m_038;  // init 0x6a5b3c
};
