#pragma once
// ==========================================================================
// DynamicVectorClass_MapRegionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MapRegionClass*>
// vtable       : 0x006a1da8  (6 slots)
// constructor  : 0x00523663
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MapRegionClass.h"

class DynamicVectorClass_MapRegionClass : public VectorClass_MapRegionClass
{
public:
    // ---- vtable 0x006a1da8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005403f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00540570  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00540350  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00540670  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00540380  // __stdcall, returns int

    DynamicVectorClass_MapRegionClass();  // 0x00523663
    virtual ~DynamicVectorClass_MapRegionClass();

    // ---- data members (offsets from ctor 0x005254bc) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // init 0x699dc8
    unsigned char pad_02c[0xc];
    unsigned long m_038;  // zero-initialised
    unsigned long m_03c;  // init 0x186a0
    unsigned long m_040;  // /* ? */
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x4];
    unsigned long m_050;  // /* ? */
    unsigned char pad_054[0x4];
    unsigned long m_058;  // /* ? */
    unsigned char pad_05c[0x4];
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned char pad_068[0x14];
    unsigned long m_07c;  // init 0x6a1da8
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // /* ? */
    unsigned char pad_088[0x1];
    unsigned long m_089;  // zero-initialised
    unsigned char pad_08d[0x3];
    unsigned long m_090;  // /* ? */
    unsigned char pad_094[0x4];
    unsigned long m_098;  // init 0x699e00
};
