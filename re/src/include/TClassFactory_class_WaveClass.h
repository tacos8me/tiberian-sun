#pragma once
// ==========================================================================
// TClassFactory_class_WaveClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class WaveClass>
// vtable       : 0x006a48b0  (5 slots)
// constructor  : 0x005e8998
// vtable store : 0x005e89bb  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
class WaveClass;

class TClassFactory_class_WaveClass : public IClassFactory
{
public:
    // ---- vtable 0x006a48b0 (5 slots) ----
    virtual void vfunc_0(int a0, int a1, int a2);  // [0] 0x005e89c0  // __stdcall, returns int
    virtual void vfunc_1(int a0);  // [1] 0x005e8a30  // __stdcall
    virtual void vfunc_2(int a0);  // [2] 0x005e8a50  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005e8a80  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005e8b00  // __stdcall, returns int

    TClassFactory_class_WaveClass();  // 0x005e8998
    virtual ~TClassFactory_class_WaveClass();
};
