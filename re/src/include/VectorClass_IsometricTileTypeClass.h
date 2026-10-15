#pragma once
// ==========================================================================
// VectorClass_IsometricTileTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<IsometricTileTypeClass*>
// vtable       : 0x0069f6d0  (6 slots)
// constructor  : 0x004a8bbe
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_IsometricTileTypeClass
{
public:
    // ---- vtable 0x0069f6d0 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1e80  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004afb30  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004afb70  // __thiscall
    virtual void vfunc_3();  // [3] 0x004afc20  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004afc50  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004afc90  // __stdcall, returns int

    VectorClass_IsometricTileTypeClass();  // 0x004a8bbe
    virtual ~VectorClass_IsometricTileTypeClass();

    // ---- data members (offsets from ctor 0x004b1e79) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f6d0
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
