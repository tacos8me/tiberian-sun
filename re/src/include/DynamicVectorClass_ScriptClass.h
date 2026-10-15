#pragma once
// ==========================================================================
// DynamicVectorClass_ScriptClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<ScriptClass*>
// vtable       : 0x006988d4  (6 slots)
// constructor  : 0x00403c93
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_ScriptClass.h"

class DynamicVectorClass_ScriptClass : public VectorClass_ScriptClass
{
public:
    // ---- vtable 0x006988d4 (6 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00403ff0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00403d40  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00403ec0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00403cf0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00403f70  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00403d20  // __stdcall, returns int

    DynamicVectorClass_ScriptClass();  // 0x00403c93
    virtual ~DynamicVectorClass_ScriptClass();
};
