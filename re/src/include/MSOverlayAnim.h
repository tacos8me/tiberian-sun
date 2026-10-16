#pragma once
// ==========================================================================
// MSOverlayAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSOverlayAnim
// vtable       : 0x006a24c4  (6 slots)
// constructor  : 0x0055868d
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSFadeAnim.h"

class MSOverlayAnim : public MSFadeAnim
{
public:
    // ---- vtable 0x006a24c4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2);  // [4] 0x005586a0  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5);  // [5] 0x005588d0  // __thiscall

    MSOverlayAnim();  // 0x0055868d
    virtual ~MSOverlayAnim();

    // ---- data members (offsets from ctor 0x0055865d) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a24c4
    unsigned char pad_004[0x3c];
    unsigned long m_040;  // /* ? */
};
