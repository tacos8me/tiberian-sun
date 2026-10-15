#pragma once
// ==========================================================================
// DynamicVectorClass_TeamTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TeamTypeClass*>
// vtable       : 0x0069f414  (6 slots)
// constructor  : 0x004a8593
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TeamTypeClass.h"

class DynamicVectorClass_TeamTypeClass : public VectorClass_TeamTypeClass
{
public:
    // ---- vtable 0x0069f414 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004adeb0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ae030  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9c80  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ae0e0  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9cb0  // __stdcall, returns int

    DynamicVectorClass_TeamTypeClass();  // 0x004a8593
    virtual ~DynamicVectorClass_TeamTypeClass();
};
