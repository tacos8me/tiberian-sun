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
    virtual void vfunc_0(int a0);  // [0] 0x00483f00  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004839f0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00483a30  // __thiscall
    virtual void vfunc_3();  // [3] 0x00483ae0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00483b10  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00483b50  // __stdcall, returns int

    VectorClass_ButtonFadeEffect();  // 0x0048338e
    virtual ~VectorClass_ButtonFadeEffect();

    // ---- data members (offsets from ctor 0x0048336b) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x74];
    unsigned long m_074;  // /* ? */
    unsigned long m_078;  // /* ? */
    unsigned long m_07c;  // /* ? */
    unsigned char pad_080[0x1];
    unsigned long m_081;  // zero-initialised
    unsigned char pad_085[0xf];
    unsigned long m_094;  // /* ? */
    unsigned char pad_098[0x1];
    unsigned long m_099;  // zero-initialised
    unsigned char pad_09d[0xf];
    unsigned long m_0ac;  // init 0x69dcf0
    unsigned long m_0b0;  // /* ? */
    unsigned long m_0b4;  // /* ? */
    unsigned char pad_0b8[0x1];
    unsigned long m_0b9;  // zero-initialised
    unsigned char pad_0bd[0x24f];
    unsigned long m_30c;  // init 0x699bc8
    unsigned char pad_310[0x5c];
    unsigned long m_36c;  // /* ? */
};
