#pragma once
// ==========================================================================
// BlitTrans_G.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class BlitTrans<G>
// vtable       : 0x006a7c34  (3 slots)
// constructor  : 0x006759e0
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Blitter.h"

class BlitTrans_G : public Blitter
{
public:
    // ---- vtable 0x006a7c34 (3 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00676420  // __thiscall
    virtual void vfunc_1(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);  // [1] 0x00676510  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2, int a3, int a4, int a5, int a6);  // [2] 0x00676540  // __stdcall

    BlitTrans_G();  // 0x006759e0
    virtual ~BlitTrans_G();

    // ---- data members (offsets from ctor 0x00675a63) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned char pad_000[0x4];
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // /* ? */
    unsigned char pad_010[0x4];
    unsigned long m_014;  // /* ? */
    unsigned long m_018;  // /* ? */
    unsigned long m_01c;  // /* ? */
    unsigned long m_020;  // /* ? */
    unsigned char pad_024[0x14];
    unsigned long m_038;  // init 0x6a7c34
    unsigned char pad_03c[0xc];
    unsigned long m_048;  // init 0x6a7c44
};
