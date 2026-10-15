#pragma once
// ==========================================================================
// DynamicVectorClass_class_BaseNodeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<class BaseNodeClass>
// vtable       : 0x00699de4  (6 slots)
// constructor  : 0x0041e8d5
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_class_BaseNodeClass.h"

class DynamicVectorClass_class_BaseNodeClass : public VectorClass_class_BaseNodeClass
{
public:
    // ---- vtable 0x00699de4 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x0041f430  // __stdcall
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x0041f490  // __thiscall
    virtual void vfunc_3();  // [3] 0x0041f390  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x0041f570  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x0041f3c0  // __stdcall, returns int

    DynamicVectorClass_class_BaseNodeClass();  // 0x0041e8d5
    virtual ~DynamicVectorClass_class_BaseNodeClass();

    // ---- data members (offsets from ctor 0x0041e8b5) ----
    // Offsets are exact; names are not yet recovered, so members
    // are emitted as byte blobs at their real displacements.
    unsigned long m_000;  // init 0x699de4
    unsigned char pad_004[0xc];
    unsigned long m_010;  // /* ? */
    unsigned long m_014;  // init 0xa
};
