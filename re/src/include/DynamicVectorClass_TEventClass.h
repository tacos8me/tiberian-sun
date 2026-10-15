#pragma once
// ==========================================================================
// DynamicVectorClass_TEventClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class DynamicVectorClass<TEventClass*>
// vtable       : 0x006a6074  (6 slots)
// constructor  : 0x00625a03
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "VectorClass_TEventClass.h"

class DynamicVectorClass_TEventClass : public VectorClass_TEventClass
{
public:
    // ---- vtable 0x006a6074 (6 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x006269e0  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x00626b60  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x00626990  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00626c10  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x006269c0  // __stdcall, returns int

    DynamicVectorClass_TEventClass();  // 0x00625a03
    virtual ~DynamicVectorClass_TEventClass();
};
