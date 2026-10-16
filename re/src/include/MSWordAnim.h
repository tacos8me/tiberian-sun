#pragma once
// ==========================================================================
// MSWordAnim.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class MSWordAnim
// vtable       : 0x006a2454  (6 slots)
// constructor  : 0x00555578
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "MSPrintAnim.h"

class MSWordAnim : public MSPrintAnim
{
public:
    // ---- vtable 0x006a2454 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00557690  // __stdcall
    virtual void vfunc_2();  // [2] 0x005576a0  // __thiscall
    virtual void vfunc_3();  // [3] 0x005576f0  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005595a0  // __thiscall
    virtual void vfunc_5(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [5] 0x005597a0  // __thiscall

    MSWordAnim();  // 0x00555578
    virtual ~MSWordAnim();

    // ---- data members (offsets from ctor 0x005554f3) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2454
    unsigned char pad_004[0x44];
    unsigned long m_048;  // /* ? */
    unsigned long m_04c;  // /* ? */
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
};
