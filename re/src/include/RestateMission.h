#pragma once
// ==========================================================================
// RestateMission.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RestateMission
// vtable       : 0x006a3a7c  (3 slots)
// constructor  : 0x005a80c0
// vtable store : 0x005a80e6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSEngine.h"

class RestateMission : public MSEngine
{
public:
    // ---- vtable 0x006a3a7c (3 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x005a8d30  // __thiscall
    virtual void vfunc_2();  // [2] 0x0055cc40  // __cdecl

    RestateMission();  // 0x005a80c0
    virtual ~RestateMission();

    // ---- data members (offsets from ctor 0x005a820c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a3a7c
    unsigned char pad_004[0x44];
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x400];
    unsigned long m_44c;  // /* ? */
    unsigned char pad_450[0x18];
    unsigned long m_468;  // /* ? */
    unsigned long m_46c;  // /* ? */
    unsigned long m_470;  // /* ? */
};
