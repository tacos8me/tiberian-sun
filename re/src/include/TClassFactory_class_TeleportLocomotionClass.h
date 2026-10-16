#pragma once
// ==========================================================================
// TClassFactory_class_TeleportLocomotionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TeleportLocomotionClass>
// vtable       : 0x006a4d78  (5 slots)
// constructor  : 0x005ed63b
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TeleportLocomotionClass.h"

class TClassFactory_class_TeleportLocomotionClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4d78 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ed640  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ed6b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ed6d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ed700  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ed780  // __stdcall, returns int

    TClassFactory_class_TeleportLocomotionClass();  // 0x005ed63b
    virtual ~TClassFactory_class_TeleportLocomotionClass();

    // ---- data members (offsets from ctor 0x005ed621) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4d78
    unsigned long m_004;  // zero-initialised
};
