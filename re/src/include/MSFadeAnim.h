#pragma once
// ==========================================================================
// MSFadeAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSFadeAnim
// vtable       : 0x006a24a8  (6 slots)
// constructor  : 0x00557ffc
// vtable store : 0x00558110  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSShapeAnim.h"

class MSFadeAnim : public MSShapeAnim
{
public:
    // ---- vtable 0x006a24a8 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00558120  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5);  // [5] 0x00558500  // __thiscall

    MSFadeAnim();  // 0x00557ffc
    virtual ~MSFadeAnim();

    // ---- data members (offsets from ctor 0x005580d4) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a24a8
    unsigned char pad_004[0x3c];
    unsigned long m_040;  // /* ? */
};
