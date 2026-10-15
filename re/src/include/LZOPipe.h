#pragma once
// ==========================================================================
// LZOPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LZOPipe
// vtable       : 0x006a1884  (5 slots)
// constructor  : 0x004fb507
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class LZOPipe : public Pipe
{
public:
    // ---- vtable 0x006a1884 (5 slots) ----
    virtual void vfunc_1();  // [1] 0x004fb810  // __thiscall
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3(int a0);  // [3] 0x00591f40  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x004fb580  // __thiscall

    LZOPipe();  // 0x004fb507
    virtual ~LZOPipe();

    // ---- data members (offsets from ctor 0x004fb4d9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a1884
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
