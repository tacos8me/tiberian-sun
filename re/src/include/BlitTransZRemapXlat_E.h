#pragma once
// ==========================================================================
// BlitTransZRemapXlat_E.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransZRemapXlat<E>
// vtable       : 0x0069b794  (3 slots)
// constructor  : 0x00460c6a
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransZRemapXlat_E : public Blitter
{
public:
    // ---- vtable 0x0069b794 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004698f0  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00463460  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x004634b0  // __stdcall

    BlitTransZRemapXlat_E();  // 0x00460c6a
    virtual ~BlitTransZRemapXlat_E();

    // ---- data members (offsets from ctor 0x00460be1) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b7b4
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
