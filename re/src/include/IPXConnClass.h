#pragma once
// ==========================================================================
// IPXConnClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class IPXConnClass
// vtable       : 0x006a0abc  (10 slots)
// constructor  : 0x004e6e50
// vtable store : 0x004e6e8f  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "ConnectionClass.h"

class IPXConnClass : public ConnectionClass
{
public:
    // ---- vtable 0x006a0abc (10 slots) ----
    virtual void vfunc_1(int a0);  // [1] 0x004e6f10  // returns ?
    virtual void vfunc_2(int a0, int a1, int a2);  // [2] 0x0045eb70  // __thiscall, returns int
    virtual void vfunc_3(int a0, int a1);  // [3] 0x0045ec10  // __thiscall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x0045edf0  // __thiscall, returns int
    virtual void vfunc_5();  // [5] 0x0045ee80  // __thiscall, returns int
    virtual void vfunc_6();  // [6] 0x0045f020  // __cdecl, returns int
    virtual void vfunc_7();  // [7] 0x0045eeb0  // __thiscall, returns int
    virtual void vfunc_8();  // [8] 0x0045f030  // __thiscall, returns int
    virtual void vfunc_9(int a0, int a1);  // [9] 0x004e7000  // __thiscall, returns ?

    IPXConnClass();  // 0x004e6e50
    virtual ~IPXConnClass();

    // ---- data members (offsets from ctor 0x004e6e50) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x20: the compiled end of the
    // base sub-object region. Evidence: ConnectionClass at +0x0 (ends +0x20).
    // 1 recovered offset(s) below +0x20 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned char pad_020[0x28];
    unsigned int m_048;  // +0x048 zero-initialised; 4-byte store: mov dword ptr [ebx + 0x48], 0
    unsigned int m_04c;  // +0x04c /* ? */; 4-byte store: mov dword ptr [ebx + 0x4c], ecx
};
