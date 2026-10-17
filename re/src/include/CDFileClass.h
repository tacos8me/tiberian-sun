#pragma once
// ==========================================================================
// CDFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CDFileClass
// vtable       : 0x00699b80  (17 slots)
// constructor  : 0x00418e88
// vtable store : 0x00418edc  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "BufferIOFileClass.h"

class CDFileClass : public BufferIOFileClass
{
public:
    // ---- vtable 0x00699b80 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x00419090  // __thiscall, returns ?
    virtual void vfunc_2(int a0);  // [2] 0x0044e320  // __thiscall, returns ?
    virtual void vfunc_3();  // [3] 0x005a6850  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a6890  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x00420520  // __thiscall, returns char
    virtual void vfunc_6();  // [6] 0x00420540  // __thiscall, returns char
    virtual void vfunc_7(int a0);  // [7] 0x0044dfe0  // __stdcall, returns ?
    virtual void vfunc_8(int a0, int a1);  // [8] 0x0044e420  // __thiscall, returns char
    virtual void vfunc_9(int a0, int a1, int a2, int a3);  // [9] 0x004208c0  // __thiscall, returns ?
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00420ad0  // __thiscall, returns int
    virtual void vfunc_11();  // [11] 0x00420bc0  // __thiscall, returns ?
    virtual void vfunc_12(int a0, int a1);  // [12] 0x00420670  // __thiscall, returns ?
    virtual void vfunc_13();  // [13] 0x00420be0  // __thiscall, returns ?
    virtual void vfunc_14();  // [14] 0x005a68f0  // __cdecl, returns int
    virtual void vfunc_15(int a0);  // [15] 0x005a6940  // __thiscall, returns int
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x005a6190  // __stdcall, returns void

    CDFileClass();  // 0x00418e88
    virtual ~CDFileClass();

    // ---- data members (offsets from ctor 0x004190a0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x50: the compiled end of the
    // base sub-object region. Evidence: BufferIOFileClass at +0x0 (ends +0x50).
    // 1 recovered offset(s) below +0x50 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
};
