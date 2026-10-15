#pragma once
// ==========================================================================
// Pipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Pipe
// vtable       : 0x0069bc04  (5 slots)
// constructor  : 0x00476e3f
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class Pipe
{
public:
    // ---- vtable 0x0069bc04 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004789b0  // __thiscall
    virtual void vfunc_1();  // [1] 0x00591fb0  // __cdecl, returns int
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3(int a0);  // [3] 0x00591f40  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x00591f90  // __stdcall

    Pipe();  // 0x00476e3f
    virtual ~Pipe();

    // ---- data members (offsets from ctor 0x00476cbc) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x2];
    unsigned long m_01a;  // /* ? */
    unsigned char pad_01e[0x2];
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x10];
    unsigned long m_034;  // init 0x69bbec
    unsigned long m_038;  // init 0x69bc04
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
    unsigned char pad_044[0x4];
    unsigned long m_048;  // /* ? */
    unsigned char pad_04c[0x4];
    unsigned long m_050;  // /* ? */
};
