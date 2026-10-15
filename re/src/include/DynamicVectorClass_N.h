#pragma once
// ==========================================================================
// DynamicVectorClass_N.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<_N>
// vtable       : 0x0069bc38  (6 slots)
// constructor  : 0x00477e23
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_N.h"

class DynamicVectorClass_N : public VectorClass_N
{
public:
    // ---- vtable 0x0069bc38 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00478a50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00478bd0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00478a00  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00478c80  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00478a30  // __stdcall, returns int

    DynamicVectorClass_N();  // 0x00477e23
    virtual ~DynamicVectorClass_N();

    // ---- data members (offsets from ctor 0x00477b70) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x13];
    unsigned long m_013;  // zero-initialised
    unsigned char pad_017[0x1];
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // init 0x698730
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x8];
    unsigned long m_02c;  // /* ? */
    unsigned char pad_030[0x1];
    unsigned long m_031;  // zero-initialised
    unsigned char pad_035[0x3];
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned char pad_040[0x4];
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // zero-initialised
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // init 0x69bc38
    unsigned long m_068;  // /* ? */
    unsigned long m_06c;  // /* ? */
    unsigned char pad_070[0x1];
    unsigned long m_071;  // zero-initialised
    unsigned char pad_075[0x3];
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // init 0x699e00
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // init 0x699dc8
    unsigned long m_088;  // /* ? */
    unsigned long m_08c;  // /* ? */
    unsigned char pad_090[0x4];
    unsigned long m_094;  // /* ? */
    unsigned long m_098;  // /* ? */
    unsigned long m_09c;  // /* ? */
    unsigned char pad_0a0[0xc];
    unsigned long m_0ac;  // /* ? */
    unsigned long m_0b0;  // /* ? */
    unsigned long m_0b4;  // /* ? */
    unsigned long m_0b8;  // /* ? */
    unsigned long m_0bc;  // zero-initialised
    unsigned long m_0c0;  // /* ? */
    unsigned long m_0c4;  // /* ? */
    unsigned long m_0c8;  // /* ? */
    unsigned char pad_0cc[0x4];
    unsigned long m_0d0;  // /* ? */
    unsigned long m_0d4;  // /* ? */
    unsigned long m_0d8;  // /* ? */
    unsigned char pad_0dc[0x4];
    unsigned long m_0e0;  // /* ? */
    unsigned char pad_0e4[0x4];
    unsigned long m_0e8;  // /* ? */
    unsigned long m_0ec;  // /* ? */
    unsigned long m_0f0;  // /* ? */
};
