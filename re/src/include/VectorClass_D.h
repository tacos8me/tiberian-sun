#pragma once
// ==========================================================================
// VectorClass_D.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<D*>
// vtable       : 0x006a173c  (6 slots)
// constructor  : 0x004f6c66
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_D
{
public:
    // ---- vtable 0x006a173c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004f7d20  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004f7ac0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004f7b00  // __thiscall
    virtual void vfunc_3();  // [3] 0x004f7bb0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004f7be0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004f7c20  // __stdcall, returns int

    VectorClass_D();  // 0x004f6c66
    virtual ~VectorClass_D();

    // ---- data members (offsets from ctor 0x004f6c21) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a173c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x14];
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
};
