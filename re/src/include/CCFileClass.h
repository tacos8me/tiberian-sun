#pragma once
// ==========================================================================
// CCFileClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class CCFileClass
// vtable       : 0x00699bc8  (17 slots)
// constructor  : 0x00418e88
// vtable store : 0x00418ec3  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "CDFileClass.h"

class CCFileClass : public CDFileClass
{
public:
    // ---- vtable 0x00699bc8 (17 slots) ----
    virtual void vfunc_1();  // [1] 0x00419090  // __thiscall, returns ?
    virtual void vfunc_2(int a0);  // [2] 0x0044e320  // __thiscall, returns ?
    virtual void vfunc_3();  // [3] 0x005a6850  // __thiscall, returns int
    virtual void vfunc_4();  // [4] 0x005a6890  // __thiscall, returns int
    virtual void vfunc_5(int a0);  // [5] 0x004475d0  // __thiscall, returns char
    virtual void vfunc_6();  // [6] 0x00447610  // __thiscall, returns char
    virtual void vfunc_7(int a0);  // [7] 0x00447650  // __thiscall, returns int
    virtual void vfunc_8(int a0, int a1);  // [8] 0x004190d0  // __thiscall, returns ?
    virtual void vfunc_9(int a0, int a1);  // [9] 0x00447490  // __thiscall, returns ?
    virtual void vfunc_10(int a0, int a1);  // [10] 0x00447520  // __thiscall, returns ?
    virtual void vfunc_11();  // [11] 0x00447580  // __thiscall, returns ?
    virtual void vfunc_12(int a0, int a1);  // [12] 0x00447460  // __thiscall, returns ?
    virtual void vfunc_13();  // [13] 0x00447620  // __thiscall, returns void
    virtual void vfunc_14();  // [14] 0x00447780  // __thiscall, returns ?
    virtual void vfunc_15(int a0);  // [15] 0x00447830  // __thiscall, returns ?
    virtual void vfunc_16(int a0, int a1, int a2);  // [16] 0x00447430  // __stdcall, returns ?

    CCFileClass();  // 0x00418e88
    virtual ~CCFileClass();

    // ---- data members (offsets from ctor 0x004190f0) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x50: the compiled end of the
    // base sub-object region. Evidence: CDFileClass at +0x0 (ends +0x50).
    // 1 recovered offset(s) below +0x50 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char pad_050[0x10];
    unsigned int m_060;  // +0x060 zero-initialised; 4-byte store: mov dword ptr [esi + 0x60], 0
};
