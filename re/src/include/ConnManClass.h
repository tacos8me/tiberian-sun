#pragma once
// ==========================================================================
// ConnManClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class ConnManClass
// vtable       : 0x006a0b7c  (16 slots)
// constructor  : 0x004e7e47
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class ConnManClass
{
public:
    // ---- vtable 0x006a0b7c (16 slots) ----
    virtual void vfunc_1();  // [1] 0x006829a5  // __cdecl
    virtual void vfunc_2();  // [2] 0x006829a5  // __cdecl
    virtual void vfunc_3();  // [3] 0x006829a5  // __cdecl
    virtual void vfunc_4();  // [4] 0x006829a5  // __cdecl
    virtual void vfunc_5();  // [5] 0x006829a5  // __cdecl
    virtual void vfunc_6();  // [6] 0x006829a5  // __cdecl
    virtual void vfunc_7();  // [7] 0x006829a5  // __cdecl
    virtual void vfunc_8();  // [8] 0x006829a5  // __cdecl
    virtual void vfunc_9();  // [9] 0x006829a5  // __cdecl
    virtual void vfunc_10();  // [10] 0x006829a5  // __cdecl
    virtual void vfunc_11();  // [11] 0x006829a5  // __cdecl
    virtual void vfunc_12();  // [12] 0x006829a5  // __cdecl
    virtual void vfunc_13();  // [13] 0x006829a5  // __cdecl
    virtual void vfunc_14();  // [14] 0x006829a5  // __cdecl
    virtual void vfunc_15();  // [15] 0x006829a5  // __cdecl

    ConnManClass();  // 0x004e7e47
    virtual ~ConnManClass();

    // ---- data members (offsets from ctor 0x004e7deb) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a0b34
    unsigned char pad_004[0x1];
    unsigned long m_005;  // /* ? */
    unsigned char pad_009[0x33];
    unsigned long m_03c;  // /* ? */
    unsigned long m_040;  // /* ? */
};
