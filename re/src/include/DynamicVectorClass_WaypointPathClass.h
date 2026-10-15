#pragma once
// ==========================================================================
// DynamicVectorClass_WaypointPathClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<WaypointPathClass*>
// vtable       : 0x006a749c  (6 slots)
// constructor  : 0x00655663
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_WaypointPathClass.h"

class DynamicVectorClass_WaypointPathClass : public VectorClass_WaypointPathClass
{
public:
    // ---- vtable 0x006a749c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x00656490  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00656610  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00655fd0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x006566c0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00656000  // __stdcall, returns int

    DynamicVectorClass_WaypointPathClass();  // 0x00655663
    virtual ~DynamicVectorClass_WaypointPathClass();
};
