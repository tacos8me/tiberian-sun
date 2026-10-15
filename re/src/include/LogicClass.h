#pragma once
// ==========================================================================
// LogicClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class LogicClass
// vtable       : 0x00698704  (10 slots)
// constructor  : 0x00402432
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "LayerClass.h"

class LogicClass : public LayerClass
{
public:
    // ---- vtable 0x00698704 (10 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004024a0  // __thiscall
    virtual void vfunc_1(int a0);  // [1] 0x00402540  // __stdcall
    virtual void vfunc_2(int a0, int a1);  // [2] 0x004026c0  // __thiscall, returns int
    virtual void vfunc_3();  // [3] 0x004024f0  // __thiscall
    virtual void vfunc_4(int a0);  // [4] 0x00402770  // __stdcall
    virtual void vfunc_5(int a0);  // [5] 0x00402520  // __stdcall, returns int
    virtual void vfunc_6(int a0, int a1);  // [6] 0x004fad40  // __stdcall
    virtual void vfunc_7();  // [7] 0x00402480  // __cdecl
    virtual void vfunc_8();  // [8] 0x00402490  // __cdecl
    virtual void vfunc_9(int a0, int a1);  // [9] 0x004fab40  // __stdcall

    LogicClass();  // 0x00402432
    virtual ~LogicClass();
};
