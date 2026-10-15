#pragma once
// ==========================================================================
// VectorClass_N.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<_N>
// vtable       : 0x0069bc1c  (6 slots)
// constructor  : 0x0047861e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_N
{
public:
    // ---- vtable 0x0069bc1c (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00478cb0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00478a50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00478a90  // __thiscall
    virtual void vfunc_3();  // [3] 0x00478b40  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00478b70  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00478bb0  // __stdcall, returns int

    VectorClass_N();  // 0x0047861e
    virtual ~VectorClass_N();

    // ---- data members (offsets from ctor 0x00478615) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x1c];
    unsigned long m_01c;  // init 0x698730
    unsigned char pad_020[0x44];
    unsigned long m_064;  // init 0x69bc1c
    unsigned long m_068;  // /* ? */
    unsigned long m_06c;  // /* ? */
    unsigned char pad_070[0x1];
    unsigned long m_071;  // zero-initialised
    unsigned char pad_075[0x7];
    unsigned long m_07c;  // init 0x699e00
    unsigned long m_080;  // /* ? */
    unsigned char pad_084[0x4];
    unsigned long m_088;  // /* ? */
};
