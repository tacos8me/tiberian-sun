#pragma once
// ==========================================================================
// TClassFactory_class_VoxelAnimClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class VoxelAnimClass>
// vtable       : 0x006a4a48  (5 slots)
// constructor  : 0x005ea318
// vtable store : 0x005ea33b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
class VoxelAnimClass;

class TClassFactory_class_VoxelAnimClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4a48 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ea340  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ea3b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ea3d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ea400  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ea480  // __stdcall, returns int

    TClassFactory_class_VoxelAnimClass();  // 0x005ea318
    virtual ~TClassFactory_class_VoxelAnimClass();

    // ---- data members (offsets from ctor 0x005ea321) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4a48
    unsigned long m_004;  // zero-initialised
};
