#pragma once
// ==========================================================================
// DynamicVectorClass_AircraftTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<AircraftTypeClass*>
// vtable       : 0x0069f564  (6 slots)
// constructor  : 0x004a8893
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_AircraftTypeClass.h"

class DynamicVectorClass_AircraftTypeClass : public VectorClass_AircraftTypeClass
{
public:
    // ---- vtable 0x0069f564 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004b1b10  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x004aecf0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004aee70  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9e60  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004aef20  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9e90  // __stdcall, returns int

    DynamicVectorClass_AircraftTypeClass();  // 0x004a8893
    virtual ~DynamicVectorClass_AircraftTypeClass();
};
