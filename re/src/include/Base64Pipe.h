#pragma once
// ==========================================================================
// Base64Pipe.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class Base64Pipe
// vtable       : 0x006a0440  (5 slots)
// constructor  : 0x004d5a97
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "Pipe.h"

class Base64Pipe : public Pipe
{
public:
    // ---- vtable 0x006a0440 (5 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x004d8060  // __thiscall
    virtual void vfunc_1();  // [1] 0x0041e0f0  // __thiscall
    virtual void vfunc_2();  // [2] 0x0044ab70
    virtual void vfunc_3(int a0);  // [3] 0x00591f40  // __thiscall
    virtual void vfunc_4(int a0, int a1, int a2, int a3);  // [4] 0x0041df60  // __thiscall

    Base64Pipe();  // 0x004d5a97
    virtual ~Base64Pipe();
};
