#pragma once
// ==========================================================================
// DynamicVectorClass_MultiMission.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<MultiMission*>
// vtable       : 0x006a41f0  (6 slots)
// constructor  : 0x005d36d3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_MultiMission.h"

class DynamicVectorClass_MultiMission : public VectorClass_MultiMission
{
public:
    // ---- vtable 0x006a41f0 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005d7280  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005d6810  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x005d6990  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x005d6370  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x005d6a40  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x005d63a0  // __stdcall, returns int

    DynamicVectorClass_MultiMission();  // 0x005d36d3
    virtual ~DynamicVectorClass_MultiMission();

    // ---- data members (offsets from ctor 0x005d3692) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // /* ? */
    unsigned long m_004;  // init 0x2
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0x4];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x2c];
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned char pad_060[0x10];
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // /* ? */
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // /* ? */
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // init 0x1
    unsigned long m_088;  // init 0x8
    unsigned long m_08c;  // /* ? */
    unsigned long m_090;  // init 0x5
    unsigned long m_094;  // init 0x3
    unsigned char pad_098[0x10];
    unsigned long m_0a8;  // /* ? */
    unsigned long m_0ac;  // /* ? */
    unsigned long m_0b0;  // /* ? */
    unsigned char pad_0b4[0x4];
    unsigned long m_0b8;  // init 0x6a41f0
    unsigned long m_0bc;  // /* ? */
    unsigned long m_0c0;  // /* ? */
    unsigned long m_0c4;  // init 0x1
    unsigned long m_0c8;  // /* ? */
    unsigned long m_0cc;  // /* ? */
    unsigned char pad_0d0[0x244];
    unsigned long m_314;  // init 0x6a41d4
    unsigned long m_318;  // /* ? */
    unsigned long m_31c;  // /* ? */
    unsigned long m_320;  // init 0x1
    unsigned long m_324;  // /* ? */
    unsigned long m_328;  // /* ? */
    unsigned char pad_32c[0xb9a];
    unsigned long m_ec6;  // /* ? */
    unsigned char pad_eca[0x6e];
    unsigned long m_f38;  // /* ? */
    unsigned char pad_f3c[0x354];
    unsigned long m_1290;  // /* ? */
    unsigned long m_1294;  // /* ? */
    unsigned char pad_1298[0xa];
    unsigned long m_12a2;  // /* ? */
    unsigned char pad_12a6[0x1e8];
    unsigned long m_148e;  // /* ? */
    unsigned char pad_1492[0x206];
    unsigned long m_1698;  // init 0x208
    unsigned long m_169c;  // /* ? */
    unsigned long m_16a0;  // /* ? */
    unsigned long m_16a4;  // /* ? */
    unsigned long m_16a8;  // init 0x1
    unsigned long m_16ac;  // /* ? */
    unsigned long m_16b0;  // /* ? */
    unsigned long m_16b4;  // /* ? */
    unsigned long m_16b8;  // /* ? */
    unsigned long m_16bc;  // /* ? */
    unsigned long m_16c0;  // init 0x1
    unsigned long m_16c4;  // /* ? */
    unsigned long m_16c8;  // /* ? */
    unsigned char pad_16cc[0x18];
    unsigned long m_16e4;  // /* ? */
    unsigned long m_16e8;  // /* ? */
    unsigned long m_16ec;  // /* ? */
    unsigned long m_16f0;  // init 0x2f8
    unsigned long m_16f4;  // init 0x3
    unsigned long m_16f8;  // init 0x2580
    unsigned long m_16fc;  // /* ? */
    unsigned long m_1700;  // /* ? */
    unsigned long m_1704;  // /* ? */
    unsigned char pad_1708[0x58];
    unsigned long m_1760;  // /* ? */
    unsigned long m_1764;  // init 0x6a419c
    unsigned long m_1768;  // /* ? */
    unsigned long m_176c;  // /* ? */
    unsigned long m_1770;  // init 0x1
    unsigned long m_1774;  // /* ? */
    unsigned long m_1778;  // /* ? */
    unsigned char pad_177c[0x18];
    unsigned long m_1794;  // init 0x6a1720
    unsigned long m_1798;  // /* ? */
    unsigned long m_179c;  // /* ? */
    unsigned long m_17a0;  // init 0x1
    unsigned long m_17a4;  // /* ? */
    unsigned long m_17a8;  // /* ? */
    unsigned long m_17ac;  // init 0x7fffffff
    unsigned long m_17b0;  // /* ? */
    unsigned long m_17b4;  // /* ? */
    unsigned char pad_17b8[0xc];
    unsigned long m_17c4;  // /* ? */
    unsigned char pad_17c8[0x4];
    unsigned long m_17cc;  // /* ? */
    unsigned long m_17d0;  // /* ? */
    unsigned long m_17d4;  // /* ? */
};
