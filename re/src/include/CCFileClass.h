#pragma once
// ==========================================================================
// CCFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CCFileClass
// vtable       : 0x00699bc8  (17 slots)
// constructor  : 0x00418ec3
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "CDFileClass.h"

class CCFileClass : public CDFileClass
{
public:
    // ---- vtable 0x00699bc8 (17 slots) ----
    virtual void vfunc_0();  // [0] 0x004190f0
    virtual void vfunc_1();  // [1] 0x00419090
    virtual void vfunc_2();  // [2] 0x0044e320
    virtual void vfunc_3();  // [3] 0x005a6850
    virtual void vfunc_4();  // [4] 0x005a6890
    virtual void vfunc_5();  // [5] 0x004475d0
    virtual void vfunc_6();  // [6] 0x00447610
    virtual void vfunc_7();  // [7] 0x00447650
    virtual void vfunc_8();  // [8] 0x004190d0
    virtual void vfunc_9();  // [9] 0x00447490
    virtual void vfunc_10();  // [10] 0x00447520
    virtual void vfunc_11();  // [11] 0x00447580
    virtual void vfunc_12();  // [12] 0x00447460
    virtual void vfunc_13();  // [13] 0x00447620
    virtual void vfunc_14();  // [14] 0x00447780
    virtual void vfunc_15();  // [15] 0x00447830
    virtual void vfunc_16();  // [16] 0x00447430

    CCFileClass();  // 0x00418ec3
    virtual ~CCFileClass();

    // ---- data members (offsets from ctor 0x00418e88) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x4];
    unsigned long m_004;  // init 0x699bc8
    unsigned char pad_008[0x5c];
    unsigned long m_064;  // zero-initialised
};
