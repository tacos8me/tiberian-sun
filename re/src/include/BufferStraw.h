#pragma once
// ==========================================================================
// BufferStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BufferStraw
// vtable       : 0x0069bbcc  (3 slots)
// constructor  : 0x00476910
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class BufferStraw : public Straw
{
public:
    // ---- vtable 0x0069bbcc (3 slots) ----
    virtual void vfunc_0();  // [0] 0x00478980
    virtual void vfunc_1();  // [1] 0x005f1020
    virtual void vfunc_2();  // [2] 0x00674260

    BufferStraw();  // 0x00476910
    virtual ~BufferStraw();

    // ---- data members (offsets from ctor 0x0047677d) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // init 0x698b58
    unsigned char pad_014[0x4];
    unsigned long m_018;  // init 0x698be8
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0x2
    unsigned char pad_02c[0x4];
    unsigned long m_030;  // /* ? */
    unsigned char pad_034[0x4];
    unsigned long m_038;  // /* ? */
    unsigned char pad_03c[0x4];
    unsigned long m_040;  // init 0x69bbcc
    unsigned long m_044;  // init 0x69bbdc
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned char pad_050[0x4];
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
};
