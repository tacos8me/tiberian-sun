#pragma once
// ==========================================================================
// DynamicVectorClass_ParticleClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ParticleClass*>
// vtable       : 0x0069f104  (6 slots)
// constructor  : 0x004a7e13
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ParticleClass.h"

class DynamicVectorClass_ParticleClass : public VectorClass_ParticleClass
{
public:
    // ---- vtable 0x0069f104 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004aab50  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aacd0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a95f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aad80  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9620  // __stdcall, returns int

    DynamicVectorClass_ParticleClass();  // 0x004a7e13
    virtual ~DynamicVectorClass_ParticleClass();

    // ---- data members (offsets from ctor 0x0058df57) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x69f104
    unsigned long m_004;  // /* ? */
    unsigned long m_008;  // /* ? */
    unsigned long m_00c;  // init 0x1
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
