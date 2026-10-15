#pragma once
// ==========================================================================
// Tactical.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Tactical
// vtable       : 0x006a527c  (6 slots)
// constructor  : 0x005f4c4c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AbstractClass.h"

class Tactical : public AbstractClass
{
public:
    // ---- vtable 0x006a527c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    Tactical();  // 0x005f4c4c
    virtual ~Tactical();

    // ---- data members (offsets from ctor 0x005f4b30) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a5298
    unsigned long m_004;  // init 0x6a527c
    unsigned char pad_008[0xc];
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x3c];
    unsigned long m_054;  // init 0xffffffff
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
    unsigned char pad_06c[0x4];
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // init 0x3ff00000
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // /* ? */
    unsigned long m_080;  // /* ? */
    unsigned long m_084;  // /* ? */
    unsigned long m_088;  // /* ? */
    unsigned long m_08c;  // /* ? */
    unsigned long m_090;  // /* ? */
    unsigned long m_094;  // /* ? */
    unsigned char pad_098[0xc80];
    unsigned long m_d18;  // /* ? */
    unsigned long m_d1c;  // /* ? */
    unsigned long m_d20;  // /* ? */
    unsigned long m_d24;  // /* ? */
    unsigned long m_d28;  // /* ? */
    unsigned long m_d2c;  // /* ? */
    unsigned long m_d30;  // init 0x1
    unsigned long m_d34;  // /* ? */
    unsigned char pad_d38[0xc];
    unsigned long m_d44;  // /* ? */
    unsigned long m_d48;  // /* ? */
    unsigned long m_d4c;  // /* ? */
    unsigned long m_d50;  // /* ? */
    unsigned long m_d54;  // /* ? */
    unsigned long m_d58;  // /* ? */
    unsigned char pad_d5c[0x4];
    unsigned long m_d60;  // /* ? */
    unsigned char pad_d64[0x30];
    unsigned long m_d94;  // init 0x40aaaaa4
    unsigned long m_d98;  // /* ? */
    unsigned long m_d9c;  // /* ? */
    unsigned long m_da0;  // /* ? */
    unsigned long m_da4;  // init 0xc0aaaaa4
    unsigned long m_da8;  // /* ? */
    unsigned long m_dac;  // /* ? */
    unsigned long m_db0;  // /* ? */
    unsigned long m_db4;  // /* ? */
    unsigned long m_db8;  // /* ? */
    unsigned long m_dbc;  // init 0x3f800000
    unsigned long m_dc0;  // /* ? */
};
