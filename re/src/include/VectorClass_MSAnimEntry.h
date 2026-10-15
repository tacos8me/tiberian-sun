#pragma once
// ==========================================================================
// VectorClass_MSAnimEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MSAnimEntry*>
// vtable       : 0x006a258c  (6 slots)
// constructor  : 0x00559a76
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MSAnimEntry
{
public:
    // ---- vtable 0x006a258c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0055b890  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055b8d0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0055b980  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055b9b0  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0055b9f0  // __stdcall, returns int

    VectorClass_MSAnimEntry();  // 0x00559a76
    virtual ~VectorClass_MSAnimEntry();

    // ---- data members (offsets from ctor 0x00559a31) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2570
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // /* ? */
    unsigned char pad_011[0x1b];
    unsigned long m_02c;  // init 0x6a258c
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned char pad_038[0x1];
    unsigned long m_039;  // /* ? */
    unsigned char pad_03d[0x7];
    unsigned long m_044;  // init 0x6a25a8
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned char pad_050[0x1];
    unsigned long m_051;  // /* ? */
};
