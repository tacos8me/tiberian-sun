#pragma once
// ==========================================================================
// VectorClass_MapRegionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<MapRegionClass*>
// vtable       : 0x006a1dc4  (6 slots)
// constructor  : 0x00523694
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_MapRegionClass
{
public:
    // ---- vtable 0x006a1dc4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00540a00  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005403f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00540430  // __thiscall
    virtual void vfunc_3();  // [3] 0x005404e0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00540510  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00540550  // __stdcall, returns int

    VectorClass_MapRegionClass();  // 0x00523694
    virtual ~VectorClass_MapRegionClass();

    // ---- data members (offsets from ctor 0x00525aff) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x1c];
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0x69ad3c
    unsigned char pad_02c[0xc];
    unsigned long m_038;  // /* ? */
    unsigned char pad_03c[0xc];
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x4];
    unsigned long m_050;  // /* ? */
    unsigned char pad_054[0xc];
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned char pad_068[0x14];
    unsigned long m_07c;  // init 0x6a1dc4
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // /* ? */
    unsigned char pad_088[0x1];
    unsigned long m_089;  // zero-initialised
    unsigned char pad_08d[0xb];
    unsigned long m_098;  // init 0x699e00
};
