#pragma once
// ==========================================================================
// TClassFactory_class_SideClass.h  —  reconstructed from GAME.EXE (C&C Tiberian Sun, 1999)
//
// RTTI class   : class TClassFactory<class SideClass>
// vtable       : 0x006a4988  (5 slots)
// constructor  : 0x005e9730
// vtable store : 0x005e973b  (inside the ctor)
//
// Virtual slots are in real vtable order; each carries its target
// address as evidence. Slots named vfunc_N have no established
// semantics yet.
// ==========================================================================

#include "ts_prelude.h"
#include "SideClass.h"

class TClassFactory_class_SideClass : public IClassFactory
{
public:
    // ---- vtable 0x006a4988 (5 slots) ----
    virtual void QueryInterface(int a0, int a1, int a2);  // [0] 0x005e9740  // __stdcall, returns int
    virtual void AddRef(int a0);  // [1] 0x005e97b0  // __stdcall
    virtual void Release(int a0);  // [2] 0x005e97d0  // __stdcall
    virtual void vfunc_3(int a0, int a1, int a2, int a3);  // [3] 0x005e9800  // __stdcall, returns int
    virtual void vfunc_4(int a0, int a1);  // [4] 0x005e9880  // __stdcall, returns int

    TClassFactory_class_SideClass();  // 0x005e9730
    virtual ~TClassFactory_class_SideClass();

    // ---- data members (offsets from ctor 0x005e9730) ----
    // Offsets are exact. Each member is declared at the access width
    // the constructor stores it with; the store instruction is quoted
    // as evidence. `/* width ? */` means no store could be classified.
    // vtable-pointer stores are omitted: the compiler supplies the vptr.
    // The machine block begins at +0x4: the compiled end of the
    // base sub-object region. Evidence: IClassFactory at +0x0 (ends +0x4).
    // 1 recovered offset(s) below +0x4 lie inside the base
    // sub-object region and are NOT declared here: they are declared and
    // compiler-checked in the base's own header -- +0x0
    unsigned int m_004;  // +0x004 zero-initialised; 4-byte store: mov dword ptr [eax + 4], 0
};
