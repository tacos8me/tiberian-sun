#pragma once
// ==========================================================================
// PKPipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class PKPipe
// vtable       : 0x006a323c  (6 slots)
// constructor  : 0x00592d5b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class PKPipe : public Pipe
{
public:
    // ---- vtable 0x006a323c (6 slots) ----
    virtual void vfunc_1();  // [1] 0x00591fb0  // __cdecl, returns int
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3(int a0);  // [3] 0x00592d70  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2);  // [4] 0x00592e20  // __thiscall
    virtual void vfunc_5(int a0);  // [5] 0x00593040  // __stdcall

    PKPipe();  // 0x00592d5b
    virtual ~PKPipe();

    // ---- data members (offsets from ctor 0x00592d0c) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a323c
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0x6a3258
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x8];
    unsigned long m_02c;  // /* ? */
    unsigned long m_030;  // /* ? */
    unsigned long m_034;  // /* ? */
    unsigned long m_038;  // /* ? */
    unsigned char pad_03c[0x100];
    unsigned long m_13c;  // /* ? */
    unsigned long m_140;  // /* ? */
};
