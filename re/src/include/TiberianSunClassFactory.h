#pragma once
// ==========================================================================
// TiberianSunClassFactory.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TiberianSunClassFactory
// vtable       : 0x0069f804  (5 slots)
// constructor  : 0x004a8ff0
// vtable store : 0x004a8ff6  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"

class TiberianSunClassFactory : public IClassFactory
{
public:
    // ---- vtable 0x0069f804 (5 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x00419410  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x004a9460  // __stdcall, returns int
    virtual void Release(int a0);  // [2] 0x004a9470  // __stdcall, returns int
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x00419470  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x004a9480  // __stdcall, returns int

    TiberianSunClassFactory();  // 0x004a8ff0
    virtual ~TiberianSunClassFactory();
};
