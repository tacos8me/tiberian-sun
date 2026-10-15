#pragma once
// ==========================================================================
// CacheStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CacheStraw
// vtable       : 0x006a0420  (3 slots)
// constructor  : 0x004d47ba
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class CacheStraw : public Straw
{
public:
    // ---- vtable 0x006a0420 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004d7ff0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f1020  // __thiscall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0046dfa0  // __thiscall

    CacheStraw();  // 0x004d47ba
    virtual ~CacheStraw();

    // ---- data members (offsets from ctor 0x004d4745) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x13];
    unsigned long m_013;  // zero-initialised
    unsigned char pad_017[0x4];
    unsigned long m_01b;  // /* ? */
    unsigned char pad_01f[0x9];
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x14];
    unsigned long m_040;  // init 0x69bbdc
    unsigned long m_044;  // /* ? */
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0xc];
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned char pad_060[0x218];
    unsigned long m_278;  // zero-initialised
    unsigned char pad_27c[0x204];
    unsigned long m_480;  // /* ? */
};
