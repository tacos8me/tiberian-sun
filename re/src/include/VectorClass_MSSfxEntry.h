#pragma once
// ==========================================================================
// VectorClass_MSSfxEntry.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MSSfxEntry*>
// vtable       : 0x006a25a8  (6 slots)
// constructor  : 0x00559a53
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MSSfxEntry
{
public:
    // ---- vtable 0x006a25a8 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0055c070  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0055baf0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055bb30  // __thiscall
    virtual void vfunc_3();  // [3] 0x0055bbe0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055bc10  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0055bc50  // __stdcall, returns int

    VectorClass_MSSfxEntry();  // 0x00559a53
    virtual ~VectorClass_MSSfxEntry();

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
