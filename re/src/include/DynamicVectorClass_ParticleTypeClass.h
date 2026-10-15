#pragma once
// ==========================================================================
// DynamicVectorClass_ParticleTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ParticleTypeClass*>
// vtable       : 0x0069f6ec  (6 slots)
// constructor  : 0x004a8c13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ParticleTypeClass.h"

class DynamicVectorClass_ParticleTypeClass : public VectorClass_ParticleTypeClass
{
public:
    // ---- vtable 0x0069f6ec (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1f70  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004afd90  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aff10  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004aa090  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004affc0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004aa0c0  // __stdcall, returns int

    DynamicVectorClass_ParticleTypeClass();  // 0x004a8c13
    virtual ~DynamicVectorClass_ParticleTypeClass();
};
