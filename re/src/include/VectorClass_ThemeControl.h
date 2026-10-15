#pragma once
// ==========================================================================
// VectorClass_ThemeControl.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<ThemeControl*>
// vtable       : 0x0069f824  (6 slots)
// constructor  : 0x004a91ce
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_ThemeControl
{
public:
    // ---- vtable 0x0069f824 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ab010  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ab050  // __thiscall
    virtual void vfunc_3();  // [3] 0x004ab100  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ab130  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004ab170  // __stdcall, returns int

    VectorClass_ThemeControl();  // 0x004a91ce
    virtual ~VectorClass_ThemeControl();

    // ---- data members (offsets from ctor 0x004b0ac9) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f824
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
