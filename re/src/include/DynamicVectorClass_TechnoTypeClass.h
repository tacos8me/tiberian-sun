#pragma once
// ==========================================================================
// DynamicVectorClass_TechnoTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TechnoTypeClass*>
// vtable       : 0x0069dd6c  (6 slots)
// constructor  : 0x00480e31
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TechnoTypeClass.h"

class DynamicVectorClass_TechnoTypeClass : public VectorClass_TechnoTypeClass
{
public:
    // ---- vtable 0x0069dd6c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0044b210  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0044b390  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x0044acc0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0044b440  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0044acf0  // __stdcall, returns int

    DynamicVectorClass_TechnoTypeClass();  // 0x00480e31
    virtual ~DynamicVectorClass_TechnoTypeClass();

    // ---- data members (offsets from ctor 0x00480aa6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x13];
    unsigned long m_013;  // zero-initialised
    unsigned char pad_017[0x4];
    unsigned long m_01b;  // init 0x1
    unsigned char pad_01f[0x4];
    unsigned long m_023;  // zero-initialised
    unsigned char pad_027[0x1];
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x2];
    unsigned long m_02e;  // zero-initialised
    unsigned char pad_032[0x6];
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
    unsigned char pad_040[0x3];
    unsigned long m_043;  // init 0x1
    unsigned long m_047;  // init 0x1
    unsigned long m_04b;  // zero-initialised
    unsigned char pad_04f[0x1];
    unsigned long m_050;  // /* ? */
    unsigned char pad_054[0x4];
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned char pad_060[0x8];
    unsigned long m_068;  // /* ? */
    unsigned long m_06c;  // /* ? */
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // init 0x69dd6c
    unsigned char pad_078[0xc];
    unsigned long m_084;  // zero-initialised
    unsigned long m_088;  // init 0xa
    unsigned long m_08c;  // init 0x69dd34
    unsigned char pad_090[0xc];
    unsigned long m_09c;  // /* ? */
    unsigned char pad_0a0[0x4];
    unsigned long m_0a4;  // /* ? */
    unsigned long m_0a8;  // /* ? */
    unsigned long m_0ac;  // init 0x69dd50
    unsigned long m_0b0;  // /* ? */
    unsigned char pad_0b4[0x8];
    unsigned long m_0bc;  // /* ? */
    unsigned long m_0c0;  // /* ? */
    unsigned long m_0c4;  // init 0x69dd6c
    unsigned char pad_0c8[0xc];
    unsigned long m_0d4;  // /* ? */
    unsigned long m_0d8;  // /* ? */
    unsigned long m_0dc;  // /* ? */
    unsigned char pad_0e0[0xc];
    unsigned long m_0ec;  // init 0x5
    unsigned long m_0f0;  // /* ? */
    unsigned long m_0f4;  // /* ? */
    unsigned long m_0f8;  // /* ? */
    unsigned long m_0fc;  // /* ? */
    unsigned char pad_100[0x8];
    unsigned long m_108;  // /* ? */
    unsigned char pad_10c[0x4];
    unsigned long m_110;  // /* ? */
    unsigned long m_114;  // /* ? */
    unsigned long m_118;  // /* ? */
    unsigned long m_11c;  // /* ? */
    unsigned long m_120;  // /* ? */
    unsigned long m_124;  // /* ? */
    unsigned char pad_128[0x4];
    unsigned long m_12c;  // /* ? */
    unsigned long m_130;  // /* ? */
    unsigned long m_134;  // /* ? */
    unsigned long m_138;  // zero-initialised
    unsigned char pad_13c[0x4];
    unsigned long m_140;  // /* ? */
    unsigned long m_144;  // /* ? */
    unsigned long m_148;  // /* ? */
    unsigned long m_14c;  // /* ? */
    unsigned char pad_150[0x24];
    unsigned long m_174;  // zero-initialised
    unsigned long m_178;  // /* ? */
    unsigned long m_17c;  // /* ? */
    unsigned char pad_180[0x8];
    unsigned long m_188;  // /* ? */
    unsigned char pad_18c[0x4];
    unsigned long m_190;  // /* ? */
    unsigned long m_194;  // /* ? */
    unsigned long m_198;  // /* ? */
    unsigned long m_19c;  // /* ? */
    unsigned char pad_1a0[0x4c];
    unsigned long m_1ec;  // /* ? */
    unsigned long m_1f0;  // /* ? */
    unsigned char pad_1f4[0x7c];
    unsigned long m_270;  // /* ? */
    unsigned long m_274;  // /* ? */
    unsigned char pad_278[0x20];
    unsigned long m_298;  // /* ? */
    unsigned char pad_29c[0xc];
    unsigned long m_2a8;  // /* ? */
    unsigned char pad_2ac[0x4];
    unsigned long m_2b0;  // /* ? */
    unsigned char pad_2b4[0x54];
    unsigned long m_308;  // zero-initialised
};
