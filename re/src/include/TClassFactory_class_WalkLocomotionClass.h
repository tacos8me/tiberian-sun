#pragma once
// ==========================================================================
// TClassFactory_class_WalkLocomotionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class WalkLocomotionClass>
// vtable       : 0x006a4d30  (5 slots)
// constructor  : 0x005ed1bb
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "WalkLocomotionClass.h"

class TClassFactory_class_WalkLocomotionClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4d30 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ed1c0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ed230  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ed250  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ed280  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ed300  // __stdcall, returns int

    TClassFactory_class_WalkLocomotionClass();  // 0x005ed1bb
    virtual ~TClassFactory_class_WalkLocomotionClass();

    // ---- data members (offsets from ctor 0x005ed1a1) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4d30
    unsigned long m_004;  // zero-initialised
};
