#pragma once
// ==========================================================================
// VectorClass_MSTextEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MSTextEntry*>
// vtable       : 0x006a2634  (6 slots)
// constructor  : 0x0055a8b8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MSTextEntry
{
public:
    // ---- vtable 0x006a2634 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0055b170  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055b1b0  // __thiscall
    virtual void vfunc_3();  // [3] 0x0055b260  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055b290  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0055b2d0  // __stdcall, returns int

    VectorClass_MSTextEntry();  // 0x0055a8b8
    virtual ~VectorClass_MSTextEntry();

    // ---- data members (offsets from ctor 0x0055a79b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x24];
    unsigned long m_024;  // init 0x6a2618
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned char pad_030[0x1];
    unsigned long m_031;  // /* ? */
    unsigned char pad_035[0x7];
    unsigned long m_03c;  // init 0x6a2634
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned char pad_048[0x1];
    unsigned long m_049;  // /* ? */
    unsigned char pad_04d[0x7];
    unsigned long m_054;  // init 0x6a2650
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned char pad_060[0x1];
    unsigned long m_061;  // /* ? */
};
