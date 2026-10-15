#pragma once
// ==========================================================================
// VectorClass_ButtonFadeEffect.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ButtonFadeEffect*>
// vtable       : 0x0069dcf0  (6 slots)
// constructor  : 0x0048338e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ButtonFadeEffect
{
public:
    // ---- vtable 0x0069dcf0 (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00483f00
    virtual void vfunc_1();  // [1] 0x004839f0
    virtual void vfunc_2();  // [2] 0x00483a30
    virtual void vfunc_3();  // [3] 0x00483ae0
    virtual void vfunc_4();  // [4] 0x00483b10
    virtual void vfunc_5();  // [5] 0x00483b50

    VectorClass_ButtonFadeEffect();  // 0x0048338e
    virtual ~VectorClass_ButtonFadeEffect();

    // ---- data members (offsets from ctor 0x004832f2) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x74];
    unsigned long m_074;  // /* ? */
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // /* ? */
    unsigned char pad_080[0x1];
    unsigned long m_081;  // zero-initialised
    unsigned char pad_085[0x7];
    unsigned long m_08c;  // init 0x69dd0c
    unsigned long m_090;  // /* ? */
    unsigned long m_094;  // /* ? */
    unsigned char pad_098[0x1];
    unsigned long m_099;  // zero-initialised
    unsigned char pad_09d[0xf];
    unsigned long m_0ac;  // init 0x69dcf0
    unsigned long m_0b0;  // /* ? */
    unsigned long m_0b4;  // /* ? */
    unsigned char pad_0b8[0x1];
    unsigned long m_0b9;  // zero-initialised
    unsigned char pad_0bd[0x7];
    unsigned long m_0c4;  // /* ? */
    unsigned long m_0c8;  // /* ? */
    unsigned long m_0cc;  // /* ? */
    unsigned char pad_0d0[0x1];
    unsigned long m_0d1;  // zero-initialised
    unsigned char pad_0d5[0x237];
    unsigned long m_30c;  // init 0x699bc8
    unsigned char pad_310[0x5c];
    unsigned long m_36c;  // /* ? */
};
