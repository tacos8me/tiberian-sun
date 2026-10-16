#pragma once
// ==========================================================================
// MSVQAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSVQAnim
// vtable       : 0x006a24e0  (6 slots)
// constructor  : 0x00558a10
// vtable store : 0x00558a5b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSAnim.h"

class MSVQAnim : public MSAnim
{
public:
    // ---- vtable 0x006a24e0 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x00558c70  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [5] 0x00558dc0

    MSVQAnim();  // 0x00558a10
    virtual ~MSVQAnim();

    // ---- data members (offsets from ctor 0x00558a0c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a24e0
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x4];
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
};
