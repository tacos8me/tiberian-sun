#pragma once
// ==========================================================================
// BlitTrans_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTrans<E>
// vtable       : 0x006a7c44  (3 slots)
// constructor  : 0x006759c1
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTrans_E : public Blitter
{
public:
    // ---- vtable 0x006a7c44 (3 slots) ----
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00676390  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x00676440  // __stdcall

    BlitTrans_E();  // 0x006759c1
    virtual ~BlitTrans_E();

    // ---- data members (offsets from ctor 0x00675901) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x10];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned char pad_01c[0x4];
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned char pad_030[0x48];
    unsigned long m_078;  // init 0x6a7c44
};
