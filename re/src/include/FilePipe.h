#pragma once
// ==========================================================================
// FilePipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FilePipe
// vtable       : 0x0069ad1c  (5 slots)
// constructor  : 0x00447db1
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class FilePipe : public Pipe
{
public:
    // ---- vtable 0x0069ad1c (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0044abc0  // __thiscall
    virtual void vfunc_1();  // [1] 0x00591fb0  // __cdecl, returns int
    virtual void vfunc_2();  // [2] 0x006741e0  // __thiscall
    virtual void vfunc_3(int a0);  // [3] 0x00591f40  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x00674210  // __thiscall

    FilePipe();  // 0x00447db1
    virtual ~FilePipe();

    // ---- data members (offsets from ctor 0x00447d7a) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0xc];
    unsigned long m_00c;  // init 0x69ad1c
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // /* ? */
    unsigned char pad_018[0x4];
    unsigned long m_01c;  // /* ? */
    unsigned char pad_020[0x4];
    unsigned long m_024;  // init 0x69acf4
    unsigned long m_028;  // /* ? */
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned char pad_034[0x14];
    unsigned long m_048;  // init 0x67452301
    unsigned long m_04c;  // init 0xefcdab89
    unsigned long m_050;  // init 0x98badcfe
    unsigned long m_054;  // init 0x10325476
    unsigned long m_058;  // init 0xc3d2e1f0
    unsigned long m_05c;  // /* ? */
    unsigned long m_060;  // /* ? */
};
