#pragma once
// ==========================================================================
// DynamicVectorClass_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<G>
// vtable       : 0x00699d8c  (6 slots)
// constructor  : 0x0041cef3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_G.h"

class DynamicVectorClass_G : public VectorClass_G
{
public:
    // ---- vtable 0x00699d8c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0041da60  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0041dbe0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0041d760  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0041dc90  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0041d790  // __stdcall, returns int

    DynamicVectorClass_G();  // 0x0041cef3
    virtual ~DynamicVectorClass_G();

    // ---- data members (offsets from ctor 0x0041ce98) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // init 0x699d8c
    unsigned long m_024;  // zero-initialised
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x1];
    unsigned long m_02d;  // zero-initialised
    unsigned char pad_031[0x3];
    unsigned long m_034;  // init 0xa
    unsigned long m_038;  // zero-initialised
    unsigned char pad_03c[0x1];
    unsigned long m_03d;  // zero-initialised
};
