#pragma once
// ==========================================================================
// BaseClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BaseClass
// vtable       : 0x00699db8  (3 slots)
// constructor  : 0x0041e8c0
// vtable store : 0x0041e943  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class BaseClass
{
public:
    // ---- vtable 0x00699db8 (3 slots) ----
    virtual void vfunc_0(int a0, int a1);  // [0] 0x0041ef90  // __stdcall, returns int
    virtual void vfunc_1(int a0, int a1);  // [1] 0x0041f1b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x0041f2b0  // __thiscall

    BaseClass();  // 0x0041e8c0
    virtual ~BaseClass();

    // ---- data members (offsets from ctor 0x0041e8b5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699db8
    unsigned char pad_004[0x18];
    unsigned long m_01c;  // /* ? */
    unsigned char pad_020[0x30];
    unsigned long m_050;  // /* ? */
    unsigned long m_054;  // /* ? */
    unsigned long m_058;  // /* ? */
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
    unsigned long m_068;  // /* ? */
    unsigned long m_06c;  // /* ? */
    unsigned long m_070;  // /* ? */
    unsigned long m_074;  // /* ? */
};
