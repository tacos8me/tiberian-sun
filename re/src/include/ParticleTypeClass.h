#pragma once
// ==========================================================================
// ParticleTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ParticleTypeClass
// vtable       : 0x006a3454  (6 slots)
// constructor  : 0x00597add
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class ParticleTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a3454 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    ParticleTypeClass();  // 0x00597add
    virtual ~ParticleTypeClass();

    // ---- data members (offsets from ctor 0x005979c0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3470
    unsigned long m_004;  // init 0x6a3454
    unsigned char pad_008[0xc5];
    unsigned long m_0cd;  // /* ? */
    unsigned char pad_0d1[0x53];
    unsigned long m_124;  // /* ? */
    unsigned long m_128;  // /* ? */
    unsigned long m_12c;  // /* ? */
    unsigned long m_130;  // /* ? */
    unsigned long m_134;  // /* ? */
    unsigned long m_138;  // /* ? */
    unsigned long m_13c;  // /* ? */
    unsigned long m_140;  // /* ? */
    unsigned long m_144;  // /* ? */
    unsigned char pad_148[0x1c];
    unsigned long m_164;  // /* ? */
    unsigned long m_168;  // /* ? */
    unsigned long m_16c;  // /* ? */
    unsigned long m_170;  // /* ? */
    unsigned long m_174;  // /* ? */
    unsigned long m_178;  // /* ? */
    unsigned long m_17c;  // /* ? */
    unsigned long m_180;  // /* ? */
    unsigned long m_184;  // /* ? */
    unsigned long m_188;  // /* ? */
    unsigned long m_18c;  // /* ? */
    unsigned long m_190;  // /* ? */
    unsigned long m_194;  // /* ? */
    unsigned long m_198;  // /* ? */
    unsigned long m_19c;  // /* ? */
    unsigned long m_1a0;  // /* ? */
    unsigned long m_1a4;  // /* ? */
};
