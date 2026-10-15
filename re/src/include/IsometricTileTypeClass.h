#pragma once
// ==========================================================================
// IsometricTileTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IsometricTileTypeClass
// vtable       : 0x006a12bc  (6 slots)
// constructor  : 0x004ea1d2
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class IsometricTileTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a12bc (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    IsometricTileTypeClass();  // 0x004ea1d2
    virtual ~IsometricTileTypeClass();

    // ---- data members (offsets from ctor 0x004ea0c0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a12d8
    unsigned long m_004;  // init 0x6a12bc
    unsigned char pad_008[0xc0];
    unsigned long m_0c8;  // init 0x1
    unsigned long m_0cc;  // init 0x1
    unsigned char pad_0d0[0x54];
    unsigned long m_124;  // /* ? */
    unsigned long m_128;  // /* ? */
    unsigned long m_12c;  // /* ? */
    unsigned long m_130;  // /* ? */
    unsigned long m_134;  // init 0x6a137c
    unsigned long m_138;  // /* ? */
    unsigned long m_13c;  // /* ? */
    unsigned long m_140;  // init 0x1
    unsigned long m_144;  // /* ? */
    unsigned long m_148;  // init 0xa
    unsigned long m_14c;  // /* ? */
    unsigned long m_150;  // /* ? */
    unsigned long m_154;  // /* ? */
    unsigned long m_158;  // /* ? */
    unsigned long m_15c;  // /* ? */
    unsigned long m_160;  // /* ? */
    unsigned long m_164;  // /* ? */
    unsigned long m_168;  // /* ? */
    unsigned long m_16c;  // /* ? */
    unsigned long m_170;  // /* ? */
    unsigned long m_174;  // /* ? */
    unsigned long m_178;  // /* ? */
    unsigned long m_17c;  // /* ? */
    unsigned long m_180;  // init 0x1
    unsigned long m_184;  // /* ? */
    unsigned char pad_188[0xd];
    unsigned long m_195;  // init 0x1
};
