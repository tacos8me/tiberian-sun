#pragma once
// ==========================================================================
// TClassFactory_class_AnimTypeClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class AnimTypeClass>
// vtable       : 0x006a4c88  (5 slots)
// constructor  : 0x005ec718
// vtable store : 0x005ec73b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "AnimTypeClass.h"

class TClassFactory_class_AnimTypeClass : public IClassFactory
{
public:
    // ---- vtable 0x006a4c88 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005ec740  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005ec7b0  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005ec7d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005ec800  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005ec880  // __stdcall, returns int

    TClassFactory_class_AnimTypeClass();  // 0x005ec718
    virtual ~TClassFactory_class_AnimTypeClass();
};
