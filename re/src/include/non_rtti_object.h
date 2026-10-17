#pragma once
// ==========================================================================
// non_rtti_object.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class __non_rtti_object
// vtable       : 0x006a7d40  (2 slots)
// constructor  : 0x00682f29
// vtable store : 0x00682f40  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "bad_typeid.h"

class non_rtti_object : public bad_typeid
{
public:
    // ---- vtable 0x006a7d40 (2 slots) ----
    virtual void vfunc_0(int a0);  // [0] 0x00682f4b  // __thiscall, returns ?
    virtual void vfunc_1();  // [1] 0x006878ac  // __thiscall, returns void*

    non_rtti_object();  // 0x00682f29
    virtual ~non_rtti_object();

    // ---- data members (offsets from ctor 0x00682f29) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0xc: the compiled end of the
    // base sub-object region. Evidence: bad_typeid at +0x0 (ends +0xc).
    // 1 recovered offset(s) below +0xc lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
};
