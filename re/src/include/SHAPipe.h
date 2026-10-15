#pragma once
// ==========================================================================
// SHAPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class SHAPipe
// vtable       : 0x0069acf4  (5 slots)
// constructor  : 0x00447c19
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class SHAPipe : public Pipe
{
public:
    // ---- vtable 0x0069acf4 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0044ab80  // __thiscall
    virtual void vfunc_1();  // [1] 0x00591fb0  // __cdecl, returns int
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3(int a0);  // [3] 0x00591f40  // __thiscall
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005d7ba0  // __thiscall

    SHAPipe();  // 0x00447c19
    virtual ~SHAPipe();

    // ---- data members (offsets from ctor 0x00447bc0) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x28];
    unsigned long m_028;  // /* ? */
    unsigned char pad_02c[0x4];
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned char pad_038[0x14];
    unsigned long m_04c;  // init 0x67452301
    unsigned long m_050;  // init 0xefcdab89
    unsigned long m_054;  // init 0x98badcfe
    unsigned long m_058;  // init 0x10325476
    unsigned long m_05c;  // init 0xc3d2e1f0
    unsigned long m_060;  // /* ? */
    unsigned long m_064;  // /* ? */
};
