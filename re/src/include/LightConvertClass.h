#pragma once
// ==========================================================================
// LightConvertClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LightConvertClass
// vtable       : 0x006a163c  (2 slots)
// constructor  : 0x004f6250
// vtable store : 0x004f62d5  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConvertClass.h"

class LightConvertClass : public ConvertClass
{
public:
    // ---- vtable 0x006a163c (2 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3);  // [1] 0x004f63e0  // __thiscall

    LightConvertClass();  // 0x004f6250
    virtual ~LightConvertClass();

    // ---- data members (offsets from ctor 0x004f6250) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a163c
    unsigned char pad_004[0x184];
    unsigned long m_188;  // /* ? */
    unsigned long m_18c;  // /* ? */
    unsigned long m_190;  // /* ? */
    unsigned long m_194;  // zero-initialised
    unsigned long m_198;  // /* ? */
    unsigned long m_19c;  // /* ? */
    unsigned long m_1a0;  // /* ? */
    unsigned long m_1a4;  // /* ? */
    unsigned long m_1a8;  // /* ? */
    unsigned long m_1ac;  // /* ? */
    unsigned long m_1b0;  // zero-initialised
};
