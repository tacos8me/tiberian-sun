#pragma once
// ==========================================================================
// FileStraw.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class FileStraw
// vtable       : 0x0069ad0c  (3 slots)
// constructor  : 0x00447b77
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Straw.h"

class FileStraw : public Straw
{
public:
    // ---- vtable 0x0069ad0c (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x0044aba0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x005f1020  // __thiscall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x006742c0  // __thiscall

    FileStraw();  // 0x00447b77
    virtual ~FileStraw();

    // ---- data members (offsets from ctor 0x00447b37) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x14];
    unsigned long m_014;  // init 0x69ad0c
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned long m_024;  // /* ? */
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
