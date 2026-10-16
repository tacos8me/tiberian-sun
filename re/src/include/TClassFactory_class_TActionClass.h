#pragma once
// ==========================================================================
// TClassFactory_class_TActionClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class TActionClass>
// vtable       : 0x006a4a60  (5 slots)
// constructor  : 0x005ea480
// vtable store : 0x005ea4bb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "TActionClass.h"

class TClassFactory_class_TActionClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4a60 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ea4c0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ea530  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ea550  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ea580  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ea600  // __stdcall, returns int

    TClassFactory_class_TActionClass();  // 0x005ea480
    virtual ~TClassFactory_class_TActionClass();

    // ---- data members (offsets from ctor 0x005ea4a1) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4a60
    unsigned long m_004;  // zero-initialised
};
