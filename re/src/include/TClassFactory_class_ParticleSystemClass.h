#pragma once
// ==========================================================================
// TClassFactory_class_ParticleSystemClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class ParticleSystemClass>
// vtable       : 0x006a4e38  (5 slots)
// constructor  : 0x005ee218
// vtable store : 0x005ee23b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ParticleSystemClass.h"

class TClassFactory_class_ParticleSystemClass : public IClassFactory, public IUnknown
{
public:
    // ---- vtable 0x006a4e38 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ee240  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ee2b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ee2d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ee300  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ee380  // __stdcall, returns int

    TClassFactory_class_ParticleSystemClass();  // 0x005ee218
    virtual ~TClassFactory_class_ParticleSystemClass();
};
