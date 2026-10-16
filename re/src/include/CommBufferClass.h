#pragma once
// ==========================================================================
// CommBufferClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CommBufferClass
// vtable       : 0x0069b094  (1 slots)
// constructor  : 0x0045d930
// vtable store : 0x0045d99e  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class CommBufferClass
{
public:
    // ---- vtable 0x0069b094 (1 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0045ea80  // __thiscall

    CommBufferClass();  // 0x0045d930
    virtual ~CommBufferClass();

    // ---- data members (offsets from ctor 0x0045d97a) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b094
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x10];
    unsigned long m_024;  // /* ? */
    unsigned char pad_028[0x8];
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned char pad_038[0x8];
    unsigned long m_040;  // /* ? */
};
