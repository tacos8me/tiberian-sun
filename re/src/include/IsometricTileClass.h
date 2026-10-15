#pragma once
// ==========================================================================
// IsometricTileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IsometricTileClass
// vtable       : 0x006a0bcc  (6 slots)
// constructor  : 0x004e90b8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ObjectClass.h"

class IsometricTileClass : public ObjectClass
{
public:
    // ---- vtable 0x006a0bcc (6 slots) ----
    virtual void vfunc_0();  // [0] 0x00405af0
    virtual void vfunc_1();  // [1] 0x00405b00
    virtual void vfunc_2();  // [2] 0x00405b10
    virtual void vfunc_3();  // [3] 0x00405740
    virtual void vfunc_4();  // [4] 0x00405750
    virtual void vfunc_5();  // [5] 0x00405760

    IsometricTileClass();  // 0x004e90b8
    virtual ~IsometricTileClass();

    // ---- data members (offsets from ctor 0x004e9090) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a0be8
    unsigned long m_004;  // init 0x6a0bcc
    unsigned char pad_008[0x27];
    unsigned long m_02f;  // zero-initialised
    unsigned char pad_033[0x19];
    unsigned long m_04c;  // /* ? */
};
