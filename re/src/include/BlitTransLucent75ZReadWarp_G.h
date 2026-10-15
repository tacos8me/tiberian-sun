#pragma once
// ==========================================================================
// BlitTransLucent75ZReadWarp_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTransLucent75ZReadWarp<G>
// vtable       : 0x0069b5f4  (3 slots)
// constructor  : 0x004612c8
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTransLucent75ZReadWarp_G : public Blitter
{
public:
    // ---- vtable 0x0069b5f4 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00469c90  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00464740  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x004647e0  // __stdcall

    BlitTransLucent75ZReadWarp_G();  // 0x004612c8
    virtual ~BlitTransLucent75ZReadWarp_G();

    // ---- data members (offsets from ctor 0x004611c7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69b644
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
};
