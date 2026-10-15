#pragma once
// ==========================================================================
// OverlayTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class OverlayTypeClass
// vtable       : 0x006a2d3c  (6 slots)
// constructor  : 0x00576c87
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectTypeClass.h"

class OverlayTypeClass : public ObjectTypeClass
{
public:
    // ---- vtable 0x006a2d3c (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    OverlayTypeClass();  // 0x00576c87
    virtual ~OverlayTypeClass();

    // ---- data members (offsets from ctor 0x00576bf0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2d3c
};
