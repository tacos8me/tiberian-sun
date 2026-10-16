#pragma once
// ==========================================================================
// MSShapeAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSShapeAnim
// vtable       : 0x006a248c  (6 slots)
// constructor  : 0x00557ac0
// vtable store : 0x00557b20  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSAnim.h"

class MSShapeAnim : public MSAnim
{
public:
    // ---- vtable 0x006a248c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2);  // [4] 0x00557c30  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5);  // [5] 0x00557f90  // __thiscall

    MSShapeAnim();  // 0x00557ac0
    virtual ~MSShapeAnim();

    // ---- data members (offsets from ctor 0x00557adf) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a248c
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned char pad_014[0x4];
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned long m_03c;  // /* ? */
};
