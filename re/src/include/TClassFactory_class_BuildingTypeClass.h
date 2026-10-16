#pragma once
// ==========================================================================
// TClassFactory_class_BuildingTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class BuildingTypeClass>
// vtable       : 0x006a4b68  (5 slots)
// constructor  : 0x005eb518
// vtable store : 0x005eb53b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "BuildingTypeClass.h"

class TClassFactory_class_BuildingTypeClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4b68 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005eb540  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005eb5b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005eb5d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005eb600  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005eb680  // __stdcall, returns int

    TClassFactory_class_BuildingTypeClass();  // 0x005eb518
    virtual ~TClassFactory_class_BuildingTypeClass();

    // ---- data members (offsets from ctor 0x005eb521) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4b68
    unsigned long m_004;  // zero-initialised
};
