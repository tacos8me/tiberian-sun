#pragma once
// ==========================================================================
// TClassFactory_class_AnimClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class AnimClass>
// vtable       : 0x006a4c70  (5 slots)
// constructor  : 0x005ec5bb
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AnimClass.h"

class TClassFactory_class_AnimClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4c70 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ec5c0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ec630  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ec650  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ec680  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ec700  // __stdcall, returns int

    TClassFactory_class_AnimClass();  // 0x005ec5bb
    virtual ~TClassFactory_class_AnimClass();

    // ---- data members (offsets from ctor 0x005ec5a1) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x6a4c70
    unsigned long m_004;  // zero-initialised
};
