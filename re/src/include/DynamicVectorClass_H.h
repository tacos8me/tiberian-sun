#pragma once
// ==========================================================================
// DynamicVectorClass_H.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<H>
// vtable       : 0x0069b05c  (6 slots)
// constructor  : 0x0045b117
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_H.h"

class DynamicVectorClass_H : public VectorClass_H
{
public:
    // ---- vtable 0x0069b05c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0044af30  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044b0b0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0044ac70  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0044b160  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0044aca0  // __stdcall, returns int

    DynamicVectorClass_H();  // 0x0045b117
    virtual ~DynamicVectorClass_H();

    // ---- data members (offsets from ctor 0x0045b0b7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69af84
    unsigned char pad_004[0x148];
    unsigned long m_14c;  // init 0x69b05c
    unsigned long m_150;  // /* ? */
    unsigned long m_154;  // /* ? */
    unsigned long m_158;  // /* ? */
    unsigned long m_15c;  // /* ? */
    unsigned long m_160;  // init 0xa
    unsigned long m_164;  // /* ? */
    unsigned long m_168;  // /* ? */
};
