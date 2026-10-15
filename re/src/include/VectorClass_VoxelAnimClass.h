#pragma once
// ==========================================================================
// VectorClass_VoxelAnimClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class VectorClass<VoxelAnimClass*>
// vtable       : 0x00698998  (6 slots)
// constructor  : 0x0040437e
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class VectorClass_VoxelAnimClass
{
public:
    // ---- vtable 0x00698998 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00404660  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00404400  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00404440  // __thiscall
    virtual void vfunc_3();  // [3] 0x004044f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00404520  // __stdcall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00404560  // __stdcall, returns int

    VectorClass_VoxelAnimClass();  // 0x0040437e
    virtual ~VectorClass_VoxelAnimClass();

    // ---- data members (offsets from ctor 0x00404656) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x698998
    unsigned long m_004;  // zero-initialised
    unsigned long m_008;  // zero-initialised
    unsigned char pad_00c[0x1];
    unsigned long m_00d;  // zero-initialised
};
