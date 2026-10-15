#pragma once
// ==========================================================================
// VectorClass_HWND.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<HWND__*>
// vtable       : 0x006a2748  (6 slots)
// constructor  : 0x0055f6ce
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_HWND
{
public:
    // ---- vtable 0x006a2748 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x005600d0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x0055fb40  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x0055fb80  // __thiscall
    virtual void vfunc_3();  // [3] 0x0055fc30  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0055fc60  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x0055fca0  // __stdcall, returns int

    VectorClass_HWND();  // 0x0055f6ce
    virtual ~VectorClass_HWND();

    // ---- data members (offsets from ctor 0x005600c6) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a2748
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
