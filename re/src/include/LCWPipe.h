#pragma once
// ==========================================================================
// LCWPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LCWPipe
// vtable       : 0x006a150c  (5 slots)
// constructor  : 0x004f412c
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class LCWPipe : public Pipe
{
public:
    // ---- vtable 0x006a150c (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004f44a0  // __thiscall
    virtual void vfunc_1();  // [1] 0x004f43e0  // __thiscall
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3(int a0);  // [3] 0x00591f40  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x004f41b0  // __thiscall

    LCWPipe();  // 0x004f412c
    virtual ~LCWPipe();

    // ---- data members (offsets from ctor 0x004f40f7) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a150c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // init 0xffff
};
