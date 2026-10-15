#pragma once
// ==========================================================================
// DynamicVectorClass_ScriptTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ScriptTypeClass*>
// vtable       : 0x0069f44c  (6 slots)
// constructor  : 0x004a8613
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ScriptTypeClass.h"

class DynamicVectorClass_ScriptTypeClass : public VectorClass_ScriptTypeClass
{
public:
    // ---- vtable 0x0069f44c (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004ae110  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004ae290  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004a9cd0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x004ae340  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x004a9d00  // __stdcall, returns int

    DynamicVectorClass_ScriptTypeClass();  // 0x004a8613
    virtual ~DynamicVectorClass_ScriptTypeClass();
};
