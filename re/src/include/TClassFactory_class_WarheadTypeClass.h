#pragma once
// ==========================================================================
// TClassFactory_class_WarheadTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class WarheadTypeClass>
// vtable       : 0x006a4dd8  (5 slots)
// constructor  : 0x005edc00
// vtable store : 0x005edc3b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "WarheadTypeClass.h"

class TClassFactory_class_WarheadTypeClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4dd8 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005edc40  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005edcb0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005edcd0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005edd00  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005edd80  // __stdcall, returns int

    TClassFactory_class_WarheadTypeClass();  // 0x005edc00
    virtual ~TClassFactory_class_WarheadTypeClass();

    // ---- data members (offsets from ctor 0x005edc21) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4dd8
    unsigned long m_004;  // zero-initialised
};
