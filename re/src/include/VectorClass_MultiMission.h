#pragma once
// ==========================================================================
// VectorClass_MultiMission.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MultiMission*>
// vtable       : 0x006a420c  (6 slots)
// constructor  : 0x005d3d01
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MultiMission
{
public:
    // ---- vtable 0x006a420c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005d6810  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005d6850  // __thiscall
    virtual void vfunc_3();  // [3] 0x005d6900  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005d6930  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005d6970  // __stdcall, returns int

    VectorClass_MultiMission();  // 0x005d3d01
    virtual ~VectorClass_MultiMission();

    // ---- data members (offsets from ctor 0x005d3bff) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xb8];
    unsigned long m_0b8;  // init 0x6a420c
    unsigned long m_0bc;  // /* ? */
    unsigned long m_0c0;  // /* ? */
    unsigned char pad_0c4[0x1];
    unsigned long m_0c5;  // /* ? */
    unsigned char pad_0c9[0x24b];
    unsigned long m_314;  // init 0x6a4228
    unsigned long m_318;  // /* ? */
    unsigned long m_31c;  // /* ? */
    unsigned char pad_320[0x1];
    unsigned long m_321;  // /* ? */
    unsigned char pad_325[0x1377];
    unsigned long m_169c;  // /* ? */
    unsigned long m_16a0;  // /* ? */
    unsigned long m_16a4;  // /* ? */
    unsigned char pad_16a8[0x1];
    unsigned long m_16a9;  // /* ? */
    unsigned char pad_16ad[0x7];
    unsigned long m_16b4;  // /* ? */
    unsigned long m_16b8;  // /* ? */
    unsigned long m_16bc;  // /* ? */
    unsigned char pad_16c0[0x1];
    unsigned long m_16c1;  // /* ? */
    unsigned char pad_16c5[0xb];
    unsigned long m_16d0;  // /* ? */
    unsigned long m_16d4;  // /* ? */
    unsigned char pad_16d8[0x1];
    unsigned long m_16d9;  // /* ? */
};
