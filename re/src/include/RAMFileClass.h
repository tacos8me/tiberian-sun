#pragma once
// ==========================================================================
// RAMFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class RAMFileClass
// vtable       : 0x006a3958  (17 slots)
// constructor  : 0x005a5990
// vtable store : 0x005a59ba  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "FileClass.h"

class RAMFileClass : public FileClass
{
public:
    // ---- vtable 0x006a3958 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x005a5c90  // __cdecl, returns int
    virtual void vfunc_2(int a0);  // [2] 0x005a5ca0  // __stdcall
    virtual void vfunc_3();  // [3] 0x005a5a20  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a5a40  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x005a5a60  // __stdcall
    virtual void vfunc_6();  // [6] 0x005a5a70  // __cdecl
    virtual void vfunc_7(int a0);  // [7] 0x005a5a90  // __thiscall
    virtual void vfunc_8(int a0, int a1);  // [8] 0x005a5a80  // __stdcall
    virtual void vfunc_9(int a0, int a1);  // [9] 0x005a5ae0  // __thiscall
    virtual void vfunc_10(int a0, int a1);  // [10] 0x005a5b60  // __thiscall
    virtual void vfunc_11();  // [11] 0x005a5be0  // __cdecl
    virtual void vfunc_12(int a0, int a1);  // [12] 0x005a5bf0  // __thiscall
    virtual void vfunc_13();  // [13] 0x005a5c80  // __cdecl
    virtual void vfunc_14();  // [14] 0x005a5cb0  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x005a5cc0  // __stdcall
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a5cd0  // __stdcall

    RAMFileClass();  // 0x005a5990
    virtual ~RAMFileClass();

    // ---- data members (offsets from ctor 0x005a5990) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x1a: the compiled end of the
    // base sub-object region. Evidence: FileClass at +0x0 (ends +0x1a).
    // 8 recovered offset(s) below +0x1a lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0, +0x4, +0x8, +0xc, +0x10, +0x14, +0x18, +0x19
};
